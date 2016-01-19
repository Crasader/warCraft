//
//  SGStoryBaseCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGStoryBaseCell.h"
#include "SGLayout.h"
#include "cocos-ext.h"
#include "ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 **  SGStoryBaseCell
 **/
SGStoryBaseCell::SGStoryBaseCell(void) : btn(NULL), spriteBackground(NULL)
{
    ;
}

SGStoryBaseCell::~SGStoryBaseCell(void)
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_storyLayer);
}

SGStoryBaseCell* SGStoryBaseCell::create()
{
    SGStoryBaseCell *baseCell = new SGStoryBaseCell();
    if (baseCell && baseCell->init()) {
        baseCell->autorelease();
        return baseCell;
    }
    CC_SAFE_DELETE(baseCell);
    return NULL;
}

bool SGStoryBaseCell::init()
{
    if (!SNSTableViewCellItem::init()) {
        return false;
    }

    CCSprite *spriteBorder = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    bgbgbg = spriteBorder;
    bgSize = spriteBorder->getContentSize();
    ////////
//   CCMenu* menu1=CCMenu::create();
//    menu1 -> setPosition(CCPointZero);
//    btn=NULL;
//    menu->addChild(btn);
//    this->addChild(menu);
    btn=SGButton::create("item_bigbg.png", NULL, this, NULL,CCPointZero,false,true);
    btn->setPosition(spriteBorder->getPosition());
    btn->setTag(ITEMBIGBGTAG);
    //menu1->addChild(btn);
//    this->addChild(menu1);
    ////////
    
    return true;
}

void SGStoryBaseCell::resetBgImg(int index)
{
    /*
    if (index%2 == 0) {
        spriteBackground->setDisplayFrame(CCSprite::create("login/server_GreenBG.png")->displayFrame());
    }else {
        spriteBackground->setDisplayFrame(CCSprite::create("login/server_BlueBG.png")->displayFrame());
    }
     */
}


/**
 **  SGStoryCardBaseCell
 **/
SGStoryCardBaseCell::SGStoryCardBaseCell(void) : spriteBorder(NULL)
{
    ;
}

SGStoryCardBaseCell::~SGStoryCardBaseCell(void)
{
    
}

SGStoryCardBaseCell* SGStoryCardBaseCell::create()
{
    SGStoryCardBaseCell *baseCell = new SGStoryCardBaseCell();
    if (baseCell && baseCell->init()) {
        baseCell->autorelease();
        return baseCell;
    }
    CC_SAFE_DELETE(baseCell);
    return NULL;
}

bool SGStoryCardBaseCell::init()
{
    if (!SNSTableViewCellItem::init()) {
        return false;
    }
	ResourceManager::sharedInstance()->bindTexture("SGStrengLayer/SGStrengLayer.plist", RES_TYPE_LAYER_UI, sg_storyLayer);

    spriteBorder = CCScale9Sprite::createWithSpriteFrameName("item_smallbg.png");
    spriteBorder->setPreferredSize(spriteBorder->getContentSize());
    this->addChild(spriteBorder, -10);
    spriteBorder->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(skewing(93/2)/2, 0)));
    bgbgbg = (CCSprite*)spriteBorder;
    bgSize = spriteBorder->getContentSize();
    
    return true;
}

