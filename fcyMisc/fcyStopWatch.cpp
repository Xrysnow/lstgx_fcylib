#include "fcyStopWatch.h"
#include "cocos2d.h"

#define QUERY(t) QueryPerformanceCounter((LARGE_INTEGER*)&t)
#define TIME_NOW high_resolution_clock::now().time_since_epoch().count()

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

// On Windows, this is about 3 times faster.
#include <Windows.h>

fcyStopWatch::fcyStopWatch()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_cFreq); // 初始化
	Reset();
}

fcyStopWatch::~fcyStopWatch()
{
}

void fcyStopWatch::Pause()
{
	QUERY(m_cFixStart);
}

void fcyStopWatch::Resume()
{
	uint64_t tNow;
	QUERY(tNow);
	m_cFixAll += tNow - m_cFixStart;
}

void fcyStopWatch::Reset()
{
	m_cFixAll = 0;
	QUERY(m_cLast);
}

double fcyStopWatch::GetElapsed()
{
	uint64_t tNow;
	QUERY(tNow);
	return double(tNow - m_cLast - m_cFixAll)/double(m_cFreq);
}

#else

using namespace std;
using namespace chrono;
auto _t = high_resolution_clock::now();
auto _t1 = _t.time_since_epoch().count();
auto _t2 = time_point_cast<nanoseconds>(_t).time_since_epoch().count();
auto factor = double(nanoseconds::period::num) / nanoseconds::period::den * (double(_t2) / _t1);

fcyStopWatch::fcyStopWatch()
{
	Reset();
}

fcyStopWatch::~fcyStopWatch()
{
}

void fcyStopWatch::Pause()
{
	m_cFixStart = (uint64_t)TIME_NOW;
}

void fcyStopWatch::Resume()
{
	m_cFixAll += (uint64_t)TIME_NOW - m_cFixStart;
}

void fcyStopWatch::Reset()
{
	m_cFixAll = 0;
	m_cLast = (uint64_t)TIME_NOW;
}

double fcyStopWatch::GetElapsed()
{
	return double((uint64_t)TIME_NOW - m_cLast - m_cFixAll)*factor;
}
#endif
