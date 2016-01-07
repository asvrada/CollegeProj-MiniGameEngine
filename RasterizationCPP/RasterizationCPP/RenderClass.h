#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H

/*
该类输出渲染的图像到屏幕
*/

#include <Windows.h>
#include "Declarations.h"

class RenderClass
{
private:
	//屏幕的
	HDC m_hdcScreen;
	//渲染缓冲区
	HDC m_hdcBuffer;
	//渲染区域
	RECT m_rectRenderArea;

	//储存背景图案的位图
	HBITMAP m_bmpBackgroundImage;
	//储存背景图案的画刷
	HBRUSH m_brushBackgroundImage;
public:
	RenderClass();
	RenderClass(RECT);
	~RenderClass();

	//初始化
	void Initialize(RECT,HDC);

	void ShutDown();

	//将所有与屏幕大小有关的资源全删了
	void DeleteResources();

	//当窗口大小变化时
	//重新设置画布大小之类
	void UpdateSettings(RECT rectNew);
	void UpdateSettings(int rectRight, int rectBottom);

	//渲染出图像
	void RenderAFrame();

	//将渲染好的图片交换至屏幕上
	inline void SwapBufferToScreen();

//重复的工作放这里
private:
	void m_UpdateSettings();
};

#endif