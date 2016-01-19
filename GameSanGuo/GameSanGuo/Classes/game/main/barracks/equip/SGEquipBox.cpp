//
//  SGEquipBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-7.
//
//

#include "SGEquipBox.h"
#include "SGMainManager.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGEquipBox::SGEquipBox()
{
}

SGEquipBox::~SGEquipBox()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_equipBox);
}

SGEquipBox *SGEquipBox::create(SGBoxDelegate *delegate)
{
    SGEquipBox *equipBox = new SGEquipBox();
   
    if (equipBox && equipBox->init(delegate,box_equip, CCSizeMake(411, 411),false))
    {
        equipBox->initView();
        equipBox->autorelease();
        return equipBox;
    }
    CC_SAFE_DELETE(equipBox);
    return NULL;
}

void SGEquipBox::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_box_equipBox);

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    tableViewHeight = 70;
    tableView->setFrame(CCRectMake(0, 0, 380, 300));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(s.width/2-tableView->getContentSize().width/2,-180)));

    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGEquipBox::boxCloseWithOutCallBack),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -260)));
    CCSprite *title = CCSprite::createWithSpriteFrameName("font_taozsx.png");
    title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 150)));
    this->addChild(title);
}

SNSTableViewCellItem *SGEquipBox::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    Equip_Item *item = (Equip_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (NULL == item) {
        item = Equip_Item::create(NULL,num);
    }
    else
    {
        item->updateOfficerCard(NULL,indexPath->getRow());
    }
    
	return item;
    
}

void SGEquipBox::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
    EFFECT_PLAY(MUSIC_BTN);
    
    //int index = tableView->getItemPointerWithIndexPath(indexPath);
}
unsigned int SGEquipBox::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return 4;
}
#pragma mark - Equip_Item Class

Equip_Item::Equip_Item():_card(NULL),
labelName(NULL),
labelLevel(NULL)
{
    
}

Equip_Item::~Equip_Item()
{
   
}

void Equip_Item::initView(int num)
{
    
    labelName = CCLabelTTF::create(CCString::createWithFormat(str_EquipBox_str1,num + 1)->getCString(), "Arial", FONTSIZE(12));
    labelName->setAnchorPoint(ccp(0, 0));
    labelName->setPosition(ccpAdd(labelName->getPosition(), ccp(-150,0)));
    this->addChild(labelName);
    
    labelLevel = CCLabelTTF::create(CCString::createWithFormat(str_EquipBox_str2,num)->getCString(), "Arial", FONTSIZE(12));
    this->addChild(labelLevel);
    labelLevel->setAnchorPoint(ccp(0, 0));
    labelLevel->setPosition(ccpAdd(labelName->getPosition(), ccp(0,-labelLevel->getContentSize().height)));
    bgbgbg =NULL;
    
   
}


Equip_Item* Equip_Item::create(SGBaseMilitaryCard *card,int num)
{
	Equip_Item *instance = new Equip_Item();
    instance->_card = card;
	if (instance &&instance->init()) {
        instance->initView(num);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

//刷新
void Equip_Item::updateOfficerCard(SGBaseMilitaryCard *card,int index)
{
    if (card != NULL) {
        _card = card;
        
        labelLevel->setString(CCString::createWithFormat(str_Format_ji,card->getCurrLevel())->getCString());
        labelName->setString(card->getOfficerName()->getCString());
    }
}