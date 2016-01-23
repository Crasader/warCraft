//
//  SGGeneralInfoLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-8.
//
//
#include "SGGeneralInfoLayer.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGButton.h"
#include "itemmodule.pb.h"
#include "SGPlayerInfo.h"
#include "cocos-ext.h"
#include "SGNoExpStrengBox.h"
#include "SGCantAdvanceBox.h"
#include "SGSkillInfoBox.h"
#include "SGSellBox.h"
#include "SGGeneralEquipBox.h"
#include "SGStaticDataManager.h"
#include "SGSkillDataModel.h"
#include "SGStaticDataManager.h"
#include "SGMenuLabel.h"
#include "SGGuideManager.h"
#include "SGStrengLvlUpBox.h"
#include "SGMainLayer.h"
#include "SGBattleScoreLayer.h"
#include "SGMyFriend.h"
#include "SGVisitLayer.h"
#include "SGAutoEquipBox.h"
#include "ResourceManager.h"
#include "SGBattleManager.h"
#include "SGTeamgroup.h"
#include "AutoEquip.pb.h"
#include "SGFateManager.h"
#include "SGFateBase.h"
#include "SGFateEffect.h"
#include "SGTouchLayer.h"
#include "SGFateInfoLayer.h"
#include "SGFindHelpLayer.h"
#include "SGGeneralinfoEffect.h"
#include "SGPiecesLayer.h"
#include "SGShopLayer.h"
#include "SGStorySectionLayer.h"
#include "SGCommonInfoBox.h"
#include "AppMacros.h"
#include "ShopModule.pb.h"
#include "SGShopLayer.h"
#include "PvpFight.pb.h"
#include "SGArrangeLayer.h"
#include "ArmyModule.pb.h"
#include "SGEmbattleLayer.h"
#include "SGRankCell.h"
#include "SGRankLayer.h"
#include "SGStringConfig.h"
#include "Invite.pb.h"
//#include "SGPiecesLayer.h"
#include "SGLootWiKiMainLayer.h"
#include "SGDevelopingOfficerLayer.h"
#include "Developing.pb.h"

//装备等级 label.的tag
#define EQUIP_LABEL_TAG 250001



//新增PLIST，包括属性图标、缘分详情、武将详情
#define GIL_PLIST_NEW_UI "sggeneralinfolayer/ofcNewUi.plist"
#define GIL_PLIST_SHOP_LAYER "sgshoplayer/sgshoplayer.plist"
//移动超过多少则切换
#define SLIDE_CHANGE_DISTANCE 70
//移动超过多少则开始移动
#define SLIDE_LIMIT 10
//调整位置动作速度
#define SLIDE_SPEED 800.0
//多少秒后清除无用的节点
#define TIME_TO_KILL 2
//ZORDER保证显示的修正
#define ZORDER_FIX 50
//箭头的移动间隔
#define ARROW_ROCKING_TIME 0.6
//箭头的移动距离
#define ARROW_ROCKING_DIST 15
//箭头的移动时间
#define ARROW_ROCKING_GOGO 0.2f

//“攻防士速统蓄格”的宏KEY
#define KEY_ATK "atk"
#define KEY_DEF "def"
#define KEY_MOR "mor"
#define KEY_SPD "spd"
#define KEY_GOV "gov"
#define KEY_RND "rnd"
#define KEY_AREA "area"

//武器/铠甲/配饰/坐骑的边框子TAG
#define TAG_FRAME_WUQI 71
#define TAG_FRAME_KAIJIA 72
#define TAG_FRAME_PEISHI 73
#define TAG_FRAME_ZUOJI 74
////武器/铠甲/配饰/坐骑的图标子TAG
//#define TAG_ICON_WUQI 81
//#define TAG_ICON_KAIJIA 82
//#define TAG_ICON_PEISHI 83
//#define TAG_ICON_ZUOJI 84
//武器/铠甲/配饰/坐骑的本身TAG
#define WUQI_TAG 9
#define KAIJIA_TAG 10
#define PEISHI_TAG 11
#define ZUOJI_TAG 12

//解保按钮，锁定
#define IMG_LOCK_ON "ofc_lock_on.png"
//锁定按钮，锁开
#define IMG_LOCK_OFF "ofc_lock_off.png"

//彩色按钮背景
#define IMG_BTN_ON "hotBtnBg.png"
//灰色按钮背景
#define IMG_BTN_OFF "hotBtnBgGray.png"

//按钮-转生编号
#define BTN_TAG_ADVANCE 1
//按钮-锁定/卖出编号
#define BTN_TAG_PROTECTED 2


//灰色按钮

//enterType宏, 以后加此页面enterType,在这加上宏定义
//=====================================================


//*******  我或别人 拥有的武将

//正常进入,军营武将
#define 		NORMAL_ENTER		0
//好友查看武将
#define FRIEND_CHECK_ENTER		1
//首页强化
#define HOME_STRENG_OFFICER		2
//首页主将栏进入
#define HOME_OFFICER_BAR		3
//搜索找人
#define SEARCH_FRIEND_ENTER		4
//黑名单查看武将
#define BLACKLIST_ENTER			5
//申请好友查看武将
#define FRIEND_REQUEST_ENTER	6
//强化主将完成后
#define STRENG_OFFICER_ENTER	7
//要转生的武将查看
#define WILL_ADVANCE_ENTER		8

//装备界面
#define EQUIP_ENTER_9			9
#define EQUIP_ENTER_10			10
#define EQUIP_ENTER_11			11
#define EQUIP_ENTER_12			12
//转生结果界面
#define ADVANCE_RESULT_ENTER	13

//布阵处查看
#define EMBATTLE_CHECK_ENTER	15
#define EMBATTLE_CHECK_ENTER_1	22
#define EMBATTLE_LORD_CHECK     500

//排行查看武将
#define RANK_LIST_ENTER			24



//*******   非拥有武将


//探访处查看武将
#define VISIT_RESULT_ENTER		14
//图鉴处查看
#define TUJIAN_ENTER			16

//转生处查看转生材料的武将
#define ADVANCE_NEED_ENTER		21

//领奖-登陆奖励
#define LOGIN_ENTER				25
//领奖-由邀请好友进入
#define INVITE_FRIEND           26

#define RANK_LIST_ENTER_X           27//扩展解决天梯排行喝聊天进入入口冲突
#define ARENA_LIST_ENTER           28//竞技场进来的
/******25之前的不再更改和增加,之后从25之后依次加********/

//好友帮助界面
#define FIND_HELP_ENTER			100
//展示界面
#define SHOW_OFFICER_ENTER		1111

//探访
#define VISIT_LAYER_ENTER		10002
#define VISIT_LAYER_ENTER_1		101

//10000 和 10001可能是强化过来,谁知道加上注释吧
#define STRENG_DIR_ENTER		10000
#define STRENG_DIR_ENTER_1		10001


//碎片召唤界面
#define SUMMON_OFFICER_ENTER	11000
//首页强化主将计
#define HOME_STRENG_ENTER		20000
#define TONGQUE_LAYER_ENTER     20001//从铜雀台的进来的

//=====================================================


CCArray* SGGeneralInfoLayer::m_myOfficerList = NULL;
int SGGeneralInfoLayer::emtype = -1;

SGGeneralInfoLayer::SGGeneralInfoLayer()
:_card(NULL)
,_before(NULL)
,itemtype(0)
,cardId(0)
,enterType(0)
, isback(false)
,isbreak(false)
,Direc(NULL)
,isFriendGeneral(false)
,m_isSupported(false)
,m_isEqpBkgReady(false)
,slideLayer(NULL)
,fateLayer(NULL)
,skillLayer(NULL)
,advanceBtn(NULL)
,strengBtn(NULL)
,plotID(-1)
,roldID(-1)
,storyID(-1)
,changeOfficerButton(NULL)
,setlordButton(NULL)
,offButton(NULL)
,lordback(NULL)
,speedValue(0)
,morValue(0)
,combatle(0)
,GovValue(0)
,isPlayOpenLockAnimation(false),
_currentRow(0),
_currentPage(0),
info(NULL),

isfromfirst(0),
leftTimes(0),
equipCard(NULL),
wuqi(NULL),
wuqilock(NULL),
kaijia(NULL),
kaijialock(NULL),
peishi(NULL),
peishilock(NULL),
zuoji(NULL),
zuojilock(NULL),
autoequip(NULL),
governing(NULL),
lock(NULL),
atkxiaoguo(NULL),
defxiaoguo(NULL),
roundxiaoguo(NULL),
speedxiaoguo(NULL),
lvlupAct(NULL),
frame1(NULL),
labelLevel(NULL),
labelMaxLevel(NULL),
labelAtk(NULL),
labelDef(NULL),
labelMorale(NULL),
labelSpeed(NULL),
labelGov(NULL),
labelRound(NULL),
labelZjj(NULL),
labelWjj(NULL),
labelCell(NULL),
m_leftObj(NULL),
m_leftX(0.0),
m_middleObj(NULL),
m_middleX(0.0),
m_rightObj(NULL),
m_rightX(0.0),
m_index(0),
m_beganX(0.0),
m_dist(0.0),
m_canMove(false),
m_isTouched(false),
m_arrowLeft(NULL),
m_arrowRight(NULL),
totalGov(0),
position(0),
availPos(0)
, m_showLev(NULL)
//, m_showExp(NULL)
, m_pieceIcon(NULL)
, m_pieceRateBg(NULL)
, m_pieceProgress(NULL)
, m_sourceBtn(NULL)
, m_pieceNum(NULL)
//, m_expText(NULL)
, m_levText(NULL)
, m_advanceCountLab(NULL)
, m_pieceHasNumLab(NULL)
, m_pieceNeedNumLab(NULL)
,officerCardlevelLimit(0)
, wuqiNotice(NULL)
, kaijiaNotice(NULL)
, peishiNotice(NULL)
, zuojiNotice(NULL)
, m_upMaxStarLab(NULL)
, m_strengNotice(NULL)
, m_devNotice(NULL)
{
    memset(m_officerBg, 0, sizeof(CCSprite*)*4);
}
SGGeneralInfoLayer::~SGGeneralInfoLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_AUTO_EQUIP);
    notification->removeObserver(this, MSG_SEASONWAR_CDLIST);

    notification->removeObserver(this, MSG_EMBATTLE_DISPATCH);
    notification->removeObserver(this, MSG_EMBATTLE_REMOVE);
    notification->removeObserver(this, MSG_EMBATTLE_SETLORD);
    notification->removeObserver(this,MSG_OFFICER_DEVELOPING);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_generalInfoLayer);

    if (_before)
        _before->release();
    
    if (Direc)
        Direc->release();
    
    //直接删除滑动列表，全是指针
    CC_SAFE_DELETE(m_myOfficerList);
    
    //    CC_SAFE_RELEASE(_card);
}
SGGeneralInfoLayer* SGGeneralInfoLayer::create(SGOfficerCard *card, int type, ERI *info, SGOfficerCard *officer,int isfirst,CCDictionary *dic,bool isFriendGeneral,int currentPage,int currentRow)
{
    
    SGGeneralInfoLayer *generalLayer = new SGGeneralInfoLayer();
	generalLayer->info = info;//返回布阵界面所需要的信息
    generalLayer->isfromfirst = isfirst;
    if(type ==911)
    {
        generalLayer->isPlayOpenLockAnimation = true;
        type = 0;
    }
    generalLayer->enterType = type;
    generalLayer->_before = officer;
    generalLayer->_card = card;
    generalLayer->_card->retain();
    generalLayer->Direc = dic;
    generalLayer->isFriendGeneral = isFriendGeneral;
    generalLayer->_currentRow=currentRow;
    generalLayer->_currentPage=currentPage;
    
    if (generalLayer && generalLayer->init(NULL, sg_generalInfoLayer))
    {
        generalLayer->initView();
        generalLayer->autorelease();
        return generalLayer;
    }
    CC_SAFE_DELETE(generalLayer);
    return NULL;
}

/**
 * 生成一个武将原画LAYER
 */
CCSprite* SGGeneralInfoLayer::generateData(int index)
{
    if (!m_myOfficerList || index < 0 || index >= m_myOfficerList->count())
        return NULL;
    SGOfficerCard* sgoc = (SGOfficerCard*) m_myOfficerList->objectAtIndex(index);
    if (sgoc == NULL)
        return NULL;
    //试图生成头像
    CCString *str=NULL;
    //非武将的图像不同，但是现在应该不可能走到这里了。
    if (sgoc->getItemType() >=7)
    {
        CCLOG("@WARNING: non-hero-card");
		//12.05
		SGMainManager::shareMain()->addHeadIconPlistByNum(sgoc->getHeadSuffixNum(), sg_generalInfoLayer);
        str = CCString::createWithFormat("head%d.png",sgoc->getHeadSuffixNum());
    }
    else
    {
		//12.06
		SGMainManager::shareMain()->addOfficerPng(sgoc->getHeadSuffixNum(), sg_generalInfoLayer);
        str = CCString::createWithFormat("officer_%d.png",sgoc->getHeadSuffixNum());
    }
     CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    if (sgoc->getAdNum()>0) {
       CCSize s = CCDirector::sharedDirector()->getWinSize();
//        CCString *strnum = CCString::createWithFormat("gener+%d.png",sgoc->getAdNum());
        SGCCLabelTTF * advancenum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",sgoc->getAdNum())->getCString(), FONT_PANGWA, 32 , ccGREEN);
        advancenum->setAnchorPoint(ccp(.5,0));
        advancenum->setPosition(ccpAdd(ccp(item->getContentSize().width/2, item->getContentSize().height/2),ccp(s.width*0.2 ,item->getContentSize().height*0.2)));
        //    advancenum->setOffset(2);
        item->addChild(advancenum);

    }
    item->setAnchorPoint(ccp(0.5,0));
    return item;
}
void SGGeneralInfoLayer::PlayOpenLockAnimation()
{  isPlayOpenLockAnimation = false;
    int nAdNum =  _card->getAdNum();
    int nCurrStar = _card->getCurrStar();
    if(nCurrStar==3)
    {
        CCSpriterX *unlock = CCSpriterX::create("animationFile/unlockAnimation.scml", true, true);
        unlock->setanimaID(0);
        unlock->setisloop(false);
        addChild(unlock, 10, 0);
        if(nCurrStar == 3&&nAdNum==0)
            unlock->setPosition(wuqi->getPosition());
        if(nCurrStar == 3&&nAdNum==1)
            unlock->setPosition(kaijia->getPosition());
        if(nCurrStar == 3&&nAdNum==2)
            unlock->setPosition(peishi->getPosition());
        if(nCurrStar == 3&&nAdNum==3)
            unlock->setPosition(zuoji->getPosition());
        unlock->play();
    }
    
}

/**
 * 清理一些数据成员。
 */
void SGGeneralInfoLayer::cleanParams()
{
    m_middleObj = m_leftObj = m_rightObj = NULL;
    m_index = -1;
    m_canMove = false;
    m_isTouched = false;
    m_canTouched = true;
    m_beganX= 0;
    m_dist = 0;
}

void SGGeneralInfoLayer::initData()
{
}


void SGGeneralInfoLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer); //提供横条图
	ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer2.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer); //提供箭头//
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer); //提供箭头//
    
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer); //提供箭头//
	ResourceManager::sharedInstance()->bindTexture(GIL_PLIST_NEW_UI, RES_TYPE_LAYER_UI, sg_generalInfoLayer); //新图片
    
    ResourceManager::sharedInstance()->bindTexture("animationFile/unlockanimation.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("ofc_frameBlue/ofc_frameBlue.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    
    ResourceManager::sharedInstance()->bindTexture("tran/tran.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_AUTO_EQUIP,
                              this,
                              callfuncO_selector(SGGeneralInfoLayer::listener));
    notification->addObserver(MSG_SEASONWAR_CDLIST, this, callfuncO_selector(SGGeneralInfoLayer::showChgScoreLayer));
    

    notification->addObserver(MSG_EMBATTLE_DISPATCH,
                              this,
                              callfuncO_selector(SGGeneralInfoLayer::setMainOfficerListener));
    notification->addObserver(MSG_EMBATTLE_REMOVE,
                              this,
                              callfuncO_selector(SGGeneralInfoLayer::offMainOfficerListener));
    notification->addObserver(MSG_EMBATTLE_SETLORD,
                              this,
                              callfuncO_selector(SGGeneralInfoLayer::setLordOfficerListener));
    notification->addObserver(MSG_OFFICER_DEVELOPING, this, callfuncO_selector(SGGeneralInfoLayer::onDevelopingReponse));

    officerCardlevelLimit = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    
    CCString *str_ = CCString::create(_card->getClassName());
    int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
    int cardMaxExp =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit+1)->getexpAll();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();
    float  hgt = skewingY(210);

    //********************************************新版武将详情 分区域展示不同的信息
    
    //标题区域： 标题 返回按钮  ***begin
    //设置标题（即武将名称）
    this->setTitle(_card->getOfficerName()->getCString() , true ,  _card->getCurrStar());
    
    //小转生次数
    int advanceCount = _card->getAdNum();
    m_advanceCountLab = SGCCLabelTTF::create(CCString::createWithFormat("+%d" , advanceCount )->getCString() , FONT_PANGWA , 36 , ccGREEN);
    m_advanceCountLab->setPosition(ccp( labelTitle->getPosition().x + labelTitle->getContentSize().width*0.5 + m_advanceCountLab->getContentSize().width * 0.8, labelTitle->getPosition().y ));
    this->addChild(m_advanceCountLab);
    if(0 == advanceCount)
        m_advanceCountLab->setVisible(false);
    
    //返回按钮
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGGeneralInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    this->addBtn(backBtn);
    
    //标题区域  ***end.
    
    
    
    
	// 形象展示区 ：星级展示 战斗力 、信息区、形象展示、换人按钮、转生按钮  ********begin
    
    //星星
    this->setstar(_card->getCurrStar(), _card->getUpgradestar());  //cgpStar
    //战斗力
    SGStaticDataManager * dataManage = SGStaticDataManager::shareStatic();
    int combat=0;
    int grid = dataManage->getOfficerById(_card->getItemId())->getArea();
    int officerCombat = dataManage->getBaseCombatValue(6, _card->getCurrLevel(), _card->getCurrStar(), _card->getAdNum());
    int skillCombat = dataManage->getSkillCombatValue(grid, _card->getLordId(), _card->getOfficerSkil());
    combat = officerCombat + skillCombat;
    //计算装备战力加成
    if(_card->getOfficerEquips())
    {
        for(int i=0; i<_card->getOfficerEquips()->count() ; ++i)
        {
            SGEquipCard * temp = (SGEquipCard*)_card->getOfficerEquips()->objectAtIndex(i);
            if(temp)
                combat += dataManage->getBaseCombatValue(7, temp->getCurrLevel(), temp->getCurrStar(), temp->getAdNum());
        }

    }
        this->setOfficerFightStrength(combat);
    
    //头像
    SGMainManager::shareMain()->addOfficerPng(_card->getHeadSuffixNum(), sg_generalInfoLayer);
    CCString *str = CCString::createWithFormat("officer_%d.png",_card->getHeadSuffixNum());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    item->setAnchorPoint(ccp(0.5,0));
    //在 是否开启武将滑动处addChild
    //this->addChild(item);

    
    CCPoint fermPos1 =  ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -368 + 26));
    CCPoint fermPos2 =  ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -368 + 26));
    
    CCSprite *bigbg = NULL;
    float showPosY = 0.0;
    if (s.height == 960)
    {
        frameSize = CCRectMake(0, 0 , 768, 466);
        bigbg = CCSprite::createWithSpriteFrameName("pad武将底图.png");
        this->addChild(bigbg,-100);
        float rate = (float)768/s.width;
        bigbg->setScale(s.width/768);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - frameSize.size.height/2/rate)));

        item->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,150)));
        
        showPosY = bigbg->getPositionY() - bigbg->getContentSize().height*0.5 + 80/466.0 * bigbg->getContentSize().height;
    }
    else if (s.height == 1136)
    {
        bigbg = CCSprite::createWithSpriteFrameName("generalcardbg_ip5.png");
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height/2 + 44)));

        item->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,0)));
        
        showPosY = bigbg->getPositionY() - bigbg->getContentSize().height*0.5 + 110/466.0 * bigbg->getContentSize().height;
    }
    else
    {
        bigbg = CCSprite::createWithSpriteFrameName("pad武将底图.png");
        bigbg->setScaleY(1.1);
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height*.45)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,155)));
        
        showPosY = bigbg->getPositionY() - bigbg->getContentSize().height*0.5 + 80/466.0 * bigbg->getContentSize().height;
    }
    
    //展示按钮
    showButton = SGButton::create("show_objects.png", NULL, this, menu_selector(SGGeneralInfoLayer::showHandler), CCPointZero, false, true);
    this->addBtn(showButton);

    
    //换人按钮
    changeOfficerButton = SGButton::create("changeOfficerBtn.png", NULL, this, menu_selector(SGGeneralInfoLayer::changeHandler), CCPointZero, false, true);

    this->addBtn(changeOfficerButton);
    changeOfficerButton->setVisible(false);
    
    if( enterType==EMBATTLE_CHECK_ENTER_1 || HOME_OFFICER_BAR == enterType)
    {
        changeOfficerButton->setVisible(true);
    }

    //处理滑动，详见enterType注释
    cleanParams();
    if (enterType == NORMAL_ENTER || enterType == 3 || enterType == EMBATTLE_CHECK_ENTER || enterType==EMBATTLE_CHECK_ENTER_1) //三个支持滑动的入口
    {
		
        if ( m_myOfficerList && m_myOfficerList->count() > 0 )
        {
            m_isSupported = true; //开启滑动
            //初始化滑动支持控件！
            float width = 600;
            float height = 500;
            slideLayer = SGTouchLayer::create(width, height, 0 /* , ccc4(0, 255, 0, 255) */);
            slideLayer->ignoreAnchorPointForPosition(false);
            slideLayer->setAnchorPoint(ccp(0.5, 0)); //保持平齐
            slideLayer->setControlMode(this, pf_began_selector(SGGeneralInfoLayer::helloBegan), pf_moved_selector(SGGeneralInfoLayer::helloMoved), pf_ended_selector(SGGeneralInfoLayer::helloEnded), pf_cancelled_selector(SGGeneralInfoLayer::helloCancelled));
            this->addChild(slideLayer, 110);
            slideLayer->setPosition(item->getPosition());
            //预置滑动相关材料
            CCObject* pObj = NULL;
            SGOfficerCard* sgoc = NULL;
            int cur = 0; //索引
            CCARRAY_FOREACH(m_myOfficerList, pObj)
            {
                sgoc = (SGOfficerCard*) pObj;
                if (sgoc != NULL)
                {
                    if (sgoc->getSsid() == _card->getSsid()) //如果与本页面要展示的武将ID相等，则设置索引相关
                    {
                        m_index = cur;
                        break;
                    }
                }
                cur++;
            }
            //获得子页面标准位置
            m_middlePos = item->getPosition();
            m_leftPos = ccpAdd(m_middlePos, ccp(-screenwidth, 0));
            m_rightPos = ccpAdd(m_middlePos, ccp(screenwidth, 0));
            
            //设置当前展示武将
            m_middleObj = item;
            CCLOG(">>>>武将详情：自己的武将----");
            //设置左右武将
            int left = m_index - 1;
            int right = m_index + 1;
            m_leftObj = generateData(left);
            m_rightObj = generateData(right);
            
            //调整坐标
            if (m_middleObj != NULL)
            {
                m_middleObj->setPosition(m_middlePos);
                this->addChild(m_middleObj, ZORDER_FIX + m_index);
            }
            if (m_leftObj != NULL)
            {
                m_leftObj->setPosition(m_leftPos);
                this->addChild(m_leftObj, ZORDER_FIX + m_index - 1);
            }
            if (m_rightObj != NULL)
            {
                m_rightObj->setPosition(m_rightPos);
                this->addChild(m_rightObj, ZORDER_FIX + m_index + 1);
            }
            
            //左箭头
            m_arrowLeft = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
            
            CCPoint posLeft = ccpAdd(fermPos1, ccp(-42, 0));
            CCPoint posLToR = ccpAdd(posLeft, ccp(ARROW_ROCKING_DIST, 0));
            CCPoint posLToL = posLeft;
            
            m_arrowLeft->setPosition(posLeft);
            m_arrowLeft->setVisible(false);
            
            CCRepeat* actLeft = CCRepeat::create( CCSequence::create( CCMoveTo::create(0, posLToR), CCDelayTime::create(ARROW_ROCKING_TIME), CCMoveTo::create(0, posLToL), CCDelayTime::create(ARROW_ROCKING_TIME), NULL), 99999999 );
            m_arrowLeft->runAction(actLeft);
            
            this->addChild(m_arrowLeft, 100, 200);
            
            //右箭头
            m_arrowRight = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
            m_arrowRight->setFlipX(true);
            
            CCPoint posRight = ccpAdd(fermPos2, ccp(42, 0));
            CCPoint posRToL = ccpAdd(posRight, ccp(-ARROW_ROCKING_DIST, 0));
            CCPoint posRToR = posRight;
            
            m_arrowRight->setPosition(posRight);
            m_arrowRight->setVisible(false);
            
            CCRepeat* actRight = CCRepeat::create( CCSequence::create( CCMoveTo::create(0, posRToL), CCDelayTime::create(ARROW_ROCKING_TIME), CCMoveTo::create(0, posRToR), CCDelayTime::create(ARROW_ROCKING_TIME), NULL), 99999999 );
            m_arrowRight->runAction(actRight);
            
            this->addChild(m_arrowRight, 100, 201);
            
            //调整箭头
            tuneArrows();
        }
    }
    
    //如果非滑动级入口，则直接置为中间显示
    if (!m_middleObj)
    {
        m_isSupported = false;
        //设置当前展示武将
        m_middleObj = item;
        m_middlePos = item->getPosition();
        this->addChild(m_middleObj, ZORDER_FIX);
    }
    
    //<<<<处理滑动列表信息END
    
    

    
    
	//适配ip5
	int shift_y = 0;
	CCSize s1 = CCEGLView::sharedOpenGLView()->getFrameSize();
//	if (CCEGLView::sharedOpenGLView()->getFrameSize().height == 1136)
    if(s.height == 1136)
	{
		shift_y = -70;
	}
	
    //四按钮背景，作为基准，需要适配分辨率
    CCSprite *bg3 = CCSprite::createWithSpriteFrameName("wqzbbg.png");
    bg3->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, shift_y + hgt + 22 + bg3->getContentSize().height/2)));
    this->addChild(bg3,5,-100);

    
    float kuangwid = skewing(5);
    menu->setZOrder(9);
    
    //四大装备按钮+自动换装按钮
    wuqi = SGButton::create("equip_dao.png",
                            NULL,
                            this,
                            menu_selector(SGGeneralInfoLayer::setEquipment),
                            ccp(0, 0),
                            false,
                            true);
    //装备转生第四步，武将器按钮高亮
    int equipTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_EUIQP_ADVANCED, 4);
    wuqi->setTag(equipTag);
    
    wuqiNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    wuqiNotice->setPosition(ccpAdd(wuqi->getContentSize(), ccp(-5, -5)));
    wuqiNotice->setVisible(false);
    wuqi->addChild(wuqiNotice ,1919);
    
    wuqi->setTag(9);
    kaijia = SGButton::create("equip_kj.png",
                              NULL,
                              this,
                              menu_selector(SGGeneralInfoLayer::setEquipment),
                              ccp(0, 0),
                              false,
                              true);
    kaijia->setTag(10);
    
    kaijiaNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    kaijiaNotice->setPosition(ccpAdd(kaijia->getContentSize(), ccp(-5, -5)));
    kaijiaNotice->setVisible(false);
    kaijia->addChild(kaijiaNotice , 1919);
    
    peishi = SGButton::create("equip_jz.png",
                              NULL,
                              this,
                              menu_selector(SGGeneralInfoLayer::setEquipment),
                              ccp(0, 0),
                              false,
                              true);
    peishi->setTag(11);
    
    peishiNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    peishiNotice->setPosition(ccpAdd(peishi->getContentSize(), ccp(-5, -5)));
    peishiNotice->setVisible(false);
    peishi->addChild(peishiNotice , 1919);
    
    zuoji = SGButton::create("equip_ma.png",
                             NULL,
                             this,
                             menu_selector(SGGeneralInfoLayer::setEquipment),
                             ccp(0, 0),
                             false,
                             true);
    
    
    zuoji->setTag(12);
    
    zuojiNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    zuojiNotice->setPosition(ccpAdd(zuoji->getContentSize(), ccp(-5, -5)));
    zuojiNotice->setVisible(false);
    zuoji->addChild(zuojiNotice , 1919);
    
    
    autoequip =SGButton::create("ofc_autoequip.png",NULL,this,
                                menu_selector(SGGeneralInfoLayer::autoequipfunc),      
                                ccp(0, 0),
                                false,
                                true);
    
    
    
    
    
    autoequip->setTag(13);
    float adjustOffX = 0;
#define ScaleNum 0.9  //cgpUI
    wuqi->setPosition(ccpAdd(bg3->getPosition(), ccp(- wuqi->getContentSize().width*1.5 * ScaleNum - kuangwid*3-adjustOffX, 0)));
    kaijia->setPosition(ccpAdd(bg3->getPosition(), ccp( - wuqi->getContentSize().width*0.5* ScaleNum - kuangwid-adjustOffX, 0)));
    peishi->setPosition(ccpAdd(bg3->getPosition(), ccp(  wuqi->getContentSize().width*0.5* ScaleNum + kuangwid-adjustOffX, 0)));
    zuoji->setPosition(ccpAdd(bg3->getPosition(), ccp( wuqi->getContentSize().width*1.5* ScaleNum + kuangwid*3-adjustOffX,0)));
    autoequip->setPosition( ccpAdd(bg3->getPosition(), ccp(wuqi->getContentSize().width * 2.5* ScaleNum + kuangwid*5- adjustOffX, -2)) );
    changeOfficerButton->setPosition(ccpAdd(bg3->getPosition(), ccp(-wuqi->getContentSize().width * 2.5* ScaleNum - kuangwid * 5 - adjustOffX, 0)));
    
    
    this->addBtn(wuqi);
    this->addBtn(kaijia);
    this->addBtn(peishi);
    this->addBtn(zuoji);
    this->addBtn(autoequip);
    wuqilock = CCSprite::createWithSpriteFrameName("suosuo_0.png");
    wuqilock->setAnchorPoint(ccp(.5,.5));
    wuqilock->setPosition(ccp(wuqi->getContentSize().width/2,wuqi->getContentSize().height/2));
    
    kaijialock = CCSprite::createWithSpriteFrameName("suosuo_0.png");
    kaijialock->setPosition(ccp(wuqi->getContentSize().width/2,wuqi->getContentSize().height/2));
    kaijialock->setAnchorPoint(ccp(.5,.5));
    zuojilock = CCSprite::createWithSpriteFrameName("suosuo_0.png");
    zuojilock->setPosition(ccp(wuqi->getContentSize().width/2,wuqi->getContentSize().height/2));
    
    peishilock = CCSprite::createWithSpriteFrameName("suosuo_0.png");
    peishilock->setPosition(ccp(wuqi->getContentSize().width/2,wuqi->getContentSize().height/2));
    wuqi->addChild(wuqilock,100);
    kaijia->addChild(kaijialock,100);
    zuoji->addChild(zuojilock,100);
    peishi->addChild(peishilock,100);
    
    //三小框依附的大框
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCScale9Sprite *bigFrm = CCScale9Sprite::createWithSpriteFrameName("tran.png");
    bigFrm->setPreferredSize(CCSizeMake(winSize.width - 50, bg3->getPositionY() - bg3->getContentSize().height * 0.5 - 120));
    this->addChild(bigFrm,7);

    bigFrm->setPosition(ccp(bg3->getPositionX() ,112 + bigFrm->getContentSize().height * 0.5));
    
    CCSprite *bigFrmBg = CCSprite::createWithSpriteFrameName("tran.png");
    this->addChild(bigFrmBg,6);
    bigFrmBg->setScaleX(bigFrm->getContentSize().width / bigFrmBg->getContentSize().width);
    bigFrmBg->setScaleY(bigFrm->getContentSize().height / bigFrmBg->getContentSize().height);
    bigFrmBg->setPosition(bigFrm->getPosition());
    
    CCSprite*  bgRealBottom = CCSprite::createWithSpriteFrameName("ofc_frameBlue.png");
    bgRealBottom->setPosition(ccp(bigFrm->getPosition().x, bigFrm->getPosition().y + 78));
    this->addChild(bgRealBottom, -2);  //在主页6按钮之下
    
    //三小kuang预置变量
    int frmZorder = 8;
    const char* frmPath = "tran.png";
    float frameWidth = bigFrm->getContentSize().width - 36;
    float frameHeight = 56; //等级，经验，转生的底框。
    CCRect neaRect = CCRect(10, 10, 80, 80);
    
    //等级经验系列框
   // CCScale9Sprite *frameLev = CCScale9Sprite::createWithSpriteFrameName(frmPath, neaRect);
    CCScale9Sprite *frameLev = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    frameLev->setPreferredSize(CCSize(frameWidth , frameHeight));
    frameLev->setPosition(ccp(bigFrm->getContentSize().width * 0.5 , bigFrm->getContentSize().height - frameLev->getContentSize().height*0.5 -  16 ));
    bigFrm->addChild(frameLev , frmZorder);

    CCScale9Sprite *framePiece = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    framePiece->setPreferredSize(CCSize(frameWidth , frameHeight));
    framePiece->setPosition(ccp( frameLev->getPositionX() + 20 , frameLev->getPositionY() - 62 ));
    bigFrm->addChild(framePiece , frmZorder);
    
    //主将计被动技框2
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    frame2->setPreferredSize(CCSize(frameWidth/2 - 30, bigFrm->getContentSize().height - 150));
    bigFrm->addChild(frame2, frmZorder);
    frame2->setPosition( ccp( bigFrm->getContentSize().width*0.5 - frameLev->getContentSize().width*0.5 + frame2->getContentSize().width*0.5 + 20, frame2->getContentSize().height*0.5 +10) );

    //缘分系列框3
    CCScale9Sprite *frame3 = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    frame3->setPreferredSize(CCSizeMake(frameWidth - frame2->getContentSize().width - 10, frame2->getContentSize().height ));
    bigFrm->addChild(frame3, frmZorder);
    frame3->setPosition(ccpAdd(frame2->getPosition(), ccp( frame2->getContentSize().width * 0.5 + 10 + frame3->getContentSize().width*0.5 + 3 , 0 )));
    
    //碎片系列框
    
    //展示等级 、 经验 、强化按钮
    int nowLev  = _card->getCurrLevel();
    int maxLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if(enterType == FRIEND_CHECK_ENTER || SHOW_OFFICER_ENTER==enterType)
        maxLev = nowLev;
    //int nowExp = _card->getCurrExp();
    //int maxExp = _card->getMaxExp();
    m_levText = SGCCLabelTTF::create(str_deji, FONT_BOXINFO, 28 ,COLOR_YELLOW);
    m_levText->setPosition(ccp(100 + m_levText->getContentSize().width * 0.5 , frameLev->getContentSize().height * 0.5));
    frameLev->addChild(m_levText);
    
    m_showLev = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d" ,nowLev , maxLev )->getCString(), FONT_BOXINFO, 28);
    m_showLev->setPosition(ccp( m_levText->getPosition().x + m_levText->getContentSize().width * 0.5 + m_showLev->getContentSize().width * 0.5   , m_levText->getPosition().y));
    frameLev->addChild(m_showLev);
    
    //强化按钮
    strengBtn = SGButton::createFromLocal("generalInfoBtnBg.png", str_qianghua_, this,
                                          menu_selector(SGGeneralInfoLayer::strengHandler),ccp( 0 , 4),FONT_PANGWA,ccWHITE,28,false,true);
    strengBtn->setPosition(ccp( zuoji->getPositionX() + 47  ,bigFrm->getPositionY() +
                               bigFrm->getContentSize().height * 0.5 -(bigFrm->getContentSize().height - frameLev->getPositionY()) + 1 ));
    
    //新手引导第16步，要点击这里的强化按钮 对应的modeltag 513
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_13);
    strengBtn->setTag(modelTag);
    this->addBtn(strengBtn);
    
    //武将培养
    developingBtn = SGButton::createFromLocal("generalInfoBtnBg.png", str_developing_officer_17, this, menu_selector(SGGeneralInfoLayer::developingBtnCallback),ccp( 0 , 4),FONT_PANGWA,ccWHITE,28,false,true);
    developingBtn->setPosition(ccpAdd(strengBtn->getPosition(), ccp(-strengBtn->getContentSize().width*0.5 - developingBtn->getContentSize().width*0.5- 20, 0)));
    this->addBtn(developingBtn);
    //给一个引导用的tag
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_ENETR_AREAN, 5);
    developingBtn->setTag(tag);
    developingBtn->setVisible(false);
    developingBtn->setEnabled(false);
    
    //碎片icon 、 经验条 、 来源按钮 、 转生按钮
    m_pieceIcon = CCSprite::createWithSpriteFrameName("pieceIcon.png");
    m_pieceIcon->setPosition(ccp(m_pieceIcon->getContentSize().width*0.6 , framePiece->getContentSize().height*0.5 ));
    framePiece->addChild(m_pieceIcon);
    
    //进度条背景
    m_pieceRateBg = CCScale9Sprite::createWithSpriteFrameName("tran.png");
