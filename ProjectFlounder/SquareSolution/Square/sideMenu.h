//template for any side menu you may want to create

#ifndef SIDEMENU_H
#define SIDEMENU_H

class SideMenu : public Sprite
{
	protected:
		bool menuOpen;
		SDL_Rect menuRect;

		double menuOpenCoordX;
		double menuOpenCoordY;

		double menuClosedCoordX;
		double menuClosedCoordY;


	public:
		SideMenu();
		~SideMenu();

		void loadMenuDesign(std::string path);
		void draw();
		void update();

		//getter/setters
		SDL_Rect getMenuRect() {return menuRect;}
		bool getMenuOpen() {return menuOpen;}
		double getMenuOpenCoordX() {return menuOpenCoordX;}
		double getMenuOpenCoordY() {return menuOpenCoordY;}

		void setMenuOpen(bool boolIn) {menuOpen = boolIn;}
};

#endif
