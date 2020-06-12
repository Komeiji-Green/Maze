///////////////////////////////////
//
//   cpp文件：UsersWidgets.cpp
//   作用：实现全体用户组件类（除迷宫主窗口以外）
//   对应头文件：CGameWidgets.h
//
//

#include "CGameWidgets.h"
#include "ExFileIO.h"

	#pragma region 通用组件

bool BtBack::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		mainApp->PushMsg(new Msg(W_CLOSE, parent));
		return true;
	}
	return CButton::eventFilter(ev);
}

bool BtClose::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		mainApp->PushMsg(new Msg(W_CLOSE, mainApp->getWatched()));
	}
	return CButton::eventFilter(ev);
}

bool BtChoose::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		CDialog* n_dlg = new DlgChoose(mainApp, parent, op, tp);

		wchar_t* text[10];
		if (tp == MP)
			n_dlg->setPattern(170, 120, 300, 240, L"选择地图", text, 0);
		else if (tp == DF)
			n_dlg->setPattern(170, 90, 300, 300, L"选择难度", text, 0);

		mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// 过滤器的绑定在Create时才进行
	}
	return CButton::eventFilter(ev);
}

void DlgChoose::create()
{
	CDialog::create();

	const int num[2] = { MAXMAPTYPE, MAXDFCTY };
	const wchar_t* text[2][4] = {
		{ L"人间之里", L"迷途竹林" },
		{ L"EASY", L"NORMAL", L"HARD", L"LUNATIC" }
	};
	for (int i = 0; i < num[tp]; i++)
	{
		CButton* bt = new BtOption(mainApp, this, op, tp, i);
		bt->setPattern(x + 120, y + 60 + 60 * i, text[tp][i]);
		bt->setBtColor(RGB(100, 100, 100));
	}
}

bool BtOption::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		if (tp == MP)
			op.mp = MAPTYPE(value);
		else if (tp == DF)
			op.df = DFCTY(value);
	}
	return BtBack::eventFilter(ev);
}

#pragma endregion

	#pragma region 游戏首页

void MainWindow::paint()
{
	setbkcolor(RGB(50, 50, 50));
	cleardevice();

	for (int i = 0; i <= 3; i++)
	{
		settextcolor(RGB(100, 100, 100));
		settextstyle(70, 0, L"微软雅黑");
		outtextxy(235 + i, 85 + i, L"迷　宫");
	}
	settextcolor(WHITE);
	settextstyle(70, 0, L"微软雅黑");
	outtextxy(235 + 3, 85 + 3, L"迷　宫");		// 输出标题

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 10;
	wcscpy_s(f.lfFaceName, L"微软雅黑 Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);

	outtextxy(249, 440, L"按 Esc 退出游戏");		// 输出其他
}

void MainWindow::create()
{
	CButton* mybt1 = new BtSwitch(mainApp, this);
	mybt1->setPattern(280, 200, L"冒险模式");
	mybt1->setBtColor(RGB(100, 100, 100));

	CButton* mybt2 = new BtSwitchrk(mainApp, this);
	mybt2->setPattern(280, 320, L"分数排行");
	mybt2->setBtColor(RGB(100, 100, 100));

	CButton* mybt3 = new BtClose(mainApp, this);
	mybt3->setPattern(280, 380, L"　退出　");
	mybt3->setBtColor(RGB(100, 100, 100));

	CButton* mybt4 = new BtSwitchPrac(mainApp, this);
	mybt4->setPattern(280, 260, L"练习模式");
	mybt4->setBtColor(RGB(100, 100, 100));

	/*CSlider* mybt4 = new CSlider(mainApp, this);
	mybt4->setPattern(120, 380, 3, 350, 30, L"度度条", 200);*/
}

bool BtSwitch::eventFilter(Event* ev) // 用于从本主界面切换到下一主界面的按钮
{
	if (Clicked(ev)) // 此过滤器只处理Click型事件
	{
		CApplication* app = mainApp;
		CWindow* cur = app->getWatched();
		CWindow* pnext = new Adventure(app, cur); // 创建目标主界面（用new进行动态内存分配，会调用构造函数，且申请失败会抛出异常）
		app->PushMsg(new Msg(W_CREATE, pnext)); // 向消息队列中里加入目标主界面的CREATE消息
		app->PushMsg(new Msg(W_REFRESH, cur));
		app->setWatched(pnext); // 告知CApplication将主界面切换到目标主界面
		return true; // 本过滤器是一个拦截式过滤器，事件经过它之后，不再传递给剩余过滤器
	}
	return CButton::eventFilter(ev); // 调用父类的过滤器函数，这利用了虚函数机制，父类中虚函数负责实现各个派生组件所共有的事件响应，如果去掉此代码，可能会导致UI显示异常等问题。
}

bool BtSwitchrk::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		CApplication* app = mainApp;
		CWindow* cur = app->getWatched();
		CWindow* pnext = new Rank(app, cur);
		app->PushMsg(new Msg(W_CREATE, pnext));
		app->PushMsg(new Msg(W_REFRESH, cur));
		app->setWatched(pnext);
		return true;
	}
	return CButton::eventFilter(ev);
}

