#include "ObjectClass.h"

ObjectClass::ObjectClass() {
	m_Initial();

	position.x = position.y = position.z = 0.0f;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

ObjectClass::ObjectClass(Vector3 _position) {
	m_Initial();

	position = _position;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

ObjectClass::ObjectClass(Vector3 _position, Vector3 _rotation) {
	m_Initial();

	position = _position;
	rotation = _rotation;
}

void ObjectClass::m_Initial() {
	//三角形的三个点
	vertices.push_back(Vector3(0.0f));
	vertices.push_back(Vector3(0.f, 100.0f, 0.f));
	vertices.push_back(Vector3(100.f, 0.f, 0.f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	
	//三角形三个点的UV坐标
	/*
	vertexUV[0].u = 0.0f; vertexUV[0].v = 0.0f;
	vertexUV[1].u = 512.0f; vertexUV[1].v = 512.0f;
	vertexUV[2].u = 0.0f; vertexUV[2].v = 512.0f;
	*/
}