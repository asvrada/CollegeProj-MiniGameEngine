#include "Utility.h"

#include "WindowFrameClass.h"

void ClippingAndBackCull(vector<Fragment> &rl) {

	/*
	Vector2<int> *a = nullptr;
	Vector2<int> *b = nullptr;
	Vector2<int> *c = nullptr;
	for (auto cur = indices.begin(); cur != indices.end(); cur += 3) {
		a = &(*cur);
		b = &(*(cur + 1));
		c = &(*(cur + 2));
		if ((vertices[a->x].x < 1.0f && vertices[a->x].x > -1.0f) && (vertices[a->x].y < 1.0f && vertices[a->x].y > -1.0f) && (vertices[a->x].z > 0) &&
			(vertices[b->x].x < 1.0f && vertices[b->x].x > -1.0f) && (vertices[b->x].y < 1.0f && vertices[b->x].y > -1.0f) && (vertices[b->x].z > 0) &&
			(vertices[c->x].x < 1.0f && vertices[c->x].x > -1.0f) && (vertices[c->x].y < 1.0f && vertices[c->x].y > -1.0f) && (vertices[c->x].z > 0)) {
			clipped_indices.push_back(*a);
			clipped_indices.push_back(*b);
			clipped_indices.push_back(*c);
		}
	}
	a = b = c = nullptr;
	*/
	/*while (!indices.empty()) {
		Vector2<int> a = indices.front();
		indices.pop();
		Vector2<int> b = indices.front();
		indices.pop();
		Vector2<int> c = indices.front();
		indices.pop();

		if ((vertices[a.x].x < 1.0f && vertices[a.x].x > -1.0f) && (vertices[a.x].y < 1.0f && vertices[a.x].y > -1.0f) && (vertices[a.x].z > 0) &&
			(vertices[b.x].x < 1.0f && vertices[b.x].x > -1.0f) && (vertices[b.x].y < 1.0f && vertices[b.x].y > -1.0f) && (vertices[b.x].z > 0) &&
			(vertices[c.x].x < 1.0f && vertices[c.x].x > -1.0f) && (vertices[c.x].y < 1.0f && vertices[c.x].y > -1.0f) && (vertices[c.x].z > 0)) {
			clipped_indices.push_back(a);
			clipped_indices.push_back(b);
			clipped_indices.push_back(c);
		}
	}*/
}

void HomoToScreenCoord(Vector4& vertex) {
	const RECT &screen = WindowFrame::rect_client;
	vertex.x = (vertex.x + 1.0f) * screen.right / 2.0f;
	vertex.y = (vertex.y + 1.0f) * screen.bottom / 2.0f;
}

bool TriangleBackcull(Fragment &fg)
{
	/*
	Vector3 view(0.0f, 0.0f, 1.0f);
	Vector3 a(set[0].x - set[1].x, set[0].y - set[1].y, set[0].z - set[1].z);
	Vector3 b(set[2].x - set[1].x, set[2].y - set[1].y, set[2].z - set[1].z);

	return (view.DotProduct(a.CrossProduct(b)) <= 0);
	*/
}

/*************************
Vector3
*************************/

inline void Vector3::VectorUnify() {
	float length = x * x + y * y + z * z;
	length = sqrtf(length);
	x /= length;
	y /= length;
	z /= length;
}

inline Vector3 Vector3::CrossProduct(const Vector3& b)
{
	Vector3 output;
	output.x = y*b.z - z*b.y;
	output.y = z*b.x - x*b.z;
	output.z = x*b.y - y*b.x;
	return output;
}

inline float Vector3::DotProduct(const Vector3& b)
{
	return x*b.x + y*b.y + z*b.z;
}

Matrix4 Vector3::GetTransitonMatrix()
{
	Matrix4 matrix_T;

	matrix_T.var[0][0] = 1.0f;
	matrix_T.var[1][1] = 1.0f;
	matrix_T.var[2][2] = 1.0f;
	matrix_T.var[3][3] = 1.0f;

	matrix_T.var[3][0] = x;
	matrix_T.var[3][1] = y;
	matrix_T.var[3][2] = z;

	return matrix_T;
}

Vector3 Vector3::operator * (const Matrix3 &b)
{
	return Vector3(
		x*b.var[0][0] + y*b.var[1][0] + z*b.var[2][0],
		x*b.var[0][1] + y*b.var[1][1] + z*b.var[2][1],
		x*b.var[0][2] + y*b.var[1][2] + z*b.var[2][2]);
}

