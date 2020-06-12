///////////////////////////////////
//
//   cpp文件：MazeCore.cpp
//   作用：游戏核心的实现，算法核心的实现在算法cpp文件中
//   对应头文件：CMazeCore.h
//

#include "CGameWidgets.h"
#include "ExFileIO.h"
#include "ExGraph.h"

	#pragma region 常量定义
const wchar_t* Str_Maptype[MAXMAPTYPE] = { L"人间之里", L"迷途竹林" };
const wchar_t* Str_Dfcty[MAXDFCTY] = { L"Easy", L"Normal", L"Hard", L"Lunatic" };
const int Adv_size[MAXMAPTYPE][MAXDFCTY] = {
	{ 25, 37, 67, 99 },
	{ 25, 55, 99, 175 }
};
#pragma endregion

	#pragma region CMazeScence
// 构造函数 由冒险模式调用
CMazeScene::CMazeScene(
	CApplication* app, CWindow* parent, Options &op, 
	int m_size, int n_size, int end_r, GAME_MODE _mode
):
	CMainWindow(app, parent), mode(_mode), op(op)
{
	game = new CGame(op);
	if (mode == ADV)
	{
		// 冒险模式
		if (op.load) // 读档
		{
			wchar_t file[MAX_PATH];
			filename(file, MAX_PATH, op);

			if (_access("mazesave", 0) == -1)	// 若文件夹不存在，创建文件夹
				_mkdir("mazesave");

			ifstream inf(file, ifstream::binary);

			if (inf)
			{
				game->load(inf);
			}
			else
			{
				op.load = false;	// 若文件打开失败，回到自动生成模式
			}

			inf.close();
		}
		if (!op.load) // 不读档
		{
			int sz = Adv_size[op.mp][op.df];
			game->setSize(sz, sz);
			game->setEnd(rand() % sz + 1);
			(rand() % 2 == 0) ? game->setStart(1, 1) : game->setStart(sz, 1);
			game->setTime(0);

			game->init();
		}
	}
	else
	{
		// 练习模式
		game->setSize(m_size, n_size);
		game->setEnd(end_r);
		game->setStart(rand() % m_size + 1, rand() % n_size + 1);
		game->setTime(0);

		game->init();
	}
}

void CMazeScene::create()
{
	// 暂停按钮
	CButton* btPause = new BtSuspend(mainApp, this);
	if (mode == ADV)
		btPause->setPattern(513, 280, L"　暂停　");
	else
		btPause->setPattern(513, 220, L"　暂停　");
	btPause->setBtColor(RGB(100, 100, 100));
	
	if (mode == ADV)
	{
		// 保存按钮
		CButton* btSave = new BtSave(mainApp, this, game);
		btSave->setPattern(513, 340, L"　保存　");
		btSave->setBtColor(RGB(100, 100, 100));
	}

	if (mode == PRAC)
	{
		// 自动寻路
		CButton* btAstar = new BtAstar(mainApp, this, game);
		btAstar->setPattern(513, 340, L"　寻路　");
		btAstar->setBtColor(RGB(100, 100, 100));

		CButton* btAnime = new BtAnime(mainApp, this, game);
		btAnime->setPattern(513, 280, L"　演示　");
		btAnime->setBtColor(RGB(100, 100, 100));
	}

	// 退出按钮
	CButton* btLoad = new BtClose(mainApp, this);
	btLoad->setPattern(513, 400, L"　退出　");
	btLoad->setBtColor(RGB(100, 100, 100));
}

void CMazeScene::paint()
{
	setbkcolor(RGB(50, 50, 50));
	cleardevice();

	setfillcolor(RGB(100, 100, 100));
	solidrectangle(16, 17, 465, 465);	// 绘制游戏区

	wchar_t tim[25];

	swprintf_s(tim, L"使用时间 %d s\0", int(_timer.GetTime()));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	f.lfWidth = 0;
	wcscpy_s(f.lfFaceName, L"黑体");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);
	if (mode == ADV)
	{
		outtextxy(480, 25, Str_Maptype[op.mp]);
		outtextxy(480, 55, Str_Dfcty[op.df]);
		outtextxy(480, 115, tim);				// 输出文字
	}
	else
	{
		outtextxy(480, 25, Str_Maptype[op.mp]);
		outtextxy(480, 85, tim);
	}

	if(game != NULL)
		game->paint();				// 绘制迷宫, 以及相关文字
}

void CMazeScene::close()
{
	delete game;
	game = NULL;
	parent->refresh();
	CMainWindow::close();
}

