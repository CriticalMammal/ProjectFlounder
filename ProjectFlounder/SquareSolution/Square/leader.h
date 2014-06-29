#ifndef LEADER_H
#define LEADER_H

class Leader : public Sprite
{
	private:

	public:
		static const int uniqueItemCount = 1; //how many unique items are in the game
		int itemsHeld[uniqueItemCount];

		Leader();
		~Leader();

		int getItemsHeld(int itemID) {return itemsHeld[itemID];}

		void setItemsHeld(int newAmt, int itemID) {itemsHeld[itemID] = newAmt;}

};

#endif