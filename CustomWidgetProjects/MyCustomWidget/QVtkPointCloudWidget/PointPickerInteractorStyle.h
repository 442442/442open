#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
/// <summary>
/// 互动点选
/// </summary>
class PointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static PointPickerInteractorStyle* New();
	vtkTypeMacro(PointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);
	/// <summary>
	/// 左键双击显示坐标
	/// </summary>
	virtual void OnLeftButtonDoubleClick() override;
	/// <summary>
	/// 右键点击清空坐标显示
	/// </summary>
	virtual void OnRightButtonDown() override;
	/// <summary>
	/// 点大小
	/// </summary>
	/// <param name="size"></param>
	void SetPointSize(double size);
	double GetPointSize()const;
	/// <summary>
	/// 字体大小
	/// </summary>
	/// <param name="size"></param>
	void SetFontSize(double size);
	double GetFontSize()const;
	/// <summary>
	/// 显示保留小数精度
	/// </summary>
	/// <param name="f">精确度</param>
	void SetDisplayPrecision(int f);
	int GetDisplayPrecision()const;

private:
	std::list<vtkSmartPointer<vtkActor>> mActors;//点和文字actor
	int mPrecision{ 3 };//显示精确度
	double mPointSize{ 1.0 };//点大小
	double mFontSize{ 1.0 };//字体大小
};
