//
//  SGHeroSkillsBase.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkillsBase__
#define __GameSanGuo__SGHeroSkillsBase__

#include <iostream>
#include "cocos2d.h"
#include "SGPlayerInfo.h"
#include "SGHeroLayer.h"
#include "SGAttackList.h"
#include "SGNormalAttackList.h"
#include "SGTwoAttackList.h"
#include "SGAIManager.h"

using namespace cocos2d;


//武将技能的基类
class SGHeroSkillsBase : public CCNode{
    
    
public:
    SGHeroSkillsBase();
    ~SGHeroSkillsBase();
    
    virtual void activateSkill_20000(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid);
    virtual float activateSkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    virtual void activateSkill_20002(SGHeroLayer *heroLayer);
    virtual void activateSkill_20003(SGHeroLayer *heroLayer, int value1, int value2, int roleid);
    virtual void activateSkill_20004(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float valule2,int heroId);
    virtual void activateSkill_20005(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, CCPoint point, int roleid, int skillid);
    virtual void activateSkill_20006(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, CCPoint point, int skillId);
    virtual void activateSkill_20007(SGHeroLayer *heroLayer, float value1, int value2, int buffid);
    virtual void activateSkill_20008(SGHeroLayer *heroLayer, float rate);
    virtual void activateSkill_20010(SGHeroLayer *heroLayer,  float rate);
    virtual void activateSkill_20012(SGHeroLayer *heroLayer,SGHeroLayer *heroLayer1, float value1, int s_id, CCPoint pos);
    virtual void activateSkill_20014(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid);
    virtual void activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num);
    virtual void activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num, bool isend);
    virtual void activateSkill_20020(SGHeroLayer *heroLayer, int addValue);
    virtual float activateSkill_20022(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid);
    virtual void activateSkill_20024(SGHeroLayer *heroLayer, int value);
    virtual void activateSkill_20028(SGHeroLayer *heroLayer,  float value1, float value2);
    virtual void activateSkill_20032(SGHeroLayer *heroLayer, float value);
    virtual void activateSkill_20030(SGHeroLayer *heroLayer, int skillsId);
    virtual void activateSkill_20033(SGHeroLayer *heroLayer, float value);
    virtual void activateSkill_20034(SGHeroLayer *heroLayer, int round, int buffid);
    virtual void activateSkill_20038(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int jianid);
    virtual void activateSkill_20046(SGHeroLayer *heroLayer, float value);
    virtual void activateSkill_20048(SGHeroLayer *heroLayer, float value);
    virtual void activateSkill_20052(SGHeroLayer *heroLayer, float value1,int buffId);
    virtual void activateSkill_20058(SGHeroLayer *heroLayer, float value1);
    virtual void activateSkill_20060(SGHeroLayer *heroLayer);
    virtual void activateSkill_20063(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, int roleid, int buffid, int round, float value1, float value2, float value3);
    virtual void activateSkill_20065(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    virtual void activateSkill_20066(SGHeroLayer *heroLayer, float value);
    
    
//    void removesb(cocos2d::CCObject *obj, SGHeroLayer*hero);
    void beginAi(SGHeroLayer *heroLayer);
    void beginUpTime(SGHeroLayer *heroLayer);
};

#endif /* defined(__GameSanGuo__SGHeroSkillsBase__) */
