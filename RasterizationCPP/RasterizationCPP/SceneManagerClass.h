#ifndef RASTERIZER_SCENEMANAGERCLASS_H
#define RASTERIZER_SCENEMANAGERCLASS_H

#include "ProjectHeader.h"

#include "CameraClass.h"
#include "ModelInfo.h"

//保存场景有什么物体
class SceneManager {
public:
	static SceneManager *s_ptr_scene_manager;

	vector<shared_ptr<Object>> objects_all;
	map<string, ModelInfo> map_models;

	Camera *main_camera;
public:
	SceneManager();

	//对所有object初始化
	SceneManager& init();

	//创建固定类
	shared_ptr<Object> createCube();
	//销毁类
	void destroyCube(shared_ptr<Object>);

	//弃用
	shared_ptr<Object> createClass(string class_name);

	//每帧调用
	//更新所有物体的情况
	SceneManager& update();

	Object* findElementByName(string name);
};

#endif