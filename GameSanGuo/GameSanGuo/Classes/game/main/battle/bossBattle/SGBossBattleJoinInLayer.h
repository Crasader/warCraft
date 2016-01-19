//
//  SGBossBattleJoinInLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-26.
//
//

#ifndef __GameSanGuo__SGBossBattleJoinInLayer__
#define __GameSanGuo__SGBossBattleJoinInLayer__

#include <iostream>

#include "SGBaseLayer.h"

struct BossBattleJoinData {
    int  canJoin;
    int bossLev;
    int startMin;
    CCString * description;
    CCString *formName;
};

class SGBossBattleJoinInLayer:public SGBaseLayer {
private:
	virtual void initMsg();
	void showBattleDetailLayer();
	void showLastBattleSituation();
	void enterBossFightListener(CCObject *sender);
	
    void showgrapPreciousLayer();
    int  canJoinbattle;
    
    SGCCLabelTTF *leftbattletimelabel;
    SGButton *joinBattle;
    SGCCLabelTTF *battletimelabel;
    SGCCLabelTTF *formerlabel;
    SGCCLabelTTF *despritionlabel;
    SGButton *lastbattle;
    virtual void update(float dt=1);
    void changeState(int canJoin);
public:
    SGBossBattleJoinInLayer();
    ~SGBossBattleJoinInLayer();
    
    static SGBossBattleJoinInLayer *create(BossBattleJoinData *data);
	
    CC_SYNTHESIZE(int, bossLevel, bossLevel);
    CC_SYNTHESIZE(int , startMin, startMin);
    CC_SYNTHESIZE_RETAIN(CCString *, despriteion, description);
    CC_SYNTHESIZE_RETAIN(CCString *, formName, formName);
    
    
    
};




#endif /* defined(__GameSanGuo__SGBossBattleJoinInLayer__) */
