#ifndef RASTERIZER_OBJECTCLASS_H
#define RASTERIZER_OBJECTCLASS_H

#include "ProjectHeader.h"

//所有物体的基类
class Object {
public:
	bool active;
	//物体名称
	//必填
	string tag;
	//物体位置
	Vector3 position;
	//物体旋转情况
	Vector3 rotation;

public:
	//构造函数
	//必须给出名字
	Object() { active = true; }
	Object(string _name) :tag(_name) {};

	//每帧调用
	virtual void update() = 0;
	//创建时调用
	virtual void init() = 0;
};

#endif