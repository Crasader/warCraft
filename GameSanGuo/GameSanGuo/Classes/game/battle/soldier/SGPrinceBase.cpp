//
//  SGPrinceBase.cpp
//  GameSanGuo
//
//  Created by wenlong w on 12-11-23.
//
//

#include "SGPrinceBase.h"

SGPrinceBase::SGPrinceBase()
{
    ;
}

SGPrinceBase::~SGPrinceBase()
{
    ;
}

SGPrinceBase *SGPrinceBase::createWithData(SGSBObj *data,SGBattleMap *bm, bool isme, bool isSpine)
{
    SGPrinceBase *prince = new SGPrinceBase();
    if (prince && prince->initWithData(data,bm, isme, isSpine))
    {

        prince->autorelease();
        
        return prince;
    }
    CC_SAFE_DELETE(prince);
    return NULL;
}
void SGPrinceBase::initGrids(GameIndex index,sgShiBingType sbType)
{
    
    for (int i = 0; i<2; i++)
    {
        SGGridBase *grid = SGGridBase::createWithType(knormal);
    
        grid->setIndex(gameIndex(index.i + i, index.j));
        grid->setSbType(sbType);
        grid->setSaveID(this->getSbId());
        grid->setSaveColorID(this->getSbColor());
        this->getPrinceGridArray()->addObject(grid);
    
        battleMap->initGrid(grid);
    }
}
void SGPrinceBase::moveToIndexCall(CCObject *obj, int speed, GameIndex index, SEL_CallFuncN selector)
{
    
    this->stopAllActions();
//    this->setSbindex(index);
//    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j,this->ismy);
//    this->setPosition(ccp(topoint.x, this->getPosition().y));
//    CCMoveTo *moveTo = CCMoveTo::create(0.5f, topoint);
//    CCCallFunc *call = CCCallFunc::create(obj, selector);
//    this->runAction(CCSequence::create( moveTo, call,NULL));
    
//    this->setOpacity(255);
    
    this->setSbindex(index);
    int nSpeed = 0;
    CCDelayTime* delay  = NULL;
    if(battleMap->m_isStartFillUpSoldier)
    {
        nSpeed = chubing_moveSpeed;
        delay = CCDelayTime::create((index.i-battleMap->m_nTheFormerPosI ) * 0.2+index.j * 0.005);
    }
    else
    {
        nSpeed = moveSpeed;
        delay = CCDelayTime::create(index.i * 0.002+index.j * 0.005);
    }
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j,this->ismy);
    topoint = ismy?ccpAdd(topoint, ccp(0,- gridHeight)):ccpAdd(topoint, ccp(0, 0));
    float s = fabs(this->getPosition().y - topoint.y);
    float t = s/ GET_ALONE_NUM(nSpeed);
    
    int rand = ismy ? -1 : 1;
    float toy = (screenheight  * (ismy ? 0 : 1)+ this->getContentSize().height)  * rand;
    this->setPosition(ccp(topoint.x, toy));
    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
    CCCallFuncN *call = CCCallFuncN::create(obj, selector);
//    CCDelayTime *time = CCDelayTime::create(0.5);
    CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
    this->runAction(CCSequence::create(delay, moveTo, callND, call, NULL));
    
    
}
void SGPrinceBase::moveToIndex(GameIndex index)
{
    this->stopAllActions();
    this->setSbindex(index);
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j,this->ismy);
    topoint = ismy?ccpAdd(topoint, ccp(0,- gridHeight)):ccpAdd(topoint, ccp(0, 0));
    CCDelayTime* delay = CCDelayTime::create(index.i * 0.05+index.j * 0.005);
    CCMoveTo *moveTo = CCMoveTo::create(0.5f, topoint);
    CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
    this->runAction(CCSequence::create( delay, moveTo, callND, NULL));
    
    
}
void SGPrinceBase::swapGridMove(SGBattleMap *battleMap,GameIndex index, SEL_CallFunc selector)
{
    this->setSbindex(index);
    this->stopActionByTag(999);
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j, ismy);
    topoint = ismy?ccpAdd(topoint, ccp(0,- gridHeight)):ccpAdd(topoint, ccp(0, 0));
