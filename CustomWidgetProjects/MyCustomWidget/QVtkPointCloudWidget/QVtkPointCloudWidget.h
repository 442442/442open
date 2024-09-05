#pragma once
#include <QtUiPlugin/QDesignerExportWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarWidget.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyData.h>

#include <string_view>
/// <summary>
/// vtk显示窗口
/// </summary>
class QDESIGNER_WIDGET_EXPORT QVtkPointCloudWidget :public QVTKOpenGLNativeWidget
{
	Q_OBJECT

public:
    QVtkPointCloudWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    QVtkPointCloudWidget(vtkGenericOpenGLRenderWindow* window, QWidget* parent = nullptr,
		Qt::WindowFlags f = Qt::WindowFlags());
	/// <summary>
	/// 获取颜色映射条窗口
	/// </summary>
	/// <returns></returns>
	vtkSmartPointer<vtkScalarBarWidget> GetScalarBarWidget();
	/// <summary>
	/// 获取坐标轴窗口
	/// </summary>
	/// <returns></returns>
	vtkSmartPointer<vtkOrientationMarkerWidget> GetMarkerWidget();
	/// <summary>
	/// 添加数据并更新所有数据颜色
	/// </summary>
	/// <param name="data"></param>
	void AddDataAndUpdateAllScalarRange(vtkSmartPointer<vtkPolyData> data);
	void AddDataAndUpdateAllScalarRange(vtkSmartPointer<vtkActor> actor);
	/// <summary>
	/// 设置选点大小
	/// </summary>
	/// <param name="size"></param>
	void SetPickerPointSize(double size);
	/// <summary>
	/// 设置选点值显示文本大小
	/// </summary>
	/// <param name="size"></param>
	void SetPickerFontSize(double size);
	/// <summary>
	/// 设置选点显示文本小数精确度
	/// </summary>
	/// <param name="f"></param>
	void SetPickerDisplayPrecision(int f);
	/// <summary>
	/// 视角
	/// </summary>
	enum ViewPort
	{
		Keep = 0,//保持当前视角
		Left,//左视图
		Right,//右视图
		Front,//正视图
		Back,//后视图
		Up,//俯视图
		Down//仰视图
	};
	/// <summary>
	/// 重置视角
	/// </summary>
	/// <param name="view">视角类型</param>
	void ResetView(ViewPort view = Keep);
	/// <summary>
	/// 读取txt点云
	/// </summary>
	/// <param name="path">路径</param>
	/// <returns></returns>
	static auto ReadTxtPointCloud(std::string_view path) -> vtkSmartPointer<vtkPolyData>;
	/// <summary>
	/// 读取txt点云并按z值上色
	/// </summary>
	/// <param name="path">路径</param>
	/// <returns></returns>
	static auto ReadTxtPointCloudWithScalarZ(std::string_view path) -> vtkSmartPointer<vtkPolyData>;

private:
	void InitActors();
	void ResetView(double lookX, double lookY, double lookZ,
		double upX, double upY, double upZ);
	void ResetCamera();
	void UpdateRange(double* range);

	vtkSmartPointer<vtkOrientationMarkerWidget> mAxiesWidget;
	vtkSmartPointer<vtkScalarBarWidget> mScalarBarWidget;

	std::list<vtkSmartPointer<vtkActor>> mActors;

    double minRange;
    double maxRange;
};

