///////////////////////////////////
//
//   cpp�ļ���UsersWidgets.cpp
//   ���ã�ʵ��ȫ���û�����ࣨ���Թ����������⣩
//   ��Ӧͷ�ļ���CGameWidgets.h
//
//

#include "CGameWidgets.h"
#include "ExFileIO.h"

	#pragma region ͨ�����

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
			n_dlg->setPattern(170, 120, 300, 240, L"ѡ���ͼ", text, 0);
		else if (tp == DF)
			n_dlg->setPattern(170, 90, 300, 300, L"ѡ���Ѷ�", text, 0);

		mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// �������İ���Createʱ�Ž���
	}
	return CButton::eventFilter(ev);
}

void DlgChoose::create()
{
	CDialog::create();

	const int num[2] = { MAXMAPTYPE, MAXDFCTY };
	const wchar_t* text[2][4] = {
		{ L"�˼�֮��", L"��;����" },
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

	#pragma region ��Ϸ��ҳ

void MainWindow::paint()
{
	setbkcolor(RGB(50, 50, 50));
	cleardevice();

	for (int i = 0; i <= 3; i++)
	{
		settextcolor(RGB(100, 100, 100));
		settextstyle(70, 0, L"΢���ź�");
		outtextxy(235 + i, 85 + i, L"�ԡ���");
	}
	settextcolor(WHITE);
	settextstyle(70, 0, L"΢���ź�");
	outtextxy(235 + 3, 85 + 3, L"�ԡ���");		// �������

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 10;
	wcscpy_s(f.lfFaceName, L"΢���ź� Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);

	outtextxy(249, 440, L"�� Esc �˳���Ϸ");		// �������
}

void MainWindow::create()
{
	CButton* mybt1 = new BtSwitch(mainApp, this);
	mybt1->setPattern(280, 200, L"ð��ģʽ");
	mybt1->setBtColor(RGB(100, 100, 100));

	CButton* mybt2 = new BtSwitchrk(mainApp, this);
	mybt2->setPattern(280, 320, L"��������");
	mybt2->setBtColor(RGB(100, 100, 100));

	CButton* mybt3 = new BtClose(mainApp, this);
	mybt3->setPattern(280, 380, L"���˳���");
	mybt3->setBtColor(RGB(100, 100, 100));

	CButton* mybt4 = new BtSwitchPrac(mainApp, this);
	mybt4->setPattern(280, 260, L"��ϰģʽ");
	mybt4->setBtColor(RGB(100, 100, 100));

	/*CSlider* mybt4 = new CSlider(mainApp, this);
	mybt4->setPattern(120, 380, 3, 350, 30, L"�ȶ���", 200);*/
}

bool BtSwitch::eventFilter(Event* ev) // ���ڴӱ��������л�����һ������İ�ť
{
	if (Clicked(ev)) // �˹�����ֻ����Click���¼�
	{
		CApplication* app = mainApp;
		CWindow* cur = app->getWatched();
		CWindow* pnext = new Adventure(app, cur); // ����Ŀ�������棨��new���ж�̬�ڴ���䣬����ù��캯����������ʧ�ܻ��׳��쳣��
		app->PushMsg(new Msg(W_CREATE, pnext)); // ����Ϣ�����������Ŀ���������CREATE��Ϣ
		app->PushMsg(new Msg(W_REFRESH, cur));
		app->setWatched(pnext); // ��֪CApplication���������л���Ŀ��������
		return true; // ����������һ������ʽ���������¼�������֮�󣬲��ٴ��ݸ�ʣ�������
	}
	return CButton::eventFilter(ev); // ���ø���Ĺ��������������������麯�����ƣ��������麯������ʵ�ָ���������������е��¼���Ӧ�����ȥ���˴��룬���ܻᵼ��UI��ʾ�쳣�����⡣
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

	#pragma region ð��ģʽ

Adventure::Adventure(CApplication* app, CWindow* parent_wnd) :
	CMainWindow(app, parent_wnd)
{
	InputBox(op.username, 50, L"�����������", L"�������");

	op.mp = FOREST;
	op.df = EASY;
	op.mode = ADV;
}

void Adventure::create()
{
	op.id = searchPlayer(op.username, info);

	CButton* btC1 = new BtChoose(mainApp, this, op, MP);
	btC1->setPattern(220, 200, L"ѡ���ͼ");
	btC1->setBtColor(RGB(100, 100, 100));

	CButton* btC2 = new BtChoose(mainApp, this, op, DF);
	btC2->setPattern(220, 260, L"ѡ���Ѷ�");
	btC2->setBtColor(RGB(100, 100, 100));

	CButton* btSt = new BtStart(mainApp, this, op, info);
	btSt->setPattern(360, 200, L"����ʼ��");
	btSt->setBtColor(RGB(100, 100, 100));

	CButton* btclose = new BtClose(mainApp, this);
	btclose->setPattern(360, 260, L"���˳���");
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
		settextstyle(70, 0, L"΢���ź�");
		outtextxy(200 + i, 85 + i, L"ð��ģʽ");
	}
	settextcolor(WHITE);
	settextstyle(70, 0, L"΢���ź�");
	outtextxy(200 + 3, 85 + 3, L"ð��ģʽ");		// �������

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 10;
	wcscpy_s(f.lfFaceName, L"΢���ź� Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);

	outtextxy(249, 320, op.username);		// �������
	outtextxy(249, 355, Str_Maptype[op.mp]);
	outtextxy(249, 380, Str_Dfcty[op.df]);
}

bool BtStart::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		UINT16 mode = 1 << (size_t)(op.mp * MAXDFCTY + op.df);
		if (op.id == -1)  // ��ɫ�����ڻ���playerdata�ļ�������
		{
			op.load = false;
			op.id = newid();	// ����ļ������ڣ���newid�����лᴴ���ļ����ҷ���0
			mainApp->PushMsg(new Msg(W_NEXT, mainApp->getWatched()));	// ֱ�ӽ�����Ϸ
		}
		else if ((mode & info.mode) == 0)	// ��ɫ���ڵ��浵�����ڣ���ʱinfo��Ӧ��ɫ��opһ����ͬ��
		{
			op.load = false;
			mainApp->PushMsg(new Msg(W_NEXT, mainApp->getWatched()));	// ֱ�ӽ�����Ϸ
		}
		else	// �浵����
		{
			CDialog* n_dlg = new DlgIfLoad(mainApp, parent, op);	// ����ѯ���Ƿ�����ĶԻ���

			wchar_t* text[10];
			text[0] = new wchar_t[20];
			wcscpy_s(text[0], 20, L"ѡ���ǡ�������ѡ���񡱸���\n");
			n_dlg->setPattern(170, 165, 300, 150, L"��⵽�浵", text, 1);

			mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	 // �������İ���Createʱ�Ž���
		}
	}
	return CButton::eventFilter(ev);
}

void DlgIfLoad::create()
{
	CDialog::create();

	CButton* btYes = new BtIfLoad(mainApp, this, op, true);
	btYes->setPattern(x + width - 165, y + height - 37, L"��");
	btYes->setBtColor(RGB(100, 100, 100));				// ����

	CButton* btNo = new BtIfLoad(mainApp, this, op, false);
	btNo->setPattern(x + width - 70, y + height - 37, L"��");
	btNo->setBtColor(RGB(100, 100, 100));				// ����
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

	#pragma region ��ϰģʽ

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
	btC1->setPattern(120, 220, L"ѡ���ͼ");
	btC1->setBtColor(RGB(100, 100, 100));

	CButton* btSt = new BtStartPrac(mainApp, this);
	btSt->setPattern(280, 220, L"����ʼ��");
	btSt->setBtColor(RGB(100, 100, 100));

	CButton* btClose = new BtClose(mainApp, this);
	btClose->setPattern(440, 220, L"���˳���");
	btClose->setBtColor(RGB(100, 100, 100));

	CSlider* Slend = new SlControl(mainApp, this, end_r);
	Slend->setPattern(160, 400, 2, 300, 1, L"����������", 4);

	CSlider* Slrow = new SlControl(mainApp, this, m_size, Slend);
	Slrow->setPattern(160, 300, 2, 300, 9, L"����", 125);

	CSlider* Slcol = new SlControl(mainApp, this, n_size);
	Slcol->setPattern(160, 350, 2, 300, 9, L"����", 125);
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
		settextstyle(70, 0, L"΢���ź�");
		outtextxy(200 + i, 85 + i, L"��ϰģʽ");
	}
	settextcolor(WHITE);
	settextstyle(70, 0, L"΢���ź�");
	outtextxy(200 + 3, 85 + 3, L"��ϰģʽ");		// �������

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 10;
	wcscpy_s(f.lfFaceName, L"΢���ź� Light");
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

	#pragma region ���а�

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
	btC1->setPattern(160, 80, L"ѡ���ͼ");
	btC1->setBtColor(RGB(100, 100, 100));

	CButton* btC2 = new BtChoose(mainApp, this, op, DF);
	btC2->setPattern(420, 80, L"ѡ���Ѷ�");
	btC2->setBtColor(RGB(100, 100, 100));

	CButton* btPage1 = new BtPage(mainApp, this, *this, 0);
	btPage1->setPattern(160, 400, L"��һҳ");
	btPage1->setBtColor(RGB(100, 100, 100));

	CButton* btPage2 = new BtPage(mainApp, this, *this, 1);
	btPage2->setPattern(420, 400, L"��һҳ");
	btPage2->setBtColor(RGB(100, 100, 100));

	CButton* btClose = new BtClose(mainApp, this);
	btClose->setPattern(540, 400, L"�˳�");
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
	wcscpy_s(f.lfFaceName, L"����");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);

	op.mp == FOREST ? outtextxy(100, 140, L"��š��������֡�����ʱ���ܷ֡�������")
		: outtextxy(80, 140, L"��š��������֡�����ҡ����ӡ���ʱ���ܷ֡���������");

	wchar_t temp[12];
	wchar_t text[100];

	int st = page * maxrow;
	const auto& list = data[mp][df];
	for (int i = 0; i < maxrow && (st + i) < list.size(); i++)		// �������
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
		if (type == 0)	// ��һҳ
		{
			if (page > 0)
				page--;
		}
		else if (type == 1)	// ��һҳ
		{
			if ((maxrow * (page + 1)) < data[mp][df].size())
				page++;
		}
	}
	return CButton::eventFilter(ev);
}

