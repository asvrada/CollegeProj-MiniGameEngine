#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H

#include "ProjectHeader.h"

class Camera;
class Object;

class Render
{
private:
	//当前程序的句柄
	HWND *m_ptr_hwnd;
	//屏幕的设备上下文
	HDC m_hdc_screen;

	//渲染缓冲区
	HDC m_hdc_buffer;
	//储存背景图案的画刷
	HBRUSH m_brush_background;

	//摄像机类
	Camera *m_ptr_camera;

//私有函数
private:
	float *m_z_depth_buffer;
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
	// 学习用 //
	////////////
	void Render::FillTriangleTopFlat_Affline(Vector4 &a, Vector2<float>& uv_a, Vector4 &b, Vector2<float> &uv_b, Vector4& c, Vector2<float>& uv_c, HDC *texture);
	void Render::FillTriangleBottomFlat_Affline(Vector4 &a, Vector2<float>& uv_a, Vector4 &b, Vector2<float> &uv_b, Vector4& c, Vector2<float>& uv_c, HDC *texture);


	//填充三角形
	void FillTriangles(vector<Fragment> &);
	void FillTriangleTopFlat(Vector4 &a, Vector2<float> &uv_a, Vector4 &b, Vector2<float> &uv_b, Vector4 &c, Vector2<float> &uv_c,HDC*);
	void FillTriangleBottomFlat(Vector4 &a, Vector2<float> &uv_a, Vector4 &, Vector2<float> &uv_b, Vector4 &c, Vector2<float> &uv_c,HDC*);
	//画三角形边框
	void DrawTriangles(const vector<Fragment> &);
	void DrawTriangle(const Vector4 p0, const Vector4 p1, const Vector4 p2, COLORREF);
	//画线
	void DrawLine(Vector2<float>, Vector2<float>, COLORREF);
	//设置单个像素的颜色
	void DrawPixel(int, int, COLORREF);

//模型数据
public:
	vector<Object> vector_objects;
};

#endif