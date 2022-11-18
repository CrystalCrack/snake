#include<iostream>
#include<string>
#include<cstdlib>
#include<Windows.h>
#include<conio.h>
#include<vector>
#include<stdlib.h>
#define RAW 40
#define COL 120
using namespace std;

enum object { AIR, SNAKE_HEAD, SNAKE_BODY, APPLE };
enum direct { NONE, UP, DOWN, LEFT, RIGHT };
enum status { CONTINUE, END };

int maxscore;
int score = 0;
int cnt = 0;

object map[RAW][COL] = { AIR };
direct sdir[RAW][COL] = { NONE };//蛇的方向

struct snake {
	snake(vector<int> h, vector<int> t, direct d, int len) :head(h), tail(t),
		dir(d), length(len) {}
	vector<int> head;
	vector<int> tail;
	direct dir;
	int length;
};

void drawmap();//打印地图
snake* initialize();//初始化
int loop(snake* snk);//游戏进行
status movehead(snake* snk);//蛇移动
void movetail(snake* snk);
void addapple();
bool isdead(snake* snk);//判断是否死亡
bool iseaten(snake* snk);//判断是否吃到苹果
int showend(snake* snk, int exitcode);//展示结算画面
void gotoxy(int x, int y);
void freshmap(int x, int y);

int main() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
	system("title 贪吃蛇");
	while (1) {
		snake* snk = initialize();
		drawmap();
		if (!showend(snk, loop(snk)))
			break;
	}
	return 0;
}

void drawmap() {
	gotoxy(0, 0);
	for (int i = 0; i <= COL; i++)
		cout << R"(━)";
	cout << R"(━)" << endl;
	for (int i = 0; i < RAW; i++) {
		cout << R"(┃)";
		for (int j = 0; j < COL; j++) {
			switch (map[i][j]) {
			case AIR:
				cout << ' ';
				break;
			case SNAKE_HEAD:
				cout << '#';
				break;
			case SNAKE_BODY:
				cout << '*';
				break;
			case APPLE:
				cout << '+';
				break;
			}
		}
		cout << R"(┃)" << endl;
	}
	for (int i = 0; i < COL; i++)
		cout << R"(━)";
	cout << R"(━)" << endl;
	cout << '\t' << "Score:" << score << '\t' << "Max score:" << maxscore << endl;
}

snake* initialize() {
	srand(time(0));
	memset(map, AIR, sizeof(map));
	memset(sdir, NONE, sizeof(sdir));
	vector<int> head = { rand() % (RAW - 14) + 7,rand() % COL };
	snake* snk = new snake(head, { head[0] - 4,head[1] }, DOWN, 5);
	map[head[0]][head[1]] = SNAKE_HEAD;
	sdir[head[0]][head[1]] = DOWN;
	for (int i = 1; i < 5; i++) {
		map[head[0] - i][head[1]] = SNAKE_BODY;
		sdir[head[0] - i][head[1]] = DOWN;
	}
	return snk;
}

int loop(snake* snk) {
	while (1) {
		int ch;
		if (_kbhit()) {
			ch = _getch();
			switch (ch) {
			case 65://A
			case 97://a
			case 75://left
				if (map[snk->head[0]][snk->head[1] - 1] == SNAKE_BODY)
					break;
				snk->dir = LEFT;
				break;
			case 68://D
			case 100://d
			case 77://right
				if (map[snk->head[0]][snk->head[1] + 1] == SNAKE_BODY)
					break;
				snk->dir = RIGHT;
				break;
			case 83://S
			case 115://s
			case 80://down
				if (map[snk->head[0] + 1][snk->head[1]] == SNAKE_BODY)
					break;
				snk->dir = DOWN;
				break;
			case 87://W
			case 119://w
			case 72://up
				if (map[snk->head[0] - 1][snk->head[1]] == SNAKE_BODY)
					break;
				snk->dir = UP;
				break;
			case 27://ESC
				return 0;
			}
		}
		if (cnt < 5)
			cnt++;
		else {
			addapple();
			if (movehead(snk) == END) {
				return 1;
			}
			cnt = 1;
		}
		Sleep(1);
	}
}

