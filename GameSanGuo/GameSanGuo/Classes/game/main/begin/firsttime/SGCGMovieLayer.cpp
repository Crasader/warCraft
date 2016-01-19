//
//  SGCGMovieLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#include "SGCGMovieLayer.h"
#include "SGCountryInfoLayer.h"
#include "SGCountryLayer.h"
#include "SGMainManager.h"
#include "SGPopWord.h"
#include "cocos-ext.h"
#include "ResourceManager.h"
#include "SGBattleManager.h"
#include "PlotModule.pb.h"
#include "LoginModule.pb.h"
#include "SGStringConfig.h"
#include "SGUpdateManager.h"

USING_NS_CC_EXT;

#define  CGMSGMOVEDIS  2
#define  CGMSGMOVERATE 15
#define  CGMSGMOVEGAP  0.04
SGCGMovieLayer::SGCGMovieLayer()
:cgScrollView(NULL),
finaleSprite(NULL),
scrollRect(CCRectZero),
posY(0.0f),
moveCgMsgDis(0),
s(CCSizeZero),
m_cdk(NULL),
box(NULL),
title(NULL),
btn(NULL),
btn2(NULL)
{
}

SGCGMovieLayer::~SGCGMovieLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_cgMovieLayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CGBATTLE_START);
    if (SGPlayerInfo::sharePlayerInfo()->getCDKeyNeed()==1) {
        SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CDK);
    }
    
}


SGCGMovieLayer *SGCGMovieLayer::create()
{
    SGCGMovieLayer *countryLayer = new SGCGMovieLayer();
    if (countryLayer && countryLayer->init(NULL, sg_cgMovieLayer))
    {
        
        countryLayer->constructView();
        countryLayer->autorelease();
        return countryLayer;
    }
    CC_SAFE_DELETE(countryLayer);
    return NULL;
}
void SGCGMovieLayer::constructView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CGBATTLE_START, this, callfuncO_selector(SGCGMovieLayer::cgBattleListener));
       s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("bg/cgBg.plist", RES_TYPE_LAYER_UI, sg_cgMovieLayer);

    CCSprite *cgBg = CCSprite::createWithSpriteFrameName("cgBg.png");
    cgBg->setAnchorPoint(ccp(0.5, 1));
    cgBg->ignoreAnchorPointForPosition(false);
    cgBg->cocos2d::CCNode::setScaleX(s.width/cgBg->getContentSize().width);
    cgBg->setPosition(ccp(s.width/2, s.height));
    addChild(cgBg);
    
//    SGButton *skipBt = SGButton::createFromLocal("box_btnbg.png", "跳 过", this, menu_selector(SGCGMovieLayer::cgSkipToBattle), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
//    skipBt->setPosition(ccp(s.width/2, skipBt->getContentSize().height*2));
//    addBtn(skipBt);
    
    CCLayerColor * blackBg = CCLayerColor::create(ccc4(0, 0, 0, 255), s.width, s.height);
    blackBg->setTouchEnabled(false);
    blackBg->ignoreAnchorPointForPosition(false);
    blackBg->setAnchorPoint(ccp(0, 0));
    blackBg->setPosition(ccp(0, 0));
    this->addChild(blackBg, -100);
    
    scrollRect = CCRectMake(0, s.height*0.2, s.width, s.height/4);
    cgScrollView = SNSScrollView::create(scrollRect);
    cgScrollView->setFrame(scrollRect);
    cgScrollView->setPosition(scrollRect.origin);
    cgScrollView->setHorizontal(false);
    cgScrollView->setVertical(true);
    cgScrollView->setTouchEnabled(false);
    this->addChild(cgScrollView);
    
//    CCSprite *scrollBg = CCSprite::createWithSpriteFrameName("barrack_kuang.png");
//    scrollBg->cocos2d::CCNode::setScale(s.width/scrollBg->getContentSize().width, s.height/4/scrollBg->getContentSize().height);
//    scrollBg->setPosition(ccpAdd(scrollRect.origin, ccp(s.width/2, s.height/8)));
//    addChild(scrollBg);
    
    char *spritData = new char[(int)s.width * (int)s.height *4];
    memset(spritData,0,(int)s.width * (int)s.height *4);
    int spriteDataSize = (int)s.width * (int)s.height*4;
    for (int j=0; j<spriteDataSize; j+=4) {
        spritData[j] = 0;
        spritData[j+1] = 0;
        spritData[j+2] = 0;
        spritData[j+3] = 255;
    }
    
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, (int)s.width, (int)s.height, s);
    
    finaleSprite = CCSprite::createWithTexture(pTexture);
    finaleSprite->setPosition(CCPointZero);
    finaleSprite->setAnchorPoint(CCPointZero);
    this->addChild(finaleSprite,10,10);
    finaleSprite->setVisible(false);
    finaleSprite->setOpacity(0);
    
    delete [] spritData;
    pTexture->autorelease();
    
    
    this->setTouchEnabled(true);
    
    const char *str[7] = { str_CGMovieLayer_str1,
                         str_CGMovieLayer_str2,
                        str_CGMovieLayer_str3,
                         str_CGMovieLayer_str4,
                        str_CGMovieLayer_str5,
                          str_CGMovieLayer_str6,
                         str_CGMovieLayer_str7};
    
    SGCCLabelTTF *cgMsg = NULL;
    posY =  -20;
    for (int i=0; i<7; i++) {
        cgMsg =  SGCCLabelTTF::create(str[i], FONT_BOXINFO, 34);
        cgMsg->cocos2d::CCNode::setPosition(s.width/2, posY);
        cgScrollView->m_body->addChild(cgMsg);
        posY -= 50;
    }
    moveCgMsgDis = CGMSGMOVEDIS;
    unschedule(schedule_selector(SGCGMovieLayer::moveCgMsgUp));
    if (SGPlayerInfo::sharePlayerInfo()->getCDKeyNeed()==1) {
        
        SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CDK, this, callfuncO_selector(SGCGMovieLayer::cdkListener));
        
        ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_cgMovieLayer);
