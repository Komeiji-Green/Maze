///////////////////
//
//  cpp文件：Game.cpp
//  main函数文件
//

#include "Maze.h"
#include "CGameWidgets.h"

int main()
{
	initgraph(640, 480);	// 生成图窗
	CApplication app;
	MainWindow* ptw = new MainWindow(&app, NULL);	// 窗口类必须在堆上分配！
	setbkmode(TRANSPARENT);
	BeginBatchDraw();		// 开启批绘图
	app.setWatched(ptw);	// 设置起始主窗口
	ptw->create();			
	ptw->repaint();			// 创建与重绘主窗口
	app.exec();				// 开启消息循环
	EndBatchDraw();			// 结束批绘图
	closegraph();			// 关闭图窗
	return 0;
}