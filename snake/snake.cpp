/***********************************************************************
*遇到的问题：
* 1.如何使图像变得透明
* 2.如何移动贪吃蛇能让他始终接在一起
* 3.内部类和外部类之间怎么互相访问
*
*/


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
#include<easyx.h>
#include<cstdio>
#include"GameObject.h"
#include <ctime>

void draw(SNAKE s) {
	putimage(-COL / 2, -RAW / 2, &bg);
	putapple();
	s.drawsnake();
}

int main() {
	int a[][4] = { 0,0 };

	initgraph(COL, RAW);
	loadimage(&bg, _T(".\\images\\background.jpg"), COL, RAW);

	setorigin(COL / 2, RAW / 2);//设置逻辑坐标原点
	setaspectratio(1, -1);

	IMAGE o_u,o_d,o_r,o_l,m_u,m_d,m_r,m_l;
	IMAGE apple_m, apple_o;

	loadimage(&o_u, _T(".\\images\\head_o_u.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&m_u, _T(".\\images\\head_m_u.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&o_d, _T(".\\images\\head_o_d.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&m_d, _T(".\\images\\head_m_d.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&o_r, _T(".\\images\\head_o_r.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&m_r, _T(".\\images\\head_m_r.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&o_l, _T(".\\images\\head_o_l.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&m_l, _T(".\\images\\head_m_l.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&apple_m, _T(".\\images\\craw_m.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&apple_o, _T(".\\images\\craw_o.png"), DEFAULTSIZE, DEFAULTSIZE);

	TRSP_IMAGE up(o_u, m_u);
	TRSP_IMAGE left(o_l, m_l);
	TRSP_IMAGE right(o_r, m_r);
	TRSP_IMAGE down(o_d, m_d);
	TRSP_IMAGE apple(apple_o, apple_m);

	APPLE::initialize(apple);

	SNAKE s(up, left, right, down);

	clock_t start,end;
	start = clock();
	
	addapple();

	while (1) {
		ExMessage m;
		if(peekmessage(&m, EX_KEY))
			if(m.message==WM_KEYDOWN)
			switch (m.vkcode) {
			case 0x57:
				s.turn(UP);
				break;
			case 0x53:
				s.turn(DOWN);
				break;
			case 0x41:
				s.turn(LEFT);
				break;
			case 0x44:
				s.turn(RIGHT);
				break;
			}
		s.move();
		if (s.iseaten())
			addapple();
		end = clock();
		long t = static_cast<long double>(end - start) / CLOCKS_PER_SEC * 1000;
		start = end;
		if (t < 10) {
			Sleep(10 - t);
		}
		BeginBatchDraw();
		cleardevice();
		draw(s);
		FlushBatchDraw();
	}

	/*
	for (int i = 0; i < 100; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	s.turn(UP);
	s.addlength();
	s.addlength();
	for (int i = 0; i < 30; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	s.turn(LEFT);
	s.addlength();
	s.addlength();
	for (int i = 0; i < 100; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	s.turn(DOWN);
	s.addlength();
	s.addlength();
	for (int i = 0; i < 30; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	getchar();
	*/
	//s.drawsnake();
	/*
	IMAGE BG, IMG1, IMG2;
	loadimage(&BG, _T(".\\background.jpg"), 1660, 900);
	loadimage(&IMG1, _T(".\\head1.png"), 60, 60);
	loadimage(&IMG2, _T(".\\head2.png"), 60, 60);
	TRSP_IMAGE image(IMG1, IMG2);
	initgraph(1660, 900);
	putimage(0, 0, &BG);
	image.drawimage(800,500);
	getchar();
	*/
}
