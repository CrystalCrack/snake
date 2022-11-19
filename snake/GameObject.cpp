#include "GameObject.h"


void TRSP_IMAGE::drawimage(int x, int y) {
	if (!init) {//没有初始化过
		SetWorkingImage(&ORIGIN);//将原图设置为工作平面
		putimage(0, 0, &MASK, SRCINVERT);//将掩码图叠加到原图，让原图空白的地方变黑
		init = 1;
	}
	SetWorkingImage(NULL);//切换工作平面为背景
	putimage(x, y, &MASK, SRCAND);//绘制掩码图，把小人部分抠黑
	putimage(x, y, &ORIGIN, SRCPAINT);//绘制叠加过后的原图
}


void HEAD::turn(direct d) {
	dir = d;
	switch (dir) {
	case UP:
		c_img = &IMG_UPS;
		break;
	case DOWN:
		c_img = &IMG_USD;
		break;
	case LEFT:
		c_img = &IMG_LEFT;
		break;
	case RIGHT:
		c_img = &IMG_RIGHT;
		break;
	}
}

void HEAD::puthead() {
	c_img->drawimage(LT.x, LT.y);
}


void BODY::putbody() {
	switch (mode) {//根据身体类型选择不同的颜色
	case SHALLOW:
		setlinecolor(0xb7b7ed);
		setfillcolor(0xb7b7ed);
		break;
	case DEEP:
		setlinecolor(0x3c3c48);
		setfillcolor(0x3c3c48);
	}
	SetWorkingImage(NULL);//切换工作平面为背景
	fillcircle(xy.x, xy.y, radius);
}



SNAKE::SNAKE(TRSP_IMAGE up, TRSP_IMAGE left, TRSP_IMAGE right, TRSP_IMAGE down):head(up,left,right,down,0,0) {
	length = 5;
	speed = 5;
	BODY body[4];
	for (int i = 0; i < 4; i++) {
		body[i] = BODY(i+1,- SNAKE_BODY::distance * (i+1),0);
		snake.push_back(body[i]);
		history.push_back(ADDX);
	}
}

void SNAKE::drawsnake() {
	auto i = snake.end();//迭代器
	do{
		i--;
		i->putbody();
	} while (i != snake.begin());
	head.puthead();
}

void SNAKE::move() {
	switch (head.dir) {
	case UP:
		head.xy.y += speed;
		history.insert(history.begin(), ADDY);
		break;
	case DOWN:
		head.xy.y -= speed;
		history.insert(history.begin(), SUBY);
		break;
	case RIGHT:
		head.xy.x += speed;
		history.insert(history.begin(), ADDX);
		break;
	case LEFT:
		head.xy.x -= speed;
		history.insert(history.begin(), SUBX);
		break;
	}
	head.LT.x = head.xy.x - head.size / 2;
	head.LT.y = head.xy.y - head.size / 2;
	//计算角度
	long double alpha;
	if (head.xy.x == snake.front().xy.x) {
		if (head.xy.y > snake.front().xy.y)
			alpha = PI / 2;
		else 
			alpha = -PI / 2;
	}else{
		alpha = atan(static_cast<long double>(head.xy.y - snake.front().xy.y) / static_cast<long double>(head.xy.x - snake.front().xy.x));
	}
	long double x = sqrt(pow(head.xy.y - snake.front().xy.y, 2) + pow(head.xy.x - snake.front().xy.x, 2));
	x -= SNAKE_BODY::distance;
	snake.front().xy.x += x * cos(alpha);
	snake.front().xy.y += x * sin(alpha);
	auto j = snake.begin();
	auto i = j++;
	while (j != snake.end()) {//迭代移动每一个身体
		long double beta;
		if (i->xy.x == j->xy.x) {
			if (i->xy.y == j->xy.y)
				beta = PI / 2;
			else
				beta = -PI / 2;
		}
		else{
			beta = atan(static_cast<long double>(i->xy.y - j->xy.y) / static_cast<long double>(i->xy.x - j->xy.x));
		}
		long double p = sqrt(pow(i->xy.y - j->xy.y, 2) + pow(i->xy.x - j->xy.x, 2));
		p -= SNAKE_BODY::distance;
		j->xy.x += p * cos(beta);
		j->xy.y += p * sin(beta);
		i = j++;
	}
}
