//
//  SGBMatchEndBox.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#include "SGBMatchEndBox.h"

#include "SGBMatchStartBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGBMatchEndBox::SGBMatchEndBox(void)
:t_delegate(NULL),
title(NULL),
armyIcon(NULL),
selfIcon(NULL),
selfIconBg(NULL),
armyIconBg(NULL),
armyName(NULL),
selfName(NULL),
armyRank(NULL),
selfRank(NULL),
armyFTime(NULL),
selfFTime(NULL),
armyLevel(NULL),
selfLevel(NULL),
armyWinR(NULL),
selfWinR(NULL),
selfservice(NULL),
armyservice(NULL),
label(NULL),
timeLabel(NULL),
cTime(0)
{

}
SGBMatchEndBox::~SGBMatchEndBox(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bMatchEndBox);
}

SGBMatchEndBox* SGBMatchEndBox::create(SGBoxDelegate *delegate)
{
    SGBMatchEndBox *box = new SGBMatchEndBox();
    if (box && box->init(delegate)) {
        box->t_delegate = delegate;
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

bool SGBMatchEndBox::init(SGBoxDelegate *delegate)
{
   
    if (!SGBaseBox::init(delegate, box_matchEnd, CCSizeMake(525, 710), false, true, CCSizeMake(525, 125))) {
        return false;
    }
    return true;
}

void SGBMatchEndBox::initView()
{
	
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox);
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    cTime = 4;
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    
//    title = CCSprite::createWithSpriteFrameName("upwar_box_match2.png");
    title = SGCCLabelTTF::create(str_BMatchEndBox_str1, FONT_PANGWA, 36 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(biao->getPosition());
    this->addChild(title);
    
    CCSprite *titleSumBg = CCSprite::createWithSpriteFrameName("upwar_bg_title.png");
    titleSumBg->setPosition(ccpAdd(biao->getPosition(), ccp(0, -65)));
    this->addChild(titleSumBg);
    // 上下边
    CCSprite *slineUp = CCSprite::createWithSpriteFrameName("upwar_bg_line2.png");
    slineUp->setScaleX((titleSumBg->getContentSize().width-2)/slineUp->getContentSize().width);
    slineUp->setPosition(ccpAdd(titleSumBg->getPosition(), ccp(0, titleSumBg->getContentSize().height*.49f)));
    this->addChild(slineUp);
    CCSprite *slineDown = CCSprite::createWithSpriteFrameName("upwar_bg_line2.png");
    slineDown->setScaleX(slineUp->getScaleX());
    slineDown->setPosition(ccpAdd(titleSumBg->getPosition(), ccp(0, -titleSumBg->getContentSize().height*.49f)));
    this->addChild(slineDown);
    CCSprite *titleSum = CCSprite::createWithSpriteFrameName("upwar_font_mt3.png");
    titleSum->setPosition(titleSumBg->getPosition());
    titleSum->setTag(101);
    this->addChild(titleSum);
    
    titleSumBg->setVisible(false);
    slineUp->setVisible(false);
    slineDown->setVisible(false);
    
    // line
    CCSprite *line = CCSprite::createWithSpriteFrameName("upwar_bgs_line.png");
    line->setScaleX(size.width/line->getContentSize().width);
    line->setPosition(ccpAdd(titleSumBg->getPosition(), ccp(0, -40)));
    this->addChild(line);
    
    float viewWid = size.width*.95f;
    float viewH = 125;
    
    for (int ii = 0; ii < 2; ii++) {
        CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
        frame2->setPreferredSize(CCSizeMake(viewWid, viewH));
        frame2->setPosition(ccpAdd(center, ccp(0, 130-ii*250)));
        this->addChild(frame2);
        frame2->setTag(200+ii);
        
        const char *file = "bingzhong_bg.png";
        ii == 1 && (file = "barrack_kuangbg.png");
        CCSprite *frame2bg = CCSprite::createWithSpriteFrameName(file);
        this->addChild(frame2bg, -1);
        frame2bg->setScaleX(viewWid/frame2bg->getContentSize().width);
        frame2bg->setScaleY(viewH/frame2bg->getContentSize().height);
        frame2bg->setPosition(frame2->getPosition());
        
        CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
        this->addChild(guang2l, -1);
        guang2l->setAnchorPoint(ccp(0, 1));
        CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
        this->addChild(guang2r, -1);
        guang2r->setFlipX(true);
        guang2r->setAnchorPoint(ccp(1, 1));
        
        CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
        this->addChild(guang2m, -1);
        guang2m->setScaleX((viewWid-120)/guang2m->getContentSize().width);
        guang2m->setAnchorPoint(ccp(0.5, 1));
        
        guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
        guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(viewWid*.5f, 0)));
        guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-viewWid*.5f, 0)));
    }
    
    CCScale9Sprite *frame1 = (CCScale9Sprite *)this->getChildByTag(200);
    CCScale9Sprite *frame2 = (CCScale9Sprite *)this->getChildByTag(201);
    
    float posU = frame1->getPositionY()-frame1->getContentSize().height*.5f;
    float posD = frame2->getPositionY()+frame2->getContentSize().height*.5f;
    float posY = frame2->getPositionY() + (posU - posD)*.5f + frame2->getContentSize().height*.5f;
    
    CCSprite *midVsBg = CCSprite::createWithSpriteFrameName("upwar_bg_vsbg.png");
    midVsBg->setPosition(ccp(center.x, posY));
