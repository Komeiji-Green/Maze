/*
* cpp文件：Al-AstarSearch.cpp
* Astar寻路算法的实现
*/

#include "CMazeCore.h"
#include "ExGraph.h"

	#pragma region 图论类声明

// 图论类
class Graph
{
	friend class Node;

private:
	const CMazeMap& map;
	vector<vector<unsigned char> >& next;
	vector<vector<bool> >visited;	// 已标记列表
	vector<vector<int> >cost_from;	// 自出发点费用值
	vector<vector<Pos> >pre;		// 前一个节点，以便找到路径
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

	#pragma region 算法核心

// 优先队列的比较仿函数
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
	Graph graph(*this, next); // 加载图论模型

	priority_queue<pair<Pos, int>, vector<pair<Pos, int> >, paircmp> q; // 优先队列

	Node st(graph, Pos(r, c));
	st.setCostFrom(0);
	q.push(pair<Pos, int>(st.getPos(), st.CostFrom() + st.CostTo())); // 初始化队列

	while (!q.empty())
	{
		Node cur(graph, q.top().first); // 取出首元素
		q.pop();
		if (cur.vis())
			continue;
		if (cur.getPos() == Pos(to_r, to_c)) // 集合S扩张到终点，跳出循环
			break;
		cur.pushToVis();  // 加入到关闭列表中
		for (auto nextpos : cur.neighbour()) // 遍历相邻点
		{
			Node next(graph, nextpos);
			if (next.vis()) // 不考虑相邻点再关闭列表的情况
				continue;
			int nextcost = next.cost() + cur.CostFrom(); // 计算可能权值和
			if (next.setCostFrom(nextcost)) // 若nextcost并不优于当前值，返回false
			{
				q.push(pair<Pos, int>(nextpos, nextcost + next.CostTo()));
				next.setPre(cur); // 记录父节点
			}
		}
	}
	graph.traceback(); // 回溯父节点生成一条路径。
}

#pragma endregion

	#pragma region 图论类实现

Graph::Graph(const CMazeMap& _map, vector<vector<unsigned char> >& _next) :
	map(_map), next(_next)
{
	m = map.map.size();		// 行数
	n = map.map[0].size();	// 列数
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
