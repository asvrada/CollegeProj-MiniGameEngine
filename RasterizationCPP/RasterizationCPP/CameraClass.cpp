#include "CameraClass.h"

CameraClass::CameraClass(float aspect,float fov) {
	NearZ = 10.0f;
	FarZ = 1500.0f;
	FOV = fov;
	speed = 5.0f;

	screenAspect = aspect;

	Position.x = Position.y = Position.z = 0.0f;
	Rotation.x = Rotation.y = Rotation.z = 0.0f;
}

//暂时用不上
CameraClass::~CameraClass()
{
}

void CameraClass::Update(float newAspect, float fov) {
	if (newAspect > 0.01f) {
		screenAspect = newAspect;
	}
	if (fov > 1.0f) {
		FOV = fov;
	}
	ViewToHomo = GetViewToHomoMatrix4();
}

void CameraClass::Update() {
	ViewToHomo = GetViewToHomoMatrix4();
}

Matrix4 CameraClass::GetWorldToViewMatrix4()
{
	Matrix4 WTV;
	WTV = Matrix4('x', Rotation)*Matrix4(Position);
	return WTV.Invert();
}

Matrix4 CameraClass::GetViewToHomoMatrix4() {
	Matrix4 hMatrix4;

	float l, r, t, b;

	r = NearZ*tanf(FOV *0.01745f / 2.0f);
	l = -r;
	t = r / screenAspect;
	b = -t;

	hMatrix4.var[0][0] = (2 * NearZ) / (r - l);
	hMatrix4.var[1][1] = (2 * NearZ) / (t - b);
	//hMatrix4.var[2][0] = (r + l) / (r - l);
	//hMatrix4.var[2][1] = (t + b) / (t - b);
	hMatrix4.var[2][2] = (FarZ + NearZ) / (FarZ - NearZ);
	hMatrix4.var[2][3] = 1.0f;
	hMatrix4.var[3][2] = -(2.0f*NearZ*FarZ) / (FarZ - NearZ);

	return hMatrix4;
}

