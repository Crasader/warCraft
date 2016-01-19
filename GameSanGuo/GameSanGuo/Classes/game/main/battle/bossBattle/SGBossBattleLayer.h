//
//  SGBossBattleLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-26.
//
//

#ifndef __GameSanGuo__SGBossBattleLayer__
#define __GameSanGuo__SGBossBattleLayer__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGStoryReward.h"
#include "SGBossRewardLayer.h"
#include "CCSpriterX.h"
struct BossBattleData {
    int bossItemId;
    int bossHp;
    int bossBaseHp;
    int lordItemId;
    int lorddamge;
    int lordType;
    bool isCrit;
};

class SGBossBattleLayer:public SGBaseLayer {
public:
    SGBossBattleLayer();
    ~SGBossBattleLayer();
    
    static SGBossBattleLayer * create(BossBattleData *bossData,BossReward *bossReward);
      void startAction();
private:
    virtual void initMsg();
    void viewDidLoad();
    void initView();
    void skipBattle();
    void showEffectOfSolider();
    void showEffectOfSolider2();
    void showEffectOfLord();//向前冲
    void showEffectOfLord2();//爆炸
    void showEffectOFBoss();//向前冲
    void showEffectOfBoss2();//爆炸
   
    void removeBufSprite();
    void finishAction();
    void showBattleLabel(int num, int w, int h, bool isBoss);
    void removeBalleLabel();
    void fuckLayer(bool isBoss=false);
    
public:
    CC_SYNTHESIZE(int ,m_bossSid, BossSid);//bossID
    CC_SYNTHESIZE(int , m_bossColor, BossColor);
    
    CC_SYNTHESIZE(int ,m_LordSid, LordSid);
    CC_SYNTHESIZE(int , m_LordColor, LordColor);
    CC_SYNTHESIZE(int , m_lordType, LordType); //1是两格的武将， 2是四个武将
    
    CC_SYNTHESIZE(int, m_countryId, CountryId);
    CC_SYNTHESIZE(int , m_lordDamage, LordDamge);
    CC_SYNTHESIZE(int , bossHp, bossHp);
    CC_SYNTHESIZE(int, bossBaseHp, bossBaseHp);
    CC_SYNTHESIZE(int, lordSkillId, lordSkill);
    CC_SYNTHESIZE(int , bossSkillId, bossSkillId);
    
    CC_SYNTHESIZE(bool, isCrit, isCrit);
    
private:
    BossReward *rewardData;
    CCPoint soldierPos[4];//光效坐标
    CCPoint bossPos;
    CCPoint lordPos;
    
    CCArray *soldiers;
    
    CCSpriterX *bossSprite;
    CCSpriterX *lordSprite;
    
    
private:
    void deleteSbObj();
    void showRemoveEff(CCObject *obj);
    void callRemoveSb(CCObject *obj);
     
};

#endif /* defined(__GameSanGuo__SGBossBattleLayer__) */
