#ifndef RASTERIZER_WINDOWFRAMECLASS_H
#define RASTERIZER_WINDOWFRAMECLASS_H

#define WIN32_LEAN_AND_MEAN
/*
类的功能：
创建具有窗口界面的程序
*/

#include "ProjectHeader.h"
#include "InputClass.h"
#include "RenderClass.h"

class WindowFrameClass {
private:
	//程序句柄
	HWND m_hWnd;

	//渲染区域（客户坐标系）
	//Rendering area(in client coordinate)
	RECT m_rectRenderScreen;

	//鼠标居中显示
	bool m_centerCursor;
	//退出程序
	bool m_quitSoftware;
	//窗口名称
	wchar_t *m_AppName;

//用到的类
private:
	//渲染类
	RenderClass *m_MainRenderer;

	//记录当前按下的按键
	InputClass *m_Input;

public:
	WindowFrameClass();
	~WindowFrameClass();

	//真正的消息处理函数
	LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//Initialize parameters
	void Initialize(int,int);
	void Shutdown();

	//在系统中注册窗口
	int RegisterCreateWindow(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd);

	//主要的程序循环
	int Run();
};

//骗Win的消息处理函数
LRESULT CALLBACK CustomWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static WindowFrameClass *AppHandler = NULL;

#endif