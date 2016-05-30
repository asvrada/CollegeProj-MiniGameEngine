#include "SceneManagerClass.h"

#include "WindowFrameClass.h"
#include "GameObjectClass.h"
#include "CameraClass.h"

SceneManager & SceneManager::update() {
	for (auto &item : objects_all) {
		item->update();
	}
	return *this;
}

SceneManager & SceneManager::init() {
	map_models["teapot"].LoadModel("Resources\\Models\\teapot.obj", TEXT("Resources\\Materials\\CheckerboardTexture.bmp"));
	
	objects_all.push_back(make_shared<Camera>());
	PTR_CONVERT(objects_all[0], Camera)->init((float)(WindowFrame::rect_client.right / (float)WindowFrame::rect_client.bottom), 70.0f).position.z = -70.0f;
	PTR_CONVERT(objects_all[0], Camera)->ChangeConfig();


	objects_all.push_back(make_shared<GameObject>());
	PTR_CONVERT(objects_all[1], GameObject)->model = "teapot";

	return *this;
}