//    this->addChild(midVsBg);
    CCSprite *midVs = CCSprite::createWithSpriteFrameName("upwar_bg_vs_n.png");
    midVs->setPosition(midVsBg->getPosition());
    this->addChild(midVs);
    
    // player icon
    armyIconBg = CCSprite::createWithSpriteFrameName("jinkuang.png");
    float discX1 = size.width*.5f - armyIconBg->getContentSize().width*.75f;
    armyIconBg->setPosition(ccpAdd(ccp(center.x+discX1, frame1->getPositionY()),ccp(0, -8)));
    armyIcon = CCSprite::createWithSpriteFrameName("nullkuang_n.png");
    armyIcon->setPosition(ccpAdd(armyIconBg->getPosition(),ccp(0, 0)));
    this->addChild(armyIcon);
    this->addChild(armyIconBg);
   
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_bMatchEndBox);
    
    selfIconBg = CCSprite::createWithSpriteFrameName("jinkuang.png");
    float discX2 = -size.width*.5f + armyIconBg->getContentSize().width*.75f;
    selfIconBg->setPosition(ccpAdd(ccp(center.x+discX2, frame2->getPositionY()),ccp(0, -8)));
    selfIcon = CCSprite::createWithSpriteFrameName("countryIcon_1.png");
    selfIcon->setPosition(ccpAdd(selfIconBg->getPosition(),ccp(selfIconBg->getContentSize().width*0.00, selfIconBg->getContentSize().height*0.00)));
    this->addChild(selfIcon);
    this->addChild(selfIconBg);
    
    float fontSize = 26;
    const char *fontName = FONT_BOXINFO;
    
    float dx1 = selfIcon->getContentSize().width*.5f+15, dx2 = 235;
    float dx3 = 92, dx4 = 278;
    float dy1 = 38, dy2 = -35;
    
    // player info
  
    armyName = SGCCLabelTTF::create("????", fontName, 28);
    armyName->setAnchorPoint(ccp(1, 0.5f));
    armyName->setPosition(ccpAdd(armyIcon->getPosition(), ccp(-armyIcon->getContentSize().width*.5f-7, dy1)));
    this->addChild(armyName);
    selfName = SGCCLabelTTF::create(str_BMatchEndBox_str2, fontName, 28);
    selfName->setAnchorPoint(ccp(0, 0.5f));
    selfName->setPosition(ccpAdd(selfIcon->getPosition(), ccp(dx1, dy1)));
    this->addChild(selfName);
    
    CCString *string = NULL;
   
    
   
    

    CCSize sizewin=CCDirector::sharedDirector()->getWinSize();
    string = CCString::create(str_BMatchEndBox_str3);
    armyRank = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    armyRank->setAnchorPoint(ccp(0, 0.5f));
//    armyRank->setPosition(ccpAdd(ccp(0, armyIcon->getPositionY()), ccp(dx3, 0)));
    armyRank->setPosition(ccpAdd(center,ccp(-armyIcon->getContentSize().width*2.2, armyIcon->getContentSize().height*1.7-40)));
    this->addChild(armyRank);
    string = CCString::createWithFormat(str_BMatchEndBox_str4, 1);
    selfRank = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    selfRank->setAnchorPoint(ccp(0, 0.5f));
    selfRank->setPosition(ccpAdd(selfIcon->getPosition(), ccp(dx1, 0)));
    this->addChild(selfRank);
    
    
    string = CCString::create(str_BMatchEndBox_str5);
    armyFTime = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    armyFTime->setAnchorPoint(ccp(0, 0.5f));
//    armyFTime->setPosition(ccpAdd(ccp(0, armyIcon->getPositionY()), ccp(dx3, dy2)));
    armyFTime->setPosition(ccpAdd(armyRank->getPosition(),ccp(0, -armyIcon->getContentSize().height*0.3) ));
    this->addChild(armyFTime);
    string = CCString::createWithFormat(str_BMatchEndBox_str6, 865);
    selfFTime = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    selfFTime->setAnchorPoint(ccp(0, 0.5f));
    selfFTime->setPosition(ccpAdd(selfIcon->getPosition(), ccp(dx1, dy2)));
    this->addChild(selfFTime);
    
    string = CCString::create(str_BMatchEndBox_str7);
    armyLevel = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    armyLevel->setAnchorPoint(ccp(0, 0.5f));
