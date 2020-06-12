////////////////////////////
//
//  头文件：Maze.h
//
#pragma once

#include <time.h>		// 时间作为随机生成种子
#include <conio.h>		// 获取键盘信息用
#include <vector>		// 使用 STL vector
#include <queue>		// 使用 STL queue
#include <string>		// 使用 STL stirng
#include <algorithm>	// 使用 random_shuffle 函数
#include <graphics.h>	// 引用图形库头文件
#include <Windows.h>	// 为了使用定时器
#include <fstream>		// 文件输入输出	
#include <io.h>
#include <direct.h>		// 用于创建与打开目录
#include "ExTimer.h"

using std::vector;
using std::queue;
using std::wstring;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::streampos;
using std::priority_queue;
using std::pair;

extern CTime _timer;