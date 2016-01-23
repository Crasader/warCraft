//
//  SGHeroLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-5.
//
//


#ifndef __GameSanGuo__SGHeroLayer__
#define __GameSanGuo__SGHeroLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SGBattleMap.h"
#include "SGBaseLayer.h"
#include "CCSpriterX.h"
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"
#include "SGCCLabelTTF.h"
#include "Utils.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

#define    TextFontName "AmericanTypeWriter-Bold"
#define    SKILLS_BULLET_TAG  50
#define    TONGJING_EFFECT_TAG 51
#define    FORBID_EFFECT_TAG   53
#define    ADD_AP_LAYER_TAG   1900
#define    XDLAN_LAYER_TAG    1899


#define    LIUGUANG_EFFECT_TAG 1969
#define    CGP_LIUGUANG_TAG    2023
#define    CGP_EFFECT_ZORDER  2024
typedef enum e_LIUGUANG_TAG
{
    KJ_Mask = 0,
    KJ_NewHero,
    KJ_ColorBg,
    KJ_head,
    KJ_Char,
    KJ_AttEff,
    KJ_MainSkillMask,
    KJ_Dazhao,
    
}E_LIUGUANG_TAG;  //开镜特效相关层次和Tag偏移值

typedef enum eff_name
{
    Eff_xiaoshi = 0,
    Eff_sanbing,
    Eff_faluo,   //法螺，代替旗帜
    
}EFF_name;


#define     RoundCountryAB  1

#define BATTLE_FIEL_DROP_TAG 2555
#define HEAD_TAG 9

//墨迹点延时显示的传参
typedef struct _mojiPos
{
    int ypos;
    bool isFlipX;    //true 反转
    
}T_mojiPos;

//城门血量状态 完整-划痕-破裂-倒塌
enum GateBloodState
{
    GBS_GOOD = 0,
    GBS_BROKEN,
    GBS_CRUSHED,
    GBS_DESTROYED,
};

class SGAttackList;

//跳动的数字， moved by cgp
#define         SB_ADD_AND_SUB_AP_W 22.3
#define         SB_ADD_AND_SUB_AP_H 32

class HeroObj : public CCObject
{
public:
    HeroObj();
    ~HeroObj();
    
    
    CC_SYNTHESIZE(int, h_roleId, RoleId);
    CC_SYNTHESIZE(int , h_hp, Hp);
    CC_SYNTHESIZE(int , h_basehp, BaseHp);
    CC_SYNTHESIZE(int, h_actionCount, ActionCount);//最大行动力
    CC_SYNTHESIZE(int, h_actionCount1, ActionCount1);//当前行动力
    CC_SYNTHESIZE(int, h_ItemId, ItemId);
    CC_SYNTHESIZE(int, h_FriendLevel, FriendLevel);
    CC_SYNTHESIZE(int, h_FriendHeadId, FriendHeadId);
    CC_SYNTHESIZE(int, h_skillId, SkillId);//主将计ID号
    CC_SYNTHESIZE(int, h_SkillRound, SkillRoundMax); //最大主将计回合数，即基础值。
    CC_SYNTHESIZE(int, h_SkillRoundCur, SkillRoundCur); //当前主将计回合数，为应对守荆州的连续战斗。 2013.12.03 逆天上市。
    
    CC_SYNTHESIZE(int, h_ClientType, ClientType);
    CC_SYNTHESIZE(int, h_Country, Country);
    CC_SYNTHESIZE(int, h_Atkmode, Atkmode);
    CC_SYNTHESIZE(int, h_max_soldierNum, Max_soldierNum);
    CC_SYNTHESIZE(int, h_FriendRound, FriendRound);
    CC_SYNTHESIZE(int, _friendRound, _friendRound);
    CC_SYNTHESIZE(int, h_LordSkillLevel, LordSkillLevel);
    CC_SYNTHESIZE(int, h_LordSkillID, LordSkillID);
    CC_SYNTHESIZE(int, h_ActionTime, ActionTime);
    CC_SYNTHESIZE_RETAIN(CCString *, h_name, Name);
};

