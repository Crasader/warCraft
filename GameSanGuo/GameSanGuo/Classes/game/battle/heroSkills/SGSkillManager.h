//
//  SGSkillManager.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#ifndef __GameSanGuo__SGSkillManager__
#define __GameSanGuo__SGSkillManager__

#include "cocos2d.h"
#include "SGHeroLayer.h"
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"
#include "SGHeroSkillsConfig.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;

enum HeroSkillType
{
    skill_id_20000 = 20000,
    skill_id_20001,
    skill_id_20002,
    skill_id_20003,
    skill_id_20004,
    skill_id_20005,
    skill_id_20006,
    skill_id_20007,
    skill_id_20008,
    skill_id_20009,
    skill_id_20010,
    skill_id_20011,
    skill_id_20012,
    skill_id_20013,
    skill_id_20014,
    skill_id_20015,
    skill_id_20016,
    skill_id_20017,
    skill_id_20018,
    skill_id_20019,
    skill_id_20020,
    skill_id_20021,
    skill_id_20022,
    skill_id_20023,
    skill_id_20024,
    skill_id_20025,
    skill_id_20026,
    skill_id_20027,
    skill_id_20028,
    skill_id_20029,
    skill_id_20030,
    skill_id_20031,
    skill_id_20032,
    skill_id_20033,
    skill_id_20034,
    skill_id_20035,
    skill_id_20036,
    skill_id_20037,
    skill_id_20038,
    skill_id_20039,
    skill_id_20040,
    skill_id_20041,
    skill_id_20042,
    skill_id_20043,
    skill_id_20044,
    skill_id_20045,
    skill_id_20046,
    skill_id_20047,
    skill_id_20048,
    skill_id_20049,
    skill_id_20050,
    skill_id_20051,
    skill_id_20052,
    skill_id_20053,
    skill_id_20054,
    skill_id_20055,
    skill_id_20056,
    skill_id_20057,
    skill_id_20058,
    skill_id_20059,
    skill_id_20060,
    skill_id_20061,
    skill_id_20062,
    skill_id_20063,
    skill_id_20064,
    skill_id_20065,
    skill_id_20066,
    skill_id_20067,
    skill_id_20068,
    skill_id_20069,
    skill_id_20070,
    skill_id_20071,
    skill_id_20072,
    skill_id_20073,
    skill_id_20074,
    skill_id_20075,
    skill_id_20076,
    skill_id_20077,
    skill_id_20078,
    skill_id_20079,
    skill_id_20080,
    skill_id_20081,
    skill_id_20082,
    skill_id_20083,
    skill_id_20084,
    skill_id_20085,
    skill_id_20086,
    skill_id_20087,
    skill_id_20088,
    skill_id_20089,
    skill_id_20090,
    skill_id_20091,
    skill_id_20092,
    skill_id_20093,
    skill_id_20094,
    skill_id_20095,
    skill_id_20096,
    skill_id_20097,
    skill_id_20098,
    skill_id_20099,
    skill_id_20100,
    skill_id_20101,
    skill_id_20102,
};

class EndObjData : public CCObject {
    
public:
    int x, y, num;
};


class SGSkillManager : public CCNode, public CCAlertViewDelegate
{
private:
    SGHeroLayer *heroLayer;
    SGHeroLayer* heroLayer1;
    int skillId;
    
    bool isRemoveSkill;
    
     CC_SYNTHESIZE(SGHeroSkillsBase*, m_HeroSkills,HeroSkills);
    
    float protoValue1;
    float protoValue2;
    float protoValue3;
    int protoRound;
    int protoBuffid;
    int protoRoleid;
    int m_operateNum;
    std::vector<CCPoint> coordPointList;
    CCArray *endData;
public:
    int num;
    CCArray *array;
    SGHeroLayer* heroMe;
    SGHeroLayer* heroEnemy;
    float valueNum;
    int roleidNum;
    int jianNum;
    bool isBeginAi;
private:
    SGSkillManager();
    void mainSkillListener(CCObject *obj);
    void removePlist(CCObject *obj, CCString *str);
    void beginMainSkill(CCObject* obj, CCString *op);
    void activeSkillById();
    bool setHeroSkillData();
    int getAttackAp(SGBattleMap *map, SGGridBase *grid);
    int attackGrid(SGBattleMap *map, SGGridBase *grid, int num);
public:
    static SGSkillManager *shareSkill();
    void initNotification();
    void sendSkillRequest(int operateNum);
    
