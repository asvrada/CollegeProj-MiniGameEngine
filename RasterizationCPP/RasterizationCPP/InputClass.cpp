#include "InputClass.h"

InputClass::InputClass() {
	rButtonUp = lButtonUp = false;
	m_hWnd = 0;
}


InputClass::~InputClass() {
	m_hWnd = 0;
}

void InputClass::Initialize(HWND hWnd) {
	isCenterSnapped = false;
	m_hWnd = hWnd;
}

void InputClass::Press(int input) {
	m_KeyPressed.m_KeyPressed[input] = true;
}

void InputClass::Press(char mouseButton) {
	switch (mouseButton) {
	case 'r':
	case 'R':
		m_KeyPressed.r = true;
		break;
	case 'l':
	case 'L':
		m_KeyPressed.l = true;
		break;
	default:
		break;
	}
}

void InputClass::Release(int input) {
	m_KeyPressed.m_KeyPressed[input] = false;
}

void InputClass::Release(char mouseButton)
{
	switch (mouseButton) {
	case 'r':
	case 'R':
		m_KeyPressed.r = false;
	case 'l':
	case 'L':
		m_KeyPressed.l = false;
	default:
		Release((int)mouseButton);
	}
}

void InputClass::UpdateCursorCenterPostion(const RECT &rectRender)
{
	rectCursorCenterPostion.x = rectRender.right / 2;
	rectCursorCenterPostion.y = rectRender.bottom / 2;
	ClientToScreen(m_hWnd, &rectCursorCenterPostion);
}

int InputClass::ReactToKeyPressed() {
	//如果按下ESC
	if (IsKeyPressed(VK_ESCAPE)) {
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
	return OK;
}

void InputClass::clearFlag()
{
	rButtonUp = lButtonUp = false;
}

inline bool InputClass::IsKeyPressed(int input) {
	return m_KeyPressed.m_KeyPressed[input];
}

bool InputClass::IsKeyPressed(char mouseButton)
{
	switch (mouseButton) {
	case 'r':
	case 'R':
		return m_KeyPressed.r;
	case 'l':
	case 'L':
		return m_KeyPressed.l;
	default:
		return IsKeyPressed((int)mouseButton);
	}
	return false;
}
