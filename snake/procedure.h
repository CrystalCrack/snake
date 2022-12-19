#pragma once
#ifndef _PROCEDURE
#define _PROCEDURE
#include <stdio.h>
#include<iostream>
#include <ctime>
#include<SDL.h>
#include<SDL_mixer.h>
#include"GameObject.h"
#define a 1280
#define b 960
using namespace std;


GAMEMODE menu();
void draw(SNAKE s, vector<APPLE>& apple);
void newgame(GAMEMODE mode);

#endif