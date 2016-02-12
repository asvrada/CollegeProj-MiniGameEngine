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
	GetViewToHomoMatrix4();
}

void CameraClass::Update() {
	GetViewToHomoMatrix4();
}

Matrix4 CameraClass::GetWorldToViewMatrix4()
{
	Matrix4 WTV;
	WTV = Matrix4('A', Rotation)*Matrix4(Position);
	return WTV.Invert();
}

void CameraClass::GetViewToHomoMatrix4() {
	float l, r, t, b;

	r = NearZ*tanf(DEGREE(FOV) / 2.0f);
	l = -r;
	t = r / screenAspect;
	b = -t;

	ViewToHomo.var[0][0] = (2 * NearZ) / (r - l);
	ViewToHomo.var[1][1] = (2 * NearZ) / (t - b);
	//hMatrix4.var[2][0] = (r + l) / (r - l);
	//hMatrix4.var[2][1] = (t + b) / (t - b);
	ViewToHomo.var[2][2] = (FarZ + NearZ) / (FarZ - NearZ);
	ViewToHomo.var[2][3] = 1.0f;
	ViewToHomo.var[3][2] = -(2.0f*NearZ*FarZ) / (FarZ - NearZ);
}