class SGHeroLayer :public SGBaseLayer, public CCAlertViewDelegate
{
public:
    void delayUseSkill();
    bool isEnemySkillReady;   //敌人主将计是否满了
   
    
    
private:
    int moveNum;//
    int allNum;
private:
    CC_SYNTHESIZE(bool, h_isRobot, IsRobot);
    //    血量
    CC_SYNTHESIZE(int, h_morale,Morale);
    //    攻击力
    CC_SYNTHESIZE(int, h_atk,Atk);
    CC_SYNTHESIZE(std::string, heroName,HeroName);
    int roleId;
    //等待区的士兵个数

    CC_SYNTHESIZE(int, soldierNum,FreeSoldierNum);

    // 血量
    //主将计能回合数（动态/初始）
    CC_SYNTHESIZE(int, skillsRoundNum,SkillsRoundNum);
    CC_SYNTHESIZE(int, baseSkillRoundNum,BaseSkillRoundNum);

    CC_SYNTHESIZE(int,country,Country);
    CC_SYNTHESIZE(int,itemId,ItemId);
    
    SGCCLabelTTF *soldierNumLabel;//援兵
    SGCCLabelTTF *roundNumLabel;//行动力
    CCLabelAtlas* roundNumAtlas;   //行动力
    
    SGCCLabelTTF *friendNumLabel;//友军/好友
    SGCCLabelTTF *skillNumLabel;//主将计

    SGCCLabelTTF *heroBloodLabel;//当前血

    SGCCLabelTTF *nameLabel; //主将姓名
    SGBattleMap *battleMap;
    //所有已形成的攻击阵列
    CCArray *_attackLists;
    CCArray *buffs;
    bool isRefreshMap;
    
    bool isBattleOver;
    
    bool isAttackBase;
    bool isAttackOver;
    
    
    
    bool isInitPve;
    //好友主将ID
    CC_SYNTHESIZE(int,friendHeadId,FriendHeadId);
    //好友主将等级
    CC_SYNTHESIZE(int,friendLevel,FriendLevel);
    
    CC_SYNTHESIZE(int, _initRoundTime ,InitRoundTime);
    
    int _roundTime;
    CCLabelAtlas *roundTimeLabel;
    CCSprite *m_RoundTimeT;
    CCSprite *tbj;
    int _controlRoundNum;
    CCPoint headPos;
    CCSprite *m_ljBj;
    CCSprite *las;
    
    CCSprite *gbSprite;//光标显示
    
    //显示的自动战斗的按钮
    SGButton *autoBattleBtn;
    
    //开启自动战斗等级
    int autoBattleHighLevel;
    //开启一倍速度等级
    int autoBattle1SpeedLevel;
    //开启三倍速度等级
    int autoBattle3SpeedLevel;
    
    //放每次武将技能头像的retain的指针，dtor的时候统一释放
    std::vector<CCSprite *>skillHeadVec;
    
public:
    int wujiangjiWhich ;  //每回合攻击的武将计第几个  added by cgp
    int wujiangjiCount;   //每回合攻击的武将总数
    int wujiangjiNum;     //每回合攻击的武将总数。
    void showAttactHeadEffect1(cocos2d::CCArray *array);
    //血量进度条
    CCProgressTimer *hpProgress;
    
    //城门血量状态
    GateBloodState m_gbs;
    
    //城墙主体
    CCSprite* uibg;
    
    //四大铭牌按钮
    SGButton *zjj_btn;     //能量槽按钮
    SGButton *end_btn;      //友军， 改为完，结束行动力
    SGButton *surrender_btn;     //我方行动力  改为投降
    SGButton *yb_btn;     //援兵
    
    CCSkeletonAnimation*   effZjjBright;  //主将计满了的闪烁光芒
    CCSkeletonAnimation*   effJiNeng;     //技能字效果
    
    
    
    CCMenu *autoMenu;
    
    void setReduceRoundLabel(SGCCLabelTTF* label, int num );
    //生成数字更改动画
    void generateLabelAnim(SGCCLabelTTF* label, int num,bool isAction =false);

    //设置autoBattleBtn是否可触
    void setAutoBattleBtnCanTouch(bool canTouch);
    //根据星级和（是不是副本）决定是否显示自动战斗
    bool displayAutoBattleBtnWithRank();

private:
    //四大铭牌铁链动画
    CCSpriterX* zjj_chain;
    CCSpriterX* yj_chain;
    CCSpriterX* xdl_chain;
    CCSpriterX* yb_chain;
    
