//
//  SGSoulExchangeLayer.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/28/13.
//
//

#include "SGSoulExchangeLayer.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGTouchLayer.h"
#include "SGCantAdvanceBox.h"
#include "SGStaticDataManager.h"
#include "sgshoujingzhouEffectLayer.h"
#include "SGStringConfig.h"


//军魂换礼UI
#define PLIST_UI_SEC "sgsoulexchangelayer/secUi.plist"

//漩涡每旋转360°持续时间
//#define EDDY_ROTATE_TIME 5.6f//暂时注释
#define EDDY_ROTATE_LONG_TIME 9.0f
#define EDDY_ROTATE_SHORT_TIME 3.0f
//漩涡每次旋转角度
#define EDDY_ROTATE_ANGLE -360
//每次调用注入的短时间隔
#define INJECTION_INTERVAL 0.05f
//每次调用注入的单位增量
#define INJECTION_COUNT 15
//"点击打开"图形文本的淡入淡出时间
#define WAOPEN_FADE_TIME 0.5f
//军魂取回和领奖的进度条折腾时间
#define WITHDRAW_TIME 1.0f
//军魂取回消耗的元宝数量
#define WITHDRAW_COST 20

//======function!======
//构造
SGSoulExchangeLayer::SGSoulExchangeLayer()
{
    _enterType = -1;
    
    _activeProg = NULL;
    _activeLabel = NULL;
    _activeType = ST_NULL;
    
    _soulCost = 0;
    _soulTotal = 0;
    _labelTotal = NULL;
    
    _touchS = NULL;
    _imageS = NULL;
    _progS = NULL;
    _labelS = NULL;
    _boxS = NULL;
    _waOpenS = NULL;
    
    _touchA = NULL;
    _imageA = NULL;
    _progA = NULL;
    _labelA = NULL;
    _boxA = NULL;
    _waOpenA = NULL;
    
    _touchB = NULL;
    _imageB = NULL;
    _progB = NULL;
    _labelB = NULL;
    _boxB = NULL;
    _waOpenB = NULL;
	
	_labelPercentA = NULL;
	_labelPercentB = NULL;
	_labelPercentS = NULL;
	_curActiveLabel = NULL;
    
	percentA = 0;
	percentB = 0;
	percentS = 0;
}

//析构
SGSoulExchangeLayer::~SGSoulExchangeLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_soulExchangeLayer);
    _starListS.clear();
    _starListA.clear();
    _starListB.clear();
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->removeObserver(this, MSG_JUNHUN_INJECT);
    sgnc->removeObserver(this, MSG_JUNHUN_REWARD);
    sgnc->removeObserver(this, MSG_JUNHUN_WITHDRAW);
}

