  //
//  SGHeroLayer.cpp
//  GameSanGuo
//  英雄界面
//  Created by gengshengcai on 12-12-5.
//
//
#include "GlobalConfig.h"
#include "SGHeroLayer.h"
#include "SGLayout.h"
#include "SGBattleManager.h"
#include "GameConfig.h"
#include "SGAttackList.h"
#include "SGNotificationCenter.h"
#include "FightBaseModule.pb.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGSkillManager.h"
#include "SG_BuffBase.h"
#include "SGAIManager.h"
#include "SGBattleRoundPromptLayer.h"
#include "SGBattleRoundSurrenderLayer.h"
#include "SGMainManager.h"

#include "SGGuideManager.h"

#include "SGBattleLeaderSkillBox.h"
#include "SGBattleCallFriendBox.h"
#include "GameConfig.h"
#include "SGTools.h"
#include "SGBattleReviveLayer.h"
#include "SGAIManager.h"
#include "CCRenderTexture.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGBattleFillUpLayer.h"
#include "ArenaModule.pb.h"
#include "ResourceManager.h"
#include "CooperationModule.pb.h"
#include "SGStringConfig.h"
#include <algorithm>
#include "SGLogicManager.h"


//PLIST-各种文件
#define PLIST_BATTLE_UI "battle/battle_new/image/battleUiImages.plist"
#define PLIST_FAIL1 "battle/fail1.plist"
#define PLIST_FAIL2 "battle/fail2.plist"
#define PLIST_FAIL3 "battle/fail3.plist"
#define PLIST_FRAME_BLINK "battle/frameblink.plist"
#define PLIST_KEY_CHAIN "battle/keychain.plist"
#define PLIST_KEY_SPARK "battle/keyspark.plist"
#define PLIST_KEY_TANKFUL "battle/keytankful.plist"
#define PLIST_PLANT1 "battle/plant1.plist"
#define PLIST_PLANT2 "battle/plant2.plist"
#define PLIST_PLANT3 "battle/plant3.plist"
#define PLIST_WALL_BLINK "battle/wallblink.plist"
#define PLIST_WALL_CHAIN "battle/wallchain.plist"
#define PLIST_WALL_FIRE "battle/wallfire.plist"
#define PLIST_YJKFD "battle/YJKFD.plist"
#define PLIST_ZJJKFD "battle/ZJJKFD.plist"

//动画-主将计可发动
#define ANIM_ZJJKFD "battle/ZJJKFD.scml"
//动画-友军可发动
#define ANIM_YJKFD "battle/YJKFD.scml"
//动画-闪烁绿框
#define ANIM_FRAME_BLINK "battle/frameblink.scml"
//动画-满槽光芒
#define ANIM_KEY_TANKFUL "battle/keytankful.scml"
//动画-满槽火焰
#define ANIM_KEY_SPARK "battle/keyspark.scml"
//动画-城墙侧边铁链
#define ANIM_WALL_CHAIN "battle/wallchain.scml"
//动画-铭牌铁链
#define ANIM_KEY_CHAIN "battle/keychain.scml"
//动画-城门失火
#define ANIM_WALL_FIRE "battle/wallfire.scml"
//动画-城墙闪烁
#define ANIM_WALL_BLINK "battle/wallblink.scml"
//动画-花草1
#define ANIM_PLANT1 "battle/plant1.scml"
//动画-花草2
#define ANIM_PLANT2 "battle/plant2.scml"
//动画-花草3
#define ANIM_PLANT3 "battle/plant3.scml"

//铁链晃动动画持续时间
#define MAX_CHAIN_TIME 1.0f
//按钮槽涨满时间
#define MAX_BUTTON_TIME 1.0f   //before is 2.4
//城门失血涨满时间
#define MAX_GATE_TIME 1.8f

//城门状态 完整-划痕-破裂-倒塌
#define GATE_STATE_GOOD 100
#define GATE_STATE_BROKEN 80
#define GATE_STATE_CRUSHED 50
#define GATE_STATE_DESTROYED 0

//城门状态贴图 同上
#define GATE_IMAGE_GOOD "gate0.png"
#define GATE_IMAGE_BROKEN "gate1.png"
#define GATE_IMAGE_CRUSHED "gate2.png"
#define GATE_IMAGE_DESTROYED "gate3.png"

////血量"绿橙红"状态贴图
//#define GATE_BLOOD_GREEN "myBloodGreen.png"
//#define GATE_BLOOD_ORANGE "myBloodOrange.png"
//#define GATE_BLOOD_RED "myBloodRed.png"

//血量"绿橙红"状态贴图
#define GATE_BLOOD_GREEN "life.png"
#define GATE_BLOOD_ORANGE "life.png"
#define GATE_BLOOD_RED "life.png"

#define BATTLENUMMOVETIME  0.3


//动画速度, 暂定三倍于原来动画速度
#define ANIMATION_3SPEED     3
#define ANIMATION_1SPEED     1
//#define ANIMATION_1SPEED     0.2  //modified by cgp

//呼吸动画的渐变时间与停留时间
#define BREATH_CHANGE_TIME  1.5f
#define BREATH_STAY_TIME    0.5f

//added by cgp
#define DELAY_TIME 1.5
#define TOP_ZORDER 20
#define ATTACK_GAP_TIME 0.25





HeroObj::HeroObj()
: h_roleId(0)
, h_basehp(0)
, h_hp(0)
, h_actionCount(0)
, h_actionCount1(0)
, h_ItemId(0)
, h_FriendLevel(0)
, h_FriendHeadId(0)
, h_skillId(0)
, h_SkillRound(0)
, h_SkillRoundCur(0)
, h_ClientType(0)
, h_Country(0)
, h_Atkmode(0)
, h_max_soldierNum(0)
, h_FriendRound(0)
, _friendRound(0)
, h_LordSkillLevel(0)
, h_LordSkillID(0)
, h_name(NULL)

{
    
}

HeroObj::~HeroObj()
{
    
}


SGHeroLayer::SGHeroLayer()
:battleMap(NULL)
,buffs(NULL)
,isBattleOver(false)
,isRefreshMap(false)
,skillsRoundNum(0)
,heroBloodMax(0)
,m_skillsId(0)
,m_skillLevel(1)
,m_skillName(NULL)
,clientType(0)
,bulletJ(3)
,isAttackBase(false)
,uibgY(0)
,m_jianId(0)
, max_soldierNum(0)
,friendNum(0)
,isInitPve(false)
,initFriendNum(0)
,isFriend(false)
,itemId(0)
,friendLevel(0)
,friendHeadId(0)
,isFriendOffDeath(false)
,m_LJNum(0)
,sbs(NULL)
,roleId(0)
,isAttackOver(true)
,_initRoundTime(0)
,_roundTime(0)
,roundTimeLabel(NULL)
,_controlRoundNum(-1)
,m_RoundTimeT(NULL)
,tbj(NULL)
,dangqianxie(0)
,isRoundEnd(false)
,statusBarLayer(NULL)
,headEffArray(NULL)
,m_ljBj(NULL)
,las(NULL)
,zjj_chain(NULL)
,yj_chain(NULL)
,xdl_chain(NULL)
,yb_chain(NULL)
,wall_left_chain_1(NULL)
,wall_left_chain_2(NULL)
,wall_right_chain_1(NULL)
,wall_right_chain_2(NULL)
,gate_blood(NULL)
,zjjReloadLight(NULL)
,zjjReloadSpark(NULL)
,yjReloadLight(NULL)
,yjReloadSpark(NULL)
,yjAnim(NULL)
,yjAnimFrame(NULL)
,skillHead(NULL)
,skillHead1(NULL)
,skillHead2(NULL)
,skillName(NULL)
,skillName1(NULL)
,skillName2(NULL),
gate_fire(NULL),
gate_state(NULL),
zjjText(NULL),
zjjTextRock(NULL),
zjjProg(NULL),
yjText(NULL),
yjTextRock(NULL),
yjProg(NULL),
portraitFrame(NULL),
lostBlood(0),
curChainTime(0),
preBlood(0)
,moveNum(0)
,allNum(0)
,h_isRobot(false)
,soldierNum(0)
,baseSkillRoundNum(0)
,country(0)
,soldierNumLabel(NULL)
,roundNumLabel(NULL)
,friendNumLabel(NULL)
,skillNumLabel(NULL)
,heroBloodLabel(NULL)
,nameLabel(NULL)
,headPos(CCPointZero)
,gbSprite(NULL)//光标显示
,hpProgress(NULL)
,uibg(NULL)
,zjj_btn(NULL)
,yj_btn(NULL)
,xdl_btn(NULL)
,yb_btn(NULL)
,_skName(NULL)
,_skName1(NULL)
,_skName2(NULL)
,_skLevel(0)
,_skLevel1(0)
,_skLevel2(0)
,wuzjj(NULL)
,heyJude(0)
,isChainStarted(false)
,isme(false)
,lordSkillId(0)
,roundNum (0)
,initRoundNum(0)
,m_skills_Bullet(NULL)
,m_skills_Bullet1(NULL)
,m_skills_Bullet2(NULL)
,m_sanCount(0)
,abandonRoundCountryABRate1(0.3)
,abandonRoundCountryABRate2(0.05)
,autoBattleBtn(NULL)
,autoBattleTip(NULL)
,baseAutoBtnPos(CCPointZero)
,isQuickPress(false)
,autoBattleHighLevel(0)
,autoBattle1SpeedLevel(0)
,autoBattle3SpeedLevel(0)
,autoMenu(NULL)
,dazhao(NULL)
,isEnemySkillReady(NULL)
{
    headEffArray = CCArray::create();
    headEffArray->retain();
    heyJude = true;
    effZjjBright = NULL;
    effJiNeng = NULL;
    
}
SGHeroLayer::~SGHeroLayer()
{
    //统一释放对应retain过的武将流光头像的指针
    for (int i = 0; i < skillHeadVec.size(); i++)
    {
        static_cast<CCSprite *>(skillHeadVec.at(i))->release();
    }
    skillHeadVec.clear();

    CC_SAFE_DELETE(buffs);
    CC_SAFE_DELETE(sbs);
    
    CC_SAFE_DELETE(headEffArray);
    CC_SAFE_RELEASE(m_skillName);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_herolayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND);
}

SGHeroLayer * SGHeroLayer::create(HeroObj *heroObj, cocos2d::CCArray *sbs, bool isme)
{
    SGHeroLayer *herolayer = new SGHeroLayer();
    if (herolayer && herolayer->init(heroObj, sbs, isme))
    {
        herolayer->autorelease();
        return herolayer;
    }
    CC_SAFE_DELETE(herolayer);
    return NULL;
}




