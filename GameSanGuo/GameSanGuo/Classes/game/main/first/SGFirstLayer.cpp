 //
//  SGFirstLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#include "SGFirstLayer.h"
#include "SGMainManager.h"
#include "SGFirstSubLayer.h"
#include "SGPlayerInfo.h"
#include "SGFriend.h"
#include "SGOtherPlayerInfo.h"
#include "SGChat.h"
#include "SGChatMessageCenter.h"
#include "SGMail.h"
#include "SGStaticDataManager.h"
#include "SGMainLayer.h"
#include "SGAnnouncementLayer.h"
#include "SGFirstStrengBox.h"
#include "SGFirstAdvanceBox.h"
#include "SGMainLayer.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "DayTaskModule.pb.h"
#include "SGGuideManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "SurvivalModule.pb.h"
#include "SGSjzData.h"
#include "SGCantAdvanceBox.h"
//#include "SGRankAllPlayer.h"

#include "FightBaseModule.pb.h"
#include "SGBossBattleJoinInLayer.h"
#include "SGStringConfig.h"
#include "SGEmbattleLayer.h"
#include "AppMacros.h"
#include "SGLottery9ViewItem.h"
#include "SGGeneralInfoLayer.h"
#include "RankModule.pb.h"
#include "SGSpecialBattleLayer.h"
#include "SpecialBattleModule.pb.h"

#include "GlobalConfig.h"


#define SHOWMENUTIME 0.3f
#define DEFAULTSCALE 0.8
#define DEFAULTSCALENEW 0.7
//#define SHOWSCALE    1.0
#define INITCOLOR    100
#define ADDBACECOLOR   510
#define SUBSCROLLWWID   260
#define GENERALICONDIS 130
#define RED_POINT_NOTICE_TAG 1314

SGTaskObj::SGTaskObj()
:m_taskcount(0),
m_taskid(0)
{
}
SGTaskObj::~SGTaskObj()
{
    
}

SGFirstLayer::SGFirstLayer()
:
spriteMailCount(NULL),
labelMailCount(NULL),
firstTouchPos(CCPointZero),
currTaskButton(NULL),
limitActiveButton(NULL),
strengthButton(NULL),
advanceButton(NULL),
daguanButton(NULL),
netbattleButton(NULL),
visitButton(NULL),
lingjiangButton(NULL),
mailButton(NULL),
fubenButton(NULL),
shouJinZhouButton(NULL),
bossButton(NULL),
//embattleNotice(NULL),
spriteEverydayFinish_1(NULL),
spriteEverydayFinish_2(NULL),
spriteEverydayFinish_3(NULL),
spriteEverydayFinish_4(NULL),
spriteEverydayFinish_5(NULL),
spriteEverydayFinish_6(NULL),
spriteEverydayFinish_7(NULL),
spr_notice(NULL),
spr_noticeDayTask(NULL),
spr_noticeBuZhen(NULL),
spr_noticactivity_gift(NULL),
left(NULL),
rgt(NULL),
realwidth(0),
m_startMoveInterval(0),
m_startDragInterval(0),
m_scrollStartVector(CCPointZero),
m_scrollLastVector(CCPointZero),
m_nowPageTop(0),
m_nowPageBottom(0),
m_nowPageLeft(0),
m_nowPageRight(0),
ItemShowRect(0),
ItemShowPosX(0),
ItemInitPosX(0),
s(CCSizeZero),
moveDist(0),
isMoveRight(false),
moveAddDist(0),
moveStartPosX(0),
liangDian_1(NULL),
liangDian_2(NULL),
liangDian_3(NULL),
liangDian_4(NULL),
liangDian_5(NULL),
liangDian_6(NULL),
liangDian_7(NULL),
showScale(0),
layerRate(0),
spr_notice_officer(NULL),
officerBtn(NULL),
specialBattleButton(NULL)
{

    realwidth = 0;
    firstTouchPos = CCPointZero;
   spr_noticeBuZhen = NULL;
}
SGFirstLayer::~SGFirstLayer()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_firstLayer);
    unschedule(schedule_selector(SGFirstLayer::updateItemPos));
    if (menu)
    {
//        menu->removeAllChildrenWithCleanup(true);
        menu->removeFromParentAndCleanup(true);
    }
}

SGFirstLayer *SGFirstLayer::create(const char *bgFile, LayerTag tag_)
{
    SGFirstLayer *firstLayer = new SGFirstLayer();
    if (firstLayer && firstLayer->init(bgFile, tag_))
    {
        firstLayer->autorelease();
        return firstLayer;
    }
    CC_SAFE_DELETE(firstLayer);
    return NULL;
}

void SGFirstLayer::initMsg()
{
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_firstLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_firstLayer, LIM_PNG_AS_PNG);
    
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_firstLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgfirstbutton/sgfirstbutton.plist", RES_TYPE_LAYER_UI, sg_firstLayer);
    
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_firstLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_firstLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_firstLayer);
    
    ResourceManager::sharedInstance()->bindTexture("animationFile/short.plist", RES_TYPE_LAYER_UI, sg_firstLayer);
    
    ResourceManager::sharedInstance()->bindTexture("AddNew/mainUI.plist", RES_TYPE_LAYER_UI, sg_firstLayer, LIM_PNG_AS_PNG);  //新增加//
    
    getMenu()->setMenuMoveDelegate(this);

    s = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    layerRate = s.height/960;
    showScale = layerRate > 1.1? 1.2:1;
   
    //出战武将滑动的背景
    CCSprite *background1 = CCSprite::createWithSpriteFrameName("mainlayer_scrollbgNew.png");
    this->addChild(background1,-1);
    background1->setAnchorPoint(ccp(0, 0.67));
    background1->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(-9, - h*layerRate + 38)));
    
    //武将阵容的字  cgp 隐藏掉
 //   int zhenrong = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
