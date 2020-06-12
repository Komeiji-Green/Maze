///////////////////////////////////
//
//   头文件：CObjects.h
//   作用：UI框架头文件
//   对应cpp文件：Objects.cpp
//
//   CApplication通过调用各组件的事件派发过程与消息响应过程，来完成消息循环。
//	 
//

#pragma once

#include "Maze.h"

	#pragma region 事件与消息

// 结构体：事件

enum EVENT_TYPE { M_MOVE = 1, M_LBUTTON, M_LRELEASE, K_PRESS };

// Event存储事件类型与具体的事件信息
struct Event
{
public:
	Event() {}
	// 通过键盘输入字符创建Event
	Event(char _ch) : 
		e_type(K_PRESS)
	{
		if (_ch == -32) // 方向键
		{
			char dir_ch = _getch();
			switch (dir_ch)
			{
			case 72:
				ch = 'w';
				break;
			case 75:
				ch = 'a';
				break;
			case 80:
				ch = 's';
				break;
			case 77:
				ch = 'd';
				break;
			}
		}
		else
			ch = _ch;
	}
	// 通过鼠标消息创建Event
	Event(MOUSEMSG M_msg): 
		m_msg(M_msg) 
	{
		switch (M_msg.uMsg)
		{
		case WM_LBUTTONDOWN :
			e_type = M_LBUTTON; break;
		case WM_LBUTTONUP :
			e_type = M_LRELEASE; break;
		case WM_MOUSEMOVE :
			e_type = M_MOVE; break;
		default:
			break;
		}
	}
	~Event() {}

	int x() { return m_msg.x; }
	int y() { return m_msg.y; }
	char getch() { return ch; }
	EVENT_TYPE type() { return e_type; }

private:
	char ch;
	MOUSEMSG m_msg;
	EVENT_TYPE e_type;

};

// 结构体：消息

class CObject;	// 前向声明

enum MSG_TYPE { W_CREATE = 1, W_PAINT, W_CLOSE, W_REFRESH, W_NEXT };

// Msg存储消息类型与目标组件
struct Msg
{
	Msg() {}
	Msg(MSG_TYPE tp, CObject *target): m_type(tp), target(target) { }
	MSG_TYPE m_type;
	CObject* target;
};

#pragma endregion

	#pragma region 组件与消息循环

class CApplication;	// 前向声明：CApplication与CObject双向依赖

// 所有界面组件的基类，依赖于CApplication
class CObject
{
public:
	CObject(CApplication *app): mainApp(app) {}
	virtual ~CObject() {}

public:
	void installEventFilter(CObject*);			// 事件：添加事件过滤器，注意过滤器注册关系不能出现环路
	void deleteEventFilter();
	bool event(Event *);						// 事件：调用eventFilter的前置函数
protected:
	virtual bool eventFilter(Event*) { return false; }
												// 事件：事件过滤器
public:
	virtual void msg(Msg*);						// 消息：消息派发函数
	virtual void create() {}					// 消息：用于处理W_CREATE型消息
	virtual void paint() {}						// 消息：用于处理W_PAINT型消息
	virtual void close();						// 消息：用于处理W_CLOSE型消息
	virtual void next() {}						// 消息：用于处理W_NEXT型消息
	virtual void deinstall() {}					// 用于使某些类解绑在接收到close消息时解绑过滤器。

public:
	void refresh();								// 公共接口：刷新数据
	void repaint();								// 公共接口：刷新画面
		
protected:
	vector<CObject*> eventFilterList;			// 事件过滤器列表

	CApplication* mainApp;						// 指向主事件循环的指针

};


// 所有窗口的基类，继承自CObject
class CWindow : public CObject
{
public:
	CWindow(CApplication *app): CObject(app) {}
	CWindow(CApplication* app, CWindow* parent_wnd):
		CObject(app), parent(parent_wnd) {}
	virtual ~CWindow() {}

	virtual void setParent(CWindow*);

protected:
	virtual bool eventFilter(Event*);
	CWindow* parent;	// 父窗口
};


// 所有主窗口的基类，继承自CWindow
class CMainWindow : public CWindow
{
public:
	CMainWindow(CApplication* app): CWindow(app) {}
	CMainWindow(CApplication* app, CWindow *parent_wnd):
		CWindow(app, parent_wnd) {}
	virtual ~CMainWindow() {}

	virtual void close();
};


// 所有对话框的基类，继承自CWindow
class CDialog : public CWindow
{
public:
	CDialog(CApplication* app): CWindow(app) {}
	CDialog(CApplication* app, CWindow* parent_wnd) :
		CWindow(app, parent_wnd) {}
	virtual ~CDialog() {}

	virtual void close();
	virtual void paint();
	virtual void create();
	virtual void deinstall();

	void setPattern(
		int px, int py,
		int wd, int ht,
		const wchar_t title[],
		wchar_t *text[],
		int text_line
	);
	void setPattern(
		int px, int py,
		int wd, int ht,
		const wchar_t title[],
		const wchar_t* text[],
		int text_line
	);

protected:
	const wchar_t* text[10];
	const wchar_t* title;
	int x, y, width, height;
	int text_line;
};


// 所有按钮的基类，继承自CObject，依赖于CWindow
class CButton : public CObject
{
public:
	CButton(CApplication* app): CObject(app) {}
	CButton(CApplication* app, CWindow* watched): CObject(app), parent(watched)
	{
		watched->installEventFilter(this);
	}
	virtual ~CButton() {}

	void setPattern(int x, int y, const wchar_t* bTtext);
	void setBtColor(COLORREF bTcolor);
	void setPos(int x, int y);

protected:
	bool inButton(int x, int y);
	bool Clicked(Event* ev);

	virtual bool eventFilter(Event*);
	virtual void paint();

	CWindow* parent;	// 按钮所在窗口
	int x, y;
	wchar_t text[50];
	COLORREF color;
	int left, top, right, bottom;
};


// 所有滑动条的基类，继承自CButton
class CSlider : public CButton
{
public:
	CSlider(CApplication* app, CWindow* watched): CButton(app, watched) {}
	virtual ~CSlider() {}

	// 左端点坐标、单像素数值跨度、总长度、左端点代表值、限制长度
	void setPattern(int _x, int _y, int _step, int _len, int _init, const wchar_t* _title, int _border = -1);
	void setBorder(int _border, bool invalue = false);
	int getValue();

protected:
	bool inSlider(int x, int y);
	bool Clicked(Event* ev);

	virtual bool eventFilter(Event*);
	virtual void paint();
	
	wchar_t title[10];
	int left_x, left_y, left_value;
	int step, len, border;

	int value;

	bool onSelected = false;
	int mxdiff = 0; // 鼠标初始偏移值，只在onSelected 为true 时允许
};


// 主事件循环，依赖于CWindow
class CApplication
{
public:
	CApplication() {}
	~CApplication() {}

	void exec();				// 事件循环

	void PeekEvent();			// 收集事件
	void TranslateEvent();		// 事件转换为消息
	void DispatchMsg();			// 事件分发

	void PushMsg(Msg *);		// 提供添加消息的接口

	void setWatched(CWindow* watched);		// 更改当前主窗口
	CWindow* getWatched();
	
private:
	Event* ev;					// 当前事件

	queue<Msg *> qMsg;			// 消息队列

	CWindow* watched;			// 当前主窗口

	/*
	// 定时器回调函数，用来定时生成一个T_TIME 消息
	static void CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	// 由于回调函数参数形式固定且为static，故需要提供一个pthis以便于它能够访问类内成员
	static CApplication* pthis;	

	static bool has_timer;
	*/
};

#pragma endregion
