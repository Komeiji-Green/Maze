///////////////////////////////////
//
//   头文件：CMazeCore.h
//   作用：游戏核心
//   对应cpp文件：MazeCore.cpp, Al-AstarSearch.cpp 与 Al-MazeGenerator.cpp
//       
//

#pragma once

#include "CObjects.h"

	#pragma region 枚举类型与常量

#define MAX_NAME 50
#define MAX_TIME 20
enum GAME_MODE { ADV = 0, PRAC = 1 };
enum MAPTYPE { FOREST = 0, DUNGEON, MAXMAPTYPE };
enum DFCTY { EASY = 0, NORMAL, HARD, LUNATIC, MAXDFCTY};
extern const wchar_t* Str_Maptype[MAXMAPTYPE];
extern const wchar_t* Str_Dfcty[MAXDFCTY];

enum GAME_OBJ { ROAD = 0, WALL, END, YOU, BONUS, BAD, TOTAL };
extern const int Adv_size[MAXMAPTYPE][MAXDFCTY];

#pragma endregion

	#pragma region 储存信息结构体

// 存储游戏基本信息的结构体
struct Options
{
	wchar_t username[50];
	MAPTYPE mp;
	DFCTY df;
	int id;
	bool load;
	GAME_MODE mode;
};
// 存储分数
struct Score
{
	Score() : Score(0, 0, 0, 0) {}
	Score(int _time, int _bonus, int _treasure, int _bad): 
		time(_time), bonus(_bonus), treasure(_treasure), bad(_bad)
	{
		score = max(bonus * 10 + 10000 - time, 0);
	}
	bool operator< (const Score& rhs) const { return (score > rhs.score); }
	void calc()
	{
		score = max(bonus * 10 + 10000 - time, 0);
	}
	int time;
	int bonus;
	int score;
	int treasure;
	int bad;
};
// 存储地图基本参数
struct MapParam
{
	int m;
	int n;
	int r;
	int c;
	int to_r;
	int to_c;
	int total_treasure;
	Score score;
};

#pragma endregion

	#pragma region 游戏界面与游戏地图

class CGame;
class CMazeMap;


// 游戏主界面，依赖于CGame
class CMazeScene : public CMainWindow
{
private:
	CGame* game = NULL;
	GAME_MODE mode;
	const Options& op;

public:
	CMazeScene(
		CApplication* app, CWindow* parent, Options &op, 
		int m_size = 0, int n_size = 0, int end_r = 0, GAME_MODE _mode = ADV
	);
	virtual ~CMazeScene() {}

	// 响应 W_CREATE 调用 游戏 init 接口 (这个在构造函数里完成）
	// 根据 GAME_MODE 添加按钮： 暂停、保存、退出、自动寻路
	virtual void create();

	// 响应 W_PAINT 刷背景，调用 游戏 paint 接口
	virtual void paint();

	// 响应 W_CLOSE 清缓存，调用 游戏 clear 接口，调用上级close
	virtual void close();

	// 主界面负责接受键盘事件
	// 对于ESC，弹出询问窗口，等同于按退出键
	// 对于WASD，立刻调用 地图与对象 move 接口
	virtual bool eventFilter(Event*);
	
};


// 游戏类（包含信息与接口），依赖于CMazeMap
// 包含参数：用户名，模式（森林、地牢等），难度（冒险模式用），地图规格
// 包含接口：参数的set，init paint clear move save load time
// 包含对象（指针形式）：Map Timer
class CGame
{
	friend class CMazeMap;

private:
	const Options &op;
	Score score;

	int m_size, n_size;
	int r = 1, c = 1;
	int end_r = 0;	// 零表示默认值
	
	CMazeMap* map = NULL;

public:
	CGame(const Options &op): op(op) {}
	~CGame();	// delete map

	// set get方法
	const Options& getop();
	const Score& getsc();

	void setSize(int m, int n);
	void getSize(int& m, int& n);

	void setStart(int _r, int _c);
	void setEnd(int end_r);

	void setTime(int);
	void readTime();

	// 接口
	void init();
	void paint();
	void clear();
	bool move(char);
	void save();
	void load(ifstream&);
	void search();
	void anime(CWindow* wnd);

};

class Pos;
class Edge;

// 游戏地图（抽象类）
class CMazeMap
{
	friend class Graph;
	friend class Node;

protected:
	// 动态申请二维数组略麻烦，这里考虑用 STL vector
	// 在经过一定处理后，二维 vector 的使用完全类似于二维数组
	vector<vector<unsigned char> > map;
	vector<vector<unsigned char> > next;
	int m_map, n_map;	// 一定是奇数
	int r, c, to_r, to_c;
	int treasure = 0, bad = 0;
	COLORREF color[TOTAL];
	MapParam cfg;
	Score& sc;

public:
	CMazeMap(Score&, ifstream& inf);
	CMazeMap(
		Score&,
		int m_size, int n_size,
		int r, int c, 
		int end_r
	);	// resize!!!
	virtual ~CMazeMap() {}

	void putRoom(GAME_MODE mode, MAPTYPE mp);		// 被CGame putRoom调用，绘制地图。
	bool move(char);					// 人物移动
	void anime(CWindow* wnd);			// 播放寻路动画

	virtual void makeMaze() = 0;		// 自动生成迷宫的算法
	void AstarSearch();					// Astar寻路算法

	void setStartEnd(int r, int c, int end_r);

	void save(ofstream &ouf, const Score& sc);

};

// 人间之里地图，继承自CMazeMap
class CForestMap : public CMazeMap
{
public:
	// 附加：设置颜色样式。
	CForestMap(Score&, ifstream& inf);
	CForestMap(Score&, int m_size, int n_size, int r, int c, int end_r);
	void makeMaze();
	void setColor();

protected:
	vector<vector<Pos> > pre; // 并查集使用
	vector<Edge> edges;	// 边集合

	Pos Find(const Pos& cur);
};

// 迷途竹林地图，继承自CMazeMap
class CDungeonMap : public CMazeMap
{
public:
	CDungeonMap(Score&, ifstream& inf);
	CDungeonMap(Score&, int m_size, int n_size, int r, int c, int end_r);
	void makeMaze();
	void setColor();

protected:
	int Generate_times;
	const int Roomsize_min = 5;
	const int Roomsize_max = 11;
	const int Roomgap = 1;	// 房间距离间隔
	const int PtreasureInRoom = 20;
	const int PtreasureInRoad = 250;
	const int Badsize = 10;

	int cur_treasure;
	int cur_bad;
	vector<vector<Pos> > pre;		// 并查集：标记父节点

	enum RANDTYPE { ODD, EVEN };
	Pos randPos(RANDTYPE);			// 随机生成一个点
	bool inMap(const Pos&);			// 判断点是否在地图中
	GAME_OBJ objType(const Pos&);	// 返回点处的区块类型
	void objSet(const Pos&, GAME_OBJ);	// 设置区块
										
	void makeRoom();				// 随机生成房间

	void makeRoad();				// 随机生成道路：算法：floodfill
	void dfsCarve(Pos *);			// floodfill子过程：dfs挖连通区域。

	void makeDoor();				// 随机生成房门：算法：Kruskal求最小生成树
	Pos setFind(const Pos&);		// 并查集：查找代表节点（路径压缩）
	void setUnion(const Pos&, const Pos&);
									// 并查集：合并两节点（路径压缩）

	void randtreasure(const Pos&, int);			
									// 随机生成宝箱
	void makeOther();				// 生成宝箱、陷阱
};

#pragma endregion