bool BtSwitchPrac::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		CApplication* app = mainApp;
		CWindow* cur = app->getWatched();
		CWindow* pnext = new Practice(app, cur);
		app->PushMsg(new Msg(W_CREATE, pnext));
		app->PushMsg(new Msg(W_REFRESH, cur));
		app->setWatched(pnext);
		return true;
	}
	return CButton::eventFilter(ev);
}

#pragma endregion

	#pragma region 冒险模式

Adventure::Adventure(CApplication* app, CWindow* parent_wnd) :
	CMainWindow(app, parent_wnd)
{
	InputBox(op.username, 50, L"输入你的名字", L"玩家名称");

	op.mp = FOREST;
	op.df = EASY;
	op.mode = ADV;
}

void Adventure::create()
{
	op.id = searchPlayer(op.username, info);

	CButton* btC1 = new BtChoose(mainApp, this, op, MP);
	btC1->setPattern(220, 200, L"选择地图");
	btC1->setBtColor(RGB(100, 100, 100));

	CButton* btC2 = new BtChoose(mainApp, this, op, DF);
	btC2->setPattern(220, 260, L"选择难度");
	btC2->setBtColor(RGB(100, 100, 100));

	CButton* btSt = new BtStart(mainApp, this, op, info);
	btSt->setPattern(360, 200, L"　开始　");
	btSt->setBtColor(RGB(100, 100, 100));

	CButton* btclose = new BtClose(mainApp, this);
	btclose->setPattern(360, 260, L"　退出　");
	btclose->setBtColor(RGB(100, 100, 100));
}

void Adventure::next()
{
	CApplication* app = mainApp;
	CWindow* pnext = new CMazeScene(app, this, op);
	app->PushMsg(new Msg(W_CREATE, pnext));
	app->setWatched(pnext);
}

