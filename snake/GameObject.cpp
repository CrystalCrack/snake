#include "GameObject.h"

int APPLE::size = DEFAULTSIZE;
TRSP_IMAGE APPLE::image;

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
	double dis = sqrt(pow(obj1.xy.x - obj2.xy.x, 2) + pow(obj1.xy.y - obj2.xy.y, 2));
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
	c_img->drawimage(LB.x, LB.y);
	if (LB.x < -COL / 2 || LB.x + size > COL / 2 || LB.y < -RAW / 2 || LB.y + size > RAW / 2) {
		POINT p;
		int initx = 0, inity = 0;
		if (LB.x < -COL / 2) {
			p.x = LB.x + COL;
			initx = 1;
		}
		else if (LB.x + size > COL / 2) {
			p.x = LB.x - COL;
			initx = 1;
		}
		if (LB.y < -RAW / 2) {
			p.y = LB.y + RAW;
			inity = 1;
		}
		else if (LB.y + size > RAW / 2) {
			p.y = LB.y - RAW;
			inity = 1;
		}
		if (!initx)
			p.x = LB.x;
		if (!inity)
			p.y = LB.y;
		c_img->drawimage(p.x, p.y);
	}
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
	if (xy.x - radius<-COL / 2 || xy.x + radius>COL / 2 || xy.y - radius<-RAW / 2 || xy.y + radius>RAW / 2) {
		POINT p;
		bool initx = 0, inity = 0;
		if (xy.x - radius < -COL / 2) {
			p.x = xy.x + COL;
			initx = 1;
		}
		else if (xy.x + radius > COL / 2) {
			p.x = xy.x - COL;
			initx = 1;
		}
		if (xy.y - radius < -RAW / 2) {
			p.y = xy.y + RAW;
			inity = 1;
		}
		else if (xy.y + radius > RAW / 2) {
			p.y = xy.y - RAW;
			inity = 1;
		}
		if (!initx)
			p.x = xy.x;
		else if (!inity)
			p.y = xy.y;
		fillcircle(p.x, p.y, radius);
	}
}


SNAKE::SNAKE(TRSP_IMAGE up, TRSP_IMAGE left, TRSP_IMAGE right, TRSP_IMAGE down):head(up,left,right,down,0,0) {
	snake.reserve(1000000);
	length = 5;
	speed = 5;
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
	if (head.xy.x > COL / 2) {
		history.push_back(std::make_pair(RIGHT, head.xy));
		head.xy.x -= COL;
	}
	else if (head.xy.x < -COL / 2) {
		history.push_back(std::make_pair(LEFT, head.xy));
		head.xy.x += COL;
	}
	if (head.xy.y > RAW / 2) {
		history.push_back(std::make_pair(UP, head.xy));
		head.xy.y -= RAW;
	}
	else if (head.xy.y < -RAW / 2) {
		history.push_back(std::make_pair(DOWN, head.xy));
		head.xy.y += RAW;
	}
	head.LB.x = head.xy.x - head.size / 2;
	head.LB.y = head.xy.y - head.size / 2;
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
			int d;
			switch (history[i->now].first) {
			case RIGHT:
				d = i->xy.x < x->second.x ? x->second.x - i->xy.x : COL - (i->xy.x - x->second.x);
				if (p >= d) {//移动一个速度之后超过节点
					p -= d;//剩余的距离
					i->xy.x = x->second.x;//移动到该节点
					i->now++;
					goto A;
				}else
					i->xy.x += p;
				break;
			case LEFT:
				d = i->xy.x > x->second.x ? i->xy.x - x->second.x : COL - (x->second.x - i->xy.x);
				if (p>=d) {//移动一个速度之后超过节点
					p -= d;//剩余的距离
					i->xy.x = x->second.x;//移动到该节点
					i->now++;
					goto A;
				}
				else
					i->xy.x -= p;
				break;
			case UP:
				d = i->xy.y < x->second.y ? x->second.y - i->xy.y : RAW - (i->xy.y - x->second.y);
				if (p>=d) {//移动一个速度之后超过节点
					p -= d;//剩余的距离
					i->xy.y = x->second.y;//移动到该节点
					i->now++;//移动迭代器到下一个节点
					goto A;
				}
				else
					i->xy.y += p;
				break;
			case DOWN:
				d = i->xy.y > x->second.y ? i->xy.y - x->second.y : RAW - (x->second.y - i->xy.y);
				if (p>=d) {//移动一个速度之后超过节点
					p -= d;//剩余的距离
					i->xy.y = x->second.y;//移动到该节点
					i->now++;//移动迭代器到下一个节点
					goto A;
				}
				else
					i->xy.y -= p;
			}
		}
		if (i->xy.x > COL / 2) {
			i->xy.x -= COL;
		}
		else if (i->xy.x < -COL / 2)
			i->xy.x += COL;
		if (i->xy.y > RAW / 2)
			i->xy.y -= RAW;
		else if (i->xy.y < -RAW / 2)
			i->xy.y += RAW;
		i++;
	}
	while (snake[snake.size() - 1].now) {//最后一位也越过了第一个节点
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
	snake.push_back(body);
}

int SNAKE::iseaten(vector<APPLE>& apple) {
	int i = 0;
	while (i != apple.size()) {
		if (ishit(this->head, apple[i]))
			break;
		i++;
	}
	return i;
}

bool SNAKE::isdead() {
	for (int i = 1; i < history.size(); i++) {
		judgeline m;
		if (i != 1)
			m = judgeline(history[i - 1].second, history[i].second, history[i-1].first, speed);
		else
			m = judgeline((snake.end() - 1)->xy, history[i].second, history[i-1].first, speed);
		if (m.judge(head.xy))
			return true;
	}
	return false;
}

void APPLE::putapple() {
	image.drawimage(LB.x, LB.y);
}

void APPLE::initialize(TRSP_IMAGE& img) {
	image = img;
};

bool judgeline::judge(POINT pos) {
	if (p1.x == p2.x && p1.y == p2.y) {//单个点不判定
		return false;//没碰到
	}
	switch (dir) {
	case RIGHT:
		if (p1.x < p2.x && (pos.x>p1.x && pos.x<p2.x) && abs(pos.y - p1.y) < width / 2)
			return true;
		else if (p1.x > p2.x && (pos.x>p1.x || pos.x<p2.x) && abs(pos.y - p1.y) < width / 2)
			return true;
		break;
	case LEFT:
		if (p1.x > p2.x && (pos.x<p1.x && pos.x>p2.x) && abs(pos.y - p1.y) < width / 2)
			return true;
		else if (p1.x < p2.x && (pos.x<p1.x || pos.x>p2.x) && abs(pos.y - p1.y) < width / 2)
			return true;
		break;
	case UP:
		if (p1.y < p2.y && (pos.y>p1.y && pos.y<p2.y) && abs(pos.x - p1.x) < width / 2)
			return true;
		else if (p1.y > p2.y && (pos.y>p1.y || pos.y<p2.y) && abs(pos.x - p1.x) < width / 2)
			return true;
		break;
	case DOWN:
		if (p1.y > p2.y && (pos.y<p1.y && pos.y>p2.y) && abs(pos.x - p1.x) < width / 2)
			return true;
		else if (p1.y < p2.y && (pos.y<p1.y || pos.y>p2.y) && abs(pos.x - p1.x) < width / 2)
			return true;
		break;
	}
	return false;
}