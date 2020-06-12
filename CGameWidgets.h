///////////////////////////////////
//
//   ͷ�ļ���CGameWidgets.h
//   ���ã�����ȫ���û�����ࣨ���Թ����������⣩
//   ��Ӧcpp�ļ���UsersButton.cpp �� UsersWnd.cpp
//
//   �������֮��ĵ��ù�ϵ����ע����˵����
//   ������أ�
//   ���ڶԻ����������Щ��ť/��������������create()��Ա������ʵ���в鿴
//   ���ڰ�ť�ᵯ����Щ�Ի���/������Щ��Ϣ��������eventFilter()��Ա������ʵ���в鿴
//   ������ؾ���paint()�в鿴
//
//

#pragma once

#include "CMazeCore.h"
#include "ExFileIO.h"

	#pragma region ͨ�����

// ��ť���رյ�ǰ������
class BtClose : public CButton
{
public:
	BtClose(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtClose() {}

protected:
	virtual bool eventFilter(Event*);

};

// ��ť��������һ��
class BtBack : public CButton
{
public:
	BtBack(CApplication* app) : CButton(app) {}
	BtBack(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtBack() {}

protected:
	virtual bool eventFilter(Event*);
};


// ö�����ͣ�������ѡ��Ի���
enum CHOOSETYPE { MP = 0, DF };

// ��ť������ѡ����������DlgChoose
class BtChoose : public CButton
{
private:
	Options& op;
	CHOOSETYPE tp;

public:
	BtChoose(CApplication* app, CWindow* parent, Options& op, CHOOSETYPE _tp) :
		CButton(app, parent), tp(_tp), op(op) {}
	virtual ~BtChoose() {}

protected:
	virtual bool eventFilter(Event*);

};

// �Ի���ѡ��Ի�����BtChoose���ã�����BtOption
class DlgChoose : public CDialog
{
private:
	CHOOSETYPE tp;
	Options& op;

public:
	DlgChoose(CApplication* app, CWindow* parent, Options& op, CHOOSETYPE tp) :
		CDialog(app, parent), op(op), tp(tp) {}
	virtual ~DlgChoose() {}

	virtual void create();
};

// ��ť��������Ϊѡ�����DlgChoose����
class BtOption : public BtBack
{
private:
	Options& op;
	CHOOSETYPE tp;
	int value;

public:
	BtOption(CApplication* app, CWindow* parent, Options& op, CHOOSETYPE tp, int value) :
		BtBack(app, parent), op(op), tp(tp), value(value) {}
	virtual ~BtOption() {}

protected:
	virtual bool eventFilter(Event*);

};

#pragma endregion

	#pragma region ��Ϸ��ҳ

// ��Ϸ�����������
class MainWindow : public CMainWindow
{
public:
	MainWindow(CApplication* app, CWindow* parent_wnd) :
		CMainWindow(app, parent_wnd) {}
	virtual ~MainWindow() {}