//    m_pieceRateBg->setScaleX(300.0/m_pieceRateBg->getContentSize().width);
    m_pieceRateBg->setPreferredSize(CCSize(250 , m_pieceRateBg->getOriginalSize().height));
    m_pieceRateBg->setPosition(ccp( m_pieceIcon->getPositionX() + m_pieceIcon->getContentSize().width  + 15 +  m_pieceRateBg->getContentSize().width*0.5*300/374 , m_pieceIcon->getPositionY() ));
    framePiece->addChild(m_pieceRateBg);
    //m_pieceRateBg->setVisible(false); //cgpUI
    
    int pieceHasNum = getOfficerPieceHasNumByOfficerProtoId(_card->getProtoId() );
    int pieceNeedNum =1;
    SGOfficerDataModel * model = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
    bool m_isMaxAdv = false;
    if(0 == model->getOfficerNextId())
        m_isMaxAdv = true;
    else
        pieceNeedNum = SGStaticDataManager::shareStatic()->getAdvOfficerNeedPieceNum(model->getOfficerCurrStarLevel(), model->getAdvNum());
    

    
    //显示碎片进度
    if(pieceHasNum >= pieceNeedNum)
        m_pieceHasNumLab = SGCCLabelTTF::create(CCString::createWithFormat("%d" , pieceHasNum)->getCString(),FONT_BOXINFO, 28 , ccGREEN);
    else
        m_pieceHasNumLab = SGCCLabelTTF::create(CCString::createWithFormat("%d" , pieceHasNum)->getCString(),FONT_BOXINFO, 28 );
    m_pieceHasNumLab->setAnchorPoint(ccp(1, 0.5));
    m_pieceHasNumLab->setPosition(ccpAdd(m_pieceRateBg->getPosition() ,ccp(-5, 0) ) );
    framePiece->addChild(m_pieceHasNumLab , 10);
    
    m_pieceNeedNumLab = SGCCLabelTTF::create(CCString::createWithFormat("/ %d" , pieceNeedNum)->getCString(),FONT_BOXINFO, 28 );
    m_pieceNeedNumLab->setAnchorPoint(ccp(0, 0.5));
    m_pieceNeedNumLab->setPosition(ccpAdd(m_pieceRateBg->getPosition(), ccp(5, 0)));
    framePiece->addChild(m_pieceNeedNumLab , 10);
    
    //到星级上限
    m_upMaxStarLab = SGCCLabelTTF::create(str_GeneralInfoLayer_str11,FONT_BOXINFO, 28 );
    m_upMaxStarLab->setPosition(m_pieceRateBg->getPosition());
    m_upMaxStarLab->setVisible(false);
    framePiece->addChild(m_upMaxStarLab , 10);
    
  
    m_pieceProgress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("pieceRate.png"));
    m_pieceProgress->setScaleX(250/ m_pieceProgress->getContentSize().width);
    m_pieceProgress->setType(kCCProgressTimerTypeBar);
    m_pieceProgress->setMidpoint(ccp(0,0));
    m_pieceProgress->setBarChangeRate(ccp(1, 0));
    m_pieceProgress->setPercentage(100.0 * pieceHasNum/pieceNeedNum);
    m_pieceProgress->setPosition(ccp(m_pieceRateBg->getPositionX()  , m_pieceRateBg->getPositionY() - 1 ) );
    framePiece->addChild(m_pieceProgress);
    
    //添加武将来源按钮，弹出对话框...
    m_sourceBtn = SGButton::create("sourceIcon.png", NULL, this, menu_selector(SGGeneralInfoLayer::onLootSourceBtnClicked), CCPointZero, false, true);
    tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_OFFICER_ADVANCED, 10);
    m_sourceBtn->setTag(tag);
    
    
    this->addBtn(m_sourceBtn);
    m_sourceBtn->setPosition(ccp(m_pieceRateBg->getPositionX() + m_pieceRateBg->getContentSize().width * 0.5 + 25 + m_sourceBtn->getContentSize().width + 25, strengBtn->getPositionY() - (frameLev->getPositionY() - framePiece->getPositionY())  ));
    
    
    
    //转生按钮
    advanceBtn = SGButton::createFromLocal("generalInfoBtnBg.png", "进阶", this,menu_selector(SGGeneralInfoLayer::advanceHandler),ccp(0,4),FONT_PANGWA,ccWHITE,28,false,true);
    advanceBtn->setPosition(ccp(strengBtn->getPositionX() , m_sourceBtn->getPositionY() - 2 ));
    this->addBtn(advanceBtn);
    
    //当转生到最高星级时 不提示进度文本
    if(0==_card->getMaxStar())
    {
        m_pieceHasNumLab->setVisible(false);
        m_pieceNeedNumLab->setVisible(false);
        m_upMaxStarLab->setVisible(true);
        m_pieceProgress->setPercentage(100);
    }

    modelTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_OFFICER_ADVANCED, 4);//阶段引导的第4步的tag
    CCLOG("LIT_GUID_OFFICER_ADVANCED ||  >>>> %d", modelTag);
    advanceBtn->setTag(modelTag);

    
    if (enterType == VISIT_LAYER_ENTER_1 ) {
        wuqi->setVisible(false);
        kaijia->setVisible(false);
        zuoji->setVisible(false);
        peishi->setVisible(false);
        autoequip->setVisible(false);
        bg3->setVisible(false);
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        //CCRect r = CCRectMake(0, 1136/2 - (hgt*1.07)/2, bg1->getContentSize().width, hgt*1.07);
        //bg1->setTextureRect(r);
        bg1->setScaleY(s.height/2 / bg1->getContentSize().height);
        bg1->setScaleX(s.width / bg1->getContentSize().width);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        
    }
    else if ( enterType == SUMMON_OFFICER_ENTER)
    {
        wuqi->setVisible(true);
        wuqi->setEnabled(false);
        kaijia->setVisible(true);
        kaijia->setEnabled(false);
        zuoji->setVisible(true);
        zuoji->setEnabled(false);
        peishi->setVisible(true);
        peishi->setEnabled(false);
        autoequip->setVisible(false);
        bg3->setVisible(true);
        
    }
    else if (enterType == FRIEND_CHECK_ENTER ||enterType == SEARCH_FRIEND_ENTER ||enterType == BLACKLIST_ENTER
			  ||enterType == FRIEND_REQUEST_ENTER ||enterType == ADVANCE_RESULT_ENTER ||enterType == VISIT_RESULT_ENTER
			  ||enterType == TUJIAN_ENTER  ||enterType == STRENG_OFFICER_ENTER || enterType == WILL_ADVANCE_ENTER
			  ||enterType == 100  || enterType == ADVANCE_NEED_ENTER|| enterType == CHATSHOWENTRYTAG )
    {
        autoequip->setVisible(false);
        wuqi->setEnabled(false);
        kaijia->setEnabled(false);
        zuoji->setEnabled(false);
        peishi->setEnabled(false);
        autoequip->setEnabled(false);
		if (enterType == STRENG_OFFICER_ENTER)//在强化页面进入后，武将详情预览不显示下方五个按钮
		{
			SGMainManager::shareMain()->getMainLayer()->setVisible(false);
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

			CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
            CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.17)/2, s.width, hgt*1.17);
			bg1->setTextureRect(r);
            bg1->setAnchorPoint(ccp(0.5, 0));
            
			bg1->setPosition(SGLayout::getPoint(kBottomCenter));
			this->addChild(bg1,5);
		}

        
    }
    else if (enterType == EMBATTLE_LORD_CHECK)
    {
    }
    else
    {
        //如果可以转生  则加上叹号提示
        _spriteNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        _spriteNotice->setPosition( ccp(advanceBtn->getContentSize().width - 8 , advanceBtn->getContentSize().height - 8 ) );
        _spriteNotice->setVisible(false);
        advanceBtn->addChild(_spriteNotice);
        
        //如果可以强化  则加上叹号提示
        m_strengNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        m_strengNotice->setPosition( ccp(strengBtn->getContentSize().width - 8 , strengBtn->getContentSize().height - 8 ) );
        m_strengNotice->setVisible(false);
        strengBtn->addChild(m_strengNotice);
        
        //如果可以强化  则加上叹号提示
        m_devNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        m_devNotice->setPosition( ccp(developingBtn->getContentSize().width - 8 , developingBtn->getContentSize().height - 8 ) );
        m_devNotice->setVisible(false);
        developingBtn->addChild(m_devNotice);
        
        //可转生提示
        if(advanceBtn->isVisible() && pieceHasNum >= pieceNeedNum && _card->getMaxStar() != 0)
            _spriteNotice->setVisible(true);
        //可强化提示
        if(strengBtn->isVisible() && _card->selfShowStrengthNotice())
            m_strengNotice->setVisible(true);
        //可培养提示
        if(developingBtn->isVisible() && _card->selfCanDeveloping())
            m_devNotice->setVisible(true);
        
		float btnOffy = bigFrm->getPosition().y - bigFrm->getContentSize().height * 0.5 - strengBtn->getContentSize().height * 0.47;
        if (enterType == 0 || enterType == HOME_STRENG_OFFICER)
		{
			int shift = 0;
			if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
                ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
			{
				shift = 50;
			}
		}
        
        //强化页面导入的话，四个按钮全部隐藏
        if (enterType == STRENG_DIR_ENTER || enterType ==STRENG_DIR_ENTER_1 ||enterType == HOME_STRENG_ENTER ||enterType == RANK_LIST_ENTER || enterType == ARENA_LIST_ENTER ||enterType == RANK_LIST_ENTER_X || enterType == LOGIN_ENTER|| enterType == INVITE_FRIEND|| enterType == TONGQUE_LAYER_ENTER|| enterType == ARENA_LIST_ENTER )
		{
            strengBtn->setVisible(false);
            developingBtn->setVisible(false);
            advanceBtn->setVisible(false);
			showButton->setVisible(false);
			
            if(enterType == RANK_LIST_ENTER ||enterType == RANK_LIST_ENTER_X ||enterType == LOGIN_ENTER || enterType == ARENA_LIST_ENTER)
            {
                autoequip->setVisible(false);
                
                SGMainManager::shareMain()->getMainLayer()->setVisible(false);
                ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

                CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
                //CCRect r  = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.17)/2, s.width, hgt*1.17);
                //modify by :zyc. change texturerect of pct.
                CCRect r;
                if (s.width >  650)
                    r = CCRectMake(0, 1136/2 - (hgt*1.17)/2, bg1->getContentSize().width, hgt*1.30);
                else
                    r = CCRect(0, 1136/2 - (hgt*1.17)/2, bg1->getContentSize().width, hgt*1.120);
                
                bg1->setTextureRect(r);
                bg1->setScaleX(1.2f);
                bg1->setAnchorPoint(ccp(0.5, 0));
                //bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter) , ccp(-2, 0) ));
                bg1->setPosition(SGLayout::getPoint(kBottomCenter));
                this->addChild(bg1,5);
            
            }
             if(enterType == LOGIN_ENTER || enterType == INVITE_FRIEND||enterType == TONGQUE_LAYER_ENTER)
             {
                 wuqi->setColor(ccc3(70, 70, 70));
                 kaijia->setColor(ccc3(70, 70, 70));
                 zuoji->setColor(ccc3(70, 70, 70));
                 peishi->setColor(ccc3(70, 70, 70));
                 autoequip->setVisible(false);
                 wuqi->setEnabled(false);
                 kaijia->setEnabled(false);
                 zuoji->setEnabled(false);
                 peishi->setEnabled(false);
                 autoequip->setEnabled(false);
             
             }
            
      
        }
        
    }
       //等级经验各文本主武系列预置变量
    CCPoint zwAnchor = ccp(0, 0.5); //锚点置于左中
    
    float move = skewing(15);

    
    //统御，蓄力在右上角
    CCPoint govPos = ccpAdd(SGLayout::getPoint(kUpRight), ccp(-100, -title_bg->getContentSize().height - 37));
    CCPoint roundPos = ccpAdd(govPos, ccp(0, -30));
    this->addLabel(stuff_gov, govPos);
    this->addLabel(stuff_round, roundPos);
	//显示武将是几格的
	this->addLabel(stuff_area, ccpAdd(roundPos, ccp(0, -30)));
    
    //攻防血速背景
    CCSprite* admsBkg = CCSprite::createWithSpriteFrameName("store_redbg.png");
    admsBkg->setScaleY(44.0 / 87.0);
    this->addChild(admsBkg, 5);
    admsBkg->setPosition(ccpAdd(bg3->getPosition(), ccp(0, bg3->getContentSize().height * 0.5 + 44.0 * 0.5) ));
    
    admsBkg->setVisible(false);
    
    //展示 和 换人按钮位置。
    showButton->setPosition(ccp(fermPos1.x + 10 , admsBkg->getPositionY() + 25 + showButton->getContentSize().height*0.5 ));
    
    //changeOfficerButton->setPosition(ccp(fermEffect1->getPositionX() - 15 , showButton->getPositionY() ));
    
    
    
    
    //攻防士速一排
    float admsOffX = wuqi->getContentSize().width * 0.5;
    float admsOffY = 78; //全幅和半幅偏移
    CCPoint atkPos = ccpAdd( wuqi->getPosition(), ccp(-admsOffX, admsOffY) );
    CCPoint defPos = ccpAdd( kaijia->getPosition(), ccp(-admsOffX * 2 / 3, admsOffY) );
    CCPoint morPos = ccpAdd( peishi->getPosition(), ccp(-admsOffX / 3, admsOffY) );
    CCPoint speedPos = ccpAdd( zuoji->getPosition(), ccp(0, admsOffY) );
    this->addLabel(stuff_atk, atkPos);
    this->addLabel(stuff_def, defPos);
    this->addLabel(stuff_mor, morPos);
    this->addLabel(stuff_speed, speedPos);
    
    
    //创建4中颜色武将背景
    std::vector<std::string> str_vec;
    if(s.height != 1136)
    {
        str_vec.push_back("greenBg.png");
        str_vec.push_back("blueBg.png");
        str_vec.push_back("purpleBg.png");
        str_vec.push_back("orangeBg.png");
    }
    //inphone 5  根据武将地图来
    else
    {
        str_vec.push_back("greenBgI5.png");
        str_vec.push_back("blueBgI5.png");
        str_vec.push_back("purpleBgI5.png");
        str_vec.push_back("orangeBgI5.png");
    }
    for(int i =0 ; i< 4; i++)
    {
        m_officerBg[i] = CCSprite::createWithSpriteFrameName( str_vec[i].c_str() );
        m_officerBg[i]->setAnchorPoint(ccp(0.5 , 1));
        if(s.height == 960)
        {
            m_officerBg[i]->setScaleX( 1.56);
            m_officerBg[i]->setScaleY( 1.5);
        }
        else if(s.height == 1024)
        {
            m_officerBg[i]->setScaleX( bigbg->getContentSize().width / m_officerBg[i]->getContentSize().width);
            m_officerBg[i]->setScaleY(bigbg->getScaleY()* 1.52);
        }
        else
        {
            m_officerBg[i]->setScaleX(bigbg->getScaleX() *1.56 );
            m_officerBg[i]->setScaleY( bigbg->getScaleY()*1.56);
            
        }
        m_officerBg[i]->setPosition(ccp(winSize.width*0.5 , titleBgStick->getPositionY() + titleBgStick->getContentSize().height *0.5));
        m_officerBg[i]->setVisible(false);
        this->addChild(m_officerBg[i] , bigbg->getZOrder()+1 );
    }
    this->showOfficerBg(_card->getCurrStar() );
    
    //测试颜色层
    CCLayerColor * layerCo = CCLayerColor::create(ccc4(0, 255, 0, 0), winSize.width, titleBgStick->getPositionY()- atkPos.y - 10) ;
    layerCo->ignoreAnchorPointForPosition(false);
    layerCo->setAnchorPoint(ccp(0.5 , 1));
    layerCo->setPosition(ccp( winSize.width*0.5 , titleBgStick->getPositionY() + titleBgStick->getContentSize().height));
    //this->addChild(layerCo );

    //主将计文本
    CCPoint zjjFontPos = ccp(15, 121);
    SGCCLabelTTF *font_zjji = SGCCLabelTTF::create(str_zhujiangji, FONT_BOXINFO, 28 , COLOR_YELLOW);
    font_zjji->setAnchorPoint(zwAnchor);
    font_zjji->setPosition(ccp(15 , frame2->getContentSize().height - font_zjji->getContentSize().height*0.5 -5));
    frame2->addChild(font_zjji,100);
    
    //被动技文本
    CCPoint wjjFontPos = ccpAdd(zjjFontPos, ccp(0, -69));
    SGCCLabelTTF *font_wjji = SGCCLabelTTF::create(str_beidongji, FONT_BOXINFO, 28 , COLOR_YELLOW);
    font_wjji->setAnchorPoint(zwAnchor);
    font_wjji->setPosition(ccp(15 , frame2->getContentSize().height * 0.5 - font_wjji->getContentSize().height*0.5 ));
    frame2->addChild(font_wjji,100);
    
    //主武更多信息标识
    CCSprite* zwMore = CCSprite::createWithSpriteFrameName("ofc_moreinfo.png");
    CCPoint zwPos = ccp(frame2->getContentSize().width - 32, 30);
    zwMore->setPosition(zwPos);
    frame2->addChild(zwMore, 10);
    
    SGSkillDataModel *lordSkill = NULL;
    if (enterType == TUJIAN_ENTER  || enterType == LOGIN_ENTER||enterType==SHOW_OFFICER_ENTER||enterType == TONGQUE_LAYER_ENTER)
        lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
    else
        lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
   
   
     //主将计文本介绍
    CCString* zjjDesc;
    if (lordSkill)
        zjjDesc = CCString::createWithFormat(str_GeneralInfoLayer_str1,lordSkill->getSkillName()->getCString(),lordSkill->getLevel());
    else
        zjjDesc = CCString::create(str_No);
    labelZjj = SGCCLabelTTF::create( zjjDesc->getCString(), FONT_BOXINFO, 24 );
    frame2->addChild(labelZjj, 100);
    labelZjj->setAnchorPoint(zwAnchor);
    labelZjj->setPosition( ccpAdd(font_zjji->getPosition(), ccp(0, -35)));
    
    //被动技文本介绍
    CCString* wjjDesc;
    int idd = _card->getOfficerSkil();
	SGSkillDataModel *officerSkill=SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
    if (officerSkill)//某种情况下直接使用_card,数据不正确
        wjjDesc = CCString::createWithFormat("%s Lv %s",officerSkill->getSkillName()->getCString(),officerSkill->getSkillClientLevel()->getCString());
    else
        wjjDesc = CCString::create(str_No);
    labelWjj = SGCCLabelTTF::create( wjjDesc->getCString(), FONT_BOXINFO, 24 );
    frame2->addChild(labelWjj, 100);
    labelWjj->setAnchorPoint(zwAnchor);
    labelWjj->setPosition( ccpAdd(font_wjji->getPosition(), ccp(0, -35)) );

    //给主武添加触控回调
    skillLayer = SGTouchLayer::create(frame2->getContentSize().width, frame2->getContentSize().height, 0 /*, ccc4(0, 255, 0, 255)*/);
    skillLayer->ignoreAnchorPointForPosition(false);
    skillLayer->setButtonMode( this, pf_callback_selector(SGGeneralInfoLayer::showSkillInfo) );
    bigFrm->addChild(skillLayer, 10);
    skillLayer->setPosition(frame2->getPosition());
    
    
    
    //获得武将总属性
    CCDictionary* officerInfoDict = getOfficerTotalValue();

    
    if (enterType == STRENG_DIR_ENTER ||enterType == HOME_STRENG_ENTER) ////////强化页面过来
    {
        _before->retain();
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(false);
        
        if (_before->getCurrExp() == cardMaxExp)
        {
            
        }else
        {
            this->setIsCanTouch(false);
        }
        
        if (_before->getLordId() && lordSkill) {
 
            labelZjj->setString(CCString::createWithFormat(str_GeneralInfoLayer_str1,lordSkill->getSkillName()->getCString(),lordSkill->getRound(),lordSkill->getLevel())->getCString());
        }
        if (s.height == 960) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -401));
            
        }
        else if (s.height == 1136) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -537));
            
        }else
        {
            pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365));
        }
        
		//12.04修改
        bg3->setVisible(true);
        wuqi->setVisible(true);
        kaijia->setVisible(true);
        peishi->setVisible(true);
        zuoji->setVisible(true);
        autoequip->setVisible(false);
        
        Direc->retain();
        int strengstate = ((CCString*)Direc->objectForKey("upstate"))->intValue();
        lvlupAct = CCSpriterX::create(CCString::createWithFormat("animationFile/success%d.scml",strengstate)->getCString(), true, true);
        lvlupAct->setanimaID(0);
        lvlupAct->setisloop(false);
        addChild(lvlupAct,100);
        lvlupAct->setPosition(ccpAdd(pos, ccp(0, 200)));
        lvlupAct->play();
        
        EFFECT_PLAY(MUSIC_35);
        
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.07)/2, s.width, hgt*1.07);
       
        bg1->setTextureRect(r);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        //等级经验
        
        CCString *str_ = CCString::create(_card->getClassName());
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
        
        int nCurrLevel = _card->getCurrLevel();
        int nNextLevelExp = 0;
        int nCurrLevelExp = 0;
        if(nCurrLevel>=officerCardlevelLimit)
        {
            nNextLevelExp =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit)->getExpValue();
            nCurrLevelExp = nNextLevelExp;
        }else{
            nNextLevelExp =  _card->getMaxExp();
            nCurrLevelExp =  _card->getCurrExp();
            
        }
        
        
        //攻防士速 原move*2.1
        this->addValueLabel(stuff_atk, ((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue() , move,_card->getAtk() - _before->getAtk(), 1);
        this->addValueLabel(stuff_def, ((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue(), move,_card->getDef() - _before->getDef(), 1);
        this->addValueLabel(stuff_mor, ((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue(), move,_card->getMorale() - _before->getMorale(), 1);
        this->addValueLabel(stuff_speed, ((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue(), move,_card->getSpeed() - _before->getSpeed(), 1);
        //统御蓄力
        this->addValueLabel(stuff_gov, ((CCInteger*) officerInfoDict->objectForKey(KEY_GOV))->getValue(), move + 10, _card->getGoverning(), 1);
        this->addValueLabel(stuff_round, ((CCInteger*) officerInfoDict->objectForKey(KEY_RND))->getValue(), move + 10,_card->getRound() - _before->getRound(), 1);
		this->addValueLabel(stuff_area, ((CCInteger*) officerInfoDict->objectForKey(KEY_AREA))->getValue(), move + 10);

        leftTimes = _card->getCurrLevel() - _before->getCurrLevel();

        float time = (_card->getCurrLevel() - _before->getCurrLevel())*0.9 + lvlupAct->getAnimaTimeLength(0, 0);
        this->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGGeneralInfoLayer::setcanttouch)),CCDelayTime::create(time) ,CCCallFuncN::create(this, callfuncN_selector(SGGeneralInfoLayer::setcantouch)),NULL));
    }
    else if (enterType==STRENG_DIR_ENTER_1)   ///////强化失败的话...
    {
        
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(false);
        if (s.height == 960) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -401));
            
        }
        else if (s.height == 1136) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -537));
            
        }else
        {
            pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365));
        }
        
        bg3->setVisible(false);
        wuqi->setVisible(false);
        kaijia->setVisible(false);
        peishi->setVisible(false);
        zuoji->setVisible(false);
        autoequip->setVisible(false);
     
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.07)/2, s.width, hgt*1.07);
        bg1->setTextureRect(r);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        
        this->addValueLabel(stuff_lvl, 0, 65, 0);
        this->addValueLabel(stuff_atk, 0, move*2.1,0);
        this->addValueLabel(stuff_def, 0, move*2.1,0);
        this->addValueLabel(stuff_round,0, move*2.1,0);
        this->addValueLabel(stuff_speed,0, move*2.1,0);
        this->addValueLabel(stuff_mor,0, move*2.1,0);
		this->addValueLabel(stuff_area, 0, move*2.1,0);
        
        
    }
    else
    {
        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("tran.png");   //cguUI
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setScaleX(s.width/bg1->getContentSize().width);
        bg1->setScaleY((hgt*1.07-btmheight*.68)/bg1->getContentSize().height);
        bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
        this->addChild(bg1,-1);
        
        CCString *str_ = CCString::create(_card->getClassName());
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
        int nCurrLevel = _card->getCurrLevel();
        int nNextLevelExp = 0;
        int nCurrLevelExp = 0;
        if(nCurrLevel>=officerCardlevelLimit)
        {
             nNextLevelExp =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit)->getExpValue();
             nCurrLevelExp = nNextLevelExp;
        }else{
             nNextLevelExp =  _card->getMaxExp();
             nCurrLevelExp =  _card->getCurrExp();
            
        }
        
        //攻防士速，原move*2.1
        this->addValueLabel(stuff_atk, ((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue(), move,0, 1);
        this->addValueLabel(stuff_def, ((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue(), move,0, 1);
        this->addValueLabel(stuff_mor, ((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue(), move,0, 1);
        this->addValueLabel(stuff_speed, ((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue(), move,0, 1);
        //统御蓄力
        this->addValueLabel(stuff_gov, ((CCInteger*) officerInfoDict->objectForKey(KEY_GOV))->getValue(), move + 10, 0, 1);
        this->addValueLabel(stuff_round, ((CCInteger*) officerInfoDict->objectForKey(KEY_RND))->getValue(), move + 10,0, 1);
		this->addValueLabel(stuff_area, ((CCInteger*) officerInfoDict->objectForKey(KEY_AREA))->getValue(), move + 10, 0, 1);
        //经验条
        
		
		if (enterType == VISIT_LAYER_ENTER )//如果探访页面进入
		{
			bg3->setVisible(true);
			wuqi->setVisible(true);
			kaijia->setVisible(true);
			peishi->setVisible(true);
			zuoji->setVisible(true);
			autoequip->setVisible(false);
			advanceBtn->setVisible(false);
			strengBtn->setVisible(false);
            developingBtn->setVisible(false);
			showButton->setVisible(false);
			
		}
		
    }
    
    //将缘文字
    CCPoint fateFontPos = ccp(12, 212);
    SGCCLabelTTF *font_fate = SGCCLabelTTF::create(str_jiangyuan, FONT_BOXINFO, 28 , COLOR_YELLOW);
    font_fate->setAnchorPoint(zwAnchor);
    font_fate->setPosition(ccp( 15 , frame3->getContentSize().height - font_fate->getContentSize().height*0.5 - 5));
    frame3->addChild(font_fate, 100);
    
    //将缘更多信息标识
    CCSprite* fateMore = CCSprite::createWithSpriteFrameName("ofc_moreinfo.png"); //聊天图片
    CCPoint fatePos = ccp(frame3->getContentSize().width - 70, 30);
    fateMore->setPosition(fatePos);
    frame3->addChild(fateMore, 10);

    //将缘LAYER
    CCPoint fyPos = ccp(frame3->getContentSize().width * 0.5, frame3->getContentSize().height * 0.5 - font_fate->getContentSize().height - 10);

    fateLayer = SGTouchLayer::create(frame3->getContentSize().width ,frame3->getContentSize().height, 0);
    fateLayer->ignoreAnchorPointForPosition(false);
    fateLayer->setButtonMode(this, pf_callback_selector(SGGeneralInfoLayer::showFateInfo));
    frame3->addChild(fateLayer, 10);
    fateLayer->setPosition(fyPos);
    
    //生成缘分信息，刷新时候也要用。
    if (enterType == SHOW_OFFICER_ENTER) {
        createFateListFromChatShow();
    }
    else
    {
        createFateList();
    }
    
    
    if (!isFriendGeneral) {
        changeEquipEffectLayer = SGChangeEquipEffectLayer::create();
        this->addChild(changeEquipEffectLayer,10001);
    }
    
    
    
    
    
    //-----------------------------------------------------enterType 控制显隐
    /*
     //正常进入,军营武将 #define NORMAL_ENTER	0
     好友查看武将   #define FRIEND_CHECK_ENTER		1
     //首页强化     #define HOME_STRENG_OFFICER		2
     //首页主将栏进入     #define HOME_OFFICER_BAR		3
     //搜索找人     #define SEARCH_FRIEND_ENTER		4
     //黑名单查看武将     #define BLACKLIST_ENTER			5
     //申请好友查看武将     #define FRIEND_REQUEST_ENTER	6
     //强化主将完成后     #define STRENG_OFFICER_ENTER	7
     //要转生的武将查看     #define WILL_ADVANCE_ENTER		8
     
     //装备界面     #define EQUIP_ENTER_9			9
     #define EQUIP_ENTER_10			10
     #define EQUIP_ENTER_11			11
     #define EQUIP_ENTER_12			12
     //转生结果界面     #define ADVANCE_RESULT_ENTER	13
     //布阵处查看     #define EMBATTLE_CHECK_ENTER	15
     #define EMBATTLE_CHECK_ENTER_1	22
     #define EMBATTLE_LORD_CHECK     500
     //排行查看武将   #define RANK_LIST_ENTER			24
     //****************************************************************************   非拥有武将
     //探访处查看武将     #define VISIT_RESULT_ENTER		14
     //图鉴处查看     #define TUJIAN_ENTER			16
     //转生处查看转生材料的武将     #define ADVANCE_NEED_ENTER		21
     //领奖-登陆奖励     #define LOGIN_ENTER				25
     //领奖-由邀请好友进入    #define INVITE_FRIEND           26
     //扩展解决天梯排行喝聊天进入入口冲突    #define RANK_LIST_ENTER_X           27
    //好友帮助界 #define FIND_HELP_ENTER			100
    //展示界面     #define SHOW_OFFICER_ENTER		1111
    //探访
     #define VISIT_LAYER_ENTER		10002
     #define VISIT_LAYER_ENTER_1		101
     #define STRENG_DIR_ENTER		10000
     #define STRENG_DIR_ENTER_1		10001
    //碎片召唤界面     #define SUMMON_OFFICER_ENTER	11000
    //首页强化主将计  #define HOME_STRENG_ENTER		20000                   */
    
    //C类
    if(enterType ==RANK_LIST_ENTER_X || enterType == RANK_LIST_ENTER || enterType == SHOW_OFFICER_ENTER || enterType == ARENA_LIST_ENTER)
    {
        //展示按钮
        showButton->setVisible(false);
        
        //强化
        strengBtn->setVisible(false);
        //武将培养
        developingBtn->setVisible(false);
        
        //转生进度条
        m_pieceProgress->setPercentage(0);
        m_pieceHasNumLab->setVisible(false);
        m_pieceNeedNumLab->setVisible(false);
        m_upMaxStarLab->setVisible(false);
        
        //转生
        advanceBtn->setVisible(false);
        
    }
    //D类
    else if(enterType== TUJIAN_ENTER || enterType == LOGIN_ENTER ||INVITE_FRIEND == enterType || enterType == SHOW_OFFICER_ENTER)
    {
        //战斗力隐藏
        m_fightStrengthLab->setVisible(false);
        m_fightStrengthNum->setVisible(false);
        
        //展示按钮
        showButton->setVisible(false);
        
        //强化
        strengBtn->setVisible(false);
        developingBtn->setVisible(false);
        
        //转生
        advanceBtn->setVisible(false);
        
        //转生进度条
        m_pieceProgress->setPercentage(0);
        m_pieceHasNumLab->setVisible(false);
        m_pieceNeedNumLab->setVisible(false);
        m_upMaxStarLab->setVisible(false);
    }
    //刷新装备区
    renewEquipment();
}

//定制的修改指定文本数字
void SGGeneralInfoLayer::setlabelValue(stuffType type, int value,int value2)
{
    SGCCLabelTTF *label= NULL;
    
    switch (type)
    {
        case stuff_lvl:
        {
            label = labelLevel;
            label->setScale(1);
            label->stopAllActions();
            label->setString(CCString::createWithFormat(str_Format_ji,value)->getCString());
            return;
        }
            break;
        case stuff_exp:
        {
            labelExp->setScale(1);
            labelExp->stopAllActions();
            labelExp->setString(CCString::createWithFormat("%d/%d", value, value2)->getCString());
            return;
        }
            break;
        case stuff_atk:
        {
            label = labelAtk;
        }
            break;
        case stuff_def:
        {
            label = labelDef;
        }
            break;
        case stuff_round:
        {
            label = labelRound;
        }
            break;
        case stuff_speed:
        {
            label = labelSpeed;
        }
            break;
        case stuff_mor:
        {
            label = labelMorale;
        }
            break;
            
        case stuff_gov:
        {
            label = labelGov;
        }
            break;
		case stuff_area:
		{
			label = labelCell;
		}
			break;
        default:
            break;
    }
    //开始动作
    if (label)
    {
        label->setScale(1);
        label->stopAllActions();
        label->setString(CCString::createWithFormat("%d",value)->getCString());
    }
}

void SGGeneralInfoLayer::changeexplabel()
{
    //currexp为当前经验值应该长到哪  exp为正在不断增加的经验条经验值
    if (exp<=currexp) {
        CCString *str = CCString::createWithFormat("%d/%d",exp,max);
        labelExp->setString(str->getCString());
        float rate = exp/(float)max;
      
        exp+=max*0.1;
        
        CCSequence *seq = CCSequence::create(CCDelayTime::create(.000000000001),CCCallFuncN::create(this,callfuncN_selector(SGGeneralInfoLayer::changeexplabel)),NULL);
        labelExp->runAction(seq);
        
    }
    else
    {     ///最后一次
        
        if(leftTimes==0)
        {
            float rate = _card->getCurrExp()/(float)max;
            
            CCString *str = CCString::createWithFormat("%d/%d",_card->getCurrExp(),max);
            labelExp->setString(str->getCString());
            
            if(_card->getCurrLevel() >= officerCardlevelLimit)
            {
               this->setlabelValue(stuff_lvl, _card->getCurrLevel(), 0);///
            }
            
        }
        else
        {
            CCString *str = CCString::createWithFormat("%d/%d",max,max);
            labelExp->setString(str->getCString());
            
          if(leftTimes==0)
          {
               if(_card->getCurrLevel() >= officerCardlevelLimit)
                  {
                    this->setbreakAction(true);
                    this->setlabelValue(stuff_lvl, _card->getCurrLevel(), 0);
                  }
            }
        }
    }
    
}

//定制的经验条动画启动
void SGGeneralInfoLayer::progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call)
{
//    exp=0;
    _call = call;
    leftTimes = times;
//    float max = 100;
    float per = m_progress->getPercentage();
    float time = 1;
    if (per != 100) {
        time = 0.01*(100 - per);
    }
    if (leftTimes == 0) {
        
        CCString *str_ = CCString::create(_card->getClassName());
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
        max =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit+1)->getexpAll();
    
        currexp = (float)_card->getCurrExp();

        if (_card->getCurrLevel()-_before->getCurrLevel()==0) {  ////单条第一次
            exp = _before->getCurrExp();
            }
        else    //多条最后一次
        {
            exp=0;
        }
        m_progress->runAction(CCCallFuncN::create(this,callfuncN_selector(SGGeneralInfoLayer::changeexplabel)));
    }
    else
    {    ////多条第一次
        if (_card->getCurrLevel()-_before->getCurrLevel() == leftTimes) {
            exp = _before->getCurrExp();
            
            CCString *str_ = CCString::create(_before->getClassName());
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _before->getItemId());
            int beforeMax =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit+1)->getexpAll();
            
            currexp = max = beforeMax;
            
        }
        else  ////多条其他次
        {
            exp = 0;
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(_before->getItemId());

            int currLvl = _card->getCurrLevel();
            SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl-leftTimes);//比如currLvl为5级 此句话获得5级升6级所需经验值
            max = num->getExpValue();
            currexp = num->getExpValue();

        }
        CCAction *action =  CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(SGGeneralInfoLayer::changeexplabel)),CCDelayTime::create(1),CCCallFuncND::create(this, callfuncND_selector(SGGeneralInfoLayer::progressCallback),card),call,NULL);
        m_progress->runAction(action);
    }

}

