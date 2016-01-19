//
//  SGSkillItem.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#include "SGSkillItem.h"
#include "SGStaticDataManager.h"
#include "SGLayout.h"
#include "SGMainManager.h"

SGSkillItem::SGSkillItem()
: spritePlayerHead(NULL)
, labelName(NULL)
, labelLevel(NULL)
, arrayStar(NULL)
, sprite(NULL)
, _card(NULL)
, lab_skill(NULL)
, lab_skillLvl(NULL)
, tableview(NULL)
{
    
}

SGSkillItem::~SGSkillItem()
{
    //ResourceManager::sharedInstance()->unBindLayerTexture(sg_skillCardsLayer);

    arrayStar->release();
}
bool SGSkillItem::initWithSize()
{
	if (!SNSTableViewCellItem::init())
    {
		return false;
	}
    
    
    
    arrayStar = CCArray::create();
    arrayStar->retain();
    
    SGButton*item=SGButton::create("item_smallbg.png", NULL, this, menu_selector(SGSkillItem::buttonClick),CCPointZero,false,true);
    spritePlayerHead = CCSprite::createWithSpriteFrameName("jinkuang.png");
    item->setPosition(ccpAdd(item->getPosition(), ccp(47, 0)));
    this->addChild(item);
    addBtn(item);
    
    
    
    addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width/2 - 47, 2)));
    
    CCSprite* spriteIsUseBG = CCSprite::createWithSpriteFrameName("carditem_infobg.png");
    spriteIsUseBG->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.3 - 15,-15)));
    this->addChild(spriteIsUseBG,1);
    
    CCSprite*kuang = CCSprite::createWithSpriteFrameName("jinengkuang.png");
    this->addChild(kuang,1);
    kuang->setPosition(spritePlayerHead->getPosition());
    
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background,-1);
    background->setPosition(spritePlayerHead->getPosition());
    
    CCSprite *namebg = CCSprite::createWithSpriteFrameName("name_bg.png");
    this->addChild(namebg);
    namebg->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.5f +namebg->getContentSize().width*.5f, 22)));
    
    labelName = SGCCLabelTTF::create("", FONT_BOXINFO, 20);
    labelName->setAnchorPoint(ccp(0, 0.5));
    labelName->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(-73, 0)));
    this->addChild(labelName,2);
    
    labelLevel = SGCCLabelTTF::create("", FONT_BOXINFO, 20);
    this->addChild(labelLevel,2);
    labelLevel->setAnchorPoint(ccp(0, 0.5f));
    labelLevel->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(15,0)));
    
    lab_skill = SGCCLabelTTF::create("", FONT_BOXINFO, 26 , COLOR_RED);
    lab_skill->setAnchorPoint(ccp(0, 0.5f));
    lab_skill->setPosition(ccpAdd(namebg->getPosition(), ccp(-73, 0)));
    this->addChild(lab_skill);
    //modify by:zyc. merge into create.
    //lab_skill->setColor(COLOR_RED);
    
    lab_skillLvl = SGCCLabelTTF::create("", FONT_PANGWA, 24);
    lab_skillLvl->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(0,-35)));
    this->addChild(lab_skillLvl);
	return true;
}
void SGSkillItem::showPlayerStar()
{
    for(int i=0;i<arrayStar->count();i++)
    {
        CCSprite *sprite = (CCSprite*)arrayStar->objectAtIndex(i);
        sprite->removeFromParentAndCleanup(true);
    }
    
    int starCount = _card->getCurrStar();
    for(int i=0; i < starCount;i++)
    {
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("xx.png");
        sprite->setAnchorPoint(ccp(0.5, 1));
        sprite->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width+sprite->getContentSize().width*i - 17,4)));
        arrayStar->addObject(sprite);
        this->addChild(sprite);
    }
}

SGSkillItem* SGSkillItem::create(SGSkillItemDelegate*del,SNSTableView* table)
{
	SGSkillItem *instance = new SGSkillItem();
	if (instance && instance->initWithSize()) {
        instance->delegate=del;
        instance->tableview=table;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}
void SGSkillItem::updataCard(SGOfficerCard *card)
{
    _card = card;
    SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getGeneralSkillById(card->getOfficerSkil());
    labelName->setString(card->getOfficerName()->getCString());
    
    SGMainManager::shareMain()->addHeadIconPlistByNum(skill->getIconId(),sg_skillcardInfoLayer);
    spritePlayerHead->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",skill->getSkillItemId())->getCString())->displayFrame());
    
    labelLevel->setString("");
    if (card->getCurrLevel()) {
        labelLevel->setString(CCString::createWithFormat("%d级",card->getCurrLevel())->getCString());
    }
    
    lab_skill->setString(CCString::createWithFormat("%s ",skill->getSkillName()->getCString())->getCString());
    
    lab_skillLvl->setString(CCString::createWithFormat("LV%d",skill->getSkillMaxLevel())->getCString());

//            
//            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
//            spritePlayerHead->setFontImage(CCSprite::create(CCString::createWithFormat("public/country_%d.png",temp->getPropsCountry())->getCString())->displayFrame());
//
 
    showPlayerStar();
}

void SGSkillItem::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);
    
}


