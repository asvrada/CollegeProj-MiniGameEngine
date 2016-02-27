#ifndef RASTERIZER_TIMECLASS_H
#define RASTERIZER_TIMECLASS_H

#include "ProjectHeader.h"

class Time {
private:
	queue<float> m_queue_delta_time;

	//用于计算帧数的东西
	float m_previous_clock;
	float m_current_clock;

	///////////
	// static //
	//////////
	static wstring m_fps_info;
	//上次渲染一帧后过去的时间
	static float m_delta_time;
	static int m_queue_size;
	static float m_sum_of_queue;

private:
	void m_Push(float gap);

public:
	Time();

	//计算每帧的耗时
	void ComputeTime();

	static float GetDeltaTime();

	//获得帧数的字符串信息
	static wstring GetFPSwstring();
};

#endif