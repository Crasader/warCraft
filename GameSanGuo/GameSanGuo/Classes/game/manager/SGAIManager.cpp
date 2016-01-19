//
//  SGAIManager.cpp
//  GameSanGuo
//
//  Created by geng on 13-2-28.
//
//

#include "SGAIManager.h"
#include "SGGridBase.h"
#include "SGAIBu.h"
#include "SGHeroLayer.h"
#include "SGBattleManager.h"
#include "SGAIHero.h"
#include "SGGuideManager.h"
#include "SGRobotGuideVO.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "AIFirstPriority.h"

static SGAIManager *manager;
SGAIManager *SGAIManager::shareManager()
{
    if (!manager)
    {
        manager = new SGAIManager();
        manager->isAi = true;
        manager->isLostCon = false;
        manager->aiStep = 0;
        manager->isActivateAI = false;
        manager->isBattleOver = true;
        manager->isInitPve = true;
        manager->m_isVarifyActiviteAI = true;
    }
    return manager;
}

 

//开始回合  ai 每一步
void SGAIManager::startRound(SGBattleMap *bm)
{
    CCLog("$#########AI#######");
    
    if (!isAi ||
        !isBattleOver ||
        SGBattleManager::sharedBattle()->getBattleLayer()->getStopAi())
    {
        CCLog("$不isAi:%d,isbattleOver:%d",isAi,isBattleOver);
        
        return;
    }
    
    if (!isPVE )
    {
        CCLog("$####AI: 没有托管不ai");
        return;
    }
    
    SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
    if (heroLayer->getRound() == 0)
    {
        CCLog("$###AI: 切换回合");
        return;
    }
    
    //判定是否是玩家一方,并且在自动战斗没有开启的情况下，才直接返回
    if (bm->getIsMe() && !SGAIManager::shareManager()->getIsOpenAutoBattle())
    {
        CCLog("$####AI: 自己操作不ai");
        return;
    }
    if(!m_isVarifyActiviteAI)
    {
        return;
    }
    
    if (SGGuideManager::shareManager()->isGuide)
    {
        CCLog("$####AI: 开启引导AI");
        this->startGuideAI(bm);
        return;
    }
    
 
    if (!isActivateAI) {
        //heroLayer->aiEndRound();
        CCLog("$$不是activateAI");
        //return;
    }

    
    
    CCLog("$$#############目前map里面未组成四格和二格的数目：%d,%d###################",bm->fourList->count(),bm->twoList->count());
    
    ///*********************让ai想一会*************************
//    int delayTime=rand()%3+1;
//    if (heroLayer->isInitPve)
//    {
//        delayTime  =0.4;
//    }
    //modified by cgp
    float  delayTime = 0;
    battleMap=bm;
    CCCallFunc *call=CCCallFunc::create(this, callfunc_selector(SGAIManager::executAI));
    battleMap->runAction(CCSequence::create(CCDelayTime::create(delayTime),call,NULL));
}

void SGAIManager::executAI()
{
    CCLog("$$###AI开始执行逻辑。。。。。");
    ReturnIndex rt_index=SGAIManager::scanAllCondition(battleMap);
    actionWithReturnIndex(rt_index, battleMap);
}

#pragma mark -新的优先级


void SGAIManager::resetSbSkye(SGBattleMap *map)
{
    
    int count=map->getTwoList()->count();
    for (int i=0; i<count; i++)
    {
        SGPrinceBase *superBase=(SGPrinceBase *)map->twoList->objectAtIndex(i);
        SGGridBase *baseGrid=superBase->getNumGrid(kdown);
        
        if (baseGrid && baseGrid->getStype()==kattack)
        {
            map->getTwoList()->removeObjectAtIndex(i);
            CCLog("果然有从新组合的两格武将");
        }
    }
    
    count=map->getFourList()->count();
    for (int i=0; i<count; i++)
    {
        SGSuperPrinceBase *superBase=(SGSuperPrinceBase *)map->getFourList()->objectAtIndex(i);
        SGGridBase *baseGrid=superBase->getNumGrid(kleftdown);
        if ( baseGrid && baseGrid->getStype()==kattack)
        {
            map->getFourList()->removeObjectAtIndex(i);
            CCLog("果然有重新组合的四格武将");
        }
    }
}