    //城墙左侧铁链1
    CCSpriterX* wall_left_chain_1;
    
    //城墙左侧铁链2
    CCSpriterX* wall_left_chain_2;
    
    //城墙右侧铁链1
    CCSpriterX* wall_right_chain_1;
    
    //城墙右侧铁链1
    CCSpriterX* wall_right_chain_2;
    
    //城门失火动画
    CCSpriterX* gate_fire;
    
    //城门静态图片
    CCSprite* gate_state;
    
    //城门被击动画
    CCSpriterX* gate_blood;
    
    //士兵数组
    CCArray *sbs;
    
    //主将计文本图片
    CCSprite* zjjText;
    
    //主将计可发动文本图片
    CCSprite* zjjTextRock;
    
    //主将计进度条
    CCProgressTimer* zjjProg;
    
    //主将计可发动文字动画
    CCSpriterX* zjjAnim;
    
    //主将计可发动边框动画
    CCSpriterX* zjjAnimFrame;
    
    //主将计满槽光芒特效
    CCSpriterX* zjjReloadLight;
    
    //主将计满槽火花特效
    CCSpriterX* zjjReloadSpark;
    
    //友军文本图片
    CCSprite* yjText;
    
    //友军可发动文本图片
    CCSprite* yjTextRock;
    
    //友军进度条
    CCProgressTimer* yjProg;
    
    //友军可发动文字动画
    CCSpriterX* yjAnim;
    
    //友军可发动边框动画
    CCSpriterX* yjAnimFrame;
    
    //友军满槽光芒特效
    CCSpriterX* yjReloadLight;
    
    //友军满槽火花特效
    CCSpriterX* yjReloadSpark;
    
    //头像底框
    CCSprite* portraitFrame;
    
    CCSprite *skillHead;
    CCSprite *skillHead1;
    CCSprite *skillHead2;
    CCSprite *skillName;
    CCSprite *skillName1;
    CCSprite *skillName2;
    CCString *_skName;
    CCString *_skName1;
    CCString *_skName2;
    int _skLevel;
    int _skLevel1;
    int _skLevel2;
    
    SGCCLabelTTF *wuzjj;
    
    std::vector<GameIndex>  m_jumpLableIndexList;
    
    float abandonRoundCountryABRate1;
    float abandonRoundCountryABRate2;
private:
    //测试用的小BOOL
    bool heyJude;
    
    //长城基准位置
    CCPoint motherPos;
    
    //被击城墙的闪屏与晃动
    void shakeAndWobble();
    
    //生成相应SHAKE动画，这个指针指向小泽·玛利强。csx=动画对象，dir=方向，which哪个按钮
    void generateAnim(SGButton* sgb, bool dir, int which);
public:
    //主将计和友军的状态与特效处理。onoff:true为开启,false为关闭 which:1为主将计，2为友军
    void changeButtonState(bool onoff, int which);
private:
    //对图片使用动态的进度条效果。cpt：进度条对象 prop：将要到达的比例 fullTime：0-100所需时间
    void setProgressEffect(CCProgressTimer* cpt, float prop, float fullTime);
    
    //四大按钮位置
    CCPoint yjBtnPos;
    CCPoint zjjBtnPos;
    CCPoint xdlBtnPos;
    CCPoint ybBtnPos;
    CCSprite* imgMove;    //行动力文本
    
    //铁链动画是否启动
    bool isChainStarted;
    
    //铁链动画已发动时间
    float curChainTime ;
    
    //覆写更新，为了监控动画
    void update(float delta);
    
    //城门状态图片更新
    void changeGateState(float state);
    void changeRoundRemindME();
    CCPoint remindMePosition(SGBattleMap *map);
    
    
    //自动战斗开始之后显示的动画
    CCSprite *autoBattleTip;
    //基础自动战斗按钮的位置
    CCPoint baseAutoBtnPos;
    //二次点击按钮不能太快
    bool isQuickPress;
    