//创建
SGSoulExchangeLayer* SGSoulExchangeLayer::create(int et)
{
    SGSoulExchangeLayer* layer = new SGSoulExchangeLayer();
    if (layer && layer->init(NULL, sg_soulExchangeLayer))
    {
        layer->setEnterType(et);
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

//初始化视图
void SGSoulExchangeLayer::initView()
{
    //////////////////////////////////////////////////////////
    //添加监听
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_JUNHUN_INJECT, this, callfuncO_selector(SGSoulExchangeLayer::recvInjectSoulResponse)); //灌注军魂
    sgnc->addObserver(MSG_JUNHUN_REWARD, this, callfuncO_selector(SGSoulExchangeLayer::recvGetRewardResponse)); //军魂领奖
    sgnc->addObserver(MSG_JUNHUN_WITHDRAW, this, callfuncO_selector(SGSoulExchangeLayer::recvWithdrawSoulResponse)); //取回军魂
    
    //加载图形
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture(PLIST_UI_SEC, RES_TYPE_LAYER_UI, sg_soulExchangeLayer);
//    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    //标题栏大系列
    CCSprite* title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg);
    SGCCLabelTTF* font_title = SGCCLabelTTF::create(str_SoulExchangeLayer_str1, FONT_LISHU, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
    //font_title->setColor(ccc3(0xff, 0x95, 0x0c));
  
//    CCSprite *font_title = CCSprite::createWithSpriteFrameName("barrack_font_kpzssx.png");
    font_title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(font_title);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r);
    
    //返回按钮
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGSoulExchangeLayer::btnBackHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSoulExchangeLayer::btnBackHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    //军魂取回按钮
    SGButton *btnWithdraw = SGButton::create("chatbtn.png",
                                         "sec_font_withdraw.png",
                                         this,
                                         menu_selector(SGSoulExchangeLayer::btnWithdrawHandler),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(btnWithdraw);
    btnWithdraw->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp( -btnWithdraw->getContentSize().width*.55, - btnWithdraw->getContentSize().height*.55)));
    
    //获得数据
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    
    //基准坐标
    CCPoint midCnt = SGLayout::getPoint(kMiddleCenter);
    CCPoint topCnt = SGLayout::getPoint(kUpCenter);
    CCPoint btmCnt = SGLayout::getPoint(kBottomCenter);
    
    //无敌背板
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_soulExchangeLayer);

    CCSprite* bigBkg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCPoint posBb = midCnt;
    bigBkg->setScaleX(size.width/bigBkg->getContentSize().width * 1.01);
    bigBkg->setPosition(posBb);
    this->addChild(bigBkg, -1);
    
    //总量底板
    CCSprite* imgTitleBar = CCSprite::createWithSpriteFrameName("sec_bar_amount.png");
    CCPoint posItb = ccpAdd(topCnt, ccp(0, -title_bg->getContentSize().height - 55));
    imgTitleBar->setPosition(posItb);
    this->addChild(imgTitleBar);
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("3");
    nJunHunMax=dic1->valueForKey("value")->intValue();
    //总量数字
    _labelTotal = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", data->getSoulAmount(),nJunHunMax)->getCString(), FONT_BOXINFO, 30);
    CCPoint _posLt = ccp(343, 31);
    _labelTotal->setPosition(_posLt);
    imgTitleBar->addChild(_labelTotal);
    
    CCPoint anchorLftCnt = ccp(0, 0.5);
    
    //传奇/精粹/荆州
    generateInnerLayers(ST_SSS);
    generateInnerLayers(ST_AAA);
    generateInnerLayers(ST_BBB);
    
    //底板边框
    float prmWidth = 100;
    float prmHeight = 194;
    float trgWidth = screenwidth;
    float trgHeight = 194;
    
    CCSprite* btmBkg = CCSprite::createWithSpriteFrameName("sec_frame_bottom.png");
    CCPoint posBtmBkg = ccpAdd(btmCnt, ccp(0, trgHeight * 0.5f));
    btmBkg->setScaleX(trgWidth / prmWidth);
    btmBkg->setScaleY(trgHeight / prmHeight);
    btmBkg->setPosition(posBtmBkg);
    this->addChild(btmBkg);
    
    //说明文字
    CCSize dimen = CCSizeMake(trgWidth * 0.8, 0);
    
    SGCCLabelTTF* ttf1 = SGCCLabelTTF::create(str_SoulExchangeLayer_str2, FONT_BOXINFO, 25, dimen, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    ttf1->setAnchorPoint(anchorLftCnt);
    CCPoint posTtf1 = ccpAdd(posBtmBkg, ccp(-trgWidth * 0.4, 60));
    ttf1->setPosition(posTtf1);
    this->addChild(ttf1);
    
    SGCCLabelTTF* ttf2 = SGCCLabelTTF::create(str_SoulExchangeLayer_str3, FONT_BOXINFO, 25, dimen, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    ttf2->setAnchorPoint(anchorLftCnt);
    CCPoint posTtf2 = ccpAdd(posBtmBkg, ccp(-trgWidth * 0.4, 22));
    ttf2->setPosition(posTtf2);
    this->addChild(ttf2);
    
    SGCCLabelTTF* ttf3 = SGCCLabelTTF::create(str_SoulExchangeLayer_str4, FONT_BOXINFO, 25, dimen, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    ttf3->setAnchorPoint(anchorLftCnt);
    CCPoint posTtf3 = ccpAdd(posBtmBkg, ccp(-trgWidth * 0.4, -20));
    ttf3->setPosition(posTtf3);
    this->addChild(ttf3);
    
    //检查箱子
    checkBoxOpen();
    
//    //TODO: 测试删除
//    data->setIsShowBonus(true);
//    data->setBonusDamage(10000);
//    data->setBonusSoul(1000);
    
    //判断是否需要为战绩提示
    if ( data->getIsShowBonus() )
    {
        showFinalInfoNotice();
    }
    
    _canOpenBox = true;
}

//产生三大件，在这里进行子节点的添加
void SGSoulExchangeLayer::generateInnerLayers(SoulTag type)
{
    //获得数据
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    
    //基准坐标
    CCPoint midCnt = SGLayout::getPoint(kMiddleCenter);
    
    ccColor4B opacity = ccc4(0, 0, 0, 0);
//    ccColor4B opacity = ccc4(0, 255, 0, 255);
    CCPoint anchorBtmCnt = ccp(0.5, 0);
    int zd1 = 1;
    int zd2 = 2;
    int zd4 = 4;
    int zd6 = 6;
    int zd8 = 8;
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    //主体层
    float lyWidth = 200;
    float lyHeight = 590;
    CCLayerColor* layerS = CCLayerColor::create(opacity, lyWidth, lyHeight);
    layerS->ignoreAnchorPointForPosition(false);
    CCPoint posLyS;
    if (type == ST_SSS)
        posLyS = ccpAdd(midCnt, ccp(-lyWidth, 0));
    else if (type == ST_AAA)
        posLyS = midCnt;
    else
        posLyS = ccpAdd(midCnt, ccp(+lyWidth, 0));
    layerS->setPosition(posLyS);
    this->addChild(layerS,1,type+100);//kanata
    
    //触控
    float tsWidth = 90;
    float tsHeight = 80;
    SGTouchLayer* tcLayer = SGTouchLayer::create(tsWidth, tsHeight, -50, opacity);
    CCPoint posTsS = ccp(100, 90);
    tcLayer->setTag(ST_SSS);
    tcLayer->setPosition(posTsS);
    
    PFBegan began;
    PFMoved move;
    PFEnded end;
    PFCancelled cancell;
    if (type == ST_SSS)
    {
        began = pf_began_selector(SGSoulExchangeLayer::helloBeganSSS);
        move =pf_moved_selector(SGSoulExchangeLayer::helloMovedSSS);
        end =pf_ended_selector(SGSoulExchangeLayer::helloEndedSSS);
        cancell =pf_cancelled_selector(SGSoulExchangeLayer::helloCancelledSSS);
    }
    else if (type == ST_AAA)
    {
        began = pf_began_selector(SGSoulExchangeLayer::helloBeganAAA);
        move =pf_moved_selector(SGSoulExchangeLayer::helloMovedAAA);
        end =pf_ended_selector(SGSoulExchangeLayer::helloEndedAAA);
        cancell =pf_cancelled_selector(SGSoulExchangeLayer::helloCancelledAAA);
    }
    else
    {
        began = pf_began_selector(SGSoulExchangeLayer::helloBeganBBB);
        move =pf_moved_selector(SGSoulExchangeLayer::helloMovedBBB);
        end =pf_ended_selector(SGSoulExchangeLayer::helloEndedBBB);
        cancell =pf_cancelled_selector(SGSoulExchangeLayer::helloCancelledBBB);
    }
    
//    tcLayer->setControlMode(this, began, pf_moved_selector(SGSoulExchangeLayer::helloMoved), pf_ended_selector(SGSoulExchangeLayer::helloEnded), pf_cancelled_selector(SGSoulExchangeLayer::helloCancelled));
      tcLayer->setControlMode(this, began, move, end, cancell);
    layerS->addChild(tcLayer);

    //注入
    CCSprite* spImg = CCSprite::createWithSpriteFrameName("sec_btn_inject.png");
    CCPoint posImgS = posTsS;
    spImg->setPosition(posImgS);
    layerS->addChild(spImg, zd8);
    
    if (type == ST_SSS)
        _imageS = spImg;
    else if (type == ST_AAA)
        _imageA = spImg;
    else
        _imageB = spImg;
    
    //漩涡们
    ////////坑爹漩涡特效需求
    /////////////
    if (type == ST_SSS)
       {
           eddyS_S = CCSprite::createWithSpriteFrameName("sec_img_eddy.png");
           CCPoint posEdyS = posImgS;
           eddyS_S->setPosition(posEdyS);
           eddyS_S->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
           layerS->addChild(eddyS_S, zd6);
       }
    
    else if (type == ST_AAA)
       {
           eddyS_A = CCSprite::createWithSpriteFrameName("sec_img_eddy.png");
           CCPoint posEdyS = posImgS;
           eddyS_A->setPosition(posEdyS);
           eddyS_A->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
           layerS->addChild(eddyS_A, zd6);
       }
    else
       {
           eddyS_B = CCSprite::createWithSpriteFrameName("sec_img_eddy.png");
           CCPoint posEdyS = posImgS;
           eddyS_B->setPosition(posEdyS);
            eddyS_B->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
           layerS->addChild(eddyS_B, zd6);
       
       }
    
    

    //进度条底图
    CCSprite* progBkgS = CCSprite::createWithSpriteFrameName("sec_prog_bar.png");
    progBkgS->setAnchorPoint(anchorBtmCnt);
    CCPoint posPbgS = posTsS;
    progBkgS->setPosition(posPbgS);
    layerS->addChild(progBkgS, zd2);
    
    //进度条动图
    const char* progPath;
    if (type == ST_SSS)
        progPath = "sec_prog_orange.png";
    else if (type == ST_AAA)
        progPath = "sec_prog_blue.png";
    else
        progPath = "sec_prog_green.png";
    
    CCSprite* progBarS = CCSprite::createWithSpriteFrameName(progPath);
    CCPoint posPbrS = posPbgS;
    CCProgressTimer* progTm = CCProgressTimer::create(progBarS);
    progTm->setType(kCCProgressTimerTypeBar);
    progTm->setMidpoint(ccp(0,0));
    progTm->setBarChangeRate(ccp(0,1));
    progTm->setAnchorPoint(anchorBtmCnt);
    progTm->setPosition(ccpAdd(posPbrS, ccp(-2, 38))); //左偏2px，上偏移38px
    float cur = data->getSoulCurByIndex(type);
    float max = data->getSoulMaxByIndex(type);
    progTm->setPercentage( cur / max * 100 );
    layerS->addChild(progTm, zd4);
	
	//数字进度
	CCLabelTTF *percentTm = CCLabelTTF::create(CCString::createWithFormat("%d%%", (int)(cur / max * 100))->getCString(), FONT_BOXINFO, 24);
	percentTm->setColor(ccGREEN);
	layerS->addChild(percentTm, zd4 + 1);
	percentTm->setPosition(ccpAdd(progTm->getPosition(), ccp(0, 100)));
	
	
    
    if (type == ST_SSS)
	{
		_progS = progTm;
		_labelPercentS = percentTm;
	}
    else if (type == ST_AAA)
	{
		_progA = progTm;
		_labelPercentA = percentTm;
	}
    else
	{
		_progB = progTm;
		_labelPercentB = percentTm;
	}
    
    //文字掠过
    
    //星云
    const char* nebulaPath;
    if (type == ST_SSS)
        nebulaPath = "sec_img_nebula_gold.png";
    else if (type == ST_AAA)
        nebulaPath = "sec_img_nebula_silver.png";
    else
        nebulaPath = "sec_img_nebula_bronze.png";
    
    CCSprite* nebulaS = CCSprite::createWithSpriteFrameName(nebulaPath);
    CCPoint posNebS = ccp(100, 504);
    nebulaS->setPosition(posNebS);
    layerS->addChild(nebulaS, zd1);
    
    //宝箱
    const char* boxPath;
    if (type == ST_SSS)
        boxPath = "sec_box_gold.png";
    else if (type == ST_AAA)
        boxPath = "sec_box_silver.png";
    else
        boxPath = "sec_box_bronze.png";
    
    CCSprite* spBox = CCSprite::createWithSpriteFrameName(boxPath);
    CCPoint posBoxS = posNebS;
//    spBox->setPosition(posBoxS);
     spBox->setPosition(ccpAdd(posBoxS,ccp(0, s.height*0.4)));
    ///////////kanata   宝箱横空出世前位置
   
    
    CCMoveTo *move1=CCMoveTo::create(0.1f, posNebS);
    CCMoveTo *move2=CCMoveTo::create(0.1f,  ccpAdd(posNebS, ccp(0, s.height*0.02)));
    CCMoveTo *move3=CCMoveTo::create(0.1f, posNebS);
   
//    CCMoveBy *move1=CCMoveBy::create(0.1f, ccp(0, -s.height*0.4));
//    CCMoveBy *move2=CCMoveBy::create(0.1f, ccp(0, s.height*0.03));
//    CCMoveBy *move3=CCMoveBy::create(0.1f, ccp(0, -s.height*0.03));
    CCAction*action=CCSequence::create(move1,CCDelayTime::create(0.1f), move2,CCDelayTime::create(0.01f),move3,NULL);
   
    
    
    //////////////
    layerS->addChild(spBox, zd4);
    
    if (type == ST_SSS)
      {
        _boxS = spBox;
        _boxS->runAction(action);
      }
    
    else if (type == ST_AAA)
      {
        _boxA = spBox;
        _boxA->runAction(action);
      }
    else
      {
        _boxB = spBox;
        _boxB->runAction(action);
      }
    
    //点击打开
    CCSprite* spOpen = CCSprite::createWithSpriteFrameName("sec_font_open_coffer.png");
    CCPoint posOpS = posBoxS;
    spOpen->setPosition(posOpS);
    spOpen->setVisible(false);
    layerS->addChild(spOpen, zd8);
    
    if (type == ST_SSS)
        _waOpenS = spOpen;
    else if (type == ST_AAA)
        _waOpenA = spOpen;
    else
        _waOpenB = spOpen;
    
    //星星众
    float ndWidth = 180;
    float ndHeight = 170;
    
    CCLayerColor* node = CCLayerColor::create(opacity, ndWidth, ndHeight);
    node->ignoreAnchorPointForPosition(false);
    CCPoint posNdS = posBoxS;
    node->setPosition(posNdS);
    layerS->addChild(node, zd6);
    
    std::vector<CCSprite*>* vec;
    if (type == ST_SSS)
        vec = &_starListS;
    else if (type == ST_AAA)
        vec = &_starListA;
    else
        vec = &_starListB;
    //大猩猩 小猩猩
    const char* starBig = "sec_img_star_big.png";
    const char* starSml = "sec_img_star_sml.png";
    //
    if (type == ST_SSS) //传奇，5颗星
    {
        CCSprite* st1 = CCSprite::createWithSpriteFrameName(starBig);
        st1->setPosition(ccp(49, 136));
        node->addChild(st1);
        vec->push_back(st1);
        
        CCSprite* st2 = CCSprite::createWithSpriteFrameName(starSml);
        st2->setPosition(ccp(116, 135));
        node->addChild(st2);
        vec->push_back(st2);
        
        CCSprite* st3 = CCSprite::createWithSpriteFrameName(starSml);
        st3->setPosition(ccp(136, 103));
        node->addChild(st3);
        vec->push_back(st3);

        CCSprite* st4 = CCSprite::createWithSpriteFrameName(starSml);
        st4->setPosition(ccp(141, 39));
        node->addChild(st4);
        vec->push_back(st4);

        CCSprite* st5 = CCSprite::createWithSpriteFrameName(starSml);
        st5->setPosition(ccp(30, 60));
        node->addChild(st5);
        vec->push_back(st5);
    }
    else if (type == ST_AAA) //精粹，3颗星
    {
        CCSprite* st1 = CCSprite::createWithSpriteFrameName(starBig);
        st1->setPosition(ccp(49, 136));
        node->addChild(st1);
        vec->push_back(st1);
        
        CCSprite* st2 = CCSprite::createWithSpriteFrameName(starSml);
        st2->setPosition(ccp(116, 135));
        node->addChild(st2);
        vec->push_back(st2);
        
        CCSprite* st3 = CCSprite::createWithSpriteFrameName(starSml);
        st3->setPosition(ccp(136, 103));
        node->addChild(st3);
        vec->push_back(st3);
    }
    else //荆州，1颗星
    {
        CCSprite* st1 = CCSprite::createWithSpriteFrameName(starBig);
        st1->setPosition(ccp(49, 136));
        node->addChild(st1);
        vec->push_back(st1);
    }
    
    //增加开箱触控
    float ckWidth = spBox->getContentSize().width;
    float ckHeight = spBox->getContentSize().height;
    SGTouchLayer* ckTc = SGTouchLayer::create(ckWidth, ckHeight, -50, opacity);
    ckTc->setPosition(posBoxS);
    PFCallBack call;
    if (type == ST_SSS)
        call = pf_callback_selector(SGSoulExchangeLayer::openCofferHandlerS);
    else if (type == ST_AAA)
        call = pf_callback_selector(SGSoulExchangeLayer::openCofferHandlerA);
    else
        call = pf_callback_selector(SGSoulExchangeLayer::openCofferHandlerB);
    ckTc->setButtonMode(this, call);
    layerS->addChild(ckTc);
    
//    //给主武添加触控回调
//    skillLayer = SGTouchLayer::create(frame2->getContentSize().width, frame2->getContentSize().height, 0 /*, ccc4(0, 255, 0, 255)*/);
//    skillLayer->ignoreAnchorPointForPosition(false);
//    skillLayer->setButtonMode( this, (PFCallBack) (&SGGeneralInfoLayer::showSkillInfo) );
//    bigFrm->addChild(skillLayer, 10);
//    skillLayer->setPosition(frame2->getPosition());
}

//返回按钮
void SGSoulExchangeLayer::btnBackHandler()
{
    _waOpenS = NULL;
    _waOpenA = NULL;
    _waOpenB = NULL;
    if (1 == _enterType) //释放本UI&返回守荆州UI
    {
        SGMainManager::shareMain()->showshoujingzhou();
    }
    else if (2 == _enterType) //释放本UI&返回主界面   //战败了
    {
        SGMainManager::shareMain()->showshoujingzhou();
    }
    else //默认返回主界面
    {
        SGMainManager::shareMain()->showFirstLayer();
    }
}

//取回军魂按钮
void SGSoulExchangeLayer::btnWithdrawHandler()
{
    //获得数据

    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    ////S箱子现在注入的军魂数量
    float cur_s = data->getSoulCurByIndex(ST_SSS);
    ////A箱子现在注入的军魂数量
    float cur_a = data->getSoulCurByIndex(ST_AAA);
     ////B箱子现在注入的军魂数量
     float cur_b = data->getSoulCurByIndex(ST_BBB);
    if((cur_a==0)&& (cur_b==0)&& (cur_s==0))
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 60, WITHDRAW_COST, NULL);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
    
    
    //////////
    else
    {
    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 53, WITHDRAW_COST, NULL);
    SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
}

//发送消息
void SGSoulExchangeLayer::sendInjectSoulRequest()
{
    CCLOG("SGSoulExchangeLayer::sendInjectSoulRequest() index=%d cost=%d total=%d", _activeType, _soulCost, _soulTotal);
    
//    //TODO: 测试bgn
//    //数据正式化
//    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
//    data->setSoulAmount(_soulTotal);
//    data->setSoulCurByIndex(_activeType, _activeAmntCur + _soulCost);
//    
//    //在这里重置所有
//    _activeProg->stopAllActions();
//    _activeProg = NULL;
//    _activeLabel = NULL;
//    _activeType = ST_NULL;
//    _activeAmntCur = -1;
//    _activeAmntMax = -1;
//    //检查箱子
//    checkBoxOpen();
//    return;
//    //TODO: 测试end
    
    //有真正的动作才发送消息
    if (_soulCost > 0 && _activeType != ST_NULL)
    {
        main::JunhunInjectRequest* req = new main::JunhunInjectRequest();
        req->set_index(_activeType);
        req->set_count(_soulCost);
        SGSocketClient::sharedSocketClient()->send(MSG_JUNHUN_INJECT, req);
    }
    else
    {
        _activeLabel = NULL;
        _activeType = ST_NULL;
        _activeAmntCur = -1;
        _activeAmntMax = -1;
    }
}

//接收消息
void SGSoulExchangeLayer::recvInjectSoulResponse(CCObject* obj)
{
    //在接收中再判断是否可以打开箱子;
    CCLOG("SGSoulExchangeLayer::recvInjectSoulResponse(CCObject* obj) state=%d cost=%d total=%d", _activeType, _soulCost, _soulTotal);
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    if (sr)
    {
        main::JunhunInjectResponse* rsp = (main::JunhunInjectResponse*) sr->m_msg;
        if (rsp)
        {
            if (rsp->state()) //成功设置
            {
                //数据正式化
                SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
                data->setSoulAmount(_soulTotal);
                data->setSoulCurByIndex(_activeType, _activeAmntCur + _soulCost);
            }
            else //错误
            {
                SGMainManager::shareMain()->showMessage(str_SoulExchangeLayer_str5);
            }
        }
    }
    //在这里重置指示变量
    _activeLabel = NULL;
    _activeType = ST_NULL;
    _activeAmntCur = -1;
    _activeAmntMax = -1;
    //检查箱子
    checkBoxOpen();
}

//取回军魂消息send
void SGSoulExchangeLayer::sendWithdrawSoulRequest()
{
    main::JunhunWithdrawRequest* req = new main::JunhunWithdrawRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_JUNHUN_WITHDRAW, req);
}

