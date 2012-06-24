#include "scenes.h"
#include "game.h"
#include <iostream>
using namespace std;

void BenchScene::action(){
	int duckCoord = getGame()->getDuck()->returnXCoord();
	if ((duckCoord >= GRANDMA_THROWSEEDS_LOWERB) && (duckCoord <= GRANDMA_THROWSEEDS_UPPERB)){
		if (getGame()->getDuck()->childrenIsFollowing() && (!unfollowed)){
			unfollowed = true;
			for (int i=0; i < getGame()->getDuck()->numberChildren(); i++){
				Duckling *curr = getGame()->getDuck()->getChild(i);
				curr->goGrandma();
				curr->unfollow();
			}
			getGame()->getDuck()->childrenStopFollow();
			childrenIsFeeding = true;
			
		}
		grandma->throwSeeds();
		getGame()->getDuck()->eat();
	} else if (childrenIsFeeding){
		grandma->throwSeeds();
	} else {
		grandma->stopThrow();
	}
	SDL_BlitSurface(background, NULL, getGame()->getScreen(), &bgrect);
	bench->refresh();
	grandma->refresh();
	if (getGame()->getDuck()->childrenIsFollowing() || childrenIsFeeding){
		int total = getGame()->getDuck()->numberChildren();
		for (int i=0; i < total; i++){
			Duckling *curr = getGame()->getDuck()->getChild(i);
			curr->automove();
			curr->refresh();
		}
	}
}

BenchScene::BenchScene(Game *currGame) : Scene(currGame){
	SDL_Surface *temp = SDL_LoadBMP("images/grandmacground.bmp");
	background = SDL_DisplayFormat(temp);
	setSprite(background);
	bench = new Bench(currGame);
	grandma = new Grandma(currGame);
	SDL_FreeSurface(temp);
	bgrect.w = background->w;
	bgrect.h = background->h;
	bgrect.x = 0;
	bgrect.y = 0;
	unfollowed = false;
	childrenIsFeeding = false;
	isDoomsdayInitiated = false;
}

WaterScene::WaterScene(Game *currGame) : Scene(currGame){
	SDL_Surface *temp = SDL_LoadBMP("images/waterbackground.bmp");
	background = SDL_DisplayFormat(temp);
	setSprite(background);
	SDL_FreeSurface(temp);
	bgrect.w = background->w;
	bgrect.h = background->h;
	bgrect.x = 0;
	bgrect.y = 0;
	femaleDuck = new Duck(currGame);
	femaleDuck->place(FEMALE_DUCK_X,FEMALE_DUCK_Y);
	femaleDuck->refresh();
	femaleDuck->startSwim();
	currGame->getDuck()->place(FROM_RIGHT_X,DEFAULT_DUCK_Y);
	isDuckSwimming = false;
	isSexing = false;
	eggHatching = false;
	turned = false;
	hasHadKids = false;
	childrenActive = false;
	timesCalled = 0;
	childShouldCome = false;
	childrenMoving = false;
	isDoomsdayBegun = false;
}

