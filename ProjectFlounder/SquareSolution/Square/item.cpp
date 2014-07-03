#include <SDL.h>
#include <stdlib.h>
#include <math.h>

#include "definitions.h"
#include "sprite.h"
#include "leader.h"
#include "item.h"


extern bool keys[];
extern double xOffset, yOffset, zoom;
extern SDL_Renderer* renderer;
extern SDL_Event evt;


Item::Item()
{
	x = 0;
	y = 0;
	width = 10;
	height = 10;
	orgWidth = 10;
	orgHeight = 10;

	moveToPointX = -2000;
	moveToPointY = -2000;

	maxDistance = 18.0;
	speed = 0.2;
	maxSpeed = 1.2;
	friction = 0.8;
	vx = 0;
	vy = 0;
	accuracy = 5;    //how close movements follow selected points
	motion = 10;	 //amount of motion/drift
	pauseInterval = 0.15*FPS; //frames to wait to renew moveTo points
	collisionPad = 1;

	itemCollected = false;
	itemType = 0;

	itemDisplay.x = (x*zoom-xOffset);
	itemDisplay.y = (y*zoom-yOffset);
	itemDisplay.w = width*zoom;
	itemDisplay.h = height*zoom;

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

	//add mini timer
	delay = 100;
	currentTick = SDL_GetTicks();
	lastTick = currentTick;
	tickAccumulation += currentTick-lastTick;
}

Item::~Item()
{

}


void Item::update()
{
	//get ticks for timer
	lastTick = currentTick;
	currentTick = SDL_GetTicks();
	tickAccumulation += currentTick-lastTick;

	timer();


	int midX = x+width/2;
	int midY = y+height/2;

	//this is the equation for readability:
	//	distance = sqrt((x2-x1)^2 + (y2 - y1)^2)
	int distance = sqrt(pow(double(moveToPointX-midX), 2) + pow(double(moveToPointY-midY), 2));


	//test if the item is close enough to the player
	if (distance < maxDistance)
	{
		//test what directions to move
		if (midX < moveToPointX - accuracy)		//too far left
			vx += speed;
		else if (midX > moveToPointX + accuracy)	//too far right
			vx -= speed;
		else
			vx *= friction;

		if (midY < moveToPointY - accuracy)		//too far left
			vy += speed;
		else if (midY > moveToPointY + accuracy) //too far right
			vy -= speed;
		else
			vy *= friction;

		//don't go over speed limits
		if (vx > maxSpeed)
			vx = maxSpeed;
		else if (vx < -maxSpeed)
			vx = -maxSpeed;
		if (vy > maxSpeed)
			vy = maxSpeed;
		else if (vy < -maxSpeed)
			vy = -maxSpeed;
		
		//shrink if close to an followable object
		width -= 1;
		height -=1;
	}
	else
	{
		vx *= friction;
		vy *= friction;

		width += 1;
		height += 1;
	}


	oldX = x;
	oldY = y;

	x += vx;
	y += vy;


	if (width <= 0 || height <= 0)
	{
		if (width <= 0)
			width = 0;
		else if (height <= 0)
			height = 0;

		itemCollected = true;
	}
	else
	{
		if (width >= orgWidth)
			width = orgWidth;
		if (height >= orgHeight)
			height = orgHeight;
	}

	itemDisplay.x = (x*zoom-xOffset);
	itemDisplay.y = (y*zoom-yOffset);
	itemDisplay.w = width*zoom;
	itemDisplay.h = height*zoom;

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

} //END update()


void Item::draw()
{
	SDL_Rect tempShadow = {itemDisplay.x+3, itemDisplay.y+3, itemDisplay.w, itemDisplay.h};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	SDL_RenderFillRect(renderer, &tempShadow);

	SDL_SetRenderDrawColor(renderer, randomNumber(50, 100), 150, randomNumber(100, 200), 255);
	SDL_RenderFillRect(renderer, &itemDisplay);

	/*
	//renders the collision rects
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	SDL_RenderDrawRect(renderer, &collisionHorz);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
	SDL_RenderDrawRect(renderer, &collisionVert);
	*/
}


void Item::newMoveToPoint(Leader *leader)
{
	//this is called every (double pauseInterval) seconds
	moveToPointX = (leader->getMidX()) + randomNumber(-motion, motion);
	moveToPointY = (leader->getMidY()) + randomNumber(-motion, motion);

	followedSprite = leader;
}



void Item::timer()
{
	if (tickAccumulation >= delay)
	{
		updateFlag = true;
		tickAccumulation = 0;
	}
}



double Item::randomNumber(double Min, double Max)
{
    return ((double(rand()) / double(RAND_MAX)) * (Max - Min)) + Min;
}