//    SGCCLabelTTF *fontWuJiang = SGCCLabelTTF::create(CCString::createWithFormat(str_FirstLayer_str1,zhenrong)->getCString(), FONT_PANGWA, 32, ccc3(0xf3, 0x89, 0x00));
    SGCCLabelTTF *fontWuJiang = SGCCLabelTTF::create("", FONT_PANGWA, 32, ccc3(0xf3, 0x89, 0x00));
    
    this->addChild(fontWuJiang);
    fontWuJiang->setPosition(ccpAdd(background1->getPosition(), ccp(135, 5)));
    
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int zhandouli =  playerInfo->getcombatValue();
    SGCCLabelTTF *fontZhan = SGCCLabelTTF::create(CCString::createWithFormat(str_FirstLayer_str2,zhandouli)->getCString(), FONT_PANGWA, 22);
    this->addChild(fontZhan);
    fontZhan->setPosition(ccpAdd(background1->getPosition(), ccp(390, 0)));
    
    //武将阵容旁边的邮件按钮
    mailButton = SGButton::create("mail.png", NULL, this, menu_selector(SGFirstLayer::buttonClickMailShow),CCPointZero,false,true);
	mailButton->setScale(1.1);
    mailButton->setPosition( ccp(s.width - mailButton->getContentSize().width*0.7, fontWuJiang->getPosition().y+h/100*layerRate));
    this->addBtn(mailButton);

    
    
    //每日必做事项6个按钮 ////////////////////////////////////////
    //任务
    currTaskButton = SGButton::createFromLocal("mainbutton_0.png", str_FirstLayer_str3, this, menu_selector(SGFirstLayer::taskHandlerSend),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    
    //活动
    limitActiveButton = SGButton::createFromLocal("mainbutton_1.png", str_FirstLayer_str4, this, menu_selector(SGFirstLayer::fubenHandler),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_ACTIVITY_CHARGE, 3);
    limitActiveButton->setTag(tag);
    //领奖
    lingjiangButton = SGButton::createFromLocal("mainbutton_2.png", str_FirstLayer_str5, this, menu_selector(SGFirstLayer::showRewardsLayer),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    //商城
    visitButton = SGButton::createFromLocal("mainbutton_3.png",str_FirstLayer_str6_1, this, menu_selector(SGFirstLayer::onShopBtnClicked),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    //强化
    strengthButton = SGButton::createFromLocal("mainbutton_4.png", str_FirstLayer_str7, this, menu_selector(SGFirstLayer::buttonClickStrengthenShow),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    //士兵卡强化的强化按钮tag, 在强制引导过后
    if (SGGuideManager::shareManager()->getCurrentGuideId() > 52)
    {
        int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_SOLDIER_STRENG, 2);
        strengthButton->setTag(tag);
    }
    //排行//
    rankBtn = SGButton::createFromLocal("mainbutton_5.png", str_FirstLayer_str6, this,menu_selector(SGFirstLayer::rankBtnClicked),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    
    currTaskButton->setOutsideColor(ccWHITE);
    limitActiveButton->setOutsideColor(ccWHITE);
    lingjiangButton->setOutsideColor(ccWHITE);
    visitButton->setOutsideColor(ccWHITE);
    strengthButton->setOutsideColor(ccWHITE);
    rankBtn->setOutsideColor(ccWHITE);
    
    this->addBtn(strengthButton);
    this->addBtn(currTaskButton);
    this->addBtn(limitActiveButton);
    this->addBtn(rankBtn);
    this->addBtn(lingjiangButton);
    this->addBtn(visitButton);
    EFFECT_PLAY(MUSIC_ITEM);
    
    
    //六个按钮，位置大小
//    limitActiveButton->setPosition(ccpAdd(ccp(center.x, 40 + background1->getPositionY()), ccp(0,  -background1->getContentSize().height/2*layerRate-limitActiveButton->getContentSize().height*layerRate)));
    lingjiangButton->setPosition(ccpAdd(ccp(center.x, 40 + background1->getPositionY() + 20), ccp(0,  -background1->getContentSize().height/2*layerRate-limitActiveButton->getContentSize().height*layerRate)));

    currTaskButton->setPosition(ccpAdd(lingjiangButton->getPosition(), ccp(-1.3 *lingjiangButton->getContentSize().width, 0)));

    limitActiveButton->setPosition(ccpAdd(lingjiangButton->getPosition(), ccp(-lingjiangButton->getContentSize().width * 0.65, 0)));

    visitButton->setPosition(ccpAdd(lingjiangButton->getPosition(), ccp(lingjiangButton->getContentSize().width * 0.65,  0)));
    
    strengthButton->setPosition(ccpAdd(lingjiangButton->getPosition(), ccp(lingjiangButton->getContentSize().width * 1.3, 0)));
    
    rankBtn->setPosition(ccpAdd(lingjiangButton->getPosition(), ccp(-strengthButton->getContentSize().width * 1.3,  -limitActiveButton->getContentSize().height*layerRate * 0.65)));
    
    limitActiveButton->setScale(0.6);
    currTaskButton->setScale(0.6);
    lingjiangButton->setScale(0.6);
    strengthButton->setScale(0.6);
    visitButton->setScale(0.6);
    rankBtn->setScale(0.6);
    
    rankBtn->setVisible(false);
    rankBtn->setEnabled(false);
    
    
   
    
    
    spriteMailCount = CCSprite::createWithSpriteFrameName("mainLayer_button_countBG.png");
    spriteMailCount->setPosition(ccpAdd(mailButton->getPosition(), ccp(mailButton->getContentSize().width*.37, mailButton->getContentSize().height*.38)));
    labelMailCount = CCLabelTTF::create("0", FONT_PANGWA, 24);
    labelMailCount->setPosition(ccpAdd(spriteMailCount->getPosition(), ccp(0, 5)));
    this->addChild(spriteMailCount,100);
    this->addChild(labelMailCount,100);
    spriteMailCount->setVisible(false);
    labelMailCount->setVisible(false);
    
    
    
    ///划屏指示亮点/////////////////////////////////////
    liangDian_1 = CCSprite::createWithSpriteFrameName("anDian.png");
    liangDian_2 = CCSprite::createWithSpriteFrameName("anDian.png");
    liangDian_3 = CCSprite::createWithSpriteFrameName("anDian.png");
    liangDian_4 = CCSprite::createWithSpriteFrameName("anDian.png");
    liangDian_6 = CCSprite::createWithSpriteFrameName("anDian.png");
    liangDian_7 = CCSprite::createWithSpriteFrameName("anDian.png");
    liangDian_5 = CCSprite::createWithSpriteFrameName("liangDian.png");
    addChild(liangDian_1);
    addChild(liangDian_2);
    addChild(liangDian_3);
    addChild(liangDian_4);
    addChild(liangDian_6);
    addChild(liangDian_7);
    addChild(liangDian_5);
    
    //unvisibleBycgp
    liangDian_1->setVisible(false);
    liangDian_2->setVisible(false);
    liangDian_3->setVisible(false);
    liangDian_4->setVisible(false);
    liangDian_6->setVisible(false);
    liangDian_7->setVisible(false);
    liangDian_5->setVisible(false);
    
  
    liangDian_1->setPosition(ccp(center.x - 100, SGMainManager::shareMain()->getBottomHeight()+skewingY(8)*layerRate));
    liangDian_2->setPosition(ccp(center.x - 60, SGMainManager::shareMain()->getBottomHeight()+skewingY(8)*layerRate));
    liangDian_3->setPosition(ccp(center.x - 20 , SGMainManager::shareMain()->getBottomHeight()+skewingY(8)*layerRate));
    liangDian_4->setPosition(ccp(center.x + 20, SGMainManager::shareMain()->getBottomHeight()+skewingY(8)*layerRate));
    liangDian_6->setPosition(ccp(center.x + 60, SGMainManager::shareMain()->getBottomHeight()+skewingY(8)*layerRate));
    liangDian_7->setPosition(ccp(center.x + 100, SGMainManager::shareMain()->getBottomHeight()+skewingY(8)*layerRate));
    liangDian_5->setPosition(liangDian_2->getPosition());
    
    //提醒的感叹号/////////////////////////////////////////////////////////////
    spr_notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    spr_notice->setPosition(ccpAdd(lingjiangButton->getPosition(), ccp(lingjiangButton->getContentSize().width*.2, lingjiangButton->getContentSize().height*.2)));
    spr_notice->setVisible(false);
    this->addChild(spr_notice,100);

    spr_noticeDayTask = CCSprite::createWithSpriteFrameName("publc_notice.png");
    spr_noticeDayTask->setPosition(ccpAdd(currTaskButton->getPosition(), ccp(currTaskButton->getContentSize().width*.2, currTaskButton->getContentSize().height*.2)));
    spr_noticeDayTask->setVisible(false);
    this->addChild(spr_noticeDayTask,111);
    
    spr_noticactivity_gift = CCSprite::createWithSpriteFrameName("publc_notice.png");
    spr_noticactivity_gift->setPosition(ccpAdd(limitActiveButton->getPosition(), ccp(limitActiveButton->getContentSize().width*.2, limitActiveButton->getContentSize().height*.2)));
    spr_noticactivity_gift->setVisible(true);
    this->addChild(spr_noticactivity_gift,112);
    
    if (playerInfo->getIsActiviNotAccess())
    {
        spr_noticactivity_gift->setVisible(true);
    }
    else
    {
        spr_noticactivity_gift->setVisible(false);
    }
    
    this->activityanimat();
    

    ////////////////////////////滑动的6个战斗模式////////////////////////////////////////
    daguanButton = SGButton::create("cg.png", "cgzi.png", this, menu_selector(SGFirstLayer::testStoryLayer),ccp(0,-100),false,true);    //闯关//
    netbattleButton = SGButton::create("dz.png", "dzzi.png", this, menu_selector(SGMainLayer::battleHandler),ccp(0,-100),false,true);    //对战//
    fubenButton = SGButton::create("fb.png", "fbzi.png", this, menu_selector(SGMainLayer::activityHandler),ccp(0,-100),false,true);       //副本//
    shouJinZhouButton = SGButton::create("sjz.png", "sjzzi.png", this, menu_selector(SGFirstLayer::sjzInfoRequest),ccp(0,-100),false,true);   //守荆州//
    bossButton= SGButton::create("fight_boss.png", "fight_boss_font.png", this, menu_selector(SGFirstLayer::bossHandler),ccp(0,-100),false,true);   //讨魔//
    
    specialBattleButton = SGButton::create("sp_battle_entry.png", "sp_battle_entry_font.png", this,                                           menu_selector(SGFirstLayer::specialBattleHandler), ccp(0, -60), false, true);       //幻境//
    
    
    daguanButton->setScale(0.45);
    netbattleButton->setScale(0.45);
    fubenButton->setScale(0.45);
    shouJinZhouButton->setScale(0.45);
    bossButton->setScale(0.45);
    specialBattleButton->setScale(0.45);
    
    //bossButton->setEnabled(false);
    
    daguanButton->setPosition(ccp(center.x, SGMainManager::shareMain()->getBottomHeight()*layerRate - 20));
    
    shouJinZhouButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-daguanButton->getContentSize().width*0.45, 0)));
    
    fubenButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-daguanButton->getContentSize().width * 0.9,0)));
    
    netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(daguanButton->getContentSize().width*0.45, 0)));
    
    bossButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(daguanButton->getContentSize().width * 0.9, 0)));
    



    
    //特殊副本战斗的按钮位置
    specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));
    
    
    
    
//    daguanButton->setPosition(ccp(center.x, SGMainManager::shareMain()->getBottomHeight()*layerRate - 45));
//    fubenButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-fubenButton->getContentSize().width*DEFAULTSCALE,0)));
//    netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
//    shouJinZhouButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
//    bossButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(bossButton->getContentSize().width*DEFAULTSCALE, 0)));
//    
//    //特殊副本战斗的按钮位置
//    specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));
    
    //**级之前在闯关按钮上播放动画