//定制的经验条动画回调
void SGGeneralInfoLayer::progressCallback(CCNode*node,SGBaseMilitaryCard *card)
{
    if (m_break) {
        return;
    }
    leftTimes--;//11.25
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    CCSprite *waifaguang = CCSprite::createWithSpriteFrameName("waifaguang.png");
    frame1->addChild(waifaguang,9);
    CCFadeOut *fade = CCFadeOut::create(0.2);
    waifaguang->setAnchorPoint(ccp(.5,.5));
    waifaguang->setScaleX(180/210.0);
    waifaguang->runAction(CCSequence::create(fade,CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGGeneralInfoLayer::callBack)),NULL));
    waifaguang->setPosition(ccpAdd(node->getPosition(), ccp(-2,0)));

    if((leftTimes == 1)&&(_card->getCurrLevel() >= officerCardlevelLimit))
        progressAction((CCProgressTimer*)node, leftTimes-1, card, _call);
     //经验条回调后计数应该减1.......
    else
        progressAction((CCProgressTimer*)node, leftTimes, card, _call);
}

void SGGeneralInfoLayer::callBack(cocos2d::CCNode *node)
{
    this->removeChild(node, true);
}

//定制的文本动画
void SGGeneralInfoLayer::labAnimation(stuffType type,int change)
{
    SGCCLabelTTF *label= NULL;
    
    switch (type) {
        case stuff_lvl:
        {
            label = labelLevel;
        }
            break;
        case stuff_atk:
        {
            label = labelAtk;
        }
            break;
        case stuff_def:
        {
            label = labelDef;
        }
            break;
        case stuff_round:
        {
            label = labelRound;
        }
            break;
        case stuff_speed:
        {
            label = labelSpeed;
        }
            break;
        case stuff_mor:
        {
            label = labelMorale;
        }
            break;
            
        case stuff_gov:
        {
            label = labelGov;
        }
            break;
		case stuff_area:
		{
			label = labelCell;
		}
			break;
        default:
            break;
    }
    if (label)
    {
		label->runactionWithScale(1.3, 1.35);
		
		label->runactionWithScale(1, 1);
		label->setString(CCString::createWithFormat("%d",change)->getCString());
		label->runactionWithScale(1, 1.35);
    }

}

