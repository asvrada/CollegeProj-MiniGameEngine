#include "InputClass.h"

//static 初始化
KeyPressedList Input::m_key_pressed = KeyPressedList();
POINT Input::point_cursor_default = POINT();
POINT Input::point_cursor_current = POINT();
bool Input::is_center_snapped = false;
bool Input::is_rbutton_up = false;
bool Input::is_lbutton_up = false;

Input::Input() {
	m_hwnd = 0;
}


Input::~Input() {
	m_hwnd = 0;
}

void Input::Initialize(HWND hWnd) {
	m_hwnd = hWnd;
}

void Input::Press(int input) {
	m_key_pressed.key[input] = true;
}

void Input::Press(char mouseButton) {
	switch (mouseButton) {
	case 'r':
	case 'R':
		m_key_pressed.right_mouse_button = true;
		break;
	case 'l':
	case 'L':
		m_key_pressed.left_mouse_button = true;
		break;
	default:
		break;
	}
}

void Input::Release(int input) {
	m_key_pressed.key[input] = false;
}

void Input::Release(char mouseButton)
{
	switch (mouseButton) {
	case 'r':
	case 'R':
		m_key_pressed.right_mouse_button = false;
	case 'l':
	case 'L':
		m_key_pressed.left_mouse_button = false;
	default:
		Release((int)mouseButton);
	}
}

void Input::UpdateCursorCenterPostion(const RECT &rectRender)
{
	point_cursor_default.x = rectRender.right / 2;
	point_cursor_default.y = rectRender.bottom / 2;
	ClientToScreen(m_hwnd, &point_cursor_default);
}

int Input::ReactToKeyPressed() {
	//如果按下ESC
	if (KeyPressed(VK_ESCAPE)) {
		SendMessage(m_hwnd, WM_CLOSE, 0, 0);
	}
	return OK;
}

void Input::ClearFlag()
{
	is_rbutton_up = is_lbutton_up = false;
}

inline bool Input::KeyPressed(int input) {
	return m_key_pressed.key[input];
}

bool Input::KeyPressed(char mouseButton)
{
	switch (mouseButton) {
	case 'r':
	case 'R':
		return m_key_pressed.right_mouse_button;
	case 'l':
	case 'L':
		return m_key_pressed.left_mouse_button;
	default:
		return KeyPressed((int)mouseButton);
	}
	return false;
}
