#pragma once
#ifndef _PROCEDURE
#define _PROCEDURE
#include <stdio.h>
#include<iostream>
#include <ctime>
#include<SDL.h>
#include<SDL_mixer.h>
#include"GameObject.h"
#define a 1024
#define b 768
using namespace std;

extern int max_score;

HDW menu();
void draw(SNAKE s, vector<APPLE>& apple);
int newgame(HDW mode);
HDW show_end_screen(int score);

void instru();

#endif