//定制的初始化6大LABEL，ATK/DEF/MORALE/SPEED改为图片按钮。
void SGGeneralInfoLayer::addLabel(stuffType type, cocos2d::CCPoint point)
{
    
    switch (type) {
        case stuff_lvl: //等级-文字
        {
            SGCCLabelTTF *lvl = SGCCLabelTTF::create(str_deji, FONT_BOXINFO, 28 , COLOR_YELLOW);
            frame1->addChild(lvl,9);
            lvl->setPosition(point);
            lvl->setAnchorPoint(ccp(0, 0.5));
            lvl->setTag((int)type);
        }
            break;
        case stuff_exp: //经验-文字
        {
            SGCCLabelTTF *exp = SGCCLabelTTF::create(str_jingyan, FONT_BOXINFO, 28 , COLOR_YELLOW);
            frame1->addChild(exp,9);
            exp->setPosition(point);
            exp->setAnchorPoint(ccp(0, 0.5));
            exp->setTag((int)type);
        }
            break;
        case stuff_atk: //攻击-图标
        {
            CCSprite* atk = CCSprite::createWithSpriteFrameName("ofc_icon_atk.png");
            this->addChild(atk,9);
            atk->setPosition(point);
            atk->setTag((int)type);
        }
            break;
        case stuff_def: //防御-图标
        {
            CCSprite* def = CCSprite::createWithSpriteFrameName("ofc_icon_def.png");
            this->addChild(def,9);
            def->setPosition(point);
            def->setTag((int)type);
        }
            break;
        case stuff_mor: //血量-图标
        {
            CCSprite* mor = CCSprite::createWithSpriteFrameName("ofc_icon_morale.png");
            this->addChild(mor,9);
            mor->setPosition(point);
            mor->setTag((int)type);
        }
            break;
        case stuff_speed: //速度-图标
        {
            CCSprite* speed = CCSprite::createWithSpriteFrameName("ofc_icon_speed.png");
            this->addChild(speed,9);
            speed->setPosition(point);
            speed->setTag((int)type);
        }
            break;
        case stuff_gov: //统御-文字
        {
            SGCCLabelTTF *gov = SGCCLabelTTF::create(str_GeneralInfoLayer_str2, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(gov,1000);
            gov->setPosition(point);
            gov->setTag((int)type);
            
        }
            break;
        case stuff_round: //蓄力-文字
        {
            SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(round,1000);
            round->setPosition(point);
            round->setTag((int)type);
        }
            break;
		case stuff_area: //武将占几格
		{
			SGCCLabelTTF *cell = SGCCLabelTTF::create(str_GeneralInfoLayer_str3, FONT_BOXINFO, 28 , COLOR_YELLOW);
			this->addChild(cell, 1000);
			cell->setPosition(point);
			cell->setTag((int)type);
		}
			break;
        default:
            break;
    }
}


void SGGeneralInfoLayer::setcantouch()
{
    this->setIsCanTouch(true);
}
void SGGeneralInfoLayer::setcanttouch()
{
    this->setIsCanTouch(false);
	runAction(CCSequence::create(CCDelayTime::create(lvlupAct->getAnimaTimeLength(0, 0)),CCCallFunc::create(this, callfunc_selector(SGGeneralInfoLayer::playAction)),NULL));
	
}
void SGGeneralInfoLayer::aaaa()
{
    if (m_break)
        return;
    int temp=0;/////中间处理
    if(leftTimes==0)
    {
        if(_card->getCurrLevel() >= officerCardlevelLimit)
            temp=1;
        else
           temp=0;
    }
    else
        temp=leftTimes;
    int times = _card->getCurrLevel() - _before->getCurrLevel()-1;
    
    if (times)
    {
        int lvlchange = _before->getCurrLevel() + times-temp+1;
        int change = _before->getAtk() + ((_card->getAtk() - _before->getAtk())/float(times))*(times - temp);
        int defchange =_before->getDef()+ ((_card->getDef() - _before->getDef())/float(times))*(times - temp);
        int spdchange =_before->getSpeed()+ ((_card->getSpeed() - _before->getSpeed())/float(times))*(times - temp);
        int morchange =_before->getMorale()+ ((_card->getMorale() - _before->getMorale())/float(times))*(times - temp);
        ////////////////
         ///if (leftTimes  == 0) {  ////原逻辑
        if (leftTimes  == 0) {

            change = _card->getAtk();
            defchange = _card->getDef();
            spdchange = _card->getSpeed();
            morchange = _card->getMorale();
            
            
          
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGGeneralInfoLayer::removeSpX));
            this->runAction(CCSequence::create(CCDelayTime::create(1),call,NULL));
            SGSkillDataModel *lordSkilla = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
            SGSkillDataModel *lordSkillb = SGStaticDataManager::shareStatic()->getLordSkillById(_before->getLordId());
            if(lordSkillb && lordSkilla && lordSkillb->getLevel()<lordSkilla->getLevel())
            {
                //主将计升级
//                SGMenuLabel *a = (SGMenuLabel *)menu->getChildByTag(888);
                CCScaleTo *scale1 = CCScaleTo::create(0.05, 1.2);
                CCScaleTo *scale2 = CCScaleTo::create(0.05, 1.5);
                CCScaleTo *scale3 = CCScaleTo::create(0.59, 1);
                CCAction *actionFunc1 =  CCSequence::create(scale1,CCCallFuncND::create(this, callfuncND_selector(SGGeneralInfoLayer::updateskill),(void *)change),scale2,CCDelayTime::create(0.3),scale3,NULL);
                labelZjj->runAction(actionFunc1);
            }
        }
        this->labAnimation(stuff_lvl, lvlchange);
        this->labAnimation(stuff_atk,change);
        this->labAnimation(stuff_def, defchange);
        this->labAnimation(stuff_speed, spdchange);
        this->labAnimation(stuff_mor, morchange);
    }else
    {
        
        if (enterType == STRENG_DIR_ENTER ||enterType == HOME_STRENG_ENTER) {
            //设置六大数值，获得人物属性
            CCDictionary* officerInfoDict = getOfficerTotalValue();
            //攻防士速
            this->setlabelValue(stuff_atk, ((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue());
            this->setlabelValue(stuff_def, ((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue());
            this->setlabelValue(stuff_mor, ((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue());
            this->setlabelValue(stuff_speed, ((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue());
            //统御蓄力格子数
            this->setlabelValue(stuff_gov, ((CCInteger*) officerInfoDict->objectForKey(KEY_GOV))->getValue());
            this->setlabelValue(stuff_round, ((CCInteger*) officerInfoDict->objectForKey(KEY_RND))->getValue());
			this->setlabelValue(stuff_area,  ((CCInteger*) officerInfoDict->objectForKey(KEY_AREA))->getValue());
            //等级经验
            this->setlabelValue(stuff_lvl, _card->getCurrLevel(), 0);

        }
        if (leftTimes==0 &&  (_card->getCurrLevel() >_before->getCurrLevel())) {
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGGeneralInfoLayer::removeSpX));
            this->runAction(CCSequence::create(CCDelayTime::create(1),call,NULL));
        }

    }
    
    //地圈
    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/qh_000.scml", true, true);
    fermEffect2->setanimaID(0);
    fermEffect2->setisloop(false);
    fermEffect2->setPosition(pos);
    //光圈
    CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/qh_001.scml", true, true);
    fermEffect3->setanimaID(0);
    fermEffect3->setisloop(false);
    fermEffect3->setPosition(pos);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    fermEffect2->setScale(s.width/768);
    fermEffect3->setScale(s.width/768);
    addChild(fermEffect2, 1, 2);
    addChild(fermEffect3, 4, 3);
    
    fermEffect2->play();
    fermEffect3->play();
    
}
void SGGeneralInfoLayer::playAction()
{
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGGeneralInfoLayer::aaaa));
	//this->progressAction(expprog, leftTimes, _card, call);

    CCString* skillstate = (CCString*)Direc->objectForKey("skillup");
    
    SGMainManager::shareMain()->showMessage(skillstate->getCString());
}
void SGGeneralInfoLayer::removeSpX()
{
    if (!isback) {
        isbreak = true;
        if (_card->getCurrLevel() > _before->getCurrLevel())
        {
            if (enterType ==2000) {
                SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,1,4);
                SGMainManager::shareMain()->showBox(LvlUpBox);
            }
            else
            {
                SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,1,isfromfirst);
                SGMainManager::shareMain()->showBox(LvlUpBox);
            }
            
        }else
        {
             SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
            if (lordSkill && _card->getCurrLevel() >= officerCardlevelLimit && (lordSkill->getNextId()==0 || lordSkill->getLevel()>=_card->getLordMaxLevel())) {
                if (enterType == 2000) {
                    SGMainManager::shareMain()->showGeneralsLayer(4);
                }
                else
                {
                    SGMainManager::shareMain()->showGeneralsLayer(2);
                }
            }
            else
            {
            strengHandler();
            }
        }
    }
}

void SGGeneralInfoLayer::soldiersDispatchListener(cocos2d::CCObject *obj)
{
}
// 强化
void SGGeneralInfoLayer::strengHandler()
{
       //cgpMain
    if (_card->getCurrLevel() >= officerCardlevelLimit)
        SG_SHOW_WINDOW(str_GeneralInfoLayer_str4);
    else
    {
        SGMainManager::shareMain()->setIsAdv(false);
       // SGMainManager::shareMain()->showStrengLayer(_card,1,isfromfirst);
        SGMainManager::shareMain()->showStrengAdvLayer(_card,1,isfromfirst);

        //GPCCLOG("\n isAdv : %d", SGMainManager::shareMain()->getIsAdv());
    }
}

void SGGeneralInfoLayer::advanceHandler()
{
       //cgpMain
    EFFECT_PLAY(MUSIC_BTN);
    if (_card->getMaxStar() == 0)
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, _card,1,NULL);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
    else
    {
        
        SGMainManager::shareMain()->setIsAdv(true);
        //SGMainManager::shareMain()->showAdvanceLayer(_card,1,isfromfirst);
        SGMainManager::shareMain()->showStrengAdvLayer(_card,1,isfromfirst);

       // GPCCLOG("\n isAdv : %d", SGMainManager::shareMain()->getIsAdv());
    }
}


void SGGeneralInfoLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *type = (CCString *)dict->valueForKey("type");
        if (type->intValue() == 2)
        {
            if (itemtype == WUQI_TAG)
            {
                wuqi->setImage("equip_dao.png");
                wuqi->removeChildByTag(TAG_FRAME_WUQI, true);
                wuqi->removeChildByTag(250,true);
                
                wuqi->removeChildByTag(EQUIP_LABEL_TAG);
            }
            else if (itemtype == KAIJIA_TAG)
            {
                kaijia->setImage("equip_kj.png");
                kaijia->removeChildByTag(TAG_FRAME_KAIJIA, true);
                kaijia->removeChildByTag(250,true);

                kaijia->removeChildByTag(EQUIP_LABEL_TAG);
            }
            else if (itemtype == PEISHI_TAG)
            {
                peishi->setImage("equip_jz.png");
                peishi->removeChildByTag(TAG_FRAME_PEISHI, true);
                peishi->removeChildByTag(250,true);
                peishi->removeChildByTag(EQUIP_LABEL_TAG);
            }
            else if (itemtype == ZUOJI_TAG)
            {
                zuoji->setImage("equip_ma.png");
                zuoji->removeChildByTag(TAG_FRAME_ZUOJI, true);
                zuoji->removeChildByTag(250,true);
                zuoji->removeChildByTag(EQUIP_LABEL_TAG);
            }
        }
    }
    
    if (enterType == LOGIN_ENTER) {
        SGMainScene *mainscene=(SGMainScene *)SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainscene->getChildByTag(sg_mainLayer);
        if (mainLayer) {
            mainLayer->setIsCanTouch(false);
        }
    }

}


//在聊天中展示
void SGGeneralInfoLayer::showHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getOfficerName(),1,temp->getOfficerCurrStarLevel(),_card->getSsid());
}




void SGGeneralInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    /* type
     * 0 正常进入
     * 1 我的好友进入
     * 2 首页强化进入
     * 0 正常进入
     * 3 主界面的主将栏进入
     * 4 从搜索好友界面进入
     * 5 从黑名单中进入
     * 6 加好友请求中进入
     * 7 强化界面进入
     * 8 进阶界面进入
     * 9 10 11 12 换装备进入
     * 13 预览进阶后武将
     * 14 抽奖结果界面进入
     * 15 布阵界面进入
     * 16 卡牌图鉴进入
     * 20 积分兑换进入
     * 21 转生的选择材料进入
     * 22 布阵页面进入
     * 23  祭天
     * 24  首页排行榜查看人物详细阵型
     * 25 累计登陆进入
	 * 100  好友帮助界面,或者好友
	 * 101	探访出武将后点击武将进入
	 * 1111 展示界面
	 * 1980	元宝商城
	 * 20000 从首页强化主将计进入
	 * 11000 从碎片召唤界面进入
	 
	 
	 
	 
     */
	//从布阵界面弹框进入此界面
	if (info && info->isFromEmbattle == true)
	{
		CCLOG("Exchanging......");
		//返回布阵页面
		SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, (SGOfficerCard *)_card, info->ty, info->em);
		SGMainManager::shareMain()->showBox(embattleBox);
		return ;
	}
	///////////////////////////////////////////////////
    if (enterType == STRENG_DIR_ENTER || enterType==STRENG_DIR_ENTER_1 ||enterType == HOME_STRENG_ENTER) {
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(true);
    }
    switch(enterType)
    {
        case FRIEND_CHECK_ENTER:
        {
            showLayerByTag(sg_myFriend);
            break;
        }
        case NORMAL_ENTER:
        {
            //SGMainManager::shareMain()->showGeneralsLayer(enterType,true,0,_currentPage,_currentRow);
            SGMainManager::shareMain()->showGeneralsBagLayer(enterType);
            break;
        }
        case HOME_STRENG_OFFICER:
        {
            SGMainManager::shareMain()->showGeneralsLayer(enterType);
            break;
        }
        case HOME_OFFICER_BAR:
        {
            SGMainManager::shareMain()->showFirstLayer();
            break;
        }
        case SEARCH_FRIEND_ENTER:
        {
            SGBaseLayer *cclayer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_findHelplayer);
            if (cclayer) {
                showLayerByTag(sg_findHelplayer);
            }else{
                showLayerByTag(sg_searchFriendFinish);
            }
            break;
        }
        case BLACKLIST_ENTER:
        {
            showLayerByTag(sg_blackFriend);
            break;
        }
        case FRIEND_REQUEST_ENTER:
        {
           
            showLayerByTag(sg_friendRequest);
            break;
        }
        case STRENG_OFFICER_ENTER:
        {
			SGMainManager::shareMain()->getMainLayer()->setVisible(true);//返回时主页面可触
            showLayerByTag(sg_strengLayer);
            break;
        }
        case WILL_ADVANCE_ENTER:
        {
            SGMainManager::shareMain()->showAdvanceLayer(_card, 1);
            break;
        }
        case EQUIP_ENTER_9:
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL, 9,0);
            break;
        }
        case EQUIP_ENTER_10:
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL, 10,0);
            break;
        }
        case EQUIP_ENTER_11:
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL, 11,0);
            break;
        }
        case EQUIP_ENTER_12:
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL, 12,0);
            break;
        }
        case ADVANCE_RESULT_ENTER:
        {
            showLayerByTag(sg_advanceLayer);
            break;
        }
        case VISIT_RESULT_ENTER:
        {
            showLayerByTag(sg_visitShowCard);
            break;
        }
           
        case EMBATTLE_CHECK_ENTER_1:
        case EMBATTLE_CHECK_ENTER:
        case EMBATTLE_LORD_CHECK:
        {
            SGMainManager::shareMain()->showEmbattleLayer(0);
            break;
        }
        case TUJIAN_ENTER:
        {
            showLayerByTag(sg_cardsLayer);
            break;
        }
        case ADVANCE_NEED_ENTER:
        {
          showLayerByTag(sg_advancechooselayer);
            break;
        }
        case RANK_LIST_ENTER:
        {
            SGMainManager::shareMain()->getMainLayer()->setVisible(true);//返回时主页面可触
            showLayerByTag(sg_rank);
            break;
        }
        case RANK_LIST_ENTER_X:
        {
            SGMainManager::shareMain()->getMainLayer()->setVisible(true);//返回时主页面可触
            showLayerByTag(sg_rank);
            break;
        }
        case ARENA_LIST_ENTER:
        {
            SGMainManager::shareMain()->getMainLayer()->setVisible(true);//返回时主页面可触
            showLayerByTag(sg_areanLayer);
            break;
        }
            
        case LOGIN_ENTER:
        {
            showLayerByTag(sg_totalLoginReward);
            break;
        }
        case FIND_HELP_ENTER:
        {

               //showLayerByTag(sg_findHelplayer);
            SGBaseLayer *cclayer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_findHelplayer);
            if (cclayer) {
                showLayerByTag(sg_findHelplayer);
            }else{
                
             showLayerByTag(sg_myFriend);
                //showLayerByTag(sg_searchFriendFinish);
            }
            break;

        }
            break;
			
        case VISIT_LAYER_ENTER:
        case VISIT_LAYER_ENTER_1:
        {
            //SGVisitLayer *layer =   (SGVisitLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_visitLayer);
            SGVisitLayer *layer =   (SGVisitLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_visitShowCard);
            
            SGMainManager::shareMain()->showLayer(layer);
            
            int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
            if (guideId == guide_tag_32) {
                SGGuideManager::shareManager()->checkIsDone(guide_tag_33);
            }
        }
            break;
        case SUMMON_OFFICER_ENTER://召唤武将
		{
            SGMainManager::shareMain()->showGeneralsBagLayer(0);
//			SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//			SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
            //			SGPiecesLayer *layer = (SGPiecesLayer *)mainScene->getChildByTag(sg_piecesLayer);
//			if (!layer)
//			{
//				layer = SGPiecesLayer::create();
//			}
//			mainLayer->showblack();
//			mainLayer->setPortraitMenuCanTouch(false);
//			mainScene->addShowLayer(layer);
//			SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
		}
			break;
		case STRENG_DIR_ENTER:
        case STRENG_DIR_ENTER_1:
        case HOME_STRENG_ENTER:
        {
            if (_card->getCurrLevel() >= officerCardlevelLimit) {
               SGMainManager::shareMain()->showGeneralsLayer(2);
            }
            else
            strengHandler();
        }
            break;
        case SHOW_OFFICER_ENTER:
        {
            SGMainManager::shareMain()->closeChatShowLayer();
        }
            break;
            //邀请好友进入
        case INVITE_FRIEND:
        {
            main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVER, request);
        }
            break;
        case TONGQUE_LAYER_ENTER:
        {
            SGMainManager::shareMain()->getMainLayer()->entryTongQueShow();
        }
            break;
        default:
            break;
    }
}
void SGGeneralInfoLayer::showChgScoreLayer(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpShopListResponse *response = (main::PvpShopListResponse *)req->m_msg;
    if (response) {
        CCArray *datas = CCArray::create();
        int size = response->items_size();
        
        //        DDLog("credit main-------------------%d", size);
        for (int ii = 0; ii < size; ii++) {
            main::PvpShopItem item = response->items(ii);
            int var1 = item.uid();
            int var2 = item.pvplevelmin();
            int var3 = item.costscore();
            int var4 = item.currentcount();
            int var5 = item.maxcount();
            //int itemId = item.itemid();
            
            SGCreditCardModel *model = SGCreditCardModel::create(var1, var2, var3, var4, var5);
            // 按服务器的15个type走
            model->setModelType(item.itemtype());
            model->setDisCount(item.discountcostscore());
            model->setitemId(item.itemid());
            
            datas->addObject(model);
        }
        SGMainManager::shareMain()->showChgCreditCardLayer(datas,1,2);
    }
}


void SGGeneralInfoLayer::showLayerByTag(LayerTag layerTag)
{
    //从内存中找到这个layer
    //使用的前提是在显示 SGGeneralInfoLayer 时不能把上一个layer释放
    
    
    
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(layerTag);
    SGBaseLayer *mainlayer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    if(mainlayer)
         mainlayer->setVisible(true);
    
    if (layerTag==sg_creditCardLayer) {
        SGBattleScoreLayer *layer=(SGBattleScoreLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_creditCardLayer);
        if (layer == NULL) {
            main::PvpShopListRequest *request = new main::PvpShopListRequest();
            request->set_type(1);
            SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_CDLIST, request);
        }
        else
        SGMainManager::shareMain()->showLayer(layer);

        return;
    }
    
    if(layerTag==sg_rank)
    {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_rank);
        SGBaseLayer*layer2 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_myFriend);
        SGBaseLayer*layer3 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendRequest);
        SGBaseLayer*layer4 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_searchFriendFinish);
        SGBaseLayer*layer5 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatLayer);
        SGBaseLayer*layer6 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossbattleranklayer);
         SGBaseBox*layer7 = (SGBaseBox *)SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank);
        
      
        if(layer)
        {
            SGMainManager::shareMain()->showLayer(layer);
//            SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
//            sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGRankLayer::receivedetail));
        }
        if(layer2)
        {
            SGMainManager::shareMain()->showLayer(layer2);
        }
        if(layer3)
        {
            SGMainManager::shareMain()->showLayer(layer3);
        }
        if(layer4)
        {
            SGMainManager::shareMain()->showLayer(layer4);
        }
        if(layer5)
        {
            if(enterType !=RANK_LIST_ENTER_X)
            {
            SGMainManager::shareMain()->showLayer(layer5);
            }
        }
        
        if(layer6)
        {
          SGMainManager::shareMain()->showLayer(layer6);
        }
       
        if( layer7)
        {
            if(enterType !=RANK_LIST_ENTER_X&&enterType!= RANK_LIST_ENTER)
            {
                main::FriendInfoRequest *infoinfo = new main::FriendInfoRequest();
                SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
                int playerid=player->getplayerID();
                infoinfo->set_type(0);
                infoinfo->set_friendid(playerid);
                
                SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, infoinfo);
            }
            else
            {
                layer7->setVisible(true);
                if(layer)
                    layer->setIsCanTouch(false);
            }
        }
        
        return;
    }
    if(layerTag == sg_areanLayer )
    {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_areanLayer);
       
        SGBaseBox*layer2 = (SGBaseBox *)SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank);
        if(layer)
        {
             SGMainManager::shareMain()->showLayer(layer);
        }
        if(layer2)
        {
            layer2->setVisible(true);
            if(layer)
                layer->setIsCanTouch(false);
        }

    }
    else if(NULL == layer)
    {
         SGChatMessageCenter::shareChatMessageCenter()->openChatRoom();//如果是空，是点击了世界聊天中得头像，然后点击了主将
        return;
    
    }
    
    SGMainManager::shareMain()->showLayer(layer);
    
    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_generalInfoLayer, true);
}

//查看主将计和被动技
void SGGeneralInfoLayer::showSkillInfo()
{
    CCLOG("<><><>显示主将计!");
    EFFECT_PLAY(MUSIC_BTN);
    SGGeneralinfoEffect *effect = SGGeneralinfoEffect::create(this, _card,enterType);
    SGMainManager::shareMain()->showBox(effect);
    
}
void SGGeneralInfoLayer::showEquipInfo()
{
    SGMainManager::shareMain()->showEquipInfoLayer(NULL,0);
}

