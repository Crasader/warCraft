//
//  SGExpandLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-28.
//
//

#include "SGExpandLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCantAdvanceBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "Itemmodule.pb.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
using namespace cocos2d;



SGExpandLayer *SGExpandLayer::create()
{
    SGExpandLayer *expandLayer = new SGExpandLayer();
    if (expandLayer && expandLayer->init(NULL, sg_expandLayer))
    {
        expandLayer->initView();
        expandLayer->autorelease();
        return expandLayer;
    }
    CC_SAFE_DELETE(expandLayer);
    return NULL;
}
SGExpandLayer::SGExpandLayer()
:cardcount(NULL),
needGold(0),
bagsize(0),
notice(NULL)
{
    
}

SGExpandLayer::~SGExpandLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_expandLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_BARRACK_EXPAND);
    CC_SAFE_RELEASE(datas);
}

void SGExpandLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_expandLayer);
    datas = CCArray::create();
    notice = SGCCLabelTTF::create("", FONT_BOXINFO, 30);
    this->addChild(notice);
    notice->setPosition(SGLayout::getPoint(kMiddleCenter));
    for (int a = 0; a <5; a++) {
        CCString *str = CCString::createWithFormat("%d",a);
        datas->addObject(str);
    }
    int temp = SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize();
   
    if ( temp > 700 && datas->objectAtIndex(4)) {
        datas->removeObject(datas->objectAtIndex(4));
        
    }
    if (temp > 750 && datas->objectAtIndex(3)) {
        datas->removeObject(datas->objectAtIndex(3));
        
    }
    if (temp > 775&& datas->objectAtIndex(2)) {
        datas->removeObject(datas->objectAtIndex(2));
        
    }
    if (temp > 790&& datas->objectAtIndex(1)) {
        datas->removeObject(datas->objectAtIndex(1));
        
    }
    if (temp >=800) {
        datas->removeAllObjects();
        notice->setString(str_ExpandLayer_str1);
    }
    datas->retain();
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BARRACK_EXPAND,
                              this,
                              callfuncO_selector(SGExpandLayer::expandListener));
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCLOG("h is %f",h);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - h - b));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
    tableViewHeight = skewing(55);
    tableView->setDown(-25);
    

    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
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
    
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("barrack_expand_title.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ExpandLayer_str2, FONT_XINGKAI, 36, COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,-title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGExpandLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGExpandLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));
    
    CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    cardcountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(cardcountbg,40);
    cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
    
    int count = SGPlayerInfo::sharePlayerInfo()->getGoodsNumInBag();
    cardcount = SGCCLabelTTF::create(CCString::createWithFormat(str_ExpandLayer_str3,count,SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize())->getCString(), FONT_BOXINFO, 28 , COLOR_UNKNOW_TAN);
    this->addChild(cardcount,42);
    //modify by:zyc. merge into create.
    //cardcount->setColor(ccc3(0xff, 0xf0, 0x00));
    cardcount->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));

}
SNSTableViewCellItem* SGExpandLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();
	TabelItem *item = (TabelItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    CCString *str = (CCString *)datas->objectAtIndex(indexPath->getRow());
        
    if (NULL == item) {
        item = TabelItem::create(size,str->intValue(),this,tableView);
            }
    int a = 0;
    int b = 0;
    switch (num) {
        case 4:
            a = 5;
            b = 60;
        break;
        case 3:
        
            a = 10;
            b = 120;
            break;
        case 2:
        
            a = 25;
            b = 270;
            break;
        case 1:
        
            a = 50;
            b = 500;
            break;
        case 0:
        
            a = 100;
            b = 800;
            break;       

        default:
            break;
    }
            CCString *str_gold = CCString::createWithFormat("%d",b);
            item->label->setString(str_gold->getCString());
            CCString *str_size = CCString::createWithFormat("%d",a);
        item->label2->setString(str_size->getCString());

       	return item;
}

