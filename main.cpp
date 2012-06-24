#include "game.h"
#include <iostream>
using namespace std;

void drid();

int main(){
	//Game *game;
	//cin.get();
	
	//We must first initialize the SDL video component, and check for success
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_WM_SetCaption("NINJA DUCK", "NINJA DUCK");
	//When this program exits, SDL_Quit must be called
	atexit(SDL_Quit);


	Game derp;
	derp.showSplash();
	derp.play();
	return 0;
}