//查看被动技，现在与查看主将计功能合并
void SGGeneralInfoLayer::showOfficerSkil()
{
    CCLOG("<><><>显示被动技!");
    showSkillInfo();
}
void  SGGeneralInfoLayer::showEquipTips(int type)
{
    if(type ==9)
        SGMainManager::shareMain()->showMessage(str_GeneralInfoLayer_str501);
    if(type ==10)
        SGMainManager::shareMain()->showMessage(str_GeneralInfoLayer_str502);

    if(type ==11)
        SGMainManager::shareMain()->showMessage(str_GeneralInfoLayer_str503);

    if(type ==12)
        SGMainManager::shareMain()->showMessage(str_GeneralInfoLayer_str504);

    
}

void SGGeneralInfoLayer::setEquipment(CCNode *sender)
{
    EFFECT_PLAY(MUSIC_BTN);

    itemtype = sender->getTag();
    int nAdNum =  _card->getAdNum();
    int nCurrStar = _card->getCurrStar();
    CCArray *equips = _card->getOfficerEquips();
    
    if (equips && equips->count() != 0)
    {
        SGEquipCard *temp = NULL;
        CCObject *obj = NULL;
        CCARRAY_FOREACH(equips, obj)
        {
            SGEquipCard *card_ = (SGEquipCard *)obj;
            if (card_->getItemType() == itemtype)
            {
                temp = card_;
                break;
            }
        }
        if (temp)
        {
            
            if(enterType == RANK_LIST_ENTER||enterType == RANK_LIST_ENTER_X || enterType == ARENA_LIST_ENTER ) //排行榜查看别人武将的装备
            {
//                SGMainManager::shareMain()->getMainLayer()->setVisible(true);
//                SGMainManager::shareMain()->showEquipInfoLayer(temp,24,false);
                SGCommonInfoBox* cib = SGCommonInfoBox::create(this, temp, NULL, SGCIBOT_OTHER_EQUIP, enterType, isfromfirst);
                SGMainManager::shareMain()->showBox(cib);
            }
            else //自己武将的已穿戴装备
            {
                //参数 “3” 时首页武将栏进入。 保证后续返回统一、正确。费解
                SGCommonInfoBox* cib = SGCommonInfoBox::create(this, temp, _card, SGCIBOT_MY_EQUIP_WEAR, 3, isfromfirst);
                SGMainManager::shareMain()->showBox(cib);
            }
        }
        else
        {
            if (enterType == FRIEND_CHECK_ENTER||enterType == RANK_LIST_ENTER||enterType == RANK_LIST_ENTER_X  || enterType == ARENA_LIST_ENTER)
            {
                SGMainManager::shareMain()->showMessage(str_GeneralInfoLayer_str5);
            }else
            {
                
                if(isfromfirst==11||isfromfirst==12)
                {
                    SGMainManager::shareMain()->showEquipsLayer(_card,itemtype,enterType, false,isfromfirst);
                }
                else
                {
                    SGMainManager::shareMain()->showEquipsLayer(_card,itemtype,enterType, false);
                }
            }
        }
    }
    else {
        if (enterType == FRIEND_CHECK_ENTER||enterType == RANK_LIST_ENTER||enterType == RANK_LIST_ENTER_X  || enterType == ARENA_LIST_ENTER) {
            SGMainManager::shareMain()->showMessage(str_GeneralInfoLayer_str5);
        }else{
            if(isfromfirst==11||isfromfirst==12)
            {
                SGMainManager::shareMain()->showEquipsLayer(_card,itemtype,enterType, false,isfromfirst);
            }
            else
            {
                SGMainManager::shareMain()->showEquipsLayer(_card,itemtype,enterType, false);
            }
        }
    }
}
void SGGeneralInfoLayer::updateskill(CCNode *node)
{
    SGMenuLabel *a = (SGMenuLabel*)node;
    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
    a->setString(CCString::createWithFormat(str_Format_round_class,lordSkill->getSkillName()->getCString(),lordSkill->getRound(), lordSkill->getLevel())->getCString());
    
}

bool SGGeneralInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (enterType == VISIT_LAYER_ENTER_1 || enterType == VISIT_LAYER_ENTER) {//10002 对应探访界面
        backHandler();
    }
    if (isbreak) {
        return false;
    }
    
    if (enterType== STRENG_DIR_ENTER ||enterType == HOME_STRENG_ENTER) {
        if (!m_break)
        {
            isback = true;
            this->setbreakAction(true);
            this->removeChildByTag(2, true);
            this->removeChildByTag(3, true);
            
            //设置六大数值，获得人物属性
            CCDictionary* officerInfoDict = getOfficerTotalValue();
            //攻防士速
            this->setlabelValue(stuff_atk, ((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue());
            this->setlabelValue(stuff_def, ((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue());
            this->setlabelValue(stuff_mor, ((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue());
            this->setlabelValue(stuff_speed, ((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue());
            //统御蓄力
            this->setlabelValue(stuff_gov, ((CCInteger*) officerInfoDict->objectForKey(KEY_GOV))->getValue());
            this->setlabelValue(stuff_round, ((CCInteger*) officerInfoDict->objectForKey(KEY_RND))->getValue());
			this->setlabelValue(stuff_area, ((CCInteger*) officerInfoDict->objectForKey(KEY_AREA))->getValue());
            //等级经验
            this->setlabelValue(stuff_lvl, _card->getCurrLevel(),officerCardlevelLimit);
            
            CCString *str_ = CCString::create(_card->getClassName());
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
            int maxExp =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit+1)->getexpAll();
            
            this->setlabelValue(stuff_exp, _card->getCurrExp(),maxExp);
            
//            SGMenuLabel *a = (SGMenuLabel *)menu->getChildByTag(888);
            labelZjj->setScale(1);
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
            if (_card->getLordId() && lordSkill) {
                
                labelZjj->setString(CCString::createWithFormat(str_Format_round_class,lordSkill->getSkillName()->getCString(),lordSkill->getRound(), lordSkill->getLevel())->getCString());
            }
//            expprog->stopAllActions();
            if (maxExp == _card->getCurrExp()) {

            }
            else
            {
                if (_card->getCurrExp() == 0) {
                }else
                {

                }
            }
            
            if (_card->getCurrLevel() > _before->getCurrLevel())
            {
                if (enterType ==STRENG_DIR_ENTER) {
                     SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,1,isfromfirst);
                     SGMainManager::shareMain()->showBox(LvlUpBox);
                }
                else if (enterType == HOME_STRENG_ENTER)
                {
                 SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,1,4);
                     SGMainManager::shareMain()->showBox(LvlUpBox);
                }
               
               
            }
            return true;
        }else
        {
            if (isback && (_card->getCurrLevel() == _before->getCurrLevel())) {
                backHandler();
            }
            
            return false;
        }
    }
    return false;
}

void SGGeneralInfoLayer::onEnter()
{
    SGBaseLayer::onEnter();
    if (storyID!=-1) {
        SGButton *enterBtn = SGButton::createFromLocal("box_btnbg.png",
                                              str_GeneralInfoLayer_str6,
                                              this,
                                                       menu_selector(SGGeneralInfoLayer::enterFightAction),
                                                       CCPointZero,
                                                       FONT_PANGWA,
                                                       ccWHITE,32);
                                              
        enterBtn -> setTag(2013);
        this->addBtn(enterBtn);
        enterBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 150)));
    } 
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -8888, false); //INT32_MAX
}

void SGGeneralInfoLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    SGBaseLayer::onExit();
}

//!!!!: 试图开启保护的回调
void SGGeneralInfoLayer::officerprotectListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::OfficerProtectionResponse *response = (main::OfficerProtectionResponse *)sr->m_msg;
    
    if (sr)
    {
        if(response->state() == 1) //如果设置成功
        {
            _card->setIsProt(1);
            changeApsButtons(BTN_TAG_PROTECTED);
            
            SGMainManager::shareMain()->showMessage(str_Equipslayer_str6);
        }else
        {
            CCLOG("设为保护失败");
        }
    }
}


void  SGGeneralInfoLayer::autoequipfunc(cocos2d::CCObject *obj)                             
{
    //SGEquipCard *temp = NULL;
    SGAutoEquipBox *autoequipbox = SGAutoEquipBox::create(this, _card,2);
    SGMainManager::shareMain()->showBox(autoequipbox);


}


