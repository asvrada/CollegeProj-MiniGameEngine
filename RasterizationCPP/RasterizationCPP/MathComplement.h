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
#include <memory>
using std::memset;

#include "MarcoDefinition.h"

class Vector3;
//todo
class Vector4;
class Matrix3;
class Matrix4;

//交换
template <typename T>
void swap(T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

/*******************************
Vector 3
用于表示三维空间的点、旋转参数
*******************************/
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
	Vector3 operator * (const Matrix3&);
	
	//加的运算符重载
	Vector3 operator + (const Vector3&);
	//减的运算符重载
	Vector3 operator - (const Vector3&);
};


/****************
Vector 4

用于齐次剪彩空间的相应运算
*******************/

class Vector4 {
public:
	//值
	float x, y, z, w;

	//构造函数
	Vector4() {
		x = y = z = w = 0.0f;
	}
	Vector4(float _both) {
		x = y = z = w = _both;
	}
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	Vector4(const Vector4 &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
		w = copy.w;
	}
	~Vector4() {}
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
	void SetZero() {
		memset(var, 0, 4 * 4 * sizeof(float));
	}


	/////////////////////////
	// 特殊用处的构造矩阵 //
	/////////////////////////

	//创建对角线元素为给定数值的矩阵
	//用于缩放
	Matrix4(float);

	//创建平移矩阵
	Matrix4(const Vector3&);
	Matrix4(float x, float y, float z);

	//创建旋转矩阵
	//旋转单位为 度数
	//比如 正90 度
	//绕单一轴的旋转
	Matrix4(char axis, float degree);
	//给定所有三个轴的旋转参数
	Matrix4(char ThisUseless, const Vector3&);
	Matrix4(char ThisUseless, float degreeX, float degreeY, float degreeZ);

	//根据 缩放、旋转、平移 生成矩阵
	Matrix4(float scale, Vector3 Rotation, Vector3 Position);

	//矩阵相乘
	Matrix4(Matrix4&, Matrix4&);

	////////////////以上是构造函数////////////////

	//根据向量生成平移矩阵
	void TransitionMatrix(const Vector3 &);

	//求余子式
	float Determinant(Matrix3&);
	//求逆矩阵
	Matrix4 Invert();

	/////////////////////
	//运算符重载部分  //
	////////////////////

	//重载乘法
	//矩阵 x 矩阵
	Matrix4 operator * (const  Matrix4&);
	//矩阵 x 数字
	Matrix4 operator * (const float&);
};


///////////////////
// 非类成员函数 //
//////////////////

//todo
//这个有必要吗
Matrix4 RotationSingleAxis(char axis, float degree);
//Matrix4 Rotation(float x, float y, float z);

#endif