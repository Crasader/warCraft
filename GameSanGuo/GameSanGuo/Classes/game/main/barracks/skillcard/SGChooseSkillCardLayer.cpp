//
//  SGChooseSkillCardLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#include "SGChooseSkillCardLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGSkillCard.h"
#include "SimpleAudioEngine.h"
#include"SGFirstStrengBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"


SGChooseSkillCardLayer::SGChooseSkillCardLayer():range(NULL)
{
}
SGChooseSkillCardLayer::~SGChooseSkillCardLayer()
{
    CCLOG("~SGChooseSkillCardLayer");
    CC_SAFE_RELEASE(datas);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_chooseSkillCard);

}

SGChooseSkillCardLayer* SGChooseSkillCardLayer::create()
{
    SGChooseSkillCardLayer *Layer = new SGChooseSkillCardLayer();
    
    if (Layer && Layer->init(NULL, sg_chooseSkillCard))
    {
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return Layer;
}

void SGChooseSkillCardLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_chooseSkillCard, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_chooseSkillCard, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_chooseSkillCard);
    
    CCArray *officers = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    datas = CCArray::create();
    datas->retain();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(officers, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        if (card->getOfficerSkil() && !SGStaticDataManager::shareStatic()->getGeneralSkillById(card->getOfficerSkil())->getSkillClientLevel()->isEqual(CCString::create(str_Ding))) {
            datas->addObject(card);
        }
    }
    
    tableViewColumns = 1;
    tableViewHeight = 120;
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGChooseSkillCardLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.05);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));

    CCSprite *font2 = CCSprite::createWithSpriteFrameName("font_yqhngwjj.png");
    font2->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(font2);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_chooseSkillCard);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(384- s.width/2, 1136/2 - (s.height - h - b - 80)/2, s.width, s.height - h - b - titleBg->getContentSize().height);
    
    bg->setTextureRect(r);
    bg->setScaleX(s.width / bg->getContentSize().width * 1.02);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-15, b));
    
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - titleBg->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-25);
    
    range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGChooseSkillCardLayer::sortHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 1));
    range->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0, titleBg->getPosition().y)));
    
    std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("strengskill");
    CCString *type = CCString::create(str);
    SGSortBox::sortCards(datas,type->intValue(),2);
    SGSortBox::updateBtn(range,5);
    
    CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    cardcountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(cardcountbg,40);
    cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
//    int total = SGPlayerInfo::sharePlayerInfo()->getTotalCardsCount();
    int total = SGPlayerInfo::sharePlayerInfo()->getGoodsNumInBag();
    SGCCLabelTTF *cardcounts = SGCCLabelTTF::create(CCString::createWithFormat(str_ExpandLayer_str3,total,SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize())->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(cardcounts,42);
    cardcounts->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));
}

SNSTableViewCellItem *SGChooseSkillCardLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGSkillItem *item = (SGSkillItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if (NULL == item)
    {
        item = SGSkillItem::create(this,tableView);
    }
    if (num<datas->count())
    {
        item ->updataCard((SGOfficerCard *)datas->objectAtIndex(num));
    }
    
    return item;
}
/////
void SGChooseSkillCardLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
    SGMainManager::shareMain()->showStrengLayer(card, 4,true);
    //    SGMainManager::shareMain()->showSkillCardInfoLayer((SGSkillCard *)card, 0);
}
////
void SGChooseSkillCardLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
//    SGMainManager::shareMain()->showStrengLayer(card, 4,true);
////    SGMainManager::shareMain()->showSkillCardInfoLayer((SGSkillCard *)card, 0);
}

void SGChooseSkillCardLayer::backHandler()
{
    //SGMainManager::shareMain()->showFirstLayer();
    SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
     SGMainManager::shareMain()->showBox(strengBox);
}
void SGChooseSkillCardLayer::sortHandler()
{
    SGSortBox *Box = SGSortBox::create(this, this, datas,2);
    SGMainManager::shareMain()->showBox(Box);
}

void SGChooseSkillCardLayer::sortLayerCallBack(CCArray *cardArray)
{
    this->datas = cardArray;
   

    tableView->reloadData();
 
 
    SGSortBox::updateBtn(range,5);

}