//         ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist",RES_TYPE_LAYER_UI ,sg_registerLayer);
        
        
        box = CCSprite::createWithSpriteFrameName("register_diban.png");
        box->setPosition(SGLayout::getPoint(kMiddleCenter));
        this->addChild(box);

        m_cdk = CCEditBox::create(CCSizeMake(500, 60), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
        m_cdk->setPlaceHolder(str_CGMovieLayer_str8);
        m_cdk->setPlaceholderFontColor(ccc3(0x82,0x3e,0x02));
        m_cdk->setMaxLength(10);
        m_cdk->setReturnType(kKeyboardReturnTypeDone);
        m_cdk->setInputMode(kEditBoxInputModeSingleLine);
        m_cdk->setPosition(ccp(box->getContentSize().width*.5,box->getContentSize().height*.48));
//        m_cdk->setDelegate(this);
        box->addChild(m_cdk);
   
        
        
        title = CCSprite::createWithSpriteFrameName("gameactive.png");
        title->setPosition(ccp(box->getContentSize().width*.5,box->getContentSize().height*.8));
        box->addChild(title);
        //确定
        btn = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGCGMovieLayer::btnclick),CCPointZero,false,true);
        this->addBtn(btn);
        btn->setPosition(ccpAdd(box->getPosition(),ccp(-140,-btn->getContentSize().height*2.5)));
        //取消
        btn2 = SGButton::create("box_btnbg.png", "public_font_quxiao.png", this, menu_selector(SGCGMovieLayer::btnclickcancel),CCPointZero,false,true);
        this->addBtn(btn2);
        btn2->setPosition(ccpAdd(box->getPosition(),ccp(140,-btn->getContentSize().height*2.5)));

        
    }
    else
    {
    schedule(schedule_selector(SGCGMovieLayer::moveCgMsgUp), CGMSGMOVEGAP);
    }
}

void SGCGMovieLayer::btnclickcancel(SGButton *sender)
{
    SGMainManager::shareMain()->showwelcomelayer();
}

void SGCGMovieLayer::btnclick(SGButton *sender)
{
    //CCLog("%s",m_cdk->getText());
    if (!strcmp( m_cdk->getText(),""))
    {
        SG_SHOW_WINDOW(str_CGMovieLayer_str8);
    }
    else
    {
        main::CDKEYRequest *request = new main::CDKEYRequest();
        request->set_cdkey(m_cdk->getText());
        SGSocketClient::sharedSocketClient()->send(MSG_CDK, request);

    }
}

void SGCGMovieLayer::moveCgMsgUp()
{
   if (posY > s.height*0.01 && !finaleSprite->isVisible()) {
        finaleSprite->setVisible(true);
        finaleSprite->runAction(CCSequence::create(CCFadeIn::create(0.5),
                                                        CCCallFunc::create(this,  callfunc_selector(SGCGMovieLayer::cgSkipToBattle))
                                                         ,NULL));
       
       //发送   过场动画开始 的消息  add by:zyc.
       SGMainManager::sendOperatorRecord(100050);
    }
    else
    {
        posY += moveCgMsgDis;
        cgScrollView->m_body->setPositionY(cgScrollView->m_body->getPositionY() + moveCgMsgDis);
    }
}

void SGCGMovieLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool SGCGMovieLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    moveCgMsgDis = CGMSGMOVEDIS * CGMSGMOVERATE;
    return true;
    
//    if (scrollRect.containsPoint(pTouch->getLocation())) {
//        moveCgMsgDis = CGMSGMOVEDIS *2;
//        return true;
//    }
//    return false;
}

void SGCGMovieLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    moveCgMsgDis = CGMSGMOVEDIS;
}

void SGCGMovieLayer::cgSkipToBattle()
{
    unschedule(schedule_selector(SGCGMovieLayer::moveCgMsgUp));
    main::PlotPrologueStartRequest *sr = new main::PlotPrologueStartRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_CGBATTLE_START, sr);
}

void SGCGMovieLayer::cgBattleListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *socketRequest = (SGSocketRequest *)obj;
    if (socketRequest)
    {
        SGBattleManager::sharedBattle()->dealCgPveListener(obj);
    }
}

void SGCGMovieLayer::cdkListener(cocos2d::CCObject *obj)
{
   SGSocketRequest *request = (SGSocketRequest *)obj;
    main::CDKEYResponse *response = (main::CDKEYResponse *)request->m_msg;

    if (response)
    {
        if (response->state()==1 || 1) {
            
            m_cdk->setEnabled(false);
//            m_cdk->setVisible(false);
            btn->setVisible(false);
            btn->setEnabled(false);
//            title->setVisible(false);
            box->setVisible(false);
            btn2->setVisible(false);
            btn2->setEnabled(false);
            schedule(schedule_selector(SGCGMovieLayer::moveCgMsgUp), CGMSGMOVEGAP);
            
        }
        else
        {
            SG_SHOW_WINDOW(str_CGMovieLayer_str9);
        }
//        SGBattleManager::sharedBattle()->dealCgPveListener(obj);
    }
}




