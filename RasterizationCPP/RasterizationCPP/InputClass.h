#ifndef RASTERIZER_INPUT_H
#define RASTERIZER_INPUT_H

#include "Declarations.h"

class InputClass {
private:
	bool *m_KeyPressed;
	HWND m_hWnd;
public:
	InputClass();
	~InputClass();

	void Initialize(HWND);
	void Press(int);
	void Release(int);

	//根据已按下的按键做出反应
	int ReactToKeyPressed();

	inline bool IsKeyPressed(int);
};

#endif