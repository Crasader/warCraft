//
//  SGSelLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-11.
//
//

#include "SGSetLayer.h"
#include "SGMainManager.h"
#include "SGSetItem.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGSetLayer::SGSetLayer()
{
}
SGSetLayer::~SGSetLayer()
{
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_setLayer);

  
}
SGSetLayer *SGSetLayer::create()
{
    SGSetLayer *setLayer = new SGSetLayer();
    if (setLayer && setLayer->init(NULL, sg_setLayer))
    {
        setLayer->initView();
        setLayer->autorelease();
        return setLayer;
    }
    CC_SAFE_DELETE(setLayer);
    return NULL;
}
void SGSetLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgsetlayer/sgsetlayer.plist", RES_TYPE_LAYER_UI, sg_setLayer);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float height = skewingY(245);
    if (s.height == 1136) {
        CCLOG("1111");
        height = skewing(245);
    }
    tableViewHeight = skewing(55);
    
    
    tableView->setFrame(CCRectMake(0, 0, s.width, height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGSetLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSetLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("help_font_yxgnsd.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_SetItem_str4, FONT_XINGKAI, 36, COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);
    
}

SNSTableViewCellItem *SGSetLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGSetItem *item = (SGSetItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if (NULL == item)
    {
        item = SGSetItem::create(num);
        
    }
    
    return item;
}
unsigned int  SGSetLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    //return 3;
    //去掉 消息推送的控制
    return  2;
}
void SGSetLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    //    int index = tableView->getItemPointerWithIndexPath(indexPath);
    
    //        switch (index) {
    //            case 0:
    //                break;
    //            case 1:
    //                break;
    //            case 2:
    //                break;
    //            case 3:
    //                break;
    //            default:
    //                break;
    //    }
    
}

void SGSetLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showHelpLayer();
}
