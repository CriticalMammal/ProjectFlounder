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
	double heuristic;

	pathTile(int element, int dist, double inHeuristic) : tileElement(element), distance(dist), heuristic(inHeuristic) {};
};

struct lessThanHeuristic
{
	bool operator() (const pathTile& tile1, const pathTile& tile2)
	{
		return (tile1.heuristic < tile2.heuristic);
	}
};

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS 60
#define NPC_EVENT 1

enum {LEFT, RIGHT, UP, DOWN, c};

#endif