//    if( !playerInfo->isGuid() &&   playerInfo->getPlayerLevel() < playerInfo->getShowPlayerNoticeLimitLev() )
//    {
//        CCSprite * animateSprite = CCSprite::createWithSpriteFrameName("cg.png");
//        animateSprite->setPosition(daguanButton->getContentSize()/2);
//        daguanButton->addChild(animateSprite ,100);
//        
//        CCSprite* asZi = CCSprite::createWithSpriteFrameName("cgzi.png");
//        animateSprite->addChild(asZi);
//        asZi->setPosition(animateSprite->getContentSize()/2 + CCSize(0, -60));
//        
//        animateSprite->setCascadeOpacityEnabled(true);
//        
//        float t = 1;
//        animateSprite->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0 , 1)  ,CCFadeIn::create(0),CCSpawn::create(CCFadeOut::create(t) , CCScaleTo::create(t, 1.4) , NULL) ,CCDelayTime::create(2), NULL )));
//    }
    
    
    
    int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitPvpBattle,2);
    netbattleButton->setTag(limittag);
    
    limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitXianshiFuben,2);
    fubenButton->setTag(limittag);
    
    limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitShouJinZhou,2);
    shouJinZhouButton->setTag(limittag);
    
    limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitBossBattle,2);
    bossButton->setTag(limittag);
    
    //幻境的tag  ==> 17
    limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitSpecailBattle, 3);
    specialBattleButton->setTag(limittag);

    //在引导需要的UI上定义数据表中预先写好的信息，这里只用tag，方便以后获取对应的UI（用来高亮）
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_12);

    currTaskButton->setTag(0);
    //闯关模式 按钮 tag == 502
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_2);
    daguanButton->setTag(modelTag);


  
    this->addBtn(fubenButton);
    this->addBtn(daguanButton);
    this->addBtn(netbattleButton);
    this->addBtn(shouJinZhouButton);
    this->addBtn(bossButton);
    this->addBtn(specialBattleButton);

//    daguanButton->setScale(showScale);
//    fubenButton->setScale(DEFAULTSCALENEW);
//    netbattleButton->setScale(DEFAULTSCALENEW);
//    shouJinZhouButton->setScale(DEFAULTSCALENEW);
//    bossButton->setScale(DEFAULTSCALENEW);
//    specialBattleButton->setScale(DEFAULTSCALENEW);
    
//    fubenButton->setAnchorPoint(ccp(0.5, 0));
//    daguanButton->setAnchorPoint(ccp(0.5, 0));
//    netbattleButton->setAnchorPoint(ccp(0.5, 0));
//    shouJinZhouButton->setAnchorPoint(ccp(0.5, 0));
//    bossButton->setAnchorPoint(ccp(0.5, 0));
//    specialBattleButton->setAnchorPoint(ccp(0.5, 0));
    
//    fubenButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
//    netbattleButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
//    shouJinZhouButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
//    bossButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
//    specialBattleButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));

    
//    fubenButton->setFontImageColor(fubenButton->getColor());
//    daguanButton->setFontImageColor(daguanButton->getColor());
//    netbattleButton->setFontImageColor(netbattleButton->getColor());
//    shouJinZhouButton->setFontImageColor(shouJinZhouButton->getColor());
//    bossButton->setFontImageColor(bossButton->getColor());
//    specialBattleButton->setFontImageColor(specialBattleButton->getColor());
    
//    fubenButton->setEnabled(false);
//    netbattleButton->setEnabled(false);
//    shouJinZhouButton->setEnabled(false);
//    bossButton->setEnabled(false);
//    specialBattleButton->setEnabled(false);
    
    ///////////limitLevel////////////////////////////////////
    
    CCSprite *limitLevelBg = NULL;
    CCSprite *limitLabel = NULL;
    CCString *labelStr = NULL;
    
    if (!playerInfo->canBreakLimitById(limitArena))
    {
        labelStr = CCString::createWithFormat(str_FirstLayer_str9,playerInfo->getLimitLevelById(limitArena));
        limitLevelBg = CCSprite::createWithSpriteFrameName("limitLevelBg.png");
        limitLevelBg->setPosition(ccp(netbattleButton->getContentSize().width/2, netbattleButton->getContentSize().height/2));
        limitLabel = SGRichLabel::getFomLabel(labelStr->getCString(), limitLevelBg->getContentSize(), FONT_PANGWA, 26);
        limitLevelBg->addChild(limitLabel);
        limitLabel->setPositionX(limitLevelBg->getContentSize().width*0.67);
        netbattleButton->addChild(limitLevelBg);
        
    }
    if (!playerInfo->canBreakLimitById(limitShouJinZhou))
    {
        labelStr = CCString::createWithFormat(str_FirstLayer_str9,playerInfo->getLimitLevelById(limitShouJinZhou));
        limitLevelBg = CCSprite::createWithSpriteFrameName("limitLevelBg.png");
        limitLevelBg->setPosition(ccp(shouJinZhouButton->getContentSize().width/2, shouJinZhouButton->getContentSize().height/2));
        limitLabel = SGRichLabel::getFomLabel(labelStr->getCString(), limitLevelBg->getContentSize(), FONT_PANGWA, 26);
        limitLevelBg->addChild(limitLabel);
        limitLabel->setPositionX(limitLevelBg->getContentSize().width*0.67);
        shouJinZhouButton->addChild(limitLevelBg);
    }
    if (!playerInfo->canBreakLimitById(limitXianshiFuben))
    {
        labelStr = CCString::createWithFormat(str_FirstLayer_str9,playerInfo->getLimitLevelById(limitXianshiFuben));
        limitLevelBg = CCSprite::createWithSpriteFrameName("limitLevelBg.png");
        limitLevelBg->setPosition(ccp(fubenButton->getContentSize().width/2, fubenButton->getContentSize().height/2));
        limitLabel = SGRichLabel::getFomLabel(labelStr->getCString(), limitLevelBg->getContentSize(), FONT_PANGWA, 26);
        limitLevelBg->addChild(limitLabel);
        limitLabel->setPositionX(limitLevelBg->getContentSize().width*0.67);
        fubenButton->addChild(limitLevelBg);
    }
    if (!playerInfo->canBreakLimitById(limitBossBattle))
    {
        labelStr = CCString::createWithFormat(str_FirstLayer_str9,playerInfo->getLimitLevelById(limitBossBattle));
        limitLevelBg = CCSprite::createWithSpriteFrameName("limitLevelBg.png");
        limitLevelBg->setPosition(ccp(bossButton->getContentSize().width/2, bossButton->getContentSize().height/2));
        limitLabel = SGRichLabel::getFomLabel(labelStr->getCString(), limitLevelBg->getContentSize(), FONT_PANGWA, 26);
        limitLevelBg->addChild(limitLabel);
        limitLabel->setPositionX(limitLevelBg->getContentSize().width*0.67);
        bossButton->addChild(limitLevelBg);
    }
    if (!playerInfo->canBreakLimitById(limitSpecailBattle))
    {
        labelStr = CCString::createWithFormat(str_FirstLayer_str9,playerInfo->getLimitLevelById(limitSpecailBattle));
        limitLevelBg = CCSprite::createWithSpriteFrameName("limitLevelBg.png");
        limitLevelBg->setPosition(ccp(bossButton->getContentSize().width/2, bossButton->getContentSize().height/2));
        limitLabel = SGRichLabel::getFomLabel(labelStr->getCString(), limitLevelBg->getContentSize(), FONT_PANGWA, 26);
        limitLevelBg->addChild(limitLabel);
        limitLabel->setPositionX(limitLevelBg->getContentSize().width*0.67);
        specialBattleButton->addChild(limitLevelBg);
    }
    ////////////////////////////////////////////////////
    ItemShowRect = shouJinZhouButton->getPositionX() - fubenButton->getPositionX() + shouJinZhouButton->getContentSize().width/2+ fubenButton->getContentSize().width/2;
    ItemShowPosX  = daguanButton->getPositionX();
    ItemInitPosX = fubenButton->getPositionX();
    
    //ccscrollview  布阵的武将阵容     /////
    scrollview = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(SUBSCROLLWWID), 112));
    CCLayer *layer = CCLayer::create();
    int width = 10;
    //显示的是布阵的武将阵容，不是全部武将

    int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();//启用的当前阵容id
    CCArray *arrayOfficer = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
    CCArray * allCards = NULL;
    allCards = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    
    bool isIdleCards = false;//是否有空闲武将
    if(allCards)
    {
        
        CCObject *obj = NULL;
        CCARRAY_FOREACH(allCards, obj)
        {
            SGOfficerCard  *pcard = (SGOfficerCard *)obj;
            int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(id,pcard);
            if(po ==0)
            {
                isIdleCards = true;
                break;
            }
        }
    }

    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel());
    
    //在这里进入武将详情 换人 需要知道换人的阵容
    SGGeneralInfoLayer::setEmType(id);
    for (int i = 0; i<player->getPlayerOfficerMax(); i++)
    {
        SGOfficerCard *card = NULL;
        SGFirstSubLayer *subLayer = NULL;
        
        CCObject *obj = NULL;
        CCARRAY_FOREACH(arrayOfficer, obj)
        {
            SGOfficerCard  *pcard = (SGOfficerCard *)obj;
            int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(id,pcard);
            if ((po-1) == i)
            {
                card = pcard;
                break;
            }
        }
        if (card)
        {   //使用武将id找到officer对象，然后得到该武将的iconId，再加载对应的plist。以前直接用itemId加载，简直就是扯淡 evan20140825
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());//通过武将id找到officer.plist对象
            SGMainManager::shareMain() -> addHeadIconPlistByNum(temp->getIconId(),sg_firstLayer);
            subLayer = SGFirstSubLayer::create(card,ccp(width, 0),CCSizeMake(112, 111),0);
         }
        else
        {
            subLayer = SGFirstSubLayer::create(NULL,ccp(width, 0),CCSizeMake(112, 111),1);
            subLayer->setPosition(ccp(width, 0));
            //只要空位 永久显示   modify by:zyc.
//            if(SGMainManager::shareMain()->getEmbattleKey())
//            {
                //这里是没有武将的空位
                int shiftLeft = 0, shiftDown = 0;
                if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
                    ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
                {
                    shiftLeft = 60;
                    shiftDown = -60;
                }
                else if (CCEGLView::sharedOpenGLView()->getFrameSize().height == iphonehdResource.size.height)
                {
                    shiftLeft = -5;
                    shiftDown = -90;
                }

                if(isIdleCards){
                    CCSprite * red_point_notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
                    red_point_notice->setPosition(ccp(111-red_point_notice->getContentSize().width/2,111-red_point_notice->getContentSize().height/2));
                    subLayer->addChild(red_point_notice,0,RED_POINT_NOTICE_TAG);
                }
        }
        //为新手引导，第一个设置为数据表中的tag
        int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_12);
        if (i == 0)
        {
            layer->addChild(subLayer,0,modelTag);
        }
        else
        {
            layer->addChild(subLayer,0,i);
        }
        subLayer->setPosition(ccp(width, 0));
        width += GENERALICONDIS;
    }
    
    if ( 0 < (10 - player->getPlayerOfficerMax()))
    {
        SGFirstSubLayer *subLayer = SGFirstSubLayer::create(NULL,ccp(width, 0),CCSizeMake(112, 111),2);
        subLayer->setPosition(ccp(width, 0));
        layer->addChild(subLayer,0,player->getPlayerOfficerMax()+1);
        width += GENERALICONDIS;
    }
    


    realwidth = width;
    if(width <= SGLayout::getSkewing(SUBSCROLLWWID))
    {
        //如果没有Scrollview那么宽，就设置成scrollview 的宽度
        width = SGLayout::getSkewing(SUBSCROLLWWID);
    }
    
    layer->setContentSize(CCSizeMake(width, 112));
    scrollview->setContainer(layer);
    scrollview->setContentSize(CCSizeMake(width, 112));
    scrollview->setPosition(ccpAdd(background1->getPosition(), ccp(skewing(14), -135)));
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(scrollview);
    scrollview->setTag(222);
    
    CCArray *_array = playerInfo->getrewardNotice();
    CCObject *obj = NULL;
    int i = 1;
    //Attention! i从1开始,与rewardlayer中的号差1
    
    //领奖红点提示
    CCARRAY_FOREACH(_array, obj)
    {
        SGRewardNoticeItem * item = (SGRewardNoticeItem*)obj;
        if (item->getstateItem() == 1 && i!=7 && i!=8 && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev() )
        {
            spr_notice->setVisible(true);
            break;
        }
        i++;
    }
    
    //每日任务的红点提示
    if (playerInfo->getCanRewardDayTask() && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev())
    {
        spr_noticeDayTask->setVisible(true);
    }
    
    
    //左右箭头
    left = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    if (left)
    {
        this->addChild(left);
        left->setPosition(ccpAdd(background1->getPosition(), ccp(skewing(10), -80)));
    }
    rgt = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    if(rgt)
    {
        this->addChild(rgt);
        rgt->setFlipX(true);
        rgt->setPosition(ccpAdd(background1->getPosition(), ccp(skewing(SUBSCROLLWWID + 21), -80)));
    }
    
    //武将按钮
    officerBtn = SGButton::create("firstLayerOfficer.png", NULL, this, menu_selector(SGFirstLayer::buttonClickOfficer),CCPointZero,false,true);
	officerBtn->setPosition(ccpAdd(background1->getPosition(), ccp(skewing(SUBSCROLLWWID + 40), -80)) );
    this->addBtn(officerBtn);
    modelTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_SUMMON_OFFICER, 3);
    officerBtn->setTag(modelTag);
    //removed by cgp
    officerBtn->setVisible(false);
   // officerBtn->setEnabled(false);
    
    spr_notice_officer = CCSprite::createWithSpriteFrameName("publc_notice.png");
    spr_notice_officer->setPosition(ccpAdd(officerBtn->getPosition(), ccp(officerBtn->getContentSize().width*.37, officerBtn->getContentSize().height*.38)));
    spr_notice_officer->setVisible(false);
    this->addChild(spr_notice_officer,100);
    
    //武将碎片可召唤
    bool temp = false;
    if(playerInfo->getPlayerLevel() > playerInfo->getShowPlayerNoticeLimitLev())
    {
        CCArray * array = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
        for(int i=0; i<array->count() ; ++i)
        {
            SGPiecesCard * card = (SGPiecesCard*)array->objectAtIndex(i);
            SGPiecesDataModel * model = SGStaticDataManager::shareStatic()->getPiecesDictById(card->getItemId());
            bool hasCurOfficer = playerInfo->alreadyHasTheOfficer( model->getOfficerProtoId() );
            if(card->getPiecesCurNum() >= card->getSummonNeed() && !hasCurOfficer)
            {
                temp = true;
                break;
            }
        }
    }
    if(temp && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev())
    {
        spr_notice_officer->setVisible(true);
    }
    else
    {
        spr_notice_officer->setVisible(false);
    }

    this->schedule(schedule_selector(SGFirstLayer::scrollViewDidScroll),.5f);
    this->menu->setTouchPriority(-254);
    this->menu->m_bNeedMoveItems = true;
}


