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

bool ishit(GameObject obj1, GameObject obj2) {
	double dis = pow(obj1.xy.x - obj2.xy.x, 2) + pow(obj1.xy.y - obj2.xy.y, 2);
	if (dis < (obj1.size + obj2.size) / 2)
		return true;
	else
		return false;
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
	snake.reserve(1000000);
	length = 5;
	speed = 10;
	BODY body[4];

	for (int i = 0; i < 4; i++) {
		body[i] = BODY(i+1,- SNAKE_BODY::distance * (i+1),0);
		snake.push_back(body[i]);
	}
	history.push_back(std::make_pair(head.dir, body[3].xy));
	for (auto x = snake.begin(); x != snake.end();x++) {
		x->now = 0;
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

void SNAKE::turn(direct dir) {
	head.turn(dir);
	switch (dir) {
	case UP:
		history.push_back(std::make_pair(UP, head.xy));//记录下转头的方向和坐标
		break;
	case DOWN:
		history.push_back(std::make_pair(DOWN, head.xy));
		break;
	case RIGHT:
		history.push_back(std::make_pair(RIGHT, head.xy));
		break;
	case LEFT:
		history.push_back(std::make_pair(LEFT, head.xy));
	}
}

void SNAKE::move() {
	switch (head.dir) {
	case UP:
		head.xy.y += speed;
		break;
	case DOWN:
		head.xy.y -= speed;
		break;
	case RIGHT:
		head.xy.x += speed;
		break;
	case LEFT:
		head.xy.x -= speed;
		break;
	}
	head.LT.x = head.xy.x - head.size / 2;
	head.LT.y = head.xy.y - head.size / 2;
	auto i = snake.begin();
	while (i != snake.end()) {//迭代移动每一个身体
		int p = speed;
		A:
		auto x = history.begin()+i->now;
		x++;
		if (x == history.end()) {//没有下一个节点
			switch (history[i->now].first) {
			case RIGHT:
				i->xy.x += p;
				break;
			case LEFT:
				i->xy.x -= p;
				break;
			case UP:
				i->xy.y += p;
				break;
			case DOWN:
				i->xy.y -= p;
				break;
			}
		}
		else {//有下一个节点
			switch (history[i->now].first) {
			case RIGHT:
				if (i->xy.x + p >= x->second.x) {//移动一个速度之后超过节点
					p = p-(x->second.x - i->xy.x);//剩余的距离
					i->xy.x = x->second.x;//移动到该节点
					i->now++;
					goto A;
				}else
					i->xy.x += p;
				break;
			case LEFT:
				if (i->xy.x - p <= x->second.x) {//移动一个速度之后超过节点
					p = p - (i->xy.x - x->second.x);//剩余的距离
					i->xy.x = x->second.x;//移动到该节点
					i->now++;
					goto A;
				}
				else
					i->xy.x -= p;
				break;
			case UP:
				if (i->xy.y + p >= x->second.y) {//移动一个速度之后超过节点
					p = p - (x->second.y - i->xy.y);//剩余的距离
					i->xy.y = x->second.y;//移动到该节点
					i->now++;//移动迭代器到下一个节点
					goto A;
				}
				else
					i->xy.y += p;
				break;
			case DOWN:
				if (i->xy.y - p <= x->second.y) {//移动一个速度之后超过节点
					p = p - (i->xy.y - x->second.y);//剩余的距离
					i->xy.y = x->second.y;//移动到该节点
					i->now++;//移动迭代器到下一个节点
					goto A;
				}
				else
					i->xy.y -= p;
			}
		}
		i++;
	}
	if (snake[snake.size() - 1].now == 1) {//最后一位也越过了第一个节点
		history.erase(history.begin());//删除第一个节点
		for (auto m = snake.begin(); m != snake.end(); m++) {
			m->now--;
		}
	}
}

void SNAKE::addlength() {
	length++;
	BODY body;
	switch (history[snake[length - 3].now].first) {//控制坐标
	case UP:
		body = BODY(length - 1, snake[length - 3].xy.x, snake[length - 3].xy.y - SNAKE_BODY::distance);
		break;
	case DOWN:
		body = BODY(length - 1, snake[length - 3].xy.x, snake[length - 3].xy.y + SNAKE_BODY::distance);
		break;
	case LEFT:
		body = BODY(length - 1, snake[length - 3].xy.x + SNAKE_BODY::distance, snake[length - 3].xy.y);
		break;
	case RIGHT:
		body = BODY(length - 1, snake[length - 3].xy.x - SNAKE_BODY::distance, snake[length - 3].xy.y);
		break;
	}
	//控制迭代器
	body.now = 0;
	snake.push_back(body);
}