//开箱子时候,进度数字递减动画
void SGSoulExchangeLayer::updateActiveLabel(float dt)
{
	if (_activeType == ST_AAA)
	{
		if (percentA == 0)
		{
			_activeType = ST_NULL;
			unschedule(schedule_selector(SGSoulExchangeLayer::updateActiveLabel));
		}
		percentA -= 5;
		(percentA < 0) && (percentA = 0);
		_labelPercentA->setString(CCString::createWithFormat("%d%%", percentA)->getCString());
	}
	else if (_activeType == ST_BBB)
	{
		if (percentB == 0)
		{
			_activeType = ST_NULL;
			unschedule(schedule_selector(SGSoulExchangeLayer::updateActiveLabel));
		}
		percentB -= 5;
		(percentB < 0) && (percentB = 0);
		_labelPercentB->setString(CCString::createWithFormat("%d%%", percentB)->getCString());
	}
	else if (_activeType == ST_SSS)
	{
		if (percentS == 0)
		{
			_activeType = ST_NULL;
			unschedule(schedule_selector(SGSoulExchangeLayer::updateActiveLabel));
		}
		percentS -= 5;
		(percentS < 0) && (percentS = 0);
		_labelPercentS->setString(CCString::createWithFormat("%d%%", percentS)->getCString());
	}
	
}

