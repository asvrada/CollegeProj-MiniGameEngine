#ifndef RASTERIZER_SCENEMANAGERCLASS_H
#define RASTERIZER_SCENEMANAGERCLASS_H

#include "ProjectHeader.h"

#include "ObjectClass.h"
#include "ModelInfo.h"
//保存场景有什么物体
class SceneManager {
public:
	vector<shared_ptr<Object>> objects_all;
	map<string, ModelInfo> map_models;

public:
	//对所有object初始化
	SceneManager& init();

	//每帧调用
	//更新所有物体的情况
	SceneManager& update();
};

#endif

//todo
//添加专门存放模型的vector