#pragma mark 新新的优先级
ReturnIndex SGAIManager::scanAllCondition(SGBattleMap *bm)
{
    resetSbSkye(bm);
    ReturnIndex rt_index=AIFirstPriority::del1Get2(bm);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描成功：删除一个得到两个阵列的情况");
        return rt_index;
    }
     CCLog("$扫描失败：删除一个得到两个阵列的情况");
    rt_index=AIFirstPriority::del1Get1(bm);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描成功：删除一个得到一个阵列的情况");
        return rt_index;
    }
   CCLog("$扫描失败：删除一个得到一个阵列的情况");
     rt_index = AIFirstPriority::move1Get2(bm);
    if (rt_index.type!=rt_null)
    {
         CCLog("$####AI: 扫描移动一次形成4格2格，一格武将，两个阵列");
        return rt_index;
    }
    CCLog("$扫描失败：扫描移动一次形成4格2格，一格武将，两个阵列");
    
    
     
    //增加一个移动武将的判断
    CCLog("$扫描移动武将");
    rt_index=AIFirstPriority::moveWJ(bm);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描成功：移动武将");
        CCLog("$武将的INdex.i=%d,Index.j= %d",rt_index.from1.i,rt_index.from1.j);
        CCLog("$武将的TOINdex.i=%d,ToIndex.j= %d",rt_index.to1.i,rt_index.to1.j);
        return rt_index;
    }
     CCLog("$扫描移动武将失败");
    
    ReturnIndex rt_index_two;
    ReturnIndex rt_index_four;
    rt_index_four=AIFirstPriority::round1GetFour(bm);
    rt_index_two =AIFirstPriority::round1GetTwo(bm);//可优化，先不管
    
    SGHeroLayer *heroLayer=(SGHeroLayer *)bm->getParent();
    int currentOp=heroLayer->roundNum;
    rt_index=rt_index_four;
    
    if (rt_index_four.type!=rt_null && rt_index_two.type!=rt_null)
    {
        if (rt_index_four.needAction>currentOp && rt_index_two.needAction<=currentOp)
        {
            rt_index=rt_index_two;
        }
        
    }
    if (rt_index_four.type==rt_null)
    {
        rt_index =rt_index_two;
    }

    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描成功：扫描一个回合内组成武将");
        return rt_index;
    }
     CCLog("$扫描失败：扫描一个回合内组成武将");
    
 
    CCLog("$扫描移动武将");
    rt_index=AIFirstPriority::moveWJ(bm);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描成功：移动武将");
        CCLog("$武将的INdex.i=%d,Index.j= %d",rt_index.from1.i,rt_index.from1.j);
        CCLog("$武将的TOINdex.i=%d,ToIndex.j= %d",rt_index.to1.i,rt_index.to1.j);
        return rt_index;
    }
     CCLog("$扫描移动武将失败");
    rt_index=AIFirstPriority::fastMakeAttackForSB(bm, ksbfour);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描成功：最快组成四格武将");
        
        return rt_index;
    }
     CCLog("$扫描失败：最快移动四格武将");
    rt_index=AIFirstPriority::fastMakeAttackForSB(bm, ksbtwo);
    if (rt_index.type!=rt_null)
    {
         CCLog("$扫描成功：最快组成二格武将");
        return rt_index;
    }
      CCLog("$扫描失败：最快组成二格武将");
//    
    bool bu=SGAIBu::bu(bm);
    if ((bu && bm->twoList->count()>0) || (bu && bm->fourList->count()>0))
    {
        rt_index.type=rt_bu;
        return rt_index;
    }

    
    rt_index=SGAIManager::fastGetAttackForOne(bm);
    if (rt_index.type!=rt_null)
    {
        if (rt_index.type==rt_bu)
        {
            CCLog("$补兵");
        }
        else
            CCLog("$扫描成功：最快组成三格阵列");
        return rt_index;
    }
    CCLog("$扫描失败：补兵");
    return rt_index;
    
}
 
ReturnIndex SGAIManager::fastGetAttackForOne(SGBattleMap *bm){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index= AIFirstPriority::move1Get1(bm);
    if (rt_index.type!=rt_null)
    {
        
        CCLog("$移动一个得到一个阵列");
        return rt_index;
    }
    
    if(SGAIBu::bu(bm))
    {
        rt_index.type=rt_bu ;
        return rt_index;
    }
    
    rt_index= AIFirstPriority::makeFastForOne(bm);
    if (rt_index.type!=rt_null)
    {
        CCLog("$移动或删除多个得到一个阵列，枚举");
        return rt_index;
    }
    
    
    
    rt_index = AIFirstPriority::fastMake3(bm);
    if (rt_index.type!=rt_null)
    {
        CCLog("$移动或删除得到了，快速");
        return rt_index;
    }
    return rt_index;

}

void SGAIManager::actionWithReturnIndex(ReturnIndex rt_index,SGBattleMap *bm)
{
    switch (rt_index.type)
    {
        case rt_del1:
             bm->deleteSbObj(rt_index.delIndex1.i, rt_index.delIndex1.j);
            break;
        case rt_from1to1:
        {
                stepIndex.stepType = st_move1del0;
                stepIndex.to1 = rt_index.to1;
                bm->initTouchGrid(rt_index.from1.j);
        }
            break;
        case rt_null:
        {
            if (!startMainSkill(bm))
            {
                this->startFillUp(bm, s_giveup);
            }
        }
            break;
        case rt_bu:
        {
            if (!startMainSkill(bm))
            {
                this->startFillUp(bm, s_scanDel);
            }
        }
            break;
        default:
        {
           
            this->startFillUp(bm, s_giveup);
        }
        break;
    }
}


