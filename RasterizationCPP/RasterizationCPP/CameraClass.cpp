#include "CameraClass.h"

#include "InputClass.h"
#include "TimeClass.h"

Camera::Camera(float _screen_aspect,float _fov) {
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
Camera::~Camera()
{
}

void Camera::Update(float _screen_aspect, float _fov) {
	screen_aspect = _screen_aspect;
	fov = _fov;
	m_UpdateViewToHomoMatrix4();
}

void Camera::Update() {
	m_UpdateViewToHomoMatrix4();
}

void Camera::CameraControl() {
	float delta_time = Time::GetDeltaTime();
	if (Input::is_rbutton_up) {
		Input::point_cursor_current = Input::point_cursor_default;
		Input::is_center_snapped = !Input::is_center_snapped;
		ShowCursor(!Input::is_center_snapped);
	}

	POINT pointCursorModify = { 0,0 };
	if (Input::is_center_snapped) {
		pointCursorModify.x = Input::point_cursor_current.x - Input::point_cursor_default.x;
		pointCursorModify.y = Input::point_cursor_current.y - Input::point_cursor_default.y;

		//dead zone
		//pointCursorModify.x /= 

		SetCursorPos((int)Input::point_cursor_default.x, (int)Input::point_cursor_default.y);

		//摄像头上下视角移动
		rotation.x += (float)pointCursorModify.y / 15.0f;
		//摄像头左右视角移动
		rotation.y += (float)pointCursorModify.x / 15.0f;
	}

	if (Input::KeyPressed(VK_UP)) { rotation.x -= rotate_speed * delta_time; }
	if (Input::KeyPressed(VK_DOWN)) { rotation.x += rotate_speed * delta_time; }
	if (Input::KeyPressed(VK_LEFT)) { rotation.y -= rotate_speed * delta_time; }
	if (Input::KeyPressed(VK_RIGHT)) { rotation.y += rotate_speed * delta_time; }

	if (rotation.x < -90.0f) { rotation.x = -90.0f; }
	if (rotation.x > 90.0f) { rotation.x = 90.0f; }
	if (rotation.y < -180.0f) { rotation.y += 360.0f; }
	if (rotation.y > 180.0f) { rotation.y -= 360.0f; }

	//摄像机空间移动

	if (Input::KeyPressed('W') || Input::KeyPressed('A') || Input::KeyPressed('S') || Input::KeyPressed('D') ||
		Input::KeyPressed('Q') || Input::KeyPressed('E'))
	{
		Vector4 MovingDirection;
		if (Input::KeyPressed('A')) { MovingDirection.x = -1.0f; }
		if (Input::KeyPressed('D')) { MovingDirection.x = 1.0f; }
		if (Input::KeyPressed('W')) { MovingDirection.z = 1.0f; }
		if (Input::KeyPressed('S')) { MovingDirection.z = -1.0f; }
		if (Input::KeyPressed('Q')) { MovingDirection.y = 1.0f; }
		if (Input::KeyPressed('E')) { MovingDirection.y = -1.0f; }

		MovingDirection = MovingDirection * (Matrix4('x', rotation.x) * Matrix4('y', rotation.y));
		MovingDirection.VectorUnify();

		position.x += MovingDirection.x*move_speed * delta_time;
		position.y += MovingDirection.y*move_speed * delta_time;
		position.z += MovingDirection.z*move_speed * delta_time;
	}
}

void Camera::m_UpdateViewToHomoMatrix4() {
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

Matrix4 Camera::GetWorldToViewMatrix4()
{
	Matrix4 WTV;
	WTV = Matrix4('A', rotation) * Matrix4(position);
	return WTV.Invert();
}