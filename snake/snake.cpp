/***********************************************************************
*遇到的问题：
* 1.如何使图像变得透明
* 2.如何移动贪吃蛇能让他始终接在一起
* 3.内部类和外部类之间怎么互相访问
*
*/


//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
#include<easyx.h>
#include<cstdio>
#include"GameObject.h"

void draw(SNAKE s) {
	putimage(-COL / 2, -RAW / 2, &bg);
	s.drawsnake();
}

int main() {
	initgraph(COL, RAW);
	loadimage(&bg, _T(".\\images\\background.jpg"), COL, RAW);
	setorigin(COL / 2, RAW / 2);//设置逻辑坐标原点

	setaspectratio(1, -1);
	IMAGE origin, mask;
	loadimage(&origin, _T(".\\images\\head_o.png"), DEFAULTSIZE, DEFAULTSIZE);
	loadimage(&mask, _T(".\\images\\head_m.png"), DEFAULTSIZE, DEFAULTSIZE);
	TRSP_IMAGE img1(origin, mask);
	TRSP_IMAGE img2(origin, mask);
	TRSP_IMAGE img3(origin, mask);
	TRSP_IMAGE img4(origin, mask);
	SNAKE s(img1, img2, img3, img4);
	for (int i = 0; i < 100; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	s.turn(UP);
	for (int i = 0; i < 30; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	s.turn(LEFT);
	for (int i = 0; i < 100; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	s.turn(DOWN);
	for (int i = 0; i < 30; i++) {
		cleardevice();
		BeginBatchDraw();
		draw(s);
		FlushBatchDraw();
		s.move();
		Sleep(10);
	}
	getchar();
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