void WaterScene::action(){
	SDL_BlitSurface(background, NULL, getGame()->getScreen(), &bgrect);
	femaleDuck->refresh();
	bool shouldStopSexing = false;
	int coord = getGame()->getDuck()->returnXCoord();
	if ((coord <= POND_BOUNDARY) && (!isDuckSwimming)){
		isDuckSwimming = true;
		getGame()->getDuck()->startSwim();
		if (childrenActive){
			for (int i=0; i < getGame()->getDuck()->numberChildren(); i++){
				Duckling *currDuckling = getGame()->getDuck()->getChild(i);
				if (currDuckling->isFollowing()){
					currDuckling->startSwim();
				}
			}
		}
	}
	if ((coord > POND_BOUNDARY) && (isDuckSwimming)){
		isDuckSwimming = false;
		getGame()->getDuck()->stopSwim();
		if (childrenActive){
			for (int i=0; i < getGame()->getDuck()->numberChildren(); i++){
				Duckling *currDuckling = getGame()->getDuck()->getChild(i);
				if (currDuckling->isFollowing()){
					currDuckling->stopSwim();
				}
			}
		}
	}

	if ((coord <= UPPER_SEX_BOUND) && (coord >= LOWER_SEX_BOUND) && !isSexing && (getGame()->getDuck()->getHealth() == MAX_HEALTH)){
		isSexing = true;
		getGame()->getDuck()->startSex();
		femaleDuck->invisible();
		getGame()->getDuck()->freeze();
		shouldStopSexing = false;


		if (!isDoomsdayBegun && childrenActive && getGame()->getDuck()->getChild(0)->isByGrandma()){
			isDoomsdayBegun = true;
			for (int i=0; i < getGame()->getDuck()->numberChildren(); i++){
				getGame()->getDuck()->getChild(i)->doomsday(); //call doomsday upon the hatchlings
			}
			getRight()->doomsday(); //call doomsday upon the bench scene
		}
	}
	if (isSexing){
		getGame()->getDuck()->engageSex();
		if ((sexCtr % 78) == 0){
			getGame()->getDuck()->decreaseHealth();
		}
		if (getGame()->getDuck()->getHealth() == MIN_HEALTH){
			shouldStopSexing = true;	
		}
		sexCtr++;
	}
	if ((isSexing && ((coord > UPPER_SEX_BOUND) || (coord < LOWER_SEX_BOUND))) || shouldStopSexing){
		isSexing = false;
		getGame()->getDuck()->stopSex();
		femaleDuck->invisible();
		getGame()->getDuck()->freeze(); //unfreeze
		femaleDuck->startSwim();
		getGame()->getDuck()->startSwim();
		if (!hasHadKids){
			hasHadKids = true;
			eggHatching = true;
		}
	}
	if (eggHatching){
		if (!turned){
			femaleDuck->move(RIGHT);
			if (femaleDuck->returnXCoord() >= POND_BOUNDARY){
				femaleDuck->stopSwim();
			}
			if (femaleDuck->returnXCoord() >= POND_BOUNDARY+35){
				turned = true;
			}
		} else {
			femaleDuck->move(UP);
			if (femaleDuck->returnYCoord() < 174){
				femaleDuck->invisible();
				eggHatching = false;
				getGame()->getDuck()->createChildren(5);
			}
		}
	}
	if (childrenActive){
		if (childShouldCome){
		 	childShouldCome = false;
			if (timesCalled == 1){
				//all ducklings stop and turn
				for (int i=0; i < getGame()->getDuck()->numberChildren();i++){
					getGame()->getDuck()->getChild(i)->stop();
				}
			} else {
				getGame()->getDuck()->getChild(timesCalled - 2)->follow(timesCalled*25);
				if ((timesCalled - 1) == getGame()->getDuck()->numberChildren()){
					getGame()->getDuck()->childrenWillFollow();
				}
			}
		}


		//blitting
		for (int i=0; i < getGame()->getDuck()->numberChildren(); i++){
			Duckling *curr = getGame()->getDuck()->getChild(i);
			if (!curr->isByGrandma()){
				curr->automove();
				curr->refresh();
			} else {
				break;
			}
		}
	}

}

void WaterScene::quacked(){
	if (childrenActive && (timesCalled <= (getGame()->getDuck()->numberChildren()+1))){
		childShouldCome = true;	
		timesCalled++;	
	}

}

void WaterScene::sceneChange(){
	if (hasHadKids){
		femaleDuck->startSwim();
		femaleDuck->place(FEMALE_DUCK_X, FEMALE_DUCK_Y);
		childrenActive = true;
	}
}

void BenchScene::sceneChange(){
}


void BenchScene::doomsday(){
	SDL_Surface *doomsdayBg = SDL_LoadBMP("images/grandmacground_fire.bmp");
	SDL_FreeSurface(background);
	background = SDL_DisplayFormat(doomsdayBg);
	SDL_FreeSurface(doomsdayBg);
	bench->doomsday();
}
