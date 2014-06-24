//main.cpp - main class for game

#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream> //you don't need this if you aren't using printf
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>

#include "definitions.h"
#include "sprite.h"
#include "player.h"
#include "nonPlayer.h"
#include "item.h"
#include "tile.h"
#include "tileMap.h"
#include "camera.h"


using namespace std;

//variables
SDL_Event evt;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *surface = NULL;
SDL_Color color = {255, 255, 255};

Camera camera;

Player *player;
NonPlayer *nonPlayer;

vector<Item*> items;

TileMap theMap;

int countedFrames = 8;
double xOffset, yOffset, zoom;
const int mapWidth = 50, mapHeight = 50;
const int blockWidth = 20, blockHeight = 20;
int mapWidthInPixels = mapWidth*(blockWidth);
int mapHeightInPixels = mapHeight*(blockHeight);
Uint32 startTime = 0;
string timeText = "";
bool keys[] = {false, false, false, false, false};
bool leftClick = false, rightClick = false;


//function prototypes
bool init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
SDL_Texture* renderText(const std::string &message, const std::string &fontLocation, 
						SDL_Color color, int fontSize);
bool collisionDetect(SDL_Rect rect1, SDL_Rect rect2);
float randomNumber(float, float);
void close();


int main(int argc, char *args[])
{
	bool quit = false;
	bool changeBlock = false, addItem = false;
	int cameraTime = 0;
	int mouseX = 0, mouseY = 0;
	xOffset = 0;
	yOffset = 0;
	zoom = 1;
	srand(time(NULL));

	player = new Player;
	nonPlayer = new NonPlayer;

	vector<pathCoord> path;

	//Initialize SDL
	if (!init())
	{
		printf("Failed to initialize\n");
		return -1;
	}

	if (!loadMedia())
	{
		printf("Failed to load media\n");
		return -1;
	}

	unsigned char treeAlpha = 255;
	SDL_Rect tree = {350, 250, 150, 150};

	//initialize tilemap
	theMap.initialize("mapFile.txt", mapHeight, mapWidth, blockHeight, blockWidth, *renderer);

	
	//randomly place items in game
	items.push_back(new Item);
	items[0]->setX(1000);
	items[0]->setY(1000);

	int itemAmt = 0;
	for (int i=0; i<itemAmt; i++)
	{
		int itemX = randomNumber(blockWidth, mapWidthInPixels);
		int itemY = randomNumber(blockHeight, mapWidthInPixels);
		int width = items[0]->getWidth();
		int height = items[0]->getHeight();
		
		SDL_Rect itemRect = {itemX, itemY, width, height};

		while (theMap.checkCollision(itemRect))
		{
			itemX = randomNumber(blockWidth, mapWidthInPixels);
			itemY = randomNumber(blockHeight, mapWidthInPixels);
			itemRect.x = itemX;
			itemRect.y = itemY;
		}

		items.push_back(new Item);
		items.back()->setX(itemX);
		items.back()->setY(itemY);
	}
	

	//set path for non player (TEMPORARY)
	nonPlayer->setX(25);
	nonPlayer->setY(25);

	/*
	//print out path coordinates
	cout << "\n\n";
	for (int i=0; i<path.size(); i++)
	{
		cout << "x: " << path[i].x << "y: " << path[i].y << endl;
	}
	*/

	//--------
	//MAIN LOOP
	//---------
	while (!quit)
	{
		//check for events
		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (evt.type == SDL_KEYDOWN)
			{
				switch(evt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_UP:
						keys[UP] = true;
						break;
					case SDLK_DOWN:
						keys[DOWN] = true;
						break;
					case SDLK_RIGHT:
						keys[RIGHT] = true;
						break;
					case SDLK_LEFT:
						keys[LEFT] = true;
						break;
					case SDLK_c:
						keys[c] = true;
						break;
				} //END switch(evt.key.keysym.sym)
			}
			else if (evt.type == SDL_KEYUP)
			{
				switch (evt.key.keysym.sym)
				{
					case SDLK_UP:
						keys[UP] = false;
						break;
					case SDLK_DOWN:
						keys[DOWN] = false;
						break;
					case SDLK_RIGHT:
						keys[RIGHT] = false;
						break;
					case SDLK_LEFT:
						keys[LEFT] = false;
						break;
					case SDLK_c:
						keys[c] = false;
						break;
				}
			}
			else if (evt.type == SDL_MOUSEBUTTONDOWN)
			{
				if (evt.button.button == SDL_BUTTON_LEFT)
				{
					leftClick = true;
					changeBlock = true;
				}
				else if (evt.button.button == SDL_BUTTON_RIGHT)
				{
					rightClick = true;
					addItem = true;
				}
			}
			else if (evt.type == SDL_MOUSEBUTTONUP)
			{
				if (evt.button.button == SDL_BUTTON_LEFT)
				{
					leftClick = false;
				}
				else if (evt.button.button == SDL_BUTTON_RIGHT)
				{
					rightClick = false;
				}
			}
			else if (evt.user.code == NPC_EVENT)
			{
				nonPlayer->handleEvent();
			}
		}


		//-----
		//LOGIC
		//-----

		//calculate frames
		float avgFPS = countedFrames / ((SDL_GetTicks()-startTime)/1000.f);
		if (avgFPS>2000000)
			avgFPS = 0;
		
		//handle player
		player->update();

		//handle NPC
		nonPlayer->update();

		if (nonPlayer->getNextPathReady())
		{
			//NOTE: something goes wrong when path tries to find a path to it's current coordinates
			//not exactly sure what causes the crash, but it definitely seems like something
			//goes wrong in the theMap.pathFind() function when you ask it to find a way to the current tile
			path = theMap.pathFind(nonPlayer->getMidX(), nonPlayer->getMidY(), player->getMidX(), player->getMidY());

			nonPlayer->setPathCoordinates(path);
			nonPlayer->setNextPathReady(false);
		}

		//handle items
		//BE CAREFUL TO REMEMBER AND CREATE
		//individual timers for each instance of an item
		//rather than basing it off of the cameraTime!!!!
		for (int i=0; i<items.size(); i++)
		{
			if (cameraTime >= items[i]->getPauseInterval())
			{
				items[i]->newMoveToPoint(player);
			}

			items[i]->update();
		}

		//handle camera
		mapWidthInPixels = mapWidth*(blockWidth*zoom);
		mapHeightInPixels = mapHeight*(blockHeight*zoom);

		camera.handleKeys(player->getSpeed());
		cameraTime ++;
		if (cameraTime >= camera.getCameraPause())
		{
			camera.newMoveToPoint(player);
			camera.newZoom(abs(1.5-(abs(player->getvy()/4)+abs(player->getvx()/4))));
			cameraTime = 0;
		}
		camera.scrollScreen();
		
		
		//update map coordinates
		theMap.setX(-xOffset);
		theMap.setY(-yOffset);


		SDL_GetMouseState(&mouseX, &mouseY);
		//changes the tile that the mouse is hovering over
		if (changeBlock)
		{
			theMap.changeTileAt((mouseX+xOffset)/zoom, (mouseY+yOffset)/zoom);
			changeBlock = false;
		}
		if (addItem)
		{
			items.push_back(new Item);
			items.back()->setX((mouseX+xOffset)/zoom);
			items.back()->setY((mouseY+yOffset)/zoom);
			addItem = false;
		}


		//COLLISIONS

		//check for player collision with the tile map
		if (theMap.checkCollision(player->getCollisionRectHorz()))
		{
			player->setX(player->getOldX());
			player->setVx(0);
		}
		if (theMap.checkCollision(player->getCollisionRectVert()))
		{
			player->setY(player->getOldY());
			player->setVy(0);
		}

		//check for item collisions
		for (int i=0; i<items.size(); i++)
		{
			/*
			//not ready yet due to items causing "traffic jams" with each other
			//check collisions with other items
			for (int t=0; t<items.size(); t++)
			{
				if (i!=t)
				{
					if (collisionDetect(items[i]->getCollisionRectHorz(), items[t]->getCollisionRect()))
					{
						items[i]->setX(items[i]->getOldX());
						items[i]->setVx(0);

						//items[t]->setX(items[t]->getOldX());
						//items[t]->setVx(0);
					}
					if (collisionDetect(items[i]->getCollisionRectVert(), items[t]->getCollisionRect()))
					{
						items[i]->setY(items[i]->getOldY());
						items[i]->setVy(0);

						//items[t]->setY(items[t]->getOldY());
						//items[t]->setVy(0);
					}
				}
			}
			*/

			//check collisions with tile map
			if (theMap.checkCollision(items[i]->getCollisionRectHorz()))
			{
				items[i]->setX(items[i]->getOldX());
				items[i]->setVx(0);
			}
			if (theMap.checkCollision(items[i]->getCollisionRectVert()))
			{
				items[i]->setY(items[i]->getOldY());
				items[i]->setVy(0);
			}
		}

		/*
		//UPDATE TEXT
		
		//set text
		SDL_Rect textRect = {0, 0, 150, 20};
		timeText = "Time Passed: " + std::to_string((SDL_GetTicks() - startTime)/1000);
		//SDL_Texture *text = renderText(to_string(avgFPS), "Prototype.ttf", color, 30);
		SDL_Texture *text = renderText(to_string(zoom), "Prototype.ttf", color, 30);

		int textW, textH;
		SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
		int textX = SCREEN_WIDTH/2 - textW/2;
		int textY = SCREEN_HEIGHT/2 - textH/2;
		*/

		//------
		//RENDER
		//------

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		//draw all the tiles
		theMap.drawTileMap();
		
		//render player
		player->draw();
		nonPlayer->draw();

		//render items
		for (int i=0; i<items.size(); i++)
		{
			items[i]->draw();
		}

		
		/*
		//render camera focus point
		SDL_Rect cameraPoint = {(camera.getMoveToPointX()+SCREEN_WIDTH/2)-xOffset-1, (camera.getMoveToPointY()+SCREEN_HEIGHT/2)-yOffset-1, 2, 2}; 
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &cameraPoint);
		
		SDL_Rect accuracyBox = {((camera.getMoveToPointX()+SCREEN_WIDTH/2)-xOffset)-camera.getAccuracy(), 
			((camera.getMoveToPointY()+SCREEN_HEIGHT/2)-yOffset)-camera.getAccuracy(), camera.getAccuracy()*2, camera.getAccuracy()*2};
		SDL_RenderDrawRect(renderer, &accuracyBox);
		*/

		//render text
		//SDL_RenderCopy(renderer, text, NULL, &textRect);

		SDL_RenderPresent(renderer);
		countedFrames++;
	}


	theMap.saveMapFile(); //quickly write map to file
	close(); //Quit SDL

	return 0;
}; //END main();



