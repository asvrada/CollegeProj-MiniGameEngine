#ifndef RASTERIZER_CUBE_H
#define RASTERIZER_CUBE_H

#include "ProjectHeader.h"

#include "GameObjectClass.h"

#include "TimeClass.h"
#include "InputClass.h"

class Cube : public GameObject {
public:
	void control() {
		if (Input::KeyPressed(65)) {
			rotation.y -= 100 * Time::GetDeltaTime();
		}
		if (Input::KeyPressed(68)) {
			rotation.z += 100 * Time::GetDeltaTime();
		}
	}

	void update() {
		control();
	}
};

#endif