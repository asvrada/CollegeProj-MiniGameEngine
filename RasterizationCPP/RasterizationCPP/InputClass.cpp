#include "InputClass.h"

InputClass::InputClass() {
	is_rbutton_up = is_lbutton_up = false;
	m_hwnd = 0;
}


InputClass::~InputClass() {
	m_hwnd = 0;
}

void InputClass::Initialize(HWND hWnd) {
	is_center_snapped = false;
	m_hwnd = hWnd;
}

void InputClass::Press(int input) {
	m_key_pressed.key[input] = true;
}

void InputClass::Press(char mouseButton) {
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

void InputClass::Release(int input) {
	m_key_pressed.key[input] = false;
}

void InputClass::Release(char mouseButton)
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

void InputClass::UpdateCursorCenterPostion(const RECT &rectRender)
{
	point_cursor_default.x = rectRender.right / 2;
	point_cursor_default.y = rectRender.bottom / 2;
	ClientToScreen(m_hwnd, &point_cursor_default);
}

int InputClass::ReactToKeyPressed() {
	//如果按下ESC
	if (KeyPressed(VK_ESCAPE)) {
		SendMessage(m_hwnd, WM_CLOSE, 0, 0);
	}
	return OK;
}

void InputClass::ClearFlag()
{
	is_rbutton_up = is_lbutton_up = false;
}

inline bool InputClass::KeyPressed(int input) {
	return m_key_pressed.key[input];
}

bool InputClass::KeyPressed(char mouseButton)
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
