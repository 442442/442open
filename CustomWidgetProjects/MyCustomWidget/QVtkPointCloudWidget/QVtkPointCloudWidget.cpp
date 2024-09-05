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

QVtkPointCloudWidget::QVtkPointCloudWidget(QWidget* parent, Qt::WindowFlags f)
	:QVTKOpenGLNativeWidget{ parent,f }
    ,minRange{ std::numeric_limits<double>::max() }
    ,maxRange{ std::numeric_limits<double>::min() }
{
	InitActors();
}

QVtkPointCloudWidget::QVtkPointCloudWidget(vtkGenericOpenGLRenderWindow* window, QWidget* parent, Qt::WindowFlags f)
	:QVTKOpenGLNativeWidget{ window,parent,f }
    ,minRange{ std::numeric_limits<double>::max() }
    ,maxRange{ std::numeric_limits<double>::min() }
{
	InitActors();
}

vtkSmartPointer<vtkScalarBarWidget> QVtkPointCloudWidget::GetScalarBarWidget()
{
	return mScalarBarWidget;
}

vtkSmartPointer<vtkOrientationMarkerWidget> QVtkPointCloudWidget::GetMarkerWidget()
{
	return mAxiesWidget;
}

void QVtkPointCloudWidget::AddDataAndUpdateAllScalarRange(vtkSmartPointer<vtkPolyData> data)
{
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(data);
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	AddDataAndUpdateAllScalarRange(actor);
}

void QVtkPointCloudWidget::AddDataAndUpdateAllScalarRange(vtkSmartPointer<vtkActor> actor)
{
	this->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
	auto range = actor->GetMapper()->GetInputAsDataSet()->GetScalarRange();
	actor->GetMapper()->SetLookupTable(this->GetScalarBarWidget()->GetScalarBarActor()->GetLookupTable());
	UpdateRange(range);
	mActors.push_back(actor);
	for (auto& a : mActors)
	{
		a->GetMapper()->SetScalarRange(minRange, maxRange);
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

auto QVtkPointCloudWidget::ReadTxtPointCloud(std::string_view path) -> vtkSmartPointer<vtkPolyData>
{
	auto polyData = vtkSmartPointer<vtkPolyData>::New();

	FILE* fp = fopen(path.data(), "r");
	if (!fp)
	{
		std::cout << "file not exist" << std::endl;
		return polyData;
	}
	vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）
	double x = 0, y = 0, z = 0;
	int i = 0;
	while (!feof(fp))
	{
		auto ret = fscanf(fp, "%lf %lf %lf", &x, &y, &z);
		m_Points->InsertPoint(i, x, y, z);		//_加入点信息
		vertices->InsertNextCell(1);		//_加入细胞顶点信息----用于渲染点集
		vertices->InsertCellPoint(i);
		i++;
	}
	fclose(fp);

	polyData->SetPoints(m_Points);		//_设置点集
	polyData->SetVerts(vertices);		//_设置渲染顶点

	return polyData;
}

auto QVtkPointCloudWidget::ReadTxtPointCloudWithScalarZ(std::string_view path) -> vtkSmartPointer<vtkPolyData>
{
	auto polyData = vtkSmartPointer<vtkPolyData>::New();
	auto scalarArray = vtkSmartPointer<vtkDoubleArray>::New();

	FILE* fp = fopen(path.data(), "r");
	if (!fp)
	{
		std::cout << "file not exist" << std::endl;
		return polyData;
	}
	vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）
	double x = 0, y = 0, z = 0;
	int i = 0;
	while (!feof(fp))
	{
		auto ret = fscanf(fp, "%lf %lf %lf", &x, &y, &z);
		m_Points->InsertPoint(i, x, y, z);		//_加入点信息
		vertices->InsertNextCell(1);		//_加入细胞顶点信息----用于渲染点集
		vertices->InsertCellPoint(i);
		scalarArray->InsertNextTuple1(z);
		i++;
	}
	fclose(fp);

	polyData->SetPoints(m_Points);		//_设置点集
	polyData->SetVerts(vertices);		//_设置渲染顶点

	polyData->GetPointData()->SetScalars(scalarArray);

	return polyData;
}

void QVtkPointCloudWidget::InitActors()
{
	auto renderer = vtkSmartPointer<vtkRenderer>::New();//在构造函数中进行初始化
	//显示
	vtkNew<vtkGenericOpenGLRenderWindow> renwindow;
	renwindow->AddRenderer(renderer);
	this->setRenderWindow(renwindow.Get());
	// 显示坐标系的vtk组件
	auto axes_actor = vtkSmartPointer<vtkAxesActor>::New();
	axes_actor->SetPosition(0, 0, 0);
	axes_actor->SetTotalLength(2, 2, 2);
	axes_actor->SetShaftType(0);
	axes_actor->SetCylinderRadius(0.03);
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
}

void QVtkPointCloudWidget::ResetView(double lookX, double lookY, double lookZ, double upX, double upY, double upZ)
{
	auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(lookX, lookY, lookZ);    // 相机位置
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);			  // 焦点位置
	renderer->GetActiveCamera()->SetViewUp(upX, upY, upZ);			  // 相机朝上方向
	renderer->ResetCamera();
	renderer->Render();
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
	minRange = range[0] < minRange ? range[0] : minRange;
	maxRange = range[1] > maxRange ? range[1] : maxRange;
}
