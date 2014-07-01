#ifndef ITEM_H
#define ITEM_H

class Item : public Sprite
{
	private:
		Uint32 delay;
		bool itemCollected, flag;
		double pauseInterval, maxDistance;
		int moveToPointX, moveToPointY, accuracy, motion,
			collisionPad, itemType, itemID;
		int currentTick, lastTick, tickAccumulation;
		Leader *followedSprite;
		SDL_Rect itemDisplay;
		SDL_TimerID timerID;
		static Item *selfReference;

		void timer();



	public:
		Item();
		~Item();
		void update();
		void draw(); //wait... should draw be in child classes?
		void newMoveToPoint(Leader *leader);
		double randomNumber(double Min, double Max);

		double getSpeed() {return speed;}
		double getPauseInterval() {return pauseInterval;}
		int   getAccuracy() {return accuracy;}
		int   getMotion() {return motion;}
		int   getMoveToPointX() {return moveToPointX;}
		int   getMoveToPointY() {return moveToPointY;}
		int   getitemType()		{return itemType;}
		bool  getItemCollected(){return itemCollected;}
		bool  getFlag()			{return flag;}
		Leader* getfollowedSprite() {return followedSprite;}

		void setitemType(int newitemType) {itemType = newitemType;}
		void setSpeed(double newSpeed) {speed = newSpeed;}
		void setPauseInterval(double newPause) {pauseInterval = newPause*FPS;}
		void setAccuracy(int newAccuracy) {accuracy = newAccuracy;}
		void setMotion(int newMotion) {motion = newMotion;}
};

#endif