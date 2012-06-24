#include "sprites.h"
#include "game.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include "scenes.h"
#include <vector>
#include <cstdlib>
using namespace std;

Sprite::Sprite(Game *currGame) : currGame(currGame){
	sprite = NULL;
}
void Sprite::refresh(){
	if (sprite != NULL){
		int colorkey = SDL_MapRGB(currGame->getScreen()->format, 255, 0, 255);
		SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
		SDL_BlitSurface(sprite, NULL, currGame->getScreen(), &rect);
	}
}
void Sprite::setSprite(SDL_Surface *surf){
	if (sprite != NULL){
		SDL_FreeSurface(sprite);
		sprite = NULL;
	}
	sprite = SDL_DisplayFormat(surf);
	rect.w = sprite->w;
	rect.h = sprite->h;
}

void Sprite::place(int x, int y){
	rect.x = x;
	rect.y = y;
}

Sprite::~Sprite(){
	SDL_FreeSurface(sprite);	
}
int Sprite::returnXCoord(){
	return rect.x;
}

Duck::~Duck(){
	SDL_FreeSurface(movingSpriteLeft);
	SDL_FreeSurface(movingSpriteRight);
	SDL_FreeSurface(quackSpriteLeft); //quack picture 
	SDL_FreeSurface(quackSpriteRight);
	SDL_FreeSurface(normalSpriteLeft); //normal picture
	SDL_FreeSurface(normalSpriteRight);
	
}

Grandma::Grandma(Game *currGame) : Sprite(currGame){
	normalMa = SDL_LoadBMP("images/grandma.bmp");
	throwMa = SDL_LoadBMP("images/grandma_lifting.bmp");
	midWayThrowMa = SDL_LoadBMP("images/grandma_lifting_inprogress.bmp");
	setSprite(normalMa);
	place(GRANDMACOORD_X,GRANDMACOORD_Y);
	throwTimer = 1;
}

void Grandma::throwSeeds(){
	//if ((throwTimer % 100) == 0){
	//	setSprite(normalMa);
	if ((throwTimer % 50) == 0){
		setSprite(throwMa);	
	} else if ((throwTimer % 25) == 0){
		setSprite(midWayThrowMa);
	}
	throwTimer++;
}

void Duck::eat(){
	if (((feedTimer % 50) == 0) && health < MAX_HEALTH){
		quack();
	} else if (((feedTimer % 25) == 0) && health < MAX_HEALTH){
		stopQuack();
	}
	if (((feedTimer % 100) == 0) && health < MAX_HEALTH){
		health++;
	}
	feedTimer++;
}

void Grandma::stopThrow(){
	setSprite(normalMa);
	throwTimer = 1;
}

Bench::Bench(Game *currGame) : Sprite(currGame){
	bench = SDL_LoadBMP("images/bench.bmp");
	setSprite(bench);
	place(BENCHCOORD_X,BENCHCOORD_Y);
	
}

void Bench::doomsday(){
	SDL_Surface *temp = SDL_LoadBMP("images/bench_fire.bmp");
	setSprite(temp);
	SDL_FreeSurface(temp);
}

void Sprite::move(int direction){
	if (direction == RIGHT){
		rect.x = rect.x + STEP;
	} else if (direction == LEFT){
		rect.x = rect.x - STEP;
	} else if (direction == UP){
		rect.y = rect.y - STEP;
	} else if (direction == DOWN){
		rect.y = rect.y + STEP;
	}
}