//特殊副本战斗入口
void SGFirstLayer::specialBattleHandler(CCObject *obj)
{
    CCLOG("Special Battle Entry");
    
    SGPlayerInfo *player=SGPlayerInfo::sharePlayerInfo();
    if (!player->canBreakLimitById(limitSpecailBattle))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str23,
                                                                           player->getLimitLevelById(limitSpecailBattle))->getCString() );
        return;
    }
    
    main::SpecialBattleStoryListRequest *storyList = new main::SpecialBattleStoryListRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_CHAP, storyList);
}



void SGFirstLayer::updateNoticeBuZhen()
{
    bool isVisible = false;
    int soulNum = 0;
    CCArray *soulCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject *obj_ = NULL;
    CCARRAY_FOREACH(soulCards, obj_)
    {
        
        SGPropsCard *card = (SGPropsCard *)obj_;
        if (card && card->getType() == 3 )
        {
            soulNum++;
        }
    }
    
    int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    int soldierId = 0;
    CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
    CCObject *obj1 = NULL;
    int playerLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    bool soldierLevelFlag = false;
    
    for (int i = 0; i<3; i++)
    {
        switch (i)
        {
            case 0:
            {
                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldierblue(teamid);
            }
                break;
            case 1:
            {
                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldierred(teamid);
            }
                break;
            case 2:
            {
                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldiergreen(teamid);
            }
                
                break;
            default:
            {
                soldierId = 0;
            }
                break;
        }
        
        CCARRAY_FOREACH(allSoldiers, obj1)
        {
            SGSoldierCard *card = (SGSoldierCard *)obj1;
            if (card->getItemId() == soldierId)
            {
                if (card->getCurrLevel() < playerLevel)
                {
                    soldierLevelFlag = true;
                    break;
                }
            }
        }
        
        if (soldierLevelFlag)
        {
            break;
        }
        
    }
    
    if(soldierLevelFlag && soulNum >= 5)
    {
        isVisible = true;
    }
    else
    {
        isVisible=false;
    }
    

}
void SGFirstLayer::taskHandlerSend()
{
    EFFECT_PLAY(MUSIC_BTN);
#if( AllTaskOpen == 1)
    main::DayTaskListRequest *request = new main::DayTaskListRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_LIST, request);
#else
    SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif


}
void SGFirstLayer::buttonClickChatAndMakeFriend()
{
    CCLOG("聊天交友");

    SGMainLayer *mainLayer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);
}

void SGFirstLayer::buttonClickStrengthenShow()
{
   EFFECT_PLAY(MUSIC_BTN);
#if(AllTaskOpen == 1)
        CCLOG("强化");
        SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
        SGMainManager::shareMain()->showBox(strengBox);
    
        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if (guideId >= guide_tag_10)
        {
            SGGuideManager::shareManager()->checkIsDone(guide_tag_13);
        }
#else
     SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif
    
   

}

void SGFirstLayer::buttonClickOfficer()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMainManager::shareMain()->showGeneralsBagLayer(0);
    //SGMainManager::shareMain()->showGeneralsLayer(0);
    
    
//    SGMainManager::shareMain()->showGeneralsLayer(3);
//    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
//    if (guideId >= guide_tag_19)
//    {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_21);
//    }
}
void SGFirstLayer::buttonClickGeneralsShow()
{
    EFFECT_PLAY(MUSIC_BTN);
    //SGMainManager::shareMain()->showGeneralsLayer(0);
    
//    main::RankRequest* req = new main::RankRequest();
//    req->set_type(0);
//    req->set_nationid(0);
//    req->set_page(1);
//    SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
}


void SGFirstLayer::showRewardsLayer()
{
    EFFECT_PLAY(MUSIC_BTN);
#if( AllTaskOpen == 1)
    SGMainManager::shareMain()->showRewardsLayer();
#else
    
#endif


}


void SGFirstLayer::buttonClickMailShow()
{
    EFFECT_PLAY(MUSIC_ITEM);
    CCLOG("邮件");
    SGMail *mail = SGMail::create();
    SGMainManager::shareMain()->showLayer(mail);
}

