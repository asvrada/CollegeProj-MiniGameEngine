#include "ObjectClass.h"

Object::Object() {
	Initial();

	Position.x = Position.y = Position.z = 0.0f;
	Rotation.x = Rotation.y = Rotation.z = 0.0f;
}

Object::Object(Vector3 Pos) {
	Initial();

	Position = Pos;
	Rotation.x = Rotation.y = Rotation.z = 0.0f;
}

Object::Object(Vector3 Pos, Vector3 rotation) {
	Initial();

	Position = Pos;
	Rotation = rotation;
}

void Object::Initial() {
	//三角形的三个点
	vertex[0].x = 0.0f; vertex[0].y = 100.0f; vertex[0].z = 0.0f;
	vertex[1].x = 100.0f; vertex[1].y = 0.0f; vertex[1].z = 0.0f;
	vertex[2].x = 0.0f; vertex[2].y = 0.0f; vertex[2].z = 0.0f;
	//三角形三个点的UV坐标
	/*
	vertexUV[0].u = 0.0f; vertexUV[0].v = 0.0f;
	vertexUV[1].u = 512.0f; vertexUV[1].v = 512.0f;
	vertexUV[2].u = 0.0f; vertexUV[2].v = 512.0f;
	*/
}