bool SGHeroLayer::init(HeroObj *heroObj, cocos2d::CCArray *_sbs, bool me_)
{
    if (!SGBaseLayer::init(NULL, sg_herolayer))
    {
        return false;
    }
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND, this, callfuncO_selector(SGHeroLayer::cooperationFillResponse));
    
    ResourceManager* rm = ResourceManager::sharedInstance();
    
    rm->bindTexture("sgherolayer/sgherolayer.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    
    rm->bindTexture(PLIST_FRAME_BLINK, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_KEY_CHAIN, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_KEY_SPARK, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_KEY_TANKFUL, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    
    rm->bindTexture(PLIST_PLANT1, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_PLANT2, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_PLANT3, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_WALL_BLINK, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_WALL_CHAIN, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_WALL_FIRE, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_YJKFD, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture(PLIST_ZJJKFD, RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture("zhendonghua/xuliqiu.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    rm->bindTexture("tran/tran.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    
    
	
	//如上面所写 android下会出空指针
	this->sbs = CCArray::create();
	this->sbs->retain();
	
	CCObject *obj = NULL;
	CCARRAY_FOREACH(_sbs, obj)
	{
		this->sbs->addObject(obj);
	}
    _sbs->release();
   
    roleId = heroObj->getRoleId();
    isme = me_;
    roundNum =  heroObj->getActionCount1();
    initRoundNum = heroObj->getActionCount();
    soldierNum = 0;
    m_skillsId = heroObj->getSkillId();
    clientType = heroObj->getClientType();
    m_jianId = heroObj->getAtkmode();   //远程攻击的特性
    country = heroObj->getCountry();
    max_soldierNum = heroObj->getMax_soldierNum();
    friendHeadId = heroObj->getFriendHeadId();
    friendLevel = heroObj->getFriendLevel();

    h_morale = heroObj->getHp();
    dangqianxie = h_morale;
    heroBloodMax = heroObj->getBaseHp();
    friendNum = heroObj->get_friendRound();
    initFriendNum = heroObj->getFriendRound();
    itemId = heroObj->getItemId();
    lordSkillId = heroObj->getLordSkillID();
    _initRoundTime = heroObj->getActionTime();
    if (_initRoundTime == 0)
        _initRoundTime = -1;
    _roundTime = _initRoundTime;
    
    if (m_skillsId > 0)
    {
        SGSkillDataModel *skdata = SGStaticDataManager::shareStatic()->getLordSkillById(lordSkillId);
        if (skdata)
        {
            m_skillLevel = skdata->getLevel();
            setSkillName(skdata->getSkillName());
        }
        else
        {
            setSkillName(CCString::createWithFormat("%s", str_Skill_name));
        }
    }
    
    buffs = CCArray::create();
    buffs->retain();
    
    if (SGAIManager::shareManager()->isPVE)
    {
        isInitPve = true;
    }
    else
    {
        isInitPve = false;
    }
    
    if (!this->isme)
    {
        //状态条， 当前选中行
        statusBarLayer = CCLayerColor::create(ccc4(255, 255, 255, 125), gridWidth, screenheight-95);
        this->addChild(statusBarLayer);
        statusBarLayer->setVisible(false);
        

        //蓝色光标，指示当前移动位置
        gbSprite=CCSprite::createWithSpriteFrameName("battle_1_green_rect.png");
        this->addChild(gbSprite);
        gbSprite->setVisible(false);
    }
    
    if (getCountry() == 2)
    {
        CCString *strcoun = CCString::createWithFormat("animationFile/zldj_%d_1.plist",getCountry());
        rm->bindTexture(strcoun->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
        strcoun = CCString::createWithFormat("animationFile/zldj_%d_2.plist",getCountry());
        rm->bindTexture(strcoun->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    }
    else
    {
        CCString *strcoun = CCString::createWithFormat("animationFile/zldj_%d.plist",getCountry());
        rm->bindTexture(strcoun->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    }
    
    rm->bindTexture("animationFile/wujiangGj.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);  //主

    //城墙背景-主体
    CCSprite*  batBg = CCSprite::createWithSpriteFrameName("lifebg.png");
    batBg->setScaleX(SGLayout::getSkewing(320)/batBg->getContentSize().width);
    batBg->setScaleY(isme ? 1: -1);
    if (isme)
    {
        batBg->setAnchorPoint(ccp(0, 0));
        batBg->setPosition(ccp(0, 0));
    }
    else
    {
        batBg->setAnchorPoint(ccp(0, 1));
        batBg->setPosition(ccp(0, 1136 - 60));
    }
    this->addChild(batBg, -2);
    
    //设置长城基准位置
    CCPoint BatPos = isme ? ccpAdd(SGLayout::getPoint(kBottomCenter),ccp(0, batBg->getContentSize().height*.5)) : ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0, -batBg->getContentSize().height*.5));
    
    
    CCSprite*  headbg = CCSprite::createWithSpriteFrameName("headbg.png");
    if (isme)
    {
        headbg->setAnchorPoint(ccp(0, 0));
        headbg->setPosition(ccp(0, 0));
        
    }
    else
    {
        headbg->setAnchorPoint(ccp(0, 0));
        headbg->setPosition(ccp(640, 1136));
        headbg->setScale(-1);
    }
    this->addChild(headbg, 5);
    
    
    
    //血条背景
    CCSprite* hpBg = CCSprite::createWithSpriteFrameName("lifekuang.png");
    hpBg->setAnchorPoint(ccp(0.5, 0.5));
    hpBg->setPosition(isme ? ccp(320, 30) : ccp(320, 1136 - 30));
    this->addChild(hpBg, -1);
    
    //血条-进度条
    m_gbs = GBS_GOOD; //城门状态初始完好
    CCSprite *progressS = CCSprite::createWithSpriteFrameName("life.png");
    hpProgress= CCProgressTimer::create(progressS);
    hpProgress->setType(kCCProgressTimerTypeBar);
    hpProgress->setMidpoint(ccp(0, 0));
    hpProgress->setBarChangeRate(ccp(1,0));
    hpProgress->setPosition(ccp(hpBg->getContentSize().width * 0.5f, hpBg->getContentSize().height * 0.5f));
    hpProgress->setPercentage(0);
    hpBg->addChild(hpProgress, 4);

    //城墙背景-主体
    uibg = CCSprite::createWithSpriteFrameName("greatWall.png");
    uibg->setScaleX(SGLayout::getSkewing(320)/uibg->getContentSize().width);
    this->addChild(uibg, -2);
    
    uibg->setVisible(false);
    
    //设置长城基准位置
    motherPos = isme ? ccpAdd(SGLayout::getPoint(kBottomCenter),ccp(0, uibg->getContentSize().height*.5 - 8)) : ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0, -uibg->getContentSize().height*.5));
    
    //关于背景主体的尺寸变量
    float uibgWidth = uibg->getContentSize().width;
    float uibgHeight = uibg->getContentSize().height;
    float uibgHalfWidth = uibgWidth * 0.5f;
    float uibgHalfHeight = uibgHeight * 0.5f;

    //头像&名字底框
    float pttOffsetXisMe = 31;
    float pttOffsetXnotMe = 610;
    float pttOffsetY = 65;
    portraitFrame = CCSprite::createWithSpriteFrameName("portraitBtm.png");
    CCPoint pttPos = isme ? ccp(pttOffsetXisMe, pttOffsetY) : ccp(pttOffsetXnotMe, pttOffsetY);
    portraitFrame->setPosition(pttPos);
    uibg->addChild(portraitFrame, 4);
    
    //角色头像，相对头名边框
    float headOffsetX = 31;
    float headOffsetY = 54;
    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
    SGMainManager::shareMain() -> addHeadIconPlistByNum(data->getIconId(),sg_herolayer);
    //GPCCLOG("上场主将id:%d\n",itemId);
    



    float headSize = 58;

   // head->setScale( headSize / head->getContentSize().height);
   // headPos = ccp(headOffsetX, headOffsetY);
   // head->setPosition(headPos);
   // portraitFrame->addChild(head, 3, HEAD_TAG);
    //CCSprite *stencilA = CCSprite::createWithSpriteFrameName("mengban.png");
    
    //切图蒙版用Png
    CCTextureCache::sharedTextureCache()->setLoadingMode((LIM_OPTION)LIM_PNG_AS_PNG);
    CCSprite *stencilA = CCSprite::create("headMask.png");
    CCTextureCache::sharedTextureCache()->setLoadingMode((LIM_OPTION)CGP_PLATFORM);
    
    
    CCClippingNode *clipA = CCClippingNode::create();
    clipA->setStencil(stencilA);
    if (isme)
    {
        //clipA->setAnchorPoint(ccp(0, 0));
        clipA->setPosition(ccp(53, 53));
    }
    else
    {
        //clipA->setAnchorPoint(ccp(1, 1));
        clipA->setScale(-1);
        clipA->setPosition(ccp(640 -53, 1136 -53));
    }
    
    char spriteName[256] = {0};
    
    sprintf(spriteName, "head%d.png",data->getIconId());//12.06
    CCSprite *head = CCSprite::createWithSpriteFrameName(spriteName);
    head->setScale(1.15);
    clipA->addChild(head);
    if (!isme)
    {
        head->setScaleY(-1.15);
    }
//    else
//    {
//        //head->setPosition(ccp(53, 53));
//    }
    clipA->setAlphaThreshold(GLfloat(0.1f));
    this->addChild(clipA, 6);
    

    
    
    
    
    //头像边框，相对头名边框
    if (country <= 0 || country > 4)
    {
        country = 1;
    }
    
    int currStarLevel = 1;
    if (data)
    {
        int level = data->getOfficerCurrStarLevel();
        
        if (level > 2)
        {
            currStarLevel = level - 1;
        }
        
    }
    CCSprite *kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",currStarLevel)->getCString());
    float kuangSize = 54;
    kuang->setScale( kuangSize / kuang->getContentSize().height * 1.2);
    CCPoint kuangPos = headPos;
    kuang->setPosition(kuangPos);
    portraitFrame->addChild(kuang, 4);
    
    if (data->getAdvNum() > 0)
    {
        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",data->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advNum->setInsideColor(ccGREEN);
        advNum->setAnchorPoint(ccp(1,1));
        advNum->ignoreAnchorPointForPosition(false);
        advNum->setPosition(ccp(140,140));
        kuang->addChild(advNum,40);
    }
    
    
    //flen=换算字符串长度，fsize=文字尺寸，foffy=文字偏移
    
    
    unsigned int flength = heroObj->getName()->length();
    unsigned int flen = flength / 3 + flength % 3;
    int fsize = 24;
    int foffy = -40;
    float fscaley = 1;
    if (flen <= 2)
        fsize = 24;
    else if (flen == 3)
        fsize = 20;
    else if (flen == 4)
    {
        fsize = 15;
        fscaley = (float) 22 / fsize;
        foffy = -43;
    }
    else if (flen == 5)
    {
        fsize = 18;
        fscaley = (float) 22 / fsize;
        foffy = -44;
    }else
    {
        fsize = 14;
        fscaley = (float) 22 / fsize;
        foffy = -44;
    }
    
    heroName = std::string(data->getOfficerName()->getCString());
    nameLabel = SGCCLabelTTF::create(data->getOfficerName()->getCString(), FONT_PENGYOU, fsize);
    CCPoint namePos = ccpAdd(headPos, ccp(-24, foffy));
    nameLabel->setScale(fscaley * 0.5);  //cgp_BattleUIName
    nameLabel->setPosition(namePos);
    portraitFrame->addChild(nameLabel, 5);
    
    baseSkillRoundNum = heroObj->getSkillRoundMax();
    
    //当前血量数字
    float heroBloodOffsetY = -30;
    CCString* curLabel = CCString::createWithFormat("%d",h_morale);
    heroBloodLabel = SGCCLabelTTF::create(curLabel->getCString(), FONT_PENGYOU, FONTSIZE(10) , ccBLACK , ccWHITE);

    heroBloodLabel->setAnchorPoint(ccp(0.5, 0.5));

    if (isme)
    {
        heroBloodLabel->setPosition(ccpAdd(motherPos, ccp( 0, uibgHalfHeight/4 + heroBloodOffsetY )));
    }
    else
    {
        heroBloodLabel->setPosition(ccpAdd(motherPos, ccp( 0, uibgHalfHeight/4 + heroBloodOffsetY + 40)));
    }
    
    addChild(heroBloodLabel, 100);
    //血条背景
    CCSprite* hpBkg = CCSprite::createWithSpriteFrameName("myBloodFrame.png");
    CCPoint hpBkgPos = ccp(uibg->getContentSize().width * 0.5f, uibg->getContentSize().height * 0.5f - 36);
    uibg->addChild(hpBkg, 3);
    hpBkg->setPosition(hpBkgPos);
    

    
    //预制四大按钮和铁链的位置
    float factor = screenwidth / 640; //需要适应
    float btnOffsetX = 49;
    float btnOffsetY = -10;

    float btnWidth = 104;

    
    //四大按钮位置（由于控件原因，此位置为相对this）
    xdlBtnPos = ccpAdd(motherPos, ccp( factor * (-btnOffsetX - btnWidth * 0.5) , btnOffsetY));
    zjjBtnPos = ccpAdd(xdlBtnPos, ccp(factor * (-btnWidth - 2), 0));
    yjBtnPos = ccpAdd(motherPos, ccp( factor * (btnOffsetX + btnWidth * 0.5) , btnOffsetY));
    //ybBtnPos = ccpAdd(yjBtnPos, ccp( factor * (btnWidth + 2), 0));

    //按钮底图，暂时采用单图，否则有显示BUG。已修复。
 //   const char* btnBtmPath = "btnSlotBtm.png";
    const float threeOffxFix = -10;
    const float twoOffxFix = -10;
//    const float threeFontxFix = 27;
    const float twoFontxFix = 15;
 
    imgMove = CCSprite::createWithSpriteFrameName("move.png");
    
    CCPoint imgMovePos;
    if (isme)
    {
        imgMovePos = ccp(160, 64);
    }
    else
    {
        imgMove->setScaleX(-1);
        imgMovePos = ccp(640 - 160, 1136 - 60);
    }
    CCMenu*  menu = this->getMenu();
    imgMove->setPosition(imgMovePos);
    menu->addChild(imgMove);
    imgMove->setZOrder(2);

    //主将计回合数文本
    ccColor3B color = ccc3(0 , 255 , 204);

    
    
    //我方行动力  改为投降
    xdl_btn = SGButton::create("touxiangTouch.png","NULL",this,menu_selector(SGHeroLayer::surrenderRound),ccp(threeOffxFix, 0),false,true);
    xdl_btn->setImage("touxiang.png");
    xdl_btn->setScaleX(-1);
    if (isme)
    {
        
        xdl_btn->setPosition(260,  62);
    }
    else
    {
        xdl_btn->setPosition(1000, 1600);
    }
    
    
    this->addBtn(xdl_btn);
    xdl_btn->setZOrder(1);
   
    //行动力-文本
    CCString *roundNums = CCString::createWithFormat("%d",roundNum);
    roundNumLabel = SGCCLabelTTF::create(roundNums->getCString(), FONT_PANGWA, FONTSIZE(12) , ccc3(255 , 128 , 0));
    
//    roundNumAtlas = CCLabelAtlas::create("0123456789", "sgherolayer/huihenum.png", 33, 30, '0');
//    imgMove->addChild(roundNumAtlas, 5);

    imgMove->addChild(roundNumLabel, 4);
    if (isme)
    {
        roundNumLabel->setPosition(ccp(105, 20));
    }
    else
    {
        roundNumLabel->setScaleX(-1);
        roundNumLabel->setPosition(ccp(-70, 20));
    }
    
    
    
    //我方主将计，没有文本
    zjj_btn = SGButton::create("energy_a.png" , NULL,this,menu_selector(SGHeroLayer::mainSkillHandler),ccp(threeOffxFix, 0),false,true);
    if (isme)
    {
        zjj_btn->setAnchorPoint(ccp(0, 0));
        zjjBtnPos = ccp(70, -5);
    }
    else
    {
        zjj_btn->setScale(-1);
        zjjBtnPos = ccp(520, 1092);
    }
    
    
    
    zjj_btn->setPosition(zjjBtnPos);
    this->addBtn(zjj_btn);
    zjj_btn->setZOrder(3);
    zjj_btn->setEnabled(false);
    
    CCSkeletonAnimation*  zjjLight = CCSkeletonAnimation::createWithFile("effSpine/gaoguang.json", "effSpine/gaoguang.atlas");
    zjjLight->setAnimation("animation", true, 0);
    zjjLight->setPosition(ccp(53, 55));
    zjj_btn->addChild(zjjLight, 5);
    
    


    //主将计进度
    CCSprite* zjjGreen = CCSprite::createWithSpriteFrameName("energy_b.png");
    zjjProg = CCProgressTimer::create(zjjGreen);
    zjjProg->setType(kCCProgressTimerTypeBar);
    if (isme)
    {
        zjjProg->setMidpoint(ccp(0, 0));
        zjjProg->setBarChangeRate(ccp(0 , 1));
    }
    else
    {
        zjjProg->setMidpoint(ccp(0, 1));
        zjjProg->setBarChangeRate(ccp(0, 1));
    }
    

    
    zjjProg->setPosition(ccp( zjj_btn->getContentSize().width * 0.5, zjj_btn->getContentSize().height * 0.5 ));
    zjjProg->setPercentage(0);
    zjj_btn->addChild(zjjProg, 3);



    //我方援兵
    yb_btn = SGButton::create("suportTouch.png","NULL",this,menu_selector(SGHeroLayer::fillUp),ccp(twoOffxFix, 0),false,true);
    yb_btn->setImage("support.png");
    
    CCSprite*  supportMen = CCSprite::createWithSpriteFrameName("supportmen.png");



    
    if (isme)
    {
        yb_btn->setScaleX(-1);
        yb_btn->setAnchorPoint(ccp(0, 0));
        yb_btn->setPosition(ccp(640, 0));
        supportMen->setAnchorPoint(ccp(1, 0));
        supportMen->setPosition(ccp(640, 36));
    }
    else
    {
        yb_btn->setAnchorPoint(ccp(0, 0));
        //yb_btn->setScaleX(-1);
        yb_btn->setScaleY(-1);
        yb_btn->setPosition(0, 1136);
        supportMen->setAnchorPoint(ccp(0, 1));
        supportMen->setPosition(ccp(0, 1098));
    }
    this->addBtn(yb_btn);
    this->addChild(supportMen, 6);
    
    //援兵-文本
    char soldier[10] = {0};
    
    sprintf(soldier, "%d",soldierNum);
    soldierNumLabel = SGCCLabelTTF::create(soldier, FONT_PANGWA, FONTSIZE(16) , color);

    
    yb_btn->addChild(soldierNumLabel, 4);
    if (isme)
    {
        soldierNumLabel->setPosition(ccp(-20 ,20));
        soldierNumLabel->setScaleX(-1);
    }
    else
    {
        soldierNumLabel->setPosition(ccp(40, 20));
        soldierNumLabel->setScaleY(-1);;
    }
    
    
    
    
    //我方友军，没有文本  现在改为结束回合
    yj_btn = SGButton::create("end.png",NULL,this,menu_selector(SGHeroLayer::abandonRound),ccp(twoOffxFix, 0),false,true);

    if (isme)
    {
        
        yj_btn->setPosition(640 - yb_btn->getContentSize().width - yj_btn->getContentSize().width * 0.5 - 30,  yj_btn->getContentSize().height * 0.5);
    }
    else
    {
        yj_btn->setPosition(1000, 2000);
    }
    
    
    
    this->addBtn(yj_btn);
    //友军进度

    //友军回合数文字特殊处理
    if (isInitPve)
    {
        friendNumLabel = SGCCLabelTTF::create(CCString::createWithFormat("%d",friendNum)->getCString(), FONT_PANGWA, FONTSIZE(10), color);
        friendNumLabel->setPosition(ccp(yj_btn->getContentSize().width * 0.5f + twoFontxFix+5, yj_btn->getContentSize().height * 0.5f));

        friendNumLabel->setAnchorPoint(ccp(0, 0.5));
        friendNumLabel->setVisible(false);
        yj_btn->addChild(friendNumLabel, 8);
    }
    else
    {
        CCLabelTTF *label = CCLabelTTF::create("N", FONT_PANGWA, FONTSIZE(12));
        label->setPosition(ccp(yj_btn->getContentSize().width * 0.5f + twoFontxFix, yj_btn->getContentSize().height * 0.5f));
        label->setAnchorPoint(ccp(0, 0.5));
        yj_btn->addChild(label, 8);
    }
    

    
    



    
    
    if (isme && !isInitPve)
    {
        //倒计时
        m_RoundTimeT = CCSprite::createWithSpriteFrameName("roundTime_T.png");
        m_RoundTimeT->setScaleX(screenwidth / m_RoundTimeT->getContentSize().width);
        this->addChild(m_RoundTimeT, 4);
        m_RoundTimeT->setVisible(false);
        m_RoundTimeT->setPosition(ccp( screenwidth * 0.5, screenheight * 0.5));
        CCString *time = CCString::createWithFormat("%d",_roundTime);
        
        //天梯赛中的每回合倒计时标签
        roundTimeLabel = CCLabelAtlas::create(time->getCString(), "sanguobigpic/round_Time_Num.png", 21, 20, '0');
        this->addChild(roundTimeLabel, 4);
        tbj = CCSprite::createWithSpriteFrameName("round_Time_BJ.png");
        this->addChild(tbj, 4);
        tbj->setPosition(ccp( screenwidth * 0.5, screenheight * 0.5 + tbj->getContentSize().height * 0.5 + m_RoundTimeT->getContentSize().height * 0.5));

        roundTimeLabel->setPosition(tbj->getPosition());
        roundTimeLabel->setAnchorPoint(ccp(0.5,0.5));
        tbj->setVisible(false);
        roundTimeLabel->setVisible(false);
    }
    
    //花花草草
    //城墙左侧植物
    CCPoint grass1Pos = ccp(218, 98);
    CCSpriterX* grass1 = CCSpriterX::create(ANIM_PLANT1, true);
    grass1->setanimaID(0);
    grass1->setisloop(true);
    grass1->setVisible(true);
    grass1->setPosition(grass1Pos);
    uibg->addChild(grass1, 6);
    //城墙右侧植物
    CCPoint grass2Pos = ccp(561, 79);
    CCSpriterX* grass2 = CCSpriterX::create(ANIM_PLANT2, true);
    grass2->setanimaID(0);
    grass2->setisloop(true);
    grass2->setVisible(true);
    grass2->setPosition(grass2Pos);
    uibg->addChild(grass2, 6);
    if (!isme)
    {
        //城墙下方植物左侧（敌军有）
        CCPoint grass3Pos = ccp(22, -5);
        CCSpriterX* grass3 = CCSpriterX::create(ANIM_PLANT2, true);
        grass3->setanimaID(0);
        grass3->setisloop(true);
        grass3->setVisible(true);
        grass3->setPosition(grass3Pos);
        uibg->addChild(grass3, 6);
        //城墙下方植物右侧（敌军有）
        CCPoint grass4Pos = ccp(575, -7);
        CCSpriterX* grass4 = CCSpriterX::create(ANIM_PLANT3, true);
        grass4->setanimaID(0);
        grass4->setisloop(true);
        grass4->setVisible(true);
        grass4->setPosition(grass4Pos);
        uibg->addChild(grass4, 6);
    }
    
    if (isme)
    {
        
    }
    else
    {
        //把敌方的四个按钮设为不可用
        zjj_btn->setEnabled(false);
        yj_btn->setEnabled(false);
        xdl_btn->setEnabled(false);
        yb_btn->setEnabled(false);
    }
    uibgY = uibg->getContentSize().height * 0.8;
    uibg->setPosition(motherPos);
    
    wuzjj = SGCCLabelTTF::create("", FONT_PANGWA, FONTSIZE(10));
    zjj_btn->addChild(wuzjj);
    wuzjj->setPosition(ccp(zjj_btn->getContentSize().width/2,zjj_btn->getContentSize().height/2));
    wuzjj->setVisible((m_skillsId <= 0));
    wuzjj->setVisible(false);
    

    setSkillsRoundNum(heroObj->getSkillRoundCur(), false);
    setBloodNum(h_morale);

    this->changBloodNum(0);
    this->changeFriendNum(0);

    //zjjText->setVisible((m_skillsId > 0) && true);
   // skillNumLabel->setVisible((m_skillsId > 0) && true);
    
    
    if (m_skillsId > 0 && skillsRoundNum == 0)
    {
        //主将计启动特效
        changeButtonState(true, 1);
    }
    
    int rateValue = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("15"))->objectForKey("value"))->intValue();
    abandonRoundCountryABRate1 = rateValue/ 100.0;
    
    rateValue = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("16"))->objectForKey("value"))->intValue();
    abandonRoundCountryABRate2 = rateValue/ 100.0;
    
    
    //controlvalue 中第36 37 38，对应开启自动战斗等级，一倍等级，三位等级
    
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("36");
    //开启自动战斗等级
   // autoBattleHighLevel = dic1->valueForKey("value")->intValue();
    
    //开启一倍速度等级
    dic1 = (CCDictionary *) dic->objectForKey("37");
    autoBattle1SpeedLevel = dic1->valueForKey("value")->intValue();
    //开启三倍速度等级
    dic1 = (CCDictionary *) dic->objectForKey("38");
    autoBattle3SpeedLevel = dic1->valueForKey("value")->intValue();
    
    autoBattleHighLevel = 1;
    autoBattle1SpeedLevel = 1;
    autoBattle3SpeedLevel = 1;
    
    //玩家等级
    int playerLvl = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    
    //防止出现按钮优先级问题，这里直接用menu来放button，方便后续的按钮优先级处理
    autoMenu = CCMenu::create();
    
    this->addChild(autoMenu, 1);
    autoMenu->setPosition(CCPointZero);
    

    
    //自动战斗按钮 (只有闯关和限时副本中可以用 且 新手引导中不可用，小于controlvalue 36行中值不开启),并且这一关卡至少一星（对副本不起作用）
//    if ((isme && SGBattleManager::sharedBattle()->getBattleType() == BT_ADVENTURE
//        && SGGuideManager::shareManager()->getCurrentGuideId() > 52
//        && playerLvl > autoBattleHighLevel && this->displayAutoBattleBtnWithRank())||(isme && SGBattleManager::sharedBattle()->getBattleType() == BT_ARENA))
    if ((isme && SGBattleManager::sharedBattle()->getBattleType() == BT_ADVENTURE))
    {
        //先用大图初始化按钮，保证触摸区域是正确的
        autoBattleBtn = SGButton::create("autoTouch.png",
                                         NULL, this,menu_selector(SGHeroLayer::pressBtnDelay),CCPointZero,false,true);
        //基础按钮位置，之后根据此进行偏移适配处理
        baseAutoBtnPos = ccpAdd(ccp(CCDirector::sharedDirector()->getWinSize().width - 142, 0 - 55),
                                ccp(-autoBattleBtn->getContentSize().width / 2 + 10, autoBattleBtn->getContentSize().height / 2 + 100));
        
        autoBattleBtn->setAnchorPoint(ccp(0, 0.5));
        autoMenu->addChild(autoBattleBtn);
        autoBattleBtn->setPosition(baseAutoBtnPos);
        
        //这里考虑到触摸区域问题，先用大的图初始化。再用真正图替换。触摸区域不会出问题
        autoBattleBtn->setImage("auto.png");

        //预加载自动战斗提示动画, 不显示
        autoBattleTip = CCSprite::createWithSpriteFrameName("auto_battle_now.png");
        this->addChild(autoBattleTip, 99);
        autoBattleTip->setPosition(ccpAdd(ccp(this->getContentSize().width / 2, this->getContentSize().height / 3), ccp(0, 0)));
        autoBattleTip->setVisible(false);
    }
    if(isme && SGBattleManager::sharedBattle()->getBattleType() == BT_ARENA)
    {
        setBtnEnable(false);
        autoBattleBtn->setEnabled(false);
    }
    //新手引导下未出现vs界面前这里不可点击
    if (SGGuideManager::shareManager()->isGuide)
    {
        setBtnEnable(false);
    }

    return true;

}
void SGHeroLayer::onClickSkipArenaFightButton()
{
    
        main::ArenaEndFightRequest  * req = new main::ArenaEndFightRequest();
        req->set_quick(true);
        SGSocketClient::sharedSocketClient()->send( MSG_ARENA_END_GIGHT, req);
   

}
//根据星级和（是不是副本）决定是否显示自动战斗
bool SGHeroLayer::displayAutoBattleBtnWithRank()
{
    
    SGPlayerInfo *pInfo = SGPlayerInfo::sharePlayerInfo();
    
    //当前的关卡的plotId
    //当前的plotId小于等于已打过的最大的关卡id，这关一定打过，可以开自动战斗
    int plotId = pInfo->getPlotId();
    int maxPlotId = pInfo->getMaxPlotId();
    
    //星级至少为1（即打过关一次），或者是活动副本，都显示自动战斗
    if (plotId >= 20000 && plotId < 30000)//精英副本
    {
        return (plotId <= pInfo->getMaxBossPlotId());
    }
    //如果是幻境，活动，或者普通闯关打过
    return ((plotId <= maxPlotId) || (plotId > 10000 && plotId < 20000) || (plotId >= 30000));
    
}



#pragma mark =========================自动战斗相关逻辑_Begin=========================
//外围设置menu是否可触
void SGHeroLayer::setAutoBattleBtnCanTouch(bool canTouch)
{
    if (autoMenu && autoBattleBtn)
    {
        autoMenu->setEnabled(canTouch);
        autoBattleBtn->setEnabled(canTouch);
    }
}

//设置我方四大按钮是否可触
void SGHeroLayer::setBtnEnable(bool canTouch)
{
    //我方下方的四个按钮都是否触摸
    zjj_btn->setEnabled(canTouch);
    yj_btn->setEnabled(canTouch);
    xdl_btn->setEnabled(canTouch);   //modified by cgp
    yb_btn->setEnabled(canTouch);
    
}



//开启自动战斗之后，我方中间显示呼吸动画
void SGHeroLayer::displayAutoBattleAnimation()
{
    if (autoBattleTip)
    {
        int speed = SGAIManager::shareManager()->getAnimationSpeed();
        autoBattleTip->setVisible(true);
        //执行呼吸动画效果
        autoBattleTip->setOpacity(255);
        CCActionInterval *breath1 = CCFadeOut::create(BREATH_CHANGE_TIME * speed);
        CCActionInterval *breath2 = breath1->reverse();
        //全透与全实之间有延时
        CCSequence *breathSeq = CCSequence::create(breath1, CCDelayTime::create(BREATH_STAY_TIME * speed),
                                                   breath2, CCDelayTime::create(BREATH_STAY_TIME * speed), NULL);
        //重复执行
        CCRepeatForever *forever = CCRepeatForever::create(breathSeq);
        
        autoBattleTip->runAction(forever);
    }
}
//由自动战斗转为手动操作的时候，呼吸动画消息
void SGHeroLayer::removeAutoBattleTips()
{
    //设置动画不可见并停止吸吸的动作效果
    if (autoBattleTip)
    {
        autoBattleTip->setVisible(false);
        autoBattleTip->stopAllActions();
    }
}


//不允许自动战斗的节
bool SGHeroLayer::notBeCanAutoBattlePlot()
{
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("51");
    
    int curPlotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    
    if (dic1)
    {
        CCArray *plotIds = split(((CCString *)(dic1->valueForKey("value")))->getCString(), ",");
        if (plotIds)
        {
            for (int i = 0; i < plotIds->count(); i++)
            {
                if (curPlotId == ((CCString *)plotIds->objectAtIndex(i))->intValue())
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}


//点击按钮延时
void SGHeroLayer::pressBtnDelay(CCObject *obj)
{
    //如果有点中状态，刚不进行自动战斗
    if (!battleMap || battleMap->m_touchState != eTouchNull
        || SGGuideManager::shareManager()->isGuide || notBeCanAutoBattlePlot())
    {
        return;
    }
    
    if (!SGBattleManager::sharedBattle()->getCanDoBattleOperation())
    {
        return;
    }
    
    
    //另一种方式来防止穿透, 如果战场上有对应的窗口打开，则CanTouchAutoBattleBtn为false，即不可触，反之可触
    if (!SGBattleManager::sharedBattle()->getCanTouchAutoBattleBtn())
    {
        return ;
    }
    
    /**
     *  isQuickPress 为true的时候已经是快速点击状态，这时对自动战斗按钮不响应，直接到1秒延时过后isQuickPress为false，按钮正常触摸
     */
    if (!isQuickPress)
    {
        CCCallFuncN *callDelay = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::setIsQuickPress));
        CCCallFuncN *callAuto = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::autoBattleWithAI));
        this->runAction(CCSequence::create(callAuto, CCDelayTime::create(1.0f), callDelay, NULL));
    }
}

//设置isQuickPress作回调用
void SGHeroLayer::setIsQuickPress()
{
    this->isQuickPress = false;
}


