#include "QVtkPointCloudWidget.h"
#include "PointPickerInteractorStyle.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkAxesActor.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointPicker.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkOutputWindow.h>
#include <vtkCallbackCommand.h>
#include <vtkActor2DCollection.h>
#include <vtkDelaunay2D.h>

#include <QFile>
#include <ranges>

std::atomic_bool gDisplayRenderEndFlag{ true };
std::atomic_bool gRenderEndFlag{ true };
int gCloudIndex{ 0 };

void RendEndCallBack(vtkObject* obj, unsigned long eid, void* clientdata, void* calldata)
{
    Q_UNUSED(obj);
    Q_UNUSED(eid);
    Q_UNUSED(clientdata);
    Q_UNUSED(calldata);
    //std::cout << "call back...." << std::endl;

    if (gDisplayRenderEndFlag.load() == false)
        gDisplayRenderEndFlag.store(true);
    if (gRenderEndFlag.load() == false)
    {
        gRenderEndFlag.store(true);
        //g_render_end = std::chrono::steady_clock::now();
    }
}

QVtkPointCloudWidget::QVtkPointCloudWidget(QWidget* parent, Qt::WindowFlags f)
	:QVTKOpenGLNativeWidget{ parent,f }
    ,minRange{ std::numeric_limits<double>::max() }
    ,maxRange{ std::numeric_limits<double>::min() }
{
    Init();
}

QVtkPointCloudWidget::QVtkPointCloudWidget(vtkGenericOpenGLRenderWindow* window, QWidget* parent, Qt::WindowFlags f)
	:QVTKOpenGLNativeWidget{ window,parent,f }
    ,minRange{ std::numeric_limits<double>::max() }
    ,maxRange{ std::numeric_limits<double>::min() }
{
    Init();
}

void QVtkPointCloudWidget::AddCloud(vtkSmartPointer<vtkPolyData> data)
{
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(data);
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
    AddCloud(actor);
}

void QVtkPointCloudWidget::AddCloud(vtkSmartPointer<vtkActor> actor)
{
	this->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
	auto range = actor->GetMapper()->GetInputAsDataSet()->GetScalarRange();
    actor->GetMapper()->SetLookupTable(mScalarBarWidget->GetScalarBarActor()->GetLookupTable());
	UpdateRange(range);
    mActors[gCloudIndex] = actor;
    for (auto& [i, a] : mActors)
	{
		a->GetMapper()->SetScalarRange(minRange, maxRange);
	}
}

int QVtkPointCloudWidget::GetCloudCount() const
{
    return mActors.size();
}

std::list<int> QVtkPointCloudWidget::GetCloudIndexList() const
{
    std::list<int> list;
    std::ranges::for_each(mActors | std::views::keys, [&list](int key){list.push_back(key); });
    return list;
}

void QVtkPointCloudWidget::SetCloudVisibility(int index, bool visible)
{
    mActors[index]->SetVisibility(visible);
}

void QVtkPointCloudWidget::RemoveCloud(int index)
{
    if(auto it = mActors.find(index); it != mActors.end())
    {
        mActors.erase(it);
    }
}

void QVtkPointCloudWidget::SetPickerPointSize(double size)
{
	auto picker = static_cast<PointPickerInteractorStyle*>(this->renderWindow()->GetInteractor()->GetInteractorStyle());
	picker->SetPointSize(size);
}

void QVtkPointCloudWidget::SetPickerFontSize(double size)
{
	auto picker = static_cast<PointPickerInteractorStyle*>(this->renderWindow()->GetInteractor()->GetInteractorStyle());
	picker->SetFontSize(size);
}

void QVtkPointCloudWidget::SetPickerDisplayPrecision(int f)
{
	auto picker = static_cast<PointPickerInteractorStyle*>(this->renderWindow()->GetInteractor()->GetInteractorStyle());
	picker->SetFontSize(f);
}

void QVtkPointCloudWidget::SetActor2DVisibility(bool visible)
{
    auto collect = this->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors2D();
    for (int i = 0; i < collect->GetNumberOfItems(); ++i) {
        auto actor = static_cast<vtkActor2D*>(collect->GetItemAsObject(i));
        if(actor)
            actor->SetVisibility(visible);
    }
}

void QVtkPointCloudWidget::ResetView(ViewPort view)
{
	switch (view)
	{
    case QVtkPointCloudWidget::Keep:
		ResetCamera();
		break;
    case QVtkPointCloudWidget::Left:
		ResetView(-1, 0, 0, 0, 1, 0);
		break;
    case QVtkPointCloudWidget::Right:
		ResetView(1, 0, 0, 0, 1, 0);
		break;
    case QVtkPointCloudWidget::Front:
		ResetView(0, 0, 1, 0, 1, 0);
		break;
    case QVtkPointCloudWidget::Back:
		ResetView(0, 0, -1, 0, 1, 0);
		break;
    case QVtkPointCloudWidget::Up:
		ResetView(0, 1, 0, 0, 0, -1);
		break;
    case QVtkPointCloudWidget::Down:
		ResetView(0, -1, 0, 0, 0, 1);
		break;
	default:
		break;
	}
}

