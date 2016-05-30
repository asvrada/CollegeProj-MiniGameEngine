#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H

#include "ProjectHeader.h"

#include "CameraClass.h"
#include "SceneManagerClass.h"

class Render
{
public:
	//场景类的指针（操）
	SceneManager *m_ptr_manager;
//私有数据成员
private:
	//当前程序的句柄
	HWND *m_ptr_hwnd;
	//屏幕的设备上下文
	HDC m_hdc_screen;

	//深度缓冲区
	float *m_z_depth_buffer;
	//渲染缓冲区
	HDC m_hdc_buffer;
	//储存背景图案的画刷
	HBRUSH m_brush_background;

//私有函数
private:
	void m_DrawObjects();
public:
	Render();
	~Render();

	void Initialize(HWND*);
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
	/////////////
	// 三角形填充 //
	////////////
	void FillTriangleTopFlat(Vector4 &a, Vector2<float>& uv_a, Vector4 &b, Vector2<float> &uv_b, Vector4& c, Vector2<float>& uv_c, HDC *texture);
	void FillTriangleBottomFlat(Vector4 &a, Vector2<float>& uv_a, Vector4 &b, Vector2<float> &uv_b, Vector4& c, Vector2<float>& uv_c, HDC *texture);

	//填充三角形
	void FillTriangles(vector<Fragment> &);
	//画三角形边框
	void DrawTriangles(const vector<Fragment> &);
	void DrawTriangle(const Vector4 p0, const Vector4 p1, const Vector4 p2, COLORREF);
	//给出两点及线段颜色，画线
	void DrawLine(Vector2<float>, Vector2<float>, COLORREF);
	//设置单个像素的颜色
	void DrawPixel(int, int, COLORREF);
};

#endif