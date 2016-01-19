//
//  SGConsumableInfoLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-18.
//
//

#ifndef __GameSanGuo__SGConsumableInfoLayer__
#define __GameSanGuo__SGConsumableInfoLayer__

#include <iostream>
#include "SGBaseStuffLayer.h"
#include "SGConsumableCard.h"

class SGConsumableInfoLayer : public SGBaseStuffLayer
{
public:
	SGConsumableInfoLayer();
	~SGConsumableInfoLayer();
	
	static SGConsumableInfoLayer *create(SGConsumableCard *card, int enterType = 0, int currentPage = 1000);
	
private:
    void initView();
    void backHandler();
	void setCardType(int type);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
	
private:
    int enterType;
    SGConsumableCard *_card;
    float btnwidth;
    float btnwid;
    int _currentPage;
};



#endif /* defined(__GameSanGuo__SGConsumableInfoLayer__) */
