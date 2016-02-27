#include "ObjectClass.h"

#include <fstream>
using std::ifstream;

Object::Object() {
	position.x = position.y = position.z = 0.0f;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

int Object::Initial(char *fileName)
{
	ifstream file(fileName);
	if (!file.is_open()) {
		return ERROR;
	}
	int maxIndex = 0;

	char type = 0;
	float x = 0, y = 0, z = 0;
	while (!file.eof()) {
		file >> type >> x >> y >> z;
		switch (type)
		{
		case 'v':
			vertices.push_back(Vector3(x, y, z));
			break;
		case 'f':
			indices.push_back((int)(x - 1));
			indices.push_back((int)(y - 1));
			indices.push_back((int)(z - 1));
			break;
		case '#':
			break;
		default:
			break;
		}
	}
	return OK;
}

Object::Object(Vector3 _position) {
	position = _position;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

Object::Object(Vector3 _position, Vector3 _rotation) {
	position = _position;
	rotation = _rotation;
}