//动态更新进度标签的数字
void SGSoulExchangeLayer::updateLabelValue(float dtime)
{
	
	if (percentA <= 0 && percentB <= 0 && percentS <= 0)
	{
		
		SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();

		percentA = data->getSoulCurByIndex(ST_AAA) / (float)data->getSoulMaxByIndex(ST_AAA) * 100;
		percentB = data->getSoulCurByIndex(ST_BBB) / (float)data->getSoulMaxByIndex(ST_BBB) * 100;
		percentS = data->getSoulCurByIndex(ST_SSS) / (float)data->getSoulMaxByIndex(ST_SSS) * 100;
		
		//移除定时器
		unschedule(schedule_selector(SGSoulExchangeLayer::updateLabelValue));
		
		_labelPercentA->setString(CCString::createWithFormat("%d%%", percentA)->getCString());
		_labelPercentB->setString(CCString::createWithFormat("%d%%", percentB)->getCString());
		_labelPercentS->setString(CCString::createWithFormat("%d%%", percentS)->getCString());
		return ;
	}
	
	int a = (percentA -= 5) < 0 ? 0 : percentA;
	int b = (percentB -= 5) < 0 ? 0 : percentB;
	int s = (percentS -= 5) < 0 ? 0 : percentS;
	
	_labelPercentA->setString(CCString::createWithFormat("%d%%", a)->getCString());
	_labelPercentB->setString(CCString::createWithFormat("%d%%", b)->getCString());
	_labelPercentS->setString(CCString::createWithFormat("%d%%", s)->getCString());


}

