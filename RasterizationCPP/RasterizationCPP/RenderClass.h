#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H
/*
渲染类
进行数学计算并将生成的图案输出至Canvas

包含类:
画布类
TODO 摄像机类
*/

////////////
// 包含类 //
////////////
#include "ProjectHeader.h"
#include "CanvasClass.h"
#include "CameraClass.h"
#include "ObjectClass.h"

class RenderClass
{
private:
	//画布类
	CanvasClass *m_Canvas;
	//摄像机类
	CameraClass *m_Camera;
public:
	RenderClass();
	~RenderClass();

	void Initialize(RECT,HDC);
	void Shutdown();

	//更改画布类的设置
	void UpdateCanvasSettings(RECT newRect);

	//渲染一帧
	void RenderAFrame();

	//模型数据
public:
	Object Triangle;

//提前声明
//大量用到的计算中间数据
private:
	Matrix4 WorldToView;
	//ViewToHomo 在摄像机类里
};

#endif