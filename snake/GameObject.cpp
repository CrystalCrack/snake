#include "GameObject.h"

int APPLE::quantity;//�洢ƻ������
vector<pair<POINT, APPLE>> APPLE::apple;//�洢����ƻ���ĵ��λ��
bool APPLE::isinitial;
int APPLE::size = DEFAULTSIZE;
TRSP_IMAGE APPLE::image;

void TRSP_IMAGE::drawimage(int x, int y) {
	if (!init) {//û�г�ʼ����
		SetWorkingImage(&ORIGIN);//��ԭͼ����Ϊ����ƽ��
		putimage(0, 0, &MASK, SRCINVERT);//������ͼ���ӵ�ԭͼ����ԭͼ�հ׵ĵط����
		init = 1;
	}
	SetWorkingImage(NULL);//�л�����ƽ��Ϊ����
	putimage(x, y, &MASK, SRCAND);//��������ͼ����С�˲��ֿٺ�
	putimage(x, y, &ORIGIN, SRCPAINT);//���Ƶ��ӹ����ԭͼ
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
	switch (mode) {//������������ѡ��ͬ����ɫ
	case SHALLOW:
		setlinecolor(0xb7b7ed);
		setfillcolor(0xb7b7ed);
		break;
	case DEEP:
		setlinecolor(0x3c3c48);
		setfillcolor(0x3c3c48);
	}
	SetWorkingImage(NULL);//�л�����ƽ��Ϊ����
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
	auto i = snake.end();//������
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
		history.push_back(std::make_pair(UP, head.xy));//��¼��תͷ�ķ��������
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
		head.xy.x -= COL;
		history.push_back(std::make_pair(RIGHT, head.xy));
	}
	else if (head.xy.x < -COL / 2) {
		head.xy.x += COL;
		history.push_back(std::make_pair(LEFT, head.xy));
	}
	if (head.xy.y > RAW / 2) {
		head.xy.y -= RAW;
		history.push_back(std::make_pair(UP, head.xy));
	}
	else if (head.xy.y < -RAW / 2) {
		head.xy.y += RAW;
		history.push_back(std::make_pair(DOWN, head.xy));
	}
	head.LB.x = head.xy.x - head.size / 2;
	head.LB.y = head.xy.y - head.size / 2;
	auto i = snake.begin();
	while (i != snake.end()) {//�����ƶ�ÿһ������
		int p = speed;
		A:
		auto x = history.begin()+i->now;
		x++;
		if (x == history.end()) {//û����һ���ڵ�
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
		else {//����һ���ڵ�
			int d;
			switch (history[i->now].first) {
			case RIGHT:
				d = i->xy.x < x->second.x ? x->second.x - i->xy.x : COL - (i->xy.x - x->second.x);
				if (p >= d) {//�ƶ�һ���ٶ�֮�󳬹��ڵ�
					p -= d;//ʣ��ľ���
					i->xy.x = x->second.x;//�ƶ����ýڵ�
					i->now++;
					goto A;
				}else
					i->xy.x += p;
				break;
			case LEFT:
				d = i->xy.x > x->second.x ? i->xy.x - x->second.x : COL - (x->second.x - i->xy.x);
				if (p>=d) {//�ƶ�һ���ٶ�֮�󳬹��ڵ�
					p -= d;//ʣ��ľ���
					i->xy.x = x->second.x;//�ƶ����ýڵ�
					i->now++;
					goto A;
				}
				else
					i->xy.x -= p;
				break;
			case UP:
				d = i->xy.y < x->second.y ? x->second.y - i->xy.y : RAW - (i->xy.y - x->second.y);
				if (p>=d) {//�ƶ�һ���ٶ�֮�󳬹��ڵ�
					p -= d;//ʣ��ľ���
					i->xy.y = x->second.y;//�ƶ����ýڵ�
					i->now++;//�ƶ�����������һ���ڵ�
					goto A;
				}
				else
					i->xy.y += p;
				break;
			case DOWN:
				d = i->xy.y > x->second.y ? i->xy.y - x->second.y : RAW - (x->second.y - i->xy.y);
				if (p>=d) {//�ƶ�һ���ٶ�֮�󳬹��ڵ�
					p -= d;//ʣ��ľ���
					i->xy.y = x->second.y;//�ƶ����ýڵ�
					i->now++;//�ƶ�����������һ���ڵ�
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
	if (snake[snake.size() - 1].now == 1) {//���һλҲԽ���˵�һ���ڵ�
		history.erase(history.begin());//ɾ����һ���ڵ�
		for (auto m = snake.begin(); m != snake.end(); m++) {
			m->now--;
		}
	}
}

void SNAKE::addlength() {
	length++;
	BODY body;
	switch (history[snake[length - 3].now].first) {//��������
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

bool SNAKE::iseaten() {
	auto i = APPLE::apple.begin();
	while (i != APPLE::apple.end()) {
		if (ishit(this->head, i->second))
			break;
		i++;
	}
	if (i == APPLE::apple.end()) {
		return false;
	}
	else {
		addlength();//���ӳ���
		APPLE::apple.erase(i);//ɾ��ƻ��
		return true;
	}
}

void putapple() {
	if (APPLE::isinitial) {
		for (auto i = APPLE::apple.begin(); i != APPLE::apple.end(); i++) {
			i->second.image.drawimage(i->second.LB.x, i->second.LB.y);
		}
	}
}

void addapple() {
	if (APPLE::isinitial) {
		APPLE::quantity++;
		int x = rand() % (COL - APPLE::size) - COL / 2;
		int y = rand() % (RAW - APPLE::size) - RAW / 2;
		POINT p = { x,y };
		APPLE newapple(p);
		APPLE::apple.push_back(std::make_pair(p, newapple));
	}
}

void addapple(POINT pos) {
	if (APPLE::isinitial) {
		APPLE::quantity++;
		APPLE newapple(pos);
		APPLE::apple.push_back(std::make_pair(pos, newapple));
	}
}

void APPLE::initialize(TRSP_IMAGE& img) {
	image = img;
	isinitial = true;
};