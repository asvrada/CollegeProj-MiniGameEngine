#ifndef RASTERIZER_INPUTCLASS_H
#define RASTERIZER_INPUTCLASS_H

#include "ProjectHeader.h"

struct KeyPressedList {
	//数组，记录按下的按键
	bool key[512];
	//鼠标左右键
	bool right_mouse_button, left_mouse_button;

	KeyPressedList() {
		memset(key, false, 512 * sizeof(char));
		right_mouse_button = left_mouse_button = false;
	}
};

class Input {
private:
	//数组，记录按下的按键
	static KeyPressedList m_key_pressed;

	HWND m_hwnd;
public:
	///////////
	// static //
	//////////
	//记录鼠标应当处于的坐标
	static POINT point_cursor_default;
	//记录鼠标当前的位置
	static POINT point_cursor_current;
	static bool is_center_snapped;
	static bool is_rbutton_up;
	static bool is_lbutton_up;
public:
	Input();
	~Input();

	void Initialize(HWND);
	void Press(int);
	void Press(char);
	void Release(int);
	void Release(char);

	//画布大小改变或
	//窗口移动后调用
	void UpdateCursorCenterPostion(const RECT &rect_of_client);
	//根据已按下的按键做出反应
	int ReactToKeyPressed();
	void ClearFlag();

	static bool KeyPressed(int);
	static bool KeyPressed(char);
};

#endif