//自动战斗按钮回调
void SGHeroLayer::autoBattleWithAI()
{
    //开始执行自动逻辑，标志修改，防止下次快速点击
    this->isQuickPress = true;
    
    SGAIManager *aiMgr = SGAIManager::shareManager();
    
    //每次点击的时候检测AIManager里的自动战斗标记
    
    //未开启自动战斗，（点击按钮后就是开启）
    if (!aiMgr->getIsOpenAutoBattle())
    {
        //设置动画速度
        aiMgr->setAnimationSpeed(3);   //modiefied by cgp , before is 1;
        aiMgr->setIsOpenAutoBattle(true);
    }
    else//自动战斗目前在开启状态
    {
        //如果是一倍速度
        if (aiMgr->getAnimationSpeed() == ANIMATION_1SPEED && aiMgr->getIsOpenAutoBattle())
        {
            //继续检测当前玩家等级, 如果玩家小于9级，并且当前在进行自动战斗，再次点击直接停止自动战斗
            if (SGPlayerInfo::sharePlayerInfo()->getPlayerLevel() < autoBattle3SpeedLevel
                && SGPlayerInfo::sharePlayerInfo()->getPlayerLevel() >= autoBattle1SpeedLevel)
            {
                aiMgr->setIsOpenAutoBattle(false);
                aiMgr->setAnimationSpeed(ANIMATION_1SPEED);
                SG_SHOW_WINDOW(CCString::createWithFormat(str_AutoBattleTips1, autoBattle3SpeedLevel)->getCString());//提示XX级以上开启三倍加速
            }
            else//开始三倍速度
            {
                aiMgr->setAnimationSpeed(ANIMATION_3SPEED);
            }
        }
        //如果已经是三倍速度,点击立刻关闭自动战斗
        else if (aiMgr->getAnimationSpeed() == ANIMATION_3SPEED)
        {
            aiMgr->setAnimationSpeed(ANIMATION_1SPEED);
            aiMgr->setIsOpenAutoBattle(false);
        }
    }
    
    CCLOG("SGAIManager::shareManager()->getIsOpenAutoBattle() || %d",
          aiMgr->getIsOpenAutoBattle());
    
    CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
    
    //检测是否是我方回合并且是开启自动战斗模式
    if (aiMgr->getIsOpenAutoBattle())
    {
        CCLOG("this->getBattleMap()->autoBattleTimerStart() || 开始调用autoBattleTimerStart [%s -- %d]", __FUNCTION__, __LINE__);
        
        //自动战斗图标转换
//        char autoBattleTipsResName[32] = "\0";
//        sprintf(autoBattleTipsResName, "auto_battle_btn_%dspeed.png", aiMgr->getAnimationSpeed());
//        autoBattleBtn->setImage(autoBattleTipsResName);
        
        //二张图片尺寸不同，这里做偏移处理
       // autoBattleBtn->setPosition(ccpAdd(baseAutoBtnPos, ccp(-6, -5)));
        
        //移除对应的操作提示
        battleMap->hideFriendlyHelp();
        
        //先移除，再开始，因为有了三倍速度
        this->removeAutoBattleTips();
        //开始显示自动战斗提示的动画
        this->displayAutoBattleAnimation();
        
        //马上开始自动战斗
        this->getBattleMap()->autoBattleTimerStart();
        //整体动画速度增加 3 倍
        SGLogicManager::gi()->setGameSpeed(aiMgr->getAnimationSpeed());
        //设置按钮是不可用
        this->setBtnEnable(false);
    }
    else
    {
        //自动战斗图标转换
        //autoBattleBtn->setImage("auto_battle_btn_before_press.png");
        //二张图尺寸不同，这里做适配偏移处理
        //autoBattleBtn->setPosition(baseAutoBtnPos);
        //移除对应的自动战斗提示
        this->removeAutoBattleTips();
        //整体动画速度还原到正常水平
        SGLogicManager::gi()->setGameSpeed(ANIMATION_1SPEED);
        //返回手动模式的时候 按钮可触
        this->setBtnEnable(true);
    }
    
}
#pragma mark =========================自动战斗相关逻辑_End=========================

int SGHeroLayer::getItemId()
{
    return itemId;
}
void SGHeroLayer::initBattleMap()
{
    //GPCCLOG("initBattleMap初始化地图");
    //初始化地图
    battleMap = SGBattleMap::create(sbs,isme);
   
    this->addChild(battleMap, ksg_mybattlemap, sg_myBattleMapTag);
    battleMap->setHero_Obj(this);
    this->schedule(schedule_selector(SGHeroLayer::sendSB2Scene), .5);
    
    bool isround = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();

    if (!isround && !isme)
    {
        int plotID= SGPlayerInfo::sharePlayerInfo()->getPlotId();
        int delayeTime = 4.0f;    //before is 8.0, modified by cgp
        if (plotID==2)
        {
            delayeTime=2.0f;
        }
            
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(SGHeroLayer::machineAction) );
            //FIXME: 延时现在是写死的噢，不靠谱，再议。--hehe
            this->runAction(CCSequence::create(CCDelayTime::create(delayeTime),func,NULL));//等待时间调整7.3
    }
    
    if (isround &&  isme && isInitPve)
    {
        beginRoundTime();
    }
    
}

void SGHeroLayer::machineAction()
{
    this->unschedule(schedule_selector(SGHeroLayer::machineAction));
    SGAIManager::shareManager()->startRound(this->battleMap);
}


void SGHeroLayer::initSkillRoundNum()
{
    skillsRoundNum = baseSkillRoundNum;
    CCString *numStr = CCString::createWithFormat("%d",skillsRoundNum);
    //XXX: 这里重置主将计
    changeButtonState(false, 1);
    if (this->getChildByTag(XDLAN_LAYER_TAG)) {
        CCSpriterX *spx = (CCSpriterX*)this->getChildByTag(XDLAN_LAYER_TAG);
        spx->removeFromParentAndCleanup(true);
        spx = NULL;
    }
}

void SGHeroLayer::delayUseSkill()
{
    if (!battleMap || battleMap->_isRefreshMap || battleMap->m_touchState != eTouchNull)
    {
        return;
    }
    
    if (!SGBattleManager::sharedBattle()->getCanDoBattleOperation())
    {
        return;
    }
    
    bool isCanAttack = false;
    if (skillsRoundNum == 0)
    {
        isCanAttack = true;
    }
    
    SGBattleLeaderSkillBox *leader = SGBattleLeaderSkillBox::create(this, true,isCanAttack, itemId, lordSkillId,skillsRoundNum);
   // SGMainManager::shareMain()->showBox(leader);
    leader->useLeaderSkill();
    battleMap->m_nAItips_time = battleMap->m_nAItips_time + 8;
    battleMap->hideFriendlyHelp(false);
    
    
    if (SGPlayerInfo::sharePlayerInfo()->getPlotId() == 17)
    {
        SGGuideManager::shareManager()->isGuide = true;
        SGGuideManager::shareManager()->startFightGuide(7);   //before is 7
    }
    
    
}

//点击我方主将计按钮
void SGHeroLayer::mainSkillHandler()
{
    if (skillsRoundNum != 0)
    {
        return;
    }

    zjj_btn->setEnabled(false);

    EFFECT_PLAY(MUSIC_BTN);
    CCCallFunc*   call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::delayUseSkill));
    
    CCDelayTime*  delay = CCDelayTime::create(MAX_BUTTON_TIME + 2);
    this->runAction(CCSequence::create(delay, call, nullptr));
    showAttactHeadEffect(NULL);
    

}

void SGHeroLayer::mainSkillAttackHandler()
{
    
    if (m_skillsId <= 0)
    {
        return;
    }
    
    switch (clientType) {
        case 1:
        {
            if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {

                SGBattleFillUpLayer *fillUPLayer = SGBattleFillUpLayer::create(this);
                SGMainManager::shareMain()->showBox(fillUPLayer);
            }
            else
                 SGSkillManager::shareSkill()->sendSkillRequest(0);
                
        }
            break;
        case 2:
        {
            setBulletSp(3);
            
            battleMap->m_touchState = eTouchSkills;
        }
            break;
        case 3:
        {
            setBulletSp(1);
            
            battleMap->m_touchState = eTouchSkills;
        }
            break;
            
        default:
        {
            SGSkillManager::shareSkill()->sendSkillRequest(0);
        }
            break;
    }
}

//点击敌方主将计
void SGHeroLayer::enemyMainSkillHandler()
{
//    if (skillsRoundNum != 0)
//    {
//        return;
//    }
//    
//    zjj_btn->setEnabled(false);
//    
//    EFFECT_PLAY(MUSIC_BTN);
//    CCCallFunc*   call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::delayUseSkill));
//    
//    CCDelayTime*  delay = CCDelayTime::create(MAX_BUTTON_TIME + 2);
//    this->runAction(CCSequence::create(delay, call, nullptr));
    if (dazhao == NULL)
    {
        //SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
        dazhao = CCSkeletonAnimation::createWithFile("effSpine/dazhao.json", "effSpine/dazhao.atlas", 1);
        dazhao->setAnimation("animation", false);
        dazhao->setPosition(ccp(320, 640));
        this->addChild(dazhao, 10000);
        
    }
    else
    {
        dazhao->setAnimation("animation", false);
    }
    
    
    EFFECT_PLAY(MUSIC_BTN);
    SGBattleLeaderSkillBox *leader = SGBattleLeaderSkillBox::create(this, false,false, itemId, lordSkillId,0);
    SGMainManager::shareMain()->showBox(leader);
}

//主将计光效
void SGHeroLayer::setBulletSp(int num)
{

    //如果是手动触发的主将技，且是那种需要点选位置的，手动已经点选了位置，这里判定直接触发
    if (!SGAIManager::shareManager()->getIsCompleteSkill()//已经手动触发了主将持
        && SGAIManager::shareManager()->getIsOpenAutoBattle()//是自战斗触发的
        && this->getBattleMap()->getIsMe())//我方回合
    {
        if (num != 1 && num != 5)
        {
            if (SGAIManager::shareManager()->isPVE)
            {
                if ((!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() && !isme )
                    || (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() && isme
                        && SGAIManager::shareManager()->getIsOpenAutoBattle()))
                {
                    //火球或者其他主将技效果的箭之类的动画存在才直接发动
                    if (num != 7 && this->getChildByTag(SKILLS_BULLET_TAG))
                    {
                        bulletShow(NULL);
                        return ;
                    }
                }
            }
        }
    }
    
    
    
    bulletJ = 3;
    //如果开启自动战斗模式，这里与对方做相同的操作
    if((!isme && SGAIManager::shareManager()->isPVE) || (SGAIManager::shareManager()->getIsOpenAutoBattle() && isme))
    {
        SGAIManager::shareManager()->isAi = false;
        bulletJ = getRandom(1, 5);
    }
    m_jianId = 0;
    
    CCString *name = NULL;
    CCString *name1 = NULL;
    if (m_skillsId == skill_id_20001||
        m_skillsId == skill_id_20072||
        m_skillsId == skill_id_20095) {
        name = CCString::createWithFormat("animationFile/jian_%d.plist", m_jianId);
        name1 = CCString::createWithFormat("animationFile/jian_%d.scml",m_jianId);
        
    }else if(m_skillsId == skill_id_20040 ||
             m_skillsId == skill_id_20041 ||
             m_skillsId == skill_id_20042)
    {
        name = CCString::createWithFormat("animationFile/jian_%d.plist", m_jianId);
        name1 = CCString::createWithFormat("animationFile/jian_%d.scml",m_jianId);
    }else{
        name = CCString::create("animationFile/nlq.plist");
        name1 = CCString::create("animationFile/nlq_001.scml");
    }

    //FIXME: it's not deleted
    ResourceManager::sharedInstance()->bindTexture( (char*) name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
    
    //name1->getCString()   能量球
    m_skills_Bullet = CCSpriterX::create(name1->getCString(), true, true);
    m_skills_Bullet->setanimaID(0);
    m_skills_Bullet->setisloop(true);
    m_skills_Bullet->setPosition(GameConfig::getGridPoint(5, bulletJ,isme));
    m_skills_Bullet->play();
    this->addChild(m_skills_Bullet, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG);
    
    if (!isme)
    {
        //MMDEBUG: FLIPY
        m_skills_Bullet->setScaleY(-1);
//        m_skills_Bullet->setCCSpriterXFlipY(true);
    }
    if (num == 1 || num == 5) {
        float a = 0.0,b = 0.0;
        for (int i = 0; i < this->battleMap->getSoldiers()->count(); i++) {
            ShiBing *sb = (ShiBing*)this->battleMap->getSoldiers()->objectAtIndex(i);
            SGGridBase *grid = this->battleMap->myGrids[sb->getSbindex().i][sb->getSbindex().j];
            if(sb->getSbType() == ksbone &&
               grid &&
               grid->getStype() == knormal)
            {
                CCSpriterX *bianqiu = CCSpriterX::create("animationFile/nlq.scml", true, true);
                bianqiu->setanimaID(0);
                bianqiu->setisloop(false);
                bianqiu->setPosition(GameConfig::getGridPoint(grid->getIndex().i, grid->getIndex().j,isme));
                bianqiu->play();
                this->addChild(bianqiu, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG - 1);
                a = bianqiu->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;

                
                CCSpriterX *bianqiu1 = CCSpriterX::create("animationFile/nlq_000.scml", true, true);
                bianqiu1->setanimaID(0);
                bianqiu1->setisloop(true);
//                bianqiu1->setPosition(bianqiu->getSprite(0, 6)->getPosition());
                bianqiu1->setPosition(GameConfig::getGridPoint(grid->getIndex().i, grid->getIndex().j,isme));
                bianqiu1->play();
                this->addChild(bianqiu1, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG - 1);
                b = bianqiu1->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;
                float c = 1.5;
                CCHide *hide = CCHide::create();
                CCDelayTime *dal = CCDelayTime::create(a);
                CCShow *show = CCShow::create();
                CCMoveTo *move = CCMoveTo::create(c, GameConfig::getGridPoint(5, bulletJ,isme));
                CCHide *hide1 = CCHide::create();
                bianqiu1->runAction(CCSequence::create(hide, dal, show, move, hide1, NULL));
            }
        }
        CCCallFunc *cal = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::randomX));
        CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::bulletShow));
        CCHide *hide2 = CCHide::create();
        CCDelayTime *dal1 = CCDelayTime::create(a + b + 1);
        CCShow *show1 = CCShow::create();
        if (num == 1) {
             m_skills_Bullet->runAction(CCSequence::create(hide2, dal1, show1, cal, call,NULL));
        }
        else if ( num == 5)
        {
             m_skills_Bullet->runAction(CCSequence::create(hide2, dal1, show1,NULL));
        }
       
        
    }
    
    
    if (num > 1 && num != 5) {
        m_skills_Bullet1 = CCSpriterX::create(name1->getCString(), true, true);
        m_skills_Bullet1->setanimaID(0);
        m_skills_Bullet1->setisloop(true);
        m_skills_Bullet1->setPosition(GameConfig::getGridPoint(5, bulletJ-1,isme));
        m_skills_Bullet1->play();
        this->addChild(m_skills_Bullet1, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG - 1);
        
        m_skills_Bullet2 = CCSpriterX::create(name1->getCString(), true, true);
        m_skills_Bullet2->setanimaID(0);
        m_skills_Bullet2->setisloop(true);
        m_skills_Bullet2->setPosition(GameConfig::getGridPoint(5, bulletJ+1,isme));
        m_skills_Bullet2->play();
        this->addChild(m_skills_Bullet2, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + 1);

        if (!isme) {
            m_skills_Bullet1->setScaleY(-1);
            m_skills_Bullet2->setScaleY(-1);
        }
    }
    
    if (m_skillsId == skill_id_20064 ||
        m_skillsId == skill_id_20065 ) {
        CCArray *ar = CCArray::create();
        for (int i = 0; i < mapRow; i++) {
            for (int j = 0; j < mapList; j++) {
                SGGridBase *g = this->battleMap->myGrids[i][j];
                if (g &&
                    g->getStype() == knormal &&
                    g->getSbType() == ksbone) {
                    ShiBing *sb = this->battleMap->getShiBingWithGrid(g);
                    if(sb)
                        ar->addObject(sb);
                }
            }
        }
        m_sanCount = ar->count();
        this->battleMap->removeSbAr(ar);
    }
    
    
    if (num != 1 && num != 5) {
        if (SGAIManager::shareManager()->isPVE) {
            if ((!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() && !isme )
                || (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() && isme && SGAIManager::shareManager()->getIsOpenAutoBattle())) {
                setSkillsPosj(getRandom(1, mapList - 2));
                if (num != 7 && this->getChildByTag(SKILLS_BULLET_TAG)) {
                    bulletShow(NULL);
                }
            }
        }
    }
}

void SGHeroLayer::randomX(){
    
    setSkillsPosj(getRandom(1, mapList - 2));
}

void SGHeroLayer::bulletShow(cocos2d::CCNode *node)
{
    CCLOG("bulletShow");
    //如果是在自动战斗模式，那主将技也自动释放
    if ((!isme &&SGAIManager::shareManager()->isPVE) || (SGAIManager::shareManager()->getIsOpenAutoBattle() && isme))
    {
        SGSkillManager::shareSkill()->sendSkillRequest(this->bulletJ);
    }
}

void SGHeroLayer::setSkillsPosj(int j)
{
    if (m_skillsId != skill_id_20064 &&
        m_skillsId != skill_id_20065) {
        if (j >= mapList - 1) {
            j = mapList - 2;
        }
        if (j <= 0) {
            j = 1;
        }
    }
    
    bulletJ = j;
    m_skills_Bullet->setPosition(GameConfig::getGridPoint(5, bulletJ,isme));
    if (m_skillsId != skill_id_20064 &&
        m_skillsId != skill_id_20065 &&
        m_skills_Bullet1 &&
        m_skills_Bullet2) {
        m_skills_Bullet1->setPosition(GameConfig::getGridPoint(5, bulletJ-1,isme));
        m_skills_Bullet2->setPosition(GameConfig::getGridPoint(5, bulletJ+1,isme));
    }
}

//修改援兵数字
void SGHeroLayer::setSoldierNum(int num)
{
    soldierNum = num;
    generateLabelAnim(soldierNumLabel, soldierNum);
    
    /*
    char soldier[256] = {0};
    sprintf(soldier, "%d",soldierNum);
    soldierNumLabel->setString(soldier);
    */
}

void SGHeroLayer::setRoundNum(int num)
{
    roundNum = num;
    char spriteName[256] = {0};
    sprintf(spriteName, "%d",roundNum);
    ccColor3B color;
    color.r=255;
    color.g=154;
    color.b=0;
//    if (num==3 && SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {
//        roundNumLabel->setInsideColor(color);
//    }
    if (num == 3)
    {
        roundNumLabel->setInsideColor(color);
    }
}

void SGHeroLayer::setBloodNum(int num)
{
    bool is = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (is) {
        CCString *str = CCString::createWithFormat(str_Format_set_hp,num);
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    
    if(num>=heroBloodMax)
        h_morale = heroBloodMax;
    else
        h_morale=num;
    CCString *booldStr = CCString::createWithFormat("%d",h_morale);
    heroBloodLabel->setString(booldStr->getCString());
    float pro = (float)h_morale /(float) heroBloodMax;
    this->updateProgress(pro);
}

void SGHeroLayer::setSkillsRound(int num)
{
    bool repair = SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap();
    if (repair)
    {
        skillsRoundNum = num;
    }
    else
    {
        skillsRoundNum += num;
    }
    CCString *str = CCString::createWithFormat("%d",skillsRoundNum);
    if (m_skillsId > 0 &&
        skillsRoundNum == 0)
    {
        //主将计启动特效
        changeButtonState(true, 1);

    }
    else
    {
        //关闭主将计特效
        changeButtonState(false, 1);

    }
}

void SGHeroLayer::sendSB2Scene()
{
    this->unschedule(schedule_selector(SGHeroLayer::sendSB2Scene));
    battleMap->chubing();
    this->changeSoldierNum(0);
}




void SGHeroLayer::fillUp()
{
    if (isme) {
        EFFECT_PLAY(MUSIC_BTN);
    }
    
    if (SGGuideManager::shareManager()->getFightGuideLayer()) {
        SGGuideManager::shareManager()->getFightGuideLayer()->closeTips();
    }
    else{
        SGGuideManager::shareManager()->getAutoGuideLayer()->closeTips();
    }
    
    if (!battleMap || battleMap->_isRefreshMap ||
        battleMap->m_touchState != eTouchNull)
    {
        return;
    }
    
    if (!SGBattleManager::sharedBattle()->getCanDoBattleOperation())
    {
        return;
    }
    
    this->getBattleMap()->hideRemindMe();
    int round = 0;
    CCDictionary *buff = NULL;
    CCObject *obj = NULL;
    
    CCARRAY_FOREACH(buffs, obj)
    {
        buff = (CCDictionary *)obj;
        int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
        switch (buffId) {
            case 1000:
            case 1002:
            case 1003:
            {
                round = ((CCString *)buff->objectForKey("round"))->intValue();
            }
                break;
            case 1007:
            case 1008:
            case 1009:
            {
                int r = ((CCString *)buff->objectForKey("round"))->intValue();
                roundNum += 1;
            }
                break;
			case 1010://幽冥盾
			{
				
			}
                
                
            default:
                break;
        }

    }
    
    
    if (SGGuideManager::shareManager()->isGuide) {
        
        //引导点击援兵时,不判断直接请求
        endRoundUpdataTime();
        //先构造一次，不然android上会莫名其妙挂掉
        std::string op(str_Player_operation);
        SGBattleManager::sharedBattle()->fightLogRequest(op);
        SGBattleManager::sharedBattle()->fillUnitRequest();
        
        battleMap->hideFriendlyHelp();

        if (isme && isInitPve && battleMap->getIsNeedRemoveArrow()) {
            battleMap->removeMoveArrow();
        }
        
        //新手引导   发送  用户点击操作   此处为点击援兵按钮
        SGMainManager::preProcessDataInFightSceneForSend(1);
        
        return;
    }
    
    
    CCLOG("$阻止对方增兵的buff:%d",round);
    if (round>0)
    {
        CCLOG("有阻止对方增兵的buff:%d",round);
//        SGAIManager::shareManager()->startRound(this->getBattleMap());
        return;
    }
    if (soldierNum > 0)
    {
        endRoundUpdataTime();
        SGBattleManager::sharedBattle()->fightLogRequest(str_Player_operation);
        SGBattleManager::sharedBattle()->fillUnitRequest();
    
        battleMap->hideFriendlyHelp();
        
        CCLOG("$开始补兵");

    }
}
CCArray *SGHeroLayer::getBuffs()
{
    return buffs;
}

//呼叫友军按钮
void SGHeroLayer::friendHardler()
{
    //GPCCLOG("SGHeroLayer::friendHardler,  call friend\n");
    EFFECT_PLAY(MUSIC_BTN);
    if (!battleMap || battleMap->_isRefreshMap || battleMap->m_touchState != eTouchNull || !isInitPve)
    {
        return;
    }
    
    if (!SGBattleManager::sharedBattle()->getCanDoBattleOperation())
    {
        return;
    }
    
    this->getBattleMap()->hideRemindMe();
    SGBattleManager::sharedBattle()->startCountingForHelp(false);
    bool isCanUp = true;
    if(friendNum != 0)
    {
        isCanUp = false;
    }
    
    SGBattleCallFriendBox *callFriend = SGBattleCallFriendBox::create(this, friendHeadId, friendLevel, isCanUp);
    SGMainManager::shareMain()->showBox(callFriend);
}



//友军上场
void SGHeroLayer::friendArmyUp()
{
    
    battleMap->hideFriendlyHelp();
    
    this->cooperationFillRequest();
}

void SGHeroLayer::cooperationFillRequest()
{
    if (SGBattleManager::sharedBattle()->getIsBattlePve())    //不需要验证
    {
        SGBattleManager::sharedBattle()->clientfillFriend();
    }
    else   //默认false，需要验证
    {
        
        if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            main::CooperationFillRequest *fill = new main::CooperationFillRequest;
            SGSocketClient::sharedSocketClient()->send(MSG_FRIEND, fill);
        }
    }
    

}

void SGHeroLayer::cooperationFillResponse(cocos2d::CCObject *obj)
{
    SGSocketRequest *msg=(SGSocketRequest *)obj;
   main::CooperationFillResponse *fill=(main::CooperationFillResponse *)msg->m_msg;
    
    if (fill && fill->state()==1) {//呼叫援军
        
        CCLog("呼叫成功");
        this->friendNum = 0;
        this->changeFriendNum(this->initFriendNum);
        this->isFriend = true;
        this->isFriendOffDeath = false;
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_NoSpace_for_thecall);
    }
    
}



