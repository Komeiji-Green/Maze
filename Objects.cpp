///////////////////////////////////
//
//   cpp文件：Objects.cpp
//   作用：UI框架类的实现
//   对应头文件：CObjects.h
//
//

#include "CObjects.h"

	#pragma region 主消息循环

// CApplication 实现

void CApplication::exec()
{
	//pthis = this;
	//SetTimer(NULL, 1, 100, TimerProc);
	while (watched != NULL)
	{
		PeekEvent();
		TranslateEvent();
		DispatchMsg();
	}
	//KillTimer(NULL, 1);
}

void CApplication::PeekEvent()
{
	if (_kbhit())
	{
		ev = new Event(_getch());
	} 
	else if (MouseHit())
	{
		ev = new Event(GetMouseMsg());
	}
	else 
	{
		ev = NULL;
		Sleep(16);
		PushMsg(new Msg(W_PAINT, watched));
	}
}

void CApplication::TranslateEvent()
{
	if (ev != NULL)
	{
		watched->event(ev);
		delete ev;
	}
}

void CApplication::DispatchMsg()
{
	Msg* pMsg;
	while (!qMsg.empty())
	{
		pMsg = qMsg.front();
		(pMsg->target)->msg(pMsg);
		qMsg.pop();
		delete pMsg;			// 释放pMsg 所指向的内存
	}
}

void CApplication::PushMsg(Msg *pMsg)
{
	qMsg.push(pMsg);
}

void CApplication::setWatched(CWindow* next_watched)
{
	watched = next_watched;		// 需要改动 ？
}

CWindow* CApplication::getWatched()
{
	return watched;
}

#pragma endregion

	#pragma region CObject

// CObject 实现

// 事件相关

bool CObject::event(Event *ev)
{
	for (int i = eventFilterList.size() - 1; i >= 0; i--)
	{
		if (eventFilterList[i]->event(ev))	// 如果事件已被上层过滤器拦截，返回true
			return true;
	}
	return eventFilter(ev);					// (this指针)执行对eventFilter的隐式动态绑定
}

void CObject::installEventFilter(CObject* pObj)
{
	eventFilterList.push_back(pObj);
}

void CObject::deleteEventFilter()
{
	eventFilterList.pop_back();
}

// 消息相关

void CObject::msg(Msg* m)
{
	switch (m->m_type)
	{
	case W_CREATE :
		create();
		repaint();
		break;
	case W_PAINT :
		repaint();
		break;
	case W_CLOSE :
		deinstall();
		close();
		break;
	case W_REFRESH :
		refresh();
		break;
	case W_NEXT :
		next();
		break;
	}
}

void CObject::close()
{
	// dfs：按覆盖次序重上而下析构对象
	for (auto pObj : eventFilterList)
	{
		pObj->close();
	}
	delete this;
}

void CObject::refresh()
{
	for (auto pObj : eventFilterList)
	{
		pObj->close();
	}
	vector<CObject*> temp;
	swap(temp, eventFilterList);	// 用于清空 FilterList
	this->create();						// 数据重置
}

void CObject::repaint()
{
	// bfs算法：按覆盖次序从下至上重刷画面
	CObject* pObj;
	queue<CObject*> q;
	q.push(this);
	while (!q.empty())
	{
		pObj = q.front();
		q.pop();
		pObj->paint();
		for (auto x : pObj->eventFilterList)
			q.push(x);
	}
	FlushBatchDraw();
}

#pragma endregion

	#pragma region CButton

// CButton 实现

bool CButton::eventFilter(Event* ev)
{
	switch (ev->type())
	{
	case M_MOVE :
		if (inButton(ev->x(), ev->y()))
			setBtColor(RGB(150, 150, 150));
		else
			setBtColor(RGB(100, 100, 100));
		break;
	case M_LBUTTON:
		if (inButton(ev->x(), ev->y()))
			setBtColor(RGB(100, 100, 100));
		break;
	case M_LRELEASE:
		if (inButton(ev->x(), ev->y()))
			setBtColor(RGB(100, 100, 100));
		break;
	}
	return false;	// 按钮不拦截 M_MOVE 事件
}

