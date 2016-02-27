#ifndef RASTERIZER_OBJECTCLASS_H
#define RASTERIZER_OBJECTCLASS_H

#include "ProjectHeader.h"

struct UV {
	float u, v;

	UV(float a, float b) {
		u = a;
		v = b;
	}
	UV() {
		u = v = 0.0f;
	}
};

//目前一个Object就是一个三角形
class Object {
	//三角形的三点
public:
	vector<Vector3> vertices;
	vector<int> indices;

	Vector3 position;
	Vector3 rotation;

public:
	///////////////
	// 构造函数 //
	///////////////
	Object();
	//todo
	int Initial(char*);
	//给出Position
	Object(Vector3);
	//给出Pos 和 Rotation
	Object(Vector3, Vector3);
};

#endif