#include <SDL.h>
#include <vector>
#include <iostream>

#include "definitions.h"
#include "sprite.h"
#include "nonPlayer.h"

using namespace std;

extern double xOffset, yOffset, zoom;
extern SDL_Renderer* renderer;
extern SDL_Event evt;

NonPlayer::NonPlayer()
{
	x = 30;
	y = 30;
	midX = x + 0.5*width;
	midY = y + 0.5*height;
	width = 10;
	height = 10;

	nonPlayerRect.x = (x*zoom-xOffset);
	nonPlayerRect.y = (y*zoom-yOffset);
	nonPlayerRect.w = width*zoom;
	nonPlayerRect.h = height*zoom;

	vx = 0;
	vy = 0;
	speed = 0.1;
	maxSpeed = 1;
	friction = 0.8;

	accuracy = 1;

	//add timer
	delay = 1000;
	timerID = SDL_AddTimer(delay, timerFunction, NULL);

	std::vector<pathCoord> tempPath;
	tempPath.push_back(pathCoord(x, y, 0));

	storedPath = tempPath;
	currentPath = tempPath;
}

NonPlayer::~NonPlayer()
{

}

void NonPlayer::update()
{
	if (midX < currentPath.front().x - accuracy)
	{
		vx += speed;
	}
	else if (midX > currentPath.front().x + accuracy)
	{
		vx -= speed;
	}
	else
	{
		vx *= friction;
	}

	if (midY < currentPath.front().y - accuracy)
	{
		vy += speed;
	}
	else if (midY > currentPath.front().y + accuracy)
	{
		vy -= speed;
	}
	else
	{
		vy *= friction;
	}

	//erase path coordinate if we're at the location and currentPath has
	//more than 1 element
	if (currentPath.size() > 1 
		&& midX >= currentPath.front().x - accuracy 
		&& midX <= currentPath.front().x + accuracy
		&& midY >= currentPath.front().y - accuracy
		&& midY <= currentPath.front().y + accuracy)
	{
		currentPath.erase(currentPath.begin());
	}

	if (vx > maxSpeed)
		vx = maxSpeed;
	else if (vx < -maxSpeed)
		vx = -maxSpeed;
	if (vy > maxSpeed)
		vy = maxSpeed;
	else if (vy < -maxSpeed)
		vy = -maxSpeed;

	oldX = x;
	oldY = y;
	x += vx;
	y += vy;
	midX = x + 0.5*width;
	midY = y + 0.5*height;

	nonPlayerRect.x = (x*zoom-xOffset);
	nonPlayerRect.y = (y*zoom-yOffset);
	nonPlayerRect.w = width*zoom;
	nonPlayerRect.h = height*zoom;
}


void NonPlayer::handleEvent()
{
	nextPathReady = true;

	//cout << "event handled" << endl;

	//currentPath = storedPath;
}


Uint32 NonPlayer::timerFunction(Uint32 interval, void* param)
{
	evt.type = SDL_USEREVENT;
	evt.user.code = NPC_EVENT;
	evt.user.data1 = 0;
	evt.user.data2 = 0;

	SDL_PushEvent(&evt);

	return interval;
}


void NonPlayer::setPathCoordinates(std::vector<pathCoord> pathIn)
{
	currentPath = pathIn;
}

void NonPlayer::draw()
{
	SDL_SetRenderDrawColor(renderer, 100, 50, 50, 255);
	SDL_RenderFillRect(renderer, &nonPlayerRect);
}