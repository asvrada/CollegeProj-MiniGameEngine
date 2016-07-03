#include "SceneManagerClass.h"

#include "WindowFrameClass.h"
#include "demo.h"
#include "XML/XMLLoader.h"

SceneManager * SceneManager::s_ptr_scene_manager = nullptr;

SceneManager::SceneManager() {
	main_camera = nullptr;
	s_ptr_scene_manager = this;
}

//根据字符串创建相应的类
shared_ptr<Object> SceneManager::createClass(string class_name) {
	if (class_name == "Camera") {
		objects_all.push_back(make_shared<Camera>());
		return objects_all.back();
	}
}

SceneManager & SceneManager::update() {
	int length = (int)objects_all.size();
	for (int lop = 0; lop < length; lop++) {
		objects_all[lop]->update();
	}
	/*
	for (auto &item : objects_all) {
		item->update();
	}
	*/
	return *this;
}

//以后由文件载入自动生成
SceneManager & SceneManager::init() {
	jeff_XML::XMLLoader xml("Resources\\Scenes\\snake.xml");
	auto models_to_load = xml["model"];

	//先载入模型和纹理
	auto tmp_texture = TEXT("Resources\\Materials\\CheckerboardTexture.bmp");
	for (auto &each : models_to_load) {
		auto &model_name = each.getValue().getString()->val;
		auto model_direct = "Resources\\Models\\" + model_name + ".obj";
		map_models[model_name].LoadModel(model_direct.c_str(), tmp_texture);
	}

	/////////////////
	// XML 载入测试 //
	/////////////////
	/*
	auto object_list = xml["object"];
	for (auto &each : object_list) {
		jeff_XML::Object tmp;
		each.findTag("class", tmp);

		auto smartptr_new_object = createClass(tmp.tag);

		//摄像机特殊一点
		if (tmp.tag == "Camera") {
			auto ptr_camera = PTR_CONVERT(smartptr_new_object, Camera);
			//todo
			ptr_camera->init((float)(WindowFrame::rect_client.right / (float)WindowFrame::rect_client.bottom), 70.0f);

		}
	}
	*/

	//创建主相机
	auto ptr_camera = make_shared<Camera>();
	
	ptr_camera->init((float)(WindowFrame::rect_client.right / (float)WindowFrame::rect_client.bottom), 70.0f);
	ptr_camera->ChangeConfig();

	objects_all.push_back(ptr_camera);

	objects_all.push_back(make_shared<demo>());

	for (auto &item : objects_all) {
		item.get()->init();
	}

	return *this;
}

shared_ptr<Object> SceneManager::createCube() {
	auto ptr = make_shared<cube>();
	ptr->init();
	objects_all.push_back(ptr);
	return ptr;
}

void SceneManager::destroyCube(shared_ptr<Object> target) {
	//暂时先这样做吧
	target->active = false;
	/*shared_ptr<Object> toDelete;
	for (auto &item : objects_all) {
		if (item.get() == target.get()) {
			toDelete = item;
			break;
		}
	}
	objects_all.erase(toDelete);*/
}

Object* SceneManager::findElementByName(string target) {
	for (auto &item : objects_all) {
		if (item->tag == target) {
			return item.get();
		}
	}
	return nullptr;
}