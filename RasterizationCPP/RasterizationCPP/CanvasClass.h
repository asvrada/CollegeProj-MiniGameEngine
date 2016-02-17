#ifndef RASTERIZER_CANVASCLASS_H
#define RASTERIZER_CANVASCLASS_H

/*
该类输出渲染的图像到屏幕
*/

#include "ProjectHeader.h"

class CanvasClass
{
//私有数据
private:
	//屏幕的句柄
	HDC m_hdcScreen;

	//渲染缓冲区
	HDC m_hdcBuffer;
	//储存背景图案的画刷
	HBRUSH m_brushBackground;

	//渲染区域
	RECT m_rectRenderArea;

//公众API
//当前类初始化的有关函数
public:
	CanvasClass();
	~CanvasClass();

	//初始化
	void Initialize(RECT,HDC);
	//关闭
	void Shutdown();
	//将所有与屏幕大小有关的资源全删了
	void DeleteResources();

	//当窗口大小变化时
	//重新设置画布大小以及相应资源
	void UpdateSettings(RECT rectNew);

	//将渲染好的图片交换至屏幕上
	void SwapBufferToScreen();

	//清空画布
	void ClearCanvas();

//公有函数
//具体在画布上进行显示的函数
public:
	//输出一行字符串
	void OutputText(wstring,int line);
	//设置单个像素的颜色
	void Draw(int, int, COLORREF);
};

#endif