    //设置isQuickPress
    void setIsQuickPress();
    //点击自动战斗的时候给一个延时再触发逻辑
    void pressBtnDelay(CCObject *obj);
public:
    //自动战斗按钮回调
    void autoBattleWithAI();
    //设置我方按钮可用
    void setBtnEnable(bool canTouch);
    //显示自动战斗的提示动画
    void displayAutoBattleAnimation();
    //移除自动战斗的动画
    void removeAutoBattleTips();

public:
    bool isme;
    //SGGridBase *m_seveBuffGrid[mapRow][mapList];
    //最大血量
    int heroBloodMax;
    
    int m_skillsId;
    int m_skillLevel;
    int lordSkillId;
    int clientType;
    int uibgY;
    int m_jianId;
    // 回合数
    int roundNum ;
    CC_SYNTHESIZE(int, initRoundNum,InitRoundNum);
    CC_SYNTHESIZE_RETAIN(CCString *, m_skillName, SkillName);
    //pve最大兵数
    int max_soldierNum;
    //友军发动回合数（动态/初始）
    int friendNum;
    int initFriendNum;
    
    CCSpriterX* m_skills_Bullet;
    CCSpriterX* m_skills_Bullet1;
    CCSpriterX* m_skills_Bullet2;
    CCLayerColor *statusBarLayer;
    int bulletJ;
    bool isFriend;
    bool isFriendOffDeath;//是否在
    int m_LJNum;//原来是连击数目,现在改为累计伤害数
    int dangqianxie;
    bool isRoundEnd;
    CCArray *headEffArray;
    
    //主将计消耗的散兵
    CC_SYNTHESIZE(int, m_sanCount,SanCount);
public:
    void setSkillsPosj(int j);
    void setBulletSp(int num);
    void mainSkillAttackHandler();  //发动我方主将计
    void stateUpdateTime();
    
    void delayRemoveAttaklist(CCNode*,  void*);
private:
    void machineAction();
    
    void sendSB2Scene();
    //开始攻击
    void startAttack();

    //主将计
    void mainSkillHandler();        //我方
    
    void enemyMainSkillHandler();   //地方

    
    
    void updateProgress(float pro);
    
    void updateRoundBuff();
    void showEmenyBuAct();
    bool heroDefSkills();
    void refreshMap(CCObject *obj, CCDirector *buff);
    void attackBegin();
    void removePlist(CCObject *obj, CCString *name);
    
    void removeEffSpine(CCObject *obj,   void*);
    void playLuiGuanEffect(CCObject *obj);
    
    void callBatHeroEffEnd(CCObject* , int*);   //added by cgp
    void callMaskRemove();     //added by cgp
    void callMainSkillMaskRemove();
    void callDisplay1(CCNode* ,  T_mojiPos*);
//    void callDisplay2(CCNode* ,  T_mojiPos*);
//    void callDisplay3(CCNode* ,  T_mojiPos*);
    void removeXian(void);
        
    void setLabelNum(CCObject *obj, int num);
    void delayTimeAttack(CCObject *obj);
    void chainApAnd();
    void updataRoundTime();
    void delayChangeRound();//MM: 暂封存不用，似乎未解决某些主将计释放在回合时间结束的问题。
    void bulletShow(CCNode *node);
    void randomX();
    //void gotoNextAutoStep();
    void headEffects(CCObject *array);
  
public:
    CCSkeletonAnimation*  dazhao;
public:
    SGHeroLayer();
    ~SGHeroLayer();
    static SGHeroLayer * create(HeroObj *heroObj, CCArray *sbs, bool isme);
    bool init(HeroObj *heroObj, cocos2d::CCArray *sbs, bool isme);
    void initBattleMap();
    //补兵
    void fillUp();
    //主将计
//    void mainSkillHandler();
    //更改等待区的兵的个数 （num 可以为负 表示减病）
    void changeSoldierNum(int num, bool isRepair = false);
    //更改回合数
    int changRoundNum(int num, bool isAction);
    //更新玩家血量
    void changBloodNum(int num, bool isBuff = false, bool skill = false);
    
    void setSkillsRoundNum(int num, bool isAction);
    void showHeadStreak();
    void showHeadStreak1();
    void showHeadStreak2( CCSprite *head, CCSprite *skname,CCString *name, int level);
    void rmeovehst();
    int getItemId();//主将id
    
    
    void abandonRound();//放弃回合
    void surrenderRound();//投降回合
    void endRound();
    void aiEndRound();
    void heroSurrender();
    void friendHardler();
    void friendArmyUp();
    