#pragma endregion

	#pragma region ��Ϸ�����

bool BtSuspend::eventFilter(Event* ev)
{
	if (Clicked(ev))
	{
		CDialog* n_dlg = new DlgSuspend(mainApp, parent);

		wchar_t* text[10];
		text[0] = new wchar_t[10];
		wcscpy_s(text[0], 9, L"�Ƿ������ͣ��\n");
		n_dlg->setPattern(170, 165, 300, 150, L"��ͣ", text, 1);		// ���öԻ�����ʽ

		mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// �������İ���Createʱ�Ž���
		_timer.Pause();	// �жϼ�ʱ��
	}

	return CButton::eventFilter(ev);
}

void DlgSuspend::create()
{
	CDialog::create();	// �󶨹�����������

	CButton* btBack = new BtBack(mainApp, this);
	btBack->setPattern(x + width - 70, y + height - 37, L"ȷ��");
	btBack->setBtColor(RGB(100, 100, 100));				// ���÷��ذ�ť
}

void DlgSuspend::close()
{
	_timer.Continue();	// ���Ѽ�ʱ��
	CDialog::close();
}


void DlgEnd::create()
{
	CDialog::create();	// �󶨹�����������

	CButton* btBack = new BtBack(mainApp, this);
	btBack->setPattern(x + width - 70, y + height - 37, L"����");
	btBack->setBtColor(RGB(100, 100, 100));				// ������Ϸ

	CButton* btOver = new BtOver(mainApp, this, game, mode);
	btOver->setPattern(x + width - 165, y + height - 37, L"����");
	btOver->setBtColor(RGB(100, 100, 100));				// ������Ϸ
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
			deletePlayerData(op);	// ɾ���浵��Ϣ

			wchar_t fname[MAX_PATH];
			filename(fname, MAX_PATH, op);
			DeleteFile(fname);		// ɾ���浵�ļ�

			addRankData(op, sc);	// ��ӵ����а�
		}

		CDialog* n_dlg = new DlgOver(mainApp, parent);

		wchar_t* text[10];
		text[0] = new wchar_t[100];
		text[1] = new wchar_t[50];
		text[2] = new wchar_t[50];
		swprintf_s(text[0], 100, L"%s %s %s\0", op.username, Str_Maptype[op.mp], Str_Dfcty[op.df]);
		swprintf_s(text[1], 20, L"��ʱ: %d\0", sc.time);
		swprintf_s(text[2], 20, L"�ܷ�: %d", sc.score);
		n_dlg->setPattern(170, 165, 300, 150, L"��Ϸ����", text, 3);		// ���öԻ�����ʽ

		mainApp->PushMsg(new Msg(W_CREATE, n_dlg));	// �������İ���Createʱ�Ž���
	}
	return CButton::eventFilter(ev);
}

void DlgOver::create()
{
	CDialog::create();	// �󶨹�����

	CButton* btClose = new BtClose(mainApp, this);
	btClose->setPattern(x + width - 70, y + height - 37, L"ȷ��");
	btClose->setBtColor(RGB(100, 100, 100));				// ���÷��ذ�ť
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