Vector4 Vector3::operator * (const Matrix4 &b)
{
	return Vector4(*this,true) * b;
}

Vector3 Vector3::operator+(const Vector3 &b)
{
	return Vector3(x + b.x, y + b.y, z + b.z);
}

Vector3 Vector3::operator-(const Vector3 &b)
{
	return Vector3(x - b.x, y - b.y, z - b.z);
}

wstringstream &operator << (wstringstream& ws, const Vector3& v) {
	ws << "[ " << v.x << ", " << v.y << ", " << v.z << " ] ";
	return ws;
}

/*************************
Vector4
*************************/


Vector4 Vector4::operator*(const Matrix4 & b)
{
	Vector4 tmp;
	tmp.x = x*b.var[0][0] + y*b.var[1][0] + z*b.var[2][0] + w * b.var[3][0];
	tmp.y = x*b.var[0][1] + y*b.var[1][1] + z*b.var[2][1] + w * b.var[3][1];
	tmp.z = x*b.var[0][2] + y*b.var[1][2] + z*b.var[2][2] + w * b.var[3][2];
	tmp.w = x*b.var[0][3] + y*b.var[1][3] + z*b.var[2][3] + w * b.var[3][3];
	return tmp;
}

void Vector4::VectorUnify()
{
	float length = x * x + y * y + z * z;
	length = sqrtf(length);
	x /= length;
	y /= length;
	z /= length;
}

wstringstream& operator << (wstringstream& ws, const Vector4& v) {
	ws << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " ]";
	return ws;
}


/************************
MARTIX
3 x 3
*************************/
Matrix3::Matrix3() {
	memset(var, 0, 3 * 3 * sizeof(float));
}

Matrix3::Matrix3(const Matrix3 &old)
{
	memcpy(var, old.var, 3 * 3 * sizeof(float));
}

void Matrix3::SetZero()
{
	memset(var, 0, 3 * 3 * sizeof(float));
}

Matrix3 Matrix3::operator*(const Matrix3 &b)
{
	Matrix3 martix;
	for (int lop = 0; lop < 3; lop++)
	{
		for (int lop2 = 0; lop2 < 3; lop2++)
		{
			martix.var[lop][lop2] =
				(var[lop][0] * b.var[0][lop2]) +
				(var[lop][1] * b.var[1][lop2]) +
				(var[lop][2] * b.var[2][lop2]);
		}
	}
	return martix;
}

Matrix3 Matrix3::operator*(const float &multi)
{
	Matrix3 tmp = *this;
	tmp.var[0][0] *= multi;
	tmp.var[1][1] *= multi;
	tmp.var[2][2] *= multi;

	return tmp;
}


/****************************
Martix 
4 x 4
**********************/

Matrix4::Matrix4()
{
	SetZero();
}

//ƽ�ƾ���
Matrix4::Matrix4(const Vector3& transition) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = transition.x;
	var[3][1] = transition.y;
	var[3][2] = transition.z;
}

Matrix4::Matrix4(float x, float y, float z) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = x;
	var[3][1] = y;
	var[3][2] = z;
}

//������ת����
Matrix4::Matrix4(char axis, float degree) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	//���û������ת
	if (-0.001f <= degree && degree <= 0.001f) {
		return;
	}

	degree = DEGREE(degree);
	float c = cosf(degree);
	float s = sinf(degree);

	switch (axis)
	{
	case 'x':
	case 'X':
		var[1][1] = c;
		var[2][2] = c;
		var[1][2] = s;
		var[2][1] = -s;
		break;
	case 'y':
	case 'Y':
		var[0][0] = c;
		var[2][2] = c;
		var[2][0] = s;
		var[0][2] = -s;
		break;
	case 'z':
	case 'Z':
		var[0][0] = c;
		var[1][1] = c;
		var[0][1] = s;
		var[1][0] = -s;
		break;
	//Should never reach here
	default:
		break;
	}
}

Matrix4::Matrix4(char ThisUseless, const Vector3 &b) {
	*this = Matrix4('x', b.x)*Matrix4('y', b.y)*Matrix4('z', b.z);
}

//ȫ�������ת����
Matrix4::Matrix4(char useless, float x, float y, float z) {
	*this = Matrix4('x', x)*Matrix4('y', y)*Matrix4('z', z);
}

