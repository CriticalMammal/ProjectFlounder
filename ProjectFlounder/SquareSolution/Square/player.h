#ifndef PLAYER_H
#define PLAYER_H

class Player : public Leader
{
	private:
		int collisionPad;
		SDL_Rect playerRect; //used to draw the player square

	public:
		Player();
		~Player();
		void update();
		void draw();
		
		//player rect is the square drawn to represent
		//the character
		SDL_Rect getPlayerRect() {return playerRect;}
};

#endif