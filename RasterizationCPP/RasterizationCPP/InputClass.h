#ifndef RASTERIZER_INPUT_H
#define RASTERIZER_INPUT_H

#include "ProjectHeader.h"

struct KeyPressedList {
	//数组，记录按下的按键
	bool m_KeyPressed[512];
	//鼠标左右键
	bool r, l;

	KeyPressedList() {
		memset(m_KeyPressed, false, 512 * sizeof(char));
		r = l = false;
	}
};

class InputClass {
private:
	//数组，记录按下的按键
	KeyPressedList m_KeyPressed;

	HWND m_hWnd;
public:
	//记录鼠标应当处于的坐标
	POINT rectCursorCenterPostion;
	//记录鼠标当前的位置
	POINT rectCursor;
	bool isCenterSnapped;
	bool rButtonUp;
	bool lButtonUp;
public:
	InputClass();
	~InputClass();

	void Initialize(HWND);
	void Press(int);
	void Press(char);
	void Release(int);
	void Release(char);

	//画布大小改变或
	//窗口移动后调用
	void UpdateCursorCenterPostion(const RECT &rectRender);
	//根据已按下的按键做出反应
	int ReactToKeyPressed();
	void clearFlag();

	bool IsKeyPressed(int);
	bool IsKeyPressed(char);
};

#endif