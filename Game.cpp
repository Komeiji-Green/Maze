///////////////////
//
//  cpp�ļ���Game.cpp
//  main�����ļ�
//

#include "Maze.h"
#include "CGameWidgets.h"

int main()
{
	initgraph(640, 480);	// ����ͼ��
	CApplication app;
	MainWindow* ptw = new MainWindow(&app, NULL);	// ����������ڶ��Ϸ��䣡
	setbkmode(TRANSPARENT);
	BeginBatchDraw();		// ��������ͼ
	app.setWatched(ptw);	// ������ʼ������
	ptw->create();			
	ptw->repaint();			// �������ػ�������
	app.exec();				// ������Ϣѭ��
	EndBatchDraw();			// ��������ͼ
	closegraph();			// �ر�ͼ��
	return 0;
}