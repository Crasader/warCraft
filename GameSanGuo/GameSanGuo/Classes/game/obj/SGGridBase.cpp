//
//  SGGridBase.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-19.
//
//

#include "SGGridBase.h"
#include "SG_BuffBase.h"

//CCPoint SGGridBase::getGridPoint(int i, int j)
//{
//    CCPoint point = CCPointZero;
//    point.x = ((screenwidth - (gridWidth * mapList)) * 0.5) + (gridWidth *(j+1)) - (gridWidth * 0.5);
//    point.y = (screenheight * 0.5) - ( gridHeight * (i + 1));
//    return point;
//}
SGGridBase *SGGridBase::createWithType(sgGridType type)
{
    SGGridBase *grid = new SGGridBase();
    if (grid && grid->initWithType(type))
    {
        grid->autorelease();
        return grid;
    }
    CC_SAFE_DELETE(grid);
    return NULL;
}
bool SGGridBase::initWithType(sgGridType type)
{
    bool bRet = false;
    do
    {
        this->setStype(type);
        
        bRet = true;
    }while (0);
    return bRet;
}
//SGGridBase *SGGridBase::createWithSoldier(SGSoldierBase *soldier)
//{
//    SGGridBase *grid = new SGGridBase();
//    if (grid && grid->initWithSoldier(soldier))
//    {
//        grid->autorelease();
//        return grid;
//    }
//    CC_SAFE_DELETE(grid);
//    return NULL;
//}
//bool SGGridBase::initWithSoldier(SGSoldierBase *soldier)
//{
//    bool bRet = false;
//    do
//    {
//        this->setSgSoldier(soldier);
//        bRet = true;
//    }while (0);
//    return bRet;
//    
//    
//}


SGGridBase::SGGridBase()
:m_baffObj(NULL)
, m_saveID(0)
, m_saveColorID(0)
{

}

SGGridBase::~SGGridBase()
{
//    CCLOG("~SGGridBase");
    CC_SAFE_RELEASE(m_baffObj);
}

//SGGridBase *SGGridBase::createWithSoldier(SGSoldierBase *soldier)
//{
//    SGGridBase *grid = new SGGridBase();
//    if (grid && grid->initWithSoldier(soldier))
//    {
//        grid->autorelease();
//        return grid;
//    }
//    CC_SAFE_DELETE(grid);
//    return NULL;
//}
//bool SGGridBase::initWithSoldier(SGSoldierBase *soldier)
//{
//    bool bRet = false;
//    do
//    {
//        this->setSgSoldier(soldier);
////        this->setSid(soldier->getSid());
////        this->setStype(soldier->getStype());
//        bRet = true;
//    }while (0);
//    return bRet;
//    
//    
//}

sgGridType SGGridBase::getStype()
{
    return sType;
}
void SGGridBase::setStype(sgGridType type)
{
    sType = type;
//    switch (sType) {
//        case kdefend:
//        {
//            this->getSgSoldier()->setColor(ccBLACK);
//        }
//            break;
//        case kattack:
//        {
////            this->getSgSoldier()->setColor(ccBLACK);
//        }
//            break;
//        default:
//            break;
//    }
}
void SGGridBase::moveToIndex(GameIndex index)
{
}
void SGGridBase::moveToBottom()
{
    
}
void SGGridBase::moveToPos(SGBattleMap *battleMap,CCPoint moveToPos)
{
    
}

void *SGGridBase::memmove(void *dest, const void *src, size_t count)
{
    assert((src != NULL)&&(dest != NULL));
    char *tmp, *s;
    if (dest <= src)
    {
        tmp = (char *) dest;
        s = (char *) src;
        while (count--)
        *tmp++ = *s++;    
    
    }
    else {
        tmp = (char *) dest + count;
        s = (char *) src + count;
        while (count--) 
         *--tmp = *--s; 
    }
    return dest;
}

void SGGridBase::initBaffObj()
{
    m_baffObj = CCArray::create();
    m_baffObj->retain();
}

CCArray* SGGridBase::getBuffList()
{
    return m_baffObj;
}

SG_BuffBase* SGGridBase::getIndexBuff(int rand)
{
    SG_BuffBase* buff = (SG_BuffBase*)m_baffObj->objectAtIndex(rand);
    return buff;
}

bool SGGridBase::setBuffListObj(SG_BuffBase* buff)
{
    bool isAdd = true;
    bool isInit = false;
    
    if (this->getBuffList()->count()) {
        for (int i = 0; i < this->m_baffObj->count(); i++) {
            SG_BuffBase *sgbuff = this->getIndexBuff(i);
            if (buff->m_buffType == sgbuff->m_buffType) {
                isAdd = false;
                
                sgbuff->setRoundNum(sgbuff->getRoundNum() + buff->getRoundNum());
                sgbuff->setAfftributeNum(sgbuff->getAfftributeNum() + buff->getAfftributeNum());
            }
        }
    }else{
        isInit = true;
        this->getBuffList()->addObject(buff);
    }
    
    if (!isInit &&
        isAdd) {
        this->getBuffList()->addObject(buff);
    }
    return isAdd;
}