bool SGAIManager::startFillUp(SGBattleMap *bm,Step nextStep)
{
    SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
    if (nextStep == s_giveup)
    {
        bool bu = SGAIBu::bu(bm);
        if (bu)
        {
            nowStep = s_scanDel;
            CCLog("$#####开始补兵");
            heroLayer->fillUp();
            return true;
        }
        else
        {
            CCLog("$放弃回合");
            heroLayer->aiEndRound();
            
        }
        return false;
    }
    CCLog("$#####扫描是否补兵");
    bool bu = SGAIBu::bu(bm);
    if (bu)
    {
        nowStep = s_scanDel;
        CCLog("$####开始补兵");
        heroLayer->fillUp();
        return true;
    }
    
    CCLog("$###扫描失败");
    nowStep = nextStep;
    this->startRound(bm);
    return false;
}
bool SGAIManager::startMainSkill(SGBattleMap *bm)
{
    SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
    int round = heroLayer->getSkillsRoundNum();
    if (round == 0)
    {
        SGAIHero * sgaihero  =  SGAIHero::createSig();
        bool heroskill = sgaihero->startSkill(bm,heroLayer->m_skillsId);
        if (heroskill)
        {
            CCLog("$可以发动主将技能了");
            nowStep = s_scanDel;
            heroLayer->mainSkillAttackHandler();
            return true;
        }
    }
    return false;
}

void SGAIManager::startDel(SGBattleMap *bm)
{
    int delI = 0;
    int delJ = 0;
    
    bm->deleteSbObj(delI, delJ);
}

void SGAIManager::startMoveToScene(SGBattleMap *battleMap)
{

    battleMap->moveTouchGrid(stepIndex.to1.j);
}
void SGAIManager::startMoveOutScene(SGBattleMap *battleMap)
{
    nowStep = s_moveToScene;
    
    int j = stepIndex.from1.j;

    battleMap->initTouchGrid(j);
}
void SGAIManager::startGuideAI(SGBattleMap *bm)
{
    aiStep++;
    
    int sceneId =  SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();
    int storyId = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    SGRobotGuideVO *vo = NULL;
    if (SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
    {
        vo = SGStaticDataManager::shareStatic()->getCgRobotGuideModel(aiStep);
    }
    else
    {
       vo = SGStaticDataManager::shareStatic()->getRobotGuideModel(storyId,sceneId, aiStep);
    }
    if(!vo)
    {
        if (!(sceneId==2
              && ((storyId == 4 && (sceneId == 11 || sceneId == 10))
                    || (storyId == 6 && (sceneId == 17 || sceneId == 18 )))
              && aiStep>=1))
        {
            SGGuideManager::shareManager()->isGuide = false;
            this->startRound(bm);
        }
        else
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            heroLayer->aiEndRound();
            nowStep = s_scanDel;
            SGBattleManager::sharedBattle()->getBattleLayer()->setIsCanTouch(false);
            
            if (SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
            {
				CCLOG("5th guide [%s-%d]", __FUNCTION__, __LINE__);

               SGGuideManager::shareManager()->startAutoGuide(5);
            }
            else
            {
               SGGuideManager::shareManager()->startFightGuide(5);
            }
        }
    }
    else
    {
        CCPoint point = vo->getPoint();
        switch (vo->getType())
        {
            case 1:
            {
                bm->initTouchGrid(point.y);
            }
                break;
            case 2:
            {
                bm->moveTouchGrid(point.y);
            }
                break;
            case 3:
            {
                bm->deleteSbObj(point.x, point.y);
            }
                break;
            case 4:
            {
                SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
                heroLayer->aiEndRound();
            }
            default:
                break;
        }
    }
    
}

void SGAIManager::setNowStep(Step _nowStep)
{
    nowStep = _nowStep;
}

void SGAIManager::setIsActivateAI(bool isai)
{
    isActivateAI = isai;
}

#pragma mark ai接口
SuggestIndex SGAIManager::returnSuggestIndex(SGBattleMap *bm)
{
    SuggestIndex suggestIndex;
    suggestIndex.actionType=at_null;
    
    ReturnIndex rt_index=scanAllCondition(bm);
    if (rt_index.type==rt_from1to1)
    {
        suggestIndex.actionType=at_move;
        suggestIndex.fromIndex=rt_index.from1;
        suggestIndex.toIndex=rt_index.to1;
    }
    else if(rt_index.type==rt_del1)
    {
        suggestIndex.actionType=at_delete;
        suggestIndex.deleteIndex=rt_index.delIndex1;
    }
    else if(rt_index.type==rt_bu)
    {
        suggestIndex.actionType=at_bu;
    }
     
    return suggestIndex;
}