#pragma mark 功能

//显示邮件数量
void SGFirstLayer::setMailCount(int count)
{
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    spriteMailCount->setVisible(0 != count && player->getPlayerLevel() >= player->getShowPlayerNoticeLimitLev());
    labelMailCount->setVisible(spriteMailCount->isVisible());
    if(0 != count)
    {
        CCString *string = CCString::createWithFormat("%d",count);
        labelMailCount->setString(string->getCString());
        if (count>99) {
            labelMailCount->setString("N");
        }
        
    }
}

 

void SGFirstLayer::setIsCanTouch(bool isTouch)
{
    strengthButton->setEnabled(isTouch);
    currTaskButton->setEnabled(isTouch);
    limitActiveButton->setEnabled(isTouch);
    
//    advanceButton->setEnabled(isTouch);
    officerBtn->setEnabled(isTouch);
    lingjiangButton->setEnabled(isTouch);
    mailButton->setEnabled(isTouch);
    visitButton->setEnabled(isTouch);
    
    setMoveBtAbles(isTouch);
    
//    buzhenBt->setEnabled(isTouch);
    officerBtn->setEnabled(isTouch);
    scrollview->setTouchEnabled(isTouch);
    
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    for (int i = 0 ; i<player->getPlayerOfficerMax()+2; i++) {
        if ((SGFirstSubLayer *)(scrollview->getContainer()->getChildByTag(i))) {
            ((SGFirstSubLayer *)(scrollview->getContainer()->getChildByTag(i)))->setiscantouch(isTouch);
        }
    }

    menu->setEnabled(isTouch);
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(isTouch);
    layer->setPortraitMenuCanTouch(isTouch);
}
void SGFirstLayer::scrollViewDidScroll()
{
//    EFFECT_PLAY(MUSIC_BTN);
//    float position = scrollview->getContentOffset().x;
//    if(rgt)
//    rgt->setVisible((position+realwidth) >skewing(SUBSCROLLWWID)?true:false);
//    if (left)
//        left->setVisible(position<0);

}
void SGFirstLayer::setFriendCount(int count)
{
    CCLOG("need friend count reFrash");
}
void SGFirstLayer::fubenHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
#if(AllTaskOpen == 1)
    {
        SGMainManager::shareMain()->requestFirstActivityLayer();
    }
#else
        SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif

}

//试图获取守荆州主界面信息
void SGFirstLayer::sjzInfoRequest()
{
#if(AllTaskOpen == 1)
        if (SGPlayerInfo::sharePlayerInfo()->getIspvp()) {
            SG_SHOW_WINDOW(str_FirstLayer_str10);
            return;
        }
        EFFECT_PLAY(MUSIC_ITEM);
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
        //守荆州20级限制修改 在守荆州页面加
        if (!player->canBreakLimitById(limitShouJinZhou))
        {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str11,player->getLimitLevelById(limitShouJinZhou))->getCString() );
        }
        else
        {
    //        int count = player->getOfficerCards()->count() + player->getEquipCards()->count() + player->getPropCards()->count() + player->getSoldierCards()->count();
            int count = player->getGoodsNumInBag();
            //在引导中不会出现背包满提示
            if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide) {
                SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getNowShowLayer(), NULL, 12, count);
                SGMainManager::shareMain()->showBox(cantadvanceBox);
            }
            else
            {
                //发送获取详情
                SGMainManager::shareMain()->sendGetSjzInfoRequest();
            }
       }
#else
    SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif

    

}

void SGFirstLayer::bossHandler()
{
    /*测试用例
    BossReward *bossreward=new BossReward();
    BossBattleData *bossData=new BossBattleData();
    SGMainManager::shareMain()->showBossBattleLayer(bossData,bossreward);
    SGBossBattleLayer *layer=(SGBossBattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossBattleLayer);
    if (layer) {
        layer->startAction();
    }
     */
    
#if(AllTaskOpen == 1)
        SGPlayerInfo *player=SGPlayerInfo::sharePlayerInfo();
        if (!player->canBreakLimitById(limitBossBattle)) {
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str12,player->getLimitLevelById(limitBossBattle))->getCString() );
            return;
        }
        if (SGPlayerInfo::sharePlayerInfo()->getIspvp()==true) {
            SG_SHOW_WINDOW(str_FirstLayer_str13);
            return;
        }
    
        main::MainBossRequest *request=new main::MainBossRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
    
#else
        SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif
    

    

}

void SGFirstLayer::setMoveBtAbles(bool enable)
{
    float distanceLi = s.width*0.2;
    float distanceBt = 0;
    distanceBt = daguanButton->getPositionX() - ItemShowPosX;
    distanceBt = distanceBt >0?distanceBt:-distanceBt;
    daguanButton->setEnabled( (distanceBt>distanceLi?false:true) && enable );
    
    distanceBt = fubenButton->getPositionX() - ItemShowPosX;
    distanceBt = distanceBt >0?distanceBt:-distanceBt;
    fubenButton->setEnabled( (distanceBt>distanceLi?false:true) && enable );
    
    distanceBt = netbattleButton->getPositionX() - ItemShowPosX;
    distanceBt = distanceBt >0?distanceBt:-distanceBt;
    netbattleButton->setEnabled( (distanceBt>distanceLi?false:true) && enable );
    
    distanceBt = shouJinZhouButton->getPositionX() - ItemShowPosX;
    distanceBt = distanceBt >0?distanceBt:-distanceBt;
    shouJinZhouButton->setEnabled( (distanceBt>distanceLi?false:true) && enable );
    
    distanceBt = bossButton->getPositionX() - ItemShowPosX;
    distanceBt = distanceBt >0?distanceBt:-distanceBt;
    bossButton->setEnabled( (distanceBt>distanceLi?false:true) && enable );
    
    distanceBt = specialBattleButton->getPositionX() - ItemShowPosX;
    distanceBt = distanceBt >0?distanceBt:-distanceBt;
    specialBattleButton->setEnabled( (distanceBt>distanceLi?false:true) && enable );
}

void SGFirstLayer::updateItemPos(float dt)
{
    if (moveDist > 0) {
        CCPoint newPos = ccp((isMoveRight == true?moveAddDist:-moveAddDist), 0);
        moveFirstItems(newPos);
        
    }
    else
    {
        isMove = false;
        unschedule(schedule_selector(SGFirstLayer::updateItemPos));
        //设置按钮enable
        setMoveBtAbles();
        
        return;
    }
    moveDist -= moveAddDist;
}

void SGFirstLayer::dealMenuMoveEnd(CCTouch *pTouch,CCNode *selectedMenuItem)
{
    EFFECT_PLAY(MUSIC_ITEM);
    CCPoint touchLocation = pTouch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    float distance = 0;
    if ( touchLocation.x - moveStartPosX >0)
    {
        //右移
        isMoveRight = true;
        if (fubenButton)
        {
            distance = fubenButton->getPositionX() - ItemShowPosX;
            distance = distance<0?distance:-fubenButton->getContentSize().width;
            moveDist = distance;
        }
        if (daguanButton)
        {
            distance = daguanButton->getPositionX() - ItemShowPosX;
            distance = distance<0?distance:-daguanButton->getContentSize().width;
            moveDist = distance>moveDist?distance:moveDist;
        }
        if (netbattleButton)
        {
            distance = netbattleButton->getPositionX() - ItemShowPosX;
            distance = distance<0?distance:-netbattleButton->getContentSize().width;
            moveDist = distance>moveDist?distance:moveDist;
        }
        if (shouJinZhouButton)
        {
            distance = shouJinZhouButton->getPositionX() - ItemShowPosX;
            distance = distance<0?distance:-shouJinZhouButton->getContentSize().width;
            moveDist = distance>moveDist?distance:moveDist;
        }
        if (bossButton)
        {
            distance = bossButton->getPositionX() - ItemShowPosX;
            distance = distance<0?distance:-bossButton->getContentSize().width;
            moveDist = distance>moveDist?distance:moveDist;
        }
        if (specialBattleButton)
        {
            distance = specialBattleButton->getPositionX() - ItemShowPosX;
            distance = distance<0?distance:-specialBattleButton->getContentSize().width;
            moveDist = distance>moveDist?distance:moveDist;
        }
        moveDist = -moveDist;
    }
    else
    {
        //左移
        isMoveRight = false;
        if (fubenButton)
        {
            distance = fubenButton->getPositionX() - ItemShowPosX;
            distance = distance>0?distance:fubenButton->getContentSize().width;
            moveDist = distance;
        }
        if (daguanButton)
        {
            distance = daguanButton->getPositionX() - ItemShowPosX;
            distance = distance>0?distance:daguanButton->getContentSize().width;
            moveDist = distance>moveDist?moveDist:distance;
        }

        if (netbattleButton)
        {
            distance = netbattleButton->getPositionX() - ItemShowPosX;
            distance = distance>0?distance:netbattleButton->getContentSize().width;
            moveDist = distance>moveDist?moveDist:distance;
        }
        if (shouJinZhouButton)
        {
            distance = shouJinZhouButton->getPositionX() - ItemShowPosX;
            distance = distance>0?distance:shouJinZhouButton->getContentSize().width;
            moveDist = distance>moveDist?moveDist:distance;
        }
        if (bossButton)
        {
            distance = bossButton->getPositionX() - ItemShowPosX;
            distance = distance>0?distance:bossButton->getContentSize().width;
            moveDist = distance>moveDist?moveDist:distance;
        }
        if (specialBattleButton)
        {
            distance = specialBattleButton->getPositionX() - ItemShowPosX;
            distance = distance>0?distance:specialBattleButton->getContentSize().width;
            moveDist = distance>moveDist?moveDist:distance;
        }
    }
    moveAddDist = moveDist/4;
    moveDist -= 0.1;
    schedule(schedule_selector(SGFirstLayer::updateItemPos), 0.02);
    
}

