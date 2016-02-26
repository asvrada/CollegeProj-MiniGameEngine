#include "WindowFrameClass.h"

WindowFrameClass::WindowFrameClass() {
	m_rect_client.bottom = m_rect_client.left = m_rect_client.right = m_rect_client.top = 0;
	m_quit_software = false;
	m_app_name = TEXT("Reasterizer");

	AppHandler = this;
	m_ptr_input = NULL;
	m_ptr_renderer = NULL;
}

WindowFrameClass::~WindowFrameClass() {
	Shutdown();
}

void WindowFrameClass::Initialize(int RENDER_X, int RENDER_Y) {
	//设定初始渲染区域的大小
	m_rect_client.top = 0; m_rect_client.left = 0; m_rect_client.bottom = RENDER_Y; m_rect_client.right = RENDER_X;
	m_quit_software = false;

	m_ptr_input = new InputClass();
	m_ptr_time = new TimeClass();
	m_ptr_renderer = new RenderClass(m_ptr_input, m_ptr_time);
}

void WindowFrameClass::Shutdown() {
	if (m_ptr_renderer != nullptr) {
		m_ptr_renderer->Shutdown();
		delete m_ptr_renderer;
		m_ptr_renderer = nullptr;
	}

	if(m_ptr_time != nullptr) {
		delete m_ptr_time;
		m_ptr_time = nullptr;
	}

	if (m_ptr_input != nullptr) {
		delete m_ptr_input;
		m_ptr_input = nullptr;
	}
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
	wnd.lpszClassName = m_app_name;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_VREDRAW | CS_HREDRAW;

	//注册窗口
	RegisterClassEx(&wnd);
	//居中显示窗口
	m_hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, m_app_name, m_app_name,
		WS_OVERLAPPEDWINDOW, (m_WINDOW_X - m_rect_client.right + m_rect_client.left) / 2, (m_WINDOW_Y - m_rect_client.bottom + m_rect_client.top) / 2,
		m_rect_client.right , m_rect_client.bottom, NULL, NULL, hInstance, NULL);

		if (!m_hwnd) {
			return ERROR;
		}
		//从系统申请到窗口句柄后
		//用它初始化各个类
		//初始化主渲染类
		m_ptr_renderer->Initialize(&m_rect_client, &m_hwnd);
		//初始化输入类
		m_ptr_input->Initialize(m_hwnd);


		//显示窗口
		ShowWindow(m_hwnd, nShowCmd);
		UpdateWindow(m_hwnd);

		return OK;
}

int WindowFrameClass::Run() {
	MSG msg = { 0 };
	//主要程序循环
	while(!m_quit_software) {
		//无消息时退出以下循环
		while (1) {
			if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_ptr_time->ComputeTime();
		GetCursorPos(&m_ptr_input->point_cursor_current);

		//进行一帧渲染
		m_ptr_renderer->RenderAFrame();

		m_ptr_input->ClearFlag();

	}
	return msg.message;
}

//消息处理函数
LRESULT CALLBACK WindowFrameClass::WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_SIZE:
		//取得新屏幕的大小
		m_rect_client.right = LOWORD(lParam);
		m_rect_client.bottom = HIWORD(lParam);
		m_rect_client.left = m_rect_client.top = 0;

		//并更改画布的大小
		m_ptr_renderer->UpdateSettings();
		//重新设置鼠标的中心
		m_ptr_input->UpdateCursorCenterPostion(m_rect_client);
		break;
	case WM_MOVE:
		m_ptr_input->UpdateCursorCenterPostion(m_rect_client);
		break;

	//以下消息响应按键
	case WM_LBUTTONDOWN:
		m_ptr_input->Press('l');
		break;
	case WM_LBUTTONUP:
		m_ptr_input->Release('l');
		m_ptr_input->is_lbutton_up = true;
		break;
	case WM_RBUTTONDOWN:
		m_ptr_input->Press('r');
		break;
	case WM_RBUTTONUP:
		m_ptr_input->Release('r');
		m_ptr_input->is_rbutton_up = true;
		break;
	case WM_KEYDOWN:
		m_ptr_input->Press((int)wParam);
		m_ptr_input->ReactToKeyPressed();
		break;
	case WM_KEYUP:
		m_ptr_input->Release((int)wParam);
		break;

	//关闭程序
	case WM_CLOSE:
	case WM_DESTROY:
		m_quit_software = true;
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