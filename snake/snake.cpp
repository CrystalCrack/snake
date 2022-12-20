/***********************************************************************
*遇到的问题：
* 1.如何使图像变得透明
* 2.如何移动贪吃蛇能让他始终接在一起
* 3.内部类和外部类之间怎么互相访问
*
*/


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
#include"procedure.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		cerr << "初始化 SDL 失败: " << SDL_GetError() << endl;
		return 1;
	}
	// 初始化音频
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cerr << "初始化音频失败:" << Mix_GetError() << endl;
		return 1;
	}

	HDW mode = MENU;
	int score;

	while (1) {
		switch (mode) {
		case MENU:
			mode = menu();
		case QUIT:
			break;
		}
		score = newgame(mode);
		mode = show_end_screen(score);
	}
	return 0;
}
