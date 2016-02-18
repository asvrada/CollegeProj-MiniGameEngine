#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H
/*
渲染类
进行数学计算并将生成的图案输出至Canvas

包含类:
画布类
摄像机类

时间管理类
计算每次渲染过去多少时间
*/

////////////
// 包含类 //
////////////
#include "ProjectHeader.h"
#include "CanvasClass.h"
#include "CameraClass.h"
#include "ObjectClass.h"

class TimeClass {
private:
	float sumOfqueue;
	queue<float> qFPS;
	int size;
	wstring InfoFPS;

	//用于计算帧数的东西
	float previousClock;
	float currentClock;
	//上次渲染一帧后过去的时间
	float deltaTime;
public:
	TimeClass() {
		previousClock = currentClock = 0.0f;
		deltaTime = 1.0f / 60;
		size = 60;

		sumOfqueue = size * deltaTime;
		for (int lop = 0; lop < size; lop++) {
			qFPS.push(deltaTime);
		}
	}

	float getDeltaTime() {
		return deltaTime;
	}

	void computeTime() {
		currentClock = (float)clock();
		push((currentClock - previousClock) / CLK_TCK);
		previousClock = currentClock;
	}

	void push(float gap) {
		sumOfqueue -= qFPS.front();
		qFPS.pop();
		sumOfqueue += gap;
		qFPS.push(gap);
	}

	float getFPS() {
		return ((float)size / sumOfqueue);
	}

	wstring getFPSwstring() {
		InfoFPS.clear();

		wstringstream ws;
		ws << TEXT("FPS : ") << getFPS();
		InfoFPS = ws.str();
		return InfoFPS;
	}
};

class RenderClass
{
private:
	//画布类
	CanvasClass *m_Canvas;
	//摄像机类
	CameraClass *m_Camera;
	
	TimeClass fps;

	//提前声明
	//大量用到的计算中间数据
private:
	Matrix4 WorldToView;
	//ViewToHomo 在摄像机类里
	void Draw();
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
	vector<Object> RenderObjects;
};

#endif