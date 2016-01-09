#include "InputClass.h"

InputClass::InputClass() {
	m_KeyPressed = nullptr;
	m_hWnd = 0;
}


InputClass::~InputClass() {
	delete []m_KeyPressed;
	m_KeyPressed = nullptr;
	m_hWnd = 0;
}

void InputClass::Initialize(HWND hWnd) {
	m_KeyPressed = new bool[512];
	memset(m_KeyPressed, 0, 512 * sizeof(char));
	m_hWnd = hWnd;
}

void InputClass::Press(int input) {
	m_KeyPressed[input] = true;
}

void InputClass::Release(int input) {
	m_KeyPressed[input] = false;
}

int InputClass::ReactToKeyPressed() {
	//如果按下ESC
	if (IsKeyPressed(VK_ESCAPE)) {
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
	return OK;
}

bool InputClass::IsKeyPressed(int input) {
	return m_KeyPressed[input];
}
