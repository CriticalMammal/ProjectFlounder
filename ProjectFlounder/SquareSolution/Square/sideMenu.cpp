#include <SDL.h>
#include <string>
#include <iostream>

#include "definitions.h"
#include "sprite.h"
#include "sideMenu.h"

extern SDL_Renderer* renderer;

SideMenu::SideMenu()
{
	menuOpen = false;


	width = SCREEN_WIDTH/4;
	height = 350;
	x = SCREEN_WIDTH+10;
	y = (SCREEN_HEIGHT/2)-(height/2);


	menuClosedCoordX = SCREEN_WIDTH+10;
	menuClosedCoordY = y;

	menuOpenCoordX = SCREEN_WIDTH-width;
	menuOpenCoordY = y;


	vx =0;
	vy = 0;
	friction = 0.8;
	speed = 0.2;
	maxSpeed = 4;


	menuRect.x = x;
	menuRect.y = y;
	menuRect.h = height;
	menuRect.w = width;

	//create clickable icons

}



SideMenu::~SideMenu()
{

}



void SideMenu::loadMenuDesign(std::string path)
{
	//load an image for the menu if you make them?
}



void SideMenu::update()
{
	if (menuOpen)
	{
		//open the menu
		if (x > menuOpenCoordX)
		{
			vx -= speed;
		}
	}
	else
	{
		//close the menu
		if (x < menuClosedCoordX)
		{
			vx += speed;
		}
	}

	if (vx > maxSpeed)
		vx = maxSpeed;
	if (vx < -maxSpeed)
		vx = -maxSpeed;


	x += vx;


	//don't go past boundaries
	if (x >= menuClosedCoordX)
	{
		x = menuClosedCoordX;
	}
	else if (x <= menuOpenCoordX)
	{
		x = menuOpenCoordX;
	}

	//update rect
	menuRect.x = x;
	menuRect.y = y;
	menuRect.h = height;
	menuRect.w = width;
}



void SideMenu::draw()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &menuRect);
}