//取回军魂消息recv
void SGSoulExchangeLayer::recvWithdrawSoulResponse(CCObject* obj)
{
//    //测试弹出储值
//    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 54, WITHDRAW_COST, NULL);
//    SGMainManager::shareMain()->showBox(cantadvanceBox);
//    return;
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    if (sr)
    {
        main::JunhunWithdrawResponse* rsp = (main::JunhunWithdrawResponse*) sr->m_msg;
        if (rsp)
        {
            const char* text = NULL;
            if (rsp->state()) //允许取回
            {
                text = str_SoulExchangeLayer_str6;
                //数据
                SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
				
				//获取每一个进度条上对应的百分比,用于显示到进度条上
				percentA = data->getSoulCurByIndex(ST_AAA) / (float)data->getSoulMaxByIndex(ST_AAA) * 100;
				percentB = data->getSoulCurByIndex(ST_BBB) / (float)data->getSoulMaxByIndex(ST_BBB) * 100;
				percentS = data->getSoulCurByIndex(ST_SSS) / (float)data->getSoulMaxByIndex(ST_SSS) * 100;
				
				CCLOG("[Soul]A:%d | B:%d | S:%d", percentA, percentB, percentS);
				
				
                int total = data->getSoulCurOfS() + data->getSoulCurOfA() + data->getSoulCurOfB() + data->getSoulAmount();
                data->setSoulAmount(total);
                data->setSoulCurOfS(0);
                data->setSoulCurOfA(0);
                data->setSoulCurOfB(0);
                //图形
                _labelTotal->setString(CCString::createWithFormat("%d/%d", total,nJunHunMax)->getCString());
                _progS->runAction(CCProgressFromTo::create(WITHDRAW_TIME, _progS->getPercentage(), 0));
                _progA->runAction(CCProgressFromTo::create(WITHDRAW_TIME, _progA->getPercentage(), 0));
                _progB->runAction(CCProgressFromTo::create(WITHDRAW_TIME, _progB->getPercentage(), 0));
				
				//定时器
				int max = MAX(percentA, MAX(percentB, percentS));
				float v = WITHDRAW_TIME / max * 1.1f;
				schedule(schedule_selector(SGSoulExchangeLayer::updateLabelValue), v);
				
            }
            else //取回错误
            {
                int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
                if (gold < WITHDRAW_COST) //元宝不够
                {
                    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 54, WITHDRAW_COST, NULL);
                    SGMainManager::shareMain()->showBox(cantadvanceBox);
                }
                else
                {
                    text =str_SoulExchangeLayer_str7;
                }
            }
            if (text)
                SGMainManager::shareMain()->showMessage(text);
        }
    }
    checkBoxOpen(); //偷懒
}
