void Adventure::paint()
{
	setbkcolor(RGB(50, 50, 50));
	cleardevice();

	for (int i = 0; i <= 3; i++)
	{
		settextcolor(RGB(100, 100, 100));
		settextstyle(70, 0, L"微软雅黑");
		outtextxy(200 + i, 85 + i, L"冒险模式");
	}
	settextcolor(WHITE);
	settextstyle(70, 0, L"微软雅黑");
	outtextxy(200 + 3, 85 + 3, L"冒险模式");		// 输出标题

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 10;
	wcscpy_s(f.lfFaceName, L"微软雅黑 Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);

	outtextxy(249, 320, op.username);		// 输出其他
	outtextxy(249, 355, Str_Maptype[op.mp]);
	outtextxy(249, 380, Str_Dfcty[op.df]);
}

bool BtStart::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		UINT16 mode = 1 << (size_t)(op.mp * MAXDFCTY + op.df);
		if (op.id == -1)  // 角色不存在或者playerdata文件不存在
		{
			op.load = false;
			op.id = newid();	// 如果文件不存在，在newid函数中会创建文件，且返回0
			mainApp->PushMsg(new Msg(W_NEXT, mainApp->getWatched()));	// 直接进入游戏
		}
		else if ((mode & info.mode) == 0)	// 角色存在但存档不存在（此时info对应角色与op一定相同）
		{
			op.load = false;
			mainApp->PushMsg(new Msg(W_NEXT, mainApp->getWatched()));	// 直接进入游戏
		}
		else	// 存档存在
		{
			CDialog* n_dlg = new DlgIfLoad(mainApp, parent, op);	// 进入询问是否读档的对话框

			wchar_t* text[10];
			text[0] = new wchar_t[20];
			wcscpy_s(text[0], 20, L"选“是”读档，选“否”覆盖\n");
			n_dlg->setPattern(170, 165, 300, 150, L"检测到存档", text, 1);

			mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	 // 过滤器的绑定在Create时才进行
		}
	}
	return CButton::eventFilter(ev);
}

void DlgIfLoad::create()
{
	CDialog::create();

	CButton* btYes = new BtIfLoad(mainApp, this, op, true);
	btYes->setPattern(x + width - 165, y + height - 37, L"是");
	btYes->setBtColor(RGB(100, 100, 100));				// 读档

	CButton* btNo = new BtIfLoad(mainApp, this, op, false);
	btNo->setPattern(x + width - 70, y + height - 37, L"否");
	btNo->setBtColor(RGB(100, 100, 100));				// 覆盖
}

bool BtIfLoad::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		op.load = value;
		mainApp->PushMsg(new Msg(W_NEXT, mainApp->getWatched()));
	}
	return CButton::eventFilter(ev);
}

#pragma endregion

	#pragma region 练习模式

Practice::Practice(CApplication* app, CWindow* parent_wnd) :
	CMainWindow(app, parent_wnd)
{
	op.mp = FOREST;
	op.df = EASY;
	op.mode = PRAC;

	swprintf_s(op.username, L"Practice");
	m_size = 9;
	n_size = 9;
	end_r = 1;
}

void Practice::create()
{
	CButton* btC1 = new BtChoose(mainApp, this, op, MP);
	btC1->setPattern(120, 220, L"选择地图");
	btC1->setBtColor(RGB(100, 100, 100));

	CButton* btSt = new BtStartPrac(mainApp, this);
	btSt->setPattern(280, 220, L"　开始　");
	btSt->setBtColor(RGB(100, 100, 100));

	CButton* btClose = new BtClose(mainApp, this);
	btClose->setPattern(440, 220, L"　退出　");
	btClose->setBtColor(RGB(100, 100, 100));

	CSlider* Slend = new SlControl(mainApp, this, end_r);
	Slend->setPattern(160, 400, 2, 300, 1, L"出口所在行", 4);

	CSlider* Slrow = new SlControl(mainApp, this, m_size, Slend);
	Slrow->setPattern(160, 300, 2, 300, 9, L"行数", 125);

	CSlider* Slcol = new SlControl(mainApp, this, n_size);
	Slcol->setPattern(160, 350, 2, 300, 9, L"列数", 125);
}

void Practice::next()
{
	CApplication* app = mainApp;
	CWindow* pnext = new CMazeScene(app, this, op, m_size, n_size, end_r, PRAC);
	app->PushMsg(new Msg(W_CREATE, pnext));
	app->setWatched(pnext);
}

