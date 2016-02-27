#include "TimeClass.h"

float Time::m_delta_time = 1.0f / 60;
int Time::m_queue_size = 60;
float Time::m_sum_of_queue = m_queue_size * m_delta_time;
wstring Time::m_fps_info = wstring();

inline void Time::m_Push(float gap)
{
	m_sum_of_queue -= m_queue_delta_time.front();
	m_queue_delta_time.pop();
	m_sum_of_queue += gap;
	m_queue_delta_time.push(gap);
}

Time::Time()
{
	m_previous_clock = m_current_clock = 0.0f;

	for (int lop = 0; lop < m_queue_size; lop++) {
		m_queue_delta_time.push(m_delta_time);
	}
}

void Time::ComputeTime()
{
	m_current_clock = (float)clock();
	m_delta_time = (m_current_clock - m_previous_clock) / CLK_TCK;
	m_Push(m_delta_time);
	m_previous_clock = m_current_clock;
}

float Time::GetDeltaTime()
{
	return m_delta_time;
}

wstring Time::GetFPSwstring()
{
	m_fps_info.clear();

	wstringstream ws;
	ws << TEXT("FPS : ") << (float)m_queue_size / m_sum_of_queue;
	m_fps_info = ws.str();
	return m_fps_info;
}
