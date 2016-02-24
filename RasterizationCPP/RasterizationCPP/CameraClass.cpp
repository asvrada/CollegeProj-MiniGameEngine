#include "CameraClass.h"

CameraClass::CameraClass(float aspect,float fov, InputClass *input) {
	m_ptr_Input = input;

	NearZ = 1.0f;
	FarZ = 1500.0f;
	FOV = fov;
	moveSpeed = 1.0f;
	rotateSpeed = 0.5f;

	screenAspect = aspect;

	Position.x = Position.y = Position.z = 0.0f;
	Position.z = -250.0f;
	Rotation.x = Rotation.y = Rotation.z = 0.0f;
}

//暂时用不上
CameraClass::~CameraClass()
{
}

void CameraClass::Update(float newAspect, float fov) {
	screenAspect = newAspect;
	FOV = fov;
	GetViewToHomoMatrix4();
}

void CameraClass::Update() {
	GetViewToHomoMatrix4();
}

void CameraClass::CameraControl() {
	if (m_ptr_Input->rButtonUp) {
		m_ptr_Input->rectCursor = m_ptr_Input->rectCursorCenterPostion;
		m_ptr_Input->isCenterSnapped = !m_ptr_Input->isCenterSnapped;
		ShowCursor(!m_ptr_Input->isCenterSnapped);
	}

	POINT pointCursorModify = { 0,0 };
	if (m_ptr_Input->isCenterSnapped) {
		pointCursorModify.x = m_ptr_Input->rectCursor.x - m_ptr_Input->rectCursorCenterPostion.x;
		pointCursorModify.y = m_ptr_Input->rectCursor.y - m_ptr_Input->rectCursorCenterPostion.y;
		SetCursorPos((int)m_ptr_Input->rectCursorCenterPostion.x, (int)m_ptr_Input->rectCursorCenterPostion.y);

		//摄像头上下视角移动
		Rotation.x += (float)pointCursorModify.y / 15.0f;
		//摄像头左右视角移动
		Rotation.y += (float)pointCursorModify.x / 15.0f;
	}

	if (m_ptr_Input->IsKeyPressed(VK_UP)) { Rotation.x -= rotateSpeed; }
	if (m_ptr_Input->IsKeyPressed(VK_DOWN)) { Rotation.x += rotateSpeed; }
	if (m_ptr_Input->IsKeyPressed(VK_LEFT)) { Rotation.y -= rotateSpeed; }
	if (m_ptr_Input->IsKeyPressed(VK_RIGHT)) { Rotation.y += rotateSpeed; }

	if (Rotation.x < -90.0f) { Rotation.x = -90.0f; }
	if (Rotation.x > 90.0f) { Rotation.x = 90.0f; }
	if (Rotation.y < -180.0f) { Rotation.y += 360.0f; }
	if (Rotation.y > 180.0f) { Rotation.y -= 360.0f; }

	//摄像机空间移动

	if (m_ptr_Input->IsKeyPressed('W') || m_ptr_Input->IsKeyPressed('A') || m_ptr_Input->IsKeyPressed('S') || m_ptr_Input->IsKeyPressed('D') ||
		m_ptr_Input->IsKeyPressed('Q') || m_ptr_Input->IsKeyPressed('E'))
	{
		Vector4 MovingDirection;
		if (m_ptr_Input->IsKeyPressed('A')) { MovingDirection.x = -1.0f; }
		if (m_ptr_Input->IsKeyPressed('D')) { MovingDirection.x = 1.0f; }
		if (m_ptr_Input->IsKeyPressed('W')) { MovingDirection.z = 1.0f; }
		if (m_ptr_Input->IsKeyPressed('S')) { MovingDirection.z = -1.0f; }
		if (m_ptr_Input->IsKeyPressed('Q')) { MovingDirection.y = 1.0f; }
		if (m_ptr_Input->IsKeyPressed('E')) { MovingDirection.y = -1.0f; }

		MovingDirection = MovingDirection * (Matrix4('x', Rotation.x) * Matrix4('y', Rotation.y));
		MovingDirection.VectorUnify();

		Position.x += MovingDirection.x*moveSpeed;
		Position.y += MovingDirection.y*moveSpeed;
		Position.z += MovingDirection.z*moveSpeed;
	}
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

Matrix4 CameraClass::GetWorldToViewMatrix4()
{
	Matrix4 WTV;
	WTV = Matrix4('A', Rotation)*Matrix4(Position);
	return WTV.Invert();
}