//灌注时的数字操作
void SGSoulExchangeLayer::changeSoulAmount()
{


    
    ////////////
    //无军魂，已加满，则停止。
    if (_soulTotal <= 0 || _activeAmntCur + _soulCost >= _activeAmntMax)
    {
        //openCofferHandlerB();

        stopInjection();
    }
    else
    {
        int add = 0;
        //判断剩余的是否够
//        if (_soulTotal <= INJECTION_COUNT) //不够全用
        int injectnum=0; ///注入的军魂量
        //////////////不同条件下不同速率
        if(_activeAmntMax==3000)
        {
            injectnum=_activeAmntMax*0.04;
        }
        else if (_activeAmntMax==10000)
        {
            injectnum=_activeAmntMax*0.02;
        }
        else
        {
             injectnum=_activeAmntMax*0.013;
        }
        
        
        
        
        ////////////
//        if (_soulTotal <= _activeAmntMax*0.02) //所涨经验值条件变更  变为百分比
         if (_soulTotal <= injectnum) //所涨经验值条件变更  变为百分比
        //////////////////
        
            add = _soulTotal;
        else //够了用标准值
//            add = INJECTION_COUNT;
            ////////////
            
//            add = _activeAmntMax*0.02;
             add = injectnum;
            ///////////
        //判断本次灌注是否浪费
        if (_activeAmntCur + _soulCost + add >= _activeAmntMax)
            add = _activeAmntMax - _activeAmntCur - _soulCost;
        //正式加减
        _soulTotal -= add;
        _soulCost += add;
        float prcn = (float) (_activeAmntCur + _soulCost) / _activeAmntMax * 100;
        CCLOG("SGSoulExchangeLayer::changeSoulAmount() percent=%f cost=%d max=%d total=%d", prcn, _soulCost, _activeAmntMax, _soulTotal);
        _activeProg->setPercentage( prcn );
		_curActiveLabel->setString(CCString::createWithFormat("%d%%", (int)(prcn))->getCString());
        _labelTotal->setString(CCString::createWithFormat("%d/%d", _soulTotal,nJunHunMax)->getCString());
    }
}

//开始灌注，生成当前活动进度条/数字标签/索引
void SGSoulExchangeLayer::startInjection(SoulTag type)
{
    
    
    
    
 
    if (_activeType != ST_NULL || type == ST_NULL)
        return;
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    _soulCost = 0;
    _soulTotal = data->getSoulAmount();
    
    if (ST_SSS == type) //传奇
    {
        _activeProg = _progS;
		_curActiveLabel = _labelPercentS;
        
        ///////////要做成start时漩涡转   end时不转 //呵呵  又改了   开始时候转的慢   注入时候转的快
    // eddyS_S->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
        eddyS_S->stopAllActions();
        eddyS_S->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_SHORT_TIME, EDDY_ROTATE_ANGLE)));
    }
    else if (ST_AAA == type) //精粹
    {
     //    eddyS_A->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_TIME, EDDY_ROTATE_ANGLE)));
        eddyS_A->stopAllActions();
        eddyS_A->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_SHORT_TIME, EDDY_ROTATE_ANGLE)));

        _activeProg = _progA;
		_curActiveLabel = _labelPercentA;
    }
    else //荆州
    {
    //     eddyS_B->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_TIME, EDDY_ROTATE_ANGLE)));
        eddyS_B->stopAllActions();
        eddyS_B->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_SHORT_TIME, EDDY_ROTATE_ANGLE)));
        _activeProg = _progB;
		
		_curActiveLabel = _labelPercentB;
    }
    _activeType = type;
    _activeAmntCur = data->getSoulCurByIndex(type);
    
    _activeAmntMax = data->getSoulMaxByIndex(type);
    
    CCSequence* away = CCSequence::create(CCDelayTime::create(INJECTION_INTERVAL), CCCallFunc::create(this, callfunc_selector(SGSoulExchangeLayer::changeSoulAmount)), NULL);
    CCRepeatForever* toto = CCRepeatForever::create(away);
    _activeProg->runAction(toto);
}

//终止灌注
void SGSoulExchangeLayer::stopInjection()
{
    if (!_activeProg)
        return;
    _activeProg->stopAllActions();
    _activeProg = NULL;
    //试图发送请求
    sendInjectSoulRequest();
    
}
//////////终止灌注函数重载
void SGSoulExchangeLayer::stopInjection(SoulTag type)
{
    /////////
    if (ST_SSS == type)
    {
   
        eddyS_S->stopAllActions();//停止注入军魂时候速度变回原来速度
        //又该需求，暂时加上  说不定啥时又变回来
         eddyS_S->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
    }
    else if (ST_AAA == type)
    {
        eddyS_A->stopAllActions();
        /////
         eddyS_A->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
    }
    else //荆州
    {
        eddyS_B->stopAllActions();
        //////
         eddyS_B->runAction(CCRepeatForever::create(CCRotateBy::create(EDDY_ROTATE_LONG_TIME, EDDY_ROTATE_ANGLE)));
    }

    
    /////////
    if (!_activeProg)
        return;
    _activeProg->stopAllActions();
    _activeProg = NULL;
    //试图发送请求
    sendInjectSoulRequest();
}


////////////

//试图领奖send，传入SoulTag区分是哪个箱子
void SGSoulExchangeLayer::sendGetRewardRequest(SoulTag type)
{
    if (_canOpenBox)
    {
        CCLOG("sendCanOpenBox=%d", _canOpenBox);
        SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
        int cur = data->getSoulCurByIndex(type);
        int max = data->getSoulMaxByIndex(type);
        if (cur > 0 && cur >= max)
        {
            _activeType = type;
            if (ST_SSS == type) //传奇
			{
				_activeProg = _progS;
				_curActiveLabel = _labelPercentS;
			}
            else if (ST_AAA == type) //精粹
			{
				_curActiveLabel = _labelPercentA;
                _activeProg = _progA;
            
			}
			else //荆州
			{
				_activeProg = _progB;
				_curActiveLabel = _labelPercentB;
			}
            _canOpenBox = false;
            main::JunhunRewardRequest* req = new main::JunhunRewardRequest;
            req->set_index(type);
            SGSocketClient::sharedSocketClient()->send(MSG_JUNHUN_REWARD, req);
            CCLOG("SGSoulExchangeLayer::sendGetRewardRequest(SoulTag type = %d) done", type);
        }
        else
        {
            _canOpenBox = true;
            _activeType = ST_NULL;
           // _activeProg = NULL;
			_curActiveLabel = NULL;
            CCLOG("SGSoulExchangeLayer::sendGetRewardRequest(SoulTag type = %d) not qualified", type);
        }
    }
    else
    {
        //等待上一个箱子打开完成
        CCLOG("canOpenBox=%d", _canOpenBox);
    }
}

