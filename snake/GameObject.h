#pragma once
#ifndef _GAME_OBJECT
#define _GAME_OBJECT
#define DEFAULTSIZE 30
#define COL 1280//横向长度
#define RAW 720//纵向长度
#define DIS 15
#define PI 3.1415926
#
#include<easyx.h>
#include<cmath>
#include<vector>
#include<map>
using std::vector;
using std::pair;
using std::map;

static IMAGE bg;

class TRSP_IMAGE {//transparent image
private:
	bool init = 0;
	IMAGE ORIGIN;
	IMAGE MASK;
public:
	TRSP_IMAGE() {};
	TRSP_IMAGE(IMAGE& origin, IMAGE& mask) :ORIGIN(origin), MASK(mask){}
	void drawimage(int x,int y);
};

//基类
class GameObject
{
protected:
	int size = DEFAULTSIZE;//默认大小
	POINT xy;//圆心坐标
public:
	inline GameObject():xy({0,0}){}
	inline GameObject(POINT in_xy) :xy(in_xy) {}
	friend bool ishit(GameObject obj1, GameObject obj2);
};

enum direct {//方向枚举
	UP,DOWN,LEFT,RIGHT
};

enum bodymode {//蛇身颜色枚举
	DEEP,//深
	SHALLOW//浅
};

class SNAKE_BODY : public GameObject{
friend class SNAKE;
public:
	static const int distance = DIS;//间距
	inline SNAKE_BODY(POINT pos):GameObject(pos) {};
};

class HEAD :public SNAKE_BODY {
friend class SNAKE;
protected:
	POINT LT;//left top
	direct dir;//蛇头的方向
	TRSP_IMAGE IMG_UPS;//UPSIDE
	TRSP_IMAGE IMG_LEFT;
	TRSP_IMAGE IMG_RIGHT;
	TRSP_IMAGE IMG_USD;//USD--UPSIDE DOWN
	TRSP_IMAGE* c_img;//current image

public:
	HEAD() :LT({ 0,0 }),c_img(nullptr),dir(RIGHT),SNAKE_BODY({size/2,size/2}) {}
	inline HEAD(TRSP_IMAGE& up, TRSP_IMAGE& left, TRSP_IMAGE& right, TRSP_IMAGE& usd,int x,int y) :
		IMG_UPS(up), IMG_LEFT(left), IMG_RIGHT(right), IMG_USD(usd), c_img(&IMG_RIGHT),dir(RIGHT),SNAKE_BODY({x,y}) {
		xy = { x,y };
		LT = { x - size / 2,y - size / 2 };
	}
	inline direct getdir() { return dir; };
	void turn(direct d);
	void puthead();
};

class BODY : public SNAKE_BODY{
	friend class SNAKE;
public:
	int now;//迭代器的替代品，指向上一个经过的节点***在SNAKE中初始化***
	bodymode mode;
	int radius;
public:
	/*@param
	* mde	奇数为深色,偶数为浅色
	*/
	BODY() :mode(DEEP),SNAKE_BODY({0,0}),now(0),radius(size/2) {};
	inline BODY(unsigned int mde,int x,int y):SNAKE_BODY({x,y}),now(0) {
		radius = size / 2;
		mode = mde % 2 == 0 ? SHALLOW : DEEP;
	};
	void putbody();
};

class SNAKE {
private:
	HEAD head;
	vector<BODY> snake;
	int speed;
	int length;
	vector<pair<direct,POINT>> history;//存储蛇运动的历史记录
public:
	SNAKE(TRSP_IMAGE up, TRSP_IMAGE left, TRSP_IMAGE right, TRSP_IMAGE down);
	inline int getlength() { return length; }
	void drawsnake();
	void move();
	void turn(direct d);
	void addlength();
	bool iseaten();
};


class APPLE : GameObject {
	friend class SNAKE;
private:
	static int quantity;//存储苹果总数
	static vector<pair<POINT, APPLE>> apple;//存储所有苹果的点和位置
	static bool isinitial;
	static int size;
	static TRSP_IMAGE image;
	POINT LT;
public:
	friend void putapple();
	friend void addapple();
	friend void addapple(POINT pos);
	inline APPLE(POINT pos) :GameObject(pos) {
		quantity++;
		LT = { xy.x - size / 2,xy.y - size / 2 };
	};
	static void initialize(TRSP_IMAGE& img);
};

void putapple();
void addapple();
void addapple(POINT pos);



#endif