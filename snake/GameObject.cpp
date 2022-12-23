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

void BODY::putbody(HDW mod) {
	switch (mode) {//根据身体类型选择不同的颜色
	case SHALLOW:
		if (mod == NORMAL) {
			setlinecolor(0xb7b7ed);
			setfillcolor(0xb7b7ed);
		}
		else if (mod == CHICKEN) {
			setlinecolor(0xed9291);
			setfillcolor(0xed9291);
		}
		break;
	case DEEP:
		if (mod == NORMAL) {
			setlinecolor(0x3c3c48);
			setfillcolor(0x3c3c48);
		}
		else if (mod == CHICKEN) {
			setlinecolor(0xf0d6a3);
			setfillcolor(0xf0d6a3);
		}
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


SNAKE::SNAKE(TRSP_IMAGE up, TRSP_IMAGE left, TRSP_IMAGE right, TRSP_IMAGE down, HDW mode) :head(up, left, right, down, 0, 0), mod(mode) {
	snake.reserve(1000000);
	length = 5;
	speed = 7;
	BODY body[4];

	for (int i = 0; i < 4; i++) {
		body[i] = BODY(i + 1, -SNAKE_BODY::distance * (i + 1), 0, RIGHT);
		snake.push_back(body[i]);
	}
	history.push_back(std::make_pair(head.dir, 0));
}

void SNAKE::drawsnake() {
	auto i = snake.end();//迭代器
	do {
		i--;
		i->putbody(mod);
	} while (i != snake.begin());
	head.puthead();
}

direct insertdir(direct dir) {
	direct n_dir;
	switch (dir) {
	case UP:
		n_dir = DOWN;
		break;
	case DOWN:
		n_dir = UP;
		break;
	case LEFT:
		n_dir = RIGHT;
		break;
	case RIGHT:
		n_dir = LEFT;
		break;
	}
	return n_dir;
}

void SNAKE::turn(direct dir) {
	head.turn(dir);
	switch (dir) {
	case UP:
		history.push_back(std::make_pair(UP, 0));//记录下转头的方向,让距离为0
		break;
	case DOWN:
		history.push_back(std::make_pair(DOWN, 0));
		break;
	case RIGHT:
		history.push_back(std::make_pair(RIGHT, 0));
		break;
	case LEFT:
		history.push_back(std::make_pair(LEFT, 0));
	}
}

void SNAKE::move() {
	//移动头
	head.xy = move1(head.xy, head.dir, speed);
	history[history.size() - 1].second += speed;
	head.LB.x = head.xy.x - head.size / 2;
	head.LB.y = head.xy.y - head.size / 2;
	//绘制剩余的身体部分
	auto it1 = history.end() - 1;
	auto it2 = snake.begin();
	int dis;
	POINT now = head.xy;
	direct n_dir = RIGHT;
	int rest_dis = it1->second;;
	dis = SNAKE_BODY::distance;
	while (it1 != history.begin() && it2 != snake.end()) {
		n_dir = inversedir(it1->first);
		int x = dis;
		if (dis >= rest_dis) {
			dis -= rest_dis;
			rest_dis = 0;
		}
		else {
			rest_dis -= dis;
			dis = 0;
		}
		now = it2->xy = move1(now, n_dir, x-dis);
		if (dis == 0) {
			it2->dir = it1->first;
			it2++;
			dis = SNAKE_BODY::distance;
		}
		if (rest_dis == 0) {
			it1--;
			 rest_dis = it1->second;
		}
	}
	A:
	if (it2 != snake.end()) {//身体没排完,此时it1一定等于history.begin()
		n_dir = inversedir(it1->first);
		//直接排列剩余的身体
		while (it2 != snake.end()) {
			it2->xy = now = move1(now, n_dir, dis);
			dis = SNAKE_BODY::distance;
			it2++;
		}
	}
	else {//所有身体已经排完，删除之后的所有历史记录
		history.erase(history.begin(), it1);
	}
}

void SNAKE::addlength() {
	length++;
	BODY body;
	switch (history[0].first) {//控制坐标
	case UP:
		body = BODY(length - 1, snake[length - 3].xy.x, snake[length - 3].xy.y - SNAKE_BODY::distance, UP);
		break;
	case DOWN:
		body = BODY(length - 1, snake[length - 3].xy.x, snake[length - 3].xy.y + SNAKE_BODY::distance, DOWN);
		break;
	case LEFT:
		body = BODY(length - 1, snake[length - 3].xy.x + SNAKE_BODY::distance, snake[length - 3].xy.y, LEFT);
		break;
	case RIGHT:
		body = BODY(length - 1, snake[length - 3].xy.x - SNAKE_BODY::distance, snake[length - 3].xy.y, RIGHT);
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
	for (auto it = snake.begin(); it != snake.end(); it++) {
		int left, right, top, bottom;
		int Left, Right, Top, Bottom;
		bool flag1 = false, flag2 = false;
		switch (it->dir) {
		case LEFT:
		case RIGHT:
			left = it->xy.x - it->radius;
			right = it->xy.x + it->radius;
			top = it->xy.y + (speed / 2 + speed % 2);
			bottom = it->xy.y - (speed / 2 + speed % 2);
			break;
		case UP:
		case DOWN:
			left = it->xy.x - (speed / 2 + speed % 2);
			right = it->xy.x + (speed / 2 + speed % 2);
			top = it->xy.y + it->radius;
			bottom = it->xy.y - it->radius;
			break;
		}
		Left = left < -COL / 2 ? left + COL : left;
		Right = right > COL / 2 ? right - COL : right;
		Top = top > RAW / 2 ? top - RAW : top;
		Bottom = bottom < -RAW / 2 ? bottom + RAW : bottom;
		if (Left < Right) {
			if (head.xy.x > Left && head.xy.x < Right)
				flag1 = true;
		}
		else {
			if (head.xy.x > Left || head.xy.x < Right)
				flag1 = true;
		}
		if (Bottom < Top) {
			if (head.xy.y > Bottom && head.xy.y < Top)
				flag2 = true;
		}
		else {
			if (head.xy.y > Bottom || head.xy.y < Top)
				flag2 = true;
		}
		if (flag1 && flag2)
			return true;
	}
	return false;
}

APPLE::APPLE(SNAKE s) {
	bool flag = false;
	do {
		xy.x = rand() % (COL - size) - (COL - size) / 2;
		xy.y = rand() % (RAW - size) - (RAW - size) / 2;
		int i = 0;
		float dis;
		for (auto it = s.snake.begin(); it != s.snake.end(); it++) {
			dis = sqrt(pow(xy.x - it->getxy().x, 2) + pow(xy.y - it->getxy().y, 2));
			if (dis <= size/2)
				flag = true;
			else
				flag = false;
		}
	} while (flag);
	LB = { xy.x - size / 2,xy.y - size / 2 };
}

void APPLE::putapple() {
	image.drawimage(LB.x, LB.y);
}

void APPLE::initialize(TRSP_IMAGE& img) {
	image = img;
}

POINT move1(POINT p, direct dir, int dis) {
	switch (dir) {
	case UP:
		p.y += dis;
		break;
	case DOWN:
		p.y -= dis;
		break;
	case RIGHT:
		p.x += dis;
		break;
	case LEFT:
		p.x -= dis;
		break;
	}
	if (p.x > COL / 2) {
		p.x -= COL;
	}
	else if (p.x < -COL / 2) {
		p.x += COL;
	}
	if (p.y > RAW / 2) {
		p.y -= RAW;
	}
	else if (p.y < -RAW / 2) {
		p.y += RAW;
	}
	return p;
}

direct inversedir(direct dir) {
	switch (dir) {
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	}
}