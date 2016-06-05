#ifndef RASTERIZER_CAMERACLASS_H
#define RASTERIZER_CAMERACLASS_H

#include "ProjectHeader.h"

#include "ObjectClass.h"

class Camera :public Object {
	//公有数据成员
public:
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

	//依赖于摄像机的当前位置
	//每帧进行计算
	Matrix4 world_to_view;
	//可以提前算出来
	//仅受高宽比 和 FOV 的影响
	Matrix4 view_to_homo;
private:
	//摄像机坐标转换到齐次剪裁空间坐标
	//仅在摄像机属性变化时重新生成
	void m_UpdateViewToHomoMatrix4();
	//控制，移动摄像机
	void CameraControl();
public:
	//构造函数必须给出高宽比
	Camera();
	~Camera();

	//继承
	void update();
	void init() {
		active = true;
	};

	//不是继承的
	Camera& init(float aspect, float fov);
	//更新数据
	//0代表不变
	Camera& ChangeConfig(float aspect, float fov);
	Camera& ChangeConfig();
};

#endif