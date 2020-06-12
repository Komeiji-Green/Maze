/*
* cpp�ļ���Al-AstarSearch.cpp
* AstarѰ·�㷨��ʵ��
*/

#include "CMazeCore.h"
#include "ExGraph.h"

	#pragma region ͼ��������

// ͼ����
class Graph
{
	friend class Node;

private:
	const CMazeMap& map;
	vector<vector<unsigned char> >& next;
	vector<vector<bool> >visited;	// �ѱ���б�
	vector<vector<int> >cost_from;	// �Գ��������ֵ
	vector<vector<Pos> >pre;		// ǰһ���ڵ㣬�Ա��ҵ�·��
	int m, n, to_r, to_c;
	const Pos dir[4] = { Pos(0, 1), Pos(1, 0), Pos(0, -1), Pos(-1, 0) };

public:
	Graph(const CMazeMap& map, vector<vector<unsigned char> >& next);
	~Graph() {}

	void traceback();
};

class Node
{
private:
	Graph& graph;
	const Pos pos;
	vector<Pos> neighbourlist;

public:
	Node(Graph& graph, const Pos& pos);
	~Node() {}

	Pos getPos();
	const vector<Pos>& neighbour();

	int cost();

	int CostFrom();
	int CostTo();

	bool setCostFrom(int);

	bool vis();
	void pushToVis();

	void setPre(const Node&);
};

#pragma endregion

	#pragma region �㷨����

// ���ȶ��еıȽϷº���
class paircmp
{
public:
	bool operator()(const pair<Pos, int>& lhs, const pair<Pos, int>& rhs)
	{
		return lhs.second > rhs.second;
	}
};

void CMazeMap::AstarSearch()
{
	Graph graph(*this, next); // ����ͼ��ģ��

	priority_queue<pair<Pos, int>, vector<pair<Pos, int> >, paircmp> q; // ���ȶ���

	Node st(graph, Pos(r, c));
	st.setCostFrom(0);
	q.push(pair<Pos, int>(st.getPos(), st.CostFrom() + st.CostTo())); // ��ʼ������

	while (!q.empty())
	{
		Node cur(graph, q.top().first); // ȡ����Ԫ��
		q.pop();
		if (cur.vis())
			continue;
		if (cur.getPos() == Pos(to_r, to_c)) // ����S���ŵ��յ㣬����ѭ��
			break;
		cur.pushToVis();  // ���뵽�ر��б���
		for (auto nextpos : cur.neighbour()) // �������ڵ�
		{
			Node next(graph, nextpos);
			if (next.vis()) // ���������ڵ��ٹر��б�����
				continue;
			int nextcost = next.cost() + cur.CostFrom(); // �������Ȩֵ��
			if (next.setCostFrom(nextcost)) // ��nextcost�������ڵ�ǰֵ������false
			{
				q.push(pair<Pos, int>(nextpos, nextcost + next.CostTo()));
				next.setPre(cur); // ��¼���ڵ�
			}
		}
	}
	graph.traceback(); // ���ݸ��ڵ�����һ��·����
}

#pragma endregion

	#pragma region ͼ����ʵ��

Graph::Graph(const CMazeMap& _map, vector<vector<unsigned char> >& _next) :
	map(_map), next(_next)
{
	m = map.map.size();		// ����
	n = map.map[0].size();	// ����
	to_r = map.to_r;
	to_c = map.to_c;

	vector<vector<unsigned char> >temp;
	swap(temp, next);

	next.resize(m);
	visited.resize(m);
	cost_from.resize(m);
	pre.resize(m);
	for (int i = 0; i < m; i++)
	{
		next[i].resize(n, NULLDIR);
		visited[i].resize(n, false);
		cost_from[i].resize(n, -1);
		pre[i].resize(n, Pos(0, 0));
	}
}

void Graph::traceback()
{
	Pos end(to_r, to_c);
	Pos cur = end;
	Pos prenode;
	while (!((prenode = pre[cur.r][cur.c]) == Pos(0, 0)))
	{
		if (cur == (prenode + Pos(-1, 0)))
			next[prenode.r][prenode.c] = UP;
		else if (cur == (prenode + Pos(1, 0)))
			next[prenode.r][prenode.c] = DOWN;
		else if (cur == (prenode + Pos(0, -1)))
			next[prenode.r][prenode.c] = LEFT;
		else if (cur == (prenode + Pos(0, 1)))
			next[prenode.r][prenode.c] = RIGHT;

		cur = prenode;
	}
}

Node::Node(Graph& _graph, const Pos& _pos) :
	pos(_pos), graph(_graph)
{
	for (int i = 0; i < 4; i++)
	{
		Pos next = pos + graph.dir[i];
		int r = next.r;
		int c = next.c;
		if (r < 0 || r >= graph.m || c < 0 || c >= graph.n)
			continue;
		if (graph.map.map[next.r][next.c] != WALL)
			neighbourlist.push_back(next);
	}
}

Pos Node::getPos()
{
	return pos;
}

const vector<Pos>& Node::neighbour()
{
	return neighbourlist;
}

int Node::cost()
{
	switch (graph.map.map[pos.r][pos.c])
	{
	case BONUS: case END:
		return 1;
		break;
	case ROAD:
		return 10;
		break;
	case BAD:
		return 50;
	}
}

int Node::CostFrom()
{
	return graph.cost_from[pos.r][pos.c];
}

int Node::CostTo()
{
	return 10 * (abs(graph.to_r - pos.r) + abs(graph.to_c - pos.c));
}

bool Node::setCostFrom(int _newcost)
{
	int& curcost = graph.cost_from[pos.r][pos.c];
	if (curcost == -1 || _newcost < curcost)
	{
		curcost = _newcost;
		return true;
	}
	else
		return false;
}

bool Node::vis()
{
	return graph.visited[pos.r][pos.c] == true;
}

void Node::pushToVis()
{
	graph.visited[pos.r][pos.c] = true;
}

void Node::setPre(const Node& pre)
{
	graph.pre[pos.r][pos.c] = pre.pos;
}

#pragma endregion