unsigned int SGExpandLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	int num = datas->count();
	return num;
}
///////////
void SGExpandLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    
    switch (index) {
        case 4:
            bagsize = 5;
            needGold = 60;
            break;
        case 3:
            bagsize = 10;
            needGold = 120;
            break;
        case 2:
            
            bagsize = 25;
            needGold = 270;
            break;
        case 1:
            
            bagsize = 50;
            needGold = 500;
            break;
        case 0:
            
            bagsize = 100;
            needGold = 800;
            break;
            
        default:
            break;
    }
    
    
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold() < needGold) {
        SGCantAdvanceBox *noGoldBox = SGCantAdvanceBox::create(this, NULL, 13, NULL);
        SGMainManager::shareMain()->showBox(noGoldBox);
    }
    else
    {
        main::ExpansionRequest *request = new main::ExpansionRequest;
        request->set_type(index);
        SGSocketClient::sharedSocketClient()->send(MSG_BARRACK_EXPAND, request);
        
    }
    

}
///////////
void SGExpandLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    
//    switch (index) {
//        case 0:
//            bagsize = 5;
//            needGold = 60;
//            break;
//        case 1:            
//            bagsize = 10;
//            needGold = 120;
//            break;
//        case 2:
//            
//            bagsize = 25;
//            needGold = 270;
//            break;
//        case 3:
//            
//            bagsize = 50;
//            needGold = 500;
//            break;
//        case 4:
//            
//            bagsize = 100;
//            needGold = 800;
//            break;
//            
//        default:
//            break;
//    }
//
//        
//    if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold() < needGold) {
//        SGCantAdvanceBox *noGoldBox = SGCantAdvanceBox::create(this, NULL, 13, NULL);
//        SGMainManager::shareMain()->showBox(noGoldBox);
//    }
//    else
//    {
//        main::ExpansionRequest *request = new main::ExpansionRequest;
//        request->set_type(index);
//        SGSocketClient::sharedSocketClient()->send(MSG_BARRACK_EXPAND, request);
//
//    }      
//
}
TabelItem::TabelItem():label(NULL),label2(NULL),delegate(NULL),tableview(NULL)
{}
bool TabelItem::initWithSize(CCSize size,int num)
{
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_expandLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_expandLayer);
	if (!SNSTableViewCellItem::init()) {
		return false;
	}
    
//    CCSprite *item = CCSprite::createWithSpriteFrameName("item_bigbg.png");
//    bgbgbg = item;
//    this->addChild(item,-1);
    //    this->addChild(item);
//    CCMenu* menu=CCMenu::create();
//    menu -> setPosition(CCPointZero);
    SGButton*item=SGButton::create("item_bigbg.png", NULL, this, menu_selector(TabelItem::buttonClick),CCPointZero,false,true);
