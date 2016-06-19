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
	GameObject() {
		scale.x = scale.y = scale.z = 1;
	}

	/////////
	// ¼Ì³Ð //
	/////////
	void init() {};
	void update() {};
};

#endif