/*
* cpp�ļ����Թ������㷨
* �Թ������㷨��ʵ��
*/

#include "CMazeCore.h"
#include "ExGraph.h"
using std::random_shuffle;

	#pragma region �Թ�����V1.0

// �˼�֮�������㷨
void CForestMap::makeMaze()
{
	// ����ͼ��ģ���е���ߵĸ���
	int m_node = (m_map + 1) / 2;
	int n_node = (n_map + 1) / 2;

	int max_edge = m_node * (n_node - 1) + n_node * (m_node - 1);

	// ��ʼ��ͼ�۵㼯��
	pre.resize(m_node + 1);
	for (int i = 0; i < pre.size(); i++)
	{
		pre[i].resize(n_node + 1);
		for (int j = 1; j < pre[i].size(); j++)
		{
			pre[i][j].r = i;
			pre[i][j].c = j; // ���е���Գ�Ϊһ����ͨ����
		}
	}

	// ��ʼ��ͼ�۱߼���
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

	// ������ɱ߼��ϵ�һ������
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

	// Kruskal �㷨���ģ����ò��鼯����С������
	Pos fu, fv;
	int ur, uc, vr, vc;
	for (int i = 0; i < edges.size(); i++) // ���б߱�����Ϻ󣬵õ�ԭ�Թ���һ����
	{
		fu = Find(edges[i].u);
		fv = Find(edges[i].v); // �ֱ��ѯ���ϴ���
		if (!(fu == fv)) // �����ϴ���ͬ��������ͨ����ͬ���ϲ�����ǽ��
		{
			ur = edges[i].u.r;
			uc = edges[i].u.c;
			vr = edges[i].v.r;
			vc = edges[i].v.c;

			pre[fu.r][fu.c] = fv;
			map[2 * ur - 1][2 * uc - 1] = ROAD;
			map[2 * vr - 1][2 * vc - 1] = ROAD;
			map[ur + vr - 1][uc + vc - 1] = ROAD; // ��Ӧ��ԭ��ͼ�ϣ��ߵ����˼��е������Ϊ·
		}
	}
	map[to_r][to_c] = END;
}

// ���鼯��������·��ѹ��
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

	#pragma region �Թ�����V2.0

// ��;���������㷨
// ��ȥ�ʼ�ĳ�ʼ�����ܹ���Ϊ�Ĳ�
// 1.������ɷ���
// 2.����DFS floodfill����䷿����ĵ�·
// 3.����Kruskal��С�������㷨����ȫ����·����������Ϊһ����ͨͼ
// 4.��һ�����ɱ��䡢����������壬���ⲿ�ֱ�������ɻ�������1��2�н���
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

// �����Թ�ǰ��Ҫ��һЩǰ����ʵ��
// �Թ��ߴ�ֻ��Ϊ����*�������������Թ���һЩ�ܺõ����ʣ�
// 1.�Թ������к��������Ϊ�����ĵ㶼��·
// 2.�Թ��������ܹ���Ϊ�սǵ�ǽ�ĺ��������Ϊż��
// 3.�Թ����ɽ�����ǰ������	��������Ϊ�桢����Ϊż��ǽ����һ����·������һ����ǽ
//							��������Ϊż������Ϊ���ǽ����һ����·������һ����ǽ

