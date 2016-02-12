#ifndef RASTERIZER_CAMERACLASS_H
#define RASTERIZER_CAMERACLASS_H

#include "ProjectHeader.h"

class CameraClass {
//公有数据成员
public:
	//坐标
	Vector3 Position;

	Vector3 Rotation;
	//x -> pitch
	//y -> yaw
	//z -> roll

	//最近的距离，最远显示的距离
	float NearZ, FarZ;
	//Field of View
	float FOV;

	//屏幕高宽比
	//根据画布分辨率设置
	float screenAspect;

	//摄像机移动速度
	float speed;

	//可以提前算出来
	//仅受高宽比 和 FOV 的影响
	Matrix4 ViewToHomo;
private:
	//摄像机坐标转换到齐次剪彩空间坐标
	//仅在摄像机属性变化时重新生成
	//不包括位置和旋转的变化
	void GetViewToHomoMatrix4();
public:
	//必须给出高宽比
	CameraClass(float aspect, float fov);
	~CameraClass();

	//更新数据
	//0代表不变
	void Update(float newAspect, float newFOV);
	void Update();

	//世界坐标转换到摄像机坐标
	//即视口坐标
	Matrix4 GetWorldToViewMatrix4();
};

#endif