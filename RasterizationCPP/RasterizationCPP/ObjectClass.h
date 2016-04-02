#ifndef RASTERIZER_OBJECTCLASS_H
#define RASTERIZER_OBJECTCLASS_H

#include "ProjectHeader.h"

//目前一个Object就是一个三角形
class Object {
public:
	vector<Vector3> vertices;
	vector<Vector2<float>> uv;
	vector<Vector2<int>> indices;

	HDC hdc_texture;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
public:
	///////////////
	// 构造函数 //
	///////////////
	Object();
	int Initial(char*, LPCWSTR);
	//给出Position
	Object(Vector3);
	//给出Pos 和 Rotation
	Object(Vector3, Vector3);
};

#endif