void SGHeroLayer::abandonRound()     //以前的行动力
{
    EFFECT_PLAY(MUSIC_BTN);
    if (!battleMap || battleMap->m_touchState != eTouchNull || battleMap->_isRefreshMap) //bymm: 1.3进来点行动力必崩。
    {
        return;
    }
    
    if (!SGBattleManager::sharedBattle()->getCanDoBattleOperation())
    {
        return;
    }

    this->battleMap->hideRemindMe();

    if (SGGuideManager::shareManager()->getFightGuideLayer())
    {
        SGGuideManager::shareManager()->getFightGuideLayer()->closeTips();
    }
    else
    {
        SGGuideManager::shareManager()->getAutoGuideLayer()->closeTips();
    }
    
    battleMap->hideFriendlyHelp(false);
    
    SGBattleRoundPromptLayer *roundPromptLayer = SGBattleRoundPromptLayer::create(this, this->getRound());
    SGMainManager::shareMain()->showBox(roundPromptLayer);

    //点击行动力 发消息
    if( SGGuideManager::shareManager()->isGuide )
    {
        SGMainManager::preProcessDataInFightSceneForSend(1);
    }

    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
    {
        SGGuideManager::shareManager()->startFightGuide(7);
    }
    else
    {
        CCLOG("Third Guide in [%s-%d]", __FUNCTION__, __LINE__);

        SGGuideManager::shareManager()->startAutoGuide(7);
    }

}


//投降
void SGHeroLayer::surrenderRound()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (!battleMap || battleMap->m_touchState != eTouchNull || battleMap->_isRefreshMap) //bymm: 1.3进来点行动力必崩。
    {
        return;
    }
    
    if (!SGBattleManager::sharedBattle()->getCanDoBattleOperation())
    {
        return;
    }
    
    this->battleMap->hideRemindMe();
    
    if (SGGuideManager::shareManager()->getFightGuideLayer())
    {
        SGGuideManager::shareManager()->getFightGuideLayer()->closeTips();
    }
    else
    {
        SGGuideManager::shareManager()->getAutoGuideLayer()->closeTips();
    }
    
    battleMap->hideFriendlyHelp(false);
    
    SGBattleRoundSurrenderLayer *roundSurrenderLayer = SGBattleRoundSurrenderLayer::create(this, this->getRound());
    SGMainManager::shareMain()->showBox(roundSurrenderLayer);
    
    //点击行动力 发消息
    if( SGGuideManager::shareManager()->isGuide )
    {
        SGMainManager::preProcessDataInFightSceneForSend(1);
    }
    
    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
    {
        SGGuideManager::shareManager()->startFightGuide(7);
    }
    else
    {
        CCLOG("Third Guide in [%s-%d]", __FUNCTION__, __LINE__);
        
        SGGuideManager::shareManager()->startAutoGuide(7);
    }
    
}


//直接放弃回合, 回合数应该在出现动画之后直接置0
void SGHeroLayer::endRound()
{
    if (battleMap->m_touchState != eTouchNull ||
        battleMap->_isRefreshMap)
    {
        return;
    }
    
    if (isme)
    {
#if RoundCountryAB
        abandonRoundCountryAB();
#endif
        SGBattleManager::sharedBattle()->giveupRequest();
        if (SGGuideManager::shareManager()->getFightGuideLayer()) {
            SGGuideManager::shareManager()->getFightGuideLayer()->closeTips();
        }
        else{
            SGGuideManager::shareManager()->getAutoGuideLayer()->closeTips();
        }
    }

    changRoundNum(-roundNum, false);
	roundNum = 0;//test by bugcode
    SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
}

void SGHeroLayer::aiEndRound()
{
    
    this->getBattleMap()->hideRemindMe();
    EFFECT_PLAY(MUSIC_BTN);
#if RoundCountryAB
        abandonRoundCountryAB();
#endif
        SGBattleManager::sharedBattle()->giveupRequest();
        changRoundNum(-roundNum, false);
        SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
}

void SGHeroLayer::abandonRoundCountryAB()
{
    if (this->getCountry() == 2)
    {
        int a = this->heroBloodMax * abandonRoundCountryABRate1;//0.3;
        if (this->getMorale() < a) {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(this->battleMap->getAttackList(), obj)
            {
                SGAttackList *att = (SGAttackList*) obj;
                int ap = att->getMax_Ap() * abandonRoundCountryABRate2;//0.05;
                ap = ap * this->getRound();
               
                att->setSpAddAp(att->getSpAddAp() + ap);
                
                att->changeAP(att->getAp() + ap);
            }
        }
    }
}


void SGHeroLayer::heroSurrender()
{
    if (isme)
    {
        SGBattleManager::sharedBattle()->giveupRequest();
        
    }
    changRoundNum(-roundNum, false);
    SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
}

//参数是剩余的主将技回合数
void SGHeroLayer::giveupMessage(int skillNum)
{
#if RoundCountryAB
    abandonRoundCountryAB();
#endif
	changeEnemyRoundNum(-roundNum, skillNum);
    SGBattleManager::sharedBattle()->getBattleLayer()->giveupMessage();
}

void SGHeroLayer::cutAttackRound(int num)
{
    #if DEBUG
    CCObject *obj = NULL;
    CCArray *attackList = battleMap->getAttackList();
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        attack->changeRound(num, false);
    }
    this->startAttack();
    #endif
}

void SGHeroLayer::changBloodNum(int num, bool isBuff, bool skill)
{
    if (num == 0)
    {
        return;
    }
    
    bool is = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (is)
    {
        CCString *str = CCString::createWithFormat(str_Damage,abs(num));
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    else
    {
        CCString *str = CCString::createWithFormat(str_Damage,abs(num));
        CCLOG("【无日志】====>%s", str->getCString());
    }
    
    //掉血则震屏
    if (num < 0)
    {
        EFFECT_PLAY(MUSIC_41);
        //闪屏与晃动
        shakeAndWobble();
        //震屏
        int actnum = 1;
        if (abs(num) < this->heroBloodMax * 0.5 &&
            abs(num) >= this->heroBloodMax * 0.1)
        {
            actnum = 2;
        }
        else if( abs(num) >= this->heroBloodMax * 0.5)
        {
            actnum = 3;
        }
        
        SGBattleManager::sharedBattle()->getBattleLayer()->fuckLayer(actnum);
        int a = heroBloodMax * 0.5;
        if (abs(num) >= a)
        {
            setSkillsRoundNum(-3, true);
        }
    }
    
    int round = 0;
    CCDictionary *buff = NULL;
    CCObject *obj = NULL;
    if (!isBuff && num < 0)
    {
        CCARRAY_FOREACH(buffs, obj)
        {
            buff = (CCDictionary *)obj;
            int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
            if (buffId == 1001 /*|| (buffId == 1004)*/)// 空城计,改变血量的时候
            {
                round = ((CCString *)buff->objectForKey("round"))->intValue();
                if (round > 0) {
                    float value1 = ((CCString *)buff->objectForKey("value"))->floatValue();
                    num = num * value1;//num是对方攻击打到我方的攻击数值,value是反弹百分比,最后得到的num就是反弹给对方的掉值
                    this->showEffectLayer(0, false);
                }
                
                break;
            }
			else if(buffId == 1010 || buffId == 1012)//虚空盾//FIX
            {
                if (num < 0)
                {
                    //isAttackBase = true;
                    float value1 = ((CCString *)buff->objectForKey("value1"))->floatValue(); //盾的抵御数值
                    num = num + value1;//攻击伤害数值与抵御的数值,
                    if (num > 0)
					{
						buff->setObject(CCString::createWithFormat("%d", num), "value1");
                        num = 0;
                    }
					else
					{
						//墙被打破, 墙的特效就被移除
						//@Note by bugcode
						/*
						 *	此处盾用的第二个sprite的tag与援军旗子那个动画tag有冲突(51)
						 *	现在的解决是援军旗子tag多加了一个flag值(为99),防止点击援军后,盾出现一块缺口
						 */
						for (int i = 0; i < mapList; i++)
						{
							CCSpriterX *spx = (CCSpriterX *)this->getChildByTag(SKILLS_BULLET_TAG + i);
							if (spx)
							{
//								spx->removeAllChildrenWithCleanup(true);
								spx->removeFromParentAndCleanup(true);
								spx = NULL;
								this->removeChildByTag(TONGJING_EFFECT_TAG, true);
							}
							
						}

						
						buff->setObject(CCString::createWithFormat("%d", 0), "value1");
					}
                    
                    if (is)
					{
                        CCString *str = CCString::createWithFormat("进入buff  %d",num);
                        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
                    }
                }
                break;
            }
        }
        if (round>0)
        {//空城计
            CCLOG("有诸葛亮的buff:%d",round);
            if (this->getOpponentHeroLayer()->getBlood() + num <= 0)
            {
                if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsOver())
                {
                    SGBattleManager::sharedBattle()->battleOver();
                }
                SGBattleManager::sharedBattle()->getBattleLayer()->setIsOver(true);
            }
			
			
            this->getOpponentHeroLayer()->changBloodNum(num, true, true);//空城计掉血是攻击方, 最后一个参数传true
            return;
        }
    }
    
    if (is)
    {
        CCString *str = CCString::createWithFormat(str_Before_damage_hp, h_morale, abs(num));
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    else
    {
        CCString *str = CCString::createWithFormat(str_Before_damage_hp, h_morale, abs(num));
        CCLOG("【无日志】 %s",str->getCString());
    }
    //如果出现空城计,,掉血要显示在攻击这一方的血条上
	
	
    showMoraleLabel(abs(num), (num > 0) ?  "sanguobigpic/add_hp_label.png" : "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, skill);
    
    preBlood=h_morale;
    lostBlood=-num;
    h_morale += num;
    if (h_morale <= 0)
    {
        h_morale = 0;
        isBattleOver = true;
    }
    if (h_morale > heroBloodMax)
    {
        h_morale = heroBloodMax;
    }
    dangqianxie = h_morale;
    
    if(lostBlood+num==0)
        this->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(SGHeroLayer::scaleForBlood)),CCDelayTime::create(0.1),CCCallFunc::create(this, callfunc_selector(SGHeroLayer::updateBooldNum)),NULL));
    
    float pro = (float)h_morale /(float) heroBloodMax;
    this->updateProgress(pro);
    if (is) {
        CCString *str = CCString::createWithFormat(str_Before_residual_hp, h_morale);
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }else{
        CCString *str = CCString::createWithFormat(str_behit_sub_hp, h_morale);
        CCLOG("被攻击者输出=最后血======> %s", str->getCString());
    }
    
}
void SGHeroLayer::scaleForBlood(){
//    heroBloodLabel->runAction(CCMoveTo::create(0.1, ccpAdd(heroBloodLabel->getPosition(), ccp(0, 30))));
    heroBloodLabel->runactionWithScale(0.2, 2.0f);
}
void SGHeroLayer::updateBooldNum(){
    int randNum = 1;
    float randN = CCRANDOM_0_1();
    int value =lostBlood;
    
    if (value >= 1000000) {
        randNum = randN*500 + (int)(value/2);
    }else if (value >= 100000) {
        randNum = randN*500 + (int)(value/5);
    }else if (value >= 10000) {
        randNum = randN*500 + (int)(value/10);
    }else if (value >= 7500 && value < 10000) {
        randNum = randN*300 + 175;
    }else if (value >= 5000 && value < 7500) {
        randNum = randN*100 + 250;
    }else if (value >= 2500 && value < 5000) {
        randNum = randN*50 + 35;
    }else if (value >= 300 && value < 2500) {
        randNum = randN*40 + 25;
    }else if (value >= 500 && value < 1000) {
        randNum = randN*30 + 20;
    }else if (value >= 250 && value < 500) {
        randNum = randN*10 + 15;
    }else if (value >= 80 && value < 250) {
        randNum = randN*6 + 16;
    }else if (value >= 15 && value < 80) {
        randNum = randN*5 + 3;
    }else if (value >= 7 && value < 15) {
        randNum = randN*3 + 2;
    }
    else
        randNum = 1;

        
        
        
    lostBlood-=randNum;
    
    if (lostBlood >= 1   ) {
        preBlood-=randNum;
        if (preBlood<=0) {
            preBlood=0;
            lostBlood=0;
        }
       
        CCString *booldStr = CCString::createWithFormat("%d",preBlood);
        heroBloodLabel->setString(booldStr->getCString());
        
        CCSequence *sequence=CCSequence::create(CCDelayTime::create(0.01),CCCallFunc::create(this, callfunc_selector(SGHeroLayer::updateBooldNum)),NULL);
        
        heroBloodLabel->runAction(sequence);
    }else {
         preBlood-=lostBlood+randNum;
        if (preBlood<=0) {
            preBlood=0;
            lostBlood=0;
        }
        
        if (preBlood>heroBloodMax) {
            preBlood=heroBloodMax;
        }
        CCString *booldStr = CCString::createWithFormat("%d",dangqianxie);
       
//        CCLog("正确的%d,得到的%d",dangqianxie,preBlood);
        
        heroBloodLabel->setString(booldStr->getCString());
//       heroBloodLabel->runAction(CCMoveTo::create(0.1, ccpAdd(heroBloodLabel->getPosition(), ccp(0, -30))));
        heroBloodLabel->runactionWithScale(0.2, 1.0f);
    }

    
}
void SGHeroLayer::updateProgress(float pro)
{
    CCLOG("pro:%f",pro);
    float state = pro * 100;
    //百分比静态更新
    changeGateState(state);
    //血量设置，走动态效果
    setProgressEffect(hpProgress, state, MAX_GATE_TIME);
//    hpProgress->setPercentage(state);
}

//对图片使用动态的进度条效果。cpt：进度条对象 prop：将要到达的比例 fullTime：0-100所需时间
void SGHeroLayer::setProgressEffect(CCProgressTimer* cpt, float prop, float fullTime)
{
    if (!cpt)
        return;
    float from = cpt->getPercentage();
    float to = prop;
    float nani = to - from;
    float time = (nani >= 0 ? nani : -nani) / 100 * fullTime; //abs()杯具了
//    CCLOG("easy>>> from = %f", from);
//    CCLOG("easy>>> to = %f", to);
//    CCLog("easy>>> TIME = %f", time);
    CCProgressFromTo* cptAct = CCProgressFromTo::create(time, from, to);
//    CCProgressTo* cptAct = CCProgressTo::create(time, prop);
    cpt->stopAllActions();
    cpt->runAction(cptAct);
}



void SGHeroLayer::changeGateState(float state)
{
    GateBloodState tpState;
    //判定状态
    if (state > GATE_STATE_BROKEN)
        tpState = GBS_GOOD;
    else if (state > GATE_STATE_CRUSHED && state <= GATE_STATE_BROKEN)
        tpState = GBS_BROKEN;
    else if (state > GATE_STATE_DESTROYED && state <= GATE_STATE_CRUSHED)
        tpState = GBS_CRUSHED;
    else if (state <= GATE_STATE_DESTROYED)
        tpState = GBS_DESTROYED;
    else
        tpState = GBS_DESTROYED;
    //尝试修改
    if (tpState != m_gbs)
    {
        m_gbs = tpState;
        //修改城门与血条
        std::string sfName;
        std::string bloodName;
        if (m_gbs == GBS_GOOD)
        {
            sfName = GATE_IMAGE_GOOD;
            bloodName = GATE_BLOOD_GREEN;
        }
        else if (m_gbs == GBS_BROKEN)
        {
            sfName = GATE_IMAGE_BROKEN;
            bloodName = GATE_BLOOD_ORANGE;
        }
        else if (m_gbs == GBS_CRUSHED)
        {
            sfName = GATE_IMAGE_CRUSHED;
            bloodName = GATE_BLOOD_RED;
        }
        else if (m_gbs == GBS_DESTROYED)
        {
            sfName = GATE_IMAGE_DESTROYED;
            bloodName = GATE_BLOOD_RED;
        }
        
        //城门

        //血条
//        CCSprite* pSpt = CCSprite::createWithSpriteFrameName(bloodName.c_str());
//        hpProgress->setSprite(pSpt);
    }
}

//重置援兵数量, commented by bugcode
void SGHeroLayer::changeSoldierNum(int num, bool isRepair)
{
    if (isFriend) return;
    
    int num0 = 0;//兵
    int num1 = 0;//二格武将
    int num2 = 0;//四格武将
    for (int i = 0; i < mapRow; i++) {
        for (int j = 0; j < mapList; j++)
		{
            if (battleMap->myGrids[i][j])
			{
                ShiBing *bing = battleMap->getShiBingWithGrid(battleMap->myGrids[i][j]);
                if (bing) {
                    switch (bing->getSbType())
					{
                        case ksbone:
                        {
                            num0++;
                        }
                            break;
                        case ksbtwo:
                        {
                            num1++;
                        }
                            break;
                        case ksbfour:
                        {
                            num2++;
                        }
                            break;
                            
                        default:
                            break;
                    }
                }
            }
        }
    }
    //如果有二格武将,士兵数加二格武将数一半
    if (num1>0) {
        num0 += num1*0.5;
    }
    //如果有四格武将,士兵数加四格武将数四分之一
    if (num2>0) {
        num0 += num2*0.25;
    }
    //兵的数量,如果兵的数量小于最大兵数,就设置兵数为最大兵数减去地图上现有的算出的num0兵数,否则设置成0
    soldierNum = (max_soldierNum - num0)>0 ? (max_soldierNum - num0) : 0;
    

    
    char soldier[256] = {0};
    sprintf(soldier, "%d",soldierNum);
    soldierNumLabel->setString(soldier);//显示在援军label上
    
}

void SGHeroLayer::roundNumMOve(int num)
{
    if(!this->isme) return;
    if (num < 0) {
        return;
    }
    CCString *strN = NULL;
    ccColor3B color = ccRED;
    if (num >0)
    {
        strN = CCString::createWithFormat("+%d", num);
        color = ccGREEN;
    }
    else
    {
        strN = CCString::createWithFormat("%d", num);
        color = ccRED;
    }
    CCLabelTTF *label = CCLabelTTF::create(strN->getCString(), FONT_PANGWA, FONTSIZE(20));
    this->addChild(label, 110);
    label->setColor(color);
    label->setPosition(xdlBtnPos);
    CCMoveTo *move = CCMoveTo::create(0.5f, ccp(label->getPositionX(), label->getPositionY() + gridHeight));
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::rmoveRoundNum));
    label->runAction(CCSequence::create(move, call, NULL));
}

void SGHeroLayer::rmoveRoundNum(CCObject *obj)
{
    CCNode *node = (CCNode*)obj;
//    node->removeAllChildrenWithCleanup(true);
    node->removeFromParentAndCleanup(true);
}

//更改行动力回合数
int SGHeroLayer::changRoundNum(int num, bool isAction)
{
    //GPCCLOG("回合数改变前:%d",roundNum);
    //GPCCLOG("回合数改变:%d",num);
    if (num == 0)
    {
        roundNumLabel->setString(CCString::createWithFormat("%d",roundNum)->getCString());
        return roundNum;
    }
    
//    if (num == 3)
//    {
////        GPCCLOG("回合数改变前:%d",roundNum);
////        GPCCLOG("回合数改变:%d",num);
//    }
   
    
    if (num < 0)
	{
        int a = num / 2;
        //int a = num;   //modified by : for endRound skill add
		setSkillsRoundNum(a, true);
    }
    this->roundNumMOve(num);
    SGBattleManager::sharedBattle()->fightLogRequest(CCString::createWithFormat(str_Move_power_change_before, roundNum)->m_sString);
    roundNum += num;
    roundNum = roundNum >=0 ? roundNum : 0;
    
    generateLabelAnim(roundNumLabel, roundNum,true);
   
    SGBattleManager::sharedBattle()->fightLogRequest(CCString::createWithFormat(str_Move_power_change_after, roundNum)->m_sString);
    return  roundNum;
}

//只改变敌人的回合数,和技能点数,并置0回合数
void SGHeroLayer::changeEnemyRoundNum(int num, int skillNum)
{
	if (num == 0)
    {
        return;
    }
	
    if (num < 0)
	{
		//通过服务器发送的主将技回合数,确定应该减多少
		int minusNum = this->getSkillsRoundNum() - skillNum;
		setSkillsRoundNum(-minusNum, true);
    }
    this->roundNumMOve(num);
	
	roundNum = 0;
    
}


void SGHeroLayer::setReduceRoundLabel(SGCCLabelTTF* label, int num )
{
    label->stopAllActions();
    label->setPosition(label->getPosition());
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() )
    {
        //修改
        setLabelNum(label,num);
        setLabelColor(label,num);
        starEffect();
        
        label->setZOrder(100);
    }
    else
    {
        //修改
        setLabelNum(label,num);
        
        ccColor3B color;
        color.r=0;
        color.g=255;
        color.b=204;
        label->setInsideColor(color);
    }
}

//生成数字更改动画    //isAction,  是否是行动力数字
void SGHeroLayer::generateLabelAnim(SGCCLabelTTF* label, int num ,bool isAction)
{
    label->stopAllActions();
    label->setPosition(label->getPosition());
     if (isAction)
     //if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() && isAction)
     {
         //        //幻象
         CCCallFuncND *call1 = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::scaleWithLabel), (void*)4);
         
         CCMoveTo *move = CCMoveTo::create(BATTLENUMMOVETIME, ccp(label->getPosition().x, label->getPosition().y+60 ));
         //修改
         CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::setLabelNum), (void*)num);
         //归位
         CCMoveTo *move1 = CCMoveTo::create(BATTLENUMMOVETIME/2, ccp(label->getPosition().x, label->getPosition().y));
         CCCallFuncND *call2= CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::scaleWithLabel), (void*)1);
         CCCallFuncND *call3= CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::setLabelColor), (void*)num);
         CCCallFunc *call4= CCCallFunc::create(this, callfunc_selector(SGHeroLayer::starEffect));
         //执行
         label->runAction(CCSequence::create(call3,call1,move, call,call2,move1,CCDelayTime::create(0.1),call4,NULL));
         label->setZOrder(100);
     }
    else
    {
         //幻象
        CCCallFuncND *call1 = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::scaleWithLabel), (void*)4);
        
        CCMoveTo *move = CCMoveTo::create(BATTLENUMMOVETIME, ccp(label->getPosition().x, label->getPosition().y+10 ));
        //修改
        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::setLabelNum), (void*)num);
        //归位
        CCMoveTo *move1 = CCMoveTo::create(BATTLENUMMOVETIME/2, ccp(label->getPosition().x, label->getPosition().y));
        CCCallFunc *call2= CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::scaleWithLabel), (void*)1);
        //   CCCallFunc *call3= CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::setLabelColor), (void*)1);
        //执行
        label->runAction(CCSequence::create(call1,move, call,call2,move1,NULL));
        
        ccColor3B color;
        color.r=0;
        color.g=255;
        color.b=204;
        label->setInsideColor(color);
    }
}