auto QVtkPointCloudWidget::ReadTxtPointCloud(const QString& path) -> vtkSmartPointer<vtkPolyData>
{
	auto polyData = vtkSmartPointer<vtkPolyData>::New();

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "file not exist" << std::endl;
        return polyData;
    }

    auto scalarColorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();

	vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）

    for (int i = 0; !file.atEnd(); i++)
	{
        auto line = file.readLine().split(' ');
        if(i % 50 != 0) continue;
        qDebug()<<i;
        switch (line.size()) {
        case 6:
        {
            vtkIdType pId = m_Points->InsertNextPoint(line.at(0).toDouble(), line.at(1).toDouble(), line.at(2).toDouble());		//_加入点信息
            vertices->InsertNextCell(1, &pId);		//_加入细胞顶点信息----用于渲染点集
            unsigned char color_array[3]={ (unsigned char)line.at(3).toInt(),(unsigned char)line.at(4).toInt(),(unsigned char)line.at(5).toInt()};
            scalarColorArray->InsertNextTypedTuple(color_array);
            break;
        }
        case 4:
        {
            vtkIdType pId = m_Points->InsertNextPoint(line.at(0).toDouble(), line.at(1).toDouble(), line.at(2).toDouble());		//_加入点信息
            vertices->InsertNextCell(1, &pId);		//_加入细胞顶点信息----用于渲染点集
            unsigned char color_array[1] = {(unsigned char)line.at(3).toInt()};
            scalarColorArray->InsertNextTypedTuple(color_array);
            break;
        }
        case 3:
        {
            vtkIdType pId = m_Points->InsertNextPoint(line.at(0).toDouble(), line.at(1).toDouble(), line.at(2).toDouble());		//_加入点信息
            vertices->InsertNextCell(1, &pId);		//_加入细胞顶点信息----用于渲染点集
            break;
        }
        default:
            break;
        }
	}
    file.close();

	polyData->SetPoints(m_Points);		//_设置点集
	polyData->SetVerts(vertices);		//_设置渲染顶点
    polyData->GetPointData()->SetScalars(scalarColorArray);

	return polyData;
}

void QVtkPointCloudWidget::MeshCloud(vtkSmartPointer<vtkPolyData> data, double alpha)
{
    vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
    delaunay->SetInputData(data);
    delaunay->SetAlpha(alpha);
    delaunay->Update();
    auto meshmapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    meshmapper->SetInputConnection(delaunay->GetOutputPort());
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(meshmapper);
    //actor->GetProperty()->EdgeVisibilityOn();
    AddCloud(actor);
}

void QVtkPointCloudWidget::Init()
{
    vtkOutputWindow::SetGlobalWarningDisplay(0);   //禁止vtk警告弹窗

	auto renderer = vtkSmartPointer<vtkRenderer>::New();//在构造函数中进行初始化
	//显示
	vtkNew<vtkGenericOpenGLRenderWindow> renwindow;
	renwindow->AddRenderer(renderer);
	this->setRenderWindow(renwindow.Get());
	// 显示坐标系的vtk组件
	auto axes_actor = vtkSmartPointer<vtkAxesActor>::New();
    axes_actor->SetPosition(100, 0, 0);
    axes_actor->SetTotalLength(200, 200, 200);
	axes_actor->SetShaftType(0);
    axes_actor->SetCylinderRadius(0.05);
	axes_actor->SetAxisLabels(1);
	axes_actor->SetTipType(0);
	// 控制坐标系，使之随视角共同变化
	mAxiesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	mAxiesWidget->SetOrientationMarker(axes_actor);
	mAxiesWidget->SetInteractor(this->renderWindow()->GetInteractor());
	mAxiesWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	mAxiesWidget->SetEnabled(1);
	mAxiesWidget->InteractiveOff();
	//点选
	auto pointPicker = vtkSmartPointer<vtkPointPicker>::New();
	this->renderWindow()->GetInteractor()->SetPicker(pointPicker);
	auto style = vtkSmartPointer<PointPickerInteractorStyle>::New();
	style->SetPointSize(0.05);
	style->SetFontSize(1);
	this->renderWindow()->GetInteractor()->SetInteractorStyle(style);
	//颜色映射条
	auto lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->Build();
	auto scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBarActor->SetOrientationToHorizontal();
	scalarBarActor->SetLookupTable(lut);
	scalarBarActor->SetNumberOfLabels(10);
	scalarBarActor->SetMaximumWidthInPixels(90);
	mScalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();
	mScalarBarWidget->SetInteractor(this->renderWindow()->GetInteractor());
	mScalarBarWidget->SetScalarBarActor(scalarBarActor);
	mScalarBarWidget->RepositionableOff();
	mScalarBarWidget->On();

    auto renderEndCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    renderEndCallback->SetCallback(RendEndCallBack); //很重要！！！
    renwindow->AddObserver(vtkCommand::RenderEvent, renderEndCallback);
}

void QVtkPointCloudWidget::ResetView(double lookX, double lookY, double lookZ, double upX, double upY, double upZ)
{
	auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(lookX, lookY, lookZ);    // 相机位置
	renderer->GetActiveCamera()->SetViewUp(upX, upY, upZ);			  // 相机朝上方向
    ResetCamera();
}

void QVtkPointCloudWidget::ResetCamera()
{
	auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	renderer->Render();
}

void QVtkPointCloudWidget::UpdateRange(double* range)
{
    maxRange = range[1] > maxRange ? range[1] : maxRange;
    minRange = range[0] < minRange ? range[0] : minRange;
}
