#include "CameraClass.h"

CameraClass::CameraClass(float _screen_aspect,float _fov, InputClass *input, TimeClass *time) {
	m_ptr_Input = input;
	m_ptr_time = time;

	near_z = 1.0f;
	far_z = 1500.0f;
	fov = _fov;
	move_speed = 150.0f;
	rotate_speed = 70.0f;

	screen_aspect = _screen_aspect;

	position.x = position.y = position.z = 0.0f;
	position.z = -250.0f;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

//暂时用不上
CameraClass::~CameraClass()
{
}

void CameraClass::Update(float _screen_aspect, float _fov) {
	screen_aspect = _screen_aspect;
	fov = _fov;
	m_UpdateViewToHomoMatrix4();
}

void CameraClass::Update() {
	m_UpdateViewToHomoMatrix4();
}

void CameraClass::CameraControl() {
	if (m_ptr_Input->is_rbutton_up) {
		m_ptr_Input->point_cursor_current = m_ptr_Input->point_cursor_default;
		m_ptr_Input->is_center_snapped = !m_ptr_Input->is_center_snapped;
		ShowCursor(!m_ptr_Input->is_center_snapped);
	}

	POINT pointCursorModify = { 0,0 };
	if (m_ptr_Input->is_center_snapped) {
		pointCursorModify.x = m_ptr_Input->point_cursor_current.x - m_ptr_Input->point_cursor_default.x;
		pointCursorModify.y = m_ptr_Input->point_cursor_current.y - m_ptr_Input->point_cursor_default.y;
		SetCursorPos((int)m_ptr_Input->point_cursor_default.x, (int)m_ptr_Input->point_cursor_default.y);

		//摄像头上下视角移动
		rotation.x += (float)pointCursorModify.y / 15.0f;
		//摄像头左右视角移动
		rotation.y += (float)pointCursorModify.x / 15.0f;
	}

	if (m_ptr_Input->KeyPressed(VK_UP)) { rotation.x -= rotate_speed * m_ptr_time->GetDeltaTime(); }
	if (m_ptr_Input->KeyPressed(VK_DOWN)) { rotation.x += rotate_speed * m_ptr_time->GetDeltaTime(); }
	if (m_ptr_Input->KeyPressed(VK_LEFT)) { rotation.y -= rotate_speed * m_ptr_time->GetDeltaTime(); }
	if (m_ptr_Input->KeyPressed(VK_RIGHT)) { rotation.y += rotate_speed * m_ptr_time->GetDeltaTime(); }

	if (rotation.x < -90.0f) { rotation.x = -90.0f; }
	if (rotation.x > 90.0f) { rotation.x = 90.0f; }
	if (rotation.y < -180.0f) { rotation.y += 360.0f; }
	if (rotation.y > 180.0f) { rotation.y -= 360.0f; }

	//摄像机空间移动

	if (m_ptr_Input->KeyPressed('W') || m_ptr_Input->KeyPressed('A') || m_ptr_Input->KeyPressed('S') || m_ptr_Input->KeyPressed('D') ||
		m_ptr_Input->KeyPressed('Q') || m_ptr_Input->KeyPressed('E'))
	{
		Vector4 MovingDirection;
		if (m_ptr_Input->KeyPressed('A')) { MovingDirection.x = -1.0f; }
		if (m_ptr_Input->KeyPressed('D')) { MovingDirection.x = 1.0f; }
		if (m_ptr_Input->KeyPressed('W')) { MovingDirection.z = 1.0f; }
		if (m_ptr_Input->KeyPressed('S')) { MovingDirection.z = -1.0f; }
		if (m_ptr_Input->KeyPressed('Q')) { MovingDirection.y = 1.0f; }
		if (m_ptr_Input->KeyPressed('E')) { MovingDirection.y = -1.0f; }

		MovingDirection = MovingDirection * (Matrix4('x', rotation.x) * Matrix4('y', rotation.y));
		MovingDirection.VectorUnify();

		position.x += MovingDirection.x*move_speed * m_ptr_time->GetDeltaTime();
		position.y += MovingDirection.y*move_speed * m_ptr_time->GetDeltaTime();
		position.z += MovingDirection.z*move_speed * m_ptr_time->GetDeltaTime();
	}
}

void CameraClass::m_UpdateViewToHomoMatrix4() {
	float l, r, t, b;

	r = near_z*tanf(DEGREE(fov) / 2.0f);
	l = -r;
	t = r / screen_aspect;
	b = -t;

	view_to_homo.var[0][0] = (2 * near_z) / (r - l);
	view_to_homo.var[1][1] = (2 * near_z) / (t - b);
	//hMatrix4.var[2][0] = (r + l) / (r - l);
	//hMatrix4.var[2][1] = (t + b) / (t - b);
	view_to_homo.var[2][2] = (far_z + near_z) / (far_z - near_z);
	view_to_homo.var[2][3] = 1.0f;
	view_to_homo.var[3][2] = -(2.0f*near_z*far_z) / (far_z - near_z);
}

Matrix4 CameraClass::GetWorldToViewMatrix4()
{
	Matrix4 WTV;
	WTV = Matrix4('A', rotation)*Matrix4(position);
	return WTV.Invert();
}