bool init()
{
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Init failed. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//create window
	window = SDL_CreateWindow("SDL Prototype", 600, 40, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("SDL Window failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		printf("SDL renderer failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//init TTF
	if (TTF_Init() != 0)
	{
		printf("TTF failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		cout << "Timer failed to init. SDL_ERROR: " << SDL_GetError() << endl;
		return false;
	}

	return true;
}


bool loadMedia()
{
	return true;
}


SDL_Texture* loadTexture (std::string path)
{
	//final image
	SDL_Texture* newTexture = NULL;

	//Load Image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (!loadedSurface)
	{
		printf("Failed to load image %s. SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		//convert surface to screen format
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (!newTexture)
		{
			printf("Failed to create texture %s. SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}

		//free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
} //END loadSurface()


SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
		SDL_Color color, int fontSize)
{
	//open font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (!font)
	{
		printf("Failed to load font. TTF_Error: %s", TTF_GetError());
		return NULL;
	}

	//create surface... then convert surface to a texture
	SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), color);
	if (!surface)
	{
		TTF_CloseFont(font);
		printf("Failed to render font to surface. SDL_Error: %s", SDL_GetError());
		return NULL;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		printf("Failed to create texture from surface. SDL_Error: %s", SDL_GetError());
		return NULL;
	}

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return texture;
} //END renderText()


bool collisionDetect(SDL_Rect rect1, SDL_Rect rect2)
{
	int left1, right1, top1, bottom1,
		left2, right2, top2, bottom2;

	//get values for rect1
	left1 = rect1.x;
	right1 = left1+rect1.w;
	top1 = rect1.y;
	bottom1 = top1+rect1.h;
	//get values for rect2
	left2 = rect2.x;
	right2 = left2+rect2.w;
	top2 = rect2.y;
	bottom2 = top2+rect2.h;

	//test it
	if (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2)
		return true;
	else 
		return false;

} //END collisionDetect


//come up with a better solution to distribute the random
//numbers evenly. rand() doesn't evenly distribute!
float randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}


void close()
{
	player->~Player();

	for (int i=0; i<items.size(); i++)
		items[i]->~Item();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	TTF_Quit();
	SDL_Quit();
}