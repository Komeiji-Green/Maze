///////////////////////////////////
//
//   ͷ�ļ���CObjects.h
//   ���ã�UI���ͷ�ļ�
//   ��Ӧcpp�ļ���Objects.cpp
//
//   CApplicationͨ�����ø�������¼��ɷ���������Ϣ��Ӧ���̣��������Ϣѭ����
//	 
//

#pragma once

#include "Maze.h"

	#pragma region �¼�����Ϣ

// �ṹ�壺�¼�

enum EVENT_TYPE { M_MOVE = 1, M_LBUTTON, M_LRELEASE, K_PRESS };

// Event�洢�¼������������¼���Ϣ
struct Event
{
public:
	Event() {}
	// ͨ�����������ַ�����Event
	Event(char _ch) : 
		e_type(K_PRESS)
	{
		if (_ch == -32) // �����
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
	// ͨ�������Ϣ����Event
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

// �ṹ�壺��Ϣ

class CObject;	// ǰ������

enum MSG_TYPE { W_CREATE = 1, W_PAINT, W_CLOSE, W_REFRESH, W_NEXT };

// Msg�洢��Ϣ������Ŀ�����
struct Msg
{
	Msg() {}
	Msg(MSG_TYPE tp, CObject *target): m_type(tp), target(target) { }
	MSG_TYPE m_type;
	CObject* target;
};

#pragma endregion

	#pragma region �������Ϣѭ��

class CApplication;	// ǰ��������CApplication��CObject˫������

// ���н�������Ļ��࣬������CApplication
class CObject
{
public:
	CObject(CApplication *app): mainApp(app) {}
	virtual ~CObject() {}

public:
	void installEventFilter(CObject*);			// �¼�������¼���������ע�������ע���ϵ���ܳ��ֻ�·
	void deleteEventFilter();
	bool event(Event *);						// �¼�������eventFilter��ǰ�ú���
protected:
	virtual bool eventFilter(Event*) { return false; }
												// �¼����¼�������
public:
	virtual void msg(Msg*);						// ��Ϣ����Ϣ�ɷ�����
	virtual void create() {}					// ��Ϣ�����ڴ���W_CREATE����Ϣ
	virtual void paint() {}						// ��Ϣ�����ڴ���W_PAINT����Ϣ
	virtual void close();						// ��Ϣ�����ڴ���W_CLOSE����Ϣ
	virtual void next() {}						// ��Ϣ�����ڴ���W_NEXT����Ϣ
	virtual void deinstall() {}					// ����ʹĳЩ�����ڽ��յ�close��Ϣʱ����������

public:
	void refresh();								// �����ӿڣ�ˢ������
	void repaint();								// �����ӿڣ�ˢ�»���
		
protected:
	vector<CObject*> eventFilterList;			// �¼��������б�

	CApplication* mainApp;						// ָ�����¼�ѭ����ָ��

};


// ���д��ڵĻ��࣬�̳���CObject
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
	CWindow* parent;	// ������
};


// ���������ڵĻ��࣬�̳���CWindow
class CMainWindow : public CWindow
{
public:
	CMainWindow(CApplication* app): CWindow(app) {}
	CMainWindow(CApplication* app, CWindow *parent_wnd):
		CWindow(app, parent_wnd) {}
	virtual ~CMainWindow() {}

	virtual void close();
};


// ���жԻ���Ļ��࣬�̳���CWindow
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


// ���а�ť�Ļ��࣬�̳���CObject��������CWindow
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

	CWindow* parent;	// ��ť���ڴ���
	int x, y;
	wchar_t text[50];
	COLORREF color;
	int left, top, right, bottom;
};


// ���л������Ļ��࣬�̳���CButton
class CSlider : public CButton
{
public:
	CSlider(CApplication* app, CWindow* watched): CButton(app, watched) {}
	virtual ~CSlider() {}

	// ��˵����ꡢ��������ֵ��ȡ��ܳ��ȡ���˵����ֵ�����Ƴ���
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
	int mxdiff = 0; // ����ʼƫ��ֵ��ֻ��onSelected Ϊtrue ʱ����
};


// ���¼�ѭ����������CWindow
class CApplication
{
public:
	CApplication() {}
	~CApplication() {}

	void exec();				// �¼�ѭ��

	void PeekEvent();			// �ռ��¼�
	void TranslateEvent();		// �¼�ת��Ϊ��Ϣ
	void DispatchMsg();			// �¼��ַ�

	void PushMsg(Msg *);		// �ṩ�����Ϣ�Ľӿ�

	void setWatched(CWindow* watched);		// ���ĵ�ǰ������
	CWindow* getWatched();
	
private:
	Event* ev;					// ��ǰ�¼�

	queue<Msg *> qMsg;			// ��Ϣ����

	CWindow* watched;			// ��ǰ������

	/*
	// ��ʱ���ص�������������ʱ����һ��T_TIME ��Ϣ
	static void CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	// ���ڻص�����������ʽ�̶���Ϊstatic������Ҫ�ṩһ��pthis�Ա������ܹ��������ڳ�Ա
	static CApplication* pthis;	

	static bool has_timer;
	*/
};

#pragma endregion
