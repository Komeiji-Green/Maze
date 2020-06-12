/*
* 头文件：ExGraph.h
* 作用：定义通用图论模型（节点与边）
* 细化图论模型会在每个算法的cpp文件中定义
*/

#pragma once

enum DIR { UP = 0, LEFT, DOWN, RIGHT, NULLDIR };

// 位置或方位向量
struct Pos
{
	Pos() {}
	Pos(int r, int c) : r(r), c(c) {}
	int r;
	int c;
	bool operator == (const Pos& rhs) const
	{
		return r == rhs.r && c == rhs.c;
	}
	bool operator != (const Pos& rhs) const
	{
		return !(*this == rhs);
	}
	Pos operator+ (const Pos& rhs) const
	{
		return Pos(r + rhs.r, c + rhs.c);
	}
	Pos operator* (int _k) const
	{
		return Pos(r * _k, c * _k);
	}
	Pos operator/ (int _k) const
	{
		if (_k == 0)
			return Pos(0, 0);
		else
			return Pos(r / _k, c / _k);
	}
};

// 边
struct Edge
{
	Edge() {}
	Edge(Pos u, Pos v) : u(u), v(v) {}
	Pos u;
	Pos v;
	void set(int ur, int uc, int vr, int vc)
	{
		u = Pos(ur, uc);
		v = Pos(vr, vc);
	}
};
