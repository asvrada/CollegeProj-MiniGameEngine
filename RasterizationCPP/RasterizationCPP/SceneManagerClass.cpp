#include "SceneManagerClass.h"

vector<Model>& SceneManager::getObjectsForRendering()
{
	return objects_all;
}

SceneManager & SceneManager::init()
{
	//初始化物体
	objects_all.push_back(Model());
	if (objects_all[0].Initial("Resources\\Models\\teapot.obj", TEXT("Resources\\Materials\\CheckerboardTexture.bmp")) == ERROR) {
		objects_all.pop_back();
	}
	objects_all[0].rotation = Vector3(0, 180.f, 0);
	return *this;
}
