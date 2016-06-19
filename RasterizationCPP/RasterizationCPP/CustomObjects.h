#ifndef RASTERIZER_CUSTOMOBJECTS_H
#define RASTERIZER_CUSTOMOBJECTS_H

#include "ProjectHeader.h"

#include "GameObjectClass.h"

//为了static
#include "SceneManagerClass.h"
#include "TimeClass.h"
#include "InputClass.h"

#include <cmath>
#include <ctime>

//随机生成0-1之间的数
double random() {
	return (double)std::rand() / RAND_MAX;
}

//本文件之外不可见
static int GridLength = 10;

//每一节身体
class Body : public GameObject {
public:
	int x;
	int y;

	//初始化位置
	Body(int _x, int _y) {
		x = _x;
		y = _y;
	}

	void init() {
		model = "cube";
		//一个身体1单位长
		scale = Vector3(0.1f);
	}

	//什么也不干
	void update() {

	}
};

//贪吃蛇的身体
class Snake : public GameObject {
public:
	//这里每插入一个
	//全局的那个也要对应插入一个
	vector<shared_ptr<Body>> snake;
	vector<shared_ptr<Object>>* reference_objects_all;

public:
	//构造函数
	Snake() {
		reference_objects_all = nullptr;
	}

	void init() {
		auto head = make_shared<Body>((int)(random() * GridLength), (int)(random() * GridLength));
		snake.push_back(head);
		reference_objects_all->push_back(head);

	}

	void update() {

	}

	void moveForward() {

	}
};

//整个贪吃蛇程序
class SnakeFrame : public GameObject {
private:
	Snake snake_body;
	
public:
	//初始化Snake
	SnakeFrame() {
		snake_body.reference_objects_all = &SceneManager::s_ptr_scene_manager->objects_all;
	}

	void init() {
		std::srand((unsigned int)std::time(0));
	}

	void update() {

	}
};

#endif