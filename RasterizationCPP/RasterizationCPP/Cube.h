#ifndef RASTERIZER_CUBE_H
#define RASTERIZER_CUBE_H

#include "ProjectHeader.h"

#include "GameObjectClass.h"

#include "TimeClass.h"
#include "InputClass.h"

class Cube : public GameObject {
public:
	void control() {
		if (Input::KeyPressed(VK_A)) {
			rotation.y -= 100 * Time::GetDeltaTime();
		}
		if (Input::KeyPressed(VK_D)) {
			rotation.x += 100 * Time::GetDeltaTime();
		}
	}

	void init() {
		rotation.y = 180.0f;
		model = "plane";
	}

	void update() {
		control();
	}
};

#endif