    void checkTheHero(SGHeroLayer *hero, float value1, int s_id);
    
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);
    
    void skillsAttack();

//    void baoZa(CCObject *obj, ShiBing *sb);
      void baoZa(CCObject *obj, GameIndex inedx);
    void huoQiu(CCObject *obj, ShiBing *sb);
    void beginUpTime();
    
    void setIsCompleteSkill();
    
public:
    void activateSkill_20000(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid);
    float activateSkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    void activateSkill_20002(SGHeroLayer *heroLayer);
    void activateSkill_20003(SGHeroLayer *heroLayer, int value1, int value2, int roleid);
    void activateSkill_20004(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float valule2,int heroId);
    void activateSkill_20005(SGHeroLayer *heroLayer, CCPoint point, int roleid);
    void activateSkill_20006(SGHeroLayer *heroLayer, float value1, CCPoint point);
    void activateSkill_20007(SGHeroLayer *heroLayer, float value1, int value2, int buffid);
    void activateSkill_20008(SGHeroLayer *heroLayer,  float rate);
    void activateSkill_20010(SGHeroLayer *heroLayer,  float rate);
    void activateSkill_20012(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int s_id);
    void activateSkill_20014(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid);
    void activateSkill_20018(SGHeroLayer *heroLayer, GameIndex index, CCArray* array, float value2, int num);
    void activateSkill_20020(SGHeroLayer *heroLayer, int addValue);
    float activateSkill_20022(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid);
    void activateSkill_20024(SGHeroLayer *heroLayer, int value);
    void activateSkill_20026(SGHeroLayer *heroLayer, CCObject *data);
    void activateSkill_20028(SGHeroLayer *heroLayer, float value1, float value2);
    void activateSkill_20030(SGHeroLayer *heroLayer, int skillsId);
    void activateSkill_20032(SGHeroLayer *heroLayer, float value);
    void activateSkill_20033(SGHeroLayer *heroLayer, float value);
    void activateSkill_20034(SGHeroLayer *heroLayer, int round, int buffid);
    void activateSkill_20036(SGHeroLayer *heroLayer, CCObject *data);
    void activateSkill_20038(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int jianid);
    void activateSkill_20040(SGHeroLayer *heroLayer, CCObject *data);
    void activateSkill_20043(SGHeroLayer *heroLayer, CCObject *data);
    void activateSkill_20046(SGHeroLayer *heroLayer, float value);
    void activateSkill_20048(SGHeroLayer *heroLayer, float value);
    void activateSkill_20050(SGHeroLayer *heroLayer, CCObject *data);
    void activateSkill_20052(SGHeroLayer *heroLayer, float value1, int buffId);
    void activateSkill_20056(SGHeroLayer *heroLayer, CCObject *data);
    void activateSkill_20058(SGHeroLayer *heroLayer, float value1);
    void activateSkill_20060(SGHeroLayer *heroLayer);
    void activateSkill_20063(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, int roleid, int buffid, int round, float value1, float value2, float value3);
    void activateSkill_20065(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    void activateSkill_20066(SGHeroLayer *heroLayer, float value);
    void activateSkill_20068(SGHeroLayer *heroLayer, CCObject *data);
public:
    void readySkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    void sbSkill_20005(CCObject *obj, int time);
    void removesb(CCObject* obj, SGHeroLayer*hero);
    void removeBullet(cocos2d::CCObject *obj, void* tag);
    void playBulletBlast(CCObject *obj, GameSbIndex *index);
    void changBloodNum(cocos2d::CCObject *obj, void* num);
    void callback();
    
    void skill_20063_Attack(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int acttime);
    
    void beginAi();
    
    void bbb(CCObject *obj);
    void ccc(CCObject *obj);
    
    void pushHskillData(int skid);

};
#endif /* defined(__GameSanGuo__SGSkillManager__) */
