#ifndef RASTERIZER_TIMECLASS_H
#define RASTERIZER_TIMECLASS_H

#include "ProjectHeader.h"

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

	//计算每帧的耗时
	void computeTime() {
		currentClock = (float)clock();
		deltaTime = (currentClock - previousClock) / CLK_TCK;
		push(deltaTime);
		previousClock = currentClock;
	}

	void push(float gap) {
		sumOfqueue -= qFPS.front();
		qFPS.pop();
		sumOfqueue += gap;
		qFPS.push(gap);
	}

	//获得当前的帧数
	float getFPS() {
		return ((float)size / sumOfqueue);
	}

	//获得帧数的字符串信息
	wstring getFPSwstring() {
		InfoFPS.clear();

		wstringstream ws;
		ws << TEXT("FPS : ") << getFPS();
		InfoFPS = ws.str();
		return InfoFPS;
	}
};


#endif