//
//  SGAIManager.h
//  GameSanGuo
//
//  Created by geng on 13-2-28.
//
//

#ifndef __GameSanGuo__SGAIManager__
#define __GameSanGuo__SGAIManager__

#include "cocos2d.h"
#include "SGBattleMap.h"
#include "ShiBing.h"
#include "SGAi.h"

USING_NS_CC;


enum StepType
{
    st_moveToScene,
    st_moveOutScene,
    st_move3del0,
    st_move2del0,
    st_move1del0,

    st_move3del1,

    st_move2del1,
    st_move2del2,
    st_move1del1,
    st_move1del2,
    
    st_move0del1,
    st_move0del2,
};
enum Step
{
    s_null,
    s_scanDel,//删除组成攻击
    s_del,

    s_moveToScene,
    s_moveOutScene,

    s_giveup
};

enum ActionType {
    at_null,
    at_delete ,
    at_move ,
    at_mainSkill,
    at_bu,
    };
struct SuggestIndex {
    SuggestIndex() : actionType(at_null)
    {};
    
    ActionType actionType;
    GameIndex deleteIndex;
    GameIndex fromIndex;
    GameIndex toIndex;
};

struct StepIndex
{
    StepIndex() : stepType(st_moveToScene)
    {};
    
    StepType stepType;
    GameIndex from1;

    GameIndex to1;

    GameIndex del1;

};
 

class SGAIManager : public CCObject
{
private:
    ReturnIndex nowIndex;
    
    Step nowStep;
    
    StepIndex stepIndex;
    //是否已经补兵
    bool isFilled;
    SGBattleMap *battleMap;
    
    //标记是否开启自动战斗,默认为false，即不开启
    CC_SYNTHESIZE(bool, _isOpenAutoBattle, IsOpenAutoBattle);
    
    //自动战斗速度
    CC_SYNTHESIZE(int, _animationSpeed, AnimationSpeed);
    
    /**
     *  标记一次切换回合的时候是不是所有的攻击阵列都攻击完成
     */
    CC_SYNTHESIZE(bool, _isCompleteAttack, IsCompleteAttack);
    
    //标记是否是手动触发主将技
    CC_SYNTHESIZE(bool, _isCompleteSkill, IsCompleteSkill);
    
    //是否已经显示过主将技头像
    CC_SYNTHESIZE(bool, _isDispSkillHead, IsDispSkillHead);
    
public:
    SGAIManager() : aiStep(0), isPVE(false), isInitPve(false), isAi(false), isLostCon(false), isActivateAI(false), isBattleOver(false), battleMap(NULL), isFilled(false), nowStep(s_null), _isOpenAutoBattle(false),_isCompleteAttack(false),_isCompleteSkill(true),_isDispSkillHead(false),_animationSpeed(1)
    {};
    
    int aiStep;
    bool isPVE;
    bool isInitPve;
    bool isAi;
    bool isLostCon;
    //是否是活跃的AI
    bool isActivateAI;
    bool isBattleOver;
    bool m_isVarifyActiviteAI;//控制验证战斗双方是否能动

public:
    //开始引导AI
    void startGuideAI(SGBattleMap *bm);
    bool startMainSkill(SGBattleMap *bm);

private:

//    //只删除
    void startDel(SGBattleMap *bm);

    bool startFillUp(SGBattleMap *bm,Step nextStep);

    
    //最新优先级的入口
    
    ReturnIndex scanAllCondition(SGBattleMap *bm);
    void actionWithReturnIndex(ReturnIndex rt_index,SGBattleMap *bm);

    ReturnIndex fastGetAttackForOne(SGBattleMap *bm);
      
public:
    static SGAIManager *shareManager();
    void setNowStep(Step _nowStep);
    
    //入口函数，每一个回合都会调用的函数
    void startRound(SGBattleMap *bm);

    //把场景外的兵移动到场景内
    void startMoveToScene(SGBattleMap *battleMap);
    void startMoveOutScene(SGBattleMap *battleMap);
    
    
    void setIsActivateAI(bool isai);
    
    SuggestIndex returnSuggestIndex(SGBattleMap *battleMap);
    void executAI();
    void resetSbSkye(SGBattleMap *map);
};
#endif /* defined(__GameSanGuo__SGAIManager__) */