bool CMazeScene::eventFilter(Event* ev)
{
	char ch;
	if (ev->type() == K_PRESS)
	{
		ch = ev->getch();
		switch (ch)
		{
		case 'w': case 'a': case 's': case 'd':
			if (game != NULL)
			{
				mainApp->PushMsg(new Msg(W_PAINT, this));
				if (game->move(ch))		// 到达End
				{
					CDialog* n_dlg = new DlgEnd(mainApp, this, game, mode);

					wchar_t* text[10];
					text[0] = new wchar_t[9];
					wcscpy_s(text[0], 9, L"是否继续游戏？\n");
					n_dlg->setPattern(170, 165, 300, 150, L"已到达终点", text, 1);		// 设置对话框样式

					mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// 过滤器的绑定在Create时才进行
					_timer.Pause();	// 中断计时器
					game->readTime();
					
				}
				return true;
			}
			break;
		// Esc
		}
	}
	return CMainWindow::eventFilter(ev);
}
#pragma endregion

	#pragma region CGame Set Get
// CGame Set Get 方法
const Options& CGame::getop()
{
	return op;
}

const Score& CGame::getsc()
{
	return score;
}

void CGame::setSize(int m, int n)
{
	m_size = m;
	n_size = n;
}

void CGame::getSize(int& m, int& n)
{
	m = m_size;
	n = n_size;
}

void CGame::setStart(int _r, int _c)
{
	r = _r;
	c = _c;
}

void CGame::setEnd(int __end_r)
{
	end_r = __end_r;
}

void CGame::setTime(int t)
{
	score.time = t;
}

void CGame::readTime()
{
	score.time = _timer.GetTime();
}
#pragma endregion

	#pragma region CGame 清空与析构
// CGame 析构函数
CGame::~CGame()
{
	if (map != NULL)
		delete map;
}

void CGame::clear()
{
	if (map != NULL)
		delete map;
	map = NULL;
}
#pragma endregion

	#pragma region 初始化 存档与读档
// 迷宫初始化或存档读档

void CGame::init()
{
	switch (op.mp)
	{
	case FOREST:
		map = new CForestMap(score, m_size, n_size, r, c, end_r);
		map->makeMaze();	// 至此，游戏开始
		break;
	case DUNGEON:
		map = new CDungeonMap(score, m_size, n_size, r, c, end_r);
		map->makeMaze();
		break;
	}
	_timer.StartTime();
}

void CGame::save()
{
	wchar_t file[MAX_PATH];
	filename(file, MAX_PATH, op);

	if (_access("mazesave", 0) == -1)	// 若文件夹不存在，创建文件夹
		_mkdir("mazesave");

	ofstream ouf(file, ofstream::binary);

	if (ouf)
	{
		map->save(ouf, score);
		addPlayerData(op);
	}

	ouf.close();
}

void CGame::load(ifstream& inf)
{
	switch (op.mp)
	{
	case FOREST:
		map = new CForestMap(score, inf);
		// 至此，游戏开始
		break;
	case DUNGEON:
		map = new CDungeonMap(score, inf);
		break;
	}
}

void CMazeMap::save(ofstream&ouf, const Score& sc)
{
	cfg.m = m_map + 2;
	cfg.n = n_map + 3;
	cfg.r = r;
	cfg.c = c;
	cfg.to_r = to_r;
	cfg.to_c = to_c;
	cfg.total_treasure = treasure;

	cfg.score.time = _timer.GetTime();
	cfg.score.calc();

	writeToMapData(ouf, map, cfg);
}
#pragma endregion

	#pragma region 迷宫地图构造函数
// 迷宫地图构造函数

CMazeMap::CMazeMap(Score& sc, ifstream& inf):
	sc(sc)
{
	readFromMapData(inf, map, cfg);
	_timer.StartTime(cfg.score.time);
	
	r = cfg.r;
	c = cfg.c;
	m_map = cfg.m - 2;
	n_map = cfg.n - 3;
	to_r = cfg.to_r;
	to_c = cfg.to_c;
	treasure = cfg.total_treasure;
}

CMazeMap::CMazeMap(Score& sc, int m_size, int n_size, int r, int c, int end_r):
	sc(sc)
{
	m_map = m_size;
	n_map = n_size;

	// 设置迷宫大小
	map.resize(m_map + 2);
	for (int i = 0; i < map.size(); i++)
	{
		// 左一堵墙、右两堵墙（最右墙不显示，用于防止出口处移动操作导致的越界）
		map[i].resize(n_map + 3, WALL);
	}

	setStartEnd(r, c, end_r);	// 设置起始点
	map[to_r][to_c] = END;
}

CForestMap::CForestMap(Score& sc, ifstream& inf) :
	CMazeMap(sc, inf)
{
	setColor();
}

