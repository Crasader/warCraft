//
//  SGBattleFieldBaseItem.cpp
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#include "SGBattleFieldBaseItem.h"
#include "ResourceManager.h"

SGBattleFieldBaseItem::SGBattleFieldBaseItem()
:bgSize(CCSize(0.0f, 0.0f))
{
    
}

SGBattleFieldBaseItem::~SGBattleFieldBaseItem()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleFieldLayer);
}


SGBattleFieldBaseItem* SGBattleFieldBaseItem::create()
{
    SGBattleFieldBaseItem *baseItem = new SGBattleFieldBaseItem();
    if (baseItem && baseItem->init()) {
        baseItem->autorelease();
        return baseItem;
    }
    CC_SAFE_DELETE(baseItem);
    return NULL;
}

bool SGBattleFieldBaseItem::init()
{
    if (!SNSTableViewCellItem::init()) {
        return false;
    }
    CCSprite *spriteBorder = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    this->addChild(spriteBorder, -10);
    bgbgbg = NULL;
    bgSize = spriteBorder->getContentSize();
    
    return true;
}


