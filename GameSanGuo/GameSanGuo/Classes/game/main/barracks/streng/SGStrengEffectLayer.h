//
//  SGStrengEffectLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-5-6.
//
//

#ifndef __GameSanGuo__SGStrengEffectLayer__
#define __GameSanGuo__SGStrengEffectLayer__

#include <iostream>
#include "SGBaseBox.h"
#include "cocos2d.h"
#include "CCSpriterX.h"
class SGStrengEffectLayer : public SGBaseBox
{
public:
	SGStrengEffectLayer();
	~SGStrengEffectLayer();
	void refreshLayer();
	static SGStrengEffectLayer *create(int enterType, int succLvl, SGBaseMilitaryCard *card);
	void initView();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
	// 来自于哪个页面
	int enterType;
	//强化成功的各类,(成功,大成功,超成功等)
	int successLvl;
	//强化哪一个武将
	SGBaseMilitaryCard *card;
    CCSpriterX *lvlupAct;
	
};



#endif /* defined(__GameSanGuo__SGStrengEffectLayer__) */
