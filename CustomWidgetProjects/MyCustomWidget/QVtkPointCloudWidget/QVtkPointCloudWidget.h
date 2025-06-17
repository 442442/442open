#pragma once
#include <QtUiPlugin/QDesignerExportWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarWidget.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyData.h>

class QDESIGNER_WIDGET_EXPORT QVtkPointCloudWidget :public QVTKOpenGLNativeWidget
{
	Q_OBJECT

public:
    QVtkPointCloudWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    QVtkPointCloudWidget(vtkGenericOpenGLRenderWindow* window, QWidget* parent = nullptr,
		Qt::WindowFlags f = Qt::WindowFlags());
    /**
     * @brief The ViewPort enum 视角类型
     */
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
    /**
     * @brief AddCloud 添加点云数据
     * @param data 数据
     */
    void AddCloud(vtkSmartPointer<vtkPolyData> data);
    void AddCloud(vtkSmartPointer<vtkActor> actor);
    /**
     * @brief GetCloudCount 获取点云数量
     * @return 数量
     */
    int GetCloudCount() const;
    /**
     * @brief GetCloudIndexList 获取点云序号列表
     * @return 序号列表
     */
    std::list<int> GetCloudIndexList() const;
    /**
     * @brief SetCloudVisibility 设置点云可见
     * @param index 序号
     * @param visible 是否可见
     */
    void SetCloudVisibility(int index, bool visible);
    /**
     * @brief RemoveCloud 移除点云
     * @param index 序号
     */
    void RemoveCloud(int index);
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
    /**
     * @brief SetActor2DVisibility 设置2D元件可见
     * @param visible 是否可见
     */
    void SetActor2DVisibility(bool visible);
    /**
     * @brief ResetView 重置视角
     * @param view 视角类型
     */
	void ResetView(ViewPort view = Keep);
	/// <summary>
	/// 读取txt点云
	/// </summary>
	/// <param name="path">路径</param>
    /// <param name="c">颜色</param>
	/// <returns></returns>
    auto ReadTxtPointCloud(const QString& path) -> vtkSmartPointer<vtkPolyData>;
    /**
     * @brief MeshCloud 三角网格化--数据量大的时候会崩溃,算法递归太深,堆栈溢出
     * @param data 数据
     * @param alpha mesh距离
     */
    void MeshCloud(vtkSmartPointer<vtkPolyData> data, double alpha);

private:
    void Init();
	void ResetView(double lookX, double lookY, double lookZ,
		double upX, double upY, double upZ);
	void ResetCamera();
	void UpdateRange(double* range);

	vtkSmartPointer<vtkOrientationMarkerWidget> mAxiesWidget;
	vtkSmartPointer<vtkScalarBarWidget> mScalarBarWidget;

    std::unordered_map<int, vtkSmartPointer<vtkActor>> mActors;

    double minRange;
    double maxRange;
};

