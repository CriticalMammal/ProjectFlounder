//FILE : camera.cpp
//PROG : Dylan Gallardo
//PURP : camera.cpp scrolls the tile-based background to positions based on
//		 the player's location. The background itself is handled by mappy_A5.c
//		 tile background created by Mappy, (which is an open source tile mapping program)

#include <SDL.h>
#include <stdlib.h>

#include "definitions.h"
#include "sprite.h"
#include "camera.h"

extern bool keys[];
extern double xOffset, yOffset, zoom;
extern int mapWidthInPixels, mapHeightInPixels;


Camera::Camera()
{
	moveToPointX = 0;
	moveToPointY = 0;
	zoomToPoint = 1;
	xOffset = moveToPointX;
	yOffset = moveToPointY;
	speed = 0.01;
	maxSpeed = 2;
	friction = 0.8;
	vx = 0;
	vy = 0;
	accuracy = 5;    //how close the camera moves to the points
	motion = 10;	 //amount of camera motion/drift
	cameraPause = 0.2*FPS; //frames to wait to renew moveTo points
}


Camera::~Camera()
{

}


void Camera::handleKeys(double objectSpeed)
{
	if (keys[LEFT])
	{
		xOffset += -objectSpeed;
	}
	else if (keys[RIGHT])
	{
		xOffset += objectSpeed;
	}
	

	if (keys[UP])
	{
		yOffset += -objectSpeed;
	}
	else if (keys[DOWN])
	{
		yOffset += objectSpeed;
	}
}


//scroll camera/screen towards the defined moveTo coordinates
void Camera::scrollScreen()
{
	//test what directions to scroll screen
	if (xOffset < moveToPointX - accuracy)		//too far left
		vx += speed;
	else if (xOffset > moveToPointX + accuracy)	//too far right
		vx -= speed;
	else
		vx *= friction;

	if (yOffset < moveToPointY - accuracy)		//too far left
		vy += speed;
	else if (yOffset > moveToPointY + accuracy) //too far right
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


	xOffset += vx;
	yOffset += vy;

	//handle zooming
	double zoomSpeed = 0.00001;
	double zoomAccuracy = 0.01;
	double zoomMaxSpeed = 0.1;

	if (zoom < zoomToPoint - zoomAccuracy)
		zoomVelocity += zoomSpeed;
	else if (zoom > zoomToPoint + zoomAccuracy)
		zoomVelocity -= zoomSpeed;
	else
		zoomVelocity *= friction;

	if (zoomVelocity > zoomMaxSpeed)
		zoomVelocity = zoomMaxSpeed;
	else if (zoomVelocity < -zoomMaxSpeed)
		zoomVelocity = -zoomMaxSpeed;

	zoom += zoomVelocity;

	//don't go past boundaries with camera
	if (xOffset < 0) 
		xOffset = 0;
	else if (xOffset > mapWidthInPixels - SCREEN_WIDTH) 
		xOffset = mapWidthInPixels - SCREEN_WIDTH;

	if (yOffset < 0 ) 
		yOffset = 0;
	else if (yOffset > mapHeightInPixels - SCREEN_HEIGHT) 
		yOffset = mapHeightInPixels - SCREEN_HEIGHT;
}


//this is called every (double cameraPause) seconds
void Camera::newMoveToPoint(Sprite *sprite)
{
	moveToPointX = ((sprite->getMidX()*zoom)-0.5*SCREEN_WIDTH) + randomNumber(-motion, motion);
	moveToPointY = ((sprite->getMidY()*zoom)-0.5*SCREEN_HEIGHT) + randomNumber(-motion, motion);
}


void Camera::newZoom(double newZoom)
{
	zoomToPoint = newZoom;
}


//come up with a better solution to distribute the random
//numbers evenly. rand() doesn't evenly distribute!
double Camera::randomNumber(double Min, double Max)
{
    return ((double(rand()) / double(RAND_MAX)) * (Max - Min)) + Min;
}