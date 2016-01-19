//
//  SGHelpBook.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-12.
//
//

#include "SGHelpBook.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "cocos-ext.h"
#include "SGHelpBookInfo.h"
#include "SGStaticDataManager.h"
#include "SGHelpTitleDataModel.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

//#include "FMLayerWebView.h"
USING_NS_CC_EXT;
SGHelpBook::SGHelpBook()

{
}
SGHelpBook::~SGHelpBook()
{
    datas->release();
//    CCSpriteFrameCache::sharedSpriteFrameCache() -> removeSpriteFramesFromFile("help/help0.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache() -> removeSpriteFramesFromFile("public/public0.plist");
	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_helpBook);
}
////////
//
//void SGHelpBook::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}
//
//bool SGHelpBook::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    CCLOG("In HelpBook!");
//    
//    CCLOG("Touch Point == %f", pTouch->getLocation().y);
//     float btmhgt = SGMainManager::shareMain()->getBottomHeight();
//    CCLOG("xxxxxxxxx == %f", btmhgt);
//    if (pTouch->getLocation().y <= btmhgt)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//    
//}
//
//void SGHelpBook::onEnter()
//{
//    CCLayer::onEnter();
//    registerWithTouchDispatcher();
//
//}
//
/////////
//void SGHelpBook::onExit()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//    CCLayer::onExit();
//}

///////
SGHelpBook *SGHelpBook::create()
{
    SGHelpBook *helpLayer = new SGHelpBook();
    if (helpLayer && helpLayer->init(NULL, sg_helpBook))
    {
        helpLayer->initView();
        helpLayer->autorelease();
        return helpLayer;
    }
    CC_SAFE_DELETE(helpLayer);
    return NULL;
}
void SGHelpBook::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_helpBook);
    
    tableViewHeight = skewing(55);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
  
    tableViewHeight = 110;
    
    
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
//                                         menu_selector(SGHelpBook::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGHelpBook::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));

    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("help_font_bzsc.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ComplainLayer_str6, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x96, 0x0c));
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);
    
    datas = SGStaticDataManager::shareStatic()->gethelptitle();
    datas->retain();
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
    
}

SNSTableViewCellItem *SGHelpBook::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    HelpLabelItem *item = (HelpLabelItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGHelpTitleDataModel *tit = (SGHelpTitleDataModel*)datas->objectAtIndex(num);

    
    CCSize size = tableView->getItemSize();
//    int num = tableView->getItemPointerWithIndexPath(indexPath);
//    LabelItem *item = (LabelItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
//    
//    if (NULL == item)
//    {
//        item = LabelItem::create(size,num,false,this,tableView);
//    }
    if (NULL == item)
    {
        item = HelpLabelItem::create(size,num,false,this,tableView);
    }
    item->update(tit->gettitle());
    return item;
}
unsigned int  SGHelpBook::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}
void SGHelpBook::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    SGHelpTitleDataModel *tit = (SGHelpTitleDataModel*)datas->objectAtIndex(index);
//    
//    SGHelpBookInfo *info = SGHelpBookInfo::create(tit);
//    SGMainManager::shareMain()->showLayer(info);
}

///////////////
void SGHelpBook::itemselect(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGHelpTitleDataModel *tit = (SGHelpTitleDataModel*)datas->objectAtIndex(index);
    
    SGHelpBookInfo *info = SGHelpBookInfo::create(tit);
    SGMainManager::shareMain()->showLayer(info);
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
}
/////////////
void SGHelpBook::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
//    FMLayerWebView *webview =  (FMLayerWebView*)this->getChildByTag(111);
//    webview->onBackbuttonClick();
    SGMainManager::shareMain()->showHelpLayer();
}
bool HelpLabelItem::initWithSize()
{
	if (!SNSTableViewCellItem::init()) {
		return false;
	}
//    CCSprite *item = CCSprite::createWithSpriteFrameName("item_bigbg.png");
//    this->addChild(item,-1);
//    bgbgbg = item;
    
    bgbgbg = NULL;
    
    
    //////////
    
    SGButton*btn=SGButton::create("item_bigbg.png", NULL, this, menu_selector(HelpLabelItem::buttonClick),CCPointZero,false,true);
    this->addChild(btn);
    addBtn(btn);
//    menu=CCMenu::create(btn, NULL);
//    menu -> setPosition(CCPointZero);
//    
//    //menu->addChild(btn);
//   
//    
//    
//    this->addChild(menu);
   // addBtn(btn);
    
    SGCCLabelTTF *title = SGCCLabelTTF::create("", FONT_PANGWA, 36 , ccWHITE);
    //modify by:zyc . merge into create.
    //title->setColor(ccWHITE);
    this->addChild(title,10,137);
    return true;
}

HelpLabelItem::HelpLabelItem()
:menu(NULL),
delegate(NULL),
tableview(NULL)
{
    
}

HelpLabelItem* HelpLabelItem::create(CCSize size,int num,bool isshow ,HelpLabelItemDelegate*del,SNSTableView*table)
{
	HelpLabelItem *instance = new HelpLabelItem();
	if (instance && instance->initWithSize()) {
        instance->delegate=del;
        instance->tableview=table;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}
void HelpLabelItem::update(cocos2d::CCString *str)
{
    SGCCLabelTTF *a = (SGCCLabelTTF*)this->getChildByTag(137);
    a->setString(str->getCString());
}


void HelpLabelItem::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);

}
//void HelpLabelItem::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}
//void HelpLabelItem::onEnter()
//{
//	CCLayer::onEnter();
//
//    menu->setTouchPriority(10);//设置menu的优先级
//
//}
//bool HelpLabelItem::ccTouchBegan(CCTouch *touches, CCEvent *pevent)
//{
//    return true;
//}
//
////////////
