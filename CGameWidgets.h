///////////////////////////////////
//
//   头文件：CGameWidgets.h
//   作用：定义全体用户组件类（除迷宫主窗口以外）
//   对应cpp文件：UsersButton.cpp 与 UsersWnd.cpp
//
//   特殊组件之间的调用关系已在注释中说明。
//   更具体地，
//   关于对话框包含了哪些按钮/滑动条，可以在create()成员函数的实现中查看
//   关于按钮会弹出哪些对话框/触发哪些消息，可以在eventFilter()成员函数的实现中查看
//   绘制相关均在paint()中查看
//
//

#pragma once

#include "CMazeCore.h"
#include "ExFileIO.h"

	#pragma region 通用组件

// 按钮：关闭当前主窗口
class BtClose : public CButton
{
public:
	BtClose(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtClose() {}

protected:
	virtual bool eventFilter(Event*);

};

// 按钮：返回上一级
class BtBack : public CButton
{
public:
	BtBack(CApplication* app) : CButton(app) {}
	BtBack(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtBack() {}

protected:
	virtual bool eventFilter(Event*);
};


// 枚举类型：服务于选项对话框
enum CHOOSETYPE { MP = 0, DF };

// 按钮：进入选项卡，点击触发DlgChoose
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

// 对话框：选项卡对话框，由BtChoose调用，包含BtOption
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

// 按钮：用其作为选项卡，由DlgChoose调用
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

	#pragma region 游戏首页

// 游戏进入的主窗口
class MainWindow : public CMainWindow
{
public:
	MainWindow(CApplication* app, CWindow* parent_wnd) :
		CMainWindow(app, parent_wnd) {}
	virtual ~MainWindow() {}

	virtual void create();
	virtual void paint();
};

// 游戏首页：按钮：进入冒险模式
class BtSwitch : public CButton
{
public:
	BtSwitch(CApplication* app) : CButton(app) {}
	BtSwitch(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSwitch() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// 游戏首页：按钮：进入排行榜
class BtSwitchrk : public CButton
{
public:
	BtSwitchrk(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSwitchrk() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// 游戏首页：按钮：进入练习模式
class BtSwitchPrac : public CButton
{
public:
	BtSwitchPrac(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSwitchPrac() {}

protected:
	virtual bool eventFilter(Event* ev);
};

#pragma endregion

	#pragma region 冒险模式

// 主窗口：冒险模式
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

// 冒险模式：按钮：开始游戏，点击触发DlgIfLoad
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

// 冒险模式：对话框：询问是否载入存档，由BtStart调用，包含BtIfLoad
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

// 冒险模式：按钮：选择是否载入存档的选项卡，由DlgIfLoad调用
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

	#pragma region 练习模式

// 主窗口：练习模式
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

// 练习模式：按钮：开始游戏
class BtStartPrac : public CButton
{
public:
	BtStartPrac(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtStartPrac() {}

protected:
	virtual bool eventFilter(Event* ev);
};

// 练习模式：滑动条：控制参数值
class SlControl : public CSlider
{
private:
	CSlider* subSl;	// 下属滑动条
	int& watched;	// 该滑动条控制的值

public:
	SlControl(CApplication* app, CWindow* parent, int& watched, CSlider* subSl = NULL) :
		CSlider(app, parent), watched(watched), subSl(subSl) {}
	virtual ~SlControl() {}

protected:
	virtual bool eventFilter(Event*);
};

#pragma endregion

	#pragma region 排行榜

// 主窗口：排行榜
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

// 按钮：翻页
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

	#pragma region 游戏内组件

// 对话框

// 游戏内：按钮：暂停，点击触发DlgSuspend
class BtSuspend : public CButton
{
public:
	BtSuspend(CApplication* app) : CButton(app) {}
	BtSuspend(CApplication* app, CWindow* parent) : CButton(app, parent) {}
	virtual ~BtSuspend() {}

protected:
	virtual bool eventFilter(Event*);
};

// 游戏内：对话框：暂停对话框，由BtSuspend调用
class DlgSuspend : public CDialog
{
public:
	DlgSuspend(CApplication* app) : CDialog(app) {}
	DlgSuspend(CApplication* app, CWindow* parent) : CDialog(app, parent) { }
	virtual ~DlgSuspend() {}

	virtual void create();
	virtual void close();
};



// 游戏内：对话框：游戏到达终点时弹出，包含BtOver
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

// 游戏内：按钮：游戏结束，由DlgEnd调用，点击触发DlgOver
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

// 游戏内：对话框：显示游戏结束后的得分信息，由BtOver触发
class DlgOver : public CDialog
{
public:
	DlgOver(CApplication* app, CWindow* parent) : CDialog(app, parent) {}
	virtual ~DlgOver() {}

	virtual void create();
};



// 游戏内：按钮：触发自动寻路
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

// 游戏内：按钮：触发演示动画
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

// 游戏内：按钮：点击以保存
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