void Duck::setLeftSprite(SpriteType t){
	switch (t){
		case SWIM_SPRITE:
			setSprite(swimSpriteLeft);
		break;
		case NORM_SPRITE:
			setSprite(normalSpriteLeft);
		break;
		case MOVE_SPRITE:
			setSprite(movingSpriteLeft);
		break;
		case QUACK_SWIM_SPRITE:
			setSprite(quackSwimLeft);
			break;
		case QUACK_SPRITE:
			setSprite(quackSpriteLeft);
			break;
	}
}
void Duck::setRightSprite(SpriteType t){
	switch (t){
		case SWIM_SPRITE:
			setSprite(swimSpriteRight);
		break;
		case NORM_SPRITE:
			setSprite(normalSpriteRight);
		break;
		case MOVE_SPRITE:
			setSprite(movingSpriteRight);
			break;
		case QUACK_SWIM_SPRITE:
			setSprite(quackSwimRight);
			break;
		case QUACK_SPRITE:
			setSprite(quackSpriteRight);
		break;
	}
}
void Duck::setLeftSprite(){
	if (isSwimming){
		setLeftSprite(SWIM_SPRITE);
	} else if (legUp){
		setLeftSprite(MOVE_SPRITE);
	} else {
		setLeftSprite(NORM_SPRITE);
	}
}

void Duck::setRightSprite(){
	if (isSwimming){
		setRightSprite(SWIM_SPRITE);
	} else if (legUp){
		setRightSprite(MOVE_SPRITE);
	} else {
		setRightSprite(NORM_SPRITE);
	}
}


void Duck::move(int direction){
	if (!isInvisible){
		if (direction == LEFT){
			setLeftSprite();
		} else {
			setRightSprite();
		}
		currDirection = direction;
		Sprite::move(direction);
		legUp = !legUp;
	}

}

void Duck::startSwim(){
	setSprite(swimSpriteLeft);
	isSwimming = true;
}

void Duck::stopSwim(){

	isSwimming = false;
}

void Duck::quack(){
	if (!isSex){
		if (currDirection == LEFT){
			if (isSwimming){
				setLeftSprite(QUACK_SWIM_SPRITE);
			} else {
				setLeftSprite(QUACK_SPRITE);
			}
		} else {
			if (isSwimming){
				setRightSprite(QUACK_SWIM_SPRITE);
			} else {
				setRightSprite(QUACK_SPRITE);
			}
		}
	}
	int channel = Mix_PlayChannel(-1, quackSound, 0);
	if(channel == -1) {
		printf("Unable to play WAV file: %s\n", Mix_GetError());
	}
	if (hasChildrenFollowing){
		for (int i=0; i < numberChildren(); i++){
			getChild(i)->shouldQuackSometime();
		}
	}
}


Duck::Duck(Game *currGame) : Sprite(currGame){
	currDirection = LEFT;
	legUp = true;
	movingSpriteLeft = SDL_LoadBMP("images/duck_moving_left.bmp");
	quackSpriteLeft = SDL_LoadBMP("images/duck_open_left.bmp");
	normalSpriteLeft = SDL_LoadBMP("images/duck_left.bmp");
	movingSpriteRight = SDL_LoadBMP("images/duck_moving_right.bmp");
	quackSpriteRight = SDL_LoadBMP("images/duck_open_right.bmp");
	normalSpriteRight = SDL_LoadBMP("images/duck_right.bmp");


	swimSpriteRight = SDL_LoadBMP("images/duck_swimming_right.bmp");
	swimSpriteLeft = SDL_LoadBMP("images/duck_swimming_left.bmp");
	quackSwimRight = SDL_LoadBMP("images/duck_swimming_right_quack.bmp");
	quackSwimLeft = SDL_LoadBMP("images/duck_swimming_left_quack.bmp");

	sexSprite1 = SDL_LoadBMP("images/ducksex.bmp");
	sexSprite2 = SDL_LoadBMP("images/ducksex2.bmp");

	invisibleSprite = SDL_LoadBMP("images/invisible.bmp");

	setSprite(normalSpriteLeft);
	place(FROM_LEFT_X,DEFAULT_DUCK_Y);
	feedTimer = 4;
	quackSound = Mix_LoadWAV("music/quack.wav");
	health = 0;
	isSwimming = false;
	isInvisible = false;
	isSex = false;
	sexCounter = 0;
	hasChildrenFollowing = false;
}

