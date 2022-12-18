#include <stdio.h>
#include <easyx.h>
#include<iostream>
#include <ctime>
#include<vector>
#include<SDL.h>
#include<SDL_mixer.h>
#include"GameObject.h"
#define a 1280
#define b 960
using namespace std;

bool menu();
void draw(SNAKE s, vector<APPLE>& apple);
void newgame();

bool menu()
{
	Mix_Chunk* chunk = Mix_LoadWAV(".\\musics\\bubble.wav");
	Mix_Music* music = Mix_LoadMUS(".\\musics\\Start-Menu.mp3");
	if (Mix_PlayMusic(music, -1) < 0) {
		cerr << "播放音乐失败: " << Mix_GetError() << endl;
	}
	initgraph(a, b);
	cleardevice();
	IMAGE image;
	BeginBatchDraw();
	loadimage(&image, _T(".\\images\\贪吃蛇menu.jpg"), a, b);
	putimage(0, 0, &image);
	FlushBatchDraw();
	while (1)
	{
		ExMessage m;
		if (peekmessage(&m, EX_MOUSE))//获取鼠标信息
		{
			if (m.message == WM_MOUSEMOVE) {
				if (m.x > 340 && m.x < 940 && m.y>380 && m.y < 490) {//鼠标在开始游戏按钮区域内
					Mix_PlayChannel(-1,chunk, 0);
					loadimage(&image, _T(".\\images\\menu按钮按下开始游戏.jpg"), a, b);
					cleardevice();
					putimage(0, 0, &image);
					FlushBatchDraw();
					while (1) {//进入循环，等待鼠标按下或者鼠标离开区域
						peekmessage(&m, EX_MOUSE);
						if (!(m.x > 340 && m.x < 940 && m.y>380 && m.y < 490)) {//鼠标离开区域
							loadimage(&image, _T(".\\images\\贪吃蛇menu.jpg"), a, b);
							cleardevice();
							putimage(0, 0, &image);
							FlushBatchDraw();
							break;//跳出循环
						}
						//检测鼠标是否按下
						if (m.message == WM_LBUTTONDOWN) {
							EndBatchDraw();
							Mix_HaltMusic();
							return true;
						}
					}
				}
				else if (m.x > 340 && m.x < 940 && m.y>550 && m.y < 660) {//鼠标在退出游戏按钮区域内
					Mix_PlayChannel(-1, chunk, 0);
					loadimage(&image, _T(".\\images\\menu按钮按下退出游戏.jpg"), a, b);
					cleardevice();
					putimage(0, 0, &image);
					FlushBatchDraw();
					while (1) {//进入循环，等待鼠标按下或者鼠标离开区域
						peekmessage(&m, EX_MOUSE);
						if (!(m.x > 340 && m.x < 940 && m.y>550 && m.y < 660)) {//鼠标离开区域
							loadimage(&image, _T(".\\images\\贪吃蛇menu.jpg"), a, b);
							cleardevice();
							putimage(0, 0, &image);
							FlushBatchDraw();
							break;//跳出循环
						}
						//检测鼠标是否按下
						if (m.message == WM_LBUTTONDOWN) {
							EndBatchDraw();
							Mix_HaltMusic();
							return false;
						}
					}
				}
			}
		}
	}
}


void draw(SNAKE s,vector<APPLE> &apple) {
	setaspectratio(1, 1);
	setorigin(0, 0);
	putimage(0, 0, &bg);
	setorigin(COL / 2, RAW / 2);//设置逻辑坐标原点
	setaspectratio(1, -1);
	for (int i = 0; i < apple.size(); i++) {
		apple[i].putapple();
	}
	s.drawsnake();
}

void newgame() {
	Mix_Chunk* chunk = Mix_LoadWAV(".\\musics\\eaten!.mp3");
	Mix_Music* music = Mix_LoadMUS(".\\musics\\Gaming.flac");
	if (Mix_PlayMusic(music, -1) < 0) {
		cerr << "播放音乐失败: " << Mix_GetError() << endl;
	}

	initgraph(COL, RAW);
	loadimage(&bg, _T(".\\images\\background.jpg"), COL, RAW);

	setorigin(COL / 2, RAW / 2);//设置逻辑坐标原点
	setaspectratio(1, -1);

	IMAGE o_u, o_d, o_r, o_l, m_u, m_d, m_r, m_l;
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

	clock_t start, end;
	start = clock();
	
	vector<APPLE> app;
	app.push_back(APPLE());

	s.addlength();
	s.addlength();
	s.addlength();

	while (1) {
		ExMessage m;
		if (peekmessage(&m, EX_KEY))
			if (m.message == WM_KEYDOWN)
				switch (m.vkcode) {
				case 0x57:
				case VK_UP:
					if (s.getdir() == DOWN)
						break;
					s.turn(UP);
					break;
				case 0x53:
				case VK_DOWN:
					if (s.getdir() == UP)
						break;
					s.turn(DOWN);
					break;
				case 0x41:
				case VK_LEFT:
					if (s.getdir() == RIGHT)
						break;
					s.turn(LEFT);
					break;
				case 0x44:
				case VK_RIGHT:
					if (s.getdir() == LEFT)
						break;
					s.turn(RIGHT);
					break;
				case VK_SPACE:
					while (1) {
						m = getmessage(EX_KEY);
						if (m.vkcode == VK_SPACE && m.message == WM_KEYDOWN)
							break;
					}
				}
		s.move();
		int pos = s.iseaten(app);
		if (pos!=app.size()) {
			Mix_PlayChannel(-1, chunk, 0);
			s.addlength();
			app.erase(app.begin()+pos);
			app.push_back(APPLE());
		}
		if (s.isdead()) {
			Mix_HaltMusic();
			closegraph();
			return;
		}
		end = clock();
		long t = static_cast<long double>(end - start) / CLOCKS_PER_SEC * 1000;
		start = end;
		if (t < 10) {
			Sleep(10 - t);
		}
		BeginBatchDraw();
		cleardevice();
		draw(s,app);
		FlushBatchDraw();
		EndBatchDraw();
	}
}