//试图领奖recv
void SGSoulExchangeLayer::recvGetRewardResponse(CCObject* obj)
{
    EFFECT_PLAY(MUSIC_43);
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    if (sr)
    {
        main::JunhunRewardResponse* rsp = (main::JunhunRewardResponse*) sr->m_msg;
        
        
        if (rsp)
        {
            if (rsp->state() && _activeType != ST_NULL && _activeProg != NULL) //领取成功
            {
//                //读取奖励，暂时用文字来处理。
//                std::vector<CCString*> vec;
                //SeRewardTypeId classdata;
                std::vector<SeRewardTypeId*> vec;
                SGStaticDataManager* sdm = SGStaticDataManager::shareStatic();
                SeRewardTypeId* temp = NULL;
                for (int k = 0; k < rsp->itemlist_size(); k++)
                 {
                     
					 temp = sdm->getItemNameByTypeAndId(rsp->itemlist(k).itemtype(), rsp->itemlist(k).itemid());
					 if (temp)
						vec.push_back(temp);
                     
                 }

				SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
				//label 递减操作
				percentA = data->getSoulCurByIndex(ST_AAA) / (float)data->getSoulMaxByIndex(ST_AAA) * 100;
				percentB = data->getSoulCurByIndex(ST_BBB) / (float)data->getSoulMaxByIndex(ST_BBB) * 100;
				percentS = data->getSoulCurByIndex(ST_SSS) / (float)data->getSoulMaxByIndex(ST_SSS) * 100;
                //设置归零
                
                data->setSoulCurByIndex(_activeType, 0);
                _activeProg->runAction(CCProgressFromTo::create(WITHDRAW_TIME, 100, 0));
				
				
				schedule(schedule_selector(SGSoulExchangeLayer::updateActiveLabel), 0.02);
				
                
                actionall(vec,_activeType);
                ////////////////
//                SGshoujingzhouEffectLayer*box=SGshoujingzhouEffectLayer::create(rsp);
//                SGMainManager::shareMain()->showBox(box);
            }
            else
            {
                _canOpenBox = true;
                SGMainManager::shareMain()->showMessage(str_SoulExchangeLayer_str8);
            }
        }
    }
    //_activeType = ST_NULL;在移除定时器的时候置为NULL
    _activeProg = NULL;
	_curActiveLabel = NULL;
    checkBoxOpen(); //偷懒
}


/////////
void SGSoulExchangeLayer::showlayerhehe()
{
    SGshoujingzhouEffectLayer*box=SGshoujingzhouEffectLayer::create(vect);
    SGMainManager::shareMain()->showBox(box);

}
  void SGSoulExchangeLayer::haha(CCObject*obj)
{
    
}





//////////

//检查哪个箱子可以打开
void SGSoulExchangeLayer::checkBoxOpen()
{
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    
   
    //传奇
    int curS = data->getSoulCurOfS();
    int maxS = data->getSoulMaxOfS();
    if (curS >= maxS)
    {
        _waOpenS->stopAllActions();
        _waOpenS->setVisible(true);
        _waOpenS->runAction(CCRepeatForever::create( CCSequence::create(CCFadeIn::create(WAOPEN_FADE_TIME), CCFadeOut::create(WAOPEN_FADE_TIME), NULL) ));
        ////////////////////////////////
        
        
        
        
        /////////////////////////////////
    }
    else
    {
        _waOpenS->stopAllActions();
        _waOpenS->setVisible(false);
    }
    //精粹
    int curA = data->getSoulCurOfA();
    int maxA = data->getSoulMaxOfA();
    if (curA >= maxA)
    {
        _waOpenA->stopAllActions();
        _waOpenA->setVisible(true);
        _waOpenA->runAction(CCRepeatForever::create( CCSequence::create(CCFadeIn::create(WAOPEN_FADE_TIME), CCFadeOut::create(WAOPEN_FADE_TIME), NULL) ));
    }
    else
    {
        _waOpenA->stopAllActions();
        _waOpenA->setVisible(false);
    }
    //荆州
    int curB = data->getSoulCurOfB();
    int maxB = data->getSoulMaxOfB();
    if (curB >= maxB)
    {
        
        _waOpenB->stopAllActions();
        _waOpenB->setVisible(true);
        _waOpenB->runAction(CCRepeatForever::create( CCSequence::create(CCFadeIn::create(WAOPEN_FADE_TIME), CCFadeOut::create(WAOPEN_FADE_TIME), NULL) ));
        

        
    }
    else
    {
        _waOpenB->stopAllActions();
        _waOpenB->setVisible(false);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////各种动作

void SGSoulExchangeLayer::actionall(std::vector<SeRewardTypeId*>vector,SoulTag type)
{
    
    vect=vector;
    
     CCPoint posNebS = ccp(0, 195);
    int tag=type;
    
    if(type==ST_SSS)
        _boxS->setVisible(false);
    else if(type==ST_AAA)
        _boxA->setVisible(false);
    else if(type==ST_BBB)
        _boxB->setVisible(false);
    
    
    CCLayerColor*layer=(CCLayerColor*) this->getChildByTag(tag+100);
    

    ////////////////////////////下半个箱子形态
    CCSprite*aa=NULL;
    if(type==ST_SSS)
    {
        aa=CCSprite::createWithSpriteFrameName("sec_box_gold_base.png");

    }
    else if(type==ST_AAA)
    {
        aa=CCSprite::createWithSpriteFrameName("sec_box_silver_base.png");
  
    
    }
    else if(type==ST_BBB)
    {
        aa=CCSprite::createWithSpriteFrameName("sec_box_bronze_base.png");
   
    
    }
    

    CCSize s=CCDirector::sharedDirector()->getWinSize();

    aa->setPosition(ccpAdd( layer->getPosition(),posNebS));
 

    this->addChild(aa,99,type+200);
    CCSequence*seq= CCSequence::create((CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::actionall1))), CCDelayTime::create(0.9), CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::visible)), CCDelayTime::create(0.01f),(CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::actionall2))),CCDelayTime::create(0.01f),(CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::actionall3))),(CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::actionall4))),CCDelayTime::create(0.01f),NULL);
    aa->runAction(seq);
    ///////////
    
 


}