Matrix4::Matrix4(float scale, Vector3 Rotation, Vector3 Position) {
	*this = Matrix4(scale) * Matrix4('a', Rotation) * Matrix4(Position);
}

Matrix4::Matrix4(Matrix4 &a, const Matrix4 &b) {
	(*this) = a*b;
}

//�Խ��߾���
//���ž���
Matrix4::Matrix4(float input)
{
	SetZero();
	var[0][0] = var[1][1] = var[2][2]  = input;
	var[3][3] = 1.0f;
}

//���ƹ��캯��
Matrix4::Matrix4(const Matrix4 &old)
{
	memcpy(var, old.var, 4 * 4 * sizeof(float));
}

//����ǰ������Ϊƽ�ƾ���
void Matrix4::TransitionMatrix(const Vector3 &transition) {
	//SetZero

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = transition.x;
	var[3][1] = transition.y;
	var[3][2] = transition.z;
}

float Matrix4::Determinant(const Matrix3 &input)
{
	return
		(input.var[0][0] * input.var[1][1] * input.var[2][2]) +
		(input.var[0][1] * input.var[1][2] * input.var[2][0]) +
		(input.var[0][2] * input.var[1][0] * input.var[2][1]) -
		(input.var[0][2] * input.var[1][1] * input.var[2][0]) -
		(input.var[0][1] * input.var[1][0] * input.var[2][2]) -
		(input.var[0][0] * input.var[1][2] * input.var[2][1]);
}

//�������
Matrix4 Matrix4::Invert()
{
	Matrix4 output;
	//���ڴ���ÿ�ε�����ʽ
	Matrix3 tmp;

	//////////////////////////
	// Ϊʲô�ִ��ˣ����� //
	//////////////////////////
	/*
	float firstPart;
	//�����ĸ������
	firstPart =
		(var[0][0] * var[1][1] * var[2][2] * var[3][3]) +
		(var[0][1] * var[1][2] * var[2][3] * var[3][0]) +
		(var[0][2] * var[1][3] * var[2][0] * var[3][1]) +
		(var[0][3] * var[1][0] * var[2][1] * var[3][2]) -
		(var[0][3] * var[1][2] * var[2][1] * var[3][0]) -
		(var[0][2] * var[1][1] * var[2][0] * var[3][3]) -
		(var[0][1] * var[1][0] * var[2][3] * var[3][2]) -
		(var[0][0] * var[1][3] * var[2][2] * var[3][1]);

	//��
	firstPart = 1.0f / firstPart;
	*/

	//������ѭ��
	int x, y;
	//���ڱ�ʶ����ʽ��x y
	int x_tmp, y_tmp;

	for (int lop = 0; lop < 4; lop++) {
		for (int lop2 = 0; lop2 < 4; lop2++) {
			tmp.SetZero();
			x_tmp = 0;
			y_tmp = 0;
			//Ϊoutput�����Ԫ����ֵ
			for (x = 0; x < 4; x++) {
				y_tmp = 0;
				if (x == lop) {
					continue;
				}
				for (y = 0; y < 4; y++) {
					if (y == lop2) {
						continue;
					}
					tmp.var[x_tmp][y_tmp++] = var[x][y];
				}
				x_tmp++;
			}

			output.var[lop2][lop] = Determinant(tmp);

			//��-1
			if ((lop + 1 + lop2 + 1) % 2) {
				output.var[lop2][lop] = (-1.0f)*output.var[lop2][lop];
			}
		}
	}
	//output = output * firstPart;
	*this = output;

	return output;
}

//�������
Matrix4 Matrix4::operator*(const Matrix4 &b) {
	Matrix4 martix;
	for (int lop = 0; lop < 4; lop++) {
		for (int lop2 = 0; lop2 < 4; lop2++) {
			martix.var[lop][lop2] =
				(var[lop][0] * b.var[0][lop2]) +
				(var[lop][1] * b.var[1][lop2]) +
				(var[lop][2] * b.var[2][lop2]) +
				(var[lop][3] * b.var[3][lop2]);
		}
	}
	return martix;
}

//�����������
Matrix4 Matrix4::operator*(const float &multi) {
	Matrix4 tmp = *this;
	tmp.var[0][0] *= multi;
	tmp.var[1][1] *= multi;
	tmp.var[2][2] *= multi;
	tmp.var[3][3] *= multi;
	return tmp;
}