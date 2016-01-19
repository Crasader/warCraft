//
//  SGStrengSoldierLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-19.
//
//

#include "SGStrengSoldierLayer.h"
#include "SGMainManager.h"
#include "SGCardItem.h"
#include "SGStaticDataManager.h"
#include "SGFirstStrengBox.h"

#include "ResourceManager.h"
#include "SGStringConfig.h"

SGStrengSoldierLayer::SGStrengSoldierLayer()
:soldiercards(NULL)
{
    
}

SGStrengSoldierLayer::~SGStrengSoldierLayer()
{
    soldiercards->release();
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_strengsoldier);
}
SGStrengSoldierLayer* SGStrengSoldierLayer::create()
{
    SGStrengSoldierLayer *soldierlayer = new SGStrengSoldierLayer();
    
    if (soldierlayer && soldierlayer->init(NULL, sg_strengsoldier))
    {
        soldierlayer->initView();
        soldierlayer->autorelease();
        return soldierlayer;
    }
    CC_SAFE_DELETE(soldierlayer);
    return soldierlayer;
}
void SGStrengSoldierLayer::initView()
{
	SGCardItem::addCardItemRes(sg_strengsoldier);
    
    datas = CCArray::create();
    datas->retain();
    
    this->setTouchEnabled(true);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    soldiercards = CCArray::create();
    soldiercards->retain();
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGStrengSoldierLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.05);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    CCSprite *font2 = CCSprite::createWithSpriteFrameName("font_yqhngsb.png");
    font2->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(font2);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_strengsoldier);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(384- s.width/2, 1136/2 - (s.height - h - b - 80)/2, s.width, s.height -h - b - titleBg->getContentSize().height);
    
    bg->setTextureRect(r);
//    bg->cocos2d::CCNode::setScale(r.size.width / bg->getContentSize().width, r.size.height / bg->getContentSize().height);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, b));
    
    CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    cardcountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(cardcountbg,40);
    cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
//    int total = SGPlayerInfo::sharePlayerInfo()->getTotalCardsCount();
    int total = SGPlayerInfo::sharePlayerInfo()->getGoodsNumInBag();
    SGCCLabelTTF *cardcounts = SGCCLabelTTF::create(CCString::createWithFormat(str_ExpandLayer_str3,total,SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize())->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(cardcounts,42);
    //modify by:zyc. merge into create.
    //cardcounts->setColor(ccc3(0xff, 0xf0, 0x00));
    cardcounts->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));
    //CCArray *soldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();

    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - headH));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    tableView->setDown(-25);
}

SNSTableViewCellItem *SGStrengSoldierLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{  ////////
    
   
   ////////
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    //////////
    CCArray *soldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
    SGSoldierCard *card = (SGSoldierCard *)soldiers->objectAtIndex(num);
    //////////
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (NULL == item)
    {
        item = SGCardItem::create(false,0,this,tableView);
        
    }

     item->updataCard(card,cardType_streng);
    return item;
}
unsigned int  SGStrengSoldierLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return 3;
}
void SGStrengSoldierLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    //    EFFECT_PLAY(MUSIC_BTN);
    //
    //    int index = tableView->getItemPointerWithIndexPath(indexPath);
    //    SGHelpTitleDataModel *tit = (SGHelpTitleDataModel*)datas->objectAtIndex(index);
    //
    //    SGHelpBookInfo *info = SGHelpBookInfo::create(tit);
    //    SGMainManager::shareMain()->showLayer(info);
}

 void SGStrengSoldierLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    
    EFFECT_PLAY(MUSIC_BTN);
    int num = tableView->getItemPointerWithIndexPath(indexpath);
    CCArray *soldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
    SGSoldierCard *card = (SGSoldierCard *)soldiers->objectAtIndex(num);
    strengHandler(card);
}
//void SGStrengSoldierLayer::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);//11.22
//}
//bool SGStrengSoldierLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    return true;
//}
//void SGStrengSoldierLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
////   CCPoint point = convertTouchToNodeSpace(pTouch);
//    CCPoint point = pTouch->getLocation();
//    
//    CCObject *obj = NULL;
//    CCARRAY_FOREACH(soldiercards, obj)
//    {
//        SGCardItem *item = (SGCardItem *)obj;
//        
//        CCRect faceRect = CCRectMake(item->getPosition().x - item->getContentSize().width * .5 ,
//                                     item->getPosition().y - 54,
//                                     skewing(320),
//                                     108);
//        if (faceRect.containsPoint(point))
//        {
//            CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
//            CCObject *obj1 = NULL;
//            CCARRAY_FOREACH(allSoldiers, obj1)
//            {
//                SGSoldierCard *card = (SGSoldierCard *)obj1;
//                if (card->getItemId() == item->getcardItemId())
//                {
//                        strengHandler(card);
//                        break;
//                }
//            }
//            break;
//        }
//    }
//}

void SGStrengSoldierLayer::strengHandler(SGSoldierCard *card)
{
//    SGSoldierDataModel *equip = SGStaticDataManager::shareStatic()->getSoliderById(card->getItemId());
    
    int type = SGStaticDataManager::shareStatic()->getStrengSoldierType(card->getItemId());
    
//    if (equip->getSoldierExpType()->isEqual(CCString::create("A"))) {
//        type = 0;
//    }
//    else if (equip->getSoldierExpType()->isEqual(CCString::create("B"))) {
//        type = 1;
//    }
//    else if (equip->getSoldierExpType()->isEqual(CCString::create("C"))) {
//        type = 2;
//    }
//    else if (equip->getSoldierExpType()->isEqual(CCString::create("D"))) {
//        type = 3;
//    }
    
    SGExpDataModel *exp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type,card->getCurrLevel());
    
    if (card->getCurrExp() == exp->getExpValue() && card->getMaxExp() == -1)
    {
        SGMainManager::shareMain()->showMessage(str_qianghua_tips1);}
    else
    {
        SGMainManager::shareMain()->showStrengLayer(card,3,true);
    }
}

void SGStrengSoldierLayer::backHandler()
{
    SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
    SGMainManager::shareMain()->showBox(strengBox);
    //SGMainManager::shareMain()->showFirstLayer();
}