//    float s = fabs(this->getPosition().y - topoint.y);
//    float t = s/GET_ALONE_NUM(moveSpeed);
//    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
//    CCDelayTime *time = CCDelayTime::create(0.25);
//    this->runAction(CCSequence::create(moveTo, time,  NULL));

//    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j, ismy);
//    topoint = ismy?ccpAdd(topoint, ccp(gridWidth * .5, - gridHeight * .5)):ccpAdd(topoint, ccp(gridWidth * .5,  gridHeight * .5));
    CCMoveTo *moveTo = CCMoveTo::create(0.25f, topoint);
    CCCallFunc *call = CCCallFunc::create(battleMap, selector);
    CCAction *act = CCSequence::create(moveTo, call, NULL);
    act->setTag(999);
    this->runAction(act);
    
    
}
void SGPrinceBase::moveToSceneCall(CCObject *obj, SEL_CallFuncN selector)
{
    
    GameIndex index = getNumGrid(kup)->getIndex();
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

void SGPrinceBase::attackAnimation(CCObject *obj, SEL_CallFunc selector)
{
//    this->stopAllActions();
//    CCCallFunc *call = CCCallFunc::create(obj, selector);
//    this->runAction(CCSequence::create(call, NULL));
    
    
    this->stopAllActions();
    CCDelayTime *time = CCDelayTime::create(0.6);
    CCCallFunc *call = CCCallFunc::create(obj, selector);
    this->runAction(CCSequence::create( time, call, NULL));
    
    
}

SGGridBase* SGPrinceBase::getNumGrid(sgGridPsnType index)
{
    
    SGGridBase *grid = (SGGridBase *)this->getPrinceGridArray()->objectAtIndex(0);
    if (!grid) {
        return NULL;
    }
    int indexI = grid->getIndex().i;
    int indexJ = grid->getIndex().j;
    
    switch (index) {
        case kup:
        case kleftup://左上
        {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(this->getPrinceGridArray(), obj)
            {
                SGGridBase *nextGrid = (SGGridBase *)obj;
                if (nextGrid && nextGrid->getIndex().i<indexI)
                {
                    indexI = nextGrid->getIndex().i;
                }
                if (nextGrid && nextGrid->getIndex().j < indexJ)
                {
                    indexJ = nextGrid->getIndex().j;
                }
            }
        }
            break;
        case krightup:
        {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(this->getPrinceGridArray(), obj)
            {
                SGGridBase *nextGrid = (SGGridBase *)obj;
                if (nextGrid && nextGrid->getIndex().i<indexI)
                {
                    indexI = nextGrid->getIndex().i;
                }
                if (nextGrid && nextGrid->getIndex().j > indexJ)
                {
                    indexJ = nextGrid->getIndex().j;
                }
            }
        }
            break;
        case kleftdown:
        {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(this->getPrinceGridArray(), obj)
            {
                SGGridBase *nextGrid = (SGGridBase *)obj;
                if (nextGrid && nextGrid->getIndex().i > indexI)
                {
                    indexI = nextGrid->getIndex().i;
                }
                if (nextGrid && nextGrid->getIndex().j < indexJ)
                {
                    indexJ = nextGrid->getIndex().j;
                }
            }
        }
            break;
        case kdown:
        case krightdown:
        {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(this->getPrinceGridArray(), obj)
            {
                SGGridBase *nextGrid = (SGGridBase *)obj;
                if (nextGrid && nextGrid->getIndex().i > indexI)
                {
                    indexI = nextGrid->getIndex().i;
                }
                if (nextGrid && nextGrid->getIndex().j > indexJ)
                {
                    indexJ = nextGrid->getIndex().j;
                }
            }
        }
            break;
        default:
            break;
    }
    CCObject *obj = NULL;
    CCARRAY_FOREACH(this->getPrinceGridArray(), obj)
    {
        SGGridBase *nextGrid = (SGGridBase *)obj;
        if (nextGrid->getIndex().i == indexI && nextGrid->getIndex().j == indexJ)
        {
            grid = nextGrid;
            break;
        }
    }
    return grid;
}