//    menu->addChild(item);
//    this->addChild(menu);
    this->addChild(item);
    addBtn(item);

    SGCCLabelTTF* label1 = SGCCLabelTTF::create(str_yuanbao, FONT_BOXINFO, 30);
    label1->setPosition(ccpAdd(item->getPosition(), ccp(label1->getContentSize().width * 1.5, 0)));
    this->addChild(label1);
    
    CCSprite* gold = CCSprite::createWithSpriteFrameName("goldicon.png");
    gold->setAnchorPoint(ccp(0.5, 0.4));
    gold->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width*.5 + gold->getContentSize().width/2, 0)));
    this->addChild(gold);
    
    SGCCLabelTTF* zengjia = SGCCLabelTTF::create(str_ExpandLayer_str4, FONT_BOXINFO, 36 , COLOR_UNKNOW_TAN);
    zengjia->setPosition(ccpAdd(item->getPosition(), ccp( - item->getContentSize().width*.35, 0)));
    this->addChild(zengjia);
    //modify by:zyc. merge into create.
    //zengjia->setColor(ccc3(0xff, 0xf0, 0x00));
    
    SGCCLabelTTF* kongjian = SGCCLabelTTF::create(str_ExpandLayer_str5, FONT_BOXINFO, 36 , COLOR_UNKNOW_TAN);
    kongjian->setPosition(ccpAdd(zengjia->getPosition(), ccp(kongjian->getContentSize().width*1.5 +15, 0)));
    this->addChild(kongjian);
    //modify by:zyc. merge into create.
    //kongjian->setColor(ccc3(0xff, 0xf0, 0x00));
    
    label = SGCCLabelTTF::create("", FONT_BOXINFO, 30);
    label->setAnchorPoint(ccp(1, 0.5));
    label->setPosition(ccpAdd(gold->getPosition(), ccp(gold->getContentSize().width*1.2, 0)));
    this->addChild(label);

    label2 = SGCCLabelTTF::create("", FONT_PANGWA, 36 , ccc3(0xff, 0x72, 0x00));
    //modify by:zyc. merge into create.
    //label2->setColor(ccc3(0xff, 0x72, 0x00));
    label2->setPosition(ccpAdd(zengjia->getPosition(), ccp(zengjia->getContentSize().width*.5+40, 0)));
    this->addChild(label2);
    
    if (num == 0) {
        CCSprite *dazhek = CCSprite::createWithSpriteFrameName("font_75_k.png");
        dazhek->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width*.5f+28, item->getContentSize().height*.1f +8)));
        this->addChild(dazhek);
    }
    if (num == 1) {
        CCSprite *dazhek = CCSprite::createWithSpriteFrameName("font_84_k.png");
        dazhek->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width*.5f+26, item->getContentSize().height*.1f +7.5)));
        this->addChild(dazhek);
    }
    if (num == 2) {
        CCSprite *dazhek = CCSprite::createWithSpriteFrameName("font_90_k.png");
        dazhek->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width*.5f+30.5, item->getContentSize().height*.1f +8)));
        this->addChild(dazhek);
    }

	return true;
}
TabelItem* TabelItem::create(CCSize size,int num,TabelItemDelegate*del,SNSTableView*table)
{
	TabelItem *instance = new TabelItem();
	if (instance && instance->initWithSize(size,num)) {
        instance->delegate=del;
        instance->tableview=table;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}
void SGExpandLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
    tableView->setTouchEnabled(isTouch);
}
void SGExpandLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showBarrackslayer();
}
void SGExpandLayer::expandListener(cocos2d::CCObject *obj)
{

    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ExpansionResponse *response = (main::ExpansionResponse *)sr->m_msg;
    
    if (sr)
    {
        if (response->state() == 1)
        {
            SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
            player->setPlayerBagSize(SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize() + bagsize);
            player->setPlayerGold(SGPlayerInfo::sharePlayerInfo()->getPlayerGold() - needGold);
            SGMainManager::shareMain()->updataUserMsg();
            CCString *newSize = CCString::createWithFormat(str_ExpandLayer_str3,player->getGoodsNumInBag(),player->getPlayerBagSize());
            cardcount->setString(newSize->getCString());
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_ExpandLayer_str6,SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize() - bagsize ,SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize())->getCString());
            int temp = SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize();
            if (datas)
            {
                if ( temp > 700 && datas->count() >= 5)
                {
                     datas->removeLastObject();
                    tableView->reloadData();
                }
                if (temp > 750 && datas->count() >= 4)
                {
                    datas->removeLastObject();
                    tableView->reloadData();
                }
                if (temp > 775&& datas->count() >= 3)
                {
                    datas->removeLastObject();
                    tableView->reloadData();
                }
                if (temp > 790&& datas->count() >= 2)
                {
                    datas->removeLastObject();
                    tableView->reloadData();
                }
                if (temp >=800)
                { /////最大值由300变800
                    datas->removeAllObjects();
                    notice->setString(str_ExpandLayer_str1);
                    tableView->reloadData();
                }       
   
            }
            SGMainManager::shareMain()->trackPurchase(CCString::create(str_ExpandLayer_str7), 1, needGold);
            CCLOG("成功");
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_fail);
            CCLOG("失败");
        }
    }

}



void TabelItem::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);

}