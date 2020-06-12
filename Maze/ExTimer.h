/*
* 头文件：ExTimer.h
* 计时器类原型
* 对应cpp文件：Timer.cpp
*/
#pragma once

#include <time.h>

class CTime
{
public:
	CTime() {}

	void StartTime(int t = 0);
	int GetTime();
	void Pause();
	void Continue();
	void EndTime();
	int GetSum();
	void Reset();

private:
	clock_t times = 0;
	clock_t pre_times;
	clock_t sum_times = 0;
	int base_time = 0;
	bool pause = false;

};