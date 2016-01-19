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

//一个Object就是一个三角形
class Object {
	//三角形的三点
public:
	Vector3 vertex[3];
	Vector4 transformedVertex[3];
	UV vertexUV[3];

	Vector3 Position;
	Vector3 Rotation;

public:
	///////////////
	// 构造函数 //
	///////////////

	Object();
	//给出Position
	Object(Vector3);
	//给出Pos 和 Rotation
	Object(Vector3, Vector3);


	//乘上矩阵
	//todo
	Vector4 Transform(Matrix4);

private:
	void Initial();
};

#endif