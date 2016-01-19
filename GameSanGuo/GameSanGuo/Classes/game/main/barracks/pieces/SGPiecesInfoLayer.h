//
//  SGPiecesInfoLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-2-26.
//
//

#ifndef __GameSanGuo__SGPiecesInfoLayer__
#define __GameSanGuo__SGPiecesInfoLayer__

#include <iostream>
#include "SGBaseStuffLayer.h"
#include "SGPiecesCard.h"

class SGPiecesInfoLayer : public SGBaseStuffLayer
{
public:
	SGPiecesInfoLayer();
	~SGPiecesInfoLayer();
	
	static SGPiecesInfoLayer *create(SGPiecesCard *card, int enterType = 0);
	
private:
    void initView();
    void backHandler();
    void showHandler();

private:
    int enterType;
    SGPiecesCard* _card;

};



#endif /* defined(__GameSanGuo__SGPiecesInfoLayer__) */
