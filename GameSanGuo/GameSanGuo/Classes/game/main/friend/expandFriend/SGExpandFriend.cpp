//
//  SGExpandFriend.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#include "SGExpandFriend.h"
#include "SGFriend.h"
#include "SGMainManager.h"
#include "SGExpandBox.h"
#include "SGPlayerInfo.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGGraySprite.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGExpandFriend::SGExpandFriend()
: labelTip(NULL)
, labelFriendLimit(NULL)
, labelExpandInfo(NULL)
, buttonAdd(NULL)
{
    
}

SGExpandFriend::~SGExpandFriend()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_expandFriend);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_EXPANDFRIEND);
}

SGExpandFriend* SGExpandFriend::create()
{
    SGExpandFriend *expand = new SGExpandFriend();
    if(expand && expand->init(NULL, sg_expandFriend))
    {
        
        expand->constructView();
        expand->autorelease();
        return expand;
    }
    
    CC_SAFE_DELETE(expand);
    return NULL;
}

void SGExpandFriend::constructView()
{
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_EXPANDFRIEND, this, callfuncO_selector(SGExpandFriend::expandListener));
    
    CCPoint leftCenter = SGLayout::getPoint(kMiddleCenter);
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    CCPoint bottomLeft = SGLayout::getPoint(kBottomLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint upRight = SGLayout::getPoint(kUpRight);
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    ResourceManager::sharedInstance()->bindTexture("sgexpandfriend/sgexpandfriend.plist", RES_TYPE_LAYER_UI, sg_expandFriend);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_expandFriend, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgexpandfriend/sgexpandfriend.plist", RES_TYPE_LAYER_UI, sg_expandFriend);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_expandFriend);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_expandFriend);

    
    //我的好友
    SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(str_ExpandFriend_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//spriteTitle->setColor(ccc3(0xff, 0x95, 0x0c));
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(spriteTitle);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    //返回按钮
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGExpandFriend::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
	
    buttonBack->setPosition(ccpAdd(spriteTitle->getPosition(), ccp(-skewing(160), 0)));
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(buttonBack);
    
    CCSprite *spriteBackground1 = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    spriteBackground1->setScaleX(SGLayout::getSkewing(280)/spriteBackground1->getContentSize().width);
    spriteBackground1->setScaleY(200/spriteBackground1->getContentSize().height);
    spriteBackground1->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72-200)));
    this->addChild(spriteBackground1);
    
    CCScale9Sprite *spriteBorder1 =  CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    spriteBorder1->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 200));
    spriteBorder1->setPosition(spriteBackground1->getPosition());
    this->addChild(spriteBorder1);
    
    //显示信息
    labelFriendLimit = SGCCLabelTTF::create(str_ExpandFriend_str2, FONT_BOXINFO, 28);
    //    含10擴充 支付金子，可以立即增加5個位置，通過這種方式您最多可增加50個好有位置
    labelExpandInfo = SGCCLabelTTF::create(str_ExpandFriend_str3 , FONT_BOXINFO, 28);
    labelFriendLimit->setPosition(ccpAdd(upCenter, ccp(-150, -350)));
    labelExpandInfo->setPosition(ccpAdd(labelFriendLimit->getPosition(), ccp(380, 0)));
    this->addChild(labelExpandInfo);
    this->addChild(labelFriendLimit);
    
    //框内的信息
    SGCCLabelTTF *labelMakeSure_1 = SGCCLabelTTF::create(str_ExpandFriend_str4, FONT_BOXINFO, 28);
    CCSprite *spriteGoldIcon =  CCSprite::createWithSpriteFrameName("goldicon.png");
    SGCCLabelTTF *labelMakeSure_2 = SGCCLabelTTF::create(str_ExpandFriend_str5, FONT_BOXINFO, 28);
    SGCCLabelTTF *labelMakeSure_3 = SGCCLabelTTF::create(str_ExpandFriend_str6, FONT_BOXINFO, 28);
    labelMakeSure_1->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(-220, labelMakeSure_1->getContentSize().height)));
    spriteGoldIcon->setPosition(ccpAdd(labelMakeSure_1->getPosition(), ccp(labelMakeSure_1->getContentSize().width/2+spriteGoldIcon->getContentSize().width/2, 0)));
    labelMakeSure_2->setPosition(ccpAdd(spriteGoldIcon->getPosition(), ccp(labelMakeSure_2->getContentSize().width/2+spriteGoldIcon->getContentSize().width/2, 0)));
    labelMakeSure_3->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(0, -labelMakeSure_3->getContentSize().height)));
    this->addChild(labelMakeSure_1);
    this->addChild(labelMakeSure_2);
    this->addChild(labelMakeSure_3);
    this->addChild(spriteGoldIcon);
    
    //提示消息
    labelTip = SGCCLabelTTF::create(str_ExpandFriend_str7, FONT_BOXINFO, 24 , ccRED);
    labelTip->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(0, -130)));
    //modify by:zyc. merge into create.
    //labelTip->setColor(ccRED);
    labelTip->setVisible(false);
    addChild(labelTip);
    
    //增加按钮
    buttonAdd = SGButton::create("public_btn_1.png", "friend_add.png", this,menu_selector(SGExpandFriend::buttonClickAdd),CCPointZero,false,true);
    buttonAdd->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(0, -170)));
    this->addBtn(buttonAdd);
    
    this->isCanExpand();
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *titlecenter =  CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    
}

