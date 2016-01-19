//
//  SGServerListLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-4.
//
//



/*
 获得服务器列表数据的所有数据格式
 
 请求
 17
 帐号名称
 
 响应
 17
 byte 是否成功
 if(true){
 服务器数量 short
 for(){
 服务器ID short
 服务器名称 String
 服务器IP String
 端口 short
 服务器状态 short
 
 byte 是否有角色信息
 if(true){
 昵称 String
 等级 short
 国家 short
 }else{
 
 }
 }
 }
 
 */




#include "SGServerListLayer.h"
#include "SGSocketClient.h"
#include "SGHttpClient.h"
#include "SGMainManager.h"
#include "SGServerListNewServerBox.h"
#include "SGServerListLoginGame.h"
#include "SGWelComeLayer.h"
#include "SGModificationPassword.h"
#include "SGVerticalSlider.h"
#include "SGHelpLayer.h"
#include "SGMsgId.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

#pragma mark 构造函数

SGServerListLayer::SGServerListLayer()
:backBtn(NULL)
{
    CCLOG("SGServerListLayer");
}

SGServerListLayer::~SGServerListLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_serverListLayer);

    CCLOG("~SGServerListLayer");
    CC_SAFE_RELEASE(datas);
}
/*
SGServerListLayer * SGServerListLayer::create(CCArray *serverList,SGWelComeLayer *welcome,SGServerListLayerComeFrom comeFrom)
{
    SGServerListLayer *slb = new SGServerListLayer();

    if (slb && slb->initWithSomeVar(serverList, welcome, comeFrom))
    {
        slb->autorelease();
        return slb;
    }
    CC_SAFE_DELETE(slb);
    return NULL;
}

bool SGServerListLayer::initWithSomeVar(cocos2d::CCArray *serverList, SGWelComeLayer *welcome, SGServerListLayerComeFrom comeFrom)
{
    
    
    if(!SGBaseTableLayer::init(NULL, sg_serverListLayer))
    {
        return false;
    }
    
    if (welcome)
    {
        _delegate = welcome;
        SGBoxDelegate *delegate = (SGBoxDelegate*)welcome;
        delegate->setIsCanTouch(false);
    }
    
    datas = serverList;
    datas->retain();
    startFrom = comeFrom;
    
    initSomeThing();

    return true;
    
}
*/
SGServerListLayer *SGServerListLayer::create(SGBoxDelegate *delegate, cocos2d::CCArray *serverList, SGServerListLayerComeFrom comeFrom)
{
    SGServerListLayer *list = new SGServerListLayer();
    if (list && list->init(delegate, serverList, comeFrom))
    {
        list->autorelease();
        return list;
    }
    return list;
}
bool SGServerListLayer::init(SGBoxDelegate *delegate, cocos2d::CCArray *serverList, SGServerListLayerComeFrom comeFrom)
{
    if (!SGBaseTableBox::init(delegate, box_serverList, CCSizeMake(411,300),false,true,false))
    {
        return false;
    }
    
    datas = serverList;
    datas->retain();
    startFrom = comeFrom;
    
    initSomeThing();
    
    return true;
}
#pragma mark  按钮点击事件

//点击返回按钮
void SGServerListLayer::buttonClickGoback()
{
//    if (_delegate)
//    {
//        _delegate->setIsCanTouch(true);
//    }
    
    
//    if(startFrom == SGServerListLayerComeFromFirst)
//    {
        //从启动界面加载
//        this->removeFromParentAndCleanup(true);
//    }else{
        //从游戏内部加载
//        SGHelpLayer *help = SGHelpLayer::create();
//        SGMainManager::shareMain()->showLayer(help);
//    }
    this->boxClose();
}


#pragma mark 界面布局
void SGServerListLayer::initSomeThing()
{
	ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_serverListLayer);
	

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,100);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,100);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,100);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_serverListLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(384- s.width/2, 1136/2 - (s.height - title_bg->getContentSize().height )/2, s.width, s.height - title_bg->getContentSize().height);
    
    //bg->cocos2d::CCNode::setScale(r.size.width / bg->getContentSize().width, r.size.height / bg->getContentSize().height);
    bg->setTextureRect(r);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(SGLayout::getPoint(kBottomCenter));
    
//    backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGServerListLayer::boxClose),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGServerListLayer::boxClose),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55 - 3, - backBtn->getContentSize().height*.55)));
//    CCSprite *title = CCSprite::createWithSpriteFrameName("server_selectServer.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ServerItem_str11, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, title_bg->getPosition().y - title_bg->getContentSize().height*.52));
    this->addChild(title);

    //tableview
    this->tableView->setZOrder(1);
    CCSize size = bg->getContentSize();
    tableView->setFrame(CCRectMake(0, 0, s.width,size.height));
    this->isAchieve_tableViewDragStartItem = true;
    this->isAchieve_tableViewDragEndItem = true;
    tableView->setDown(-38);
    tableViewHeight = 117;
    
}

#pragma mark - tableView
SNSTableViewCellItem* SGServerListLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGServerItem *item = (SGServerItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if(NULL == item){
        item = SGServerItem::create();
    }
    if (num<datas->count())
    {
        item ->updataData(datas->objectAtIndex(num));
    }
    else
    {
        item->setVisible(false);
    }
    
    return item;
}

void SGServerListLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    CCDictionary *dict = (CCDictionary *)datas->objectAtIndex(index);
    dict->setObject(CCString::create("serverList"), "key");
    deletage->showBoxCall(dict);
    
    this->boxClose();
    
}