void SGHeroLayer::scaleWithLabel(cocos2d::CCObject *obj,int num){
    SGCCLabelTTF *label = (SGCCLabelTTF *) obj;
    label->runactionWithScale(0.1, num);

}

//修改数字本体
void SGHeroLayer::setLabelNum(cocos2d::CCObject *obj, int num)
{
    CCString *str = CCString::createWithFormat("%d", num);
    SGCCLabelTTF *label = (SGCCLabelTTF *) obj;
    
    label->setString(str->getCString());

   
}

void SGHeroLayer::starEffect(){
    
  
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("particle/erwer.plist");
   // CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    particle->setPosition(ccpAdd(roundNumLabel->getPosition(),ccp(0,0)));
    imgMove->addChild( particle);
    
}
void SGHeroLayer::setLabelColor(cocos2d::CCObject *obj,int num){

    ccColor3B color;
    if (num==1) {
        color.r=255;
        color.g=0;
        color.b=0;

    }else if(num==2)
    {
        color.r=255;
        color.g=194;
        color.b=0;

    }
    else if(num==3)
    {
        color.r=0;
        color.g=154;
        color.b=0;

    }else{
        color.r=0;
        color.g=255;
        color.b=204;

    }
  
    SGCCLabelTTF *label = (SGCCLabelTTF *) obj;
    label->setInsideColor(color);
}
//切换回合
void SGHeroLayer::changeRound(bool isSetTouch, bool isMyRound, bool isFirst)
{
    //GPCCLOG("SGHeroLayer::changeRound");
    menu->setEnabled(isMyRound);
    if(isme && SGBattleManager::sharedBattle()->getBattleType() == BT_ARENA)
    {
        setBtnEnable(false);
        
        if(autoBattleBtn)
        {
            autoBattleBtn->setEnabled(false);
        }
    }
    m_jumpLableIndexList.clear();
    if (isMyRound) {
        menu->setColor(ccc3(255, 255, 255));

    }else{
        if (battleMap->getIsNeedRemoveArrow()) {
            SGGuideManager::shareManager()->getFightGuideLayer()->deleteFreeTip();
            battleMap->removeMoveArrow();
        }
    }
	//主将技没有激活
    if (m_skillsId <= 0)
    {
       
       // zjjText->setVisible(false);
        //skillNumLabel->setVisible(false);
        zjj_btn->setEnabled(false);
    }
    wuzjj->setVisible((m_skillsId <= 0));
    //如果是玩家操作,设置回合数
    
    if(battleMap->getIsMe())
    {
      
        battleMap->setIsMoveAble();
        
    }
    battleMap->changeRound(isSetTouch, isMyRound, isFirst);
    battleMap->isUpdateEnd = true;
    if (isMyRound)
    {
        if(SGBattleManager::sharedBattle()->getIsBattlePve())
        {
             this->setRoundNum(this->roundNum);
        }
		//这里做每一次回合改变的时候,攻击队列攻击相关逻辑
        this->startAttack();
        if (!isFirst)
        {
            this->setSkillsRoundNum(-1, false);//技能回合-1
            this->changeFriendNum(-1, false);//援军中回合-1
            this->updateRoundBuff();//buff更新,是否还起效等等
        }
        
    }
    else
    {
        this->setRoundNum(0);//不是玩家的回合,就直接置回合为0
    }
   // GPCCLOG("SGHeroLayer::changeRound_End");
}

void SGHeroLayer::activateBuff()
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(battleMap->m_heroBuff, obj)
    {
        CCDictionary *data = (CCDictionary*)obj;
        int buffId = ((CCString *)data->objectForKey("buffId"))->intValue();
        switch (buffId) {
            case 1004:
            {
                float value1 = ((CCString*)data->objectForKey("value1"))->floatValue();
                int blood = heroBloodMax * value1;
                changBloodNum(-blood, false, true);
                if (this->h_morale<= 0) {
                    SGBattleManager::sharedBattle()->getBattleLayer()->showIsWin();
                }
                return;
            }
                break;
                
            default:
                break;
        }
    }
}
//显示敌方的buff的动作
void SGHeroLayer::showEmenyBuAct()
{
    CCDictionary *buff = NULL;
    CCObject *obj = NULL;
    bool isRemove = false;
    CCARRAY_FOREACH(buffs, obj)
    {
        buff = (CCDictionary *)obj;
        int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
        CCLOG("buffId:%d",buffId);
        if ( (buffId == 1002) || (buffId == 1003)) {
            int  round = ((CCString *)buff->objectForKey("round"))->intValue();
            if (round>0)
            {
                CCLOG("减一:%d",round);
                round-=1;
            }
            if (round>0)
            {
                buff->setObject(CCString::createWithFormat("%d",round), "round");
            }
            else
            {
                CCLOG("最后一回合");
                isRemove = true;
                if (this->getChildByTag(FORBID_EFFECT_TAG * 20)) {
                    
                    CCSpriterX *spx = (CCSpriterX*)this->getChildByTag(FORBID_EFFECT_TAG * 20);
                    CCPoint pos = spx->getPosition();
                    spx->removeFromParentAndCleanup(true);
                    spx = NULL;
                    
                    
                    this->removeChildByTag(FORBID_EFFECT_TAG * 20, true);

                    ResourceManager::sharedInstance()->bindTexture("animationFile/jzan.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
                    
                    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jzan_001.scml", true, true);
                    fermEffect->setanimaID(0);
                    fermEffect->setisloop(false);

                    fermEffect->setPosition(pos);
                    
                    addChild(fermEffect, FORBID_EFFECT_TAG, FORBID_EFFECT_TAG);
                    fermEffect->play();

                }
            }
            break;
        }
    }
    if (isRemove &&
        buff)
    {
        buffs->removeObject(buff);
    }
}
//更新回合buff
void SGHeroLayer::updateRoundBuff()
{
    
    CCObject *obj1 = NULL;
    CCARRAY_FOREACH(this->getBattleMap()->m_heroBuff, obj1)
    {
        CCDictionary *buff = (CCDictionary *)obj1;
        int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
        SGBattleManager::sharedBattle()->fightLogRequest(CCString::createWithFormat("buffid:%d", buffId)->getCString());   
    }
    
    
    CCDictionary *buff = NULL;
    CCObject *obj = NULL;
    bool isRemove = false;
    CCARRAY_FOREACH(buffs, obj)
    {
        buff = (CCDictionary *)obj;
        int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
        CCLOG("buffId:%d",buffId);
        
        if ( (buffId == 1000) || (buffId == 1001) || (buffId == 1004) || (buffId == 1007))
        {
        
			//1001,对应空城计
            int  round = ((CCString *)buff->objectForKey("round"))->intValue();//更新回合的时候,字典里的对应buff的回合数要更新
            if (round>0)
            {
                CCLOG("减一:%d",round);
                round-=1;
            }
            
            SGBattleManager::sharedBattle()->fightLogRequest(CCString::createWithFormat("buffid:%d, %d", buffId, round)->getCString());
            
            if (round>0)
            {
                 buff->setObject(CCString::createWithFormat("%d",round), "round");
            }
            else
            {
                CCLOG("最后一回合");
                isRemove = true;
                
                if (buffId == 1000) {
                    if (this->getChildByTag(FORBID_EFFECT_TAG * 20)) {
                        
                        CCSpriterX *spx = (CCSpriterX*)this->getChildByTag(FORBID_EFFECT_TAG * 20);
                        CCPoint pos = spx->getPosition();
//                        spx->removeAllChildrenWithCleanup(true);
                        spx->removeFromParentAndCleanup(true);
                        spx = NULL;
                        this->removeChildByTag(FORBID_EFFECT_TAG * 20, true);
                        ResourceManager::sharedInstance()->bindTexture("animationFile/jzan.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
                        CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jzan_001.scml", true, true);
                        fermEffect->setanimaID(0);
                        fermEffect->setisloop(false);
                        fermEffect->setPosition(pos);
                        
                        addChild(fermEffect, FORBID_EFFECT_TAG, FORBID_EFFECT_TAG);
                        fermEffect->play();
                    }
                }
                
            }
            break;
        }
    }
    if (isRemove && buff)
    {
        buffs->removeObject(buff);
    }
}

//修改友军回合数
void SGHeroLayer::changeFriendNum(int num, bool isAction)
{
    if (!isInitPve|| isFriendOffDeath) return;
    if (!this->isme) return;
    friendNum += num;
    friendNum = friendNum> 0 ? friendNum : 0;
    
    
    //友军状态与特效处理
    if (friendNum <= 0)
    {
        if (SGGuideManager::shareManager()->isGuide)
        {
            return;
        }
        changeButtonState(true, 2);
    }
    else
    {
        changeButtonState(false, 2);
    }

}

//开始攻击, 切换回合时候每次的蓄力,检测是否有可攻击阵列,等
void SGHeroLayer::startAttack()
{
   
   // GPCCLOG("startAttack");
    if (isBattleOver) {
        return;
    }
    
    if (!battleMap->getAttackList() || battleMap->getAttackList()->count() < 1)
    {
        changeRoundRemindME();
        schedule(schedule_selector(SGHeroLayer::heroDefSkills), 1.5);
        //heroDefSkills();
        return;
    }
    bool notAttack = true;   //回合数没到，没有进行攻击
    wujiangjiNum = 0;

    CCObject *obj = NULL;
    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        CCLOG("attackRound:%d",attack->getRoundV());
        if (attack->getRoundV() == 0)
        {
           
            isRefreshMap = false;
            notAttack = false;
            isAttackOver = false;
            
            break;
        }
    }
    
    float delayTime = 0.0f;    //攻击之前的延时，等待头像特效结束，纯士兵为0
    bool haveWujiang = false;   //有武将阵列
    //显示两格四格攻击阵列的武将头像
    CCObject *obj1 = NULL;
    CCARRAY_FOREACH(battleMap->getAttackList(), obj1)
    {
        SGAttackList *attack = (SGAttackList *)obj1;
        
        if (attack->getAttackType() == kattacktwo ||
            attack->getAttackType() == kattackfour)
        {
            if (attack->getRoundV() == 0)
            {
                
                //delayTime = 3.0;
                headEffArray->addObject(attack);
                haveWujiang = true;
                wujiangjiNum ++;
            }
        }
    }
    
    
    //回合数没到，检测防御技能
    if (notAttack)
    {
        
        schedule(schedule_selector(SGHeroLayer::heroDefSkills), 1.5);

    }
    
    //回合数到了，开始攻击
    if (!notAttack)
    {
        if (haveWujiang)
        {
            delayTime = 3.25 + wujiangjiNum * 0.25 + 0.5 ;
            //GPCCLOG("attackDelayTime = %f, wujiangCount = %d", delayTime, wujiangjiCount);
        }
        else
        {
            delayTime = 0;
        }
        SGBattleManager::sharedBattle()->getBattleLayer()->showColorSp(true);
        CCDelayTime *attackDelay = CCDelayTime::create(delayTime);

        CCCallFunc *headEffCall = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::headEffects));
        //CCDelayTime *beforeHeadDelay = CCDelayTime::create(1.5 + (wujiangjiCount -1) * 0.25 + 1.2);   //before is 1.0 modified by cgp for 1.6
       // CCDelayTime *time1 = CCDelayTime::create(1.6);
        CCDelayTime *beforeHeadDelay = CCDelayTime::create(1.0);
        CCCallFunc *attackCall = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::attackBegin));
        this->runAction(CCSequence::create(beforeHeadDelay, headEffCall, attackDelay, attackCall, NULL));
        battleMap->hideFriendlyHelp(false);

    }
    else
    {
//        没有可攻击的 回合 
//    打开
        battleMap->hideFriendlyHelp();
    }
    
}

void SGHeroLayer::headEffects(CCObject *array)
{
    if (headEffArray->count())
    {
        //GPCCLOG("headEffect武将计");
        this->showAttactHeadEffect(headEffArray);
        headEffArray->removeAllObjects();
    }
}
static bool AttackSortRule(const CCObject * obj1, const CCObject * obj2)
{
    SGAttackList *attack1 = (SGAttackList *)obj1;
    SGAttackList *attack2 = (SGAttackList *)obj2;
    if(attack1->getAttackType() != kattacknormal && attack2->getAttackType() == kattacknormal)
    {
        return  true;
    }
    else if(attack1->getAttackType() == kattacknormal && attack2->getAttackType() != kattacknormal)
    {
        return false;
    }
    else if(attack1->getAttackType() == kattacknormal && attack2->getAttackType() == kattacknormal)
    {
        int n1 = 0;
        int n2 = 0;
        if(attack1->getSbColor() == kcolorBlue)
        {
            n1 = 3;
        }
        else if(attack1->getSbColor() == kcolorRed)
        {
            n1 = 2;
        }
        else if(attack1->getSbColor() == kcolorGreen)
        {
            n1 = 1;
        }
        
        if(attack2->getSbColor() == kcolorBlue)
        {
            n2 = 3;
        }
        else if(attack2->getSbColor() == kcolorRed)
        {
            n2 = 2;
        }
        else if(attack2->getSbColor() == kcolorGreen)
        {
            n2 = 1;
        }
        return n1 < n2;
    }
    else
    {
        return false;
    }
    
}
//开始攻击,包括技能特效展示,
void SGHeroLayer::attackBegin()
{
    //GPCCLOG("SGHeroLayer::attackBegin");
    if (isBattleOver)
    {
        return;
    }
    
    bool notAttack = true;
    CCObject *obj = NULL;
    SGBattleLayer *battleLayer = (SGBattleLayer *)this->getParent();
    SGHeroLayer *opponent = battleLayer->getOpponent(isme);
    
    
    bool hasAttack=false;
    std::sort(battleMap->getAttackList()->data->arr,battleMap->getAttackList()->data->arr + battleMap->getAttackList()->data->num,AttackSortRule);
	//检测每个攻击阵列,并检测技能,画出相关特效
    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        CCLOG("attackRound:%d",attack->getRoundV());
        if (attack->getRoundV() == 0)
        {
            hasAttack=true;

            SGBattleManager::sharedBattle()->getBattleLayer()->showColorSp(true);

            SGBattleManager::sharedBattle()->startCountingForHelp(false);
            attack->startAttack(this, opponent);
            if (attack->getAttackType() != kattacknormal)
            {
                EFFECT_PLAY(MUSIC_39);
            }
            else
            {
                EFFECT_PLAY(MUSIC_42);
            }
			//设置可复活的次数
            attack->setReviveNum(0);
            isRefreshMap = false;
            notAttack = false;
            
            break;
        }
    }
    
    if (!hasAttack)
    {
        changeRoundRemindME();
    }
    if (notAttack)
    {
        heroDefSkills();
    }
}

void SGHeroLayer::delayRemoveAttaklist(cocos2d::CCNode* obj, void * psender)
{
    SGAttackList* attackList = (SGAttackList*)psender;
    battleMap->removeAttackList(attackList, NULL, NULL, true);
}

void SGHeroLayer::attackOver(SGAttackList *attackList_, bool notRemove)    //cgpUsefulAction
{
    CCDelayTime *delayAttack = NULL;
    
    if (notRemove == false)
    {
        battleMap->removeAttackList(attackList_, NULL, NULL, true);
        delayAttack = CCDelayTime::create(0.5);
    }
    else
    {
        delayAttack = CCDelayTime::create(0.5);
        CCDelayTime *delayRemove = CCDelayTime::create(0.4);
        
//        delayAttack = CCDelayTime::create(0.8);
//        CCDelayTime *delayRemove = CCDelayTime::create(0.7);
        CCCallFuncND*  callNd = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::delayRemoveAttaklist), (void*)attackList_);
        this->runAction(CCSequence::create(delayRemove, callNd, NULL));
    }
    
    isRefreshMap = true;

    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::delayTimeAttack));
    this->runAction(CCSequence::create(delayAttack, call, NULL));
}

void SGHeroLayer::chainApAnd()
{
    if (isBattleOver) {
        return;
    }
    
    if (!battleMap->getAttackList() || battleMap->getAttackList()->count() < 1)
    {
        return;
    }
    CCObject *obj = NULL;
    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        if (attack->getRoundV() == 0)
        {
            attack->m_effectsLayer->removeAllChildren();
            //连锁
            attack->calculateChainAp(this);
            attack->calculateFuseAp(this);
            break;
        }
    }

}


void SGHeroLayer::delayTimeAttack(cocos2d::CCObject *obj)
{
   // GPCCLOG("delayTimeAttack");
    this->attackBegin();
    if (isRefreshMap)
    {
        //    打开 回合
        
        battleMap->hideFriendlyHelp();
        
        //SGBattleManager::sharedBattle()->getBattleLayer()->showColorSp();
        SGBattleManager::sharedBattle()->getBattleLayer()->changeColorSp();
        isAttackOver = true;
        SGHeroLayer *hero = NULL;
        bool isRefresh = true;
        
        bool my = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        hero = SGBattleManager::sharedBattle()->getHeroLayer(!my);
       // showLJEff(true);    //removed by cgp
        if (hero->h_morale <= 0)
        {
            SGBattleManager::sharedBattle()->getBattleLayer()->showIsWin();
        }
        else if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()
                 && !SGGuideManager::shareManager()->isGuide )
        {
//            SGMainManager::shareMain()->showMessage("可以行动了哟！！！");
                changeRoundRemindME();
        }
        m_LJNum = 0;
        if (isRefresh)
        {
            if (heroDefSkills())
            {
                battleMap->refreshMapGrid(true);
               
            }
        }
    }
}

bool SGHeroLayer::heroDefSkills()
{
    unschedule(schedule_selector(SGHeroLayer::heroDefSkills));
    bool isRefres = true;
    int round = 1;
    if (!isAttackBase) {
        int time = 1.5;
        CCDictionary *buff = NULL;
        CCObject *obj = NULL;
        CCARRAY_FOREACH(buffs, obj)
        {
            buff = (CCDictionary *)obj;
            int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
            if (buffId == 1010) {
                
                round = ((CCString *)buff->objectForKey("round"))->intValue();
                if (round > 0) {
                    round = round - 1;
                    buff->setObject(CCString::createWithFormat("%d",round), "round");
                }
                if (round <= 0) {
                    isRefres = false;
                    
                    SGHeroLayer *hero = (SGHeroLayer*)buff->objectForKey("hero");
                    SGHeroLayer *hero1 = (SGHeroLayer*)buff->objectForKey("hero1");
                    float a = ((CCString *)buff->objectForKey("value1"))->floatValue();
                    int b = ((CCString *)buff->objectForKey("roleid"))->intValue();
                    if(a!=0)
                    {
                        SGSkillManager::shareSkill()->skill_20063_Attack(hero, hero1, a, b, time);
                    }
                }
            }else if(buffId == 1012){
                round = ((CCString *)buff->objectForKey("round"))->intValue();
                if (round > 0) {
                    round = round - 1;
                    buff->setObject(CCString::createWithFormat("%d",round), "round");
                }
                if (round <= 0) {
                    isRefres = false;
                    SGHeroLayer *hero = (SGHeroLayer*)buff->objectForKey("hero");
                    for (int i = 0; i < mapList; i++) {
                        hero->removeChildByTag(SKILLS_BULLET_TAG + i, true);
                    }
                }
            }
        }
        
        if (!isRefres)
        {
            buffs->removeObject(buff);
        }
    }else
    {
        bool isRemove = false;
        CCDictionary *buff = NULL;
        CCObject *obj = NULL;
        CCARRAY_FOREACH(buffs, obj)
        {
            buff = (CCDictionary *)obj;
            int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
            if (buffId == 1010 ||
                buffId == 1011 ||
                buffId == 1012){
                round = ((CCString *)buff->objectForKey("round"))->intValue();
                if (round > 0) {
                    round = round - 1;
                    buff->setObject(CCString::createWithFormat("%d",round), "round");
                }
                if (round <= 0) {
                    isRemove = true;
                    SGHeroLayer *hero = (SGHeroLayer*)buff->objectForKey("hero");
                    for (int i = 0; i < mapList; i++) {
                        hero->removeChildByTag(SKILLS_BULLET_TAG + i, true);
                    }
                }
            }
        }
        if (isRemove) {
            buffs->removeObject(buff);
            isAttackBase = false;
        }
        
        
    }
    
    return isRefres;
}

void SGHeroLayer::refreshMap(CCObject *obj, CCDirector *buff)
{
    //GPCCLOG("SGHeroLayer::refreshMap");
    buffs->removeObject(buff);
    isAttackBase = false;
    battleMap->refreshMapGrid(true);
}

//修改主将计回合数
void SGHeroLayer::setSkillsRoundNum(int num, bool isAction)
{
    if (num == 0)
	{
        return;
    }
    if (num <= 0)
	{
        if (skillsRoundNum == 0)
		{
            return;
        }
    }
    
    if (skillsRoundNum == 0)
	{
        //        点亮主将计按钮
    }
	else
	{
        //        屏蔽主将计按钮 不管有没有点亮
        
        if (num < 0)
		{
            
            CCObject *obj = NULL;
            CCARRAY_FOREACH(battleMap->getAttackList(), obj)
            {
                SGAttackList *attack = (SGAttackList *)obj;
                
                if (attack->getSkillsDelegate())
                {
                    attack->activateSkills10(NULL, attack, num);
                }
            }
        }
        
    }
    
    skillsRoundNum += num;
    
    if (skillsRoundNum <= 0)
    {
        skillsRoundNum = 0;
        if ( SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId() == 1 && SGGuideManager::shareManager()->isGuide)
        {
            return;
        }

        changeButtonState(true, 1);

        if (isme)
        {
            if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
            {
                SGGuideManager::shareManager()->startFightGuide(8);
            }
            else
            {
            }
        }
    }
    else
    {

            changeButtonState(false, 1);

    }

}


int SGHeroLayer::getRoleId()
{
    return roleId;
}

SGBattleMap *SGHeroLayer::getBattleMap()
{
    return battleMap;
}

int SGHeroLayer::getCountry()
{
    return country;
}

int SGHeroLayer::getFriendNum()
{
    return friendNum;
}

bool SGHeroLayer::getIsInitPve()
{
    return isInitPve;
}

