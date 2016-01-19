//
// SGAttackList.h
//  GameSanGuo
//  攻击阵列类
//  Created by gengshengcai on 12-11-19.
//
//

#ifndef __GameSanGuo__BattleArrayBase__
#define __GameSanGuo__BattleArrayBase__

#include "cocos2d.h"
#include "SGGridBase.h"
#include "ShiBing.h"
#include "GameConfig.h"
#include "SGSoldier.h"
#include "SGBattleMap.h"
#include "SGBattleManager.h"
#include "SGHeroLayer.h"
#include "SG_SkillsManager.h"
#include "GameMusicmanage.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"

#define spineAtkTimeTest (1.3333/0.7)   //timeScale
#define Effect_DelayTime 1.0


#define     ChainSp_Tag 10
#define     SkillsAdd_Tag 11
#define     FERM_EFFECT_TAG 12
#define     SUB_ROUND_EFFECT_TAG 13
//#define     JIAN_EFFECT_TAG 14
#define     JIAN_EFFECT_TAG 20001 //modified by cgp
#define     JIAN_EFFECT_TAG2 114
#define     SKILL_JIAN_EFFECT_TAG 15
#define     SKILL_EFFECT_TAG 16
#define     LIANSUO_EFFECT_TAG 17
#define     RONGHE_EFFECT_TAG 18
#define     LIANSUO_CHAR_TAG     19


#define     EFFECT_LAYER_TAG 20000 //数字变化是名字挡住了所以改成了20000，原来是2000
#define     ATTACK_ADD_AP_NUM_TAG 2001
#define     ATTACK_ROUND_EFF_TAG 555
//#define     ANIMA_TIME_NUM  1.0   //cgpTime
#define     ANIMA_TIME_NUM  1.0
#define     FLOAT_TIMES     10000


#define     PUSH_ATTACK_DATA 0  //modified by cgp before is 1

//#define  ATTACK_DEALY_TIME 0.05  //cgpSPine
#define  ATTACK_DEALY_TIME 0  //cgpSPine
//#define  ATTACK_DEALY_TIME 0.2

#define         SB_XUE_LABEL_W      15  //形成阵列后的血条宽度和高度
#define         SB_XUE_LABEL_H      18

#define         SB_ROUND_LABEL_W    20.1  //回合数字
#define         SB_ROUND_LABEL_H    32


using namespace cocos2d;
enum sgLRtype
{
    lr_normal,//普通
    lr_l,//链接
    lr_r,//融合
    lr_lr//链接 融合
};
//形成的队列类型
enum sgAttackSoldierType
{
    kattacknormal,
    kattacktwo,
    kattackfour
};
enum sgAttackType
{
    kattackfont,//向前冲
    kattackstay//原地不动
};
enum sgEffectType {
    kAttackEffect,//远程攻击和近战打底线
    kStoreEffect,//形成蓄力时
    kBlockEffect,//挨打没死 抵挡
    kChargeEffect,//近战前冲
    kDeadEffect//死亡
    
};

class SG_SkillsBase;
class SGAttackList : public SGSoldier
{
public:
    bool mAttIsSpine;   //是否是spine
    CC_SYNTHESIZE(sgAttackSoldierType, sg_attackType,AttackType);
    CC_SYNTHESIZE(sgLRtype, sg_lrType, LRType);
    CC_SYNTHESIZE(SG_SkillsBase*, m_Skills_Delegate, SkillsDelegate);
    //@deprecated
    CC_SYNTHESIZE(SG_SkillsManager*, m_SkillsManager_Delegate, SkillsManager_Delegate);
    
    CC_SYNTHESIZE(bool, m_isDeath, IsDeath);
    CC_SYNTHESIZE(bool, m_isRevive, IsRevive);
    CC_SYNTHESIZE(int, m_reviveNum, ReviveNum);
    CC_SYNTHESIZE(float, jianDelayTime, JianDelayTime);
       
    void testlog();
    
    int defendBloodV;
    bool IsRoundBuff;
    CCPoint posxx;
    CCArray *attackSbs;
    CCLayer *m_effectsLayer;
    
    CCArray *m_attackBuffObj;
    
	//受到伤害的数值
    int damageNum;
    CCSpriterX *fermEffect;
    CCSkeletonAnimation    *wjhbEffect;    //武将阵列形成时的动画
    
    
    //CCSpriterX *jianEffect;
    CCSkeletonAnimation* jianEffect;
    float JianDelayTime;    //箭延时发出的时间，读表。
    
    CCSkeletonAnimation* effBottomHit;   //打底线特效
    CCSkeletonAnimation* effWallHit;     //打城墙特效
    
    
    
    CCSpriterX *sb_jianEffect;
    bool attOver;
    
    float shengYuTime;

    CCProgressTimer *m_sb_xueTiao;
    
    CCString *pushData;
    CCSprite *roundNum;
    
protected:
    int attackI;
    int thisJ;
    int attackJ;
    int actionCount;
    CC_SYNTHESIZE_READONLY(int, fuseNum, FuseNum);
  
