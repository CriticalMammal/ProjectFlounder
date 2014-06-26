#include <SDL.h>

#include "definitions.h"
#include "sprite.h"
#include "player.h"

extern bool keys[];
extern double xOffset, yOffset, zoom;
extern SDL_Renderer* renderer;

Player::Player()
{
	x = 210*zoom;
	y = 330*zoom;
	width = 10;
	height = 10;


	playerRect.x = x;
	playerRect.y = y;
	playerRect.w = width*zoom;
	playerRect.h = height*zoom;

	collisionRect.x = x;
	collisionRect.y = y;
	collisionRect.w = width;
	collisionRect.h = height;

	collisionPad = 1;

	collisionHorz.x = x;
	collisionHorz.y = y+collisionPad;
	collisionHorz.w = width;
	collisionHorz.h = height-collisionPad*2;

	collisionVert.x = x+collisionPad;
	collisionVert.y = y;
	collisionVert.w = width-collisionPad*2;
	collisionVert.h = height;

	vx = 0;
	vy = 0;
	friction = 0.8;
	speed = 0.1;
	maxSpeed = 1;
	oldMaxSpeed = maxSpeed;
}

Player::~Player()
{

}

void Player::update()
{
	//check left/right key presses
	if (keys[LEFT])
	{
		vx -= speed;
	}
	else if (keys[RIGHT])
	{
		vx += speed;
	}
	else
	{
		vx *= friction;
	}

	//check up/down key presses
	if (keys[UP])
	{
		vy -= speed;
	}
	else if (keys[DOWN])
	{
		vy += speed;
	}
	else
	{
		vy *= friction;
	}

	//if holding c
	if (keys[c])
	{
		maxSpeed = oldMaxSpeed/2;
	}
	else
	{
		maxSpeed = oldMaxSpeed;
	}


	//don't go over speed limits
	if (vx >= maxSpeed)
		vx = maxSpeed;
	else if (vx <= -maxSpeed)
		vx = -maxSpeed;

	if (vy >= maxSpeed)
		vy = maxSpeed;
	else if (vy <= -maxSpeed)
		vy = -maxSpeed;

	//update position
	oldX = x;
	oldY = y;
	x += vx;
	y += vy;

	//update collision rects etc
	playerRect.x = (x*zoom-xOffset);
	playerRect.y = (y*zoom-yOffset);
	playerRect.w = width*zoom;
	playerRect.h = height*zoom;

	collisionRect.x = x;
	collisionRect.y = y;
	collisionRect.w = width;
	collisionRect.h = height;

	collisionHorz.x = x;
	collisionHorz.y = y+collisionPad;
	collisionHorz.w = width;
	collisionHorz.h = height-collisionPad*2;

	collisionVert.x = x+collisionPad;
	collisionVert.y = y;
	collisionVert.w = width-collisionPad*2;
	collisionVert.h = height;
}

void Player::draw()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &playerRect);

	/*
	//renders the collision rects
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	SDL_RenderDrawRect(renderer, &collisionHorz);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
	SDL_RenderDrawRect(renderer, &collisionVert);
	*/
}