#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <SDL.h>
#include <vector>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int STEP = 1;

const int HEALTHBAR_X = 10;
const int HEALTHBAR_Y = 450;
const int STOP_QUACKING_CTR = 20;

class Duck;
class Bench;
class Grandma;
class BenchScene;
class WaterScene;

class Game{
	private:
		SDL_Surface *screen;
		SDL_Surface *splash;
		SDL_Surface *background;
		Duck *duck;
		SDL_Surface *healthBar;
		std::vector<SDL_Surface*> healthBarImages;
		BenchScene *benchScene;
		WaterScene *waterScene;
		Mix_Music *splashMusic;
		Mix_Music *mainTheme;
	public:
		Game();
		SDL_Surface* getScreen(); //returns pointer to the screen
		void play(); //initiate play function, handling keyboard calls
		void showSplash();
		SDL_Surface* getBackground();
		Duck* getDuck(){return duck;}; //return duck
};

#endif
