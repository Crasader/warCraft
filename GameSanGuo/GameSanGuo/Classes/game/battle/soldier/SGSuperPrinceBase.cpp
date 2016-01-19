//
//  SGSuperPrinceBase.cpp
//  GameSanGuo
//  4格英雄
//  Created by wenlong w on 12-11-23.
//
//

#include "SGSuperPrinceBase.h"

SGSuperPrinceBase::SGSuperPrinceBase()
{
    
}

SGSuperPrinceBase:: ~SGSuperPrinceBase()
{

}

SGSuperPrinceBase *SGSuperPrinceBase::createWithData(SGSBObj *data, SGBattleMap *bm, bool isme, bool isSpine)
{
    SGSuperPrinceBase *superPrince = new SGSuperPrinceBase();
    if (superPrince && superPrince->initWithData(data, bm, isme, isSpine))
    {
        superPrince->autorelease();
        
        return superPrince;
    }
    CC_SAFE_DELETE(superPrince);
    return NULL;
}

void SGSuperPrinceBase::initGrids(GameIndex index, sgShiBingType sbType)
{
    
    for (int i = 0; i<2; i++)
    {
        for (int j = 0; j<2; j++)
        {
            SGGridBase *grid = SGGridBase::createWithType(knormal);
            
            grid->setIndex(gameIndex(index.i + i, index.j + j));
            grid->setSbType(sbType);
            grid->setSaveID(this->getSbId());
            grid->setSaveColorID(this->getSbColor());
            this->getPrinceGridArray()->addObject(grid);
            
            battleMap->initGrid(grid);
        }
    }
}

CCPoint SGSuperPrinceBase::getSuperSoldierPS()
{
    GameIndex index = getSbindex();
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j, this->ismy);
    topoint = ismy?ccpAdd(topoint, ccp(gridWidth * .5, - gridHeight)):ccpAdd(topoint, ccp(-gridWidth * .5, 0));

    return topoint;
}

void SGSuperPrinceBase::initSoldierPsn()
{
    CCPoint sgsoldierPos = getSuperSoldierPS();
    bool repair = SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap();
    if (repair) {
        this->setPosition(sgsoldierPos);
    }else{
        float needy = this->ismy?-this->getContentSize().height:screenheight + this->getContentSize().height;
        sgsoldierPos.y = needy;
        this->setPosition(sgsoldierPos);
    }
    
}

void SGSuperPrinceBase::moveToIndexCall(CCObject *obj,int speed, GameIndex index, SEL_CallFuncN selector)
{
//    this->stopAllActions();
//    this->setSbindex(index);
//    CCPoint topoint = getSuperSoldierPS();
//    this->setPosition(ccp(topoint.x, this->getPosition().y));
//    CCMoveTo *moveTo = CCMoveTo::create(0.5f, topoint);
//    CCCallFunc *call = CCCallFunc::create(obj, selector);
//    this->runAction(CCSequence::create( moveTo, call,NULL));
    
    this->stopAllActions();
    this->setSbindex(index);
    int nSpeed = 0;
    CCDelayTime* delay  = NULL;
    if(battleMap->m_isStartFillUpSoldier)
    {
        nSpeed = chubing_moveSpeed;
        delay = CCDelayTime::create((index.i- battleMap->m_nTheFormerPosI)* 0.2+index.j * 0.005);
    }
    else
    {
        nSpeed = moveSpeed;
        delay = CCDelayTime::create(index.i * 0.002+index.j * 0.005);
    }
    CCPoint topoint = getSuperSoldierPS();
    float s = fabs(this->getPosition().y - topoint.y);
    float t = s/GET_ALONE_NUM(nSpeed);
    int rand = ismy ? -1 : 1;
    float toy = (screenheight  * (ismy ? 0 : 1)+ this->getContentSize().height)  * rand;
    this->setPosition(ccp(topoint.x, toy));
    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
    CCCallFuncN *call = CCCallFuncN::create(obj, selector);
//    CCDelayTime *time = CCDelayTime::create(0.5);
    CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
    this->runAction(CCSequence::create( delay,moveTo, callND, call, NULL));

}

void SGSuperPrinceBase::moveToIndex(GameIndex index)
{
    this->stopAllActions();
    this->setSbindex(index);
    CCPoint topoint = getSuperSoldierPS();
    float s = fabs(this->getPosition().y - topoint.y);
    float t = s/GET_ALONE_NUM(moveSpeed);
    CCDelayTime* delay = CCDelayTime::create(index.i * 0.2+index.j * 0.005);
    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
    CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
    this->runAction(CCSequence::create(delay, moveTo, callND, NULL));
    
}

void SGSuperPrinceBase::swapGridMove(SGBattleMap *battleMap,GameIndex index, SEL_CallFunc selector)
{
    this->setSbindex(index);
    this->stopActionByTag(999);
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j, ismy);
    topoint = ismy?ccpAdd(topoint, ccp(gridWidth * .5, - gridHeight)):ccpAdd(topoint, ccp( -gridWidth * .5, 0));
    CCMoveTo *moveTo = CCMoveTo::create(0.25f, topoint);

    CCCallFunc *call = CCCallFunc::create(battleMap, selector);
    CCAction *act = CCSequence::create(moveTo, call, NULL);
    act->setTag(999);
    this->runAction(act);
    
}

void SGSuperPrinceBase::moveToSceneCall(CCObject *obj, SEL_CallFuncN selector)
{
    GameIndex index = getNumGrid(kleftup)->getIndex();
    if (obj)
    {
        moveToIndexCall(obj, moveSpeed, index, selector);
    }
    else
    {
        moveToIndex(index);
    }
    setBingZOrder();
}

void SGSuperPrinceBase::attackAnimation(CCObject *obj, SEL_CallFunc selector)
{
//    this->stopAllActions();
//    CCCallFunc *call = CCCallFunc::create(obj, selector);
//    this->runAction(CCSequence::create(call, NULL));
    this->stopAllActions();
    CCDelayTime *time = CCDelayTime::create(0.6);
    CCCallFunc *call = CCCallFunc::create(obj, selector);
    this->runAction(CCSequence::create( time, call, NULL));

}

bool SGSuperPrinceBase::isAtLeft(int indexj)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(this->getPrinceGridArray(), obj)
    {
        SGGridBase *nextGrid = (SGGridBase *)obj;
        if (indexj < nextGrid->getIndex().j)
        {
            return true;
        }
    }
    return false;
}