void SGExpandFriend::isCanExpand()
{
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("public/public.plist");
    
    if(SGPlayerInfo::sharePlayerInfo()->getExpandCount() >= 10)
    {
        buttonAdd->setEnabled(false);
        CCSprite *gray =  CCSprite::createWithSpriteFrameName("public_hui_1.png");
        buttonAdd->setBackFrame(gray->displayFrame());
        labelTip->setVisible(true);
    }else{
        buttonAdd->setEnabled(true);
        buttonAdd->setImage("public_btn_1.png");
        labelTip->setVisible(false);
    }
    
    labelExpandInfo->setString(CCString::createWithFormat(str_ExpandFriend_str8,SGPlayerInfo::sharePlayerInfo()->getExpandCount()*5)->getCString());
    labelFriendLimit->setString(CCString::createWithFormat(str_ExpandFriend_str9,SGPlayerInfo::sharePlayerInfo()->getFriendSize())->getCString());
}

void SGExpandFriend::buttonClickAdd()
{
    
    if(SGPlayerInfo::sharePlayerInfo()->getPlayerGold() < 60)
    {
        SGExpandBox *box = SGExpandBox::create(this);
        SGMainManager::shareMain()->showBox(box);
        return;
    }
    
    main::expandFriendRequest *expand = new main::expandFriendRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_EXPANDFRIEND,expand);
    
}

void SGExpandFriend::expandListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("expandListener");
        return;
    }
    
    //0; 如果大于最大扩展上限
    //1; 金子不够
    //2; 成功
    
    main::expandFriendResponse *response = (main::expandFriendResponse*)request->m_msg;
    if(response->state() == 0)
    {
        SG_SHOW_WINDOW(str_ExpandFriend_str10);
        EFFECT_PLAY(MUSIC_BTN);
        
    }
    else if(response->state() == 1)
    {
        SGExpandBox *box = SGExpandBox::create(this);
        SGMainManager::shareMain()->showBox(box);
        EFFECT_PLAY(MUSIC_BTN);
    }
    else if(response->state() == 2)
    {
        
        EFFECT_PLAY(MUSIC_34);
        int friendSize = SGPlayerInfo::sharePlayerInfo()->getFriendSize();
        int expandSize = SGPlayerInfo::sharePlayerInfo()->getExpandCount();
        int gole = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
        
        CCString *string = CCString::createWithFormat(str_ExpandFriend_str11,friendSize,friendSize+5);
        SGPlayerInfo::sharePlayerInfo()->setFriendSize(friendSize+5);
        SGPlayerInfo::sharePlayerInfo()->setExpandCount(expandSize+1);
        SGPlayerInfo::sharePlayerInfo()->setPlayerGold(gole-60);
        SG_SHOW_WINDOW(string->getCString());
        
        //刷新界面
        SGMainManager::shareMain()->updataUserMsg();
        SGMainManager::shareMain()->trackPurchase(CCString::create(str_ExpandFriend_str12), 1, 60);
        
    }else if(response->state() == 3)
    {
        if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
            SG_SHOW_WINDOW(str_ExpandFriend_str1012);
        }
        else
        {
            SG_SHOW_WINDOW(str_ExpandFriend_str101);
        }
        
 
    }
    
    this->isCanExpand();
    
}

void SGExpandFriend::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGFriend *friends = SGFriend::create();
    SGMainManager::shareMain()->showLayer(friends);
}

