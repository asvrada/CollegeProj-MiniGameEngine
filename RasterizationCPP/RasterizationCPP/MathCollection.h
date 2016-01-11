#ifndef RASTERIZER_MATHCLASS_H
#define RASTERIZER_MATHCLASS_H

/*
包含各种数学计算所需的函数等等
具体包括
二维向量
三维向量

向量之间的点积，叉积
向量之间的加法，减法
向量的归一化

三阶矩阵
四阶矩阵

以及矩阵之间的乘法，加法
矩阵的单位化
矩阵与数字的乘法
*/

#include "Declarations.h"

//三维空间向量
class FLOAT3D {
public:
	//值
	float x, y, z;

	//构造函数
	FLOAT3D() {
		x = y = z = 0.0f;
	}
	FLOAT3D(float _both) {
		x = y = z = _both;
	}
	FLOAT3D(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	FLOAT3D(const FLOAT3D &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	//向量叉乘
	FLOAT3D CrossProduct(const FLOAT3D);

	//向量点乘
	float DotProduct(const FLOAT3D);
	
	//加的运算符重载
	FLOAT3D operator + (const FLOAT3D);
	//减的运算符重载
	FLOAT3D operator - (const FLOAT3D);
};


#endif