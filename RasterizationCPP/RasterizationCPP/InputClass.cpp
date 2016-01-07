#include "InputClass.h"

InputClass::InputClass() {
	KeyPressed = nullptr;
}


InputClass::~InputClass() {
	delete []KeyPressed;
	KeyPressed = nullptr;
}

void InputClass::Initialize() {
	KeyPressed = new bool[512];
	for (int lop = 0; lop < 512; lop++) {
		KeyPressed[lop] = false;
	}
}

void InputClass::Press(char input) {
	KeyPressed[(unsigned int)input] = true;
}

void InputClass::Release(char input) {
	KeyPressed[(unsigned int)input] = false;
}

bool InputClass::IsKeyPressed(char input) {
	return KeyPressed[(unsigned int)input];
}
