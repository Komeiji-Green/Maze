/*
* cpp�ļ���Timer.cpp
* ���ã���ʱ�����ʵ�֣�_timer�����ڸ��ļ���
*/

#include "ExTimer.h"

// ��ʼ��ʱ
void CTime::StartTime(int t)
{
	base_time = t;
	times = 0;
	pre_times = clock();
	pause = false;
}

// ��ȡ��ǰʱ��
int CTime::GetTime()
{
	if (pause)
		return (times / CLK_TCK) + base_time;
	else
		return ((clock() - pre_times + times) / CLK_TCK) + base_time;
}

// �жϼ�ʱ
void CTime::Pause()
{
	if (pause == true)
		return;
	times += clock() - pre_times;
	pause = true;
}

// ������ʱ
void CTime::Continue()
{
	if (pause == false)
		return;
	pre_times = clock();
	pause = false;
}

// ������ʱ���ۼӵ��ۼ���
void CTime::EndTime()
{
	times += clock() - pre_times;
	sum_times += times;
	times = 0;
}

// ��ȡ�ۼ�ʱ��
int CTime::GetSum()
{
	return sum_times / CLK_TCK;
}

// ����
void CTime::Reset()
{
	times = 0;
	sum_times = 0;
}

CTime _timer;	// ͬһʱ��ֻ������һ����ʱ����ʹ�ã�Ϊ�����������ã�����Ϊ�ⲿ����