#include"procedure.h"
#include"conio.h"

HDW menu()
{
	Mix_Chunk* chunk = Mix_LoadWAV(".\\musics\\bubble.wav");
	Mix_Music* music = Mix_LoadMUS(".\\musics\\Start-Menu.mp3");
	if (Mix_PlayMusic(music, -1) < 0) {
		cerr << "播放音乐失败: " << Mix_GetError() << endl;
	}

	IMAGE image;
	IMAGE inst_o, inst_m;

	loadimage(&image, _T(".\\images\\menu.jpg"), a, b);
	loadimage(&inst_o, L".\\images\\inst_o.jpg", a, b);
	loadimage(&inst_m, L".\\images\\inst_m.jpg", a, b);

	TRSP_IMAGE inst(inst_o, inst_m);

	initgraph(a, b);
	cleardevice();
	
	BeginBatchDraw();
	
	putimage(0, 0, &image);
	inst.drawimage(a - 94, 0);
	FlushBatchDraw();
	while (1)
	{
		ExMessage m;
		if (peekmessage(&m, EX_MOUSE))//获取鼠标信息
		{
			if (m.message == WM_LBUTTONUP && m.x > 932 && m.x < a && m.y>212 && m.y < 559) {
				instru();
				continue;
			}
			if (m.message == WM_MOUSEMOVE) {
				if (m.x > 272 && m.x < 752 && m.y>304 && m.y < 392) {//鼠标在开始游戏按钮区域内
					Mix_PlayChannel(-1,chunk, 0);
					loadimage(&image, _T(".\\images\\menu_start.jpg"), a, b);
					cleardevice();
					putimage(0, 0, &image);
					inst.drawimage(a - 94, 0);
					FlushBatchDraw();
					while (1) {//进入循环，等待鼠标按下或者鼠标离开区域
						peekmessage(&m, EX_MOUSE);
						if (!(m.x > 272 && m.x < 752 && m.y>304 && m.y < 392)) {//鼠标离开区域
							loadimage(&image, _T(".\\images\\menu.jpg"), a, b);
							cleardevice();
							putimage(0, 0, &image);
							inst.drawimage(a - 94, 0);
							FlushBatchDraw();
							break;//跳出循环
						}
						//检测鼠标是否按下
						switch (m.message) {
						case WM_LBUTTONUP:
							EndBatchDraw();
							Mix_HaltMusic();
							return NORMAL;
						case WM_RBUTTONUP:
							EndBatchDraw();
							Mix_HaltMusic();
							return CHICKEN;
						}
					}
				}
				else if (m.x > 272 && m.x < 752 && m.y>440 && m.y < 528) {//鼠标在退出游戏按钮区域内
					Mix_PlayChannel(-1, chunk, 0);
					loadimage(&image, _T(".\\images\\menu_exit.jpg"), a, b);
					cleardevice();
					putimage(0, 0, &image);
					inst.drawimage(a - 94, 0);
					FlushBatchDraw();
					while (1) {//进入循环，等待鼠标按下或者鼠标离开区域
						peekmessage(&m, EX_MOUSE);
						if (!(m.x > 272 && m.x < 752 && m.y>440 && m.y < 528)) {//鼠标离开区域
							loadimage(&image, _T(".\\images\\menu.jpg"), a, b);
							cleardevice();
							putimage(0, 0, &image);
							inst.drawimage(a - 94, 0);
							FlushBatchDraw();
							break;//跳出循环
						}
						//检测鼠标是否按下
						if (m.message == WM_LBUTTONUP) {
							EndBatchDraw();
							Mix_HaltMusic();
							return QUIT;
						}
					}
				}
			}
		}
	}
}

