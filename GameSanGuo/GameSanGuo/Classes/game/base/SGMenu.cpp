//
//  SGMenu.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-29.
//
//

#include "SGMenu.h"

SGMenu* SGMenu::create(CCRect rect)
{
    SGMenu *m = new SGMenu();
    m->_rect = rect;
    
    if(m && m->init())
    {
        m->autorelease();
        return m;
    }
    
    CC_SAFE_DELETE(m);
    return NULL;
}

bool SGMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    _canTouch = false;
    CCPoint pt = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    if(!_rect.containsPoint(pt))
    {
        return false;
    }
    
    CC_UNUSED_PARAM(event);
    if (m_eState != kCCMenuStateWaiting || ! m_bVisible || !isEnabled())
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    
    
    return false;
}

void SGMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    _canTouch = true;
    return;
//    CC_UNUSED_PARAM(event);
//    CCMenuItem *currentItem = this->itemForTouch(touch);
//    if (currentItem != m_pSelectedItem)
//    {
//        if (m_pSelectedItem)
//        {
//            m_pSelectedItem->unselected();
//        }
//        m_pSelectedItem = currentItem;
//        if (m_pSelectedItem)
//        {
//            m_pSelectedItem->selected();
//        }
//    }
}


void SGMenu::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
//    CC_UNUSED_PARAM(touch);
//    CC_UNUSED_PARAM(event);
    
    m_pSelectedItem->unselected();

    if (m_pSelectedItem &&!_canTouch)
    {
        m_pSelectedItem->activate();
    }
    
    m_eState = kCCMenuStateWaiting;
}