CForestMap::CForestMap(Score& sc, int m_size, int n_size, int r, int c, int end_r) :
	CMazeMap(sc, m_size, n_size, r, c, end_r)
{
	setColor();
}

CDungeonMap::CDungeonMap(Score& sc, ifstream& inf) :
	CMazeMap(sc, inf)
{
	setColor();
}

CDungeonMap::CDungeonMap(Score& sc, int m_size, int n_size, int r, int c, int end_r):
	CMazeMap(sc, m_size, n_size, r, c, end_r)
{
	setColor();
}

void CForestMap::setColor()
{
	color[WALL] = RGB(0, 100, 0);
	color[ROAD] = BROWN;
	color[END] = BLUE;
	color[YOU] = YELLOW;
	color[BONUS] = RGB(254, 254, 65);
	color[BAD] = BLACK;
}

void CDungeonMap::setColor()
{
	color[WALL] = RGB(0, 100, 0);
	color[ROAD] = BROWN;
	color[END] = BLUE;
	color[YOU] = YELLOW;
	color[BONUS] = RGB(254, 254, 65);
	color[BAD] = BLACK;
}

#pragma endregion

	#pragma region 人物移动 重绘
// 人物移动、重绘等响应函数

bool CGame::move(char ch)
{
	if (map != NULL)
		return map->move(ch);
	else
		return false;
}

bool CMazeMap::move(char ch)	// 应该去记录移动前的区块，尽量不使用end_r参数
{
	if (r <= 0 || c <= 0 || r >= m_map + 1 || c >= n_map + 2)	// 防溢出
	{
		return false;
	}

	bool end = (map[r][c] == END);	// 记录move之前是否已经到达end

	if (ch == 'w' && map[r - 1][c] != WALL)		// 上移处理
	{
		r--;
	}
	else if (ch == 's' && map[r + 1][c] != WALL)	// 下移处理
	{
		r++;
	}
	else if (ch == 'a' && map[r][c - 1] != WALL)	// 左移处理
	{
		c--;
	}
	else if (ch == 'd' && map[r][c + 1] != WALL)	// 右移处理
	{
		c++;
	}

	if (map[r][c] == BONUS)
	{
		cfg.score.bonus += 10;
		map[r][c] = ROAD;
		cfg.score.treasure++;
	}
	else if (map[r][c] == BAD)
	{
		cfg.score.bonus -= 2;
		cfg.score.bad++;
		map[r][c] = ROAD;
	}

	cfg.score.calc();
	sc = cfg.score;

	map[to_r][to_c] = END;
	if (end)
		return false;
	else if (map[r][c] == END)	// 判断是否到达终点
		return true;
	else
		return false;
}

void CGame::paint()
{
	if (map != NULL)
		map->putRoom(op.mode, op.mp);
}

