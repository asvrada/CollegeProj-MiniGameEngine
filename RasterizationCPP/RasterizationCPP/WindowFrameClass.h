#ifndef RASTERIZER_WINDOWFRAMECLASS_H
#define RASTERIZER_WINDOWFRAMECLASS_H

#define WIN32_LEAN_AND_MEAN
/*
类的功能：
创建具有窗口界面的程序
*/

#include "ProjectHeader.h"

#include "RenderClass.h"
#include "SceneManagerClass.h"
#include "InputClass.h"
#include "TimeClass.h"

class WindowFrame {
public:
	//程序句柄
	HWND m_hwnd;
	//渲染区域（客户坐标系）
	//Rendering area(in client coordinate)
	static RECT rect_client;

	///////////
	// FLAG //
	///////////
	//32 bit
	//16 ^ 8 == 2^32 == long
	//8 number each
	//0x00000000L
	static DWORD STYLE_CHECKER;
private:

	//退出程序
	bool m_quit_software;
	//窗口名称
	wchar_t *m_app_name;

	//包含的类
	//渲染类
	Render *m_ptr_renderer;
	//场景管理类
	SceneManager *m_ptr_manager;
	//时间类
	Time *m_ptr_time;
	//记录当前按下的按键
	Input *m_ptr_input;

public:
	WindowFrame(DWORD checker = RENDER_MODE_FILL);
	~WindowFrame();

	//真正的消息处理函数
	LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//Initialize parameters
	void Initialize(int, int);
	void Shutdown();

	//在系统中注册窗口
	int RegisterCreateWindow(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd);

	//主要的程序循环
	int Run();
};

//骗Win的消息处理函数
LRESULT CALLBACK CustomWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static WindowFrame *AppHandler = NULL;

#endif