#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H

/*
该类输出渲染的图像到屏幕
*/

#include <Windows.h>
#include "Declarations.h"


class RenderClass
{
//私有数据
private:
	//屏幕的
	HDC m_hdcScreen;
	//渲染缓冲区
	HDC m_hdcBuffer;
	//渲染区域
	RECT m_rectRenderArea;

	//储存背景图案的位图
	HBITMAP m_bmpBackgroundImage;

//私有函数
//重复的工作放这里
private:
	void m_UpdateSettings();

//公众API
//当前类初始化的有关函数
public:
	RenderClass();
	RenderClass(RECT);
	~RenderClass();

	//初始化
	void Initialize(RECT,HDC);
	//关闭
	void ShutDown();

	//将所有与屏幕大小有关的资源全删了
	void DeleteResources();

	//当窗口大小变化时
	//重新设置画布大小以及相应资源
	void UpdateSettings(RECT rectNew);
	void UpdateSettings(int rectRight, int rectBottom);

	//渲染一帧
	void RenderAFrame();

	//将渲染好的图片交换至屏幕上
	inline void SwapBufferToScreen();

//公有函数
//输出详情
public:
	inline void OutputText(wchar_t[]);
};

#endif