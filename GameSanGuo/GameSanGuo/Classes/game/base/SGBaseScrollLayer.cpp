//
//  SGBaseScrollLayer.cpp
//  GameSanGuo
//  暂只用作武将展示界面，名字有BASE，但不是基类。
//
//  Created by Fygo Woo on 11/6/13.
//
//

#include "SGBaseScrollLayer.h"
#include "SGPlayerInfo.h"
#include "SGGeneralInfoLayer.h"
#include "SGLayout.h"

//移动超过多少则切换
#define SLIDE_CHANGE_DISTANCE 200
//移动超过多少则开始移动
#define SLIDE_LIMIT 10
//调整位置动作速度
#define SLIDE_SPEED 800.0
//多少秒后清除无用的节点
#define TIME_TO_KILL 2


/**
 * 创建
 */
SGBaseScrollLayer::SGBaseScrollLayer() : m_leftX(0), m_middleX(0), m_rightX(0), m_leftPos(CCSizeZero), m_middlePos(CCSizeZero), m_rightPos(CCSizeZero)
{
    cleanParams();
    m_index = -1;
}

/**
 * 析构
 */
SGBaseScrollLayer::~SGBaseScrollLayer()
{
    removeAllChildrenWithCleanup(true);
    cleanParams();
}

/**
 * 创建一个武将详情列表
 */
SGBaseScrollLayer* SGBaseScrollLayer::create(SGGeneralInfoLayer *sgil)
{
    SGBaseScrollLayer* sgbsl = new SGBaseScrollLayer();
    if (sgbsl && sgbsl->init(NULL, sg_generalInfoScrollLayer) && sgbsl->initView(sgil))
    {
        sgbsl->autorelease();
        return sgbsl;
    }
    CC_SAFE_DELETE(sgbsl);
    return NULL;
}

/**
 * 添加layer，并判定初始化。
 */
bool SGBaseScrollLayer::initView(SGGeneralInfoLayer* sgbl)
{
//    SGPlayerInfo* sgpi = SGPlayerInfo::sharePlayerInfo();
    cleanParams();
    //背包武将数组
    CCArray* arr = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    CCObject* pObj = NULL;
    SGOfficerCard* sgoc = NULL;
    int cur = 0;
    //int max = arr->count();
    CCARRAY_FOREACH(arr, pObj)
    {
        sgoc = (SGOfficerCard*) pObj;
        if (sgoc != NULL)
        {
            if (sgoc->getSsid() == sgbl->_card->getSsid()) //如果与添加的武将ID相等，则为主要展示
            {
                m_index = cur;
                break;
            }
        }
        cur++;
    }
    //如果没找到，出错返回
    if (-1 == m_index)
    {
        CCLOG("-----打开武将详情出错-----");
        return false;
    }
    
    //获得子页面标准位置
//    m_middlePos = ccp(screenwidth * 0.5, screenheight * 0.5);
    m_middlePos = ccp(0, 0);
    m_leftPos = ccpAdd(m_middlePos, ccp(-screenwidth, 0));
    m_rightPos = ccpAdd(m_middlePos, ccp(screenwidth, 0));
    
    //设置当前展示武将
    m_middleObj = sgbl;
    //特殊情况：来自朋友的武将信息，则只显示一个。
    if (sgbl->isFriendGeneral)
    {
        CCLOG(">>>>武将详情：朋友的武将----");
        m_leftObj = NULL;
        m_rightObj = NULL;
    }
    else
    {
        CCLOG(">>>>武将详情：自己的武将----");
        //设置左右武将
        int left = m_index - 1;
        int right = m_index + 1;
        m_leftObj = generatedData(m_middleObj, left);
        m_rightObj = generatedData(m_middleObj, right);
    }
    
    //调整坐标
    
    if (m_middleObj != NULL)
    {
        m_middleObj->setPosition(m_middlePos);
        this->addChild(m_middleObj, m_index);
    }
    if (m_leftObj != NULL)
    {
        m_leftObj->setPosition(m_leftPos);
        this->addChild(m_leftObj, m_index - 1);
    }
    if (m_rightObj != NULL)
    {
        m_rightObj->setPosition(m_rightPos);
        this->addChild(m_rightObj, m_index + 1);
    }
    return true;
}

/**
 * 生成一个武将详情LAYER
 */
SGGeneralInfoLayer* SGBaseScrollLayer::generatedData(SGGeneralInfoLayer* midLayer, int index)
{
    CCArray* arr = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    if (index < 0 || index > arr->count())
        return NULL;
    SGOfficerCard* sgoc = (SGOfficerCard*) arr->objectAtIndex(index);
    if (sgoc == NULL)
        return NULL;
//    SGGeneralInfoLayer::create(SGOfficerCard *card, int type,SGOfficerCard *officer,bool isfirst,CCDictionary *dic,bool isFriendGeneral)
    SGGeneralInfoLayer* retLayer = SGGeneralInfoLayer::create(sgoc, midLayer->enterType, NULL, midLayer->_before, midLayer->isfromfirst, midLayer->Direc, midLayer->isFriendGeneral);
    return retLayer;
}


/**
 * 移动layer之后的事件回调
 */
void SGBaseScrollLayer::handleEvent()
{
    CCLOG(">>>>武将详情handleEvent");
}

/**
 * 移动layer
 */