//    armyLevel->setPosition(ccpAdd(ccp(0, armyIcon->getPositionY()), ccp(dx4, 0)));
    armyLevel->setPosition(ccpAdd(armyRank->getPosition(),ccp(armyIcon->getContentSize().width*1.6, 0) ));
    this->addChild(armyLevel);
    string = CCString::createWithFormat(str_BMatchEndBox_str8, 156);
    selfLevel = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    selfLevel->setAnchorPoint(ccp(0, 0.5f));
    selfLevel->setPosition(ccpAdd(selfIcon->getPosition(), ccp(dx2, 0)));
    this->addChild(selfLevel);
    
    
    
    string = CCString::create(str_BMatchEndBox_str9);
    armyWinR = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    armyWinR->setAnchorPoint(ccp(0, 0.5f));
//    armyWinR->setPosition(ccpAdd(ccp(0, armyIcon->getPositionY()), ccp(dx4, dy2)));
    armyWinR->setPosition(ccpAdd(armyRank->getPosition(),ccp(armyIcon->getContentSize().width*1.6, -armyIcon->getContentSize().height*0.3)  ));
    this->addChild(armyWinR);
    string = CCString::createWithFormat(str_BMatchEndBox_str10, 0.8525*100);
    selfWinR = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    selfWinR->setAnchorPoint(ccp(0, 0.5f));
    selfWinR->setPosition(ccpAdd(selfIcon->getPosition(), ccp(dx2, dy2)));
    this->addChild(selfWinR);
    
    const char *bdesc = CCString::create(str_BMatchEndBox_str11)->getCString();
    SGCCLabelTTF *blabel = SGCCLabelTTF::create(bdesc, fontName, 27 , ccORANGE);
    //modify by:zyc. merge into create.
    //blabel->setColor(ccORANGE);
    blabel->setPosition(ccpAdd(center, ccp(0, -size.height*.5f + 110)));
    
    
    selfservice = SGCCLabelTTF::create(str_BMatchEndBox_str12, fontName, fontSize);
    selfservice->setAnchorPoint(ccp(0, 0.5f));
    selfservice->setPosition(ccpAdd(ccp(selfLevel->getPosition().x, selfName->getPosition().y), ccp(0,0)));
//    selfservice->setPosition(ccpAdd(center, ccp(0,0)));
    this->addChild(selfservice);
    armyservice = SGCCLabelTTF::create("????", fontName, fontSize);
    armyservice->setAnchorPoint(ccp(0, 0.5f));
    armyservice->setPosition(ccpAdd(ccp(armyRank->getPosition().x, armyName->getPosition().y), ccp(0,0)));
    this->addChild(armyservice);

    
    
//    this->addChild(blabel);
    
//    const char *fname = "animationFile/wzfg2.plist";
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fname);
//    
//    fname = "animationFile/wzfg2.scml";
//    CCSpriterX *resultx = CCSpriterX::create(fname, true, true);
//    resultx->setAnchorPoint(ccp(0.5, 0.5));
//    resultx->setPosition(ccpAdd(center, ccp(27, -size.height*.5f + 90)));
//    resultx->setanimaID(0);
//    resultx->setisloop(true);
//    this->addChild(resultx, 10);
//    resultx->play();
//    
    
    SGCCLabelTTF *resultLabel = SGCCLabelTTF::create(str_BMatchEndBox_str13, FONT_BOXINFO, 26 , ccWHITE);
    resultLabel->setAnchorPoint(ccp(0.5, 0.5));
    resultLabel->setPosition(ccpAdd(center, ccp(10, -size.height*.5f + 55)));
    //resultLabel->setColor(ccWHITE);
    this->addChild(resultLabel, 10);
   
}

void SGBMatchEndBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}

//正在寻找对手，先刷新自己信息
void SGBMatchEndBox::setSelfData(SGRankPlayer *player)
{    
    // 刷新自己信息
//    CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("headIcon/headCountry.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("sgcardslayer/sgcardslayer.plist");
//     CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("sgfriendmaillayer/sgfriendmaillayer.plist");
	ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox);
	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox);
	
	
    CCString *string = CCString::createWithFormat("countryIcon_%d.png", player->getHeadId());
    selfIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    selfIcon->setScale(0.9);
    string = CCString::createWithFormat("country_%d.png", SGPlayerInfo::sharePlayerInfo()->getPlayerCountry());
    selfIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    selfName->setString(player->getName()->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str14, player->getRank());
    selfRank->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str15, player->getBcount());
    selfFTime->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str16, player->getLevel());
    selfLevel->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str17, player->getWinRate());
    selfWinR->setString(string->getCString());
    
