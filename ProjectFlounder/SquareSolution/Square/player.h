#ifndef PLAYER_H
#define PLAYER_H

class Player : public Sprite
{
	private:
		int collisionPad;
		static const int uniqueItemCount = 1; //how many unique items are in the game
		int itemsHeld[uniqueItemCount];
		SDL_Rect playerRect; //used to draw the player square

	public:
		Player();
		~Player();
		void update();
		void draw();
		
		//player rect is the square drawn to represent
		//the character
		SDL_Rect getPlayerRect() {return playerRect;}
		int getItemsHeld(int itemID) {return itemsHeld[itemID];}

		void setItemsHeld(int newAmt, int itemID) {itemsHeld[itemID] = newAmt;}
};

#endif