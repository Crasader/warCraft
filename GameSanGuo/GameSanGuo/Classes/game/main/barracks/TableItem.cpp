//
//  TableItem.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-4.
//
//

#include "TableItem.h"

TableItem::TableItem()
:label(NULL)
{
    
}

bool TableItem::initWithSize(CCSize size)
{
	if (!SNSTableViewCellItem::init()) {
		return false;
	}
	CCSprite *face = CCSprite::create("Icon.png");
	this->addChild(face);
    
    label = CCLabelTTF::create("111", "Arial", 27);
    this->addChild(label);
	return true;
}

TableItem* TableItem::create(CCSize size)
{
	TableItem *instance = new TableItem();
	if (instance && instance->initWithSize(size)) {
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}