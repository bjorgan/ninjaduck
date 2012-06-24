#ifndef SCENES_H_DEFINED
#define SCENES_H_DEFINED

#include "sprites.h"
#include <cstdlib>

const double INVRANDMAX = 1.0/(RAND_MAX*1.0);

const int GRANDMA_THROWSEEDS_LOWERB = 220;
const int GRANDMA_THROWSEEDS_UPPERB = 320;


//coordinates for female duck in pond
const int FEMALE_DUCK_X = 200; 
const int FEMALE_DUCK_Y = 305;

const int FROM_RIGHT_X = 600; //default coordinate when entering from the right 
const int FROM_LEFT_X = 10;


//scene change trigger coordinates
const int LEFT_TRIGGER = 0;
const int RIGHT_TRIGGER = 640;


//swimming triggers
const int POND_BOUNDARY = 395;

//sex triggers

const int LOWER_SEX_BOUND = 190;
const int UPPER_SEX_BOUND = 210;




class Game;

class Scene : public Sprite{
	private:
		Scene *right;
		Scene *left;
	public:
		Scene(Game *currGame) : Sprite(currGame){};
		virtual void action() = 0;
		Scene* getRight(){sceneChange(); return right;}; //return the scene right of current scene
		Scene* getLeft(){sceneChange(); return left;};
		void setRight(Scene* r){right = r;};
		void setLeft(Scene* l){left = l;};
		virtual void sceneChange(){};
		virtual void quacked(){}; //when the duck has quacked and something context-sensitive should happen within the scene
		virtual void doomsday(){};
};


class BenchScene : public Scene{
	private:
		Bench *bench;
		Grandma *grandma;
		SDL_Rect bgrect;
		SDL_Surface *background;
		bool unfollowed;
		bool childrenIsFeeding;
		bool isDoomsdayInitiated;
	public:
		virtual void action();
		BenchScene(Game *currGame);
		virtual void sceneChange();
		virtual void doomsday();

};

class WaterScene : public Scene{
	private:
		Duck *femaleDuck;
		SDL_Rect bgrect;
		SDL_Surface *background;
		bool isDuckSwimming;
		bool isSexing;
		int sexCtr;

		bool hasHadKids;
		bool eggHatching;

		bool turned;
		bool childrenActive;


		int timesCalled; //how many times the duck has called for its children
		bool childShouldCome; //a new child should come for the duck
		bool childrenMoving;

		bool isDoomsdayBegun;
	public:
		WaterScene(Game *currGame);
		virtual void action();
		virtual void sceneChange();
		virtual void quacked(); //external duck quacked, children should come
};

#endif
