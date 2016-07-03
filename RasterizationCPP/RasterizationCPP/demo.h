#ifndef DEMO_H
#define DEMO_H

#include "GameObjectClass.h"
#include "ProjectHeader.h"

#include "SceneManagerClass.h"
#include "InputClass.h"

class cube : public GameObject {
public:
	float timeleft;
	void init() {
		model = "cube";
		rotation = Vector3(1, 1, 1);
		timeleft = 10;
	}

	void update() {
		if (active) {
			timeleft -= Time::GetDeltaTime();
			if (timeleft < 0) {
				active = false;
			}
		}
	}
};

class demo : public GameObject {
public:
	//初始化
	//不显示模型
	void init() {
		model = "";
	}

	void customControl() {
		auto &pressed_key = Input::last_pressed_key;
		if (Input::KeyPressed(pressed_key)) {
			auto ptr_scenemanager = SceneManager::s_ptr_scene_manager;
			auto ptr_cube = ptr_scenemanager->createCube();
			auto tmp = PTR_CONVERT(ptr_cube, cube);
			tmp->model = "cube";
			int margin = 25;
			switch (pressed_key) {
			case VK_A:
				tmp->position = Vector3(margin * 1, 0, margin * 2);
				break;
			case VK_B:
				tmp->position = Vector3(margin * 5, 0, margin * 1);
				break;
			case VK_C:
				tmp->position = Vector3(margin * 3, 0, margin * 1);
				break;
			case VK_D:
				tmp->position = Vector3(margin * 3, 0, margin * 2);
				break;
			case VK_E:
				tmp->position = Vector3(margin * 3, 0, margin * 3);
				break;
			case VK_F:
				tmp->position = Vector3(margin * 4, 0, margin * 2);
				break;
			case VK_G:
				tmp->position = Vector3(margin * 5, 0, margin * 2);
				break;
			case VK_H:
				tmp->position = Vector3(margin * 6, 0, margin * 2);
				break;
			default:
				break;
			}
		}
	}

	//每帧执行
	void update() {
		if (Input::KeyPressed(VK_3)) {
			active = !active;
		}
		if (active)
			customControl();
	}
};


#endif