void SGFirstLayer::dealMenuMove(CCTouch *pTouch,CCNode *selectedMenuItem)
{
    
    //removed by cgp
//    if (selectedMenuItem == shouJinZhouButton
//        || selectedMenuItem == fubenButton
//        || selectedMenuItem == netbattleButton
//        || selectedMenuItem == daguanButton
//        || selectedMenuItem == bossButton
//        || selectedMenuItem == specialBattleButton)
//    {
//        unschedule(schedule_selector(SGFirstLayer::updateItemPos));
//        CCPoint touchLocation = pTouch->getLocationInView();
//        CCPoint prevLocation = pTouch->getPreviousLocationInView();
//        
//        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
//        prevLocation = CCDirector::sharedDirector()->convertToGL(prevLocation);
//        CCPoint diff = ccpSub(touchLocation, prevLocation);
//
//        //判断时间大于阈值且移动速度大于阈值那么重新设置上一次的移动矢量且更新开始移动的时间
//        if (!isMove)
//        {
//            if ( firstTouchPos.equals(CCPointZero)) {
//                firstTouchPos = touchLocation;
//            }
//            float distance = ccpDistance(touchLocation, firstTouchPos);
//            if (distance >10) {//1.误移判断
//                isMove = true; //2.响应处理
//                moveStartPosX = touchLocation.x;
//                firstTouchPos = CCPointZero;
//            }
//        }
//        else
//        {
//            diff.y = 0;
//            diff.x =diff.x*0.6;//3.阻力值
//            moveFirstItems(diff);
//        }
//    }
//    else if(isMove == true)
//    {
//        //已经移动，但是移出,按照移动结束处理开始吸附
//        dealMenuMoveEnd(pTouch,selectedMenuItem);
//    }
}