void Duck::startSex(){
	isSex = true;
	setSprite(sexSprite1);
}

void Duck::stopSex(){
	isSex = false;
	setSprite(normalSpriteLeft);
	
}

void Duck::engageSex(){
	if (isSex){
		if ((sexCounter % 100) == 0){
			setSprite(sexSprite1);
			quack();		
		} else if ((sexCounter % 50) == 0){
			setSprite(sexSprite2);
			quack();
		} else if ((sexCounter % 25) == 0){
			quack();
		}
		sexCounter++;
	}
}

void Duck::invisible(){
	if (!isInvisible){
		isInvisible = true;
		setSprite(invisibleSprite);
	} else {
		isInvisible = false;
		setSprite(normalSpriteLeft);
	}
}

void Duck::stopQuack(){
	if (currDirection == LEFT){
		if (isSwimming){
			setSprite(swimSpriteLeft);
		} else {
			setSprite(normalSpriteLeft);
		}
	} else {
		if (isSwimming){
			setSprite(swimSpriteRight);
		} else {
			setSprite(normalSpriteRight);
		}
	}
}

Duckling* Duck::getChild(int i){
	if (i < children.size()){
		return children[i];
	} else {
		return children[children.size()-1];
	}
}

void Duckling::automove(){
	if (!stopped){
		int p;
		//with probability 0.9: stand still
		//with probability 0.3: go left or right
		//throw a random number:
		double r = rand()*INVRANDMAX;
		if (r <= 0.95){
			p = 3; //stand still
		} else {
			p = rand() % 2;
		}
	
	
		switch (p){
			case 0:
				move(LEFT);
				break;
			case 1:
				move(RIGHT);
				break;
		}
		//one of the cases will result in doing nothing
	} else if (shouldFollow){
		int distance = getGame()->getDuck()->returnXCoord() - returnXCoord(); //distance between father and child
		if ((distance < 0) && (distance <= followDistance*(-1))){
			move(LEFT);
		} else if ((distance > 0) && (distance >= followDistance)){
			move(RIGHT);
		}
	} else if (shouldGoToGrandma){
		int distance = GRANDMACOORD_X - returnXCoord();
		if (distance < 0){
			move(LEFT);
		} else if (distance > 0){
			move(RIGHT);
		} else if (distance == 0){
			shouldGoToGrandma = false;
			stopped = false;
			byGrandma = true;
		}
	}
	if (shouldQuack){
		double r = rand()*INVRANDMAX;
		if (r >= 0.98){
			quack();
			shouldQuack = false;
			quackCtr = 0;
			isQuacking = true;
		}
	} else if (isQuacking){
		quackCtr++;
		if (quackCtr >= STOP_QUACKING_CTR){
			stopQuack();
		}
	}
}

void Duckling::doomsday(){
	SDL_Surface *tleft = SDL_LoadBMP("images/duckling_left_mad.bmp");
	SDL_Surface *tright = SDL_LoadBMP("images/duckling_right_mad.bmp");
	setDefaultSprite(LEFT,MOVE_SPRITE,tleft);
	setDefaultSprite(RIGHT,MOVE_SPRITE,tright);
	setDefaultSprite(LEFT, QUACK_SPRITE,tleft);
	setDefaultSprite(RIGHT, QUACK_SPRITE, tright);
	setDefaultSprite(LEFT,NORM_SPRITE,tleft);
	setDefaultSprite(RIGHT, NORM_SPRITE,tright);
	setDefaultSprite(RIGHT, SWIM_SPRITE,tright);
	setDefaultSprite(LEFT, SWIM_SPRITE, tleft);
	setDefaultSprite(RIGHT, QUACK_SWIM_SPRITE,tright);
	setDefaultSprite(LEFT, QUACK_SWIM_SPRITE, tleft);
	int x = rand()%SCREEN_WIDTH;
	if (x > 200){
		x -= 200;
	}
	int y = rand()%SCREEN_WIDTH;
	if (y > 200){
		y -= 200;
	}
	place(x,y);
}

