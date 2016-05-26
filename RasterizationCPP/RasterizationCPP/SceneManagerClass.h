#ifndef RASTERIZER_SCENEMANAGERCLASS_H
#define RASTERIZER_SCENEMANAGERCLASS_H

#include "ProjectHeader.h"
#include "ObjectClass.h"

//保存场景有什么物体
//摄像机位置等等
class SceneManager {
private:
	vector<Object> objects_all;
public:
	//返回物体列表，以供渲染
	vector<Object>& getObjectsForRendering();
};

#endif