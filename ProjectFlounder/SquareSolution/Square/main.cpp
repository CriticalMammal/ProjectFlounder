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
#include "leader.h"
#include "player.h"
#include "nonPlayer.h"
#include "item.h"
#include "tile.h"
#include "tileMap.h"
#include "camera.h"
#include "button.h"
#include "sideMenu.h"
#include "tileEditorMenu.h"

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

TileEditorMenu tileEditorMenu;

vector<Item*> items;

TileMap theMap;


int countedFrames = 8;
double xOffset, yOffset, zoom;

<<<<<<< HEAD
=======
const int mapWidth = 50, mapHeight = 50;
>>>>>>> origin/feature-inventoryZoom
const int blockWidth = 20, blockHeight = 20;

//the time it would take to cross from one side to another
float minutes = 1;
float mapWidthInMinutes = (3600/blockWidth)*minutes;
float mapHeightInMinutes = (3600/blockHeight)*minutes;

const int mapWidth = mapWidthInMinutes, mapHeight = mapHeightInMinutes;
int mapWidthInPixels = mapWidth*(blockWidth);
int mapHeightInPixels = mapHeight*(blockHeight);

Uint32 startTime = 0;
string timeText = "";

bool keys[] = {false, false, false, false, false, false};
bool leftClick = false, rightClick = false;
int mouseX, mouseY;


//function prototypes
bool init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
SDL_Texture* renderText(const std::string &message, const std::string &fontLocation, 
						SDL_Color color, int fontSize);
bool collisionDetect(SDL_Rect rect1, SDL_Rect rect2);
double randomNumber(double, double);
void close();



