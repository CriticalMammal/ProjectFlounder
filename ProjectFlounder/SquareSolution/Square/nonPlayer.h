#ifndef NONPLAYER_H
#define NONPLAYER_H


class NonPlayer : public Leader
{
	private:
		//variables
			Uint32 delay;
			double accuracy;
			bool nextPathReady;
			SDL_Rect nonPlayerRect;
			std::vector<pathCoord> storedPath;
			std::vector<pathCoord> currentPath;
			SDL_TimerID timerID;

		//functions
			static Uint32 timerFunction(Uint32, void*);

	public:
		NonPlayer();
		~NonPlayer();
		void update();
		void handleEvent();
		void setPathCoordinates(std::vector<pathCoord>);
		void draw();

		void setNextPathReady(bool setBool) {nextPathReady = setBool;}
		bool getNextPathReady() {return nextPathReady;}
};

#endif