void SGFirstLayer::moveFirstItems(CCPoint &newPos)
{
    float distance = 0;
    float scale = 0;
    float showDianPos = 0;
    float showDianScale = 0;
    int color = INITCOLOR;//!!!@!@!@@!@!@!::每设置过一个按钮 ，请恢复color的此初始值。否则 你懂的。
    if (fubenButton && specialBattleButton)
    {
        if (fubenButton->getPositionX() +newPos.x+fubenButton->getContentSize().width*DEFAULTSCALE < ItemInitPosX) {
            fubenButton->setPositionX(specialBattleButton->getPositionX()+newPos.x+specialBattleButton->getContentSize().width*DEFAULTSCALE/2+fubenButton->getContentSize().width*DEFAULTSCALE/2);
            
        }
        else if (fubenButton->getPositionX() +newPos.x > s.width + fubenButton->getContentSize().width*DEFAULTSCALE)
        {
            fubenButton->setPositionX(daguanButton->getPositionX() +newPos.x- daguanButton->getContentSize().width*DEFAULTSCALE/2 - fubenButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else
        {
            fubenButton->setPositionX(newPos.x+fubenButton->getPositionX());
        }
        
        distance = fubenButton->getPositionX() - ItemShowPosX;
        distance = distance>0?distance:(-distance);
        if (distance > s.width*0.4) {
            fubenButton->setScale(DEFAULTSCALENEW);
            fubenButton->setFontImageScale(1);
            fubenButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
        }
        else
        {
            scale = showScale-distance*(showScale - DEFAULTSCALENEW)/s.width *2;
            fubenButton->setScale(scale);
            fubenButton->setFontImageScale(fubenButton->getScale());
            color += (int)(255 - INITCOLOR)*(scale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);
            fubenButton->setColor(ccc3(color,color,color));
            color = INITCOLOR;
            if (showDianScale < scale) {
                showDianPos = liangDian_1->getPositionX();
                showDianScale = scale;
            }
        }
        fubenButton->setFontImageColor(fubenButton->getColor());
    }

    if (daguanButton && fubenButton )
    {
        if (daguanButton->getPositionX() +newPos.x+daguanButton->getContentSize().width*DEFAULTSCALE<ItemInitPosX) {
            daguanButton->setPositionX(fubenButton->getPositionX()+fubenButton->getContentSize().width*DEFAULTSCALE/2+daguanButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else if (daguanButton->getPositionX() +newPos.x > s.width + daguanButton->getContentSize().width*DEFAULTSCALE)
        {
            daguanButton->setPositionX(netbattleButton->getPositionX() +newPos.x- netbattleButton->getContentSize().width*DEFAULTSCALE/2 - daguanButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else
        {
            daguanButton->setPositionX(newPos.x+daguanButton->getPositionX());
        }
        
        distance = daguanButton->getPositionX() - ItemShowPosX;
        distance = distance>0?distance:(-distance);
        if (distance > s.width*0.4) {
            daguanButton->setScale(DEFAULTSCALENEW);
            daguanButton->setFontImageScale(1);
            daguanButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
        }
        else
        {
            scale = showScale-distance*(showScale - DEFAULTSCALENEW)/s.width *2;
            daguanButton->setScale(scale);
            daguanButton->setFontImageScale(daguanButton->getScale());
            
            color += (int)(255 - INITCOLOR)*(scale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);
            daguanButton->setColor(ccc3(color,color,color));
            color = INITCOLOR;
            if (showDianScale < scale) {
                showDianPos = liangDian_2->getPositionX();
                showDianScale = scale;
            }
        }
        daguanButton->setFontImageColor(daguanButton->getColor());
    }
    
    if (netbattleButton && daguanButton)
    {
        if (netbattleButton->getPositionX()+newPos.x+netbattleButton->getContentSize().width*DEFAULTSCALE<ItemInitPosX) {
            netbattleButton->setPositionX(daguanButton->getPositionX()+daguanButton->getContentSize().width*DEFAULTSCALE/2+netbattleButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else if (netbattleButton->getPositionX() +newPos.x > s.width + netbattleButton->getContentSize().width*DEFAULTSCALE)
        {
            netbattleButton->setPositionX(shouJinZhouButton->getPositionX() +newPos.x- shouJinZhouButton->getContentSize().width*DEFAULTSCALE/2 - netbattleButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else
        {
           netbattleButton->setPositionX(newPos.x+netbattleButton->getPositionX());
        }
        
        distance = netbattleButton->getPositionX() - ItemShowPosX;
        distance = distance>0?distance:(-distance);
        if (distance > s.width*0.4) {
            netbattleButton->setScale(DEFAULTSCALENEW);
            netbattleButton->setFontImageScale(1);
            netbattleButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
        }
        else
        {
            scale = showScale-distance*(showScale - DEFAULTSCALENEW)/s.width *2;
            netbattleButton->setScale(scale);
            netbattleButton->setFontImageScale(netbattleButton->getScale());
            color += (int)(255 - INITCOLOR)*(scale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);
            netbattleButton->setColor(ccc3(color,color,color));
            color = INITCOLOR;
            if (showDianScale < scale) {
                showDianPos = liangDian_3->getPositionX();
                showDianScale = scale;
            }
        }
        netbattleButton->setFontImageColor(netbattleButton->getColor());
    }
    
    if (shouJinZhouButton && netbattleButton  && bossButton)
    {
        if (shouJinZhouButton->getPositionX() +newPos.x+shouJinZhouButton->getContentSize().width*DEFAULTSCALE<ItemInitPosX) {
            shouJinZhouButton->setPositionX(netbattleButton->getPositionX()+netbattleButton->getContentSize().width*DEFAULTSCALE/2+shouJinZhouButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else if (shouJinZhouButton->getPositionX() +newPos.x > s.width + shouJinZhouButton->getContentSize().width*DEFAULTSCALE)
        {
            shouJinZhouButton->setPositionX(bossButton->getPositionX() - bossButton->getContentSize().width*DEFAULTSCALE/2 - shouJinZhouButton->getContentSize().width*DEFAULTSCALE/2 +newPos.x);
        }
        else
        {
            shouJinZhouButton->setPositionX(newPos.x+shouJinZhouButton->getPositionX());
        }
        distance = shouJinZhouButton->getPositionX() - ItemShowPosX;
        distance = distance>0?distance:(-distance);
        if (distance > s.width*0.4) {
            shouJinZhouButton->setScale(DEFAULTSCALENEW);
            shouJinZhouButton->setFontImageScale(1);
            shouJinZhouButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
        }
        else
        {
            scale = showScale-distance*(showScale - DEFAULTSCALENEW)/s.width *2;
            shouJinZhouButton->setScale(scale);
            shouJinZhouButton->setFontImageScale(shouJinZhouButton->getScale());
            color += (int)(255 - INITCOLOR)*(scale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);
            shouJinZhouButton->setColor(ccc3(color,color,color));
            
            color = INITCOLOR;
            if (showDianScale < scale) {
                showDianPos = liangDian_4->getPositionX();
                showDianScale = scale;
            }
        }
        shouJinZhouButton->setFontImageColor(shouJinZhouButton->getColor());
    }
    
    
    
    if (shouJinZhouButton && specialBattleButton &&  bossButton)
    {
        if (bossButton->getPositionX() +newPos.x+bossButton->getContentSize().width*DEFAULTSCALE<ItemInitPosX) {
            bossButton->setPositionX(shouJinZhouButton->getPositionX()+shouJinZhouButton->getContentSize().width*DEFAULTSCALE/2+bossButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else if (bossButton->getPositionX() +newPos.x > s.width + bossButton->getContentSize().width*DEFAULTSCALE)
        {
            bossButton->setPositionX(specialBattleButton->getPositionX() - specialBattleButton->getContentSize().width*DEFAULTSCALE/2 - bossButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else
        {
            bossButton->setPositionX(newPos.x+bossButton->getPositionX());
        }
        distance = bossButton->getPositionX() - ItemShowPosX;
        distance = distance>0?distance:(-distance);
        if (distance > s.width*0.4) {
            bossButton->setScale(DEFAULTSCALENEW);
            bossButton->setFontImageScale(1);
            bossButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
        }
        else
        {
            scale = showScale-distance*(showScale - DEFAULTSCALENEW)/s.width *2;
            bossButton->setScale(scale);
            bossButton->setFontImageScale(bossButton->getScale());
            color += (int)(255 - INITCOLOR)*(scale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);
            bossButton->setColor(ccc3(color,color,color));
            color = INITCOLOR;
            if (showDianScale < scale) {
                showDianPos = liangDian_6->getPositionX();
            }
        }
        bossButton->setFontImageColor(bossButton->getColor());
    }
    
    
    
    if (fubenButton &&  bossButton && specialBattleButton)
    {
        if (specialBattleButton->getPositionX() +newPos.x+specialBattleButton->getContentSize().width*DEFAULTSCALE<ItemInitPosX) {
            specialBattleButton->setPositionX(bossButton->getPositionX()+bossButton->getContentSize().width*DEFAULTSCALE/2
                                              +specialBattleButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else if (specialBattleButton->getPositionX() +newPos.x > s.width + specialBattleButton->getContentSize().width*DEFAULTSCALE)
        {
            specialBattleButton->setPositionX(fubenButton->getPositionX() - fubenButton->getContentSize().width*DEFAULTSCALE/2
                                              - specialBattleButton->getContentSize().width*DEFAULTSCALE/2);
        }
        else
        {
            specialBattleButton->setPositionX(newPos.x+specialBattleButton->getPositionX());
        }
        distance = specialBattleButton->getPositionX() - ItemShowPosX;
        distance = distance>0?distance:(-distance);
        if (distance > s.width*0.4) {
            specialBattleButton->setScale(DEFAULTSCALENEW);
            specialBattleButton->setFontImageScale(1);
            specialBattleButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
        }
        else
        {
            scale = showScale-distance*(showScale - DEFAULTSCALENEW)/s.width *2;
            specialBattleButton->setScale(scale);
            specialBattleButton->setFontImageScale(specialBattleButton->getScale());
            color += (int)(255 - INITCOLOR)*(scale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);
            specialBattleButton->setColor(ccc3(color,color,color));
            color = INITCOLOR;
            
            if (showDianScale < scale) {
                showDianPos = liangDian_7->getPositionX();
            }
        }
        specialBattleButton->setFontImageColor(specialBattleButton->getColor());
    }
    
    

    

    
    liangDian_5->setPositionX(showDianPos);
}

void SGFirstLayer::testStoryLayer()
{
    if (SGPlayerInfo::sharePlayerInfo()->getIspvp()==true)
    {
        SG_SHOW_WINDOW(str_FirstLayer_str15);  //"你在參加PVP,不能參加闖關"//
    }
    else
    {
        SGMainScene *mainScence = SGMainManager::shareMain()->getMainScene();
        int tag = mainScence->nowLayerTag;
        if (tag == sg_firstLayer)   //首页//
        {
            SGMainManager::shareMain()->getMainLayer()->storyHandler();
        }

    }
}

void SGFirstLayer::resetMoveBtns(int buttonId)
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    daguanButton->setScale(DEFAULTSCALENEW);
    netbattleButton->setScale(DEFAULTSCALENEW);
    shouJinZhouButton->setScale(DEFAULTSCALENEW);
    fubenButton->setScale(DEFAULTSCALENEW);
    bossButton->setScale(DEFAULTSCALENEW);
    specialBattleButton->setScale(DEFAULTSCALENEW);
    
    fubenButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
    daguanButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
    netbattleButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
    shouJinZhouButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
    bossButton->setColor(ccc3(INITCOLOR,INITCOLOR,INITCOLOR));
    specialBattleButton->setColor(ccc3(INITCOLOR, INITCOLOR, INITCOLOR));
    
    fubenButton->setFontImageColor(fubenButton->getColor());
    daguanButton->setFontImageColor(daguanButton->getColor());
    netbattleButton->setFontImageColor(netbattleButton->getColor());
    shouJinZhouButton->setFontImageColor(shouJinZhouButton->getColor());
    bossButton->setFontImageColor(bossButton->getColor());
    specialBattleButton->setFontImageColor(specialBattleButton->getColor());
    
    daguanButton->setEnabled(false);
    fubenButton->setEnabled(false);
    netbattleButton->setEnabled(false);
    shouJinZhouButton->setEnabled(false);
    bossButton->setEnabled(false);
    specialBattleButton->setEnabled(false);
    
    
    
   if(buttonId == 1)
    {
        //fuben
        fubenButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        daguanButton->setPosition(ccpAdd(fubenButton->getPosition(), ccp(daguanButton->getContentSize().width*DEFAULTSCALE,0)));
        netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
        shouJinZhouButton->setPosition(ccpAdd(fubenButton->getPosition(), ccp(-shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
        bossButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(-bossButton->getContentSize().width*DEFAULTSCALE, 0)));
        specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));
        
        liangDian_5->setPositionX(liangDian_1->getPositionX());
        
        fubenButton->setScale(showScale);
        fubenButton->setColor(ccWHITE);
        fubenButton->setFontImageColor(fubenButton->getColor());
        fubenButton->setEnabled(true);
    }
    else if (buttonId == 2)
    {
        //daguan
        daguanButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        fubenButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-fubenButton->getContentSize().width*DEFAULTSCALE,0)));
        netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
        shouJinZhouButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
        bossButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(bossButton->getContentSize().width*DEFAULTSCALE, 0)));
        specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));

        liangDian_5->setPositionX(liangDian_2->getPositionX());
        
        daguanButton->setScale(showScale);
        daguanButton->setColor(ccWHITE);
        daguanButton->setFontImageColor(daguanButton->getColor());
        daguanButton->setEnabled(true);
    }
    else if(buttonId == 3)
    {
        //netbattle
        netbattleButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        shouJinZhouButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
        daguanButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(-daguanButton->getContentSize().width*DEFAULTSCALE,0)));
        fubenButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-fubenButton->getContentSize().width*DEFAULTSCALE, 0)));
        bossButton->setPosition(ccpAdd(fubenButton->getPosition(), ccp(-bossButton->getContentSize().width*DEFAULTSCALE, 0)));
        specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-bossButton->getContentSize().width * DEFAULTSCALE, 0)));

        liangDian_5->setPositionX(liangDian_3->getPositionX());
        
        netbattleButton->setScale(showScale);
        netbattleButton->setColor(ccWHITE);
        netbattleButton->setFontImageColor(netbattleButton->getColor());
        netbattleButton->setEnabled(true);
    }

    else if(buttonId == 4)
    {
        //shouJinZhou
        shouJinZhouButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        fubenButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(fubenButton->getContentSize().width*DEFAULTSCALE,0)));
        netbattleButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(-netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
        daguanButton->setPosition(ccpAdd(fubenButton->getPosition(), ccp(daguanButton->getContentSize().width*DEFAULTSCALE, 0)));
        bossButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(bossButton->getContentSize().width*DEFAULTSCALE, 0)));
        specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));

        liangDian_5->setPositionX(liangDian_4->getPositionX());
        
        shouJinZhouButton->setScale(showScale);
        shouJinZhouButton->setColor(ccWHITE);
        shouJinZhouButton->setFontImageColor(shouJinZhouButton->getColor());
        shouJinZhouButton->setEnabled(true);
    }
    else if(buttonId == 5)
    {
        //boss
        bossButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        daguanButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-daguanButton->getContentSize().width*DEFAULTSCALE,0)));
        fubenButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-fubenButton->getContentSize().width*DEFAULTSCALE,0)));
        netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
        shouJinZhouButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
        specialBattleButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(-specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));
        bossButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(bossButton->getContentSize().width*DEFAULTSCALE, 0)));

        liangDian_5->setPositionX(liangDian_6->getPositionX());
        
        bossButton->setScale(showScale);
        bossButton->setColor(ccWHITE);
        bossButton->setFontImageColor(daguanButton->getColor());
        bossButton->setEnabled(true);
    }
    else if (buttonId == 17)
    {
        //specialBattle
        specialBattleButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        bossButton->setPosition(ccpAdd(specialBattleButton->getPosition(), ccp(-bossButton->getContentSize().width*DEFAULTSCALE, 0)));
        fubenButton->setPosition(ccpAdd(specialBattleButton->getPosition(), ccp(fubenButton->getContentSize().width*DEFAULTSCALE,0)));

        daguanButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-daguanButton->getContentSize().width*DEFAULTSCALE,0)));
        netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
        shouJinZhouButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
        
        liangDian_5->setPositionX(liangDian_7->getPositionX());
        
        specialBattleButton->setScale(showScale);
        specialBattleButton->setColor(ccWHITE);
        specialBattleButton->setFontImageColor(specialBattleButton->getColor());
        specialBattleButton->setEnabled(true);
    }
    else
    {
        //daguan
        daguanButton->setPosition(ccp(center.x,SGMainManager::shareMain()->getBottomHeight()*layerRate));
        fubenButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(-fubenButton->getContentSize().width*DEFAULTSCALE,0)));
        netbattleButton->setPosition(ccpAdd(daguanButton->getPosition(), ccp(netbattleButton->getContentSize().width*DEFAULTSCALE, 0)));
        shouJinZhouButton->setPosition(ccpAdd(netbattleButton->getPosition(), ccp(shouJinZhouButton->getContentSize().width*DEFAULTSCALE, 0)));
        bossButton->setPosition(ccpAdd(shouJinZhouButton->getPosition(), ccp(bossButton->getContentSize().width*DEFAULTSCALE, 0)));
        specialBattleButton->setPosition(ccpAdd(bossButton->getPosition(), ccp(-specialBattleButton->getContentSize().width * DEFAULTSCALE, 0)));

        liangDian_5->setPositionX(liangDian_2->getPositionX());
        
        daguanButton->setScale(showScale);
        daguanButton->setColor(ccWHITE);
        daguanButton->setFontImageColor(daguanButton->getColor());
        daguanButton->setEnabled(true);
    }
}
void  SGFirstLayer::activityanimat(){
    
    CCScaleTo* scaleTo= CCScaleTo::create(1, 1.1);
    CCScaleTo* scaleTo1 = CCScaleTo::create(1, 1);
    CCSequence *scquence= CCSequence::create(scaleTo,scaleTo1,NULL);
    
    CCRepeatForever* repeatscale = CCRepeatForever::create(scquence);
    spr_noticactivity_gift->runAction(repeatscale);
}

