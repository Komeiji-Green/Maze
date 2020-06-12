/*
* ͷ�ļ���ExGraph.h
* ���ã�����ͨ��ͼ��ģ�ͣ��ڵ���ߣ�
* ϸ��ͼ��ģ�ͻ���ÿ���㷨��cpp�ļ��ж���
*/

#pragma once

enum DIR { UP = 0, LEFT, DOWN, RIGHT, NULLDIR };

// λ�û�λ����
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

// ��
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