void Practice::paint()
{
	setbkcolor(RGB(50, 50, 50));
	cleardevice();

	for (int i = 0; i <= 3; i++)
	{
		settextcolor(RGB(100, 100, 100));
		settextstyle(70, 0, L"微软雅黑");
		outtextxy(200 + i, 85 + i, L"练习模式");
	}
	settextcolor(WHITE);
	settextstyle(70, 0, L"微软雅黑");
	outtextxy(200 + 3, 85 + 3, L"练习模式");		// 输出标题

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 10;
	wcscpy_s(f.lfFaceName, L"微软雅黑 Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);

	outtextxy(249, 425, Str_Maptype[op.mp]);
}

bool BtStartPrac::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		mainApp->PushMsg(new Msg(W_NEXT, mainApp->getWatched()));
	}
	return CButton::eventFilter(ev);
}

bool SlControl::eventFilter(Event* ev)
{
	bool flag = CSlider::eventFilter(ev);
	watched = getValue();
	if (subSl != NULL)
	{
		subSl->setBorder(getValue(), true);
	}
	return flag;
}

#pragma endregion

	#pragma region 排行榜

Rank::Rank(CApplication* app, CWindow* parent_wnd) :
	CMainWindow(app, parent_wnd), mp(FOREST), df(EASY), page(0)
{
	op.mp = FOREST;
	op.df = EASY;

	ifstream inf;
	wchar_t file[MAX_PATH];

	for (int i = 0; i < MAXMAPTYPE; i++)
	{
		for (int j = 0; j < MAXDFCTY; j++)
		{
			filename_rank(file, MAX_PATH, i, j);

			inf.clear();
			inf.open(file, ios::binary);
			readRankData(inf, data[i][j]);
			inf.close();
		}
	}
}

void Rank::create()
{
	CButton* btC1 = new BtChoose(mainApp, this, op, MP);
	btC1->setPattern(160, 80, L"选择地图");
	btC1->setBtColor(RGB(100, 100, 100));

	CButton* btC2 = new BtChoose(mainApp, this, op, DF);
	btC2->setPattern(420, 80, L"选择难度");
	btC2->setBtColor(RGB(100, 100, 100));

	CButton* btPage1 = new BtPage(mainApp, this, *this, 0);
	btPage1->setPattern(160, 400, L"上一页");
	btPage1->setBtColor(RGB(100, 100, 100));

	CButton* btPage2 = new BtPage(mainApp, this, *this, 1);
	btPage2->setPattern(420, 400, L"下一页");
	btPage2->setBtColor(RGB(100, 100, 100));

	CButton* btClose = new BtClose(mainApp, this);
	btClose->setPattern(540, 400, L"退出");
	btClose->setBtColor(RGB(100, 100, 100));
}

void Rank::paint()
{
	setbkcolor(RGB(50, 50, 50));
	cleardevice();

	if ((mp != op.mp) || (df != op.df))
	{
		page = 0;
		mp = op.mp;
		df = op.df;
	}

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 18;
	f.lfWidth = 0;
	wcscpy_s(f.lfFaceName, L"黑体");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);

	op.mp == FOREST ? outtextxy(100, 140, L"序号　　　名字　　用时　总分　　日期")
		: outtextxy(80, 140, L"序号　　　名字　　金币　池沼　用时　总分　　　日期");

	wchar_t temp[12];
	wchar_t text[100];

	int st = page * maxrow;
	const auto& list = data[mp][df];
	for (int i = 0; i < maxrow && (st + i) < list.size(); i++)		// 输出标题
	{
		const auto& tuple = list[st + i];
		wcsncpy_s(temp, tuple.username, 11);
		if (op.mp == FOREST)
		{
			swprintf_s(text, L"%2d %12s %5d %6d %18s", st + i + 1,
				temp, tuple.sc.time, tuple.sc.score, tuple.systime);
			outtextxy(100, 140 + (i + 1) * 25, text);
		}
		else
		{
			swprintf_s(text, L"%2d %12s %5d %5d %5d %6d %17s", st + i + 1,
				temp, tuple.sc.treasure, tuple.sc.bad, tuple.sc.time, tuple.sc.score, tuple.systime);
			outtextxy(80, 140 + (i + 1) * 25, text);
		}
	}

}

