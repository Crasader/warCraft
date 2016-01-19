//
//  SGTouchLayer.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/12/13.
//
//

#include "SGTouchLayer.h"

//构造
SGTouchLayer::SGTouchLayer() 
{
    m_isWorking = false;
    m_targetObj = NULL;
    m_mode = SGTM_BUTTON;
    m_state = SGTS_NULL;
    m_pfCallBack = NULL;
    m_pfBegan = NULL;
    m_pfMoved = NULL;
    m_pfEnded = NULL;
    m_pfCancelled = NULL;
    m_priority = 0;
}

//析构
SGTouchLayer::~SGTouchLayer()
{
    
}

//创建。 size：本layer的触控范围，即ContentSize。  prior：触控级别，默认-100。
SGTouchLayer* SGTouchLayer::create(float width, float height, int prior, ccColor4B color)
{
    SGTouchLayer* player = new SGTouchLayer();
    if (player && player->initWithColor(color, width, height))
    {
        player->setPriority(prior);
        player->ignoreAnchorPointForPosition(false);
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return NULL;
}

//进场，注册触控。
void SGTouchLayer::onEnter()
{
    CCLayerColor::onEnter();
    if (m_isWorking)
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, true);
}

//退场，移除触控。
void SGTouchLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

//①变身为按钮形态，注册回调函数。
void SGTouchLayer::setButtonMode(CCObject* obj, PFCallBack pf)
{
    m_targetObj = obj;
    m_pfCallBack = pf;
    m_mode = SGTM_BUTTON;
    m_isWorking = true;
}

//②变身为触控形态，注册四种触控函数。
void SGTouchLayer::setControlMode(CCObject* obj, PFBegan began, PFMoved moved, PFEnded ended, PFCancelled cancelled)
{
    m_targetObj = obj;
    m_pfBegan = began;
    m_pfMoved = moved;
    m_pfEnded = ended;
    m_pfCancelled = cancelled;
    m_mode = SGTM_CONTROL;
    m_isWorking = true;
}

//began
bool SGTouchLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isWorking)
        return false;
    
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    
    //可见，并且无状态
    if (this->isVisible() && this->m_state == SGTS_NULL)
    {
        CCPoint touchLocation = pTouch->getLocation();
        CCPoint local = this->convertToNodeSpace(touchLocation);
        CCRect rect = CCRectMake( m_obPosition.x - m_obContentSize.width * m_obAnchorPoint.x,
                                 m_obPosition.y - m_obContentSize.height * m_obAnchorPoint.y,
                                 m_obContentSize.width, m_obContentSize.height);
        rect.origin = CCPointZero;
        //点击在控件内
        if (rect.containsPoint(local))
        {
            if (m_mode == SGTM_BUTTON) //按钮形态
            {
                m_state = SGTS_TOUCHED;
                return true;
            }
            else if (m_mode == SGTM_CONTROL) //自控形态
            {
                if (m_targetObj && m_pfBegan)
                {
                    bool ret = (m_targetObj->*m_pfBegan) (pTouch, pEvent);
                    if (ret)
                        m_state = SGTS_TOUCHED;
                    return ret;
                }
            }
            else
                return false;
        }
    }
    return false;
};

//moved
void SGTouchLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isWorking)
        return;
    
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    
    //可见，为按下状态
    if (this->isVisible() && m_state == SGTS_TOUCHED)
    {
        if (m_mode == SGTM_BUTTON) //按钮形态
        {
            ;
        }
        else if (m_mode == SGTM_CONTROL) //自控形态
        {
            if (m_targetObj && m_pfMoved)
                (m_targetObj->*m_pfMoved) (pTouch, pEvent);
        }
        else
        {
            ;
        }
    }
}

//ended
void SGTouchLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isWorking)
        return;
    
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    
    //可见，为按下状态
    if (this->isVisible() && m_state == SGTS_TOUCHED)
    {
        CCPoint touchLocation = pTouch->getLocation();
        if (m_mode == SGTM_BUTTON) //按钮形态
        {
            CCPoint local = this->convertToNodeSpace(touchLocation);
            CCRect rect = CCRectMake( m_obPosition.x - m_obContentSize.width * m_obAnchorPoint.x,
                                     m_obPosition.y - m_obContentSize.height * m_obAnchorPoint.y,
                                     m_obContentSize.width, m_obContentSize.height);
            rect.origin = CCPointZero;
            //点击在控件内，触发事件
            if (rect.containsPoint(local))
            {
                if (m_targetObj && m_pfCallBack)
                    (m_targetObj->*m_pfCallBack) ();
            }
        }
        else if (m_mode == SGTM_CONTROL) //自控形态
        {
            if (m_targetObj && m_pfEnded)
                (m_targetObj->*m_pfEnded) (pTouch, pEvent);
        }
    }
    
    m_state = SGTS_NULL;
}

//cancelled
void SGTouchLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isWorking)
        return;
    
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    
    //可见，为按下状态
    if (this->isVisible() && m_state == SGTS_TOUCHED)
    {
        if (m_mode == SGTM_BUTTON) //按钮形态
        {
            ;
        }
        else if (m_mode == SGTM_CONTROL) //自控形态
        {
            if (m_targetObj && m_pfCancelled)
                (m_targetObj->*m_pfCancelled) (pTouch, pEvent);
        }
    }
    
    m_state = SGTS_NULL;
}
