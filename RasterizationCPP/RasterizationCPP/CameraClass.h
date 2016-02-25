#ifndef RASTERIZER_CAMERACLASS_H
#define RASTERIZER_CAMERACLASS_H

#include "ProjectHeader.h"
#include "InputClass.h"
#include "TimeClass.h"

class CameraClass {
//公有数据成员
public:
	//坐标
	Vector3 position;

	//绕各个轴旋转的角度
	//x -> pitch
	//y -> yaw
	//z -> roll
	Vector3 rotation;

	//最近的距离，最远显示的距离
	float near_z, far_z;
	//Field of View
	float fov;

	//屏幕高宽比
	//根据画布分辨率设置
	float screen_aspect;

	//摄像机移动速度
	float move_speed;
	float rotate_speed;

	//可以提前算出来
	//仅受高宽比 和 FOV 的影响
	Matrix4 view_to_homo;
private:
	TimeClass *m_ptr_time;
	InputClass *m_ptr_Input;
	//摄像机坐标转换到齐次剪彩空间坐标
	//仅在摄像机属性变化时重新生成
	//不包括位置和旋转的变化
	void m_UpdateViewToHomoMatrix4();
public:
	//必须给出高宽比
	CameraClass(float aspect, float fov, InputClass *, TimeClass*);
	~CameraClass();

	//更新数据
	//0代表不变
	void Update(float aspect, float fov);
	void Update();

	void CameraControl();
	//世界坐标转换到摄像机坐标
	//即视口坐标
	Matrix4 GetWorldToViewMatrix4();
};

#endif