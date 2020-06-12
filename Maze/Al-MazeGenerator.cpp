/*
* cpp文件：迷宫生成算法
* 迷宫生成算法的实现
*/

#include "CMazeCore.h"
#include "ExGraph.h"
using std::random_shuffle;

	#pragma region 迷宫生成V1.0

// 人间之里生成算法
void CForestMap::makeMaze()
{
	// 计算图论模型中点与边的个数
	int m_node = (m_map + 1) / 2;
	int n_node = (n_map + 1) / 2;

	int max_edge = m_node * (n_node - 1) + n_node * (m_node - 1);

	// 初始化图论点集合
	pre.resize(m_node + 1);
	for (int i = 0; i < pre.size(); i++)
	{
		pre[i].resize(n_node + 1);
		for (int j = 1; j < pre[i].size(); j++)
		{
			pre[i][j].r = i;
			pre[i][j].c = j; // 所有点独自成为一个连通区域
		}
	}

	// 初始化图论边集合
	edges.resize(max_edge);

	int cnt = 0;
	for (int i = 1; i <= m_node; i++)
	{
		for (int j = 1; j < n_node; j++)
		{
			edges[cnt++].set(i, j, i, j + 1);
		}
	}

	for (int j = 1; j <= n_node; j++)
	{
		for (int i = 1; i < m_node; i++)
		{
			edges[cnt++].set(i, j, i + 1, j);
		}
	}

	// 随机生成边集合的一个排列
	int rdj;
	Edge temp;
	srand(time(NULL));
	for (int i = edges.size() - 1; i >= 0; i--)
	{
		rdj = rand() % (i + 1);
		temp = edges[rdj];
		edges[rdj] = edges[i];
		edges[i] = temp;
	}

	// Kruskal 算法核心：利用并查集求最小生成树
	Pos fu, fv;
	int ur, uc, vr, vc;
	for (int i = 0; i < edges.size(); i++) // 所有边遍历完毕后，得到原迷宫的一个树
	{
		fu = Find(edges[i].u);
		fv = Find(edges[i].v); // 分别查询集合代表
		if (!(fu == fv)) // 若集合代表不同即所在连通区域不同，合并，拆墙。
		{
			ur = edges[i].u.r;
			uc = edges[i].u.c;
			vr = edges[i].v.r;
			vc = edges[i].v.c;

			pre[fu.r][fu.c] = fv;
			map[2 * ur - 1][2 * uc - 1] = ROAD;
			map[2 * vr - 1][2 * vc - 1] = ROAD;
			map[ur + vr - 1][uc + vc - 1] = ROAD; // 对应于原地图上，边的两端及中点均设置为路
		}
	}
	map[to_r][to_c] = END;
}

// 并查集：查找与路径压缩
Pos CForestMap::Find(const Pos& cur)
{
	int r = cur.r;
	int c = cur.c;
	if (pre[r][c] == cur)
	{
		return pre[r][c];
	}
	else
	{
		return pre[r][c] = Find(pre[r][c]);
	}
}

#pragma endregion

	#pragma region 迷宫生成V2.0

// 迷途竹林生成算法
// 除去最开始的初始化，总共分为四步
// 1.随机生成房间
// 2.利用DFS floodfill，填充房间外的道路
// 3.利用Kruskal最小生成树算法，将全部道路、房间连接为一个连通图
// 4.进一步生成宝箱、随机生成陷阱，此外部分宝箱的生成会连带在1、2中进行
void CDungeonMap::makeMaze()
{
	pre.resize(m_map + 1);
	for (int i = 0; i < pre.size(); i++)
	{
		pre[i].resize(n_map + 1);
		for (int j = 0; j < pre[i].size(); j++)
		{
			pre[i][j] = Pos(i, j);
		}
	}
	int other = (m_map * n_map) / 300 + (m_map + n_map) / 20 + 4;
	cur_treasure = (treasure != 0) ? treasure : (treasure = other);
	cur_bad = (bad != 0) ? bad : (bad = other);

	srand(time(NULL));
	makeRoom();
	makeRoad();
	makeDoor();
	makeOther();
	map[to_r][to_c] = END;
}

// 生成迷宫前需要的一些前置事实：
// 迷宫尺寸只能为奇数*奇数，这样的迷宫有一些很好的性质：
// 1.迷宫中所有横纵坐标均为奇数的点都是路
// 2.迷宫中所有能够成为拐角的墙的横纵坐标均为偶数
// 3.迷宫生成进行完前两步后，	所有行数为奇、列数为偶的墙左右一定是路，上下一定是墙
//							所有行数为偶、列数为奇的墙上下一定是路，左右一定是墙

