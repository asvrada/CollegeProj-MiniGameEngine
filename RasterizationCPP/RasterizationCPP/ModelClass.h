#ifndef RASTERIZER_MODELCLASS_H
#define RASTERIZER_MODELCLASS_H

#include "ProjectHeader.h"

//目前一个Object就是一个三角形
class Model {
public:
	//顶点的 x y z 
	vector<Vector3> vertices;
	//顶点的 UV
	vector<Vector2<float>> uv;
	//每对索引 x -> 顶点, y -> UV
	vector<Vector2<int>> indices;

	//纹理
	HDC hdc_texture;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
public:
	///////////////
	// 构造函数 //
	///////////////
	Model();
	//给出Position
	Model(Vector3);
	//给出Pos 和 Rotation
	Model(Vector3, Vector3);

	//初始化模型数据
	int Initial(char*, LPCWSTR);
};

#endif