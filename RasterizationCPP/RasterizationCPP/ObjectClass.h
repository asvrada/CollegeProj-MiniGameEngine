#ifndef RASTERIZER_OBJECTCLASS_H
#define RASTERIZER_OBJECTCLASS_H

#include "ProjectHeader.h"

#include "jsonparser\jsonxx.h"
#include <fstream>
using std::ifstream;

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
class ObjectClass {
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
	ObjectClass();
	//todo
	void Initial(char*);
	//给出Position
	ObjectClass(Vector3);
	//给出Pos 和 Rotation
	ObjectClass(Vector3, Vector3);
};

#endif