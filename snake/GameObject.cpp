#include "GameObject.h"


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
	auto i = snake.end();//������
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
	//����Ƕ�
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
	while (j != snake.end()) {//�����ƶ�ÿһ������
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
