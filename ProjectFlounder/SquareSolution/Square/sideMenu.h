#ifndef SIDEMENU_H
#define SIDEMENU_H

class SideMenu : public Sprite
{
	private:
		bool menuOpen;
		SDL_Rect menuRect;

	public:
		SideMenu();
		~SideMenu();

		void loadMenuDesign(std::string path);
		void draw();
		void update();


		SDL_Rect getMenuRect() {return menuRect;}
		bool getMenuOpen() {return menuOpen;}

		void setMenuOpen(bool boolIn) {menuOpen = boolIn;}
};

#endif