// 房间墙壁的四角横纵坐标一定均为偶数
// 生成互不重叠的房间，间距可调，房间大小的最值可调
// 在此过程中还会生成金币，金币主要集中生成在房间内
void CDungeonMap::makeRoom()
{
	Generate_times = (m_map * n_map) / 100;
	const int randbound = (Roomsize_max - Roomsize_min) / 2 + 1;

	Pos p1, p2;
	int nrows;		// 行数
	int ncols;		// 列数
	for (int times = 0; times < Generate_times; times++)
	{
		p1 = randPos(EVEN);	// 房间左上墙角
		nrows = (rand() % randbound) * 2 + Roomsize_min + 1;
		ncols = (rand() % randbound) * 2 + Roomsize_min + 1;
		p2 = p1 + Pos(nrows, ncols);

		if (!inMap(p2))	continue;

		bool intersect = false;
		for (int i = p1.r - Roomgap; i <= p2.r + Roomgap && !intersect; i++)
		{
			for (int j = p1.c - Roomgap; j <= p2.c + Roomgap && !intersect; j++)
			{
				if (!inMap(Pos(i, j))) continue;
				intersect = (objType(Pos(i, j)) != WALL);
			}
		}
		if (intersect) continue;

		Pos present = p1 + Pos(1, 1);	// 房间的并查集代表
		for (int i = p1.r + 1; i < p2.r; i++)
		{
			for (int j = p1.c + 1; j < p2.c; j++)
			{
				objSet(Pos(i, j), ROAD);
				randtreasure(Pos(i, j), PtreasureInRoom);

				pre[i][j] = present;
			}
		}
	}
}

// 算法：DFS，floodfill，由一个二重循环与回溯DFS嵌套而成。
// 回溯DFS能够遍历当前点所在的连通区域，在遍历的过程中“日”字型挖墙以保持迷宫的性质。
// 在二重循环结束后，所有的奇数坐标点均会成为路，整个迷宫由若干个连通区域构成。
void CDungeonMap::makeRoad()
{
	for (int i = 1; i <= m_map; i += 2)
	{
		for (int j = 1; j <= n_map; j += 2)
		{
			if (objType(Pos(i, j)) == WALL)	// 因为坐标全奇数，所以一定不是某个房间的墙壁
				dfsCarve(&Pos(i, j));
		}
	}
}

// 回溯DFS挖单连通路
// 回溯条件：若当前点已经是路，则返回。为了节省栈帧，这里将回溯改为忽略法。
void CDungeonMap::dfsCarve(Pos* p)
{
	//！！！！VS里release模式下运行到这里崩溃了？不用担心，只是栈溢出了。
	// 只需要点击项目--属性--链接器--系统--堆栈保留大小--将值设置为10485760即可。
	Pos* dir = new Pos[4];
	dir[0] = Pos(-1, 0); 
	dir[1] = Pos(0, -1);
	dir[2] = Pos(1, 0);
	dir[3] = Pos(0, 1);			// 在堆上开辟方向数组，以节省栈帧。

	random_shuffle(dir, dir + 4);
	for (int i = 0; i < 4; i++)
	{
		Pos next = *p + (dir[i] * 2);

		if (!inMap(next)) continue;             // 如果超出了地图边界，忽略
		if (objType(next) != WALL) continue;	// 如果下一点已经是路，忽略

		objSet(next, ROAD);                   
		randtreasure(next, PtreasureInRoad);
		objSet(*p + dir[i], ROAD);
		randtreasure(*p + dir[i], PtreasureInRoad);  // 挖穿这个方向上连续两个方块，并随机生成金币

		setUnion(*p, *p + dir[i]);
		setUnion(*p + dir[i], next);	// 并查集：将新挖的两个点并入集合

		dfsCarve(&next);	// 递归
	}

	delete[] dir;
}

