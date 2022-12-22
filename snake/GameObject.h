#pragma once
#ifndef _GAME_OBJECT
#define _GAME_OBJECT
#define DEFAULTSIZE 30
#define DIS 15
#include<easyx.h>
#include<cmath>
#include<vector>
#include"global.h"
using std::vector;
using std::pair;

extern int COL;//横向长度
extern int RAW;//纵向长度

extern IMAGE bg;

class TRSP_IMAGE {//transparent image
private:
	bool init = 0;
	IMAGE ORIGIN;
	IMAGE MASK;
public:
	TRSP_IMAGE() {};
	TRSP_IMAGE(IMAGE& origin, IMAGE& mask) :ORIGIN(origin), MASK(mask) {}
	void drawimage(int x, int y);
};

//基类
class GameObject
{
protected:
	int size = DEFAULTSIZE;//默认大小
	POINT xy;//圆心坐标
public:
	inline GameObject() :xy({ 0,0 }) {}
	inline GameObject(POINT in_xy) : xy(in_xy) {}
	inline POINT getxy() { return xy; }
	friend bool ishit(GameObject obj1, GameObject obj2);
};

class APPLE : GameObject {
	friend class SNAKE;
private:
	static int size;
	static TRSP_IMAGE image;
	POINT LB;
public:
	void putapple();
	APPLE(SNAKE s);
	inline APPLE(POINT pos) :GameObject(pos) {
		LB = { xy.x - size / 2,xy.y - size / 2 };
	};
	static void initialize(TRSP_IMAGE& img);
};

class SNAKE_BODY : public GameObject {
	friend class SNAKE;
public:
	static const int distance = DIS;//间距
	inline SNAKE_BODY(POINT pos) :GameObject(pos) {};
};

class HEAD :public SNAKE_BODY {
	friend class SNAKE;
protected:
	POINT LB;//left bottom
	direct dir;//蛇头的方向
	TRSP_IMAGE IMG_UPS;//UPSIDE
	TRSP_IMAGE IMG_LEFT;
	TRSP_IMAGE IMG_RIGHT;
	TRSP_IMAGE IMG_USD;//USD--UPSIDE DOWN
	TRSP_IMAGE* c_img;//current image

public:
	HEAD() :LB({ 0,0 }), c_img(nullptr), dir(RIGHT), SNAKE_BODY({ size / 2,size / 2 }) {}
	inline HEAD(TRSP_IMAGE& up, TRSP_IMAGE& left, TRSP_IMAGE& right, TRSP_IMAGE& usd, int x, int y) :
		IMG_UPS(up), IMG_LEFT(left), IMG_RIGHT(right), IMG_USD(usd), c_img(&IMG_RIGHT), dir(RIGHT), SNAKE_BODY({ x,y }) {
		xy = { x,y };
		LB = { x - size / 2,y - size / 2 };
	}
	inline direct getdir() { return dir; };
	void turn(direct d);
	void puthead();
};

class BODY : public SNAKE_BODY {
	friend class SNAKE;
private:
	direct dir;
	bodymode mode;
public:
	int radius;
	/*@param
	* mde	奇数为深色,偶数为浅色
	*/
	BODY() :mode(DEEP), SNAKE_BODY({ 0,0 }), dir(RIGHT), radius(size / 2) {};
	inline BODY(unsigned int mde, int x, int y, direct d) :SNAKE_BODY({ x,y }), dir(d) {
		radius = size / 2;
		mode = mde % 2 == 0 ? SHALLOW : DEEP;
	};
	void putbody(HDW);
};

class SNAKE {
	friend class APPLE;
	friend class judgeline;
private:
	HDW mod;
	HEAD head;
	vector<BODY> snake;
	int speed;
	int length;
	vector<pair<direct, int>> history;//存储蛇运动的历史记录
public:
	SNAKE(TRSP_IMAGE up, TRSP_IMAGE left, TRSP_IMAGE right, TRSP_IMAGE down, HDW mod);
	inline int numberofhistory() {
		return history.size();
	}
	inline int getspeed() { return speed; }
	inline int getlength() { return length; }
	inline direct getdir() {
		return head.getdir();
	}

	void drawsnake();

	void move();
	void turn(direct d);
	void addlength();
	inline void setspeed(int v) {
		speed = v;
	}
	inline void speedup() { speed++; }

	int iseaten(vector<APPLE>&);
	bool isdead();

};


POINT move1(POINT p, direct dir, int dis);
direct inversedir(direct dir);

#endif