void instru() {
	Mix_Chunk* chunk = Mix_LoadWAV(".\\music\\pull-out.mp3");
	Mix_PlayChannel(-1, chunk, 0);
	IMAGE inst_o, inst_m;
	loadimage(&inst_o, L".\\images\\inst_o.jpg",a,b);
	loadimage(&inst_m, L".\\images\\inst_m.jpg",a,b);
	TRSP_IMAGE inst(inst_o, inst_m);
	IMAGE menu;
	loadimage(&menu, _T(".\\images\\menu.jpg"), a, b);
	putimage(0, 0, &menu);
	inst.drawimage(a-94, 0);
	FlushBatchDraw();
	ExMessage m;

	int x = a - 94;
	float i;
	clock_t start = clock();
	for (i = 2; i*i/3 <= 30; i+=0.4) {
		x -= i * i / 3;
		inst.drawimage(x, 0);
		FlushBatchDraw();
		Sleep(1000 / 60 - (clock() - start));
		start = clock();
	}
	while (x > 239) {
		if (x - 30 < 239)
			x = 239;
		else
			x -= 30;
		inst.drawimage(x, 0);
		FlushBatchDraw();
		Sleep(1000 / 60 - (clock() - start));
		start = clock();
	}
	for (i = 9.2; i >= 2; i -= 0.4) {
		x -= i * i / 3;
		inst.drawimage(x, 0);
		FlushBatchDraw();
		Sleep(1000 / 60 - (clock() - start));
		start = clock();
	}
	while (1) {
		peekmessage(&m, EX_MOUSE);
		if (m.message == WM_LBUTTONUP && m.x > 0 && m.x < 95 && m.y>212 && m.y < 559)
			break;
	}
	Mix_PlayChannel(-1, chunk, 0);
	start = clock();
	for (i = 2; i * i / 3 <= 30; i += 0.4) {
		x += i * i / 3;
		putimage(0, 0, &menu);
		inst.drawimage(x, 0);
		FlushBatchDraw();
		Sleep(1000 / 60 - (clock() - start));
		start = clock();
	}
	while (x < a - 333) {
		if (x + 30 > a - 333)
			x = a - 333;
		else
			x += 30;
		putimage(0, 0, &menu);
		inst.drawimage(x, 0);
		FlushBatchDraw();
		Sleep(1000 / 60 - (clock() - start));
		start = clock();
	}
	for (i = 9.6; i >= 2; i -= 0.4) {
		if (x + i * i / 3 > a - 94){
			x = a - 94;
		break;
		}
		else
			x += i * i / 3;
		putimage(0, 0, &menu);
		inst.drawimage(x, 0);
		FlushBatchDraw();
		Sleep(1000 / 60 - (clock() - start));
		start = clock();
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

int newgame(HDW mode) {
	srand(time(0));

	int score = 0;

	loadimage(&bg, _T(".\\images\\background.jpg"), 2000, 2000);//预加载

	Mix_Chunk* fail = Mix_LoadWAV(".\\musics\\normal_version\\fail.wav");
	Mix_Chunk* chunk = Mix_LoadWAV(".\\musics\\normal_version\\eaten!.mp3");
	Mix_Chunk* eat[5] = { Mix_LoadWAV(".\\musics\\chicken_version\\gene.mp3") ,Mix_LoadWAV(".\\musics\\chicken_version\\knee.mp3"),Mix_LoadWAV(".\\musics\\chicken_version\\tie.mp3"),Mix_LoadWAV(".\\musics\\chicken_version\\may.mp3"),Mix_LoadWAV(".\\musics\\chicken_version\\bro_kun.mp3") };
	Mix_Chunk* C = Mix_LoadWAV(".\\musics\\chicken_version\\C.mp3");
	Mix_Chunk* T = Mix_LoadWAV(".\\musics\\chicken_version\\T.mp3");
	Mix_Chunk* R = Mix_LoadWAV(".\\musics\\chicken_version\\R.mp3");
	Mix_Chunk* L = Mix_LoadWAV(".\\musics\\chicken_version\\L.mp3");
	Mix_Chunk* NGM = Mix_LoadWAV(".\\musics\\chicken_version\\NGM~.mp3");
	Mix_Chunk* mua = Mix_LoadWAV(".\\musics\\chicken_version\\mua.mp3");
	Mix_Chunk* getup = Mix_LoadWAV(".\\musics\\chicken_version\\getup.mp3");

	int current_music = rand() % 51;
	Mix_Music* music_list[51];
	Mix_Music* music = Mix_LoadMUS(".\\musics\\normal_version\\Gaming.flac");

	IMAGE o_u, o_d, o_r, o_l, m_u, m_d, m_r, m_l;
	IMAGE apple_m, apple_o;

	if (mode == NORMAL) {
		COL = 1024;
		RAW = 768;
		if (Mix_PlayMusic(music, 0) < 0) {
			cerr << "播放音乐失败: " << Mix_GetError() << endl;
		}
		//加载图片
		{
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
			initgraph(COL, RAW);
			loadimage(&bg, _T(".\\images\\background.jpg"), COL, RAW);
		}
		setorigin(COL / 2, RAW / 2);//设置逻辑坐标原点
		setaspectratio(1, -1);
	}
	else if (mode == CHICKEN) {
		COL = 1132;
		RAW = 622;
		//加载音乐
		{
			music_list[0] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (1).mp3");
			music_list[1] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (10).mp3");
			music_list[2] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (11).mp3");
			music_list[3] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (12).mp3");
			music_list[4] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (13).mp3");
			music_list[5] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (14).mp3");
			music_list[6] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (15).mp3");
			music_list[7] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (16).mp3");
			music_list[8] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (17).mp3");
			music_list[9] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (18).mp3");
			music_list[10] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (19).mp3");
			music_list[11] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (2).mp3");
			music_list[12] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (20).mp3");
			music_list[13] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (21).mp3");
			music_list[14] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (22).mp3");
			music_list[15] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (23).mp3");
			music_list[16] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (24).mp3");
			music_list[17] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (25).mp3");
			music_list[18] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (26).mp3");
			music_list[19] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (27).mp3");
			music_list[20] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (28).mp3");
			music_list[21] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (29).mp3");
			music_list[22] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (3).mp3");
			music_list[23] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (30).mp3");
			music_list[24] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (31).mp3");
			music_list[25] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (32).mp3");
			music_list[26] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (33).mp3");
			music_list[27] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (34).mp3");
			music_list[28] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (35).mp3");
			music_list[29] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (36).mp3");
			music_list[30] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (37).mp3");
			music_list[31] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (38).mp3");
			music_list[32] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (39).mp3");
			music_list[33] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (4).mp3");
			music_list[34] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (40).mp3");
			music_list[35] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (41).mp3");
			music_list[36] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (42).mp3");
			music_list[37] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (43).mp3");
			music_list[38] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (44).mp3");
			music_list[39] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (45).mp3");
			music_list[40] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (46).mp3");
			music_list[41] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (47).mp3");
			music_list[42] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (48).mp3");
			music_list[43] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (49).mp3");
			music_list[44] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (5).mp3");
			music_list[45] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (50).mp3");
			music_list[46] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (51).mp3");
			music_list[47] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (6).mp3");
			music_list[48] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (7).mp3");
			music_list[49] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (8).mp3");
			music_list[50] = Mix_LoadMUS(".\\musics\\chicken_version\\chicken_music (9).mp3");
		}
		//播放音乐
		if (Mix_PlayMusic(music_list[current_music], 0) < 0) {
			cerr << "播放音乐失败: " << Mix_GetError() << endl;
		}
		//加载图片
		{
			loadimage(&o_u, _T(".\\images\\chicken_version\\head_o_u.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&m_u, _T(".\\images\\chicken_version\\head_m_u.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&o_d, _T(".\\images\\chicken_version\\head_o_d.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&m_d, _T(".\\images\\chicken_version\\head_m_d.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&o_r, _T(".\\images\\chicken_version\\head_o_r.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&m_r, _T(".\\images\\chicken_version\\head_m_r.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&o_l, _T(".\\images\\chicken_version\\head_o_l.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&m_l, _T(".\\images\\chicken_version\\head_m_l.png"), DEFAULTSIZE + 15, DEFAULTSIZE + 15);
			loadimage(&apple_m, _T(".\\images\\chicken_version\\basketball_m.png"), DEFAULTSIZE, DEFAULTSIZE);
			loadimage(&apple_o, _T(".\\images\\chicken_version\\basketball_o.png"), DEFAULTSIZE, DEFAULTSIZE);
			initgraph(COL, RAW);
			loadimage(&bg, _T(".\\images\\chicken_version\\background.png"), COL, RAW);
		}
	}

	TRSP_IMAGE up(o_u, m_u);
	TRSP_IMAGE left(o_l, m_l);
	TRSP_IMAGE right(o_r, m_r);
	TRSP_IMAGE down(o_d, m_d);
	TRSP_IMAGE apple(apple_o, apple_m);

	APPLE::initialize(apple);

	SNAKE s(up, left, right, down, mode);

	clock_t start, end;
	start = clock();
	
	vector<APPLE> app;
	app.push_back(APPLE(s));

	s.addlength();
	s.addlength();
	s.addlength();

	int count = 0;

	ExMessage m;

	while (1) {
		//判断游戏音乐是否播放完毕
		if (Mix_PlayingMusic() == 0) {
			// 当前音乐已播放完毕，将当前音乐的索引设置为下一首音乐的索引
			current_music = (current_music + 1) % 51;
			// 重新开始播放
			Mix_PlayMusic(music_list[current_music], 0);
		}
		if (peekmessage(&m, EX_KEY))
			if (m.message == WM_KEYDOWN)
				switch (m.vkcode) {
				case 0x57:
				case VK_UP:
					if (s.getdir() == DOWN)
						break;
					s.turn(UP);
					if (mode == CHICKEN)
						Mix_PlayChannel(-1, C, 0);
					break;
				case 0x53:
				case VK_DOWN:
					if (s.getdir() == UP)
						break;
					s.turn(DOWN);
					if (mode == CHICKEN)
						Mix_PlayChannel(-1, R, 0);
					break;
				case 0x41:
				case VK_LEFT:
					if (s.getdir() == RIGHT)
						break;
					s.turn(LEFT);
					if (mode == CHICKEN)
						Mix_PlayChannel(-1, L, 0);
					break;
				case 0x44:
				case VK_RIGHT:
					if (s.getdir() == LEFT)
						break;
					s.turn(RIGHT);
					if (mode == CHICKEN)
						Mix_PlayChannel(-1, T, 0);
					break;
				case VK_SPACE:
					if (mode == CHICKEN) {
						Mix_PauseMusic();
						Mix_PlayChannel(-1, mua, 0);
					}
					while (1) {
						m = getmessage(EX_KEY);
						if (m.vkcode == VK_SPACE && m.message == WM_KEYDOWN) {
							if(mode==CHICKEN)
								Mix_PlayChannel(-1, getup, 0);
							Mix_ResumeMusic();
							break;
						}
					}
				case VK_ESCAPE:
					max_score = max_score < score ? score : max_score;
					Mix_HaltMusic();
					closegraph();
					return score;
				case VK_SHIFT:
					s.setspeed(5);
					break;
				}
			else if(m.message==WM_KEYUP)
				switch (m.vkcode) {
				case VK_SHIFT:
					s.setspeed(3);
				}
		s.move();
		int pos = s.iseaten(app);
		if (pos!=app.size()) {
			score++;
			if(mode==NORMAL)
				Mix_PlayChannel(-1, chunk, 0);
			else if (mode == CHICKEN) {
				Mix_PlayChannel(-1, eat[count], 0);
				count = (count + 1) % 5;
			}
			s.addlength();
			app.erase(app.begin()+pos);
			app.push_back(APPLE(s));
		}
		if (s.isdead()) {
			max_score = max_score < score ? score : max_score;
			Mix_HaltMusic();
			if (mode == CHICKEN) {
				Mix_PlayChannel(-1, NGM, 0);
				Sleep(3000);
			}
			else if (mode == NORMAL) {
				Mix_PlayChannel(-1, fail, 0);
				Sleep(3000);
			}

			closegraph();
			return score;
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

HDW show_end_screen(int score) {

	Mix_Chunk* chunk = Mix_LoadWAV(".\\musics\\bubble.wav");
	Mix_Music* music = Mix_LoadMUS(".\\musics\\Start-Menu.mp3");
	if (Mix_PlayMusic(music, -1) < 0) {
		cerr << "播放音乐失败: " << Mix_GetError() << endl;
	}
	
	//画图前准备
	initgraph(a, b);
	BeginBatchDraw();
	cleardevice();

	//声明背景图片
	IMAGE img;

	//声明消息盒子
	ExMessage m;

	//初始化分数字符串以及文字样式
	wchar_t arr[20];
	wchar_t max[20];
	swprintf_s(max, L"%d", max_score);
	swprintf_s(arr, L"%d", score);
	settextstyle(80, 40, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	//绘制初始界面
	loadimage(&img, _T(".\\images\\ending.jpg"), a, b);
	putimage(0, 0, &img);
	outtextxy(400, 560, arr);
	outtextxy(400, 640, max);
	FlushBatchDraw();

	while (1)
	{
		
		if (peekmessage(&m, EX_MOUSE))
		{
			if (m.message == WM_MOUSEMOVE)
			{
				if (m.x > 272 && m.x < 752 && m.y>304 && m.y < 392)
				{
					Mix_PlayChannel(-1, chunk, 0);
					//更换图片
					loadimage(&img, _T(".\\images\\end_retry.jpg"), a, b);
					cleardevice();
					putimage(0, 0, &img);
					outtextxy(400, 560, arr);
					outtextxy(400, 640, max);
					FlushBatchDraw();
					while (1)
					{
						peekmessage(&m, EX_MOUSE);
						if (!(m.x > 272 && m.x < 752 && m.y>304 && m.y < 392)) {
							//绘制初始界面
							loadimage(&img, _T(".\\images\\ending.jpg"), a, b);
							putimage(0, 0, &img);
							outtextxy(400, 560, arr);
							outtextxy(400, 640, max);
							FlushBatchDraw();
							break;
						}
						if (m.message == WM_LBUTTONUP) {
							EndBatchDraw();
							return NORMAL;
						}
						else if (m.message == WM_RBUTTONUP) {
							EndBatchDraw();
							return CHICKEN;
						}
					}
				}
				else if (m.x > 272 && m.x < 752 && m.y>440 && m.y < 528)
				{
					Mix_PlayChannel(-1, chunk, 0);
					loadimage(&img, _T(".\\images\\end_back.jpg"), a, b);
					cleardevice();
					putimage(0, 0, &img);
					outtextxy(400, 560, arr);
					outtextxy(400, 640, max);
					FlushBatchDraw();
					while (1)
					{
						peekmessage(&m, EX_MOUSE);
						if (!(m.x > 272 && m.x < 752 && m.y>440 && m.y < 528)) {
							//绘制初始界面
							loadimage(&img, _T(".\\images\\ending.jpg"), a, b);
							putimage(0, 0, &img);
							outtextxy(400, 560, arr);
							outtextxy(400, 640, max);
							FlushBatchDraw();
							break;
						}
						if (m.message == WM_LBUTTONUP) {
							EndBatchDraw();
							return MENU;
						}
					}
				}
			}
		}
	}
}