#ifndef RASTERIZER_GAMEOBJECTCLASS_H
#define RASTERIZER_GAMEOBJECTCLASS_H

#include "ProjectHeader.h"

#include "ObjectClass.h"

class GameObject : public Object {
public:
	//Ëõ·ÅÒò×Ó
	Vector3 scale;
	string model;
public:
	GameObject() {}

	/////////
	// ¼Ì³Ð //
	/////////
	void init() {};
	void update() {};
};

#endif