void Duckling::follow(int dist){
	shouldFollow = true;
	followDistance = dist;
}


void Duckling::unfollow(){
	shouldFollow = false;
}

Duckling::Duckling(Game *currGame) : Duck(currGame){
	setDefaultSprite(LEFT,MOVE_SPRITE,SDL_LoadBMP("images/duckling_left_walk.bmp"));
	setDefaultSprite(RIGHT,MOVE_SPRITE,SDL_LoadBMP("images/duckling_right_walk.bmp"));
	setDefaultSprite(LEFT, QUACK_SPRITE, SDL_LoadBMP("images/duckling_left_quack.bmp"));
	setDefaultSprite(RIGHT, QUACK_SPRITE, SDL_LoadBMP("images/duckling_right_quack.bmp"));
	setDefaultSprite(LEFT,NORM_SPRITE,SDL_LoadBMP("images/duckling_left.bmp"));
	setDefaultSprite(RIGHT, NORM_SPRITE, SDL_LoadBMP("images/duckling_right.bmp"));
	setDefaultSprite(RIGHT, SWIM_SPRITE, SDL_LoadBMP("images/duckling_right_swim.bmp"));
	setDefaultSprite(LEFT, SWIM_SPRITE,  SDL_LoadBMP("images/duckling_left_swim.bmp"));
	setDefaultSprite(RIGHT, QUACK_SWIM_SPRITE, SDL_LoadBMP("images/duckling_right_swim_quack.bmp"));
	setDefaultSprite(LEFT, QUACK_SWIM_SPRITE, SDL_LoadBMP("images/duckling_left_swim_quack.bmp"));

	startSwim();
	stopped = false;
	shouldFollow = false;
	shouldQuack = false;
	quackCtr = 0;
	isQuacking = false;
	shouldGoToGrandma = false;
	byGrandma = false;
}
void Duck::createChildren(int number){
	for (int i=0; i < number; i++){
		children.push_back(new Duckling(getGame()));
		children[i]->place(FEMALE_DUCK_X + 50 + i*2,DEFAULT_DUCK_Y + 20 + i);
	}
}
	
void Duck::setDefaultSprite(Direction dir, SpriteType t, SDL_Surface *newSprite){
	bool isLeft = false;
	if (dir == LEFT){
		isLeft = true;
	}
	switch (t){
		case SWIM_SPRITE:
			if (isLeft){
				SDL_FreeSurface(swimSpriteLeft);
				swimSpriteLeft = newSprite;
			} else {
				SDL_FreeSurface(swimSpriteRight);
				swimSpriteRight = newSprite;

			}
			break;
		case NORM_SPRITE:
			if (isLeft){
				SDL_FreeSurface(normalSpriteLeft);
				normalSpriteLeft = newSprite;
				
			} else {
				SDL_FreeSurface(normalSpriteRight);
				normalSpriteRight = newSprite;

			}
			break;
		case MOVE_SPRITE:
			if (isLeft){
				SDL_FreeSurface(movingSpriteLeft);
				movingSpriteLeft = newSprite;
				
			} else {
				SDL_FreeSurface(movingSpriteRight);
				movingSpriteRight = newSprite;

			}
			break;
		case QUACK_SWIM_SPRITE:
			if (isLeft){
				SDL_FreeSurface(quackSwimLeft);
				quackSwimLeft = newSprite;
				
			} else {
				SDL_FreeSurface(quackSwimRight);
				quackSwimRight = newSprite;

			}
			break;
		case QUACK_SPRITE:
			if (isLeft){
				SDL_FreeSurface(quackSpriteLeft);
				quackSpriteLeft = newSprite;
				
			} else {
				SDL_FreeSurface(quackSpriteRight);
				quackSpriteRight = newSprite;

			}
			setSprite(quackSpriteRight);
			break;
	}
}


void Duckling::stop(){
	stopped = true;
	setRightSprite(SWIM_SPRITE);
}
