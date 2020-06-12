/*
* cpp文件：Timer.cpp
* 作用：计时器类的实现，_timer定义在该文件中
*/

#include "ExTimer.h"

// 开始计时
void CTime::StartTime(int t)
{
	base_time = t;
	times = 0;
	pre_times = clock();
	pause = false;
}

// 获取当前时间
int CTime::GetTime()
{
	if (pause)
		return (times / CLK_TCK) + base_time;
	else
		return ((clock() - pre_times + times) / CLK_TCK) + base_time;
}

// 中断计时
void CTime::Pause()
{
	if (pause == true)
		return;
	times += clock() - pre_times;
	pause = true;
}

// 继续计时
void CTime::Continue()
{
	if (pause == false)
		return;
	pre_times = clock();
	pause = false;
}

// 结束计时，累加到累加器
void CTime::EndTime()
{
	times += clock() - pre_times;
	sum_times += times;
	times = 0;
}

// 获取累加时间
int CTime::GetSum()
{
	return sum_times / CLK_TCK;
}

// 结束
void CTime::Reset()
{
	times = 0;
	sum_times = 0;
}

CTime _timer;	// 同一时间只可能有一个计时器在使用，为方便管理与调用，定义为外部变量