bool BtPage::eventFilter(Event* ev)
{
	if (Clicked(ev) && (rank.mp == rank.op.mp) && (rank.df == rank.op.df))
	{
		const MAPTYPE& mp = rank.mp;
		const DFCTY& df = rank.df;
		const int maxrow = rank.maxrow;
		const auto& data = rank.data;
		int& page = rank.page;
		if (type == 0)	// 上一页
		{
			if (page > 0)
				page--;
		}
		else if (type == 1)	// 下一页
		{
			if ((maxrow * (page + 1)) < data[mp][df].size())
				page++;
		}
	}
	return CButton::eventFilter(ev);
}

#pragma endregion

	#pragma region 游戏内组件

bool BtSuspend::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		CDialog* n_dlg = new DlgSuspend(mainApp, parent);

		wchar_t* text[10];
		text[0] = new wchar_t[10];
		wcscpy_s(text[0], 9, L"是否结束暂停？\n");
		n_dlg->setPattern(170, 165, 300, 150, L"暂停", text, 1);		// 设置对话框样式

		mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// 过滤器的绑定在Create时才进行
		_timer.Pause();	// 中断计时器
	}

	return CButton::eventFilter(ev);
}

void DlgSuspend::create()
{
	CDialog::create();	// 绑定过滤器！！！

	CButton* btBack = new BtBack(mainApp, this);
	btBack->setPattern(x + width - 70, y + height - 37, L"确定");
	btBack->setBtColor(RGB(100, 100, 100));				// 设置返回按钮
}

void DlgSuspend::close()
{
	_timer.Continue();	// 唤醒计时器
	CDialog::close();
}


void DlgEnd::create()
{
	CDialog::create();	// 绑定过滤器！！！

	CButton* btBack = new BtBack(mainApp, this);
	btBack->setPattern(x + width - 70, y + height - 37, L"继续");
	btBack->setBtColor(RGB(100, 100, 100));				// 返回游戏

	CButton* btOver = new BtOver(mainApp, this, game, mode);
	btOver->setPattern(x + width - 165, y + height - 37, L"结束");
	btOver->setBtColor(RGB(100, 100, 100));				// 结束游戏
}

void DlgEnd::close()
{
	_timer.Continue();
	CDialog::close();
}

bool BtOver::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		const Options& op = game->getop();
		const Score& sc = game->getsc();

		if (mode == ADV)
		{
			deletePlayerData(op);	// 删除存档信息

			wchar_t fname[MAX_PATH];
			filename(fname, MAX_PATH, op);
			DeleteFile(fname);		// 删除存档文件

			addRankData(op, sc);	// 添加到排行榜
		}

		CDialog* n_dlg = new DlgOver(mainApp, parent);

		wchar_t* text[10];
		text[0] = new wchar_t[100];
		text[1] = new wchar_t[50];
		text[2] = new wchar_t[50];
		swprintf_s(text[0], 100, L"%s %s %s\0", op.username, Str_Maptype[op.mp], Str_Dfcty[op.df]);
		swprintf_s(text[1], 20, L"用时: %d\0", sc.time);
		swprintf_s(text[2], 20, L"总分: %d", sc.score);
		n_dlg->setPattern(170, 165, 300, 150, L"游戏结束", text, 3);		// 设置对话框样式

		mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// 过滤器的绑定在Create时才进行
	}
	return CButton::eventFilter(ev);
}

void DlgOver::create()
{
	CDialog::create();	// 绑定过滤器

	CButton* btClose = new BtClose(mainApp, this);
	btClose->setPattern(x + width - 70, y + height - 37, L"确定");
	btClose->setBtColor(RGB(100, 100, 100));				// 设置返回按钮
}


bool BtAstar::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		game->search();
	}
	return CButton::eventFilter(ev);
}

bool BtAnime::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		game->search();
		game->anime(parent);
	}
	return CButton::eventFilter(ev);
}

bool BtSave::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		game->save();
	}
	return CButton::eventFilter(ev);
}

#pragma endregion