// 利用Kruskal最小生成树算法，将全部道路、房间连接为一个连通图。
// 将每个连通分支均视为图论中的一个点，两分支间的墙体是一个边（用ExGraph.h中Edge结构体来表示）
// 而根据前文性质，任意两连通分支间的墙的横纵坐标一定是一奇一偶的。
// Kruskal算法：按序（这里是随机）生成所有边（奇偶墙）的一个排列。维护一个边集，一开始为空。
// 若当前边所连接的两点已经属于同一连通分支，则忽略，否则将该边加入边集（也就是拆墙）。
// 最后得到的边集就是原图的一个最小生成树。
// 另外给拆墙的条件增加一些随机性，使得最后的迷宫有环路。
//
// 在此过程中，判断两个点是否属于统一连通分支是利用并查集来实现的。
void CDungeonMap::makeDoor()
{
	const int PbreakWALL = 20;	// 1/20概率随机拆墙

	vector<Edge> edgelist; // 存放全体边（也就是奇偶墙）

	for (int i = 1; i <= m_map; i += 2)
	{
		for (int j = 2; j <= n_map; j += 2)
		{
			if (objType(Pos(i, j)) == WALL)	// 它左右一定是路，上下一定不是路
			{
				edgelist.push_back(Edge(Pos(i, j - 1), Pos(i, j + 1)));
			}
		}
	}

	for (int i = 2; i <= m_map; i += 2)
	{
		for (int j = 1; j <= n_map; j += 2)
		{
			if (objType(Pos(i, j)) == WALL)	// 它上下一定是路，左右一定不是路
			{
				edgelist.push_back(Edge(Pos(i - 1, j), Pos(i + 1, j)));
			}
		}
	}

	random_shuffle(edgelist.begin(), edgelist.end()); // 打乱全部边的顺序

	for (auto edge : edgelist) // Kruskal核心，遍历所有边。
	{
		if (setFind(edge.u) != setFind(edge.v) || (rand() % PbreakWALL) == 0)
		{
			setUnion(edge.u, edge.v);
			objSet((edge.u + edge.v) / 2, ROAD);
		}

	}
}

// 生成一片沼泽的方法：简单的BFS
void CDungeonMap::makeOther()
{
	int max_times = 1000;
	while (cur_treasure > 0 && (max_times--))
	{
		Pos p = randPos(ODD);
		if (objType(p) != BONUS)
		{
			objSet(p, BONUS);
			cur_treasure--;
		}
	}

	Pos dir[4] = { Pos(-1, 0), Pos(0, -1), Pos(1, 0), Pos(0, 1) };
	Pos u;
	while (cur_bad--)
	{
		queue<Pos> q;
		q.push(randPos(ODD));

		int cnt = (rand() % Badsize) + 1;
		while (!q.empty() && cnt > 0)
		{
			u = q.front();
			q.pop();
			if (objType(u) == ROAD)	// 周围全是墙，不用担心越界
			{
				cnt--;
				objSet(u, BAD);
				random_shuffle(dir, dir + 4);
				for (int i = 0; i < 4; i++)
					q.push(u + dir[i]);
			}
		}

	}
}

void CDungeonMap::randtreasure(const Pos& u, int P)
{
	if (cur_treasure > 0 && rand() % P == 0)
	{
		objSet(u, BONUS);
		cur_treasure--;
	}
}

Pos CDungeonMap::randPos(RANDTYPE tp)
{
	if (m_map <= 1 || n_map <= 1)
		return Pos(0, 0);
	if (tp == EVEN)
	{
		return Pos(2 * ((rand() % (m_map / 2)) + 1), 2 * ((rand() % (n_map / 2)) + 1));
	}
	else
	{
		return Pos(2 * (rand() % ((m_map / 2) + 1)) + 1, 2 * (rand() % ((n_map / 2) + 1)) + 1);
	}
}

bool CDungeonMap::inMap(const Pos& u)
{
	return u.r >= 1 && u.r <= m_map && u.c >= 1 && u.c <= n_map;
}

GAME_OBJ CDungeonMap::objType(const Pos& u)
{
	return GAME_OBJ(map[u.r][u.c]);
}

void CDungeonMap::objSet(const Pos& u, GAME_OBJ tp)
{
	map[u.r][u.c] = tp;
}

// 并查集：
// 一种可以快速合并两个集合，判断两点是否属于同一集合的算法
// 并查集介绍：https://blog.csdn.net/niushuai666/article/details/6662911

// 并查集实现

// 得到该点所属集合的代表，在此过程中也进行路径压缩。
Pos CDungeonMap::setFind(const Pos& cur)
{
	int r = cur.r;
	int c = cur.c;
	if (pre[r][c] == cur)
	{
		return pre[r][c];
	}
	else
	{
		return pre[r][c] = setFind(pre[r][c]);
	}
}

void CDungeonMap::setUnion(const Pos& u, const Pos& v)
{
	Pos p_u = setFind(u);
	Pos p_v = setFind(v);
	pre[p_v.r][p_v.c] = p_u;
}

#pragma endregion