void CButton::paint()
{
	setfillcolor(color);
	solidrectangle(left, top, right, bottom);

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWidth = 0;
	wcscpy_s(f.lfFaceName, L"微软雅黑 Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);
	outtextxy(x, y + 3, text);	// 输出文字

	// Flush 注意！！
}

void CButton::setPattern(int p_x, int p_y, const wchar_t* bTtext)
{
	x = p_x;
	y = p_y;
	wcscpy_s(text, bTtext);
	left = x - 25;
	top = y;
	right = x + 19 * wcslen(text) + 25;
	bottom = y + 30;
}

void CButton::setBtColor(COLORREF bTcolor)
{
	color = bTcolor;
}

void CButton::setPos(int _x, int _y)
{
	x = _x;
	y = _y;
	left = x - 9;
	top = y - 9;
	right = x + 9;
	bottom = y + 9;
	text[0] = '\0';
}

bool CButton::inButton(int m_x, int m_y)
{
	return (m_x > left) && (m_x < right) && (m_y > top) && (m_y < bottom);
}

bool CButton::Clicked(Event* ev)
{
	return (ev->type() == M_LBUTTON) && inButton(ev->x(), ev->y());
}

#pragma endregion

	#pragma region CSlider

void CSlider::setPattern(int _x, int _y, int _step, int _len, int _init, const wchar_t* _title, int _border)
{
	CButton::setPos(_x, _y);
	CButton::setBtColor(RGB(100, 100, 100));
	left_x = _x;
	left_y = _y;
	step = _step;
	len = _len;
	left_value = _init;
	value = _init;
	wcscpy_s(title, _title);
	if (_border == -1)
		border = len;
	else
		setBorder(_border);
}

void CSlider::setBorder(int _v, bool invalue)
{
	int _border = _v;
	if (invalue)
	{
		_border = (_v - left_value) / step;
	}
	if (_border >= 0 && _border <= len)
	{
		border = _border;
		if (!inSlider(CButton::x, CButton::y))
			CButton::setPos(left_x + border, left_y);
	}
	else if (_border < 0)
		border = 0;
	else
		border = len;
}

int CSlider::getValue()
{
	return value;
}

bool CSlider::inSlider(int _x, int _y)
{
	return (_x >= left_x) && (_x <= left_x + border) && (_y >= left_y - 5) && (_y <= left_y + 5);
}

bool CSlider::Clicked(Event* ev)
{
	return (ev->type() == M_LBUTTON) && inSlider(ev->x(), ev->y());
}

bool CSlider::eventFilter(Event* ev)
{
	if (!onSelected)
	{
		if (CButton::Clicked(ev))
		{
			onSelected = true;
			mxdiff = CButton::x - ev->x();
		}
		else if (Clicked(ev))
		{
			CButton::setPos(ev->x(), left_y);
			value = (ev->x() - left_x) * step + left_value;
		}
	}
	else
	{
		if (ev->type() == M_MOVE)
		{
			int temp_x = ev->x() + mxdiff;
			if (inSlider(temp_x, left_y))
			{
				CButton::setPos(temp_x, left_y);
				value = (temp_x - left_x) * step + left_value;
			}
		}
		else if (ev->type() == M_LRELEASE)
		{
			onSelected = false;
			mxdiff = 0;
		}
	}
	return CButton::eventFilter(ev);
}

void CSlider::paint()
{
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 18;
	f.lfWidth = 0;
	wcscpy_s(f.lfFaceName, L"微软雅黑 Light");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(WHITE);
	outtextxy(left_x, y - 25, title);	// 输出文字

	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 4);
	setlinecolor(RGB(40, 40, 40));
	line(left_x, left_y, left_x + len, left_y);

	wchar_t strvalue[10];
	swprintf_s(strvalue, L"%d", value);
	outtextxy(left_x + len + 9, y - 9, strvalue);

	CButton::paint();
}