    bool isMy;
    //
    CCLabelAtlas *roundNumLabel;
    CCLabelAtlas *apLabel;
    //自己的英雄
    SGHeroLayer *myHero;
    //被攻击的英雄
    SGHeroLayer *attackHero;
    
    int sb_xuetiao;//长度
 
public:
    SGGridBase*  mCgpGrid;     //cgp add for remove attacklist delay
    int resistForce;     //added by cgp for bloodNum delay
    float sbTime;
    int m_gridRow;     //存储被攻击格子的位置
    int m_gridList;
    
    bool m_isDead;
    float m_attackTime;  //攻击时间，后续读表
    float m_effTime;
    GameIndex   m_dispearIndex;   //存储近战阵列消失时的index
    GameIndex   m_dispearRemote;  //存储远程阵列消失时的初始index
    
public:
    void cgpEffectTest(CCNode* , int);
    void callRemoveHitBottom();
    void callRemoveHitWall();
    
    void cgpCallJianEffDelay();
    void cgpCallRmSbDelay(CCNode*);
    void cgpCallRmAttackList();
    void attackEff(CCNode* pSender,   GameSbIndex *sbindex);
    void delayAttackOver(CCNode*  psender, void *mydate);
    void myDispear(CCNode* psender, GameIndex * sbindex);  //打不过敌人，自几消失，
    void myDispearRemote(CCNode* psender, GameIndex * sbindex);
    SGAttackList();
    ~SGAttackList();
    
    
    static SGAttackList* createWithSoldier(ShiBing *sb, bool ismy, sgAttackSoldierType type); //两格武将
    bool initWithSoldier(ShiBing *sb, bool ismy, sgAttackSoldierType type);  //两格四格公用

    virtual void startAttack(SGHeroLayer *myHero_, SGHeroLayer *opponentHero);         //两格武将，四格在FourAttackList
    
    virtual void changeRound(int round, bool isAction = false);
    virtual void changeAP(int ap, bool isAction = false, bool isPlayEff = false, bool isDrawXuetiao = true);
    virtual void updateBlood(int ap);
    virtual int getAttackJ();
    virtual bool chackAttackChain(SGAttackList *attalkList);//如果是整合返回true
    virtual bool chackAttackFuse(SGAttackList *attalkList);
    virtual void drawEffectSp(int effectId);//绘制特效
    virtual void hideEffectSp();
    virtual void subRoundEffect();
    virtual void drawSkillsEffect(int num);
    virtual void removePlist(CCObject *obj, CCString *str);
    void playAttackEf(int num);
    CCArray *getAttackSbs();
    bool isGridin(SGGridBase *grid);
    void calculateChainAp(SGHeroLayer *hero);
    void calculateFuseAp(SGHeroLayer *hero);
    void attackOver();
    
    void activateSkills( SGBattleMap* battleMap, SGAttackList *attackList, int skID);
    void activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num);
    bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage);
    bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index);  //远程小兵
    bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    void activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);

    int activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb);
    int activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1);
    
    
    void activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage);
    void activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    void activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    bool activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList, ShiBing *sb);
    void activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1);
    void activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row = 0);
    void activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    
    bool assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    void changeLRType(sgLRtype lrtype);
    
    int attackSubAp(int ap);
    
    void setBuffListObj(SG_BuffBase* buff);
    
    GameIndex getAttackIndex();
    
    void activateBuff();
    void attackAddAp(float num);
    void heroSkillSubAp(int num);
    void setFuseNum(int num);
    
    void removeAttackSb(CCObject *obj);
    void changBlood();
    
    void removesb(cocos2d::CCObject *obj, GameSbIndex *sbindex);   //两格武将，四格在FourAttackList
    
    float setActAndTime(ShiBing *sb, bool onlyTime = false);
//    void setAct(CCObject *obj, int time);
    void setAct(CCObject *obj, GameSbIndex *sbindex);
    
    
    void vanishAttackList(CCObject *obj);                //两格武将，四格在FourAttackList
    void addSkillsBuff(CCObject *obj, int* rand);
    void drawXueTiaoProportion();
    
    void labelJump(CCLabelAtlas *label, int num);
    void setLabelNum(CCObject *obj, int num);

    void showBattleLabel(int num, const char *name, int w, int h, GameIndex index);
    void removeBalleLabel(CCObject *obj);
    
    void pushAttackData(CCString *str, bool push);

    void changeRevive();
    void attackSbVibration();
    void movePause(CCObject *obj, GameSbIndex *sbindex);
    void playEffectWithType(sgEffectType effectType);
    void showSkillName();
    void removeEff(CCObject *obj);
    void showAttackTip(const char* name, float delayT = 0.0f);
    void showAttackName();
    void updatebloodNum();
    void scaleTOSec();
    
    
    virtual void setAp(int appVal);

	
	void changAPLabel(); //更新血条显示
	

private:
    void addShowMsg(sgAttackSoldierType type);
    void removeAttack(CCObject *obj);
public:
    int currentAp;
    int finialAp;
    int addAp;
    bool isAdd;
};
#endif /* defined(__GameSanGuo__AttackList__) */