//     std::string servername = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
//    std::string serverid = CCUserDefault::sharedUserDefault()->getStringForKey("serverid");
//    string = CCString::createWithFormat("%s服:%s",serverid.c_str(),servername.c_str());
    
    //MM: 对手的serverId即serverShowId，这点由服务器保证。但是自身的serverId为真实id，所以自身的serverShowId改为从本地读取，就不去修改SGPlayerInfo相关逻辑了，切记。这里原为player->getServerId();
    string = CCString::createWithFormat(str_BMatchEndBox_str18,CCUserDefault::sharedUserDefault()->getIntegerForKey("serverShowId", 1),player->getServerName()->getCString());
    selfservice->setString(string->getCString());
}

// 已经找到对手，刷新对手信息
void SGBMatchEndBox::setArmyData(SGRankPlayer *player)
{
	//改为调用者的layer
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_bMatchEndBox);
    title->setString(str_BMatchEndBox_str19);
    
    CCSize size = this->boxSize;
    CCSize sizewin=CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    this->removeChildByTag(101, true);
    
//    CCString *desc = CCString::create("已找到对手，马上进入战场");
//    CCLabelTTF *label = CCLabelTTF::create(desc->getCString(), FONT_BOXINFO, FONTSIZE(10));
//    label->setPosition(ccpAdd(center, ccp(0, this->boxSize.height/2-125)));
//    this->addChild(label);
//    return;
    
    CCString *desc = CCString::create(str_BMatchEndBox_str20);
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(desc->getCString(), FONT_BOXINFO, 26);
//    float discX = -size.width/2+label1->getContentSize().width+95;
//    label1->setPosition(ccpAdd(center, ccp(discX, this->boxSize.height*.5f-47)));
    label1->setPosition(ccpAdd(title->getPosition(),ccp(0, -armyIconBg->getContentSize().height*0.5) ));
    label1->setAnchorPoint(ccp(1, 0.5f));
    this->addChild(label1);
    desc = CCString::createWithFormat(str_BMatchEndBox_str21, cTime);
    timeLabel= SGCCLabelTTF::create(desc->getCString(), FONT_BOXINFO, 26 , ccRED);
    //modify by:zyc. merge into create.
    //timeLabel->setColor(ccRED);
    timeLabel->setPosition(ccpAdd(label1->getPosition(), ccp(30, 0)));
    this->addChild(timeLabel);
    desc = CCString::create(str_BMatchEndBox_str22);
    SGCCLabelTTF *label3= SGCCLabelTTF::create(desc->getCString(), FONT_BOXINFO, 26);
    label3->setAnchorPoint(ccp(0, 0.5f));
    label3->setPosition(ccpAdd(timeLabel->getPosition(), ccp(30, 0)));
    this->addChild(label3);
    
    CCAction *action = CCSequence::create(CCDelayTime::create(1.0f),
                                          CCCallFunc::create(this, callfunc_selector(SGBMatchEndBox::resetTime)),
                                          NULL);
    timeLabel->runAction(action);
    
    // 刷新对手信息
    
    
    CCString *string = CCString::createWithFormat("countryIcon_%d.png", player->getHeadId());
    armyIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    
    armyIcon->setScale(0.9);
    
    string = CCString::createWithFormat("country_%d.png", player->getHeadId());
    armyIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    armyIcon->setPosition(ccpAdd(armyIconBg->getPosition(),ccp(armyIconBg->getContentSize().width*0.00, armyIconBg->getContentSize().height*0.05)));
    
    armyName->setString(player->getName()->getCString());
    
    //MM: 若对方排名为0，则显示成"未知"。
    if (player->getRank() <= 0)
    {
        string = CCString::create(str_BMatchEndBox_str23);
    }
    else
    {
        string = CCString::createWithFormat(str_BMatchEndBox_str14, player->getRank());
    }
    armyRank->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str15, player->getBcount());
    armyFTime->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str16, player->getLevel());
    armyLevel->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str17, player->getWinRate());
    armyWinR->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str18,player->getServerId(),player->getServerName()->getCString());
    armyservice->setString(string->getCString());

}

void SGBMatchEndBox::resetTime()
{
    cTime -= 1;
    if (cTime == 1) {
        EFFECT_PLAY(MUSIC_32);
    }
    if (cTime <= 0) {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        mainLayer->loginPvpServer();
    }else {
        CCString *desc = CCString::createWithFormat(str_BMatchEndBox_str21, cTime);
        timeLabel->setString(desc->getCString());
        CCAction *action = CCSequence::create(CCDelayTime::create(1.0f),
                                              CCCallFunc::create(this, callfunc_selector(SGBMatchEndBox::resetTime)),
                                              NULL);
        timeLabel->runAction(action);
    }
}