#pragma endregion

	#pragma region CWindow

// CWindow

void CWindow::setParent(CWindow* p_wnd)
{
	parent = p_wnd;
}

bool CWindow::eventFilter(Event* ev)
{
	if (ev->type() == M_MOVE)
	{
		mainApp->PushMsg(new Msg(W_PAINT, this));
	}
	return true;	// 拦截任何事件，此程序内所有窗口均为阻塞窗口。
}

#pragma endregion

	#pragma region CMainWindow&CDialog

// CMainWindow

void CMainWindow::close()
{
	mainApp->setWatched(parent);	// close 返回后，回到父窗口的事件循环
	if (parent != NULL)
		parent->repaint();			// 重刷画面
	CObject::close();				// 销毁
}

// CDialog

void CDialog::create()
{
	parent->installEventFilter(this);	// 等到create时才绑定过滤器
}

void CDialog::close()
{
	for (int i = 0; i < text_line; i++)
		delete[] text[i];
	CObject::close();
}

void CDialog::deinstall()
{
	parent->deleteEventFilter();
}

void CDialog::paint()
{
	setfillcolor(RGB(25, 25, 25));
	solidrectangle(x, y, x + width, y + height); // 绘制边框

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWidth = 0;
	wcscpy_s(f.lfFaceName, L"黑体");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	outtextxy(x + 20, y + 10, title);

	for (int i = 0; i < text_line && i < 10; i++)		// 输出标题（正文）
	{
		f.lfHeight = 18;
		f.lfQuality = ANTIALIASED_QUALITY;
		settextstyle(&f);
		outtextxy(x + 20, y + 45 + i * 20, text[i]);
	}
}

void CDialog::setPattern(
	int px, int py,
	int wd, int ht,
	const wchar_t temptitle[],
	const wchar_t* temptext[],
	int t_num
)
{
	x = px;
	y = py;
	width = wd;
	height = ht;
	title = temptitle;
	text_line = t_num;
	for (int i = 0; i < t_num && i < 10; i++)
	{
		text[i] = temptext[i];
	}
}

void CDialog::setPattern(
	int px, int py,
	int wd, int ht,
	const wchar_t temptitle[],
	wchar_t* temptext[],
	int t_num
)
{
	setPattern(px, py, wd, ht, temptitle, const_cast<const wchar_t**>(temptext), t_num);
}

#pragma endregion

	/*

// OpenButton 与 CloseButton

void OpenButton::setTargetWnd(CWindow* wnd)
{
	target_wnd = wnd;
}

bool OpenButton::eventFilter(Event* ev)
{
	switch (ev->type())
	{
	case M_LBUTTON :
		target_wnd->setParent(watched_wnd);		// 窗口注册：将目标窗口的父窗口设置为本窗口
		mainApp->PushMsg(new Msg(W_CREATE, target_wnd));	// 窗口创建：调用目标窗口 create 函数
		// 根据是否为主窗口改变Main watched （是否有必要？:这一项应该在窗口消息过程函数内进行）
		return true;	// 拦截
		break;
	}
	return CButton::eventFilter(ev);
}


void CloseButton::setTargetWnd(CWindow* wnd)
{
	target_wnd = wnd;
}

bool CloseButton::eventFilter(Event* ev)
{
	switch (ev->type())
	{
	case M_LBUTTON :
		mainApp->PushMsg(new Msg(W_CLOSE, target_wnd));
		return true;
		break;
	}
	return CButton::eventFilter(ev);
}

*/

	/*

// 定时器回调函数：每隔100ms 设置has_timer
void CALLBACK CApplication::TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (iTimerID == 1)
		pthis->has_timer = true;
}

CApplication* CApplication::pthis = NULL;
bool CApplication::has_timer = true;

*/