void SGGeneralInfoLayer::listener(CCObject *obj)
{
    
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer2.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::AutoEquipResponse *msg = (main::AutoEquipResponse *)sr->m_msg;
    main::UpdateBagInfo msg1 = msg->updatebaginfo();
    SGMainScene::upDateBagInfoHandler(&msg1);

    //刷新装备区
    renewEquipment();
    
   
    //设置六大数值，获得人物属性
    CCDictionary* officerInfoDict = getOfficerTotalValue();
    
    SGEquipCard *card=new SGEquipCard();
  
    //CCLog("%d",atoi(labelAtk->getString()));
    card->setAtk(((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue()-atoi(labelAtk->getString())) ;
    card->setDef(((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue()-atoi(labelDef->getString())) ;
    card->setMorale(((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue()-atoi(labelMorale->getString())) ;
    card->setSpeed(((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue()-atoi(labelSpeed->getString())) ;
    
    
    SGMainManager::shareMain()->showEffect(card, 1);
    //攻防士速
    this->setlabelValue(stuff_atk, ((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue());
    this->setlabelValue(stuff_def, ((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue());
    this->setlabelValue(stuff_mor, ((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue());
    this->setlabelValue(stuff_speed, ((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue());
    //统御蓄力
    this->setlabelValue(stuff_gov, ((CCInteger*) officerInfoDict->objectForKey(KEY_GOV))->getValue());
    this->setlabelValue(stuff_round, ((CCInteger*) officerInfoDict->objectForKey(KEY_RND))->getValue());
	this->setlabelValue(stuff_area, ((CCInteger*) officerInfoDict->objectForKey(KEY_AREA))->getValue());
}

void SGGeneralInfoLayer::addValueLabel(stuffType type, int value1,float move, int value2, int anchorType,int qianvalue, int strengthtype,bool ismax)
{
    //比较坑爹的特殊处理
    CCSprite* sp;
    if (type == stuff_lvl || type == stuff_exp) //等级经验特殊，从frame1里找。
    {
        sp = (CCSprite *) frame1->getChildByTag(type);
    }
    else //其他的从本UI里找
    {
        sp = (CCSprite *) this->getChildByTag(type);
    }
    //@warning: 只有等级和经验的VALUE2目前有作用！
    switch (type) {
        case stuff_lvl: //等级
        {
            CCString *lvel = NULL;// CCString::create("");
            //临时解决办法，援军武将信息等级上限 取值有问题。目前等级和上限取相同值
            if(SEARCH_FRIEND_ENTER == enterType)
                lvel = CCString::createWithFormat("   %d/%d",value1,value1);
            else
                lvel = CCString::createWithFormat("   %d/%d",value1,value2);
            //lvel->setString(CCString::createWithFormat("%d级",value1)->getCString());
           
            
            labelLevel = SGCCLabelTTF::create(lvel->getCString(),FONT_BOXINFO, 28);
            
            labelLevel->setTag(999);
            labelLevel->setAnchorPoint(ccp(0, 0.5));
            frame1->addChild(labelLevel,9);
            labelLevel->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            //带分号的
           // CCLOG("%d",_card->getCurrLevel());
        }
            break;
        case stuff_exp: //经验
        {
            CCString *def = CCString::createWithFormat("%d/%d",value1,value2);
            labelExp = SGCCLabelTTF::create(def->getCString(),FONT_BOXINFO,28);
            frame1->addChild(labelExp,9);
            labelExp->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
        }
            break;
        case stuff_atk: //攻击
        {
            CCString *atk = CCString::createWithFormat("%d",value1);
            labelAtk = SGCCLabelTTF::create(atk->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelAtk->setAnchorPoint(anchor);
            labelAtk->setTag(1000);
//			labelAtk->setPosition(ccp(sp->getPositionX() + move, framesp->getPositionY()));
            this->addChild(labelAtk,9);
            labelAtk->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
			
        }
            break;
        case stuff_def: //防御
        {
            CCString *def = CCString::createWithFormat("%d",value1);
            labelDef = SGCCLabelTTF::create(def->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelDef->setAnchorPoint(anchor);
            labelDef->setTag(1001);
            this->addChild(labelDef,9);
            labelDef->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
        }
            break;
        case stuff_mor: //血量
        {
            CCString *shiqi = CCString::createWithFormat("%d",value1);
            labelMorale = SGCCLabelTTF::create(shiqi->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelMorale->setAnchorPoint(anchor);
            labelMorale->setTag(1004);
            this->addChild(labelMorale,9);
            labelMorale->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
        }
            break;
        case stuff_speed: //速度
        {
            CCString *speed = CCString::createWithFormat("%d",value1);
            labelSpeed = SGCCLabelTTF::create(speed->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelSpeed->setAnchorPoint(anchor);
            labelSpeed->setTag(1003);
            this->addChild(labelSpeed,9);
            labelSpeed->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
        }
            break;
        case stuff_gov: //统御
        {
            CCString *shiqi = CCString::createWithFormat("%d",value1);
            labelGov = SGCCLabelTTF::create(shiqi->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelGov->setAnchorPoint(anchor);
            labelGov->setTag(1005);
            this->addChild(labelGov,9);
            labelGov->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
        }
            break;
        case stuff_round: //蓄力
        {
            CCString *xuli = CCString::createWithFormat("%d",value1);
            labelRound = SGCCLabelTTF::create(xuli->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelRound->setAnchorPoint(anchor);
            labelRound->setTag(1002);
            this->addChild(labelRound,9);
            labelRound->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
        }
            break;
		case stuff_area:
		{
			CCString *cell = CCString::createWithFormat("%d",value1);
            labelCell = SGCCLabelTTF::create(cell->getCString(), FONT_BOXINFO, 28);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            labelCell->setAnchorPoint(anchor);
            labelCell->setTag(1007);
            this->addChild(labelCell,9);
            labelCell->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
		}
			break;
        default:
            break;
    }
}

//显示缘分信息
void SGGeneralInfoLayer::showFateInfo()
{
    SGFateInfoLayer* sgfil = SGFateInfoLayer::create(_card, enterType);
    sgfil->setPosition(0, 20);
    this->addChild(sgfil, 1500);
    CCLOG("<><><><>显示缘分信息!!!!");
}

/**
 * 移动layer
 */
void SGGeneralInfoLayer::moveItemLayer()
{
    CCLOG(">>>>武将详情moveItemLayer");
    if (m_isTouched && m_canMove)
    {
        //预制滑动状态
        int type = 0;
        //超过此距离可以发起转换移除
        if (m_dist > SLIDE_CHANGE_DISTANCE) //向右移动，则向左展现
        {
            int trick = m_index - 2;
            if (trick >= -1) //滑到左侧尽头
            {
                CCSprite* temp = generateData(trick);
                //轮替逻辑
                if (m_rightObj)
                    m_rightObj->removeFromParentAndCleanup(true);
                m_rightObj = m_middleObj;
                m_middleObj = m_leftObj;
                m_leftObj = temp;
                if (m_leftObj)
                {
                    m_leftObj->setPosition(m_leftPos);
                    this->addChild(m_leftObj, ZORDER_FIX + trick);
                }
                m_index -= 1;
                type = -1;
                
                SGOfficerCard * officerCard = (SGOfficerCard *)m_myOfficerList->objectAtIndex(m_index);
                if(officerCard)
                    showOfficerBg(officerCard->getCurrStar());
            }
        }
        else if (m_dist < -SLIDE_CHANGE_DISTANCE) //向左移动，则向右展现
        {
            int trick = m_index + 2;
            if (trick <= m_myOfficerList->count()) //滑到右侧尽头
            {
                CCSprite* temp = generateData(trick);
                //轮替逻辑
                if (m_leftObj)
                    m_leftObj->removeFromParentAndCleanup(true);
                m_leftObj = m_middleObj;
                m_middleObj = m_rightObj;
                m_rightObj = temp;
                if (m_rightObj)
                {
                    m_rightObj->setPosition(m_rightPos);
                    this->addChild(m_rightObj, ZORDER_FIX + trick);
                }
                m_index += 1;
                type = 1;
                
                SGOfficerCard * officerCard = (SGOfficerCard *)m_myOfficerList->objectAtIndex(m_index);
                if(officerCard)
                    showOfficerBg(officerCard->getCurrStar());
            }
        }
        else //回归原位，不用动。
        {
            ;
        }
        //设置归位动画，这里也兼任刷新武将信息功能
        adjustPosition(type);

    }
    m_isTouched = m_canMove = false;
}

/**
 * 使用动作调整位置。-2=不调整。-1=左不调整，0=全部调整，1=右不调整。
 */
void SGGeneralInfoLayer::adjustPosition(int type)
{
    m_canTouched = false;
    float time = abs(m_dist) / SLIDE_SPEED;
    if (m_leftObj && type != -1)
        m_leftObj->runAction( CCMoveTo::create( time, m_leftPos) );
    if (m_middleObj)
        m_middleObj->runAction( CCMoveTo::create( time, m_middlePos) );
    if (m_rightObj && type != 1)
        m_rightObj->runAction( CCMoveTo::create( time, m_rightPos) );
    //动画完毕后0.2秒才允许再触控，否则可能杯具
    CCDelayTime* cdt = CCDelayTime::create(time + 0.2);
    CCCallFunc* ccf = CCCallFunc::create(this, callfunc_selector(SGGeneralInfoLayer::resetCanTouched));
    CCSequence* action = CCSequence::create(cdt, ccf, NULL);
    this->runAction(action);
    //重置武将信息
    if (type == -1 || type == 1)
        refreshOfficerInfo();
    //调整箭头
    tuneArrows();
}

/**
 * 重置LAYER使能够被触控。
 */
void SGGeneralInfoLayer::resetCanTouched()
{
    m_canTouched = true;
}

//began
bool SGGeneralInfoLayer::helloBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_canTouched)
        return false;
//    CCLOG(">>>>武将详情ccTouchBegan");
    m_isTouched = true;
    m_beganX = pTouch->getLocation().x;
    if (m_leftObj)
        m_leftX = m_leftObj->getPosition().x;
    if (m_middleObj)
        m_middleX = m_middleObj->getPosition().x;
    if (m_rightObj)
        m_rightX = m_rightObj->getPosition().x;
    return true;
}

//moved
void SGGeneralInfoLayer::helloMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isTouched || !m_canTouched)
        return;
//    CCLOG(">>>>武将详情ccTouchMoved");
    CCPoint tp = pTouch->getLocation();
    m_dist = (tp.x - m_beganX);
    //X向移动距离>10，切未在移动中，则允许移动
    if (abs(m_dist) >= SLIDE_LIMIT && !m_canMove)
    {
        m_canMove = true;
    }
    if (m_canMove)
    {
        if (m_leftObj)
            m_leftObj->setPositionX(m_leftX + m_dist);
        if (m_middleObj)
            m_middleObj->setPositionX(m_middleX + m_dist);
        if (m_rightObj)
            m_rightObj->setPositionX(m_rightX + m_dist);
    }
}

//ended
void SGGeneralInfoLayer::helloEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_canTouched)
        return;
//    CCLOG(">>>>武将详情ccTouchEnded");
    moveItemLayer();

}

//cancelled
void SGGeneralInfoLayer::helloCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_canTouched)
        return;
    moveItemLayer();
}

//调整左右箭头的显隐
void SGGeneralInfoLayer::tuneArrows()
{
    if (!m_arrowLeft || !m_arrowRight)
        return;
    m_arrowLeft->setVisible( !(m_leftObj == NULL) );
    m_arrowRight->setVisible( !(m_rightObj == NULL) );
}

//刷新武将信息！
void SGGeneralInfoLayer::refreshOfficerInfo()
{
    if ( m_myOfficerList && m_index >= 0 && m_index < m_myOfficerList->count() )
    {
        SGOfficerCard* newCard = (SGOfficerCard*) m_myOfficerList->objectAtIndex(m_index);
        if (newCard)
        {
            //预置变量
            CCString* str = NULL;
            _card = newCard;
            //标题名称
            str = _card->getOfficerName();
            if (!str)
                str = CCString::create(str_GeneralInfoLayer_str7);
            labelTitle->setString(str->getCString());
            ccColor3B color[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
            ccColor3B titleColor = ccc3(0,0,0);
            int PinZhi = newCard->getCurrStar();
            if(PinZhi <3 || PinZhi>6)
                titleColor = ccGREEN;
            else
                titleColor = color[PinZhi - 3];

            labelTitle->setInsideColor(titleColor);
            if(0 < _card->getAdNum())
            {
                m_advanceCountLab->setString(CCString::createWithFormat("+%d", _card->getAdNum())->getCString());
                m_advanceCountLab->setVisible(true);
            }
            else
                m_advanceCountLab->setVisible(false);
            //星星

            int maxStar = _card->getUpgradestar();
            this->setstar(_card->getCurrStar(), _card->getUpgradestar());  //cgpStar
            
            
            
            //六大属性修改
            renewSimpleInfo();
            
            
            //等级(50级 (50/70)
//            int curLevel = _card->getCurrLevel();
//            int maxLevel = officerCardlevelLimit;
//            int maxLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
//            //临时解决办法  援军信息取等级上限有误。
//            if(SEARCH_FRIEND_ENTER == enterType)
//                str = CCString::createWithFormat("   %d/%d", curLevel,curLevel);
//            else
//                str = CCString::createWithFormat("   %d/%d", curLevel,maxLevel);
            //刷新战斗力
            //战斗力
            SGStaticDataManager * dataManage = SGStaticDataManager::shareStatic();
            int combat=0;
            int grid = dataManage->getOfficerById(_card->getItemId())->getArea();
            int officerCombat = dataManage->getBaseCombatValue(6, _card->getCurrLevel(), _card->getCurrStar(), _card->getAdNum());
            int skillCombat = dataManage->getSkillCombatValue(grid, _card->getLordId(), _card->getOfficerSkil());
            combat = officerCombat + skillCombat;
            //计算装备战力加成
            if(_card->getOfficerEquips())
            {
                for(int i=0; i<_card->getOfficerEquips()->count() ; ++i)
                {
                    SGEquipCard * temp = (SGEquipCard*)_card->getOfficerEquips()->objectAtIndex(i);
                    if(temp)
                        combat += dataManage->getBaseCombatValue(7, temp->getCurrLevel(), temp->getCurrStar(), temp->getAdNum());
                }
                
            }
            setOfficerFightStrength( combat );
            
            //经验
            CCString *str_ = CCString::create(_card->getClassName());
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
            int nCurrLevel = _card->getCurrLevel();
            int nNextLevelExp = 0;
            int nCurrLevelExp = 0;
            if(nCurrLevel>=officerCardlevelLimit)
            {
                nNextLevelExp =  SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, officerCardlevelLimit)->getExpValue();
                nCurrLevelExp = nNextLevelExp;
            }else{
                nNextLevelExp =  _card->getMaxExp();
                nCurrLevelExp =  _card->getCurrExp();
                
            }
            
            str = CCString::createWithFormat("%d/%d", nCurrLevelExp, nNextLevelExp);
            //刷新等级
            int nowLev = _card->getCurrLevel();
            int maxLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
            if(SEARCH_FRIEND_ENTER == enterType || SHOW_OFFICER_ENTER==enterType)
                maxLev = nowLev;
            m_showLev->setString(CCString::createWithFormat("%d/%d" , nowLev , maxLev )->getCString());
            
            //刷新经验
//            m_showExp->setString(CCString::createWithFormat("%d/%d" , nCurrLevelExp , nNextLevelExp)->getCString());
            
            //todo : 碎片获取
            int hasPieceNum = getOfficerPieceHasNumByOfficerProtoId(_card->getProtoId());
            int needPieceNum = 1;
            SGOfficerDataModel * model = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
            bool m_isMaxAdv = false;
            if(0 == model->getOfficerNextId())
                m_isMaxAdv = true;
            else
                needPieceNum = SGStaticDataManager::shareStatic()->getAdvOfficerNeedPieceNum(model->getOfficerCurrStarLevel(), model->getAdvNum());


            
            m_pieceProgress->setPercentage(100.0 * hasPieceNum/needPieceNum);
            if(_card->getMaxStar() !=0)
            {
                m_pieceNeedNumLab->setVisible(true);
                m_pieceHasNumLab->setVisible(true);
                
                m_pieceHasNumLab->setString(CCString::createWithFormat("%d" , hasPieceNum)->getCString());
                if(hasPieceNum >= needPieceNum)
                    m_pieceHasNumLab->setInsideColor(ccGREEN);
                else
                    m_pieceHasNumLab->setInsideColor(ccWHITE);
                m_pieceNeedNumLab->setString(CCString::createWithFormat("/ %d" , needPieceNum)->getCString());
                
                m_upMaxStarLab->setVisible(false);
            }
            else
            {
                m_pieceNeedNumLab->setVisible(false);
                m_pieceHasNumLab->setVisible(false);
                m_upMaxStarLab->setVisible(true);
                m_pieceProgress->setPercentage(100);
            }
            
            if(advanceBtn->isVisible() && hasPieceNum >= needPieceNum && _card->getMaxStar() != 0)
                _spriteNotice->setVisible(true);
            else
                _spriteNotice->setVisible(false);
            
            if(strengBtn->isVisible() && _card->selfShowStrengthNotice())
                m_strengNotice->setVisible(true);
            else
                m_strengNotice->setVisible(false);
            
            if(developingBtn->isVisible() && _card->selfCanDeveloping())
                m_devNotice->setVisible(true);
            else
                m_devNotice->setVisible(false);


            //主将计
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
            if (lordSkill != NULL)
                str = CCString::createWithFormat(str_GeneralInfoLayer_str1,lordSkill->getSkillName()->getCString(),lordSkill->getLevel());
            else
                str = CCString::create(str_No);
            labelZjj->setString(str->getCString());
            //被动技
            SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
            if (officerSkill)
                str = CCString::createWithFormat("%s Lv%s",officerSkill->getSkillName()->getCString(),officerSkill->getSkillClientLevel()->getCString());
            else
                str = CCString::create(str_No);
            labelWjj->setString(str->getCString());
            //缘分
            if (enterType == CHATSHOWENTRYTAG) {
                createFateListFromChatShow();
            }
            else
            {
                createFateList();
            }
            
            //如果该武将可以转生  则  显示红色叹号提示。
            if (advanceBtn->isVisible())
            {
            }
            //装备刷新
            renewEquipment();
        }
        else
            CCLOG("<><><><>切换武将信息错误");
    }
}

//生成缘分信息！
void SGGeneralInfoLayer::createFateList()
{
    //首先清空
    fateLayer->removeAllChildrenWithCleanup(true);
    //缘分列表
//    const float fItemOffX = 70; //单个缘分文本偏移宽度
//    const float fItemOffY = 25; //单个缘分文本偏移高度
    
    SGFateManager* sgfm = SGFateManager::sharedManager();
    SGFateBase* sgfb = NULL;
    CCString* name = NULL;
    SGCCLabelTTF* label = NULL;
    float finX = 0; //最终确定的文本偏移X
    float finY = 0; //最终确定的文本偏移Y
    
    //橙色和灰色
    ccColor3B cOn = ccc3(0xf4, 0xac, 0x00);
    ccColor3B cOff = ccc3(0xa4, 0xa4, 0xa4);
    
    int xxCur = 1; //用于指示位置，从1开始，自然理解
//    const int xxMAX = 6; //最多6个缘分文本
    std::vector<int> fateVec = _card->getFateList();
    std::vector<int>::iterator iter = fateVec.begin();
    std::vector<SGCCLabelTTF * > vec;
//    float labHeight = 0.0;
    for (; iter != fateVec.end(); iter++)
    {
        sgfb = sgfm->getFateBaseById(*iter);
        if (sgfb)
        {
            name = sgfb->getName();
            //条件达成（白色）
            if (sgfm->checkIsFateGoal(_card, *iter) && enterType!=20)
                label = SGCCLabelTTF::create(name->getCString(), FONT_BOXINFO, 26 , cOn);
            //未达成（灰色）
            else
                label = SGCCLabelTTF::create(name->getCString(), FONT_BOXINFO, 26 , cOff);
            label->setAnchorPoint(ccp(0,0));
            vec.push_back(label);
            
            fateLayer->addChild(label, 10);
            if (xxCur % 2 == 1) //1列
            {
//                finX = 15 + label->getContentSize().width * 0.5;
                finX = 15;
                finY = fateLayer->getContentSize().height -20 - (1.1*label->getContentSize().height * (xxCur/2 ) ) ;
            }
            else //2列
            {
//                finX = 20 + vec[xxCur-2]->getContentSize().width*0.5 + vec[xxCur-2]->getPosition().x + label->getContentSize().width*0.5;
                finX = 160;
                finY = vec[xxCur-2]->getPosition().y;
            }
            label->setPosition(ccp(finX, finY));

            //指示位置+1
            xxCur++;
        }
    }
}

void SGGeneralInfoLayer::createFateListFromChatShow()
{
    //首先清空
    fateLayer->removeAllChildrenWithCleanup(true);
    //缘分列表
    const float fItemOffX = 60; //单个缘分文本偏移宽度
    const float fItemOffY = 30; //单个缘分文本偏移高度
    
    SGFateManager* sgfm = SGFateManager::sharedManager();
    SGFateBase* sgfb = NULL;
    CCString* name = NULL;
    SGCCLabelTTF* label = NULL;
    float finX = 0; //最终确定的文本偏移X
    float finY = 0; //最终确定的文本偏移Y
    
    //橙色和灰色
    ccColor3B cOn = ccc3(0xf4, 0xac, 0x00);
    ccColor3B cOff = ccc3(0xa4, 0xa4, 0xa4);
    //    ccColor3B cPink = ccc3(0xe0, 0x92, 0x92);
    
    int xxCur = 1; //用于指示位置，从1开始，自然理解
    const int xxMAX = 6; //最多6个缘分文本
    std::vector<int> fateVec = _card->getFateList();
    int fateNum = 0;
    int fateId = 0;
    int fateState = 0;
    
    std::vector<SGCCLabelTTF * > vec;

    for (std::vector<int>::iterator iter = fateVec.begin(); iter != fateVec.end(); iter++)
    {
        fateNum = *iter;
        fateId = fateNum / 10;
        fateState = fateNum - fateId * 10;
        sgfb = sgfm->getFateBaseById(fateId);
        name = sgfb->getName();
        if (fateState != 1 )
        {
            //条件达成（白色）
            label = SGCCLabelTTF::create(name->getCString(), FONT_BOXINFO, 26);
            label->setInsideColor(cOn);
        }
        else
        {
            //未达成（灰色）
            label = SGCCLabelTTF::create(name->getCString(), FONT_BOXINFO, 26);
            label->setInsideColor(cOff);
        }
        vec.push_back(label);
        label->setAnchorPoint(ccp(0,0));
        fateLayer->addChild(label, 10);
    
        if (xxCur % 2 == 1) //1列
        {
            finX = 15;
            finY = fateLayer->getContentSize().height -20 - (1.1*label->getContentSize().height * (xxCur/2 ) ) ;
        }
        else //2列
        {
            finX = 160;
            finY = vec[xxCur-2]->getPosition().y;
        }
        label->setPosition(ccp(finX, finY));
        
        //指示位置+1
        xxCur++;
        
//        if (xxCur % 2 == 1) //1列
//            finX = fItemOffX;
//        else //2列
//            finX = fItemOffX * 3;
//        finY = ( ((xxMAX - xxCur) / 2 + 1) * 2 - 1 ) * fItemOffY;
//        label->setPosition(ccp(finX, finY));
//        //指示位置+1
//        xxCur++;
    }
}
//获得所有自身属性的加成
CCDictionary* SGGeneralInfoLayer::getOfficerTotalValue()
{
	//重刷缘分列表
	if (fateLayer)
    {
        if (enterType == CHATSHOWENTRYTAG) {
            createFateListFromChatShow();
        }
        else
        {
           createFateList();//
        }
    }
    //初始为人物自身
    int atk = _card->getAtk();
    int def = _card->getDef();
    int mor = _card->getMorale();
    float speed= _card->getSpeed();
    int gov = _card->getGoverning();
    int round = _card->getRound();
	SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
	int area = temp->getArea();
	((area == -1) && (area = 0));
    //循环装备
    if (enterType != CHATSHOWENTRYTAG) {
        //聊天展示只用服务器下发数据
        CCObject *obj = NULL;
        CCARRAY_FOREACH(_card->getOfficerEquips(), obj)
        {
            SGEquipCard *card = (SGEquipCard *)obj;
//            atk += card->getAtk();
//            def += card->getDef();
//            mor += card->getMorale();
//            speed += card->getSpeed();
            gov += card->getGoverning();
            round += card->getRound();
        }
    }

    //修正统御
    if (gov < 0)
        gov = 0;
    //构造字典
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(CCInteger::create(atk), KEY_ATK);
    dict->setObject(CCInteger::create(def), KEY_DEF);
    dict->setObject(CCInteger::create(mor), KEY_MOR);
    dict->setObject(CCInteger::create(speed), KEY_SPD);
    dict->setObject(CCInteger::create(gov), KEY_GOV);
    dict->setObject(CCInteger::create(round), KEY_RND);
	dict->setObject(CCInteger::create(area), KEY_AREA);
    return dict;
}

//获得装备对自身的加成
CCDictionary* SGGeneralInfoLayer::getOfficerEquipAddedValue()
{
    int atk = 0;
    int def = 0;
    int mor = 0;
    float speed= 0;
    int gov = 0;
    int round = 0;
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_card->getOfficerEquips(), obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
       
//        atk += card->getAtk();
//        def += card->getDef();
//        mor += card->getMorale();
//        speed += card->getSpeed();
        gov += card->getGoverning();
        round += card->getRound();
    }
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(CCInteger::create(atk), KEY_ATK);
    dict->setObject(CCInteger::create(def), KEY_DEF);
    dict->setObject(CCInteger::create(mor), KEY_MOR);
    dict->setObject(CCInteger::create(speed), KEY_SPD);
    dict->setObject(CCInteger::create(gov), KEY_GOV);
    dict->setObject(CCInteger::create(round), KEY_RND);
    return dict;
}

//转生,保护,卖出的按钮状态切换，1=转生，2=锁定+卖出。
void SGGeneralInfoLayer::changeApsButtons(int type)
{
   
    if (BTN_TAG_ADVANCE == type) //转生
    {
        if (advanceBtn)
        {
            if (_card->getMaxStar() == 0);
//                advanceBtn->setImage(IMG_BTN_OFF);
            else;
//                advanceBtn->setImage(IMG_BTN_ON);
        }
    }
}
void SGGeneralInfoLayer::setTheLockEquip()
{
    int nAdNum =  _card->getAdNum();
    int nCurrStar = _card->getCurrStar();
    wuqilock->setVisible(false);
    kaijialock->setVisible(false);
    peishilock->setVisible(false);
    zuojilock->setVisible(false);
    if(nCurrStar>=3)
    {
        wuqilock->setVisible(false);
        if(nAdNum==1)
            kaijialock->setVisible(false);
        if(nAdNum==2)
        {
            kaijialock->setVisible(false);
            peishilock->setVisible(false);
        }
        if(nAdNum==3)
        {
            kaijialock->setVisible(false);
            peishilock->setVisible(false);
            zuojilock->setVisible(false);
        }
        if(nCurrStar>=4)
        {
            kaijialock->setVisible(false);
            peishilock->setVisible(false);
            zuojilock->setVisible(false);
        }
    }else{
         wuqilock->setVisible(false);
        kaijialock->setVisible(false);
        peishilock->setVisible(false);
        zuojilock->setVisible(false);
    }
}

//刷新四件装备按钮，同时刷新"转生""锁定""卖出"按钮，因为一定更换了武将。
void SGGeneralInfoLayer::renewEquipment()
{
    setTheLockEquip();
    //if(isPlayOpenLockAnimation)
        //PlayOpenLockAnimation();
    //边框清理
    wuqi->removeChildByTag(TAG_FRAME_WUQI, true);
    kaijia->removeChildByTag(TAG_FRAME_KAIJIA, true);
    peishi->removeChildByTag(TAG_FRAME_PEISHI, true);
    zuoji->removeChildByTag(TAG_FRAME_ZUOJI, true);
    wuqi->removeChildByTag(250, true);
    kaijia->removeChildByTag(250, true);
    peishi->removeChildByTag(250, true);
    zuoji->removeChildByTag(250, true);
    
    
    //清理等级label
    wuqi->removeChildByTag(EQUIP_LABEL_TAG);
    kaijia->removeChildByTag(EQUIP_LABEL_TAG);
    peishi->removeChildByTag(EQUIP_LABEL_TAG);
    zuoji->removeChildByTag(EQUIP_LABEL_TAG);
//    //图标清理
//    wuqi->removeChildByTag(TAG_ICON_WUQI, true);
//    kaijia->removeChildByTag(TAG_ICON_KAIJIA, true);
//    peishi->removeChildByTag(TAG_ICON_PEISHI, true);
//    zuoji->removeChildByTag(TAG_ICON_ZUOJI, true);
    //背景清理
    if (!m_isEqpBkgReady)
    {
        CCSprite *sp;
        //武器
        sp = CCSprite::createWithSpriteFrameName("card_bj.png");
        wuqi->addChild(sp,-5,10);
        sp->setPosition(ccp(wuqi->getContentSize().width*.5f, wuqi->getContentSize().height*.5f));
        //铠甲
        sp = CCSprite::createWithSpriteFrameName("card_bj.png");
        kaijia->addChild(sp,-5,10);
        sp->setPosition(ccp(wuqi->getContentSize().width*.5f, wuqi->getContentSize().height*.5f));
        //配饰
        sp = CCSprite::createWithSpriteFrameName("card_bj.png");
        peishi->addChild(sp,-5,10);
        sp->setPosition(ccp(wuqi->getContentSize().width*.5f, wuqi->getContentSize().height*.5f));
        //坐骑
        sp = CCSprite::createWithSpriteFrameName("card_bj.png");
        zuoji->addChild(sp,-5,10);
        sp->setPosition(ccp(wuqi->getContentSize().width*.5f, wuqi->getContentSize().height*.5f));
        m_isEqpBkgReady = true;
    }
    //提供card_bg
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer, LIM_PNG_AS_PNG);
    //提供equip_xxx原型
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    
    CCObject *obj = NULL;
    
    bool wuqiDone = false;
    bool kaijiaDone = false;
    bool peishiDone = false;
    bool zuojiDone = false;
    
    CCARRAY_FOREACH(_card->getOfficerEquips(), obj)
    {
        //卡牌数据
        SGEquipCard *card = (SGEquipCard *)obj;
        SGMainManager::shareMain()->addHeadIconPlistByNum(card->getHeadSuffixNum(),sg_generalInfoLayer);
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(card->getItemId());
        //品质边框图
        CCSprite *tempk = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
        tempk->setPosition(ccp(wuqi->getContentSize().width/2,wuqi->getContentSize().height/2));
        
        
        //真正图标图
//        CCString *aa = CCString::createWithFormat("head%d.png",card->getItemId());
		CCString *aa = CCString::createWithFormat("head%d.png",card->getHeadSuffixNum());//12.06
        if (card->getItemType() == WUQI_TAG)
        {
            wuqi->setImage(aa->getCString());
            wuqi->addChild(tempk, 1, TAG_FRAME_WUQI);
            wuqi->removeChildByTag(250);
            
            if (card->getAdNum()>0)
            {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create((CCString::createWithFormat("+%d",card->getAdNum()))->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(115,115));
                wuqi->addChild(advancenumber,50,250);
            }

            wuqiDone = true;
            if (card->getCurrLevel() > 0)//加上等级
            {
                SGCCLabelTTF * curLev = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d" , card->getCurrLevel())->getCString(), FONT_PANGWA, 24   , CCSizeZero , kCCTextAlignmentCenter , kCCVerticalTextAlignmentCenter);
                curLev->setPosition(ccp(wuqi->getContentSize().width * 0.5f, curLev->getContentSize().height * 0.5f - 2));
                curLev->setTag(EQUIP_LABEL_TAG);
                wuqi->addChild(curLev);
            }
        }
        else if(card->getItemType() == KAIJIA_TAG)
        {
            kaijia->setImage(aa->getCString());
            kaijia->addChild(tempk, 1, TAG_FRAME_KAIJIA);
            kaijia->removeChildByTag(250);
            
            if (card->getAdNum()>0)
            {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",card->getAdNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(115,115));
                kaijia->addChild(advancenumber,50,250);
            }

            kaijiaDone = true;
            if(card->getCurrLevel() > 0)      //加上等级
            {
                SGCCLabelTTF * curLev = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d" , card->getCurrLevel())->getCString(), FONT_PANGWA, 24 , CCSizeZero , kCCTextAlignmentCenter , kCCVerticalTextAlignmentCenter);
                curLev->setPosition(ccp(kaijia->getContentSize().width * 0.5f, curLev->getContentSize().height * 0.5f - 2));
                curLev->setTag(EQUIP_LABEL_TAG);
                kaijia->addChild(curLev);

            }
        }
        else if(card->getItemType() == PEISHI_TAG)
        {
            peishi->setImage(aa->getCString());
            peishi->addChild(tempk, 1, TAG_FRAME_PEISHI);
            peishi->removeChildByTag(250);
            
            if (card->getAdNum()>0)
            {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(115,115));
                peishi->addChild(advancenumber,50,250);
            }
            peishiDone = true;
            if (card->getCurrLevel() > 0 ) //加上等级
            {
                SGCCLabelTTF * curLev = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d" , card->getCurrLevel())->getCString(), FONT_PANGWA, 24 , CCSizeZero , kCCTextAlignmentCenter , kCCVerticalTextAlignmentCenter);
                curLev->setPosition(ccp(peishi->getContentSize().width * 0.5f, curLev->getContentSize().height * 0.5f - 2));
                curLev->setTag(EQUIP_LABEL_TAG);
                peishi->addChild(curLev);

            }
        }
        else if(card->getItemType() == ZUOJI_TAG)
        {
            zuoji->setImage(aa->getCString());
            zuoji->addChild(tempk, 1, TAG_FRAME_ZUOJI);
            zuoji->removeChildByTag(250);
            
            if (card->getAdNum()>0)
            {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(115,115));
                zuoji->addChild(advancenumber,50,250);
            }

            zuojiDone = true;
            if(card->getCurrLevel() > 0 ) //加上等级
            {
                SGCCLabelTTF * curLev = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d" , card->getCurrLevel())->getCString(), FONT_PANGWA, 24 , CCSizeZero , kCCTextAlignmentCenter , kCCVerticalTextAlignmentCenter);
                curLev->setPosition(ccp(zuoji->getContentSize().width * 0.5f, curLev->getContentSize().height * 0.5f - 2));
                curLev->setTag(EQUIP_LABEL_TAG);
                zuoji->addChild(curLev);
            }
        }
    }
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    //没有的图变为原始图片
    if (!wuqiDone)
        wuqi->setImage("equip_dao.png");
    if (!kaijiaDone)
        kaijia->setImage("equip_kj.png");
    if (!peishiDone)
        peishi->setImage("equip_jz.png");
    if (!zuojiDone)
        zuoji->setImage("equip_ma.png");
    //转生/锁定+卖出按钮
    changeApsButtons(BTN_TAG_ADVANCE);
    changeApsButtons(BTN_TAG_PROTECTED);
    
    //刷新装备红点
    this->refreshEquipNotice(_card);
}


//刷新六大属性，用于卸下装备
void SGGeneralInfoLayer::renewSimpleInfo()
{
    CCString* str = NULL;
    //六大属性修改bgn>>>>
    //获得武将总属性
    CCDictionary* officerInfoDict = getOfficerTotalValue();
    //攻击
    str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_ATK))->getValue());
    labelAtk->setString(str->getCString());
    //防御
    str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_DEF))->getValue());
    labelDef->setString(str->getCString());
    //血量
    str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_MOR))->getValue());
    labelMorale->setString(str->getCString());
    //速度
    str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_SPD))->getValue());
    labelSpeed->setString(str->getCString());
    //统御
    str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_GOV))->getValue());
    labelGov->setString(str->getCString());
    //蓄力
    str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_RND))->getValue());
    labelRound->setString(str->getCString());
	//占几格
	str = CCString::createWithFormat("%d", ((CCInteger*) officerInfoDict->objectForKey(KEY_AREA))->getValue());
	labelCell->setString(str->getCString());
    renewEquipment();
    //<<<<六大属性修改end
}