void CMazeMap::putRoom(GAME_MODE mode, MAPTYPE mp)
{
	HRGN rgn = CreateRectRgn(16, 17, 465, 465);
	// 将该矩形区域设置为裁剪区
	setcliprgn(rgn);
	// 不再使用 rgn，清理 rgn 占用的系统资源
	DeleteObject(rgn);

	for (int i = r - 4; i <= r + 4; i++)	// 绘制迷宫
	{
		for (int j = c - 4; j <= c + 4; j++)
		{
			if (i < 0 || j < 0 || i > m_map + 1 || j > n_map + 1)
			{
				continue;
			}
			switch (map[i][j])
			{
			case ROAD: case WALL: case BAD: case END:
				setfillcolor(color[map[i][j]]);
				solidrectangle(
					(j - 1 - c) * 50 + 266,
					(i - 1 - r) * 50 + 266,
					(j - c) * 50 + 265,
					(i - r) * 50 + 265
				);
				break;
			case BONUS:
				setfillcolor(color[ROAD]);
				solidrectangle(
					(j - 1 - c) * 50 + 266,
					(i - 1 - r) * 50 + 266,
					(j - c) * 50 + 265,
					(i - r) * 50 + 265
				);
				setfillcolor(color[BONUS]);
				solidellipse(
					(j - 1 - c) * 50 + 266 + 9,
					(i - 1 - r) * 50 + 266 + 9,
					(j - c) * 50 + 265 - 9,
					(i - r) * 50 + 265 - 9
				);
				setfillcolor(color[ROAD]);
				solidrectangle(
					(j - 1 - c) * 50 + 266 + 20,
					(i - 1 - r) * 50 + 266 + 20,
					(j - c) * 50 + 265 - 20,
					(i - r) * 50 + 265 - 20
				);
				break;
			}
		}
	}
	
	if (!next.empty())
	{
		for (int i = r - 5; i <= r + 5; i++)	// 绘制轨迹
		{
			for (int j = c - 5; j <= c + 5; j++)
			{
				if (i < 0 || j < 0 || i > m_map + 1 || j > n_map + 1)
				{
					continue;
				}
				switch (next[i][j])
				{
				case LEFT:
					setfillcolor(DARKGRAY);
					solidrectangle(
						(j - 2 - c) * 50 + 266 + 16,
						(i - 1 - r) * 50 + 266 + 16,
						(j - c) * 50 + 265 - 16,
						(i - r) * 50 + 265 - 16
					);
					break;
				case RIGHT:
					setfillcolor(DARKGRAY);
					solidrectangle(
						(j - 1 - c) * 50 + 266 + 16,
						(i - 1 - r) * 50 + 266 + 16,
						(j + 1 - c) * 50 + 265 - 16,
						(i - r) * 50 + 265 - 16
					);
					break;
				case UP:
					setfillcolor(DARKGRAY);
					solidrectangle(
						(j - 1 - c) * 50 + 266 + 16,
						(i - 2 - r) * 50 + 266 + 16,
						(j - c) * 50 + 265 - 16,
						(i - r) * 50 + 265 - 16
					);
					break;
				case DOWN:
					setfillcolor(DARKGRAY);
					solidrectangle(
						(j - 1 - c) * 50 + 266 + 16,
						(i - 1 - r) * 50 + 266 + 16,
						(j - c) * 50 + 265 - 16,
						(i + 1 - r) * 50 + 265 - 16
					);
					break;
				}
			}
		}
	}

	setfillcolor(color[YOU]);
	solidellipse((-1) * 50 + 266, (-1) * 50 + 266, 265, 265);

	setcliprgn(NULL);

	cfg.score.time = _timer.GetTime();
	cfg.score.calc();
	sc = cfg.score;

	wchar_t text_bonus[25];
	swprintf_s(text_bonus, L"金币: %d/%d", cfg.score.treasure, treasure);
	wchar_t text_bad[25];
	swprintf_s(text_bad, L"沼泽: %d", cfg.score.bad);
	wchar_t text_score[25];
	swprintf_s(text_score, L"总分：%08d", cfg.score.score);

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	f.lfWidth = 0;
	wcscpy_s(f.lfFaceName, L"黑体");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);

	int mov = 0;
	if (mode == ADV) 
		mov = 30;
	outtextxy(480, 55 + mov, text_score);
	if (mp == DUNGEON)
	{
		outtextxy(480, 115 + mov, text_bonus);
		outtextxy(480, 145 + mov, text_bad);
	}
}
#pragma endregion

	#pragma region 迷宫生成
// 用于生成迷宫

void CMazeMap::setStartEnd(int tpr, int tpc, int end_r)
{
	// 设置起始点，保证坐标均为奇数且不越界
	to_c = n_map + 1;

	if (end_r == 0)	// 默认值
		to_r = m_map;
	else if (end_r % 2 != 0)
		to_r = end_r;
	else if (end_r + 1 <= m_map)
		to_r = end_r + 1;
	else
		to_r = end_r - 1;

	if (tpr % 2 != 0)
		r = tpr;
	else if (tpr + 1 <= m_map)
		r = tpr + 1;
	else
		r = tpr - 1;


	if (tpc % 2 != 0)
		c = tpc;
	else if (tpc + 1 <= n_map)
		c = tpc + 1;
	else
		c = tpc - 1;
}

#pragma endregion

	#pragma region 自动寻路与演示

void CGame::anime(CWindow* wnd)
{
	search();
	map->anime(wnd);
	FlushMouseMsgBuffer();
	fflush(stdin);
}

void CMazeMap::anime(CWindow *wnd)
{
	char keydir[4] = { 'w', 'a', 's', 'd' };
	while (next[r][c] != NULLDIR)
	{
		if (_kbhit())
		{
			_getch();
			break;
		}
		move(keydir[next[r][c]]);
		wnd->repaint();
		Sleep(33);
	}
}

void CGame::search()
{
	map->AstarSearch();
}

#pragma endregion

	/*void CMazeScene::choose(GAME_MAP gamemode, DFCTY difficulty)
{
	game->setMode(gamemode);
	game->setDf(difficulty);
	game->setSize(15 + gamemode * 10 + difficulty * 4,
		15 + gamemode * 10 + difficulty * 4);
	wchar_t name[50];
	InputBox(name, 45, L"告诉我你的名字\n", L"用户名");
	game->setName(name);
}*/