int main(int argc, char *args[])
{
	bool quit = false;
	bool changeBlock = false, addItem = false, openPlayerInventory = false,
		 editMode = false;
	int cameraTime = 0, itemToUpdate = 0;

	mouseX = 0;
	mouseY = 0;

	xOffset = 0;
	yOffset = 0;
<<<<<<< HEAD
	zoom = 2;
	srand(time(NULL));
=======
	zoom = 1;
>>>>>>> origin/feature-inventoryZoom

	srand((unsigned)time(NULL)); //seed random numbers


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


	player = new Player;
	nonPlayer = new NonPlayer;

	camera.setfollowedObject(player);

	vector<pathCoord> path;
	vector<int> itemsUpdateList;


	//initialize tilemap
	theMap.initialize("generate-new", mapHeight, mapWidth, blockHeight, blockWidth, *renderer);

	//initialize menus
	tileEditorMenu.setTileImages(theMap.getBlocks());
	
	//randomly place items in game
	items.push_back(new Item);
	items[0]->setX(-1000);
	items[0]->setY(-1000);

	int itemAmt = 200;
	for (int i=0; i<itemAmt; i++)
	{
		double itemX = randomNumber(blockWidth, mapWidthInPixels);
		double itemY = randomNumber(blockHeight, mapWidthInPixels);
		int width = items[0]->getWidth();
		int height = items[0]->getHeight();
		
		SDL_Rect itemRect = {int(itemX), int(itemY), width, height};

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
	

	//set non player position
	nonPlayer->setX(80);
	nonPlayer->setY(80);


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
					case SDLK_TAB:
						keys[TAB] = true;
						player->setInventoryOpen(!player->getInventoryOpen());
						break;
					case SDLK_i:
						editMode = !editMode;
						tileEditorMenu.setMenuOpen(!tileEditorMenu.getMenuOpen());
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
					case SDLK_TAB:
						keys[TAB] = false;
						break;
				}
			}
			else if (evt.type == SDL_MOUSEBUTTONDOWN)
			{
				if (evt.button.button == SDL_BUTTON_LEFT)
				{
					leftClick = true;

					if (editMode)
					{
						changeBlock = true;
					}
				}
				else if (evt.button.button == SDL_BUTTON_RIGHT)
				{
					rightClick = true;

					if (editMode)
					{
						addItem = true;
					}
				}
			}
			else if (evt.type == SDL_MOUSEBUTTONUP)
			{
				if (evt.button.button == SDL_BUTTON_LEFT)
				{
					leftClick = false;
					changeBlock = false;
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
			//find closest item to npc
			int closestItem = 0;
			int closestDist = INT_MAX;
			int currentDist = INT_MAX;

			for (int i=0; i<items.size(); i++)
			{
				int nextTileDistX = abs(nonPlayer->getX() - items[i]->getX());
				int nextTileDistY = abs(nonPlayer->getY() - items[i]->getY());
				currentDist = nextTileDistX+nextTileDistY;

				if (currentDist < closestDist)
				{
					closestItem = i;
					closestDist = currentDist;
				}
			}

			path = theMap.pathFind(nonPlayer->getMidX(), nonPlayer->getMidY(), items[closestItem]->getMidX(), items[closestItem]->getMidY());

			nonPlayer->setPathCoordinates(path);
			nonPlayer->setNextPathReady(false);
		}



		//get mouse mouse clicks
		SDL_GetMouseState(&mouseX, &mouseY);


		//handle menus
		tileEditorMenu.update();
		tileEditorMenu.updateTileEditor();

		//changes the tile that the mouse is hovering over
		if (changeBlock && tileEditorMenu.getMenuMouseFocus() == false)
		{
			theMap.changeTileAt((mouseX+xOffset)/zoom, (mouseY+yOffset)/zoom, tileEditorMenu.getClickedButtonType());
			//changeBlock = false;
		}
		if (addItem && tileEditorMenu.getMenuMouseFocus() == false)
		{
			items.push_back(new Item);
			items.back()->setX((mouseX+xOffset)/zoom);
			items.back()->setY((mouseY+yOffset)/zoom);
			addItem = false;
		}



		//HANDLE ITEMS
		for (int i=0; i<items.size(); i++)
		{
			if (items[i]->getUpdateFlag())
			{
				if (collisionDetect(player->getCollisionRect(), items[i]->getCollisionRect()))
				{
					items[i]->newMoveToPoint(player);
				}
				else if (collisionDetect(nonPlayer->getCollisionRect(), items[i]->getCollisionRect()))
				{
					items[i]->newMoveToPoint(nonPlayer);
				}
			}

			items[i]->update();

			if (items[i]->getItemCollected())
			{
				Leader *leaderCollector = items[i]->getfollowedSprite();
				int currentitemType = items[i]->getitemType();
				int heldItems = leaderCollector->getItemsHeld(currentitemType);

				//add item to followed object
				leaderCollector->setItemsHeld(heldItems + 1, currentitemType);
				system("cls");
				cout << "player items: " << player->getItemsHeld(currentitemType) << endl;
				cout << "npc items:    " << nonPlayer->getItemsHeld(currentitemType) << endl;

				//remove item
				items[i]->~Item();
				items.erase(items.begin()+i);
				i--;
			}
		}


		//HANDLE CAMERA
		mapWidthInPixels = mapWidth*(blockWidth*zoom);
		mapHeightInPixels = mapHeight*(blockHeight*zoom);

		camera.handleKeys(player->getSpeed());
		cameraTime ++;
		if (cameraTime >= camera.getCameraPause())
		{
<<<<<<< HEAD
			camera.newMoveToPoint(player);
			camera.newZoom(2);
=======
			if (player->getInventoryOpen())
			{
				//zoom very close into the player
				camera.setZoomVelocity(0);
				camera.newZoom(40, 0.001, 0.2, 0.01);

				camera.setAccuracy(5);
				camera.setMotion(0);
				camera.setFriction(0.8);

				camera.newMoveToPoint(player, 2, 0);
			}
			else
			{
				//normal camera movement etc
				camera.setZoomVelocity(0);
				camera.newZoom(2.5, 0.001, 0.2, 0.01);

				camera.setAccuracy(5);
				camera.setMotion(0);
				camera.setFriction(0.8);

				camera.newMoveToPoint(player, 0, 0);
			}

>>>>>>> origin/feature-inventoryZoom
			cameraTime = 0;
		}

		camera.scrollScreen();
		
		
		//update map coordinates
		theMap.setX(-xOffset);
		theMap.setY(-yOffset);




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
		

		//------
		//RENDER
		//------

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		//draw all the tiles
		SDL_Rect screenRect = {xOffset, yOffset, xOffset+SCREEN_WIDTH, yOffset+SCREEN_HEIGHT};
		theMap.drawTileMap(screenRect);
		
		//render player
		player->draw();
		nonPlayer->draw();

		//render items
		for (int i=0; i<items.size(); i++)
		{
			items[i]->draw();
		}

		//render menus
		tileEditorMenu.draw();
		tileEditorMenu.drawButtons();

		
		
		//render camera focus point
		/*
		SDL_Rect cameraPoint = {(camera.getMoveToPointX()+SCREEN_WIDTH/2)-xOffset-1, (camera.getMoveToPointY()+SCREEN_HEIGHT/2)-yOffset-1, 2, 2}; 
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &cameraPoint);
		
		SDL_Rect accuracyBox = {((camera.getMoveToPointX()+SCREEN_WIDTH/2)-xOffset)-camera.getAccuracy(), 
			((camera.getMoveToPointY()+SCREEN_HEIGHT/2)-yOffset)-camera.getAccuracy(), camera.getAccuracy()*2, camera.getAccuracy()*2};
		SDL_RenderDrawRect(renderer, &accuracyBox);
		*/

		//render fly on the screen
		/*
		SDL_Rect marker = {300, 400, 2, 2};
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &marker);
		*/

		//render text
		SDL_RenderCopy(renderer, text, NULL, &textRect);

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
double randomNumber(double Min, double Max)
{
    return ((double(rand()) / double(RAND_MAX)) * (Max - Min)) + Min;
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