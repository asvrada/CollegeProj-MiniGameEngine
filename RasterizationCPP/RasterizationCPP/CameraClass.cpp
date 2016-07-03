#include "CameraClass.h"

//为了用 static
#include "WindowFrameClass.h"

Camera::Camera() {
	near_z = 0;
	far_z = 0;
	fov = 0;
	move_speed = 0;
	rotate_speed = 0;

	screen_aspect = 0;

	position.x = position.y = position.z = 0;
	rotation.x = rotation.y = rotation.z = 0;
}

//暂时用不上
Camera::~Camera()
{
}

Camera& Camera::init(float _aspect, float _fov) {
	near_z = 10.0f;
	far_z = 1500.0f;
	fov = _fov;
	move_speed = 150.0f;
	rotate_speed = 70.0f;

	screen_aspect = _aspect;

	position.x = position.y = position.z = 0.0f;
	position.z = -250.0f;
	rotation.x = rotation.y = rotation.z = 0.0f;

	return *this;
}

Camera& Camera::ChangeConfig(float _screen_aspect, float _fov) {
	screen_aspect = _screen_aspect;
	if (_fov > 1.0f)
		fov = _fov;
	m_UpdateViewToHomoMatrix4();

	return *this;
}

Camera& Camera::ChangeConfig() {
	m_UpdateViewToHomoMatrix4();

	return *this;
}

void Camera::CameraControl() {
	float delta_time = Time::GetDeltaTime();
	if (Input::is_rbutton_up) {
		Input::is_center_snapped = !Input::is_center_snapped;
		ShowCursor(!Input::is_center_snapped);
		if (Input::is_center_snapped) {
			//重置鼠标的位置
			Input::point_cursor_current = Input::point_cursor_last_frame = Input::point_cursor_center_snapped;
			SetCursorPos(Input::point_cursor_center_snapped.x, Input::point_cursor_center_snapped.y);
		}
	}

	if (Input::is_center_snapped) {
		POINT pointCursorModify = { Input::point_cursor_current.x - Input::point_cursor_last_frame.x ,
			Input::point_cursor_current.y - Input::point_cursor_last_frame.y };
		Input::point_cursor_last_frame = Input::point_cursor_current;

		//超出范围才重置位置
		//范围为窗口的一半
		//左移右移hack
		if ((abs(Input::point_cursor_current.x - Input::point_cursor_center_snapped.x) > (WindowFrame::rect_client.right >> 2)) ||
			(abs(Input::point_cursor_current.y - Input::point_cursor_center_snapped.y) > (WindowFrame::rect_client.bottom >> 2))) {
			Input::point_cursor_current = Input::point_cursor_last_frame = Input::point_cursor_center_snapped;
			SetCursorPos((int)Input::point_cursor_center_snapped.x, (int)Input::point_cursor_center_snapped.y);
		}

		//摄像头上下视角移动
		rotation.x += (float)pointCursorModify.y / 15.0f;
		//摄像头左右视角移动
		rotation.y += (float)pointCursorModify.x / 15.0f;
	}

	if (Input::KeyPressed(VK_UP)) { rotation.x -= rotate_speed * delta_time * 0.7f; }
	if (Input::KeyPressed(VK_DOWN)) { rotation.x += rotate_speed * delta_time * 0.7f ; }
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

	t = near_z*tanf(DEGREE(fov) / 2.0f);
	b = -t;
	r = t * screen_aspect;
	l = -r;

	view_to_homo.var[0][0] = (2 * near_z) / (r - l);
	view_to_homo.var[1][1] = (2 * near_z) / (t - b);
	//view_to_homo.var[2][0] = (l + r) / (l - r);
	//view_to_homo.var[2][1] = (b + t) / (b - t);
	view_to_homo.var[2][2] = (far_z) / (far_z - near_z);
	view_to_homo.var[2][3] = 1.0f;
	view_to_homo.var[3][2] = (near_z*far_z) / (near_z - far_z);
}

void Camera::update() {
	if (Input::KeyPressed(VK_4)) {
		active = !active;
	}
	if (active) {
		CameraControl();
	}
	
	world_to_view.createRotationMatrix4(rotation).changePosition(position).Invert();
	//world_to_view = (Matrix4('A', rotation) * Matrix4(position)).Invert();
}