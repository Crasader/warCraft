//
//  SGGameSPAttack.h
//  GameSanGuo
//
//  Created by wenlong w on 12-11-28.
//  4格英雄攻击阵列
//

#ifndef __GameSanGuo__SGGameSPAttack__
#define __GameSanGuo__SGGameSPAttack__

#include "SGAttackList.h"
class SGFourAttackList : public SGAttackList
{
private:
    //四个武将第二个箭
    CCSkeletonAnimation  *theNextGridSward;
    
public:
    
    SGFourAttackList();
    ~SGFourAttackList();
    
    static SGFourAttackList* createWithSoldier(ShiBing *sb, bool ismy);
    virtual void startAttack(SGHeroLayer *myHero_, SGHeroLayer *opponentHero);
    
    void showFormEffect();
    
    int  attackGrid(SGBattleMap* map, SGGridBase * grid,int damage);
    void vanishAttackList(CCObject *obj);
    void removesb(cocos2d::CCObject *obj, GameSbIndex *sbindex);
    
    //特殊被动技
    void specialHeroSkillOfPangTong();
    void specialHeroSkillOfPangTong_2();
    void specialHeroSkillOfPangTong_3();

    
};

#endif /* defined(__GameSanGuo__SGGameSPAttack__) */
