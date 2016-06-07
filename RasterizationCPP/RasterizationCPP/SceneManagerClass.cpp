#include "SceneManagerClass.h"

#include "WindowFrameClass.h"

SceneManager::SceneManager() {
	main_camera = nullptr;
}

SceneManager & SceneManager::update() {
	for (auto &item : objects_all) {
		item->update();
	}
	return *this;
}

//以后由文件载入自动生成
SceneManager & SceneManager::init() {
	map_models["teapot"].LoadModel("Resources\\Models\\teapot.obj", TEXT("Resources\\Materials\\CheckerboardTexture.bmp"));
	map_models["cube"].LoadModel("Resources\\Models\\cube.obj", TEXT("Resources\\Materials\\CheckerboardTexture.bmp"));
	map_models["plane"].LoadModel("Resources\\Models\\plane.obj", TEXT("Resources\\Materials\\CheckerboardTexture.bmp"));

	//创建主相机
	objects_all.push_back(make_shared<Camera>());
	main_camera = PTR_CONVERT(objects_all[0], Camera);
	main_camera->init((float)(WindowFrame::rect_client.right / (float)WindowFrame::rect_client.bottom), 70.0f).position.z = -140.0f;
	main_camera->ChangeConfig().active = false;


	objects_all.push_back(make_shared<Cube>());
	

	for (auto &item : objects_all) {
		item.get()->init();
	}

	return *this;
}

Object* SceneManager::findElementByName(string target) {
	for (auto &item : objects_all) {
		if (item->name == target) {
			return item.get();
		}
	}
	return nullptr;
}