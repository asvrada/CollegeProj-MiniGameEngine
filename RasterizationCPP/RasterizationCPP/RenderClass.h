#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H
/*
渲染类
进行数学计算并将生成的图案输出至Canvas

包含类:
画布类
摄像机类

时间管理类
计算每次渲染过去多少时间
*/

////////////
// 包含类 //
////////////
#include "ProjectHeader.h"
#include "CameraClass.h"
#include "ObjectClass.h"
#include "InputClass.h"
#include "TimeClass.h"

class RenderClass
{
private:
	//当前程序的句柄
	HWND *m_ptr_hwnd;
	//屏幕的设备上下文
	HDC m_hdc_screen;
	//渲染区域
	RECT *m_ptr_rect_client;

	//渲染缓冲区
	HDC m_hdc_buffer;
	//储存背景图案的画刷
	HBRUSH m_brush_background;


	//输入类的指针
	InputClass *m_ptr_input;
	//时间类
	TimeClass m_time;
	//摄像机类
	CameraClass *m_camera;

//私有函数
private:
	//ViewToHomo 矩阵在摄像机类里
	Matrix4 m_world_to_view;
	void m_DrawObjects();
public:
	RenderClass(InputClass*);
	~RenderClass();

	void Initialize(RECT*,HWND*);
	void DeleteResources();
	void Shutdown();

	//渲染一帧
	void RenderAFrame();

	//当窗口大小变化时
	//重新设置画布大小以及相应资源
	void UpdateSettings();

	//////////////////////////////
	// Canvas Draw Functions //
	/////////////////////////////

	//前后台缓存交换
	void SwapBufferToScreen();

	//清空画布
	void ClearCanvas();

	//输出文字
	void OutputText(const wstring&, int linenumber);
	//画单个三角形
	void DrawTriangle(const Vector4 p0, const Vector4 p1, const Vector4 p2, COLORREF);
	//画线
	void DrawLine(Vector2, Vector2, COLORREF);
	//设置单个像素的颜色
	void DrawPixel(int, int, COLORREF);

//模型数据
public:
	vector<Object> vector_objects;
};

#endif