    void cutAttackRound(int num);//减攻击阵列的回合数  测试
    
    void changeRound(bool isSetTouch, bool isMyRound, bool isFirst = false);
    void changeFriendNum(int num, bool isAction = false);
    SGBattleMap *getBattleMap();
    void showAttactHeadEffect(CCArray *array);
    void attackOver(SGAttackList *attackList_, bool notRemove = false);
    
    
    void setSoldierNum(int num);
    void setRoundNum(int num);
    void setBloodNum(int num);
    void setSkillsRound(int num);
    void setRoundTime(int Time);
    int getRoleId();
    int getSoldier(bool remainder = false);
    int getRound();
    int getBlood();
    int getSkillsRoundNum();
    void initSkillRoundNum();
    int getInitRoundNum();
    int getCountry();
    int getFriendNum();
    bool getIsInitPve();
    
    CCArray *getBuffs();
    
    SGHeroLayer *getOpponentHeroLayer();
    void addBuff(CCDictionary *buff);
    
    //void showBattleId(std::string str);

    void giveupMessage(int skillNum);
    
    void playBuffEffect();
    
    void activateBuff();
    void abandonRoundCountryAB();
    
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);

    void showEffectLayer(int type, bool isRelease, GameIndex index = gameIndex(0, 0), CCPoint difPos = ccp(0,0));
    //added by cgp for spineEff
    void showEffectLayerCgp(int type, bool isRelease, GameIndex index = gameIndex(0, 0), CCPoint difPos = ccp(0,0));
    void showBattleLabel( int num, const char* name, int w, int h, GameIndex index, bool skill = false,float delayTime = 0, float removeDelay = 1.0f);
    void removeBalleLabel(CCObject *obj);
    void showRevivelLayer();
    void showMoraleLabel(int num, const char *name, int w, int h, bool skill = false);
    void removeMoraleLabel(CCObject *obj);
    void showLJEff(bool isHide = false ,int damage = 0);//原来是连击特效显示,现在改为累计伤害
    void delayRemoveLJEff(void);
    void hideLjEff(CCObject *obj);
    void showSbXueLabel(CCLabelAtlas *label, ShiBing* sb);
    void repair();
    void showJianBlastEffect(int type, bool isRelease, GameIndex index = gameIndex(0, 0), bool isHome = false, bool isScale = false);
    void beginRoundTime();
    void restartRoundUpdataTime();
    void endRoundUpdataTime(); //在某些耗时的操作时候，会调用这个，比如补兵中/技能释放中，不会让玩家时间空流逝。
    int getCntrolRoundNum();
    virtual void setIsCanTouch(bool isTouch);
    virtual void showBoxCall(CCObject *obj);
    void upDAtaControlRound(int rond);
    void chackBattleFieldDrop(CCArray *array);
    void battleEnd(float t);
    void showBattleWin(CCObject *obj);
    void playBattleW(CCObject *obj);
    void showBattleFieldDrop(bool isCard, CCPoint pos = ccp(0,0), int num = 1, bool isWin = false);
    void rmeoveDrop(CCObject *obj);
    
    void setLabelColor(cocos2d::CCObject *obj,int num);
    void starEffect();
    void scaleWithLabel(cocos2d::CCObject *obj,int num);
    void showAddApEff(bool round, CCPoint _pos);
    void rmeoveaddeff(CCObject *obj);
	
	void changeEnemyRoundNum(int num, int skillNum);
	
	bool notBeCanAutoBattlePlot();

private:
    bool isCanCallFriend(int GridNum);
    void rmeovehst1(CCObject* obj);
private:
    void updateBooldNum();
    void scaleForBlood();
    int lostBlood;//改变的血量
    int preBlood;//改变之前的血量
    
private:
    void cooperationFillResponse(cocos2d::CCObject *obj);
    void cooperationFillRequest();
    
private:
    void setBtnPosition(CCObject *obj,int num);
    void specialEffictBtn(SGButton *button,int num);
    void roundNumMOve(int num);
    void rmoveRoundNum(CCObject *obj);
    void onClickSkipArenaFightButton();//跳过竞技场战斗
};
#endif /* defined(__GameSanGuo__SGHeroLayer__) */