void SGSoulExchangeLayer::actionall1(CCObject*obj)
{
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    shita=(CCSprite*)obj;
   int tag= shita->getTag()-200;
 ////   两个上半个箱子形态
    // _boxB->setVisible(false);
    if(tag==ST_SSS)
    {
        ue=CCSprite::createWithSpriteFrameName("sec_box_gold_cap0.png");
        ue2=CCSprite::createWithSpriteFrameName("sec_box_gold_cap1.png");
        
    }
    else if(tag==ST_AAA)
    {
        ue=CCSprite::createWithSpriteFrameName("sec_box_silver_cap0.png");
        ue2=CCSprite::createWithSpriteFrameName("sec_box_silver_cap1.png");
        
        
    }
    else if(tag==ST_BBB)
    {
        ue=CCSprite::createWithSpriteFrameName("sec_box_bronze_cap0.png");
        ue2=CCSprite::createWithSpriteFrameName("sec_box_bronze_cap1.png");
        
        
    }
    
    
   
    ue->setPosition(ccpAdd(shita->getPosition(), ccp(shita->getContentSize().width*0.0, shita->getContentSize().height*0.3)));
    this->addChild(ue,99);
    this->addChild(ue2,99);
    ue2->setVisible(false);
    CCMoveBy*move1=CCMoveBy::create(0.1f, ccp(-3,7));
    CCMoveBy*move2=CCMoveBy::create(0.1f, ccp(3,-7));
    CCMoveBy*move3=CCMoveBy::create(0.1f, ccp(-2,5));
    CCMoveBy*move4=CCMoveBy::create(0.1f, ccp(2,-5));
    CCAction*action=CCSequence::create(move1,move2,move3,move4,NULL);
    CCMoveBy*move11=CCMoveBy::create(0.1f, ccp(-2,6));
    CCMoveBy*move22=CCMoveBy::create(0.1f, ccp(2,-6));
    CCMoveBy*move33=CCMoveBy::create(0.1f, ccp(-1,3));
    CCMoveBy*move44=CCMoveBy::create(0.1f, ccp(1,-3));
    
    CCSequence*action1=CCSequence::create(move22,move11,move44,move33,move44,move33,NULL);
    shita->runAction(action);
    ue->runAction(action1);
    
    
    
}
  void SGSoulExchangeLayer::visible(CCObject*node)
  {
      ue->setVisible(false);
  }
                                        
void SGSoulExchangeLayer::actionall2(CCObject*obj)
{
     CCSprite*shita=(CCSprite*)obj;
    
     ue2->setPosition(ccpAdd(shita->getPosition(), ccp(shita->getContentSize().width*0.09, shita->getContentSize().height*0.5)));
     ue2->setVisible(true);
}
void SGSoulExchangeLayer::actionall3(CCObject*node)
{
      CCSprite*shita=(CCSprite*)node;
    //////光效
    hikari=CCSprite::createWithSpriteFrameName("sec_img_light.png");
    hikari->setScale(0.2);
    hikari->setPosition(ccpAdd(shita->getPosition(), ccp(0, 20)));
    CCSpawn*spawn=CCSpawn::create(CCRotateBy::create(5.80f, EDDY_ROTATE_ANGLE),CCScaleTo::create(5.0f, 3.1f),NULL);
    CCAction*action9= CCSequence::create(spawn,NULL);
    this->addChild(hikari,99);
    hikari->runAction(action9);

    
    
    
}

void  SGSoulExchangeLayer::actionall4(CCObject*obj)
{
    CCSprite*sp=(CCSprite*)obj;
    int tag= sp->getTag()-200;
    //////////
    CCLayerColor *black = CCLayerColor::create(ccc4(255, 255, 255,0), 920, 1200);
    this->addChild(black,99,tag+200);
    black->setPosition(ccp(0, 0));
    CCFadeIn*fadein=CCFadeIn::create(2.0f);
    CCFadeOut*fadeout=CCFadeOut::create(2.0f);
    CCSequence*seq1= CCSequence::create(fadein,CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::showlayerhehe)),fadeout,CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::actionall5)),CCDelayTime::create(0.05f),CCCallFuncN::create(this, callfuncN_selector(SGSoulExchangeLayer::actionall6)),NULL);
    CCSequence*seq2= CCSequence::create(CCDelayTime::create(1.01f),CCDelayTime::create(0.05f),NULL);
    CCAction*action10=CCSpawn::create(seq1,seq2,NULL);
    black->runAction(action10);
}


void SGSoulExchangeLayer::actionall5(CCObject*obj)
{
    hikari->setVisible(false);
    ue->setVisible(false);
    ue2->setVisible(false);
    shita->setVisible(false);
}
void SGSoulExchangeLayer::actionall6(CCObject*obj)
{
    CCLayerColor*layer=(CCLayerColor*)obj;
    int tag= layer->getTag()-200;
    if(tag==ST_SSS)
        _boxS->setVisible(true);
    else if(tag==ST_AAA)
        _boxA->setVisible(true);
    else if(tag==ST_BBB)
        _boxB->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//显示战绩提示窗口
void SGSoulExchangeLayer::showFinalInfoNotice()
{
    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 51, 0, NULL);
    SGMainManager::shareMain()->showBox(cantadvanceBox);
}

//began传奇
bool SGSoulExchangeLayer::helloBeganSSS(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----BeganSSS");
    startInjection(ST_SSS);
    return true;
}

//began精粹
bool SGSoulExchangeLayer::helloBeganAAA(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----BeganAAA");
    startInjection(ST_AAA);
    return true;
}

//began荆州
bool SGSoulExchangeLayer::helloBeganBBB(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----BeganBBB");
    startInjection(ST_BBB);
    return true;
}

//moved
void SGSoulExchangeLayer::helloMovedSSS(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCLOG("SGSoulExchangeLayer----Moved");
}
void SGSoulExchangeLayer::helloMovedAAA(CCTouch *pTouch, CCEvent *pEvent)
{
    //    CCLOG("SGSoulExchangeLayer----Moved");
}
void SGSoulExchangeLayer::helloMovedBBB(CCTouch *pTouch, CCEvent *pEvent)
{
    //    CCLOG("SGSoulExchangeLayer----Moved");
}

//ended
void SGSoulExchangeLayer::helloEndedSSS(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----Ended");
    stopInjection(ST_SSS);
}
void SGSoulExchangeLayer::helloEndedAAA(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----Ended");
    stopInjection(ST_AAA);
}
void SGSoulExchangeLayer::helloEndedBBB(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----Ended");
    stopInjection(ST_BBB);
}

//cancelled
void SGSoulExchangeLayer::helloCancelledSSS(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----Cancelled");
    stopInjection();
}
void SGSoulExchangeLayer::helloCancelledAAA(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----Cancelled");
    stopInjection();
}
void SGSoulExchangeLayer::helloCancelledBBB(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("SGSoulExchangeLayer----Cancelled");
    stopInjection();
}

//传奇开箱子回调
void SGSoulExchangeLayer::openCofferHandlerS()
{
    sendGetRewardRequest(ST_SSS);
}

//精粹开箱子回调
void SGSoulExchangeLayer::openCofferHandlerA()
{
    sendGetRewardRequest(ST_AAA);
}

//荆州开箱子回调
void SGSoulExchangeLayer::openCofferHandlerB()
{
    sendGetRewardRequest(ST_BBB);
}
//////////


 SeRewardTypeId::SeRewardTypeId()
{

}
 SeRewardTypeId::~SeRewardTypeId()
{

}

