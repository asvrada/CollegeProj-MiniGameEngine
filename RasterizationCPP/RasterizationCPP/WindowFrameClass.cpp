#include "WindowFrameClass.h"

WindowFrameClass::WindowFrameClass() {
	m_rectRenderScreen.bottom = m_rectRenderScreen.left = m_rectRenderScreen.right = m_rectRenderScreen.top = 0;
	m_centerCursor = false;
	m_quitSoftware = false;
	m_AppName = TEXT("Reasterizer");

	AppHandler = this;
	m_Input = NULL;
	m_MainRenderer = NULL;
}

WindowFrameClass::~WindowFrameClass() {
	m_AppName = NULL;
	delete m_Input;
	m_Input = NULL;

	delete m_MainRenderer;
	m_MainRenderer = NULL;
}

void WindowFrameClass::Initialize(int RENDER_X, int RENDER_Y) {
	//设定初始渲染区域的大小
	m_rectRenderScreen.top = 0; m_rectRenderScreen.left = 0; m_rectRenderScreen.bottom = RENDER_Y; m_rectRenderScreen.right = RENDER_X;
	m_centerCursor = false;
	m_quitSoftware = false;

	m_Input = new InputClass();

	m_MainRenderer = new RenderClass();
}

void WindowFrameClass::Shutdown() {
	m_MainRenderer->Shutdown();
	delete m_MainRenderer;
	m_MainRenderer = NULL;

	delete m_Input;
	m_Input = NULL;
}

int WindowFrameClass::RegisterCreateWindow(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd) {
	//获取桌面分辨率
	//用于居中显示窗口
	int m_WINDOW_X = GetSystemMetrics(SM_CXSCREEN);
	int m_WINDOW_Y = GetSystemMetrics(SM_CYSCREEN);

	//窗口注册相关
	//不必太在意
	WNDCLASSEX wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = CustomWinProc;
	wnd.lpszClassName = m_AppName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_VREDRAW | CS_HREDRAW;

	//注册窗口
	RegisterClassEx(&wnd);
	//居中显示窗口
	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, m_AppName, m_AppName,
		WS_OVERLAPPEDWINDOW, (m_WINDOW_X - m_rectRenderScreen.right + m_rectRenderScreen.left) / 2, (m_WINDOW_Y - m_rectRenderScreen.bottom + m_rectRenderScreen.top) / 2,
		m_rectRenderScreen.right - m_rectRenderScreen.left, m_rectRenderScreen.bottom - m_rectRenderScreen.top, NULL, NULL, hInstance, NULL);

		if (!m_hWnd) {
			return ERROR;
		}
		//从系统申请到窗口句柄后
		//用它初始化各个类
		//初始化主渲染类
		m_MainRenderer->Initialize(&m_rectRenderScreen, &m_hWnd);
		//初始化输入类
		m_Input->Initialize(m_hWnd);


		//显示窗口
		ShowWindow(m_hWnd, nShowCmd);
		UpdateWindow(m_hWnd);

		return OK;
}

int WindowFrameClass::Run() {
	MSG msg = { 0 };
	//主要程序循环
	while(!m_quitSoftware) {
		//无消息时退出以下循环
		while (1) {
			if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//进行一帧渲染
		m_MainRenderer->RenderAFrame();
	}
	return msg.message;
}

//消息处理函数
LRESULT CALLBACK WindowFrameClass::WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_SIZE:
		//取得新屏幕的大小
		m_rectRenderScreen.right = LOWORD(lParam);
		m_rectRenderScreen.bottom = HIWORD(lParam);
		m_rectRenderScreen.left = m_rectRenderScreen.top = 0;

		//并更改画布的大小
		m_MainRenderer->UpdateSettings();
		break;

	case WM_KEYDOWN:
		m_Input->Press((int)wParam);
		m_Input->ReactToKeyPressed();
		break;
	case WM_KEYUP:
		m_Input->Release((int)wParam);
		break;

	//关闭程序
	case WM_CLOSE:
	case WM_DESTROY:
		m_quitSoftware = true;
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

//无视
LRESULT CALLBACK CustomWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	return AppHandler->WinProc(hWnd, Msg, wParam, lParam);
}