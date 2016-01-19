//
//  SGBattleFieldItem.cpp
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#include "SGBattleFieldItem.h"
#include "SNSButton.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"


SGBattleFieldItem::SGBattleFieldItem()
:data(NULL),
delegate(NULL)
{
    
}

SGBattleFieldItem::~SGBattleFieldItem()
{
    //ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleFieldLayer);
}

SGBattleFieldItem* SGBattleFieldItem::create(SGStorySectionDelegate *_delegate, SGBattleFieldData *_data)
{
    
    SGBattleFieldItem *fieldItem = new SGBattleFieldItem();
    if (fieldItem &&
        fieldItem->init()) {
        fieldItem->data = _data;
        fieldItem->delegate = _delegate;
        fieldItem->initItem();
        fieldItem->autorelease();
        return fieldItem;
        
    }
    
    CC_SAFE_DELETE(fieldItem);
    return NULL;
}

void SGBattleFieldItem::initItem()
{
    const char *fontName = FONT_BOXINFO;
    CCMenu *menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    SGButton *backBtn = SGButton::create("mainBtnZhan_bg.png",
                                         NULL,
                                         this,
                                         menu_selector(SGBattleFieldItem::entrySectionHandler),
                                         ccp(0, 0),
                                         false,
                                         true);
    
    backBtn->setAnchorPoint(ccp(0.5, 0.5f));
    backBtn->setPosition(ccp(bgSize.width/2-20-backBtn->getContentSize().width/2, 0));
    
    SGButton *bigBGBtn = SGButton::create("upwar_cell_bg.png",
                                          NULL,
                                          this,
                                          menu_selector(SGBattleFieldItem::showSectionInfoHandler),
                                          ccp(0, 0),
                                          false,
                                          true);
    bigBGBtn->setAnchorPoint(ccp(0.5,0.5));
    bigBGBtn->setPosition(CCPointZero);
    
    if (data->getBattleFieldType() == 2 &&
        SGPlayerInfo::sharePlayerInfo()->getPvpWinPvp() < SGPlayerInfo::sharePlayerInfo()->getWinPvp()) {
        CCSprite *lock = CCSprite::createWithSpriteFrameName("card_suo.png");
        lock->setScale(1.2f);
        float cellW = bigBGBtn->getContentSize().width;
        lock->setPosition(ccpAdd(bigBGBtn->getPosition(), ccp(-cellW/2+35, 0)));
        this->addChild(lock);
}
    
    if (data->getBattleFieldType() == 1 &&
        SGPlayerInfo::sharePlayerInfo()->getPvpWinPvp() > SGPlayerInfo::sharePlayerInfo()->getWinPvp()) {
        CCSprite *lock = CCSprite::createWithSpriteFrameName("card_suo.png");
        lock->setScale(1.2f);
        float cellW = bigBGBtn->getContentSize().width;
        lock->setPosition(ccpAdd(bigBGBtn->getPosition(), ccp(-cellW/2+35, 0)));
        //    lock->setVisible(false);
        this->addChild(lock);
    }
    
    SGCCLabelTTF *nameLabel = SGCCLabelTTF::create(data->getBattleFieldName()->getCString(), fontName, 36 , ccYELLOW);
    //modify by:zyc. merge into create.
    //nameLabel->setColor(ccYELLOW);
    nameLabel->setPosition(ccp(-50, 0));

    nameLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    this->addChild(bigBGBtn,-5);

    addBtn(bigBGBtn);
    menu->addChild(backBtn);
    
    this->addChild(nameLabel);
}

void SGBattleFieldItem::entrySectionHandler()
{
    int winPvp = SGPlayerInfo::sharePlayerInfo()->getPvpWinPvp();
    if (this->data->getBattleFieldType() == 2 &&
        winPvp < SGPlayerInfo::sharePlayerInfo()->getWinPvp()) {
        SGMainManager::shareMain()->showMessage(str_BattleFieldItemstr1);
        return;
    }
    else if(this->data->getBattleFieldType() == 1)
    {
    }
    
    if (delegate)
    {
        delegate->entrySectionHandler(this);
    }
    
}

void SGBattleFieldItem::showSectionInfoHandler()
{
    if (delegate) {
        delegate->showSectionInfoHandler(this);
    }
}