//遮挡作用
void SGFirstLayer::pressMask()
{
//    if (buzhenBt)
//    {
//        buzhenBt->activate();
//    }
}

//void SGFirstLayer::gotoEmbattleLayer()
//{
//    SGMainManager::shareMain()->showEmbattleLayer(1,SGTeamgroup::shareTeamgroupInfo()->getiscurrent());
//
//}

//
//void SGFirstLayer::rankListener(CCObject*obj)
//{
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RANK);
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    if (sr)
//    {
//        main::RankResponse *rsp = (main::RankResponse *) sr->m_msg;
//        if (rsp)
//        {
//            
//                int size=rsp->size();
//    
//               //自己的信息
//                main::RankProto myrankinfo=rsp->rankinfo();
//                int myrank=myrankinfo.value();
//                int rankstate=myrankinfo.ranking();
// 
//            
//                
//            
//                CCArray*array=CCArray::create();
//                for (int i = 0; i < rsp->rankinfos_size(); i++)
//                {
//                   
//                    main::RankProto rankinfo= rsp->rankinfos(i);
//                    SGRankAllPlayer*player=new SGRankAllPlayer();
//                    player->setplayerId(rankinfo.roleid());
//                    std::string str(rankinfo.nickname());
//                    CCString* ns=CCString::createWithFormat("%s",str.c_str());
//                    player->setName(ns);
//                    player->setHeadId(rankinfo.nation());
//                    ////////
//                    player->setRank(rankinfo.value());
//                    player->setServerNum(rankinfo.areaid());
//                    std::string str2(rankinfo.areaname());
//                    CCString* ns2=CCString::createWithFormat("%s",str2.c_str());
//                    player->setServerName(ns2);
//                    
//                    //////////
//                    
//                    player->setIsIn(rankinfo.ranking()); ///是否进榜
//                    array->addObject(player);
//                }
//            
//            SGMainLayer *mainlayer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
//            mainlayer->setPortraitMenuCanTouch(false);
//            //mainlayer->setIsCanTouch(true);
//            SGMainManager::shareMain()->showRankLayerLayer(array,myrank,rankstate,size);
//            
//            }
// 
//        }
//}


void SGFirstLayer:: showFirstByTag(int tag)
{
    
    CCPoint bossPoint=bossButton->getPosition();
    CCPoint fubenPoint=fubenButton->getPosition();
    CCPoint daguanpoint=daguanButton->getPosition();
    CCPoint netPoint=netbattleButton->getPosition();
    CCPoint shouPoint=shouJinZhouButton->getPosition();
    CCPoint specialBattlePos = specialBattleButton->getPosition();
    
    if (tag==0) {
        
    }else if (tag==2)
    {
        bossButton->setPosition(daguanpoint);
        specialBattleButton->setPosition(netPoint);

//        fubenButton->setPosition(specialBattlePos);
//        daguanButton->setPosition(shouPoint);
//        netbattleButton->setPosition(bossPoint);
//        shouJinZhouButton->setPosition(fubenPoint);
        
        fubenButton->setPosition(shouPoint);
        daguanButton->setPosition(bossPoint);
        netbattleButton->setPosition(specialBattlePos);
        shouJinZhouButton->setPosition(fubenPoint);
        
        bossButton->setEnabled(true);
        bossButton->setScale(showScale);
//        int color= (int)(255 - INITCOLOR)*(showScale-DEFAULTSCALENEW)/(showScale-DEFAULTSCALENEW);

        bossButton->setColor(ccWHITE);
        bossButton->setFontImageColor(bossButton->getColor());
        liangDian_5->setPositionX(liangDian_6->getPositionX());
        
    }else
    {
        
    }
    
}
void SGFirstLayer::showAddAttributeEffect(int blood, int speed, int compbatle){
    
    CCPoint center=SGLayout::getPoint(kMiddleCenter);
    SGCCLabelTTF *label=SGCCLabelTTF::create("", FONT_PANGWA, 30 , ccc3(0x0c, 0xff, 0x00));
    label->setPosition(ccpAdd(center, ccp(100, 300)));
    label->setTag(110);
    //modify by:zyc. merge into create.
    //label->setColor(ccc3(0x0c, 0xff, 0x00));
    this->addChild(label,11111);
    label->setVisible(false);
    std::string attribute;
    CCString *buffer=NULL;
    if (blood>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str18,blood);
        attribute.append(buffer->getCString());
        label->setInsideColor(ccc3(0x0c, 0xff, 0x00));
    }
    else if(blood<0)
    {
        buffer=CCString::createWithFormat(str_FirstLayer_str16,blood);
        attribute.append(buffer->getCString());
        label->setInsideColor(ccRED);
    }
    if (speed>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str19,speed);
        attribute.append(buffer->getCString());
    }else if(speed<0)
    {
        buffer=CCString::createWithFormat(str_FirstLayer_str17,speed);
        attribute.append(buffer->getCString());
        label->setInsideColor(ccRED);
    }
    if (compbatle>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str20,compbatle);
        attribute.append(buffer->getCString());
    }else if (compbatle<0)
    {
        buffer=CCString::createWithFormat(str_FirstLayer_str18,compbatle);
        attribute.append(buffer->getCString());
        label->setInsideColor(ccRED);
    }
    
    
    
    
    CCMoveTo *moveTo=CCMoveTo::create(0.5+1.0, ccpAdd(center, ccp(100, 500)));
    
    label->setString(attribute.c_str());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGFirstLayer::showEffectLabel));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGFirstLayer::removeEffectLabel));
    
    CCMoveTo *moveTo2=CCMoveTo::create(0.5, ccpAdd(center, ccp(100,580)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, 0.7+0.5);
    label->runAction(CCSequence::create(fun1,label->fadin(0.5),easeIn,CCDelayTime::create(1),label->fadout(0.5),moveTo2,fun2,NULL));
}
void SGFirstLayer::showEffectLabel()
{
    
    SGCCLabelTTF *label=(SGCCLabelTTF *)this->getChildByTag(110);
    label->setVisible(true);
}
void SGFirstLayer::removeEffectLabel()
{
    
    SGCCLabelTTF *label=(SGCCLabelTTF *)this->getChildByTag(110);
    label->removeFromParentAndCleanup(true);
}

void SGFirstLayer::onShopBtnClicked()
{
    EFFECT_PLAY(MUSIC_BTN);
#if(AllTaskOpen == 1)
    SGMainManager::shareMain()->showShopLayer();
#else
    SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif
}

void SGFirstLayer::animateOverCallback()
{
    
    this->runAction(CCSequence::create(CCDelayTime::create(2) , CCCallFunc::create(this, callfunc_selector(SGFirstLayer::animateRePlay) ),NULL ));
}

void SGFirstLayer::animateRePlay()
{
}

void SGFirstLayer::rankBtnClicked()
{
    main::RankRequest* req = new main::RankRequest();
    req->set_type(0);
    req->set_nationid(0);
    req->set_page(1);
    SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
}