status movehead(snake* snk) {
	map[snk->head[0]][snk->head[1]] = SNAKE_BODY;//原来的蛇头换成蛇身
	freshmap(snk->head[0], snk->head[1]);//更新地图
	sdir[snk->head[0]][snk->head[1]] = snk->dir;//更新方向
	switch (snk->dir) {
	case UP:
		snk->head[0]--;
		break;
	case DOWN:
		snk->head[0]++;
		break;
	case LEFT:
		snk->head[1]--;
		break;
	case RIGHT:
		snk->head[1]++;
		break;
	}
	if (isdead(snk)) {
		return END;
	}
	if (iseaten(snk)) {//不移动蛇尾
		snk->length++;//增加长度
	}
	else {
		movetail(snk);//移动蛇尾
	}
	map[snk->head[0]][snk->head[1]] = SNAKE_HEAD;//移动蛇头
	freshmap(snk->head[0], snk->head[1]);//更新地图
	return CONTINUE;
}

void movetail(snake* snk) {
	map[snk->tail[0]][snk->tail[1]] = AIR;//移除蛇尾
	freshmap(snk->tail[0], snk->tail[1]);
	switch (sdir[snk->tail[0]][snk->tail[1]]) {
	case UP:
		snk->tail[0]--;
		sdir[snk->tail[0] + 1][snk->tail[1]] = NONE;//移除方向
		break;
	case DOWN:
		snk->tail[0]++;
		sdir[snk->tail[0] - 1][snk->tail[1]] = NONE;//移除方向
		break;
	case LEFT:
		snk->tail[1]--;
		sdir[snk->tail[0]][snk->tail[1] + 1] = NONE;//移除方向
		break;
	case RIGHT:
		snk->tail[1]++;
		sdir[snk->tail[0]][snk->tail[1] - 1] = NONE;//移除方向
	}
}

bool isdead(snake* snk) {//判断蛇是否死亡
	if (snk->head[0] == RAW || //蛇碰到下边墙壁
		snk->head[0] == -1 || //蛇碰到上边墙壁
		snk->head[1] == COL || //碰到右边墙壁
		snk->head[1] == -1 || //碰到左边墙壁
		map[snk->head[0]][snk->head[1]] == SNAKE_BODY)//撞到自己
	{
		return true;
	}
	return false;
}

bool iseaten(snake* snk) {
	if (map[snk->head[0]][snk->head[1]] == APPLE) {
		score++;
		if (score > maxscore)
			maxscore = score;
		gotoxy(0, RAW + 2);
		cout << '\t' << "Score:" << score << '\t' << "Max score:" << maxscore << endl;
		return true;
	}
	else
		return false;
}

int showend(snake* snk, int exitcode) {
	system("cls");
	if (exitcode == 0) {
		cout << R"(━━━━━━━━━━━━━━━━━━━━━━━━━━━━
┃                          ┃
┃                          ┃
┃       forced quit        ┃
┃                          ┃
┃                          ┃
━━━━━━━━━━━━━━━━━━━━━━━━━━━━)";
	}
	else {
		cout << R"(━━━━━━━━━━━━━━━━━━━━━━━━━━━━
┃                          ┃
┃                          ┃
┃         you died!        ┃
┃                          ┃
┃                          ┃
━━━━━━━━━━━━━━━━━━━━━━━━━━━━)";
	}
	if (score > maxscore)maxscore = score;
	cout << "snake length:" << snk->length << endl;
	cout << "score:" << score << endl;
	cout << "max score:" << maxscore << endl;
	if (score == maxscore)
		cout << "Congratulations! You just break your records!" << endl
		<< "Wanna try again?" << endl
		<< "[y/n]" << endl;
	score = 0;
	char opt;
	while (1) {
		cin >> opt;
		if (opt == 'y' || opt == 'Y') {
			delete snk;
			return 1;
		}
		else if (opt == 'n' || opt == 'N') {
			delete snk;
			return 0;
		}
		else {
			cout << "Invalid! Please re-enter!" << endl;
		}
	}
}

void addapple() {
	int ran = rand() % 100;
	if (ran >= 10 && ran <= 13) {//%3的机会生成苹果
		int i, j;
		do {
			i = rand() % RAW;
			j = rand() % COL;
		} while (map[i][j] != AIR);
		map[i][j] = APPLE;
		freshmap(i, j);
	}
}

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void freshmap(int y, int x) {
	gotoxy(x + 1, y + 1);
	switch (map[y][x]) {
	case AIR:
		cout << ' ';
		break;
	case SNAKE_HEAD:
		cout << '#';
		break;
	case SNAKE_BODY:
		cout << '*';
		break;
	case APPLE:
		cout << '+';
		break;
	}
}