// ����ǽ�ڵ��ĽǺ�������һ����Ϊż��
// ���ɻ����ص��ķ��䣬���ɵ��������С����ֵ�ɵ�
// �ڴ˹����л������ɽ�ң������Ҫ���������ڷ�����
void CDungeonMap::makeRoom()
{
	Generate_times = (m_map * n_map) / 100;
	const int randbound = (Roomsize_max - Roomsize_min) / 2 + 1;

	Pos p1, p2;
	int nrows;		// ����
	int ncols;		// ����
	for (int times = 0; times < Generate_times; times++)
	{
		p1 = randPos(EVEN);	// ��������ǽ��
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

		Pos present = p1 + Pos(1, 1);	// ����Ĳ��鼯����
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

// �㷨��DFS��floodfill����һ������ѭ�������DFSǶ�׶��ɡ�
// ����DFS�ܹ�������ǰ�����ڵ���ͨ�����ڱ����Ĺ����С��ա�������ǽ�Ա����Թ������ʡ�
// �ڶ���ѭ�����������е��������������Ϊ·�������Թ������ɸ���ͨ���򹹳ɡ�
void CDungeonMap::makeRoad()
{
	for (int i = 1; i <= m_map; i += 2)
	{
		for (int j = 1; j <= n_map; j += 2)
		{
			if (objType(Pos(i, j)) == WALL)	// ��Ϊ����ȫ����������һ������ĳ�������ǽ��
				dfsCarve(&Pos(i, j));
		}
	}
}

// ����DFS�ڵ���ͨ·
// ��������������ǰ���Ѿ���·���򷵻ء�Ϊ�˽�ʡջ֡�����ｫ���ݸ�Ϊ���Է���
void CDungeonMap::dfsCarve(Pos* p)
{
	//��������VS��releaseģʽ�����е���������ˣ����õ��ģ�ֻ��ջ����ˡ�
	// ֻ��Ҫ�����Ŀ--����--������--ϵͳ--��ջ������С--��ֵ����Ϊ10485760���ɡ�
	Pos* dir = new Pos[4];
	dir[0] = Pos(-1, 0); 
	dir[1] = Pos(0, -1);
	dir[2] = Pos(1, 0);
	dir[3] = Pos(0, 1);			// �ڶ��Ͽ��ٷ������飬�Խ�ʡջ֡��

	random_shuffle(dir, dir + 4);
	for (int i = 0; i < 4; i++)
	{
		Pos next = *p + (dir[i] * 2);

		if (!inMap(next)) continue;             // ��������˵�ͼ�߽磬����
		if (objType(next) != WALL) continue;	// �����һ���Ѿ���·������

		objSet(next, ROAD);                   
		randtreasure(next, PtreasureInRoad);
		objSet(*p + dir[i], ROAD);
		randtreasure(*p + dir[i], PtreasureInRoad);  // �ڴ���������������������飬��������ɽ��

		setUnion(*p, *p + dir[i]);
		setUnion(*p + dir[i], next);	// ���鼯�������ڵ������㲢�뼯��

		dfsCarve(&next);	// �ݹ�
	}

	delete[] dir;
}

// ����Kruskal��С�������㷨����ȫ����·����������Ϊһ����ͨͼ��
// ��ÿ����ͨ��֧����Ϊͼ���е�һ���㣬����֧���ǽ����һ���ߣ���ExGraph.h��Edge�ṹ������ʾ��
// ������ǰ�����ʣ���������ͨ��֧���ǽ�ĺ�������һ����һ��һż�ġ�
// Kruskal�㷨������������������������бߣ���żǽ����һ�����С�ά��һ���߼���һ��ʼΪ�ա�
// ����ǰ�������ӵ������Ѿ�����ͬһ��ͨ��֧������ԣ����򽫸ñ߼���߼���Ҳ���ǲ�ǽ����
// ���õ��ı߼�����ԭͼ��һ����С��������
// �������ǽ����������һЩ����ԣ�ʹ�������Թ��л�·��
//
// �ڴ˹����У��ж��������Ƿ�����ͳһ��ͨ��֧�����ò��鼯��ʵ�ֵġ�
void CDungeonMap::makeDoor()
{
	const int PbreakWALL = 20;	// 1/20���������ǽ

	vector<Edge> edgelist; // ���ȫ��ߣ�Ҳ������żǽ��

	for (int i = 1; i <= m_map; i += 2)
	{
		for (int j = 2; j <= n_map; j += 2)
		{
			if (objType(Pos(i, j)) == WALL)	// ������һ����·������һ������·
			{
				edgelist.push_back(Edge(Pos(i, j - 1), Pos(i, j + 1)));
			}
		}
	}

	for (int i = 2; i <= m_map; i += 2)
	{
		for (int j = 1; j <= n_map; j += 2)
		{
			if (objType(Pos(i, j)) == WALL)	// ������һ����·������һ������·
			{
				edgelist.push_back(Edge(Pos(i - 1, j), Pos(i + 1, j)));
			}
		}
	}

	random_shuffle(edgelist.begin(), edgelist.end()); // ����ȫ���ߵ�˳��

	for (auto edge : edgelist) // Kruskal���ģ��������бߡ�
	{
		if (setFind(edge.u) != setFind(edge.v) || (rand() % PbreakWALL) == 0)
		{
			setUnion(edge.u, edge.v);
			objSet((edge.u + edge.v) / 2, ROAD);
		}

	}
}

// ����һƬ����ķ������򵥵�BFS
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
			if (objType(u) == ROAD)	// ��Χȫ��ǽ�����õ���Խ��
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

// ���鼯��
// һ�ֿ��Կ��ٺϲ��������ϣ��ж������Ƿ�����ͬһ���ϵ��㷨
// ���鼯���ܣ�https://blog.csdn.net/niushuai666/article/details/6662911

// ���鼯ʵ��

// �õ��õ��������ϵĴ����ڴ˹�����Ҳ����·��ѹ����
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
