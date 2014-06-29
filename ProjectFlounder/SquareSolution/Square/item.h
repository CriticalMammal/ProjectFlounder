#ifndef ITEM_H
#define ITEM_H

class Item : public Sprite
{
	private:
		bool itemCollected;
		double pauseInterval, maxDistance;
		int moveToPointX, moveToPointY, accuracy, motion,
			collisionPad, itemID;
		SDL_Rect itemDisplay;

	public:
		Item();
		~Item();
		void update();
		void draw(); //wait... should draw be in child classes?
		void newMoveToPoint(Sprite *sprite);
		double randomNumber(double Min, double Max);

		double getSpeed() {return speed;}
		double getPauseInterval() {return pauseInterval;}
		int   getAccuracy() {return accuracy;}
		int   getMotion() {return motion;}
		int   getMoveToPointX() {return moveToPointX;}
		int   getMoveToPointY() {return moveToPointY;}
		int   getItemID()		{return itemID;}
		bool  getItemCollected(){return itemCollected;}

		void setItemID(int newItemID) {itemID = newItemID;}
		void setSpeed(double newSpeed) {speed = newSpeed;}
		void setPauseInterval(double newPause) {pauseInterval = newPause*FPS;}
		void setAccuracy(int newAccuracy) {accuracy = newAccuracy;}
		void setMotion(int newMotion) {motion = newMotion;}
};

#endif