void SGBaseScrollLayer::moveItemLayer()
{
    CCLOG(">>>>武将详情moveItemLayer");
    if (m_isTouched && m_canMove)
    {
        //预制滑动状态
        int type = 0;
        //超过此距离可以发起转换移除
        if (m_dist > SLIDE_CHANGE_DISTANCE) //向右移动，则向左展现
        {
            SGGeneralInfoLayer* temp = generatedData(m_middleObj, m_index - 1);
            if (temp) //如果NULL则表示无法向左展现
            {
//                //隐藏，延迟删除
//                CCAction *action = CCSequence::create(CCHide::create(), CCDelayTime::create(TIME_TO_KILL), CCCallFuncND::create(m_rightObj, callfuncND_selector(SGGeneralInfoLayer::removeFromParentAndCleanup), (void*) SGBaseScrollLayer::removeBool), NULL);
//                m_rightObj->runAction(action);
                m_rightObj->removeFromParentAndCleanup(true);
                m_rightObj = m_middleObj;
                m_middleObj = m_leftObj;
                m_leftObj = temp;
                m_leftObj->setPosition(m_leftPos);
                this->addChild(m_leftObj, m_index - 1);
                m_index -= 1;
                type = -1;
            }
        }
        else if (m_dist < -SLIDE_CHANGE_DISTANCE) //向左移动，则向右展现
        {
            SGGeneralInfoLayer* temp = generatedData(m_middleObj, m_index + 1);
            if (temp) //如果NULL则表示无法向左展现
            {
//                //隐藏，延迟删除
//                CCAction *action = CCSequence::create(CCHide::create(), CCDelayTime::create(TIME_TO_KILL), CCCallFuncND::create(m_rightObj, callfuncND_selector(SGGeneralInfoLayer::removeFromParentAndCleanup), (void*) SGBaseScrollLayer::removeBool), NULL);
//                m_leftObj->runAction(action);
                m_leftObj->removeFromParentAndCleanup(true);
                m_leftObj = m_middleObj;
                m_middleObj = m_rightObj;
                m_rightObj = temp;
                m_rightObj->setPosition(m_rightPos);
                this->addChild(m_rightObj, m_index + 1);
                m_index += 1;
                type = 1;
            }
        }
        else //回归原位，不用动。
        {
            ;
        }
        //设置归位动画
        adjustPosition(type);
    }
    m_isTouched = m_canMove = false;
}

/**
 * 使用动作调整位置。-1=左不调整，0=全部调整，1=右不调整。
 */
void SGBaseScrollLayer::adjustPosition(int type)
{
    m_canTouched = false;
    float time = abs(m_dist) / SLIDE_SPEED;
    if (m_leftObj && type != -1)
        m_leftObj->runAction( CCMoveTo::create( time, m_leftPos) );
    if (m_middleObj)
        m_middleObj->runAction( CCMoveTo::create( time, m_middlePos) );
    if (m_rightObj && type != 1)
        m_rightObj->runAction( CCMoveTo::create( time, m_rightPos) );
    //动画完毕后0.2秒才允许再触控，否则可能杯具
    CCSequence* action = CCSequence::create(CCDelayTime::create(time + 0.2), CCCallFunc::create(this, callfunc_selector(SGBaseScrollLayer::resetCanTouched)));
    this->runAction(action);
}

/**
 * 清理一些数据成员。
 */
void SGBaseScrollLayer::cleanParams()
{
    m_middleObj = m_leftObj = m_rightObj = NULL;
    m_index = -1;
    m_canMove = false;
    m_isTouched = false;
    m_canTouched = true;
    m_beganX= 0;
    m_dist = 0;
}

/**
 * 重置LAYER使能够被触控。
 */
void SGBaseScrollLayer::resetCanTouched()
{
    m_canTouched = true;
}

/**
 * 进场，注册触控。
 */
void SGBaseScrollLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, false);
}

/**
 * 退场，移除触控。
 */
void SGBaseScrollLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    SGBaseLayer::onExit();
}


bool SGBaseScrollLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_canTouched)
        return false;
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCLOG(">>>>武将详情ccTouchBegan");
    m_isTouched = true;
    m_beganX = pTouch->getLocation().x;
    if (m_leftObj)
        m_leftX = m_leftObj->getPosition().x;
    if (m_middleObj)
        m_middleX = m_middleObj->getPosition().x;
    if (m_rightObj)
        m_rightX = m_rightObj->getPosition().x;
    return true;
}
// optional

void SGBaseScrollLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isTouched || !m_canTouched)
        return;
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCLOG(">>>>武将详情ccTouchMoved");
    CCPoint tp = pTouch->getLocation();
    m_dist = (tp.x - m_beganX);
    //X向移动距离>10，切未在移动中，则允许移动
    if (abs(m_dist) >= SLIDE_LIMIT && !m_canMove)
    {
        m_canMove = true;
    }
    if (m_canMove)
    {
        if (m_leftObj)
            m_leftObj->setPositionX(m_leftX + m_dist);
        if (m_middleObj)
            m_middleObj->setPositionX(m_middleX + m_dist);
        if (m_rightObj)
            m_rightObj->setPositionX(m_rightX + m_dist);
    }
}

void SGBaseScrollLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_canTouched)
        return;
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCLOG(">>>>武将详情ccTouchEnded");
    moveItemLayer();
}

void SGBaseScrollLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_canTouched)
        return;
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCLOG(">>>>武将详情ccTouchCancelled");
    moveItemLayer();
}