int SGHeroLayer::getSoldier(bool remainder)
{
    int num = 0;
    int num1 = 0;
    int num2 = 0;
    for (int i = 0; i < mapRow; i++) {
        for (int j = 0; j < mapList; j++) {
            
            SGGridBase *grid = battleMap->myGrids[i][j];
            if (NULL != grid) {
                ShiBing *bing = battleMap->getShiBingWithGrid(grid);
                if (NULL != bing) {
                    switch (bing->getSbType()) {
                        case ksbone:
                        {
                            num++;
                        }
                            break;
                        case ksbtwo:
                        {
                            num1++;
                        }
                            break;
                        case ksbfour:
                        {
                            num2++;
                        }
                            break;
                            
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    if (num1>0) {
        num += num1*0.5;
    }
    
    if (num2>0) {
        num += num2*0.25;
    }
    
    if (remainder) {
        return num;
    }else{
        soldierNum = (max_soldierNum - num)>0 ? (max_soldierNum - num) : 0;
        char soldier[256] = {0};
        sprintf(soldier, "%d",soldierNum);
        soldierNumLabel->setString(soldier);
        
        return soldierNum;
    }
}

int SGHeroLayer::getRound()
{
    return roundNum;
}

int SGHeroLayer::getInitRoundNum()
{
    return initRoundNum;
}

int SGHeroLayer::getBlood()
{
    if (dangqianxie != h_morale) {
        CCString *str = CCString::createWithFormat(str_now_sending, dangqianxie, h_morale);
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    return h_morale;
}

int SGHeroLayer::getSkillsRoundNum()
{
    return skillsRoundNum;
}

SGHeroLayer *SGHeroLayer::getOpponentHeroLayer()
{
    return SGBattleManager::sharedBattle()->getHeroLayer(!isme);
}

void SGHeroLayer::addBuff(cocos2d::CCDictionary *buff)
{
    buffs->addObject(buff);
}

void SGHeroLayer::playBuffEffect()
{
    return;
    for (int j = 0; j < mapList; j++) {
        for (int i = 0; i < mapRow; i++) {
            if (battleMap->m_seveBuffGrid[j][i]->m_baffObj->count()) {
                for (int k = 0; k < battleMap->m_seveBuffGrid[j][i]->m_baffObj->count(); k++) {
                    SG_BuffBase *buff = (SG_BuffBase*)(battleMap->m_seveBuffGrid[j][i]->m_baffObj->objectAtIndex(k));
                    if (buff) {
                        
                    }
                    buff->activateBuff(gameIndex(j, i));
                }
            }
        }
    }
}

void SGHeroLayer::alertViewClickedButtonAtIndex(int buttonIndex)
{

}


void SGHeroLayer::showEffectLayerCgp(int type, bool isRelease, GameIndex index, CCPoint difPos)
{

    bool isloop = false;
    float scale = 1.0;
    float completeTime = 0;   //整个动作的时间
    CCString *str = NULL;
    CCString *animationName = NULL;
    switch (type)
    {
        case Eff_xiaoshi://消失
        {
            str = CCString::create("xiaoshi");
            animationName = CCString::create("animation");
            completeTime = 0.7;
            break;
        }
        case Eff_sanbing:  //散兵被攻击
        {
            str = CCString::create("sanbing");
            animationName = CCString::create("animation");
            completeTime = 0.4;
            break;
        }
            
        case Eff_faluo:  //法螺，代替旗帜
        {
            str = CCString::create("faluo");
            animationName = CCString::create("animation");
            completeTime = 1.0;
            break;
        }
   
        default:
            break;
    }
    if (str)
    {
        CCString*   jsonName = CCString::createWithFormat("effSpine/%s.json",str->getCString());
        CCString*   atlasName = CCString::createWithFormat("effSpine/%s.atlas",str->getCString());
        CCSkeletonAnimation*  effect = CCSkeletonAnimation::createWithFile(jsonName->getCString(), atlasName->getCString(), scale);

        effect->setAnimation(animationName->getCString(), isloop);
        
        bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        CCPoint effPos = GameConfig::getGridPoint(index.i, index.j, !round);

        if (type == Eff_faluo)
        {
            effPos.x = 320;
            effPos.y = isme ? 350 : 850;
        }
        effect->setPosition(ccpAdd(effPos, difPos));
        this->addChild(effect, CGP_EFFECT_ZORDER);
        if (isRelease)
        {
            CCDelayTime *time = CCDelayTime::create(completeTime);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::removeEffSpine), (void*)1);
            CCAction *act = CCSequence::create(time, call, NULL);
            effect->runAction(act);
        }
    }
    
}

void SGHeroLayer::showEffectLayer(int type, bool isRelease, GameIndex index, CCPoint difPos)
{
    
	int flag = 0;
    bool isloop = false;
    float scale = 1.0;
    int i = 0;
    CCString *str = NULL;
    switch (type) {
        case 0://铜镜
        {
            str = CCString::create("sgtj");
        }
            break;
        case 1://魂飞出
        {
            str = CCString::create("fanXi");
        }
            break;
        case 2://喷血
        {
            str = CCString::create("pxd");
        }
            break;
        case 3://一闪
        {
            str = CCString::create("ys");
        }
            break;
        case 4://天使之光
        {
            str = CCString::create("tssg");
        }
            break;
        case 5://火球爆炸
        {
            str = CCString::create("hq");                                                                                                                                   
        }
            break;
        case 6://音波溅射
        {
            str = CCString::create("ybjs");
        }
            break;
        case 7://变形
        {
            str = CCString::create("qbx");
            i = 1;
        }
            break;
        case 8://炸弹爆炸
        {
            str = CCString::create("zdbz_000");
            i = 1;
        }
            break;
        case 9://圆火球
        {
            str = CCString::create("qbhq");
        }
            break;
        case 10://攻击到底线
        {
            str = CCString::create("jian_2_bz");
        }
            break;
        case 11://刀波攻击
        {
            str = CCString::create("dbgj");
        }
            break;
        case 12://旗子燃烧
        {
            str = CCString::create("qzrs");
        }
            break;
        case 13://电墙爆炸
        {
            str = CCString::create("dq_002");
            i = 1;
        }
            break;
        case 14:
        {
            str = CCString::create("lx_000");
        }
            break;
        case 15:
        {
            str = CCString::create("rh_000");
            scale = 3.0;
        }
            break;
        case 16://旗子
        {
            str = CCString::create("qz");
            isloop = true;
			flag = 99;//旗子的单独tag,与幽冥盾与虚空盾分开
        }
            break;
        case 17:
        {
            str = CCString::create("eff_fire6");


        }
            break;
        case 18:
        {
            str = CCString::create("zhanHun");
        }
            
            break;
        case 19:
        {
            str = CCString::create("zx");
        }
            break;
        case 20:
        {
            str = CCString::create("ll");
        }
            break;
        case 21:
        {
            str = CCString::create("jxbz");
            break;
        }
        
            //散兵消失，add by cgp
        case 22:
        {
            str = CCString::create("sanbing");
            break;
        }
            //阵列消失,add by cgp
        case 23:
        {
            str = CCString::create("xiaoshi");
            scale = 1.3;
            break;
        }
            
        default:
            break;
    }
    if (str)
    {
        //加载资源
        CCString*   name;
        if (type > 21)
        {
            name = CCString::createWithFormat("effects/%s.plist",str->getCString());

        }
        else
        {
           name = CCString::createWithFormat("animationFile/%s.plist",str->getCString());

        }
        if (type != 2 &&   type != 14 &&    type != 15 &&   type != 16 && type!=13)
        {

            //FIXME: it's not deleted
            ResourceManager::sharedInstance()->bindTexture( (char*) name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);

        }
        if (type == 13)
        {
            ResourceManager::sharedInstance()->bindTexture( "animationFile/dq.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
        }
        
        ResourceManager::sharedInstance()->bindTexture("animationFile/wjdsl.plist", RES_TYPE_BATTLE_IMAGE, sg_herolayer);
        
        
        CCString*  name1;
        if (type > 21)
        {
            name1 = CCString::createWithFormat("effects/%s.scml",str->getCString());
            
        }
        else
        {
            name1 = CCString::createWithFormat("animationFile/%s.scml",str->getCString());
            
        }


        CCSpriterX *effect = CCSpriterX::create(name1->getCString(), true, true);
        effect->setanimaID(0);
        effect->setisloop(isloop);
        bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        if (type == 0 ||
            type == 16)
        {
            effect->setPosition(ccp(screenwidth * 0.5, this->isme ? screenheight * 0.2 : screenheight - screenheight * 0.3));
        }
        else if(type == 23 || type == 22 )  //所有阵列消失
        {
            CCPoint effPos = GameConfig::getGridPoint(index.i, index.j, !round);
            if (round)
            {
                //effPos.y -= 0.5 * gridWidth;
                 effPos.y += gridHeight;
            }
            else
            {
                //effPos.x -= 0.5*gridWidth;
                effPos.y += gridHeight;
            }
            effect->setPosition(ccpAdd(effPos, difPos));
            
        }
        else
        {
            
            effect->setPosition(ccpAdd(GameConfig::getGridPoint(isme ? index.i: -(index.i + 1),   abs(isme ? -index.j :  (mapList - 1 - index.j)),true), ccp(0,round ? (i * gridHeight) : 0)));
            
        }
        if(scale != 1)
        effect->setScale(scale);
        effect->play();

        this->addChild(effect, CGP_EFFECT_ZORDER, TONGJING_EFFECT_TAG + flag);
        if (isRelease)
        {
            CC_SAFE_RETAIN(str);
            float a = (effect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM) + 1.8;
            CCDelayTime *time = CCDelayTime::create(a);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::removePlist), (void*)str);
            this->stopActionByTag(TONGJING_EFFECT_TAG-12);
            CCAction *act = CCSequence::create(time, call, NULL);
            act->setTag(TONGJING_EFFECT_TAG-12);
            this->runAction(act);
        }
    }
}

void SGHeroLayer::removePlist(CCObject *obj, CCString *name)
{
    CC_SAFE_RELEASE(name);
}

void SGHeroLayer::removeEffSpine(cocos2d::CCObject *psender, void *a)
{
    CCSkeletonAnimation*  sk = static_cast<CCSkeletonAnimation*>(psender);
    sk->removeFromParentAndCleanup(true);
}


//播放攻击中打中敌人的爆照（bz）特效
void SGHeroLayer::showJianBlastEffect(int type, bool isRelease, GameIndex index, bool isHome, bool isScale)
{
    CCString *str = NULL;
    CCString *str1 = NULL;
    switch (type) {
        case 0://火球
        {
            str = CCString::create("hq");
            str1 = CCString::create("hq");
        }
            break;
        case 1://音波溅射
        {
            str = CCString::create("ybjs");
            str1 = CCString::create("ybjs");
        }
            break;
        case 2://被攻击特效
        {
            str = CCString::create("jian_2_bz");
            str1 = CCString::create("jian_2_bz");
        }
            break;
        case 6://貂蝉主将计
        {
            str = CCString::create("jian_26");
            str1 = CCString::create("jian_26_bz");
        }
            break;
        case 7://贾诩主将计
        {
            str = CCString::create("jian_27");
            str1 = CCString::create("jian_27_bz");
        }
            break;
        case 8://刀波攻击
        {
            str = CCString::create("dbgj");
            str1 = CCString::create("dbgj");
        }
            break;
        case 9://大乔主将计
        {
            str = CCString::create("jian_9");
            str1 = CCString::create("jian_9_bz");
        }
            break;
        case 10://电墙
        {
            str = CCString::create("jian_10");
            str1 = CCString::create("jian_10_bz");
        }
            break;
        case 11://糜夫人攻击
        {
            str = CCString::create("jian_11");
            str1 = CCString::create("jian_11_bz");
        }
            break;
        case 13://司马懿主将计
        {
            str = CCString::create("jian_13");
            str1 = CCString::create("jian_13_bz");
        }
            break;
        case 15://诸葛亮攻击
        {
            str = CCString::create("jian_35");
            str1 = CCString::create("jian_35_bz");
        }
            break;
        case 16://小巧攻击
        {
            str = CCString::create("jian_16");
            str1 = CCString::create("jian_16");
        }
            break;
        case 17://徐庶攻击
        {
            str = CCString::create("jian_17");
            str1 = CCString::create("jian_17");
        }
            break;
        default:
        {
            if (type >= 100) {
                bool isme = true;
                if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
                {
                    isme = false;
                    //        heroLayer->setBulletSp(1);
                }
                str = CCString::createWithFormat("jian_%d",type);
                if (isme) {
                    if (isHome) {
                        str1 = CCString::createWithFormat("jian_%d_1_bz", type);
                    }else{
                        str1 = CCString::createWithFormat("jian_%d_bz", type);
                    }
                }else{
                    if (isHome) {
                        str1 = CCString::createWithFormat("jian_%d_1_bz_flip",type);
                    }else{
                        str1 = CCString::createWithFormat("jian_%d_bz_flip",type);
                    }
                }
            }else{
                return;
            }
        }
            break;
    }
    if (str && str1) {
        CCString *name = CCString::createWithFormat("animationFile/%s.plist",str->getCString());
        if (type < 100) {
            ResourceManager::sharedInstance()->bindTexture( name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
        }
        if (type >= 100) {
            CCString *name = CCString::createWithFormat("animationFile/%s_bz.plist",str->getCString());
            ResourceManager::sharedInstance()->bindTexture( name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
        }

        CCString *name1 = CCString::createWithFormat("animationFile/%s.scml",str1->getCString());
        CCSpriterX *effect = CCSpriterX::create(name1->getCString(), true, true);
        effect->setScale(isScale ? 0.5 : 1);
        effect->setanimaID(0);
        effect->setisloop(false);

        effect->setPosition(GameConfig::getGridPoint(isme ? index.i : -(index.i + 1),   abs(isme ? -index.j :  (mapList - 1 - index.j)),true));
        effect->play();
        
        this->addChild(effect, TONGJING_EFFECT_TAG, TONGJING_EFFECT_TAG);
        if (isRelease) {
            CC_SAFE_RETAIN(str);
            float a = (effect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM) + 1;
            CCDelayTime *time = CCDelayTime::create(a);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::removePlist), (void*)str);
            this->stopActionByTag(TONGJING_EFFECT_TAG-11);
            CCAction *act = CCSequence::create(time, call, NULL);
            act->setTag(TONGJING_EFFECT_TAG-11);
            this->runAction(act);
        }
    }
    
}

void SGHeroLayer::callMaskRemove()
{
    SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_Mask, true);
}

void SGHeroLayer::callMainSkillMaskRemove()
{
    SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_MainSkillMask, true);
    //battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_Dazhao, true);
}


//    //五彩的背景，有线在上
void SGHeroLayer::callDisplay1(CCNode* psender, T_mojiPos* mojiPos)
{

    CCSkeletonAnimation*  colorBg = CCSkeletonAnimation::createWithFile("effSpine/kaiyin.json", "effSpine/kaiyin.atlas", 1);
    colorBg->setAnimation("kaiyin1", false, 0);
    
    CCSkeletonAnimation*  wenzi = CCSkeletonAnimation::createWithFile("effSpine/wenzi.json", "effSpine/wenzi.atlas", 1);
    wenzi->setAnimation("wenzi1", false, 0);

    SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
    CCMoveBy*  moveby = NULL;
    if(mojiPos->isFlipX)   //反转， 从右边进来
    {
        colorBg->setScaleX(-1);
        colorBg->setPosition(ccp(960, mojiPos->ypos));
        wenzi->setPosition(ccp(340, mojiPos->ypos));
        moveby = CCMoveBy::create(1.0, ccp(-220, 0));
        
    }
    else
    {
        colorBg->setPosition(ccp(-320, mojiPos->ypos));
        wenzi->setPosition(ccp(300, mojiPos->ypos));
        moveby = CCMoveBy::create(1.0, ccp(220, 0));
    }
    wenzi->runAction(moveby);
    battleLayer->addChild(colorBg,  TOP_ZORDER + KJ_ColorBg, CGP_LIUGUANG_TAG + KJ_ColorBg);
    battleLayer->addChild(wenzi, TOP_ZORDER + KJ_Char, CGP_LIUGUANG_TAG + KJ_Char);
    
    
    free(mojiPos);
}

void SGHeroLayer::callBatHeroEffEnd(CCObject*  node, int*  mydata)
{
    SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_ColorBg, true);
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_AttEff, true);
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_NewHero, true);
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_head, true);
    battleLayer->removeChildByTag(CGP_LIUGUANG_TAG + KJ_Char, true);
}

static int indexOfEff = 0;
//武将发动技能的时候,中间出的横条,包括武将头像,技能名称,技能级数


void SGHeroLayer::showAttactHeadEffect(cocos2d::CCArray *array)
{
    //GPCCLOG("showAttackHeadEffect %d", indexOfEff++);
    EFFECT_PLAY(MUSIC_23);
    int y = 0;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    if (NULL == array)    //主将计
    {
        SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
        GPCCLOG("########大绝招来了###################\n");
        if (effZjjBright)
        {
             effZjjBright->removeFromParentAndCleanup(true);
            effZjjBright = NULL;
        }
       
        if(isme && effJiNeng)
        {
            effJiNeng->removeFromParentAndCleanup(true);
            effJiNeng = NULL;
        }
        
        
        if (dazhao == NULL)
        {
            dazhao = CCSkeletonAnimation::createWithFile("effSpine/dazhao.json", "effSpine/dazhao.atlas", 1);
            dazhao->setAnimation("animation", false);
            dazhao->setPosition(ccp(320, 640));
            battleLayer->addChild(dazhao, 10000);

        }
        else
        {
            dazhao->setAnimation("animation", false);
        }

        CCSprite *mask = CCSprite::createWithSpriteFrameName("zhezhaodian.png");
        mask->setScaleY(size.height / mask->getContentSize().height);
        mask->setScaleX(size.width / mask->getContentSize().width);
        mask->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        battleLayer->addChild(mask, TOP_ZORDER + KJ_MainSkillMask, CGP_LIUGUANG_TAG + KJ_MainSkillMask);
        CCFiniteTimeAction*  time001 = CCDelayTime::create(2);
        CCFiniteTimeAction*  call001 = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::callMainSkillMaskRemove));
        mask->runAction(CCSequence::create(time001, call001, NULL));
        
        
        
    }
    else     //武将计
    {
        if (array->count() == 1)
        {
            y = 0;
            wujiangjiCount = 1;
        }
        else if(array->count() == 2)
        {
            y = 200;
            wujiangjiCount = 2;
        }
        else
        {
            y = 400;
            wujiangjiCount = 3;
        }
        

        SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();

        for (int i = 0; i < array->count(); i++)
        {
            wujiangjiWhich = i;
            int yy = y - (i * 400) + size.height * 0.5 ;
            T_mojiPos   *stMojiPos  = (T_mojiPos*)malloc(sizeof(T_mojiPos));   //墨迹

            
            SGAttackList *attact = (SGAttackList*)array->objectAtIndex(i);
            
            int iFileId = attact->getFileId();
            int colorid = attact->getSbColor();
            char colorSufix[3] = {'r', 'y', 'b'};
            CCString*  jsonName  = CCString::createWithFormat("spine/wujiang_%03d_%c.json",iFileId,  colorSufix[colorid]);
            CCString* atlasName = CCString::createWithFormat("spine/wujiang_%03d.atlas",iFileId);
       
            CCSkeletonAnimation*   newHero = CCSkeletonAnimation::createWithFile(jsonName->getCString(), atlasName->getCString(), 0.15f);
            newHero->timeScale = 0.7;
            newHero->setAnimation(acActionNames[ACT_xuli], true);   //ready fight

           // SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound());
            
            // ID is everything
            SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(attact->getID());
            if (data == NULL)
            {
                return;
            }
            int id = data->getSkillHead();
            
            SGMainManager::shareMain()->addSkillPlistByNum(id, sg_herolayer);
            
            CCString *name = CCString::createWithFormat("skill_head_%d.png",id);
           // GPCCLOG("itemId : %d,      skill_head id : %d", hero->itemId,  id);
            
            
            CCSprite* head = CCSprite::createWithSpriteFrameName(name->getCString());
            
            //每一个武将动作
            if (head)
            {
                //head->retain();
                //留到最后释放
                // skillHeadVec.push_back(head);
                //added by cgp for cccliping test
                head->setPosition(ccp(-320, yy));
                CCFiniteTimeAction*  moveIn = CCEaseExponentialOut::create(CCMoveBy::create(1.0f, ccp(640, 0)));
                stMojiPos->ypos = yy;
                stMojiPos->isFlipX = false;
                
                //多个同时，反转一个
                if (1 == i) //默认只有3个， 中间的反转
                {
                    head->setPosition(ccp(960, yy));
                    moveIn = CCEaseExponentialOut::create(CCMoveBy::create(1.0f, ccp(-640, 0)));
                    head->setFlipX(true);
                    stMojiPos->isFlipX = true;
                }

                CCCallFunc *callCgp = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::callBatHeroEffEnd), (void*)&i);
                //显示
                CCCallFuncND* callColorBg = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::callDisplay1), (void*)stMojiPos);
              
                
                CCDelayTime*  originDelay = CCDelayTime::create(DELAY_TIME + ATTACK_GAP_TIME * i);
                CCDelayTime*  fadeoutDelay = CCDelayTime::create(1.0f);
                CCDelayTime*  removeDelay = CCDelayTime::create(0.2f);
                CCFadeOut*  fadeout1 = CCFadeOut::create(0.2f);
                head->runAction(CCSequence::create(originDelay, moveIn,  fadeoutDelay, fadeout1, removeDelay, callCgp, NULL));
                CCDelayTime*   allDelay = CCDelayTime::create(DELAY_TIME + ATTACK_GAP_TIME * i + 0.2);
                battleLayer->runAction(CCSequence::create(allDelay, callColorBg, NULL));
                //hero positon
                GameIndex aa =   attact->getAttackIndex();
                bool isMe = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
                if (isMe)
                {
                    aa.i = aa.i + 1;
                }
                
                CCPoint newHeroPos;
                if(attact->getAttackType() == kattackfour)
                {
                    newHeroPos =  GameConfig::getGridPoint(aa.i, aa.j , isMe);
                    newHeroPos.x += 0.5 * gridWidth;
                }
                else
                {
                    newHeroPos =  GameConfig::getGridPoint(aa.i, aa.j, isMe);
                }
                
                
                newHero->setPosition(newHeroPos);
                
                //特效, 蓄力球
                CCSkeletonAnimation*  attackStartEff = CCSkeletonAnimation::createWithFile( "effSpine/szg.json", "effSpine/szg.atlas");
                attackStartEff->setAnimation("animation", false);

                attackStartEff->setScale(1.6);
                attackStartEff->setPosition(ccp(newHeroPos.x, newHeroPos.y + gridHeight));
              
                battleLayer->addChild(newHero, TOP_ZORDER + KJ_NewHero, CGP_LIUGUANG_TAG + KJ_NewHero);
                battleLayer->addChild(attackStartEff, TOP_ZORDER + KJ_AttEff, CGP_LIUGUANG_TAG + KJ_AttEff);
                battleLayer->addChild(head, TOP_ZORDER + KJ_head, CGP_LIUGUANG_TAG + KJ_head);
                
            }
            
        }

        CCSprite *mask = CCSprite::createWithSpriteFrameName("zhezhaodian.png");
        mask->setScaleY(CCDirector::sharedDirector()->getWinSize().height / mask->getContentSize().height);
        mask->setScaleX(CCDirector::sharedDirector()->getWinSize().width / mask->getContentSize().width);
        mask->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        battleLayer->addChild(mask, TOP_ZORDER + KJ_Mask, CGP_LIUGUANG_TAG + KJ_Mask);
        CCFiniteTimeAction*  time001 = CCDelayTime::create(3.25 + wujiangjiCount * 0.25 );
        CCFiniteTimeAction*  call001 = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::callMaskRemove));
        mask->runAction(CCSequence::create(time001, call001, NULL));
        
    }
    
}

