#ifndef SPRITES_H_DEFINED
#define SPRITES_H_DEFINED

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>


class Game;

//for checking directions in move etcetera

enum Direction{LEFT,RIGHT, UP, DOWN};


const int DEFAULT_DUCK_Y = 310;

const int MAX_HEALTH = 5;
const int MIN_HEALTH = 0;


const int GRANDMACOORD_X = 248;
const int GRANDMACOORD_Y = 188;

const int BENCHCOORD_X = 300;
const int BENCHCOORD_Y = 250;


enum SpriteType{NORM_SPRITE, MOVE_SPRITE, SWIM_SPRITE, QUACK_SWIM_SPRITE, QUACK_SPRITE};

class Sprite{
	private:
		SDL_Rect rect; //bounding rectangle
		SDL_Surface *sprite; //current sprite
		Game *currGame; //pointer to main game object
	public:
		Sprite(Game *currGame);
		void refresh(); //update the sprite
		virtual void move(int direction); //move the coordinates of the sprite, refresh



		void place(int x, int y); //place sprite at specified coordinates
		void setSprite(SDL_Surface *surf); //change the sprite to another image
		int returnXCoord();
		int returnYCoord(){return rect.y;};
		~Sprite();
		Game* getGame(){return currGame;};
};

class Bench : public Sprite{
	private:
		SDL_Surface *bench;
	public:
		Bench(Game *currGame);
		void doomsday();
};


class Duckling;

class Duck : public Sprite{
	private:
		SDL_Surface *movingSpriteLeft; //move picture
		SDL_Surface *movingSpriteRight;
		SDL_Surface *quackSpriteLeft; //quack picture 
		SDL_Surface *quackSpriteRight;
		SDL_Surface *normalSpriteLeft; //normal picture
		SDL_Surface *normalSpriteRight;

		SDL_Surface *swimSpriteRight;
		SDL_Surface *swimSpriteLeft;
		SDL_Surface *quackSwimRight;
		SDL_Surface *quackSwimLeft;

		SDL_Surface *invisibleSprite;

		SDL_Surface *sexSprite1;
		SDL_Surface *sexSprite2;

		int currDirection;
		bool legUp;
		int feedTimer;
		Mix_Chunk *quackSound;
		int health;

		bool isSwimming;

		bool isInvisible;
		bool isSex;

		int sexCounter;

		std::vector<Duckling*> children;

		bool hasChildrenFollowing;


	public:
		Duck(Game *currGame);
		void quack();
		void stopQuack();
		void eat();
		Duckling* getChild(int i);
		void invisible();
		~Duck();
		int getHealth(){return health;};
		virtual void move(int direction); //also change sprite image in addition to calling sprite::move 
		void startSwim(); 
		void stopSwim(); //revert swim state

		void startSex();
		void stopSex();
		void engageSex();
		void freeze(){isInvisible = !isInvisible;};
		void decreaseHealth(){health--;};

		bool childrenIsFollowing(){return hasChildrenFollowing;};
		void childrenWillFollow(){hasChildrenFollowing = true;}
		void childrenStopFollow(){hasChildrenFollowing = false;};

		virtual void setLeftSprite();
		virtual void setRightSprite();

		void setDefaultSprite(Direction dir, SpriteType t, SDL_Surface *newSprite);

		virtual void setLeftSprite(SpriteType t);
		virtual void setRightSprite(SpriteType t);

		void createChildren(int number);
		int numberChildren(){return children.size();};
};


class Duckling : public Duck{
	private:
		bool stopped; //whether it should stay still
		bool shouldFollow; //should follow DADDAY
		int followDistance;
		bool shouldQuack;
		int quackCtr;
		bool isQuacking;
		bool shouldGoToGrandma;
		bool byGrandma;

	public:
		Duckling(Game *currGame);
		void automove(); //move by itself
		void stop(); //stop and turn to the left
		void follow(int dist); //follow DADDAYYY with distance dist
		void unfollow(); //unfollow DADDDDDYYYYYY
		bool isFollowing(){return shouldFollow;};
		void goGrandma(){shouldGoToGrandma = true;}; //go to the grandma
		void shouldQuackSometime(){shouldQuack = true;}; //will set the shouldQuack flag so that the duckling will quack sometime in the future
		bool isByGrandma(){return byGrandma;};
		void doomsday();

};

class Grandma : public Sprite{
	private:
		SDL_Surface *normalMa;
		SDL_Surface *throwMa;
		SDL_Surface *midWayThrowMa;
		int throwTimer;
	public:
		Grandma(Game *currGame);
		void throwSeeds(); //start/continue throw animation
		void stopThrow(); //stop throw animation
};

#endif
