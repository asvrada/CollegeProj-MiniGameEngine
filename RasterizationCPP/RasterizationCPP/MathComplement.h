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

#include <cmath>
#include "Declarations.h"

class Vector3;
class Vector4;
class Matrix3;
class Matrix4;


//三维空间向量
class Vector3 {
public:
	//值
	float x, y, z;

	//构造函数
	Vector3() {
		x = y = z = 0.0f;
	}
	Vector3(float _both) {
		x = y = z = _both;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3(const Vector3 &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}
	~Vector3(){}

	//向量的归一化
	void VectorUnify();

	//向量叉乘
	Vector3 CrossProduct(const Vector3);

	//向量点乘
	float DotProduct(const Vector3);

	//返回根据当前向量生成的平移矩阵
	Matrix4 GetTransitonMatrix();


	/////////////////////
	//运算符重载部分  //
	////////////////////

	//3阶向量乘以3阶矩阵
	Vector3 operator * (const Matrix3);
	
	//加的运算符重载
	Vector3 operator + (const Vector3);
	//减的运算符重载
	Vector3 operator - (const Vector3);
};

/*
矩阵
3 x 3

由于3阶矩阵在使用过程中不是很常见
所以暂不添加求逆矩阵等的"高级"功能
*/
class Matrix3 {
public:
	float var[3][3];

	Matrix3();
	Matrix3(const Matrix3&);
	~Matrix3() {}

	void SetZero();


	/////////////////////
	//运算符重载部分  //
	////////////////////

	//重载乘法
	Matrix3 operator * (const Matrix3&);
	Matrix3 operator * (const float&);
};


/***************************
矩阵
4 x 4
***************************/

class Matrix4 {
public:
	float var[4][4];

	//常见的构造矩阵
	Matrix4();
	Matrix4(const Matrix4&);
	~Matrix4() {}
	void SetZero();


	/////////////////////////
	// 特殊用处的构造矩阵 //
	/////////////////////////

	//创建对角线元素为给定数值的矩阵
	//其他元素为0
	Matrix4(float);

	//创建平移矩阵
	Matrix4(const Vector3&);
	//todo
	//MAtrix4(float x,float y,float z);

	//创建旋转矩阵
	//旋转单位为 度数
	//比如 +90 度
	//绕单一轴的旋转
	Matrix4(char axis, float degree);
	//给定所有三个轴的旋转参数
	//todo
	Matrix4(float degreeX, float degreeY, float degreeZ);

	////////////////以上是构造函数////////////////

	//根据向量生成平移矩阵
	void TransitionMatrix(const Vector3 &);

	//fixme
	//根据旋转的参数将当前矩阵设置成为旋转矩阵
	//void GetRotateSingleAxis(char axis, float degree);
	
	//求余子式
	float Determinant(Matrix3&);
	//求逆矩阵
	void Invert();

	/////////////////////
	//运算符重载部分  //
	////////////////////

	//重载乘法
	//矩阵 x 矩阵
	Matrix4 operator * (const  Matrix4&);
	//矩阵 x 数字
	Matrix4 operator * (const float&);
};

#endif