//回调刷新本页面，主要是更新装备
void SGGeneralInfoLayer::refreshView()
{
    renewEquipment();
    renewSimpleInfo();
}


//重写触控，方便控制三个组件的开关。
void SGGeneralInfoLayer::setIsCanTouch(bool isTouch)
{
    SGBaseStuffLayer::setIsCanTouch(isTouch);
    if (slideLayer)
        slideLayer->setIsWorking(isTouch);
    if (skillLayer)
        skillLayer->setIsWorking(isTouch);
    if (fateLayer)
        fateLayer->setIsWorking(isTouch);
}

//存储本次将要参与滑动的武将卡列表
void SGGeneralInfoLayer::setMyOfficerList(CCArray* arr)
{
    //筛出武将的牌，当前武将占有1-6的类型。
    CCArray* list = CCArray::create();
    CCObject* pObj = NULL;
    SGBaseMilitaryCard* sgmc = NULL;
    CCARRAY_FOREACH(arr, pObj)
    {
        sgmc = (SGBaseMilitaryCard*) pObj;
        if (sgmc && sgmc->getItemType() >= 1 && sgmc->getItemType() <= 6)
        {
            list->addObject(sgmc);
        }
    }
    CC_SAFE_RETAIN(list);
    m_myOfficerList = list;
}

void SGGeneralInfoLayer::enterFightAction()
{
    //CCLog("%d,",roldID);
    //CCLog("%d",plotID);
   // CCLog("%d",storyID);
 
    SGBattleManager::sharedBattle()->pveStart(getStoryID(), getPlotID(), getRoldID());
}

//阵容换人
void SGGeneralInfoLayer::changeHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
	
    
    //这里重新计算上阵相关数据，不然会在主页的进入详情，点击换将，会出错，数据全是错的。泥妹！！！！
	//emtype = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,_card);
    CCArray *cards = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    availPos = player->getPlayerOfficerMax()-cards->count();
    
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    CCObject *obj = NULL;
    int mor = 0;
    int speed = 0;
    totalGov = 0;
    CCARRAY_FOREACH(array, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        mor += card->getMorale();
        speed += card->getSpeed();
        totalGov += card->getGoverning();
		
        if (card->getOfficerEquips())
        {
            for (int i = 0; i <card->getOfficerEquips()->count(); i++)
            {
                SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
//                mor += equip->getMorale();
//                speed += equip->getSpeed();
                if ((card->getGoverning()+equip->getGoverning())<0)
                    totalGov -= card->getGoverning();
                else
                    totalGov += equip->getGoverning();
            }
        }
    }
    
    
    
	SGArrangeLayer *layer = SGArrangeLayer::create(po,totalGov,11,emtype,availPos);
    layer->setComBattle(combatle);
    layer->setSpeedValue(speedValue);
    layer->setMorValue(morValue);
    layer->setGovValue(GovValue);
    SGMainManager::shareMain()->showLayer(layer);
}

void SGGeneralInfoLayer::setSpeedAndMorValue(int speedV, int MOr, int com)
{
    combatle=com;
    speedValue=speedV;
    morValue=MOr;
}
void SGGeneralInfoLayer::offHandler()
{
    EFFECT_PLAY(MUSIC_BTN);

    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    if (array->count() == 1)
        SGMainManager::shareMain()->showMessage(str_EmbattlBox_str4);
    else
    {
		main::RemoveOfficerRequest *request = new main::RemoveOfficerRequest();
        request->set_position(SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,_card));
        request->set_teamid(emtype-1);
		SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_REMOVE, request);
    }
}
void SGGeneralInfoLayer::setMainOfficerHandler()
{
    EFFECT_PLAY(MUSIC_BTN);

    if (SGTeamgroup::shareTeamgroupInfo()->islord(emtype,_card)) {
        this->removeFromParentAndCleanup(true);
        SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
        return;
    }
    main::SetLordRequest *request = new main::SetLordRequest();
    request->set_ssid(_card->getSsid());
    request->set_teamid(emtype-1);
    SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_SETLORD, request);
}
void SGGeneralInfoLayer::setMainOfficerListener(CCObject *obj)
{
    CCLOG("setMainOfficerListener");
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::DispatchOfficerResponse *response = (main::DispatchOfficerResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCLOG("上场成功");
            //设置武将保护
            _card->setIsProt(1);
            SGMainManager::shareMain()->showEmbattleLayer(0,(response->teamid()+1));
            
        }
        else
        {
            CCLOG("上场失败");
        }
        //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
    }
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
    SGMainManager::shareMain()->updateEmbattleInfo();
}
void SGGeneralInfoLayer::offMainOfficerListener(CCObject *obj)
{
    int bufcombattle=combatle;
    int bufspeedValue=speedValue;
    int bufMorvalue=morValue;
    int bufGov=GovValue;
    
    CCLOG("offMainOfficerListener");
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::RemoveOfficerResponse *response = (main::RemoveOfficerResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCLOG("下场成功%d",_card->retainCount());

            _card->setIsProt(0);
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_EmbattlBox_str2,_card->getOfficerName()->getCString())->getCString());
            emtype = (response->teamid()+1);

            if (SGTeamgroup::shareTeamgroupInfo()->islord(emtype,_card))
            {
                CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
                SGOfficerCard *card1 = (SGOfficerCard*)array->objectAtIndex(0);
                for (int i = 1; i<array->count(); i++)
                {
                    SGOfficerCard* card2 = (SGOfficerCard*)array->objectAtIndex(i);
                    int p1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype, card1);
                    int p2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype, card2);
                    card1 = p1 <p2 ?card1:card2;
                }
            }
            this->removeFromParentAndCleanup(true);
            
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            
            SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
        }
        else
        {
            CCLOG("下场失败");
        }
    }
    SGMainManager::shareMain()->updateEmbattleInfo();
    SGEmbattleLayer *embattleLayer=(SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer);
    CCLog("" );
    embattleLayer->showAddAttributeEffect(embattleLayer->getmorValue()-bufMorvalue, embattleLayer->getspeedvalue()-bufspeedValue, SGPlayerInfo::sharePlayerInfo()->getcombatValue()-bufcombattle,embattleLayer->getgovValue()-bufGov);
}
void SGGeneralInfoLayer::setLordOfficerListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SetLordResponse *response = (main::SetLordResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::createWithFormat("%d",_card->getSsid()), "lordid");
            dict->setObject(CCString::create("lord"), "type");
            
            //SGTeamgroup::shareTeamgroupInfo()->setlordId(emtype,officerCard->getSsid());
            //CCLog("@@@%d",SGTeamgroup::shareTeamgroupInfo()->array[emtype].getLordSkill());
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_EmbattlBox_str3,_card->getOfficerName()->getCString())->getCString());
            
            CCLOG("设为主将成功");
            this->removeFromParentAndCleanup(true);
           
            SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
            
            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
            SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
            if (mainLayer) {
                mainLayer->setIsCanTouch(true,false);
            }
            SGEmbattleLayer *embattleLayer = (SGEmbattleLayer *)mainScene->getChildByTag(sg_embattleLayer);
            if (embattleLayer) {
                embattleLayer->showBoxCall(dict);
            }
            SGMainManager::shareMain()->updateEmbattleInfo();
            SGGuideManager::shareManager()->checkIsDone(guide_tag_42);
        }
        else
        {
            CCLOG("设为主将失败");
        }
    }
}
void SGGeneralInfoLayer::setGovAndType(int _gov, int _emType,int _position,int availbPos)
{
    totalGov=_gov;
    emtype=_emType;
    position=_position;
    availPos=availbPos;
}


void SGGeneralInfoLayer::onLootSourceBtnClicked()
{
    SGMainManager::shareMain()->showMessage("功能未开放!");
    return;
    int pieceId = -1;
    SGPiecesDataModel * piece = SGStaticDataManager::shareStatic()->getPiecesDictByOfficerProtoId(_card->getProtoId());
    if(piece)
        pieceId = piece->getPiecesId();
    
    SGLootWiKiMainLayer * lootWiKiMainLayer = SGLootWiKiMainLayer::create( pieceId);
    SGMainManager::shareMain()->showBox(lootWiKiMainLayer);
}

void SGGeneralInfoLayer::showLevExp(int nowLev, int maxLevel, int nowExp, int maxExp , bool isVisible)
{
    if(!isVisible)
    {
        m_showLev->setVisible(false);
//        m_showExp->setVisible(false);
        strengBtn->setVisible(false);
        m_levText->setVisible(false);
//        m_expText->setVisible(false);
    }
    else
    {
        m_showLev->setVisible(true);
//        m_showExp->setVisible(true);
        strengBtn->setVisible(true);
//        m_expText->setVisible(true);
        m_levText->setVisible(true);

        m_showLev->setString(CCString::createWithFormat("%d/%d" , nowLev , maxLevel)->getCString());
//        m_showExp->setString(CCString::createWithFormat("%d/%d" , nowExp , maxExp)->getCString());
    }
}

void SGGeneralInfoLayer::showPieceRate(int hasNum, int needNum , bool isVisible)
{
    if(!isVisible)
    {
        m_pieceProgress->setVisible(false);
        m_pieceRateBg->setVisible(false);
        m_pieceIcon->setVisible(false);
//        m_sourceBtn->setVisible(false);
        advanceBtn->setVisible(false);
        m_pieceNum->setVisible(false);
    }
    else
    {
        m_pieceProgress->setVisible(true);
        m_pieceRateBg->setVisible(true);
        m_pieceIcon->setVisible(true);
//        m_sourceBtn->setVisible(true);
        advanceBtn->setVisible(true);
        m_pieceNum->setVisible(true);
        
        m_pieceProgress->setPercentage(1.0*hasNum / needNum);
        m_pieceNum->setString("");
    }
}

void SGGeneralInfoLayer::showOfficerBg(int quality)
{
    //如果有意外情况 品质不属于 绿 蓝 紫 橙
    if(quality >6 || quality<3)
        quality = 3;
    for(int i =0 ; i<4; i++)
        m_officerBg[i]->setVisible(false);
    m_officerBg[quality-3]->setVisible(true);
}

int SGGeneralInfoLayer::getOfficerPieceHasNumByOfficerProtoId(int protoId)
{
    int pieceHasNum = 0;
    SGPiecesDataModel * piece = SGStaticDataManager::shareStatic()->getPiecesDictByOfficerProtoId(_card->getProtoId());
    if(piece)
    {
        int itemId = piece->getPiecesId();
        CCArray * array = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
        SGPiecesCard * temp;
        for(int i=0 ; i<array->count() ; ++i)
        {
            temp = (SGPiecesCard*)array->objectAtIndex(i);
            if(itemId == temp->getItemId())
            {
                pieceHasNum = temp->getPiecesCurNum();
                break;
            }
        }
    }
    return pieceHasNum;
}

void SGGeneralInfoLayer::refreshEquipNotice(SGOfficerCard * refreshCard)
{
    //不能强化武将的页面 不提示。
    if(!strengBtn->isVisible())
        return;
    if(refreshCard->isWearEquip(9))
    {
        if(refreshCard->hisEquipCanAdv(9) || refreshCard->hisEquipCanStreng(9) )
            wuqiNotice->setVisible(true);
        else
            wuqiNotice->setVisible(false);
    }
    else if(refreshCard->canWearEquip(9) )
        wuqiNotice->setVisible(true);
    else
        wuqiNotice->setVisible(false);
    
    
    if(refreshCard->isWearEquip(10))
    {
        if(refreshCard->hisEquipCanAdv(10) || refreshCard->hisEquipCanStreng(10))
            kaijiaNotice->setVisible(true);
        else
            kaijiaNotice->setVisible(false);
    }
    else if(refreshCard->canWearEquip(10) )
        kaijiaNotice->setVisible(true);
    else
        kaijiaNotice->setVisible(false);
    
    
    if(refreshCard->isWearEquip(11))
    {
        if(refreshCard->hisEquipCanAdv(11) || refreshCard->hisEquipCanStreng(11))
            peishiNotice->setVisible(true);
        else
            peishiNotice->setVisible(false);
    }
    else if(refreshCard->canWearEquip(11) )
        peishiNotice->setVisible(true);
    else
        peishiNotice->setVisible(false);
    
    
    if(refreshCard->isWearEquip(12))
    {
        if(refreshCard->hisEquipCanAdv(12) || refreshCard->hisEquipCanStreng(12))
            zuojiNotice->setVisible(true);
        else
            zuojiNotice->setVisible(false);
    }
    else if(refreshCard->canWearEquip(12))
        zuojiNotice->setVisible(true);
    else
        zuojiNotice->setVisible(false);
    
}

void SGGeneralInfoLayer::developingBtnCallback()
{
     //cgpMain
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();

    if(player->getDevOfficerLimitPlayerLev() > player->getPlayerLevel())
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat("主公达到%d级开启此功能" , player->getDevOfficerLimitPlayerLev() )->getCString());
        return ;
    }
    else if(player->getDevOfficerLimitLev() > _card->getCurrLevel())
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat("武将达到%d级开启此功能" , player->getDevOfficerLimitLev() )->getCString());
        return;
    }
    
    main::DevelopOfficerDetailRequest * res = new main::DevelopOfficerDetailRequest();
    res->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_OFFICER_DEVELOPING, res);
}

void SGGeneralInfoLayer::onDevelopingReponse(cocos2d::CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if(sr)
    {
        main::DevelopOfficerDetailResponse * response = (main::DevelopOfficerDetailResponse *)sr->m_msg;
        CCLog("&m_msg = %p , &reponse = %p" , sr->m_msg , response);
        if(response)
        {
            DevelopingOfficerData data;
            CCLog("&data = %p , pointInfo[0] = %p , pointInfo[2] = %p" , &data, &data.pointInfo.a[0] , &data.pointInfo.a[2]);
            data.isSaved = response->saved();
            
            data.deveCost.normal_coin = response->norcoins();
            data.deveCost.normal_material = response->noritemnum();
            data.deveCost.careful_gold = response->spegold();
            data.deveCost.careful_material = response->speitemnum();
            
            main::DevPointDetail devPointDetail = response->pointdetail();
            data.pointInfo.a[0] = devPointDetail.devpoint();
            data.pointInfo.a[1] = devPointDetail.adddevpoint();
            data.pointInfo.a[2] = devPointDetail.maxdevpoint();
            
            main::DevOfficerDetail devOfficerDetail = response->officerdetail();
            data.nowAttribute.a[0] = devOfficerDetail.extatk();
            data.nowAttribute.a[1] = devOfficerDetail.extdef();
            data.nowAttribute.a[2] = devOfficerDetail.exthp();
            data.nowAttribute.a[3] = devOfficerDetail.extsp();
            data.nowAttribute.b[0] = devOfficerDetail.maxatk();
            data.nowAttribute.b[1] = devOfficerDetail.maxdef();
            data.nowAttribute.b[2] = devOfficerDetail.maxhp();
            data.nowAttribute.b[3] = devOfficerDetail.maxsp();
            
            main::NoSavedDetail noSavedDetail = response->nosaveddetail();
            data.preview.a[0] = noSavedDetail.atk();
            data.preview.a[1] = noSavedDetail.def();
            data.preview.a[2] = noSavedDetail.hp();
            data.preview.a[3] = noSavedDetail.sp();
            
            SGDevelopingOfficerLayer * layer = SGDevelopingOfficerLayer::create(data, _card, enterType);
            SGMainManager::shareMain()->showLayer(layer , true);
        }
    }
}









