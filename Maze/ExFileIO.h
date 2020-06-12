/*
* 头文件：ExFileIO.h
* 作用：声明存档系统接口
*/

#pragma once

#include "CMazeCore.h"


void filename(wchar_t* _filename, int size, const Options& _op);

void readFromMapData(ifstream& inf, vector<vector<unsigned char> >& map, MapParam &cfg);

void writeToMapData(ofstream& ouf, const vector<vector<unsigned char> >& map, const MapParam& cfg);


struct Tuple_Playerinfo
{
	wchar_t player[MAX_NAME];
	UINT16 mode;
	unsigned int id;
	bool operator< (const Tuple_Playerinfo& rhs) const
	{
		return wcsncmp(player, rhs.player, MAX_NAME) < 0;
	}
	bool operator==(const Tuple_Playerinfo& rhs) const
	{
		return wcsncmp(player, rhs.player, MAX_NAME) == 0;
	}
	Tuple_Playerinfo() = default;
	Tuple_Playerinfo(const Options& op)
	{
		wcsncpy_s(player, op.username, MAX_NAME);
		mode = 1 << (size_t)(op.mp * MAXDFCTY + op.df);
		id = op.id;
	}
	Tuple_Playerinfo(const wchar_t* name)
	{
		wcsncpy_s(player, name, MAX_NAME);
	}
	Tuple_Playerinfo(const wchar_t* name, MAPTYPE mp, DFCTY df)
	{
		wcsncpy_s(player, name, MAX_NAME);
		mode = 1 << (size_t)(mp * MAXDFCTY + df);
	}
	Tuple_Playerinfo(const wchar_t* name, UINT16 _mode, unsigned int _id)
	{
		wcsncpy_s(player, name, MAX_NAME);
		mode = _mode;
		id = _id;
	}
};

// 返回用于创建新角色的id
int newid();

void readPlayerData(ifstream &input, vector<Tuple_Playerinfo>& data);

// 按player查找playerdata.dat, 若找到，返回id并将对应信息写入info
int searchPlayer(const wchar_t* player, Tuple_Playerinfo& info);

void addPlayerData(const Options& op);

void deletePlayerData(const Options& op);



struct Tuple_Rankinfo
{
	Tuple_Rankinfo() = default;
	Tuple_Rankinfo(const Options& op, const Score& sc) :
		sc(sc), df(op.df), mp(op. mp)
	{
		wcsncpy_s(username, op.username, MAX_NAME);
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		swprintf_s(systime, L"%d/%d/%d %d:%d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);
	}
	bool operator< (const Tuple_Rankinfo& rhs) const
	{
		return sc < rhs.sc;
	}
	wchar_t username[MAX_NAME];
	wchar_t systime[MAX_TIME];
	Score sc;
	DFCTY df;
	MAPTYPE mp;
};

void filename_rank(wchar_t* _filename, int size, int mp, int df);

void readRankData(ifstream& input, vector<Tuple_Rankinfo>& data);

void writeRankData(ofstream& output, const vector<Tuple_Rankinfo>& data);

void addRankData(const Options& op, const Score& sc);