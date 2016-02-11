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

#include <ctime>
using std::clock;
#include <queue>
using std::queue;

class fpsClass {
private:
	float sumOfqueue;
	queue<float> qFPS;
	int size;
	wchar_t charFPS[30];

	//用于计算帧数的东西
	float previousClock;
	float currentClock;
	float previousRenderTime;
public:
	fpsClass() {
		previousClock = currentClock = previousRenderTime = 0.0f;

		memset(charFPS, 0, sizeof(charFPS));
		sumOfqueue = 0.01f;
		size = 60;
	}

	void computeTime() {
		currentClock = (float)clock();
		push((currentClock - previousClock) / CLK_TCK);
		previousClock = currentClock;
	}

	void push(float gap) {
		if ((int)qFPS.size() > size) {
			sumOfqueue -= qFPS.front();
			qFPS.pop();
		}
		sumOfqueue += gap;
		qFPS.push(gap);
	}

	int getFPS() {
		return (int)((float)size / sumOfqueue);
	}

	wchar_t* getFPSInfo() {
		wsprintf(charFPS, TEXT("FPS : %3d"), getFPS());
		return charFPS;
	}
};

class RenderClass
{
private:
	//画布类
	CanvasClass *m_Canvas;
	//摄像机类
	CameraClass *m_Camera;
	
	fpsClass fps;
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