//removed by cgp for test
void SGHeroLayer::showAttactHeadEffect1(cocos2d::CCArray *array)
{

    EFFECT_PLAY(MUSIC_23);
    int y = 0;
    if (NULL == array)    //主将计
    {
        skillHead = NULL;
        y = screenheight * 0.5;

        CCSpriterX *effect = CCSpriterX::create("animationFile/wqlg2.scml", true, true);
        
        effect->setanimaID(0);
        effect->setisloop(false);
        effect->setPosition(ccp(screenwidth * 0.5, y));
        effect->setScaleX(screenwidth / 640);
        effect->play();
        
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound());
        
        SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(hero->itemId);
        if (data)
        {
            int id = data->getSkillHead();
            
            SGMainManager::shareMain()->addSkillPlistByNum(id, sg_herolayer);
            
            CCString *name = CCString::createWithFormat("skill_head_%d.png",id);
            skillHead = CCSprite::createWithSpriteFrameName(name->getCString());
            
            
            if (skillHead)
            {
                
                skillName = CCSprite::create();
                skillName->setContentSize(CCSize(screenwidth * 0.5, 160));
                skillName->setAnchorPoint(ccp(0.5f, 0));

                CCLabelTTF *label1_ = CCLabelTTF::create(m_skillName->getCString(), FONT_PANGWA, FONTSIZE(24));
                CCLabelTTF *label_ = CCLabelTTF::create(m_skillName->getCString(), FONT_PANGWA, FONTSIZE(24));
                label_->addChild(label1_);
                label_->enableStroke(ccc3(52, 0, 0), 4);
                label1_->setPosition(ccp(label1_->getContentSize().width * 0.5, label_->getContentSize().height * 0.52 + 2));
                label1_->setColor(ccc3(255, 192, 0));
                label_->setColor(ccc3(136, 0, 0));
                label_->setPosition(ccp(label_->getContentSize().width, skillName->getContentSize().height * 0.6f));
                
                CCString *skilL = CCString::createWithFormat("LV %d", m_skillLevel);
                CCLabelTTF *label1__ = CCLabelTTF::create(skilL->getCString(), FONT_PANGWA, FONTSIZE(20));
                CCLabelTTF *label__ = CCLabelTTF::create(skilL->getCString(), FONT_PANGWA, FONTSIZE(20));
                label__->addChild(label1__);
                label__->enableStroke(ccc3(52, 0, 0), 4);
                label1__->setPosition(ccp(label1__->getContentSize().width * 0.5, label__->getContentSize().height * 0.52 + 2));
                label1__->setColor(ccc3(255, 75, 4));
                label__->setColor(ccc3(52, 0, 0));
                label__->setPosition(ccp(label1_->getPosition().x + label_->getContentSize().width, skillName->getContentSize().height * 0.25f));

                skillName->addChild(label_, 0, 998);
                skillName->addChild(label__, 0, 999);
                SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(effect, LIUGUANG_EFFECT_TAG - 5, LIUGUANG_EFFECT_TAG - 5 );

                SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(skillHead, LIUGUANG_EFFECT_TAG, LIUGUANG_EFFECT_TAG );
                SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(skillName, LIUGUANG_EFFECT_TAG, LIUGUANG_EFFECT_TAG + 5 );
                skillHead->setAnchorPoint(ccp(0.5, 0));
                int yyy = y - 80;
                skillName->setPosition(ccp(screenwidth + skillName->getContentSize().width, yyy));
                skillHead->setPosition(ccp(-skillHead->getContentSize().width , yyy));
                CCMoveTo *move4 = CCMoveTo::create(0.8f, ccp(screenwidth * 0.4f, yyy));
                CCMoveTo *move = CCMoveTo::create(1.0f, ccp(screenwidth * 0.4f + 50, yyy));
                CCMoveTo *move1 = CCMoveTo::create(0.5f, ccp(-skillHead->getContentSize().width , yyy));
                CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::rmeovehst));
                CCAction *act = (CCAction*)CCSequence::create(move4, move, move1, call, NULL);
                skillHead->runAction(act);
                
                CCMoveTo *move_ = CCMoveTo::create(0.8f, ccp(screenwidth * 0.6f + 50, yyy));
                CCMoveTo *move_1 = CCMoveTo::create(1.0f, ccp(screenwidth * 0.6f, yyy));
                CCMoveTo *move_2 = CCMoveTo::create(0.5f, ccp(screenwidth + skillName->getContentSize().width, yyy));
                CCAction *act_ = (CCAction*) CCSequence::create(move_, move_1, move_2, NULL);
                skillName->runAction(act_);
                
                this->schedule(schedule_selector(SGHeroLayer::showHeadStreak), 0.03f);
            }
        }
    }
    else     //武将计
    {
        switch (array->count())
        {
            case 1:
            {
                y = screenheight * 0.5;
            }
                break;
            case 2:
            {
                y = screenheight * 0.5 + 121;
            }
                break;
            case 3:
            {
                y = screenheight * 0.5 + 200;
            }
                break;
            default:
                break;
        }

        for (int i = 0; i < array->count(); i++)
        {
            int yy = y - (i * 240);
            CCSpriterX *effect = CCSpriterX::create("animationFile/wqlg.scml", true, true);
            effect->setanimaID(0);
            effect->setisloop(false);
            effect->setPosition(ccp(screenwidth * 0.5, yy));
            
            effect->setScaleX(screenwidth / 640);
            
            float a = i * 0.5;
            CCHide *hide = CCHide::create();
            CCDelayTime *time = CCDelayTime::create(a);
            CCShow *show = CCShow::create();
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::playLuiGuanEffect));
            effect->runAction(CCSequence::create(hide, time, show, call, NULL));
            
            SGAttackList *attact = (SGAttackList*)array->objectAtIndex(i);
            int id = attact->getSkill_Head();
            
            SGMainManager::shareMain()->addSkillPlistByNum(id, sg_herolayer);

            CCString *name = CCString::createWithFormat("skill_head_%d.png",id);
            CCSprite *head = CCSprite::createWithSpriteFrameName(name->getCString());
            if (head)
            {
                head->retain();
                //留到最后释放
                skillHeadVec.push_back(head);
                
                CCSpriterX *effname = CCSpriterX::create("animationFile/gjf.scml", true, true);
                
                effname->setanimaID(0);
                effname->setisloop(false);
                effname->setPosition(ccp(screenwidth * 0.5, yy));
                effname->setScaleX(screenwidth / 640);
                
                CCHide *hide = CCHide::create();
                CCDelayTime *time = CCDelayTime::create(a);
                CCShow *show = CCShow::create();
                CCCallFuncN *_call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::playLuiGuanEffect));
                effname->runAction(CCSequence::create(hide, time, show, _call, NULL));
                
                if (i==0)
                {
                    skillHead = head;
                }
                else if(i == 1)
                {
                    skillHead1 = head;
                }
                else
                {
                    skillHead2 = head;
                }

                int yyy = yy - 80;

                SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG + i, true);
                SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(effect, LIUGUANG_EFFECT_TAG-1, LIUGUANG_EFFECT_TAG + i);
                SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(head, LIUGUANG_EFFECT_TAG, LIUGUANG_EFFECT_TAG + i);
                SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(effname, LIUGUANG_EFFECT_TAG, LIUGUANG_EFFECT_TAG + i);
                head->setAnchorPoint(ccp(0.5, 0));

                CCDelayTime *time9 = CCDelayTime::create(a + 0.1);
                head->setPosition(ccp(-head->getContentSize().width , yyy));
                CCMoveTo *move4 = CCMoveTo::create(0.8f, ccp(screenwidth * 0.4f, yyy));
                CCMoveTo *move = CCMoveTo::create(1.0f, ccp(screenwidth * 0.4f + 50, yyy));
                CCMoveTo *move1 = CCMoveTo::create(0.5f, ccp(-head->getContentSize().width , yyy));
                CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::rmeovehst));
                CCAction *act = (CCAction*)CCSequence::create( time9, move4, move, move1, ((i== array->count() - 1) ? call:NULL ), NULL);
                head->runAction(act);

                
                this->schedule(schedule_selector(SGHeroLayer::showHeadStreak1), 0.03f);
            }
            
        }
    }
}

void SGHeroLayer::showHeadStreak()
{
    SGAIManager::shareManager()->setIsDispSkillHead(true);
    if (skillHead) {
        CCSprite *s = CCSprite::createWithSpriteFrame(skillHead->displayFrame());
        s->setPosition(skillHead->getPosition());
        s->setAnchorPoint(ccp(0.5, 0));
        s->setOpacity(120);
        SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(s, LIUGUANG_EFFECT_TAG - 1, LIUGUANG_EFFECT_TAG - 2 );
        CCFadeOut * fade = CCFadeOut::create(0.2f);
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::rmeovehst1));
        s->runAction(CCSequence::create(fade, call, NULL));
        
        CCSprite *s_ = CCSprite::createWithSpriteFrame(skillName->displayFrame());
        s_->setPosition(skillName->getPosition());
        s_->setAnchorPoint(ccp(0.5f, 0));
        
        SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(s_, LIUGUANG_EFFECT_TAG - 1, LIUGUANG_EFFECT_TAG + 5 - 1 );
        CCFadeOut * fade_ = CCFadeOut::create(0.2f);
        CCFadeOut * fade__ = CCFadeOut::create(0.2f);
        CCFadeOut * fade___ = CCFadeOut::create(0.2f);
        CCDelayTime * time = CCDelayTime::create(0.2f);
        CCLabelTTF *label_ = (CCLabelTTF*)skillName->getChildByTag(998);
        CCLabelTTF *label__ = (CCLabelTTF*)skillName->getChildByTag(999);
        CCLabelTTF *__label = CCLabelTTF::create(m_skillName->getCString(), FONT_PANGWA, FONTSIZE(24));
        __label->setPosition(label_->getPosition());
        __label->setOpacity(120);
        __label->setColor(ccc3(255, 192, 0));

        CCString *skilL = CCString::createWithFormat("LV %d", m_skillLevel);
        CCLabelTTF *___label = CCLabelTTF::create(skilL->getCString(), FONT_PANGWA, FONTSIZE(20));
        ___label->setPosition(label__->getPosition());
        ___label->setOpacity(120);
        ___label->setColor(ccc3(255, 75, 4));

        s_->addChild(__label);
        s_->addChild(___label);
        __label->runAction(CCSequence::create(fade__, call, NULL));
        ___label->runAction(CCSequence::create(fade___, call, NULL));
        s_->runAction(CCSequence::create(time, call, NULL));
        
    }
}

void SGHeroLayer::rmeovehst1(cocos2d::CCObject *obj)
{
    CCNode *node = (CCNode *) obj;
    node->stopAllActions();
    node->removeFromParentAndCleanup(true);
    node = NULL;
}

void SGHeroLayer::rmeovehst()
{
    this->unschedule(schedule_selector(SGHeroLayer::showHeadStreak));
    this->unschedule(schedule_selector(SGHeroLayer::showHeadStreak1));
    if (skillHead)
    {
        skillHead->removeFromParentAndCleanup(true);
        skillHead = NULL;
    }
    if (skillName) {
        skillName->removeFromParentAndCleanup(true);
        skillName = NULL;
    }
    
    SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG - 5, true);
    SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG - 3, true);
    SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG, true);
    SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG + 5, true);
    SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG - 2, true);
    SGBattleManager::sharedBattle()->getHeroLayer(true)->removeChildByTag(LIUGUANG_EFFECT_TAG + 5 - 1, true);
}
// 攻击阵列开打(?)
void SGHeroLayer::showHeadStreak1()
{
    if (skillHead) {
        showHeadStreak2(skillHead, skillName, _skName, _skLevel);
    }
    if (skillHead1) {
        showHeadStreak2(skillHead1, skillName1, _skName1, _skLevel1);
    }
    if (skillHead2) {
        showHeadStreak2(skillHead2, skillName2, _skName2, _skLevel2);
    }
}


void SGHeroLayer::showHeadStreak2( CCSprite *head, CCSprite *skname,CCString *name, int level)
{
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::rmeovehst1));
    CCSprite *s = CCSprite::createWithSpriteFrame(head->displayFrame());
    s->setPosition(head->getPosition());
    s->setAnchorPoint(ccp(0.5, 0));
    s->setOpacity(120);
    SGBattleManager::sharedBattle()->getHeroLayer(true)->addChild(s, LIUGUANG_EFFECT_TAG - 1);
    CCFadeOut * fade = CCFadeOut::create(0.2f);
    s->runAction(CCSequence::create(fade, call, NULL));
}
//是播放绿光特效么(???LuiGuan)
void SGHeroLayer::playLuiGuanEffect(CCObject *obj)
{
    CCSpriterX *sp = (CCSpriterX*)obj;
    sp->play();

}

//战斗时跳动出来的数字,如攻击数值
void SGHeroLayer::showBattleLabel(int num, const char *name, int w, int h, GameIndex index, bool skill,float delayTime, float removeDelay)
{
    if (num == 0) return;
    
    if (num < 0) {
        num = -num;
    }
    CCString *strN = CCString::createWithFormat("%d", num);
    
    bool meRound = false;
    if (!skill)
    {
        if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            meRound = true;
        }
    }
    else
    {
        if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            meRound = true;
        }
    }
    
    
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
    CCLabelAtlas *label = CCLabelAtlas::create(strN->getCString(), name, w, h, '0');
    
    
    GameIndex jiumpLableIndex;
    jiumpLableIndex.i = meRound ? index.i/* + 1*/ : -(index.i+1);
    jiumpLableIndex.j = meRound ? index.j :  (mapList - 1 - index.j);
    
    for (int index =0; index < m_jumpLableIndexList.size(); index++)
    {
            if(m_jumpLableIndexList[index].i == jiumpLableIndex.i && m_jumpLableIndexList[index].j == jiumpLableIndex.j)
            {
                jiumpLableIndex.i += meRound ? (-1) : 1;
                break;
            }
    }

    m_jumpLableIndexList.push_back(jiumpLableIndex);
    
    CCPoint pos = GameConfig::getGridPoint(jiumpLableIndex.i, jiumpLableIndex.j,true);
    
    label->setPosition(ccpAdd(pos, ccp(0, skill ? 0 :  meRound ? 0: gridHeight * 1.5 )));
    
    label->setScale(0.1f);
    CCJumpTo *jumpBy1=CCJumpTo::create(0.3f, ccpAdd(label->getPosition(),ccp(gridWidth, 0)), gridHeight, 1);
    CCScaleTo *sc = CCScaleTo::create(0.1, 2.0f);
    CCScaleTo *sc1 = CCScaleTo::create(0.1, 1.0f);
    CCSpawn *spa = CCSpawn::create(sc,jumpBy1, NULL);

    CCJumpTo *jumpBy2=CCJumpTo::create(0.3f, ccpAdd(label->getPosition(),ccp(gridWidth*1.3, 0)), gridHeight*0.5, 1);

    CCJumpTo *jump1 = CCJumpTo::create(0.2f, ccp(label->getPosition().x + gridWidth * 1.5, label->getPosition().y), gridHeight*0.3 * 0.2, 1);
    CCFadeOut *fad = CCFadeOut::create(0.2f);
    CCSpawn *spawn1 = CCSpawn::createWithTwoActions(fad, jump1);
    CCDelayTime *delay = CCDelayTime::create(removeDelay);
    CCCallFuncN* call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::removeBalleLabel));
    CCAction *act = NULL;
    if (delayTime > 0) {
        CCDelayTime* delay1 = CCDelayTime::create(delayTime);
        CCFadeOut *fadIn = CCFadeOut::create(0.04f);
        label->setOpacity(0);
        act = (CCAction*)CCSequence::create(delay1,fadIn,spa,sc1,jumpBy2, spawn1, delay, call, NULL);
    }
    else
    {
        act = (CCAction*)CCSequence::create(spa,sc1,jumpBy2, spawn1, delay, call, NULL);
    }
  
    label->runAction(act);
    hero->addChild(label, ADD_AP_LAYER_TAG, ADD_AP_LAYER_TAG);

}

//去掉掉血数字
void SGHeroLayer::removeBalleLabel(CCObject *obj)
{
    CCLabelAtlas *label = (CCLabelAtlas*)obj;
    label->removeFromParentAndCleanup(true);
}

//城墙加血特效(绿色十字)
void SGHeroLayer::showAddApEff(bool round, CCPoint _pos)
{
    CCPoint pos = CCPointZero;
    CCPoint pos_ = CCPointZero;
    if (round) {
        pos = ccp(screenwidth * 0.5, 80);
        pos_ = ccp(screenwidth * 0.5, screenheight - 80);
    }else{
        pos = ccp(screenwidth * 0.5, screenheight - 80);
        pos_ = ccp(screenwidth * 0.5, 80);
    }
    int w = (screenwidth * 0.5f);
    int a = rand() % 3 + 2;
    for (int i = 0; i < a; i++) {
        int x = (rand() % w) - w*0.5f;
        CCSprite *sp = CCSprite::createWithSpriteFrameName("green.png");
        if (_pos.x <= 0 &&
            _pos.y <= 0) {
            sp->setPosition(pos_);
        }else
        {
            sp->setPosition(ccpAdd(_pos, ccp(0, round ? gridHeight * 2.f : -gridHeight * 2.f)));
        }
        
        this->addChild(sp, ADD_AP_LAYER_TAG);
        CCHide *hide = CCHide::create();
        CCDelayTime *delay = CCDelayTime::create(0.2f + (i * 0.4f));
        CCShow *show = CCShow::create();
        CCJumpTo *jump = CCJumpTo::create(0.4f, sp->getPosition(), round ? gridHeight : -gridHeight, 1);
        CCMoveTo *move = CCMoveTo::create(0.5f, ccp(pos.x + x, pos.y));
        CCDelayTime * del = CCDelayTime::create(0.2f);
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::rmeoveaddeff));
        sp->runAction(CCSequence::create(hide, delay, show, jump, move, del, call, NULL));
    }
}
//移除特效
void SGHeroLayer::rmeoveaddeff(CCObject *obj)
{
    CCNode *node = (CCNode*)obj;
    node->removeFromParentAndCleanup(true);
}

void SGHeroLayer::showMoraleLabel(int num, const char *name, int w, int h, bool skill)
{
    
    CCString *strN = CCString::createWithFormat("%d", num);
    
    CCPoint pos = ccp(0, 0);
    if (!skill) {
        pos = ccp(screenwidth * 0.5, 80);
        if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            pos = ccp(screenwidth * 0.5, screenheight - 80);
        }
    }else{
        pos = ccp(screenwidth * 0.5, screenheight - 80);
        if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            pos = ccp(screenwidth * 0.5, 80);
        }
    }
    
    
    
    CCLabelAtlas *label = CCLabelAtlas::create(strN->getCString(), name, w, h, '0');
    CCLog("打到底线的%d",num);
    label->setPosition(pos);
    label->setScale(0.1f);
    CCJumpTo *jump = CCJumpTo::create(0.3, ccp(label->getPosition().x + 40, label->getPosition().y), 50, 1);
    CCScaleTo *scaleTo=CCScaleTo::create(0.1,2.0f);
    CCScaleTo *scaleTo2=CCScaleTo::create(0.1, 1.0f);
    CCSpawn *spac1=CCSpawn::create(scaleTo,jump ,NULL);
    
    CCJumpTo *jump1 = CCJumpTo::create(0.3, ccp(label->getPosition().x + 60, label->getPosition().y), 25, 1);
    
    CCDelayTime *delay = CCDelayTime::create(1.);
    CCCallFuncN* call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::removeBalleLabel));
    CCAction *act = (CCAction*)CCSequence::create(spac1,scaleTo2,jump1, delay, call, NULL);
    
    label->runAction(act);
    this->addChild(label, 9090);
}

void SGHeroLayer::removeMoraleLabel(CCObject *obj)
{
    CCLabelAtlas *label = (CCLabelAtlas*)obj;
    label->removeFromParentAndCleanup(true);
}
//显示士兵血条
void SGHeroLayer::showSbXueLabel(cocos2d::CCLabelAtlas *label, ShiBing *sb)
{
    bool meRound = true;
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        meRound = false;
    }
    GameIndex index = sb->getSbindex();
    
     label->setPosition(GameConfig::getGridPoint(meRound ? index.i : -index.i,   abs(meRound ? -index.j :  (mapList - 1 - index.j)),true));
    this->addChild(label);
    
}

#pragma mark - ------ SGBoxDelegate  ------

void SGHeroLayer::setIsCanTouch(bool isTouch)
{
    bool isround = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();

    if (isround)
    {
        SGBaseLayer::setIsCanTouch(isTouch);
        
        SGHeroLayer *hero = this->getOpponentHeroLayer();
        hero->menu->setTouchEnabled(isTouch);
        
        if (hero->battleMap)
        {
            hero->battleMap->setTouchEnabled(!isme);
        }
        if (this->battleMap)
        {
            this->battleMap->setTouchEnabled(isTouch);
        }
    }

}

void SGHeroLayer::showBoxCall(CCObject *obj)
{
     SGBattleManager::sharedBattle()->startCountingForHelp();
    if(obj)
    {
        CCDictionary *dict = (CCDictionary*)obj;
        CCString *string = (CCString*)dict->objectForKey("key");
        if(string->isEqual(CCString::create("Friend")))
        {
            //友军上场
            friendArmyUp();
        }else if(string->isEqual(CCString::create("LeaderSkill")))
        {
            //主将计发动
            //我方手动发动主将技，这个时候不允许我方自动战斗
            SGAIManager::shareManager()->setIsCompleteSkill(false);
            CCLOG("setIsCompleteSkill || %d -- [%s -> %d]", SGAIManager::shareManager()->getIsCompleteSkill(), __FUNCTION__, __LINE__);

            if (!SGAIManager::shareManager()->isPVE)
            {
                if(_roundTime <= 0)
                    return;
            }
            mainSkillAttackHandler();
        }
    }
}
//显示复活弹窗
void SGHeroLayer::showRevivelLayer()
{
    SGBattleReviveLayer *reviveLayer = SGBattleReviveLayer::create(this, 0);
    SGMainManager::shareMain()->showBox(reviveLayer);
}

