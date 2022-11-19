#pragma once
#ifndef _GAME_OBJECT
#define _GAME_OBJECT
#define DEFAULTSIZE 30
#define COL 1280//���򳤶�
#define RAW 720//���򳤶�
#define DIS 16
#define PI 3.1415926
#
#include<easyx.h>
#include<list>
#include<cmath>
#include<vector>
using std::list;
using std::vector;


static IMAGE bg;

class TRSP_IMAGE {//transparent image
private:
	bool init;
	IMAGE ORIGIN;
	IMAGE MASK;
public:
	TRSP_IMAGE() {};
	TRSP_IMAGE(IMAGE& origin, IMAGE& mask) :ORIGIN(origin), MASK(mask){}
	void drawimage(int x,int y);
};

//����
class GameObject
{
protected:
	static const int size = DEFAULTSIZE;//Ĭ�ϴ�С
	POINT xy;//Բ������
public:
	inline GameObject():xy({0,0}){}
	inline GameObject(POINT in_xy) :xy(in_xy) {}
};

enum direct {//����ö��
	UP,DOWN,LEFT,RIGHT
};

enum bodymode {//������ɫö��
	DEEP,//��
	SHALLOW//ǳ
};

class SNAKE_BODY : public GameObject{
friend class SNAKE;
public:
	static const int distance = DIS;//���
	inline SNAKE_BODY(){};
};

class HEAD :public SNAKE_BODY {
friend class SNAKE;
protected:
	POINT LT;//left top
	direct dir;//��ͷ�ķ���
	TRSP_IMAGE IMG_UPS;//UPSIDE
	TRSP_IMAGE IMG_LEFT;
	TRSP_IMAGE IMG_RIGHT;
	TRSP_IMAGE IMG_USD;//USD--UPSIDE DOWN
	TRSP_IMAGE* c_img;//current image

public:
	HEAD() :LT({ 0,0 }),c_img(nullptr),dir(RIGHT) {}
	inline HEAD(TRSP_IMAGE& up, TRSP_IMAGE& left, TRSP_IMAGE& right, TRSP_IMAGE& usd,int x,int y) :
		IMG_UPS(up), IMG_LEFT(left), IMG_RIGHT(right), IMG_USD(usd), c_img(&IMG_RIGHT),dir(RIGHT) {
		xy = { x,y };
		LT = { x - size / 2,y - size / 2 };
	}
	inline direct getdir() { return dir; };
	void turn(direct d);
	void puthead();
};

class BODY : public SNAKE_BODY{
public:
	bodymode mode;
	static const int radius = size / 2;
public:
	/*@param
	* mde	����Ϊ��ɫ,ż��Ϊǳɫ
	*/
	BODY() :mode(DEEP) {};
	inline BODY(unsigned int mde,int x,int y){
		xy = { x,y };
		mode = mde % 2 == 0 ? SHALLOW : DEEP;
	};
	void putbody();
};

enum opr {//���˶���¼ö��
	ADDX, SUBX, ADDY, SUBY
};

class SNAKE {
private:
	HEAD head;
	list<BODY> snake;
	int speed;
	int length;
	vector<opr> history;//�洢���˶�����ʷ��¼
public:
	SNAKE(TRSP_IMAGE up, TRSP_IMAGE left, TRSP_IMAGE right, TRSP_IMAGE down);
	inline int getlength() { return length; }
	void drawsnake();
	void move();
	inline void turn(direct d) {
		head.turn(d);
	}
};

#endif