	virtual void create();
	virtual void paint();
};

// ��Ϸ��ҳ����ť������ð��ģʽ
class BtSwitch : public CButton
{
public:
	BtSwitch(CApplication* app) : CButton(app) {}
	BtSwitch(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSwitch() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// ��Ϸ��ҳ����ť���������а�
class BtSwitchrk : public CButton
{
public:
	BtSwitchrk(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSwitchrk() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// ��Ϸ��ҳ����ť��������ϰģʽ
class BtSwitchPrac : public CButton
{
public:
	BtSwitchPrac(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSwitchPrac() {}

protected:
	virtual bool eventFilter(Event* ev);
};

#pragma endregion

	#pragma region ð��ģʽ

// �����ڣ�ð��ģʽ
class Adventure : public CMainWindow
{
private:
	Options op;
	Tuple_Playerinfo info;

public:
	Adventure(CApplication* app, CWindow* parent_wnd);
	virtual ~Adventure() {}

	virtual void create();
	virtual void paint();
	virtual void next();
};

// ð��ģʽ����ť����ʼ��Ϸ���������DlgIfLoad
class BtStart : public CButton
{
private:
	Options& op;
	Tuple_Playerinfo& info;

public:
	BtStart(CApplication* app, CWindow* parent, Options& op, Tuple_Playerinfo& info) :
		CButton(app, parent), op(op), info(info) {}
	virtual ~BtStart() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// ð��ģʽ���Ի���ѯ���Ƿ�����浵����BtStart���ã�����BtIfLoad
class DlgIfLoad : public CDialog
{
private:
	Options& op;

public:
	DlgIfLoad(CApplication* app, CWindow* parent, Options& op) :
		CDialog(app, parent), op(op) {}
	virtual ~DlgIfLoad() {}

	virtual void create();
};

// ð��ģʽ����ť��ѡ���Ƿ�����浵��ѡ�����DlgIfLoad����
class BtIfLoad : public CButton
{
private:
	Options& op;
	bool value;

public:
	BtIfLoad(CApplication* app, CWindow* parent, Options& op, bool value) :
		CButton(app, parent), op(op), value(value) {}
	virtual ~BtIfLoad() {}

protected:
	virtual bool eventFilter(Event* ev);
};

#pragma endregion

	#pragma region ��ϰģʽ

// �����ڣ���ϰģʽ
class Practice : public CMainWindow
{
private:
	Options op;
	int m_size, n_size, end_r;

public:
	Practice(CApplication* app, CWindow* parent_wnd);
	virtual ~Practice() {}

	virtual void create();
	virtual void paint();
	virtual void next();
};

// ��ϰģʽ����ť����ʼ��Ϸ
class BtStartPrac : public CButton
{
public:
	BtStartPrac(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtStartPrac() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// ��ϰģʽ�������������Ʋ���ֵ
class SlControl : public CSlider
{
private:
	CSlider* subSl;	// ����������
	int& watched;	// �û��������Ƶ�ֵ

public:
	SlControl(CApplication* app, CWindow* parent, int& watched, CSlider* subSl = NULL) :
		CSlider(app, parent), watched(watched), subSl(subSl) {}
	virtual ~SlControl() {}

protected:
	virtual bool eventFilter(Event*);
};

#pragma endregion

	#pragma region ���а�

// �����ڣ����а�
class Rank : public CMainWindow
{
private:
	const int maxrow = 8;
	Options op;
	int page = 0;
	MAPTYPE mp;
	DFCTY df;
	vector<Tuple_Rankinfo> data[MAXMAPTYPE][MAXDFCTY];

public:
	Rank(CApplication* app, CWindow* parent_wnd);
	virtual ~Rank() {}

	virtual void create();
	virtual void paint();

	friend class BtPage;
};

// ��ť����ҳ
class BtPage : public CButton
{
private:
	Rank& rank;
	int type;

public:
	BtPage(CApplication* app, CWindow* parent, Rank& rank, int type) :
		CButton(app, parent), rank(rank), type(type) {}
	virtual ~BtPage() {}

protected:
	virtual bool eventFilter(Event*);
};

#pragma endregion

	#pragma region ��Ϸ�����

// �Ի���

// ��Ϸ�ڣ���ť����ͣ���������DlgSuspend
class BtSuspend : public CButton
{
public:
	BtSuspend(CApplication* app) : CButton(app) {}
	BtSuspend(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSuspend() {}

protected:
	virtual bool eventFilter(Event*);
};

// ��Ϸ�ڣ��Ի�����ͣ�Ի�����BtSuspend����
class DlgSuspend : public CDialog
{
public:
	DlgSuspend(CApplication* app) : CDialog(app) {}
	DlgSuspend(CApplication* app, CWindow* parent) : CDialog(app, parent) { }
	virtual ~DlgSuspend() {}

	virtual void create();
	virtual void close();
};



// ��Ϸ�ڣ��Ի�����Ϸ�����յ�ʱ����������BtOver
class DlgEnd : public CDialog
{
public:
	DlgEnd(CApplication* app, CWindow* parent) : CDialog(app, parent), game(NULL) {}
	DlgEnd(CApplication* app, CWindow* parent, CGame* game, GAME_MODE mode) :
		CDialog(app, parent), game(game), mode(mode) {}
	virtual ~DlgEnd() {}

	virtual void create();
	virtual void close();
private:
	CGame* game;
	GAME_MODE mode;
};

// ��Ϸ�ڣ���ť����Ϸ��������DlgEnd���ã��������DlgOver
class BtOver : public CButton
{
public:
	BtOver(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	BtOver(CApplication* app, CWindow* parent, CGame* game, GAME_MODE mode) :
		CButton(app, parent), game(game), mode(mode) {}
	virtual ~BtOver() {}

protected:
	virtual bool eventFilter(Event*);

private:
	CGame* game = NULL;
	GAME_MODE mode;
};

// ��Ϸ�ڣ��Ի�����ʾ��Ϸ������ĵ÷���Ϣ����BtOver����
class DlgOver : public CDialog
{
public:
	DlgOver(CApplication* app, CWindow* parent) : CDialog(app, parent) {}
	virtual ~DlgOver() {}

	virtual void create();
};



// ��Ϸ�ڣ���ť�������Զ�Ѱ·
class BtAstar : public CButton
{
public:
	BtAstar(CApplication* app, CWindow* parent, CGame* game) :
		CButton(app, parent), game(game) {}
	virtual ~BtAstar() {}

protected:
	virtual bool eventFilter(Event*);

private:
	CGame* game = NULL;
};

// ��Ϸ�ڣ���ť��������ʾ����
class BtAnime : public CButton
{
public:
	BtAnime(CApplication* app, CWindow* parent, CGame* game) :
		CButton(app, parent), game(game) {}
	virtual ~BtAnime() {}

protected:
	virtual bool eventFilter(Event*);

private:
	CGame* game = NULL;
};

// ��Ϸ�ڣ���ť������Ա���
class BtSave : public CButton
{
public:
	BtSave(CApplication* app, CWindow* parent, CGame* game) : 
		CButton(app, parent), game(game) {}
	virtual ~BtSave() {}

protected:
	virtual bool eventFilter(Event*);

private:
	CGame* game = NULL;
};

#pragma endregion