void SGHeroLayer::beginRoundTime()
{
    if (isInitPve || !isme)
    {
        return;
    }
    if (this->isBattleOver)
    {
        endRoundUpdataTime();
        return;
    }
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
    if (hero->isBattleOver)
    {
        endRoundUpdataTime();
        return;
    }
    
    bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (round)
    {
        endRoundUpdataTime();
        
        restartRoundUpdataTime();
        
        if (!m_RoundTimeT->isVisible())  m_RoundTimeT->setVisible(true);
        if (!tbj->isVisible()) tbj->setVisible(true);
        if (!roundTimeLabel->isVisible()) roundTimeLabel->setVisible(true);
        
    }
    
    
}

void SGHeroLayer::stateUpdateTime()
{
    if (isInitPve || !isme) {
        return;
    }
    updataRoundTime();
}

void SGHeroLayer::restartRoundUpdataTime()
{
    if (isInitPve || !isme) {
        return;
    }
   this->schedule(schedule_selector(SGHeroLayer::updataRoundTime), 1.);
}

void SGHeroLayer::endRoundUpdataTime()
{
    if (isInitPve || !isme) {
        return;
    }
    unschedule(schedule_selector(SGHeroLayer::updataRoundTime));
    
}

void SGHeroLayer::updataRoundTime()
{
    
    if (isInitPve || !isme) {
        return;
    }
    if (!SGAIManager::shareManager()->m_isVarifyActiviteAI)
    {
        return;
    }
    
    _roundTime--;
    if (_roundTime <= 0)
    {
        this->battleMap->setTouchInitSbOpacity();
        unschedule(schedule_selector(SGHeroLayer::updataRoundTime));
        bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        if (round)
        {
            {
                if (this->battleMap->m_touchState == eTouchNull)
                {
                    endRound();
                }
                else if (this->battleMap->m_touchState == eTouchRemoveIn || this->battleMap->m_touchState == eTouchFang)
                {
                    isRoundEnd = true;
                }
                else if (this->battleMap->m_touchState == eTouchRemove)
                {
                    this->battleMap->backRemoveSB();
                    endRound();
                }
                else if(this->battleMap->m_touchState == eTouchXuan || this->battleMap->m_touchState == eTouchMove)
                {
                    this->battleMap->removeReplaceSoldier();
                    this->battleMap->m_touchState = eTouchNull;
                    this->battleMap->setXuanSbPos();
                    endRound();
                }
                else if(this->battleMap->m_touchState == eTouchSkills) //技能选位状态
                {
                    //时间到后，选位型技能强制释放。
                    if (this->battleMap->m_touchState == eTouchSkills)
                    {

                        SGSkillManager::shareSkill()->sendSkillRequest(this->bulletJ);

                    }

                }
                else if (this->battleMap->m_touchState == eTouchSkillsFang) //技能施放中状态
                {
                    ;
                }
            }
        }
        
    }
    else
    {
//        _roundTime--;
    }
    setRoundTime(_roundTime);
    
}
void SGHeroLayer::delayChangeRound()
{
    if(SGAIManager::shareManager()->getIsCompleteSkill())
    {
        this->battleMap->m_touchState = eTouchNull;
        unschedule(schedule_selector(SGHeroLayer::delayChangeRound));
    }
}

void SGHeroLayer::setRoundTime(int Time)
{
    
    if (isInitPve || !isme) {
        return;
    }
    if (Time >= 0) {
        _roundTime = Time;
        if (_roundTime >= 200) {
            _roundTime = 200;
        }
        CCString* time = CCString::createWithFormat("%d",_roundTime);
        roundTimeLabel->setString(time->getCString());
    }else{
        unschedule(schedule_selector(SGHeroLayer::updataRoundTime));
        _roundTime = _initRoundTime;
        CCString* time = CCString::createWithFormat("%d",_roundTime);
        roundTimeLabel->setString(time->getCString());
        if (m_RoundTimeT->isVisible()) 
        m_RoundTimeT->setVisible(false);
        if (tbj->isVisible())
        tbj->setVisible(false);
        if (roundTimeLabel->isVisible())
        roundTimeLabel->setVisible(false);
    }
    
    if (_roundTime > _initRoundTime) {
        m_RoundTimeT->setScaleX(screenwidth / m_RoundTimeT->getContentSize().width);
    }
    else
    {
        float sc = screenwidth / m_RoundTimeT->getContentSize().width;
        float _sc = sc / _initRoundTime;
        float __sc = _roundTime * _sc;
        m_RoundTimeT->setScaleX(__sc);
    }
}

void SGHeroLayer::upDAtaControlRound(int rond)
{
    if (rond < 0)
	{
		_controlRoundNum = rond;// 为了5回合血量判定胜负即时显示,目前未知有何负作用,by bugcode
        return;
    }
    _controlRoundNum = rond;
}

int SGHeroLayer::getCntrolRoundNum()
{
    return _controlRoundNum;
}

//检查战斗掉落
void SGHeroLayer::chackBattleFieldDrop(cocos2d::CCArray *array)
{
    if (!array) {
        return;
    }
    EFFECT_PLAY(MUSIC_10);
    CCLOG("chackBattleFieldDrop");
    for (int i = 0; i < array->count(); i++) {
        ShiBing* sb = (ShiBing*) array->objectAtIndex(i);
        CCPoint sbpos = sb->getPosition();
        bool isshow = false;
        if (sb->getSbType() != ksbone) {
            CCLOG("sbx====%f,  sby====%f", sbpos.x, sbpos.y);
            int cradNum = SGBattleManager::sharedBattle()->getBattleLayer()->getBattleCardNum();
            if ( cradNum > 0) {
                isshow = true;
                this->showBattleFieldDrop(true, sbpos);
            }else{
                isshow = true;
                this->showBattleFieldDrop(false, sbpos, (rand() % 3) + 2);
            }
        }else{
            int a = rand() % 100;
            if (a <= 100 &&
                a > 90) {
                isshow = true;
                int cradNum = SGBattleManager::sharedBattle()->getBattleLayer()->getBattleCardNum();
                if ( cradNum > 0)
                this->showBattleFieldDrop(true, sbpos);
            }else {
                isshow = true;
                this->showBattleFieldDrop(false, sbpos);
            }
        }
        if (isshow) {
            CCSpriterX *sp = CCSpriterX::create("animationFile/battle_star.scml", true, true);
            sp->setAnchorPoint(ccp(0.5, 0));
            sp->setPosition(sbpos);
            this->addChild(sp);
            sp->setanimaID(0);
            sp->setisloop(false);
            sp->play();
        }
    }
    
}

//战斗结束
void SGHeroLayer::battleEnd(float t)
{
   
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
    CCDelayTime *time = CCDelayTime::create(t);
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::showBattleWin));
    CCAction *act = CCSequence::create(time, call, NULL);
    hero->runAction(act);
}

//显示战斗胜利
void SGHeroLayer::showBattleWin(cocos2d::CCObject *obj)
{
    CCPoint pos = ccp( screenwidth * 0.5, screenheight * 0.7);
    SGHeroLayer *heor = (SGHeroLayer *)obj;
    int cradNum = SGBattleManager::sharedBattle()->getBattleLayer()->getBattleCardNum();
    heor->showBattleFieldDrop(true, pos, cradNum, true);
    int coinNUm = ((rand() % 4)+4);
    heor->showBattleFieldDrop(false, pos, coinNUm, true);
    
    CCSpriterX *sp = CCSpriterX::create("animationFile/battle_star.scml", true, true);
    sp->setPosition(pos);
    this->addChild(sp);
    sp->setanimaID(0);
    sp->setisloop(false);
    sp->setScale(2);
    sp->play();
}

void SGHeroLayer::playBattleW(cocos2d::CCObject *obj)
{

}

//显示战斗掉落(蹦金币)
void SGHeroLayer::showBattleFieldDrop(bool isCard, cocos2d::CCPoint pos, int num, bool isWin)
{
    if (!this->isInitPve) return;
    CCLOG(">>>>>>>>>>>>>>>>>>>>>>");
    for (int i = 0; i < num; i++) {
        CCSpriterX *sp = NULL;
        if (isCard) {
            int cradNum = SGBattleManager::sharedBattle()->getBattleLayer()->getBattleCardNum();
            SGBattleManager::sharedBattle()->getBattleLayer()->setBattleCardNum(cradNum - 1);
            sp = CCSpriterX::create("animationFile/battle_cardok.scml", true, true);
            sp->setScale(0.6);
        }else{
            sp = CCSpriterX::create("animationFile/battle_coinicon.scml", true, true);
            sp->setScale(0.4);
        }
        
        sp->setAnchorPoint(ccp(0.5, 0));
        CCPoint pos1 = pos;
        sp->setPosition(pos);
        this->addChild(sp, BATTLE_FIEL_DROP_TAG);
        sp->setanimaID(0);
        sp->setisloop(true);
        sp->play();

        float playDelay = 0.;
        int x, w, y, h;
        if (isWin) {
            playDelay = 0.15;
            x = pos.x - screenwidth * 0.3;
            w = screenwidth * 0.6;
            y = pos.y - screenheight * 0.1;
            h = screenheight * 0.2;
        }else{
            x = pos.x - gridWidth;
            w = gridWidth * 2;
            y = pos.y;
            h = gridHeight;
        }
        int x1 = (rand() % w) + x;
        int y1 = (rand() % h) + y;
        pos1 = ccp(x1, y1);
        CCHide *hide = CCHide::create();
        CCDelayTime *hideT = CCDelayTime::create(i * playDelay);
        CCShow *show = CCShow::create();
        CCJumpTo *jumpt = CCJumpTo::create(0.3, pos1, gridHeight * 0.8, 1);
        
        CCDelayTime *delayt = CCDelayTime::create(1.);
        CCMoveTo *movet = CCMoveTo::create(0.8, headPos);
        CCScaleTo *scaleto = CCScaleTo::create(0.8, 0.0);
        CCSpawn *spawn = CCSpawn::createWithTwoActions(movet, scaleto);
        CCEaseSineOut *eso = CCEaseSineOut::create(spawn);
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGHeroLayer::rmeoveDrop));
        CCAction *seq = CCSequence::create(hide, hideT, show, jumpt, delayt, eso, call, NULL);
        sp->runAction(seq);
        
    }
}
//删除掉落的显示
void SGHeroLayer::rmeoveDrop(cocos2d::CCObject *obj)
{
    CCSpriterX *sp = (CCSpriterX*) obj;
    sp->removeFromParentAndCleanup(true);
}

void SGHeroLayer::repair()
{
    EFFECT_PLAY(MUSIC_BTN);
}

void SGHeroLayer::shakeAndWobble()
{
    //城门被击动画
//    gate_blood = NULL;
//    gate_blood = CCSpriterX::create(ANIM_WALL_BLINK, true);
//    if (gate_blood != NULL)
//    {
//        float gate_blood_offx = 0;
//        float gate_blood_offy = 4;
//        gate_blood->setanimaID(0);
//        gate_blood->setisloop(false);
//        gate_blood->setVisible(true);
//        gate_blood->setPosition( ccpAdd(motherPos, ccp(gate_blood_offx, gate_blood_offy)) );
//        gate_blood->setScale(uibg->getScaleX(), uibg->getScaleY());
//        this->addChild(gate_blood, 20);
//        gate_blood->play();
//    }
    //启动监听
    this->unscheduleUpdate();
    this->scheduleUpdate();
    isChainStarted = true;
    curChainTime = 0;

}

//生成相应SHAKE动画，这个指针指向小泽·玛利强。csx=动画对象，dir=方向，which哪个按钮
void SGHeroLayer::generateAnim(SGButton* sgb, bool dir, int which)
{
    sgb->stopActionByTag(119);
    
    
    CCPoint pos;
    if (1 == which)
    {
        pos = zjjBtnPos;
    }
    else if (2 == which)
    {
        pos = yjBtnPos;
    }
    else if (3 == which)
    {
        pos = xdlBtnPos;
    }
    else
    {
        pos = ybBtnPos;
    }
    
    sgb->setPosition(pos);
    
    float shake1 = -30;
    float shake2 = -5;
    float shake3 = 8;
    float shake4 = -2;
    float shake5 = -8;
    float shake6 = -4;
    float shake7 = 0;
    
    float time1 = 0.1;
    float time2 = 0.1;
    float time3 = 0.1;
    float time4 = 0.1;
    float time5 = 0.1;
    float time6 = 0.12;
    float time7 = 0.15;
    
    if (!dir)
    {
        shake1 = -shake1; shake2 = -shake2; shake3 = -shake3; shake4 = -shake4; shake5 = -shake5; shake6 = -shake6; shake7 = -shake7;
    }
    
    CCMoveTo *move1 = CCMoveTo::create(time1, ccpAdd(pos,ccp( shake1, 0)));
    CCMoveTo *move2 = CCMoveTo::create(time2, ccpAdd(pos,ccp( shake2, 0)));
    CCMoveTo *move3 = CCMoveTo::create(time3, ccpAdd(pos,ccp( shake3, 0)));
    CCMoveTo *move4 = CCMoveTo::create(time4, ccpAdd(pos,ccp( shake4, 0)));
    CCMoveTo *move5 = CCMoveTo::create(time5, ccpAdd(pos,ccp( shake5, 0)));
    CCMoveTo *move6 = CCMoveTo::create(time6, ccpAdd(pos,ccp( shake6, 0)));
    CCMoveTo *move7 = CCMoveTo::create(time7, ccpAdd(pos,ccp( shake7, 0)));
    //铭牌摇晃
    CCAction *act = (CCAction*)CCSequence::create( move1, move2, move3, move4, move5, move6, move7, NULL);
    act->setTag(119);
    sgb->runAction(act);
}

void SGHeroLayer::update(float delta)
{
    if (isChainStarted)
    {
        curChainTime += delta;
        if (curChainTime > MAX_CHAIN_TIME)
        {
            this->unscheduleUpdate();
            curChainTime = 0;
            isChainStarted = false;
        }
    }

}
//改变各种按钮的状态
void SGHeroLayer::changeButtonState(bool onoff, int which)
{
    //新手引导中不激活
    if (SGGuideManager::shareManager()->getCurrentGuideId() < 49)
    {
        return ;
    }
    
    if (1 == which) //主将计
    {
        if (onoff) //开启
        {
            //this->specialEffictBtn(zjj_btn, 1);
        }
        else //关闭
        {
//            zjj_btn->stopAllActions();
//            zjj_btn->setPosition(zjjBtnPos);
        }
        wuzjj->setVisible((m_skillsId <= 0));
        //无论如何设置进度
        CCLOG("isMe=%d, skillsRoundNum=%d, baseSkillRoundNum=%d", isme, skillsRoundNum, baseSkillRoundNum);
        float percent;
        if (baseSkillRoundNum == 0)
        {
            percent = 0;
        }
        else
        {
            percent = (float) (baseSkillRoundNum - skillsRoundNum) / (float) baseSkillRoundNum * 100;
        }
        if (!isme && percent == 100)
        {
            isEnemySkillReady = true;
        }
        
        if (!isme && percent == 0 && isEnemySkillReady == true)
        {
            isEnemySkillReady = false;
            SGBattleLayer *battleLayer = SGBattleManager::sharedBattle()->getBattleLayer();
            GPCCLOG("########大绝招来了###################\n");
            if (dazhao == NULL)
            {
                dazhao = CCSkeletonAnimation::createWithFile("effSpine/dazhao.json", "effSpine/dazhao.atlas", 1);
                dazhao->setAnimation("animation", false);
                dazhao->setPosition(ccp(320, 640));
                battleLayer->addChild(dazhao, 10000);
                
            }
            else
            {
                dazhao->setAnimation("animation", false);
            }
            
        }

        setProgressEffect(zjjProg, percent, MAX_BUTTON_TIME);
        
        GPCCLOG("skillsRoundNum: %d", skillsRoundNum);

        if (skillsRoundNum == 0)
        {

            

            if (isme)
            {
                effJiNeng = CCSkeletonAnimation::createWithFile("effSpine/ji.json", "effSpine/ji.atlas");
                effJiNeng->setAnimation("animation", true, 0);
                
                zjj_btn->addChild(effJiNeng, 7);
                effJiNeng->setPosition(ccp(70, 50));
                
                effZjjBright = CCSkeletonAnimation::createWithFile("effSpine/beiguang.json", "effSpine/beiguang.atlas");
                effZjjBright->setAnimation("animation", true, 0);
                effZjjBright->setPosition(ccp(42, 80));
                zjj_btn->addChild(effZjjBright, 7);
                
                zjj_btn->setEnabled(isme);   //我方开启点击
            }
            else
            {

            }
  
        }

    }
    else if (2 == which) //友军
    {
        if (onoff) //开启
        {
            friendNumLabel->setVisible(false);
        }
        else //关闭
        {
            yj_btn->stopAllActions();
            yj_btn->setPosition(yjBtnPos);
        }
        //无论如何设置进度
        CCLOG("isMe=%d, friendNum=%d, initFriendNum=%d", isme, friendNum, initFriendNum);
    }
}

//删除屏幕中间显示伤害
void SGHeroLayer::delayRemoveLJEff()
{
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
    if (m_ljBj)
    {
        hero->removeChild(m_ljBj, true);
        m_ljBj = NULL;
    }
    if (las)
    {
        hero->removeChild(las, true);
        las = NULL;
    }
}


//屏幕中间显示伤害
void SGHeroLayer::showLJEff(bool isHede ,int damage)
{
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
    if (!isHede)    //显示
    {
        m_LJNum += damage;
        if (NULL == m_ljBj)
        {
            m_ljBj = CCSprite::createWithSpriteFrameName("battle_change_bar_big_black.png");
            m_ljBj->setPosition(SGLayout::getPoint(kMiddleCenter));
            m_ljBj->setScale(0.6f);
            hero->addChild( m_ljBj, 9898);
        }
        if (las)
        {
            hero->removeChild(las, true);
            las = NULL;
        }
        las = CCSprite::create();
        las->setPosition(SGLayout::getPoint(kMiddleCenter));
        hero->addChild(las, 9898);
        CCString *num = CCString::createWithFormat("%d", m_LJNum);
        SGCCLabelTTF *ljLabel = SGCCLabelTTF::create(str_lianji_str1, FONT_PANGWA, FONTSIZE(26) , ccc3(196, 58, 0) ,ccc3(83, 16, 7) );
        SGCCLabelTTF *ljLabel1 = SGCCLabelTTF::create(num->getCString(), FONT_PANGWA, FONTSIZE(26) , ccc3(252, 143, 20) , ccc3(83, 16, 7));
        ljLabel->setPosition(ccp(ljLabel1->getContentSize().width * 0.5, 0));
        ljLabel1->setPosition(ccp(-ljLabel->getContentSize().width * 0.5, 0));
        las->addChild(ljLabel1);
        las->addChild(ljLabel);
        CCScaleTo *sct = CCScaleTo::create(0.05f, 1.50f);
        CCDelayTime *t = CCDelayTime::create(0.05f);
        CCScaleTo *sct1 = CCScaleTo::create(0.10f, 1.0f);
        las->runAction(CCSequence::create(sct, t, sct1, NULL));
        
        CCDelayTime* delay = CCDelayTime::create(1.0f);
        CCCallFunc*  call = CCCallFunc::create(this, callfunc_selector(SGHeroLayer::delayRemoveLJEff));
        this->runAction(CCSequence::create(delay, call, nullptr));
        
        
    }
    else    //掩藏
    {
        if (m_ljBj)
        {
            hero->removeChild(m_ljBj, true);
            m_ljBj = NULL;
        }
        if (las)
        {
            hero->removeChild(las, true);
            las = NULL;
        }
        
    }
  
}
//切换回合通知我
void SGHeroLayer::changeRoundRemindME(){

    SGAIManager::shareManager()->setIsCompleteAttack(true);
    
    SGHeroLayer *herolayer=(SGHeroLayer *)SGBattleManager::sharedBattle()->getBattleLayer()->getChildByTag(sg_enemyHeroLayerTag);
    SGHeroLayer *herolayer1=(SGHeroLayer *)SGBattleManager::sharedBattle()->getBattleLayer()->getChildByTag(sg_myHeroLayerTag);
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound() && !SGGuideManager::shareManager()->isGuide && this->isme && herolayer->h_morale >0) {
        
        herolayer->statusBarLayer->setVisible(true);
        CCPoint index= remindMePosition(herolayer1->getBattleMap());
        CCPoint p = GameConfig::getGridPoint(index.x, index.y);
        herolayer->statusBarLayer->setPosition(p.x- gridWidth * 0.5, 0);
        
        herolayer->gbSprite->setVisible(true);
        herolayer->gbSprite->cocos2d::CCNode::setPosition(p.x,p.y-gridHeight*0.5);
        
        CCRepeatForever *repeat=CCRepeatForever::create(CCBlink::create(1.0f, 2));
        herolayer->gbSprite->runAction(repeat);
        repeat->setTag(1008611);
        
        herolayer1->battleMap->hideFriendlyHelp();
    }
    

}

CCPoint SGHeroLayer::remindMePosition(SGBattleMap *map){
    
    for (int i=4; i>0; i--) {
        for (int j=3; j<6; j++) {
            SGGridBase *baseGrid=map->myGrids[i][j];
            SGGridBase *baseGrid1=map->myGrids[i-1][j];
            if (!baseGrid && baseGrid1) {
                return ccp(i-1, j);
            }
        }
        
    }
    
    return ccp(2,3);
}
 //设置按钮的位置,援军,主将计
void SGHeroLayer::setBtnPosition(CCObject *obj,int num){

    SGButton *button=(SGButton *)obj;
    
    if (num==1) {
        button->setPosition(zjjBtnPos);
        
    }else
    {
        button->setPosition(yjBtnPos);
    }
}
//技能的标签,友军的标签
void SGHeroLayer::specialEffictBtn(SGButton *button,int num){
    button->stopAllActions();
    float moveTime=0.3f;
    float delayTime=6.0f;
    int jumpTime=6;
    
    if (num==1) {

        
    }
    else
        friendNumLabel->setVisible(false);
//        friendNumLabel->setString("");
    
    CCMoveBy *move = CCMoveBy::create(moveTime,ccp(0,10));
    CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGHeroLayer::setBtnPosition), (void *)num);
    CCSequence *seq=static_cast<CCSequence *>(CCSequence::create(move,move->reverse(),call,NULL));
    
    CCRepeat *repeat=static_cast<CCRepeat *>(CCRepeat::create(seq,jumpTime));
   
    button->runAction(CCRepeatForever::create(static_cast<CCSequence *>(CCSequence::create(repeat,call,CCDelayTime::create(delayTime),NULL))));
    
}
