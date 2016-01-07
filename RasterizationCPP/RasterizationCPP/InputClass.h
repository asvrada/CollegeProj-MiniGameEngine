#ifndef RASTERIZATIONCPP_INPUT_H
#define RASTERIZATIONCPP_INPUT_H

#include "Declarations.h"

class InputClass {
private:
	bool *KeyPressed;
public:
	InputClass();
	~InputClass();

	void Initialize();
	void Press(char);
	void Release(char);

	bool IsKeyPressed(char);
};

#endif