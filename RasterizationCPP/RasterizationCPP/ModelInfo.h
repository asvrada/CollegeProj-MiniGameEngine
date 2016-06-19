#ifndef RASTERIZER_MODELINFO_H
#define RASTERIZER_MODELINFO_H

#include "ProjectHeader.h"

//存放模型信息
class ModelInfo {
public:
	//顶点的 x y z
	vector<Vector3> vertices;
	//顶点的 UV
	vector<Vector2<float>> uv;
	//每对索引 x -> 顶点, y -> UV
	vector<Vector2<int>> indices;

	//纹理
	HDC hdc_texture;

public:
	ModelInfo() {};
	~ModelInfo() {};

	int LoadModel(const char *fileName, LPCWSTR texturename);
};

#endif