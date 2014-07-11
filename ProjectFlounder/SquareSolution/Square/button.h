#ifndef BUTTON_H
#define BUTTON_H

class Button : public Sprite
{
	private:
		double menuXOffset, menuYOffset;
		bool mouseClicked, buttonIsHovered, buttonClicked;
		//double mouseXpos, mouseYpos;
		SDL_Texture* buttonTexture;

		bool pointCollisionCheck(double x, double y);

	public:
		Button();
		~Button();

		void init(SDL_Rect rectIn);
		void update();
		void draw();

		void setMouseClicked(bool boolIn) {mouseClicked = boolIn;}
		void setButtonClicked(bool boolIn) {buttonClicked = boolIn;}
		void setButtonHovered(bool boolIn) {buttonIsHovered = boolIn;}
		void setImage(SDL_Texture* texture) {buttonTexture = texture;}
		void setMenuXOffset(double xVal) {menuXOffset = xVal;}
		void setMenuYOffset(double yVal) {menuYOffset = yVal;}

		bool getButtonClicked() {return buttonClicked;}
};

#endif