//
//  SGNormalAttackList.h
//  GameSanGuo
//  三格攻击阵列
//  Created by gengshengcai on 12-12-4.
//
//
//git test
#ifndef __GameSanGuo__SGNormalAttackList__
#define __GameSanGuo__SGNormalAttackList__
#include "SGAttackList.h"

class SGNormalAttackList : public SGAttackList
{
private:
    GameIndex getFirstIndex();
    
    ShiBing *getLastSB();
    void addShowMsg();
public:
    
    SGNormalAttackList();
    ~SGNormalAttackList();
    
    static SGNormalAttackList* createWithSbs(CCArray *sbs, bool ismy);
    bool initWithSBs(CCArray *sbs, bool ismy);
    virtual void changeAP(int ap, bool isAction = false, bool isPlayEff = false, bool isDrawXuetiao = true);
    virtual void drawEffectSp(int effectId);//绘制特效
    ShiBing *getFirstSB();
    void suicideAttack();
    
public:
    GameIndex getLastIndex();
    
public:
    CCSpriterX *effect;



};

#endif /* defined(__GameSanGuo__SGNormalAttackList__) */
