///////////////////////////////////
//
//   ͷ�ļ���CMazeCore.h
//   ���ã���Ϸ����
//   ��Ӧcpp�ļ���MazeCore.cpp, Al-AstarSearch.cpp �� Al-MazeGenerator.cpp
//       
//

#pragma once

#include "CObjects.h"

	#pragma region ö�������볣��

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

	#pragma region ������Ϣ�ṹ��

// �洢��Ϸ������Ϣ�Ľṹ��
struct Options
{
	wchar_t username[50];
	MAPTYPE mp;
	DFCTY df;
	int id;
	bool load;
	GAME_MODE mode;
};
// �洢����
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
// �洢��ͼ��������
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

	#pragma region ��Ϸ��������Ϸ��ͼ

class CGame;
class CMazeMap;


// ��Ϸ�����棬������CGame
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

	// ��Ӧ W_CREATE ���� ��Ϸ init �ӿ� (����ڹ��캯������ɣ�
	// ���� GAME_MODE ��Ӱ�ť�� ��ͣ�����桢�˳����Զ�Ѱ·
	virtual void create();

	// ��Ӧ W_PAINT ˢ���������� ��Ϸ paint �ӿ�
	virtual void paint();

	// ��Ӧ W_CLOSE �建�棬���� ��Ϸ clear �ӿڣ������ϼ�close
	virtual void close();

	// �����渺����ܼ����¼�
	// ����ESC������ѯ�ʴ��ڣ���ͬ�ڰ��˳���
	// ����WASD�����̵��� ��ͼ����� move �ӿ�
	virtual bool eventFilter(Event*);
	
};


// ��Ϸ�ࣨ������Ϣ��ӿڣ���������CMazeMap
// �����������û�����ģʽ��ɭ�֡����εȣ����Ѷȣ�ð��ģʽ�ã�����ͼ���
// �����ӿڣ�������set��init paint clear move save load time
// ��������ָ����ʽ����Map Timer
class CGame
{
	friend class CMazeMap;

private:
	const Options &op;
	Score score;

	int m_size, n_size;
	int r = 1, c = 1;
	int end_r = 0;	// ���ʾĬ��ֵ
	
	CMazeMap* map = NULL;

public:
	CGame(const Options &op): op(op) {}
	~CGame();	// delete map

	// set get����
	const Options& getop();
	const Score& getsc();

	void setSize(int m, int n);
	void getSize(int& m, int& n);

	void setStart(int _r, int _c);
	void setEnd(int end_r);

	void setTime(int);
	void readTime();

	// �ӿ�
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

// ��Ϸ��ͼ�������ࣩ
class CMazeMap
{
	friend class Graph;
	friend class Node;

protected:
	// ��̬�����ά�������鷳�����￼���� STL vector
	// �ھ���һ������󣬶�ά vector ��ʹ����ȫ�����ڶ�ά����
	vector<vector<unsigned char> > map;
	vector<vector<unsigned char> > next;
	int m_map, n_map;	// һ��������
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

	void putRoom(GAME_MODE mode, MAPTYPE mp);		// ��CGame putRoom���ã����Ƶ�ͼ��
	bool move(char);					// �����ƶ�
	void anime(CWindow* wnd);			// ����Ѱ·����

	virtual void makeMaze() = 0;		// �Զ������Թ����㷨
	void AstarSearch();					// AstarѰ·�㷨

	void setStartEnd(int r, int c, int end_r);

	void save(ofstream &ouf, const Score& sc);

};

// �˼�֮���ͼ���̳���CMazeMap
class CForestMap : public CMazeMap
{
public:
	// ���ӣ�������ɫ��ʽ��
	CForestMap(Score&, ifstream& inf);
	CForestMap(Score&, int m_size, int n_size, int r, int c, int end_r);
	void makeMaze();
	void setColor();

protected:
	vector<vector<Pos> > pre; // ���鼯ʹ��
	vector<Edge> edges;	// �߼���

	Pos Find(const Pos& cur);
};

// ��;���ֵ�ͼ���̳���CMazeMap
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
	const int Roomgap = 1;	// ���������
	const int PtreasureInRoom = 20;
	const int PtreasureInRoad = 250;
	const int Badsize = 10;

	int cur_treasure;
	int cur_bad;
	vector<vector<Pos> > pre;		// ���鼯����Ǹ��ڵ�

	enum RANDTYPE { ODD, EVEN };
	Pos randPos(RANDTYPE);			// �������һ����
	bool inMap(const Pos&);			// �жϵ��Ƿ��ڵ�ͼ��
	GAME_OBJ objType(const Pos&);	// ���ص㴦����������
	void objSet(const Pos&, GAME_OBJ);	// ��������
										
	void makeRoom();				// ������ɷ���

	void makeRoad();				// ������ɵ�·���㷨��floodfill
	void dfsCarve(Pos *);			// floodfill�ӹ��̣�dfs����ͨ����

	void makeDoor();				// ������ɷ��ţ��㷨��Kruskal����С������
	Pos setFind(const Pos&);		// ���鼯�����Ҵ���ڵ㣨·��ѹ����
	void setUnion(const Pos&, const Pos&);
									// ���鼯���ϲ����ڵ㣨·��ѹ����

	void randtreasure(const Pos&, int);			
									// ������ɱ���
	void makeOther();				// ���ɱ��䡢����
};

#pragma endregion
