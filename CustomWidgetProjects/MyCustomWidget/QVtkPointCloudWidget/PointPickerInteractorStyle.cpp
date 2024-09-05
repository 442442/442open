#include "PointPickerInteractorStyle.h"

#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkVectorText.h>
#include <vtkFollower.h>
#include <vtkPointPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

#include <format>
#include <random>

vtkStandardNewMacro(PointPickerInteractorStyle);

void PointPickerInteractorStyle::OnLeftButtonDoubleClick()
{
	//随机tgb颜色
	double r = 0, g = 0, b = 0;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> uniform_dist(0.1, 1.0);
	r = uniform_dist(gen);
	g = uniform_dist(gen);
	b = uniform_dist(gen);

	auto renderer = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	//注册拾取点函数
	this->Interactor->GetPicker()->Pick(
		this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1], 0,  // always zero.
		renderer
	);
	//拾取点空间位置
	double picked[3];
	this->Interactor->GetPicker()->GetPickPosition(picked);
	//对拾取点进行标记
	auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(mPointSize);
	sphereSource->Update();
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());
	//加点
	mActors.emplace_back(vtkSmartPointer<vtkActor>::New());
	decltype(auto) actor = mActors.back();
	actor->SetMapper(mapper);
	actor->SetPosition(picked);
	actor->SetScale(mPointSize);
	actor->GetProperty()->SetColor(r, g, b);
	renderer->AddActor(actor);
	//写字
	auto textSource = vtkSmartPointer<vtkVectorText>::New();
	textSource->SetText(std::format("({1:.{0}f},{2:.{0}f},{3:.{0}f})", mPrecision, picked[0], picked[1], picked[2]).c_str());
	auto textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	textMapper->SetInputConnection(textSource->GetOutputPort());
	auto textFollower = vtkSmartPointer<vtkFollower>::New();
	textFollower->SetMapper(textMapper);
	textFollower->SetScale(mFontSize);
	textFollower->SetPosition(picked);
	textFollower->GetProperty()->SetColor(r, g, b);
	textFollower->SetCamera(renderer->GetActiveCamera());
	renderer->AddActor(textFollower);
	mActors.push_back(textFollower);
	//渲染
	this->Interactor->GetRenderWindow()->Render();
}

void PointPickerInteractorStyle::OnRightButtonDown()
{
	for (auto& actor : mActors)
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(actor);
	this->Interactor->GetRenderWindow()->Render();
}

void PointPickerInteractorStyle::SetPointSize(double size)
{
	mPointSize = size;
}

double PointPickerInteractorStyle::GetPointSize() const
{
	return mPointSize;
}

void PointPickerInteractorStyle::SetFontSize(double size)
{
	mFontSize = size;
}

double PointPickerInteractorStyle::GetFontSize() const
{
	return mFontSize;
}

void PointPickerInteractorStyle::SetDisplayPrecision(int f)
{
	mPrecision = f;
}

int PointPickerInteractorStyle::GetDisplayPrecision() const
{
	return mPrecision;
}
