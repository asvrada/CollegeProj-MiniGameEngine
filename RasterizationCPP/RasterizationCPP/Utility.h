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
矩阵与数字的乘法
*/

#include "ProjectHeader.h"

///////////////
// 提前声明 //
//////////////

template < class T = float >
class Vector2;

class Vector3;
class Vector4;
class Matrix3;
class Matrix4;
typedef struct Fragment_TYPE Fragment;

///////////
// 函数 //
//////////

//交换
template <typename T>
void swap(T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

//Clipping
//只要有一点落在外面，就不要
//同时进行背面剔除
void ClippingAndBackCull(vector<Fragment> &render_list);

//tranform vertices from Homo to Screen coordinate
void HomoToScreenCoord(Vector4& vertex);

//////////////////////
// 数学类、结构体 //
/////////////////////
template <class T>
class Vector2 {
public:
	T x, y;

	Vector2() {
		x = y = 0;
	}

	Vector2(T all) {
		x = y = all;
	}

	Vector2(T _x, T _y) {
		x = _x;
		y = _y;
	}

	Vector2(const Vector2& old) {
		x = old.x;
		y = old.y;
	}

	Vector2<T> operator * (float b) {
		return Vector2<T>(x * b, y * b);
	}
};

/*******************************
Vector 3
用于表示三维空间的点、旋转参数
*******************************/
class Vector3 {
public:
	//值
	float x, y, z;

	//构造函数
	Vector3() :x(0), y(0), z(0) {}
	Vector3(float _both) {
		x = y = z = _both;
	}
	Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
	Vector3(const Vector3 &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}
	~Vector3(){}

	//向量的归一化
	void VectorUnify();

	//向量叉乘
	Vector3 CrossProduct(const Vector3&);

	//向量点乘
	float DotProduct(const Vector3&);

	//返回根据当前向量生成的平移矩阵
	Matrix4 GetTransitonMatrix();


	/////////////////////
	//运算符重载部分  //
	////////////////////

	//3阶向量乘以3阶矩阵
	Vector3 operator * (const Matrix3&);
	//3阶向量乘以4阶矩阵
	Vector4 operator * (const Matrix4&);

	//加的运算符重载
	Vector3 operator + (const Vector3&);
	//减的运算符重载
	Vector3 operator - (const Vector3&);

	//输出流重载
	friend wstringstream& operator << (wstringstream&,  const Vector3&);
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

	//点 或 向量 转换至齐次坐标系
	Vector4(const Vector3& old, bool isVertex) {
		x = old.x;
		y = old.y;
		z = old.z;
		w = isVertex ? 1.0f : 0.0f;
	}

	~Vector4() {}

	//齐次坐标系的变换
	Vector4 operator* (const Matrix4& b);

	void VectorUnify();

	//输出流重载
	friend wstringstream &operator << (wstringstream&, const Vector4&);
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

	//创建平移矩阵
	Matrix4(const Vector3&);
	Matrix4(float x, float y, float z);

	//创建旋转矩阵
	//旋转单位为 度数
	//比如 正90 度
	//绕单一轴的旋转
	Matrix4(char axis, float degree);

	//根据 缩放、旋转、平移 生成矩阵
	Matrix4(Vector3 scale, Vector3 Rotation, Vector3 Position);

	//矩阵相乘
	Matrix4(Matrix4&, const Matrix4&);

	////////////////以上是构造函数////////////////
	//todo
	//inline

	//根据向量生成平移矩阵
	Matrix4& createTransitionMatrix4(const Vector3&);
	Matrix4& createTransitionMatrix4(float x = 0, float y = 0, float z = 0);

	//旋转矩阵
	Matrix4& createRotationMatrix4(const Vector3&);
	Matrix4& createRotationMatrix4(float x = 0, float y = 0, float z = 0);

	//缩放矩阵
	Matrix4& createScaleMatrix4(const Vector3&);
	Matrix4& createScaleMatrix4(float x = 0, float y = 0, float z = 0);

	//在当前矩阵上进行操作
	//进行平移
	Matrix4& changePosition(const Vector3&);
	Matrix4& changePosition(float x = 0, float y = 0, float z = 0);

	//进行旋转
	Matrix4& changeRotationSingleAxis(char axis, float degree);
	Matrix4& changeRotationMultiAxes(const Vector3&);
	Matrix4& changeRotationMultiAxes(float x = 0, float y = 0, float z = 0);

	//进行缩放
	Matrix4& changeScale(const Vector3&);
	Matrix4& changeScale(float x = 0, float y = 0, float z = 0);


	//求余子式
	float Determinant(const Matrix3&);
	//求逆矩阵
	Matrix4& Invert();

	////////////////
	// static 函数 //
	///////////////

	//todo

	/////////////////////
	//运算符重载部分  //
	////////////////////

	//重载乘法
	//矩阵 x 矩阵
	Matrix4 operator * (const  Matrix4&);
	//矩阵 x 数字
	Matrix4 operator * (const float&);
};

//存放一个将要渲染的三角形
typedef struct Fragment_TYPE {
	int state;
	HDC *texture;

	//变换后的顶点
	array<Vector4, 3> trans_vList;
	//顶点的贴图坐标索引
	//每个顶点有 2 个坐标信息
	array<Vector2<float>, 3> uvList;
	//该面的法向量
	Vector3 n;

	Fragment_TYPE(int _state, HDC *_texture) : state(_state), texture(_texture) {}
	~Fragment_TYPE() {
		texture = nullptr;
	}
}Fragment;

#endif
