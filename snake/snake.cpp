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
	s.addlength();
	s.addlength();
	s.addlength();

	while (1) {
		ExMessage m;
		if(peekmessage(&m, EX_KEY))
			if(m.message==WM_KEYDOWN)
			switch (m.vkcode) {
			case 0x57:
				if (s.getdir() == DOWN)
					break;
				s.turn(UP);
				break;
			case 0x53:
				if (s.getdir() == UP)
					break;
				s.turn(DOWN);
				break;
			case 0x41:
				if (s.getdir() == RIGHT)
					break;
				s.turn(LEFT);
				break;
			case 0x44:
				if (s.getdir() == LEFT)
					break;
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
}
