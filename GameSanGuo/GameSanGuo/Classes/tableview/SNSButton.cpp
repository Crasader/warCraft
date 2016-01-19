//
//  SNSUIButton.cpp
//  tableView
//	version 2.0
//  Created by yang jie on 28/11/2011.
//  Copyright 2011 ourpalm.com. All rights reserved.
//

#include "SNSButton.h"
#include "SNSTableView.h"

#define MOVE_PIXEL (10) //移动超过这个像素的点 不触发按钮事件

SNSButton* SNSButton::menuWithItems(CCMenuItem* item, ...)
{
	va_list args;
    va_start(args,item);
    SNSButton *pRet = new SNSButton();
    if (pRet && pRet->menuWithItems(item, args))
    {
        pRet->autorelease();
        va_end(args);
        return pRet;
    }
    va_end(args);
    CC_SAFE_DELETE(pRet);
    return NULL;
}

SNSButton* SNSButton::menuWithItem(CCMenuItem* item)
{
    return create(item, NULL);
}

SNSButton* SNSButton::create(CCMenuItem *item, ...)
{
	va_list args;
    va_start(args,item);
    SNSButton *pRet = new SNSButton();
    if (pRet && pRet->menuWithItems(item, args))
    {
        pRet->autorelease();
        va_end(args);
        return pRet;
    }
    va_end(args);
    CC_SAFE_DELETE(pRet);
    return NULL;
}

SNSButton* SNSButton::create(CCMenuItem *item)
{
	return create(item, NULL);
}

void SNSButton::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

bool SNSButton::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    m_noEnded = false;
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
	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
		SNSTableView* tableView = dynamic_cast<SNSTableView*>(c);
        if (tableView) {
			// 找到tableView
			bool isTouchInRect = tableView->getTouchEffectiveArea().containsPoint(touch->getLocation());
			// 必须是在剪裁区域内，且点击到了指定的item
            if (isTouchInRect && m_pSelectedItem) {
				m_eState = kCCMenuStateTrackingTouch;
				m_pSelectedItem->selected();
				return true;
			} else {
				return false;
			}
        }
    }
	
    if (m_pSelectedItem)
    {
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    return false;
}

void SNSButton::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if ( !m_noEnded && ccpDistance(touch->getLocation(), touch->getPreviousLocation()) > MOVE_PIXEL ) {
        m_noEnded = true;
    }
    
    CCMenu::ccTouchMoved(touch, event);
}

void SNSButton::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if ( !m_noEnded ) {
		CCMenu::ccTouchEnded(touch, event);
    } else {
        ccTouchCancelled(touch, event);
    }
}

void SNSButton::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    m_noEnded = false;
	CCMenu::ccTouchCancelled(touch, event);
}
