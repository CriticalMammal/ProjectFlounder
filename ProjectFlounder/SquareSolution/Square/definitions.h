//File: definitions.h
//Prog: Dylan Gallardo
//Purp: defines some common constants to use

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

struct pathCoord
{
	double x, y;
	int distance;

	pathCoord(double paramX, double paramY, int dist) : x(paramX), y(paramY), distance(dist) {};
};

struct pathTile
{
	int tileElement, distance;

	pathTile(int element, int dist) : tileElement(element), distance(dist) {};
};

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS 60
#define NPC_EVENT 1

enum {LEFT, RIGHT, UP, DOWN, c};

#endif