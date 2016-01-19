//
//  SNSTableViewCellItem.cpp
//  tableView
//	version 2.0
//  Created by Jie Yang on 12/7/11.
//  Copyright 2011 ourpalm.com. All rights reserved.
//

#include "SNSTableViewCellItem.h"
#include "SNSButton.h"

bool SNSTableViewCellItem::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    btns = CCArray::create();
    btns->retain();
    
    return true;
}

SNSTableViewCellItem* SNSTableViewCellItem::create()
{
	SNSTableViewCellItem* pRet = new SNSTableViewCellItem();
    
    if ( pRet && pRet->init() ) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void SNSTableViewCellItem::addBtn(cocos2d::CCNode *node)
{
    btns->addObject(node);
}

CCArray *SNSTableViewCellItem::getAllBtn()
{
    return btns;
}