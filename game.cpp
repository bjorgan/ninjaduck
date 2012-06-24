#include "game.h"
#include "sprites.h"
#include <SDL.h>
#include <SDL_mixer.h> 
#include <vector>
#include "scenes.h"
using namespace std;

Game::Game(){
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_DOUBLEBUF);
	SDL_Surface *temp2 = SDL_LoadBMP("images/splash.bmp");
	splash = SDL_DisplayFormat(temp2);
	SDL_FreeSurface(temp2);

	healthBarImages.push_back(SDL_LoadBMP("images/bar1.bmp"));
	healthBarImages.push_back(SDL_LoadBMP("images/bar2.bmp"));
	healthBarImages.push_back(SDL_LoadBMP("images/bar3.bmp"));
	healthBarImages.push_back(SDL_LoadBMP("images/bar4.bmp"));
	healthBarImages.push_back(SDL_LoadBMP("images/bar5.bmp"));
	healthBarImages.push_back(SDL_LoadBMP("images/bar6.bmp"));

	healthBar = SDL_DisplayFormat(healthBarImages[0]);


		//audiodritt, hentet rett fra tutorial
	int audio_rate = 22050;			//Frequency of audio playback
	Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
	int audio_channels = 2;			//2 channels = stereo
	int audio_buffers = 4096;		//Size of the audio buffers in memory
	
	
	//Initialize SDL_mixer with our chosen audio settings
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	
	duck = new Duck(this);
	benchScene = new BenchScene(this);
	waterScene = new WaterScene(this);



	benchScene->setRight(benchScene);
	benchScene->setLeft(waterScene);
	//benchScene->setLeft(benchScene);

	waterScene->setLeft(waterScene);
	waterScene->setRight(benchScene);

	splashMusic = Mix_LoadMUS("music/splash2.wav");
	mainTheme = Mix_LoadMUS("music/maintheme.wav");

}

void Game::showSplash(){
	bool clicked = false;
	SDL_Event event;
	SDL_Rect splashrect;
	splashrect.w = splash->w;
	splashrect.h = splash->h;
	splashrect.x = 0;
	splashrect.y = 0;
	//play intro music
	Mix_PlayMusic(splashMusic, -1);

	while (!clicked){
		if (SDL_PollEvent(&event)){
			if (event.type == SDL_KEYDOWN){
					if (event.key.keysym.sym == SDLK_RETURN){
						clicked = true;
					}
			}
		}
		SDL_BlitSurface(splash, NULL, screen, &splashrect);
		SDL_Flip(screen);
	}
	Mix_HaltMusic();
	SDL_FreeSurface(splash);
}

SDL_Surface* Game::getScreen(){
	return screen;
}



void Game::play(){
	bool finished = false;
	SDL_Event event;
	Uint8 *keystate;
	
	bool isQuacking = false;
	int quackingCtr;


	SDL_Rect hbrect;
	hbrect.w = healthBar->w;
	hbrect.h = healthBar->h;
	hbrect.x = HEALTHBAR_X;
	hbrect.y = HEALTHBAR_Y;

	//play main theme music
	Mix_PlayMusic(mainTheme, -1);
	Scene *currScene = benchScene;

	while (!finished){
		//keyboard check
		if (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					finished = true;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym){
						case SDLK_SPACE:
							isQuacking = true;
							currScene->quacked();
							quackingCtr = 0;
							break;
						case SDLK_q:
							finished=true;
							break;
						case SDLK_k:
							duck->startSwim();
							break;

					}
				break;
			}
		}

		//move duck
		keystate = SDL_GetKeyState(NULL);
		if (keystate[SDLK_LEFT]){
			duck->move(LEFT);
		} else if (keystate[SDLK_RIGHT]){
			duck->move(RIGHT);
		} else if ((quackingCtr == 0) && isQuacking){
			duck->quack();
		}


		
		//quack duck
		if (isQuacking == true){
			quackingCtr++;
		}
		if ((quackingCtr > STOP_QUACKING_CTR) && isQuacking){
			duck->stopQuack();
			isQuacking = false;
		}

		//duck and scene reload
		currScene->action();
		duck->refresh();

		//health bar
		SDL_FreeSurface(healthBar);
		healthBar = SDL_DisplayFormat(healthBarImages[duck->getHealth()]);
		int colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
		SDL_SetColorKey(healthBar, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
		SDL_BlitSurface(healthBar, NULL, screen, &hbrect);

		SDL_Flip(screen);

		if (duck->returnXCoord() == RIGHT_TRIGGER){
			currScene = currScene->getRight();
			duck->place(FROM_LEFT_X,DEFAULT_DUCK_Y);
		} else if (duck->returnXCoord() == LEFT_TRIGGER){
			currScene = currScene->getLeft();
			duck->place(FROM_RIGHT_X,DEFAULT_DUCK_Y);
		}
	}
	Mix_HaltMusic();
}
