#include "SGDevelopingOfficerLayer.h"
#include "SGAddDevelopingPointBox.h"
#include "ResourceManager.h"
#include "SGShowString.h"
#include "SGMainManager.h"
#include "SGDrawCards.h"
#include "SGTools.h"
#include "SGHelpBookInfo.h"
#include "Developing.pb.h"
#include "SGCantVisitBox.h"
#include "SGTouchLayer.h"
#include "SGCommonInfoBox.h"
#include "SGGuideManager.h"

#define ATTRIBUTE_BG_TAG 21
#define CURRENCY_BG_TAG 22
#define COST_BG_TAG 22

#define ATK_DEF_MORALE_SPEED 200

#define POS_FOR_ANIMATE_TAG 300


int getConsumeNumById(int itemId)
{
    int num =0;
    CCArray * arr = SGPlayerInfo::sharePlayerInfo()->getConsumableCards();
    for(int i=0 ; i<arr->count() ; ++i)
    {
        SGConsumableCard * temp = dynamic_cast<SGConsumableCard *>(arr->objectAtIndex(i));
        if(temp->getItemId() == itemId)
        {
            num = temp->getCurrNum();
            break;
        }
    }
    return num;
}


SGDevelopingOfficerLayer::SGDevelopingOfficerLayer()
: m_state(false)
, m_developingModel(1)
, m_onceBtn(NULL)
, m_tenBtn(NULL)
, m_saveBtn(NULL)
, m_cancelBtn(NULL)
, m_developingPointLab(NULL)
, m_coinLab(NULL)
, m_goldLab(NULL)
, m_coinNeedLab(NULL)
, m_goldNeedLab(NULL)
, m_beginning(false)
, m_pei_yang_dan_id(352003)
{
    memset(equipsAdd, 0, sizeof(equipsAdd));
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
}

SGDevelopingOfficerLayer::~SGDevelopingOfficerLayer()
{
    ResourceManager * resM =  ResourceManager::sharedInstance();
    resM->unBindLayerTexture(sg_developingOfficerLayer);
    
    SGNotificationCenter * notif = SGNotificationCenter::sharedNotificationCenter();
    notif->removeObserver(this, MSG_BEGIN_DEVELOPING);
    notif->removeObserver(this, MSG_SAVE_DEVELOPING);
}

SGDevelopingOfficerLayer * SGDevelopingOfficerLayer::create(const DevelopingOfficerData & data , SGOfficerCard * card , int generinfoType)
{
    SGDevelopingOfficerLayer * ret = new SGDevelopingOfficerLayer();
    if(ret && ret->init(NULL, sg_developingOfficerLayer))
    {
        ret->m_preview = data.preview;
        ret->m_nowAttribute = data.nowAttribute;
        ret->m_pointInfo = data.pointInfo;
        ret->m_deveCost = data.deveCost;
        ret->m_card = card;
        ret->m_generinfoType = generinfoType;
        ret->m_state = data.isSaved;
        ret->initView();
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    
    return ret;
}

void SGDevelopingOfficerLayer::initView()
{
    //picture
    ResourceManager * resM =  ResourceManager::sharedInstance();
    resM->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI , sg_developingOfficerLayer);
    resM->bindTexture("sgDevelopingOfficerLayer/sgDevelopingOfficerLayer.plist", RES_TYPE_LAYER_UI , sg_developingOfficerLayer);
    resM->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI , sg_developingOfficerLayer);
    resM->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_developingOfficerLayer);
    resM->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_developingOfficerLayer);
    resM->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_developingOfficerLayer);
    //培养动画资源
    resM->bindTexture("animationFile/officer_developing_effect1.plist", RES_TYPE_LAYER_UI, sg_developingOfficerLayer);
    resM->bindTexture("animationFile/officer_developing_effect2.plist", RES_TYPE_LAYER_UI, sg_developingOfficerLayer);

    //listen msg
    SGNotificationCenter * notif = SGNotificationCenter::sharedNotificationCenter();
    notif->addObserver(MSG_BEGIN_DEVELOPING, this,callfuncO_selector(SGDevelopingOfficerLayer::developingResponse));
    notif->addObserver(MSG_SAVE_DEVELOPING, this, callfuncO_selector(SGDevelopingOfficerLayer::saveDevelopingResponse));
    
    //compute
//    CCArray * equips = m_card->getOfficerEquips();
//    if(equips)
//    {
//        for(int i=0 ; i<equips->count() ; i++)
//        {
//            SGEquipCard * card = dynamic_cast<SGEquipCard*>(equips->objectAtIndex(i));
//            equipsAdd[0] += card->getAtk();
//            equipsAdd[1] += card->getDef();
//            equipsAdd[2] += card->getMorale();
//            equipsAdd[3] += card->getSpeed();
//        }
//    }
    
    //ui
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //几个ui区域位置适配
    float adaptPosY = 0.0;
    if(winSize.height == 1024)
        adaptPosY = 10.0;
    else if (winSize.height == 1136)
        adaptPosY = 60.0;
    //大背景
    CCSprite *bigbg = CCSprite::createWithSpriteFrameName("storyBg.png");
    bigbg->setScaleY((winSize.height-112- 70)/ bigbg->getContentSize().height);
    bigbg->setScaleX(winSize.width / bigbg->getContentSize().width);
    bigbg->setAnchorPoint(ccp(0.5, 0));
    bigbg->setPosition(ccp(winSize.width*0.5 , 112));
    this->addChild(bigbg,-2);
    
    //标题部分
    CCSprite * title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);
    
    CCSprite * titleBgStick = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titleBgStick->setAnchorPoint(ccp(0.5, 0));
    titleBgStick->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titleBgStick,2);
    titleBgStick->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,10);
    
    SGCCLabelTTF * titleText = SGCCLabelTTF::create(str_developing_officer_11, FONT_PANGWA, 36 , COLOR_LIGHT_YELLOW);
    titleText->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*0.5)));
    this->addChild(titleText);
    
    //back btn
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGDevelopingOfficerLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    this->addBtn(backBtn);
    //-----------------标题部分结束
    
    CCSprite * attributeBg = CCSprite::createWithSpriteFrameName("officer_developing_info_border.png");
    attributeBg->setPosition(ccpAdd(titleBgStick->getPosition(), ccp(0, -title_bg_l->getContentSize().height*0.7 - attributeBg->getContentSize().height*0.5  -  adaptPosY)));
    this->addChild(attributeBg ,1 ,ATTRIBUTE_BG_TAG);
    
    //红填充图
    CCScale9Sprite * attributeRedBg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuangbg.png");
    attributeRedBg->setPreferredSize(CCSize(600 , 274));
    attributeRedBg->setPosition(ccp(attributeBg->getPositionX(), attributeBg->getPositionY()-attributeBg->getContentSize().height*0.5 + attributeRedBg->getContentSize().height*0.5));
    this->addChild(attributeRedBg , 0 , POS_FOR_ANIMATE_TAG);

    //潜力点
    m_developingPointLab = SGCCLabelTTF::create(CCString::createWithFormat(str_developing_officer_1 , m_pointInfo.a[0])->getCString(), FONT_PANGWA, 28 , COLOR_UNKNOW_TAN);
    m_developingPointLab->setAnchorPoint(ccp(0, 0.5));
    m_developingPointLab->setPosition(ccp( 30 , 310));
    attributeBg->addChild(m_developingPointLab);
    
    //增加潜力点btn
    SGButton * addDevelopingPoint = SGButton::create("firstpageplus.png", NULL, this, menu_selector(SGDevelopingOfficerLayer::addDevelopingPointCallback));
    addDevelopingPoint->setPosition(ccp(winSize.width*0.5 - attributeBg->getContentSize().width*0.5 + 240, attributeBg->getPositionY() - attributeBg->getContentSize().height*0.5 + m_developingPointLab->getPosition().y - 2));
    this->addBtn(addDevelopingPoint);
    
    //规则btn
    SGButton * rule = SGButton::create("battle_rules.png", NULL, this, menu_selector(SGDevelopingOfficerLayer::ruleCallback));
    rule->setAnchorPoint(ccp(1, 0.5));
    rule->setPosition(ccp(attributeBg->getPositionX() + attributeBg->getContentSize().width*0.5 , addDevelopingPoint->getPositionY()));
    this->addBtn(rule);
    
    //武将头像
    CCSprite * officer = SGDrawCards::createSimpleSprite(m_card->getItemId(), 6, sg_developingOfficerLayer);
    officer->setPosition(ccp(80, 170));
    attributeBg->addChild(officer);
    
    //等级
    SGCCLabelTTF * level = SGCCLabelTTF::create(CCString::createWithFormat(" Lv %d" , m_card->getCurrLevel())->getCString(), FONT_PANGWA, 24);
    level->setAnchorPoint(ccp(0.5, 0));
    level->setPosition(ccp(officer->getContentSize().width*0.5 , 0));
    officer->addChild(level);
    
    //等级背景
    CCScale9Sprite * nameBg  = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(7,7,86,86));
    nameBg->setPreferredSize(CCSize(120 , 50));
    nameBg->setPosition(ccpAdd(officer->getPosition(), ccp(0, -officer->getContentSize().height*0.5 - 30 - nameBg->getContentSize().height*0.5)));
    attributeBg->addChild(nameBg);
    
    SGCCLabelTTF * name = SGCCLabelTTF::create(m_card->getOfficerName()->getCString(), FONT_PANGWA, 28 , ccGREEN);
    name->setPosition(nameBg->getContentSize()/2);
    nameBg->addChild(name);
    
    /*攻 防 血 速*/
    //背景
    CCScale9Sprite * bg2 = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png");
    bg2->setPreferredSize(CCSize(440 , 220));
    bg2->setPosition(ccp( officer->getPositionX() + officer->getContentSize().width*0.5 + 20 + bg2->getContentSize().width*0.5 , 0.5*(officer->getPositionY() + nameBg->getPositionY() )));
    attributeBg->addChild(bg2);
    float delayPosY = 15;
    CCPoint anchorPoint(0,0.5);
    SGCCLabelTTF * atkLab = SGCCLabelTTF::create(CCString::createWithFormat(str_CardItem_str5 , m_card->getAtk() + equipsAdd[0])->getCString() , FONT_BOXINFO, 28);
    atkLab->setAnchorPoint(anchorPoint);
    atkLab->setPosition(ccp(bg2->getPositionX() - bg2->getContentSize().width*0.5 + 30 , bg2->getPositionY() + bg2->getContentSize().height*0.5 - 30));
    attributeBg->addChild(atkLab , 10, ATK_DEF_MORALE_SPEED);
    
    SGCCLabelTTF * defLab = SGCCLabelTTF::create(CCString::createWithFormat(str_CardItem_str7 , m_card->getDef()+equipsAdd[1])->getCString(), FONT_BOXINFO, 28);
    defLab->setAnchorPoint(anchorPoint);
    defLab->setPosition(ccpAdd(atkLab->getPosition(), ccp(0, -defLab->getContentSize().height - delayPosY)));
    attributeBg->addChild(defLab , 10 ,ATK_DEF_MORALE_SPEED+1);
    
    SGCCLabelTTF * moraleLab = SGCCLabelTTF::create(CCString::createWithFormat(str_CardItem_str4 , m_card->getMorale() + equipsAdd[2])->getCString(), FONT_BOXINFO, 28);
    moraleLab->setAnchorPoint(anchorPoint);
    moraleLab->setPosition(ccpAdd(defLab->getPosition() , ccp(0, -defLab->getContentSize().height - delayPosY)));
    attributeBg->addChild(moraleLab ,10 , ATK_DEF_MORALE_SPEED+2);
    
    SGCCLabelTTF * speedLab = SGCCLabelTTF::create(CCString::createWithFormat(str_CardItem_str6 , (int)m_card->getSpeed() + equipsAdd[3])->getCString(), FONT_BOXINFO, 28);
    speedLab->setAnchorPoint(anchorPoint);
    speedLab->setPosition(ccpAdd(moraleLab->getPosition() , ccp(0, -defLab->getContentSize().height - delayPosY)));
    attributeBg->addChild(speedLab , 10 , ATK_DEF_MORALE_SPEED+3);
    
    //引导tag
    attributeBg->setTag(SGStaticDataManager::shareStatic()->getimitTagById(18, 11));
    
    //培养结果预览页面、未开始培养 展示增加属性lab
    this->createLabels(attributeBg , ccpAdd(atkLab->getPosition() , ccp(atkLab->getContentSize().width  , 0)) , delayPosY);
    
    /*培养一次 十次  放弃 保存的 按钮*/
    m_onceBtn = SGButton::createFromLocal("visit_anniu.png", str_developing_officer_4, this, menu_selector(SGDevelopingOfficerLayer::deveOnce) , CCPointZero , FONT_PANGWA ,ccWHITE , 30);
    m_onceBtn->setAnchorPoint(ccp(1, 0));
    m_onceBtn->setPosition(ccp(winSize.width*0.5 - 10 , 130 + adaptPosY / 2));
    this->addBtn(m_onceBtn);
    //引导使用的tag
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(18, 9);
    m_onceBtn->setTag(tag);
    
    m_tenBtn = SGButton::createFromLocal("visit_anniu.png", str_developing_officer_5, this, menu_selector(SGDevelopingOfficerLayer::deveTen) , CCPointZero , FONT_PANGWA ,ccWHITE , 30);
    m_tenBtn->setAnchorPoint(ccp(0, 0));
    m_tenBtn->setPosition(winSize.width*0.5 + 10 , m_onceBtn->getPositionY());
    this->addBtn(m_tenBtn);
    
    m_saveBtn = SGButton::createFromLocal("box_btnbg.png", str_developing_officer_6, this, menu_selector(SGDevelopingOfficerLayer::saveCallback) , CCPointZero , FONT_PANGWA ,ccWHITE , 32);
    m_saveBtn->setAnchorPoint(ccp(1, 0));
    m_saveBtn->setPosition(ccpAdd(m_onceBtn->getPosition() , ccp(-20, 0)) );
    this->addBtn(m_saveBtn);
    //设置引导需要的tag
    m_saveBtn->setTag(SGStaticDataManager::shareStatic()->getimitTagById(18, 13));
    
    m_cancelBtn = SGButton::createFromLocal("box_btnbg.png", str_developing_officer_7, this, menu_selector(SGDevelopingOfficerLayer::cancelCallback) , CCPointZero , FONT_PANGWA ,ccWHITE , 32);
    m_cancelBtn->setAnchorPoint(ccp(0, 0));
    m_cancelBtn->setPosition(ccpAdd(m_tenBtn->getPosition() , ccp(20, 0) ) );
    this->addBtn(m_cancelBtn);
    
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    //货币的背景
    CCScale9Sprite * currencyOutBg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    currencyOutBg->setPreferredSize(CCSize( attributeBg->getContentSize().width, 70));
    
    CCScale9Sprite * downBg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");//消耗区域bg
    downBg->setPreferredSize(CCSize(attributeBg->getContentSize().width , 190));
    
    //float borderDelayPosY = (attributeBg->getPositionY()-attributeBg->getContentSize().height*0.5 - m_onceBtn->getPositionY() - m_onceBtn->getContentSize().height - currencyOutBg->getContentSize().height - downBg->getContentSize().height)/3;
    float borderDelayPosY = 0.0;
    if(960 == winSize.height)
    {
        borderDelayPosY = 20;
    }
    else if(1024 == winSize.height)
    {
        borderDelayPosY = 30;
    }
    else
        borderDelayPosY = 30.0;
    currencyOutBg->setPosition(ccp(attributeBg->getPositionX() , attributeBg->getPositionY() - attributeBg->getContentSize().height*0.5 - borderDelayPosY - currencyOutBg->getContentSize().height*0.5 ));
    this->addChild(currencyOutBg ,1 );
    
    CCScale9Sprite * currencyOutRedBg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuangbg.png" , CCRect(5,5,170,170));
    currencyOutRedBg->setPreferredSize(CCSize(attributeBg->getContentSize().width , 70));
    currencyOutRedBg->setPosition(currencyOutBg->getPosition());
    this->addChild(currencyOutRedBg , 0);
    
    //铜钱icon 和 lab
    CCScale9Sprite * coinBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(20 ,20 , 20,60));
    coinBg->setPreferredSize(CCSize(220 , 50));
    coinBg->setAnchorPoint(ccp(1, 0.5));
    coinBg->setPosition(ccp(currencyOutBg->getContentSize().width*0.5 - 30, currencyOutBg->getContentSize().height*0.5));
    currencyOutBg->addChild(coinBg);
    
    CCSprite * coinIcon = CCSprite::createWithSpriteFrameName("coinicon.png");
    coinIcon->setPosition(ccp(coinIcon->getContentSize().width*0.6 , coinBg->getContentSize().height*0.5));
    coinBg->addChild(coinIcon);
    
    m_coinLab = SGCCLabelTTF::create(CCString::createWithFormat("%d" , player->getPlayerCoins())->getCString(), FONT_BOXINFO, 28);
    m_coinLab->setAnchorPoint(ccp(0, 0.5));
    m_coinLab->setPosition(ccp(coinIcon->getPositionX() + coinIcon->getContentSize().width*0.5 + 10 , coinBg->getContentSize().height*0.5));
    coinBg->addChild(m_coinLab);
    
    //元宝背景
    CCScale9Sprite * goldBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png",CCRect(20 ,20 , 20,60));
    goldBg->setPreferredSize(CCSize(220 , 50));
    goldBg->setAnchorPoint(ccp(0, 0.5));
    goldBg->setPosition(ccp(currencyOutBg->getContentSize().width*0.5 +  30, currencyOutBg->getContentSize().height*0.5));
    currencyOutBg->addChild(goldBg);
    
    CCSprite * goldIcon = CCSprite::createWithSpriteFrameName("store_yb.png");
    goldIcon->setPosition(ccp(goldIcon->getContentSize().width*0.5 + 10 , goldBg->getContentSize().height*0.5));
    goldBg->addChild(goldIcon);
    
    m_goldLab = SGCCLabelTTF::create(CCString::createWithFormat("%d" , player->getPlayerGold())->getCString(), FONT_BOXINFO, 28);
    m_goldLab->setAnchorPoint(ccp(0, 0.5));
    m_goldLab->setPosition(ccp(goldIcon->getPositionX() + goldIcon->getContentSize().width*0.5 + 10 , goldBg->getContentSize().height*0.5));
    goldBg->addChild(m_goldLab);
    
    //消耗区域
    int costFontSize = 32;
    downBg->setPosition(ccp(attributeBg->getPositionX() , currencyOutBg->getPositionY() - currencyOutBg->getContentSize().height*0.5 - borderDelayPosY - downBg->getContentSize().height*0.5));
    this->addChild(downBg);
    CCScale9Sprite * downRedBg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuangbg.png");
    downRedBg->setPreferredSize(downBg->getContentSize());
    downRedBg->setPosition(downBg->getPosition());
    this->addChild(downRedBg , downBg->getZOrder() -1);
    
    //引导需要高亮
    downBg->setTag(SGStaticDataManager::shareStatic()->getimitTagById(18, 7));
    
    
    //普通培养
    CCScale9Sprite * normal = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(10, 10, 80, 80));
    normal->setPreferredSize(CCSize(570 , 60));
    normal->setPosition(ccp(downBg->getContentSize().width*0.5, downBg->getContentSize().height*0.5 + 5 + normal->getContentSize().height*0.5));
    downBg->addChild(normal);
    
    SGButton * emptyPoint = SGButton::create("officer_developing_emptypoint.png", NULL, this, menu_selector(SGDevelopingOfficerLayer::choseNormalCallback));
    emptyPoint->setPosition(ccp(winSize.width*0.5 - downBg->getContentSize().width*0.5 + 40, downBg->getPositionY()-downBg->getContentSize().height*0.5 + normal->getPositionY()));
    this->addBtn(emptyPoint);
    
    CCSprite * normalName = CCSprite::createWithSpriteFrameName("pu_tong.png");
    normalName->setPosition(ccp(80, normal->getContentSize().height*0.5));
    normal->addChild(normalName);
    
    SGCCLabelTTF * normalDes = SGCCLabelTTF::create(str_developing_officer_8, FONT_BOXINFO, costFontSize-10);
    normalDes->setPosition(ccp(-5 + normalName->getPosition().x + normalName->getContentSize().width*0.5 + normalDes->getContentSize().width*0.5 , normalName->getPosition().y));
    normal->addChild(normalDes);
    
    CCSprite * coinIcon2 = CCSprite::createWithSpriteFrameName("coinicon.png");
    coinIcon2->setPosition(ccp(normalDes->getPosition().x + normalDes->getContentSize().width*0.5 + coinIcon2->getContentSize().width*0.5  , normal->getContentSize().height*0.5));
    normal->addChild(coinIcon2);
    
    CCScale9Sprite * needBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(7,7,86,86));
    needBg->setPreferredSize(CCSize(110,30));
    needBg->setPosition(ccpAdd(coinIcon2->getPosition(), ccp(coinIcon2->getContentSize().width*0.5 + needBg->getContentSize().width*0.5 + 10, 0)));
    normal->addChild(needBg);
    
    m_coinNeedLab = SGCCLabelTTF::create(CCString::createWithFormat("%d" , m_deveCost.normal_coin)->getCString(), FONT_BOXINFO, costFontSize);
    m_coinNeedLab->setPosition(needBg->getContentSize()/2);
    needBg->addChild(m_coinNeedLab);
    
    CCSprite* consumeIcon = CCSprite::createWithSpriteFrameName("pei_yang_dan_small_v.png");
    consumeIcon->setPosition(ccpAdd(needBg->getPosition(), ccp(needBg->getContentSize().width*0.5 + consumeIcon->getContentSize().width*0.5 + 16, 0)));
    normal->addChild(consumeIcon);
    
    CCScale9Sprite * materialNeedBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(7,7,86,86));
    materialNeedBg->setPreferredSize(CCSize(115,needBg->getContentSize().height));
    materialNeedBg->setPosition(ccp(normal->getContentSize().width - materialNeedBg->getContentSize().width*0.5 - 10, needBg->getPositionY()));
    normal->addChild(materialNeedBg);
    
    m_materialNeedLab = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d" ,getConsumeNumById(m_pei_yang_dan_id),m_deveCost.normal_material)->getCString(), FONT_BOXINFO, costFontSize);
    m_materialNeedLab->setPosition(materialNeedBg->getContentSize()/2);
    materialNeedBg->addChild(m_materialNeedLab);
    
    //精心培养
    CCScale9Sprite * careful = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(10, 10, 80, 80));
    careful->setPreferredSize(normal->getContentSize());
    careful->setPosition(ccp(downBg->getContentSize().width*0.5, downBg->getContentSize().height*0.5 - 5 - normal->getContentSize().height*0.5));
    downBg->addChild(careful);
    
    SGButton * emptyPoint2 = SGButton::create("officer_developing_emptypoint.png", NULL, this, menu_selector(SGDevelopingOfficerLayer::choseCarefunCallback));
    emptyPoint2->setPosition(ccp(emptyPoint->getPositionX(), downBg->getPositionY()-downBg->getContentSize().height*0.5 + careful->getPositionY()));
    this->addBtn(emptyPoint2);
    
    CCSprite * carefulName = CCSprite::createWithSpriteFrameName("jing_xin.png");
    carefulName->setPosition(ccp(80, careful->getContentSize().height*0.5));
    careful->addChild(carefulName);
    
    SGCCLabelTTF * carefulDes = SGCCLabelTTF::create(str_developing_officer_8, FONT_BOXINFO, costFontSize-10);
    carefulDes->setPosition(ccp(-5 + carefulName->getPosition().x + carefulName->getContentSize().width*0.5 + carefulDes->getContentSize().width*0.5 , carefulName->getPosition().y));
    careful->addChild(carefulDes);
    
    CCSprite * goldIcon2 = CCSprite::createWithSpriteFrameName("store_yb.png");
    goldIcon2->setPosition(ccp(carefulDes->getPosition().x + carefulDes->getContentSize().width*0.5 + goldIcon2->getContentSize().width*0.5 , careful->getContentSize().height*0.5));
    careful->addChild(goldIcon2);
    
    CCScale9Sprite * needGoldBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png",CCRect(7,7,86,86));
    needGoldBg->setPreferredSize(CCSize(110,30));
    needGoldBg->setPosition(ccpAdd(goldIcon2->getPosition(), ccp(goldIcon2->getContentSize().width*0.5 + needGoldBg->getContentSize().width*0.5 + 10, 0)));
    careful->addChild(needGoldBg);
    
    m_goldNeedLab = SGCCLabelTTF::create(CCString::createWithFormat("%d" , m_deveCost.careful_gold)->getCString(), FONT_BOXINFO, costFontSize);
    m_goldNeedLab->setPosition(needGoldBg->getContentSize()/2);
    needGoldBg->addChild(m_goldNeedLab);
    
    CCSprite* consumeIcon2 = CCSprite::createWithSpriteFrameName("pei_yang_dan_small_v.png");
    consumeIcon2->setPosition(ccp(consumeIcon->getPositionX() , needGoldBg->getPositionY() ));
    careful->addChild(consumeIcon2);
    
    CCScale9Sprite * materialNeedBg2 = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png",CCRect(7,7,86,86));
    materialNeedBg2->setPreferredSize(CCSize(115,needGoldBg->getContentSize().height));
    materialNeedBg2->setPosition(ccp(careful->getContentSize().width - materialNeedBg2->getContentSize().width*0.5 - 10, needGoldBg->getPositionY()));
    careful->addChild(materialNeedBg2);
    
    m_materialNeedLab2 = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d" ,getConsumeNumById(m_pei_yang_dan_id),m_deveCost.careful_material)->getCString(), FONT_BOXINFO, costFontSize);
    m_materialNeedLab2->setPosition(materialNeedBg2->getContentSize()/2);
    materialNeedBg2->addChild(m_materialNeedLab2);
    
    //选中状态亮点
    lightPoint = CCSprite::createWithSpriteFrameName("officer_developing_solidpoint.png");
    lightPoint->setPosition(emptyPoint->getPosition());
    this->addChild(lightPoint ,100);
    
    m_touchLayer = SGTouchLayer::create(120,120, 0);
    m_touchLayer->ignoreAnchorPointForPosition(false);
    m_touchLayer->setAnchorPoint(ccp(0.5, 0.5));
    m_touchLayer->setPosition(ccp(consumeIcon2->getPositionX(), downBg->getContentSize().height*0.5));
    m_touchLayer->setButtonMode( this, pf_callback_selector(SGDevelopingOfficerLayer::showComsumeInfo) );
    downBg->addChild(m_touchLayer);
    //需要刷新页面
    this->refreshAfterOperator();
}

void SGDevelopingOfficerLayer::refreshAfterOperator()
{
    bool visible = m_state;
    for(int i=0 ; i<4; i++)
    {
        SGCCLabelTTF * lab = dynamic_cast<SGCCLabelTTF*>(getChildByTag(ATTRIBUTE_BG_TAG)->getChildByTag(1000+i*2));
        //lab->setVisible(visible);
        if (lab)
        {
            (dynamic_cast<CCNode*>(getChildByTag(ATTRIBUTE_BG_TAG)->getChildByTag(1000+i*2+1)))->setVisible(visible);
        //if(visible)
            lab->setString(CCString::createWithFormat("( +%d" , m_nowAttribute.a[i])->getCString());
        }
        lab = dynamic_cast<SGCCLabelTTF*>(getChildByTag(ATTRIBUTE_BG_TAG)->getChildByTag(1001+i*2));
        if (lab)
        {
            lab->setVisible(visible);
        }
        if(visible && lab)
            lab->setString(CCString::createWithFormat(str_developing_officer_3 , m_nowAttribute.b[i])->getCString());
    }
    
    //预览lab
    visible = !m_state;
    for(int i=0 ; i<4; i++)
    {
        SGCCLabelTTF * lab = dynamic_cast<SGCCLabelTTF*>(getChildByTag(ATTRIBUTE_BG_TAG)->getChildByTag(100+i*2));
        if (lab)
        {
            lab->setVisible(visible);
        }
        if(m_preview.a[i]==0 && lab)
            lab->setVisible(false);
        if(visible && m_preview.a[i]!= 0)
        {
            if(m_preview.a[i] > 0 && lab)
            {
                lab->setString(CCString::createWithFormat("+%d" , m_preview.a[i])->getCString());
                lab->setInsideColor(ccGREEN);
            }
            else
            {
                if (lab)
                {
                    lab->setString(CCString::createWithFormat("%d" , m_preview.a[i])->getCString());
                    lab->setInsideColor(ccYELLOW);
                }
            }
        }
        //括号
        lab = dynamic_cast<SGCCLabelTTF*>(getChildByTag(ATTRIBUTE_BG_TAG)->getChildByTag(101+i*2));
        if (lab)
            lab->setVisible(visible);
    }
    
    this->setBtnEnabled(m_state);
    
    m_materialNeedLab->setString(CCString::createWithFormat("%d/%d" ,getConsumeNumById(m_pei_yang_dan_id),m_deveCost.normal_material)->getCString());
    m_materialNeedLab2->setString(CCString::createWithFormat("%d/%d" ,getConsumeNumById(m_pei_yang_dan_id),m_deveCost.careful_material)->getCString());
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    m_coinLab->setString(CCString::createWithFormat("%d" , player->getPlayerCoins())->getCString());
    m_goldLab->setString(CCString::createWithFormat("%d" , player->getPlayerGold())->getCString());
    
    //刷潜力点
    this->refreshView();
}

void SGDevelopingOfficerLayer::setBtnEnabled(bool enable)
{
    m_beginning = !enable;
    
    m_onceBtn->setVisible(enable);
    m_onceBtn->setEnabled(enable);
    m_tenBtn->setVisible(enable);
    m_tenBtn->setEnabled(enable);
    
    m_saveBtn->setVisible(!enable);
    m_saveBtn->setEnabled(!enable);
    m_cancelBtn->setVisible(!enable);
    m_cancelBtn->setEnabled(!enable);
}

void SGDevelopingOfficerLayer::refreshAttribute()
{
    SGCCLabelTTF * lab = NULL;
    int arr[] = {m_card->getAtk() + equipsAdd[0] , m_card->getDef() + equipsAdd[1] , m_card->getMorale() +equipsAdd[2] , (int) (m_card->getSpeed()) + equipsAdd[3] };
    const char * str[] = {str_CardItem_str5 , str_CardItem_str7 , str_CardItem_str4 , str_CardItem_str6};
    for(int i=0 ; i<4 ; ++i)
    {
        lab = dynamic_cast<SGCCLabelTTF*>(getChildByTag(ATTRIBUTE_BG_TAG)->getChildByTag(ATK_DEF_MORALE_SPEED+i));
        if (lab)
        {
            lab->setString(CCString::createWithFormat(str[i] , arr[i])->getCString());
        }
    }
}


//约定培养结果预览页面攻防血速 tag 100 、101.。。。
//未开始培养 攻防血速 tag 1000 、 1001.。。。1007
void SGDevelopingOfficerLayer::createLabels(CCNode * parent , CCPoint point , float delayPosy)
{
    SGCCLabelTTF * lab = NULL;
    CCPoint anchorPoint = ccp(0 , 0.5);
    float delayPosX = 10;
    int fontSize = 28;
    for(int i =0 ; i< 4; i++)
    {
        lab = SGCCLabelTTF::create("(+123456", FONT_BOXINFO, fontSize);
        lab->setAnchorPoint(anchorPoint);
        lab->setPosition(ccpAdd(point, ccp( delayPosX , -1*i*(lab->getContentSize().height + delayPosy))));
        parent->addChild(lab , 5 , i*2 + 1000);
        lab = SGCCLabelTTF::create(CCString::createWithFormat(str_developing_officer_3 , m_nowAttribute.b[i])->getCString(), FONT_BOXINFO, fontSize);
        lab->setAnchorPoint(anchorPoint);
        lab->setPosition(ccpAdd( parent->getChildByTag(2*i + 1000)->getPosition()  , ccp(parent->getChildByTag(i*2 + 1000)->getContentSize().width + 5 , 0)));
        parent->addChild(lab , 5,i*2 + 1000 + 1);
    }
    
    //培养预览
    for(int i = 0 ; i < 4 ; ++i)
    {
        lab = SGCCLabelTTF::create("(+123456)", FONT_BOXINFO, fontSize , ccGREEN);
        lab->setAnchorPoint(anchorPoint);
        lab->setPosition(ccpAdd(point, ccp(120, -1*i*(lab->getContentSize().height + delayPosy) )));
        parent->addChild(lab , 5 , i*2 + 100);
        
        //右括号
        lab = SGCCLabelTTF::create(")", FONT_BOXINFO, fontSize);
        lab->setAnchorPoint(anchorPoint);
        lab->setPosition(ccpAdd(parent->getChildByTag(2*i + 100)->getPosition(), ccp(80, 0) ) );
        parent->addChild(lab , 5 , i*2 + 101);
    }
}

void SGDevelopingOfficerLayer::choseNormalCallback(cocos2d::CCObject *obj)
{
    CCNode * node = dynamic_cast<CCNode *>(obj);
    lightPoint->setPosition(node->getPosition());
    m_developingModel = 1;
    CCLog("chose %s . state = %d" , "普通培养" , m_developingModel);
}

void SGDevelopingOfficerLayer::choseCarefunCallback(cocos2d::CCObject * obj)
{
    CCNode * node = dynamic_cast<CCNode *>(obj);
    lightPoint->setPosition(node->getPosition());
    m_developingModel = 2;
    CCLog("chose %s . state = %d" , "精心培养" , m_developingModel);
}

void SGDevelopingOfficerLayer::addDevelopingPointCallback()
{
    showAddBox();
}


void SGDevelopingOfficerLayer::ruleCallback()
{
    SGHelpTitleDataModel *tit = new SGHelpTitleDataModel;
    tit->setbeg(1701);
    tit->setend(1725);
    tit->setnum(25);
    CCString *str = CCString::create(str_developing_officer_11);
    tit->settitle(str);
    
    SGHelpBookInfo *info = SGHelpBookInfo::create(tit,4);
    SGMainManager::shareMain()->showLayer(info , false);
    delete tit;
}

void SGDevelopingOfficerLayer::showAddBox()
{
    SGAddDevelopingPointBox * box = SGAddDevelopingPointBox::create(m_card , this, &m_pointInfo , &m_nowAttribute);
    SGMainManager::shareMain()->showBox(box);
}

void SGDevelopingOfficerLayer::deveOnce()
{
    if (SGGuideManager::shareManager()->isGuide && SGGuideManager::shareManager()->limitStep != 9)
    {
        return ;
    }
    sendDevelopingRequest(1);
}

void SGDevelopingOfficerLayer::deveTen()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    sendDevelopingRequest(10);
}

void SGDevelopingOfficerLayer::sendDevelopingRequest(int num)
{
    if(m_beginning)
        return;
    else
        m_beginning = true;
    
    bool moneyEnough = true;
    bool materialEnough = true;
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    if(1==m_developingModel)
    {
        if( num*m_deveCost.normal_material > getConsumeNumById(m_pei_yang_dan_id) )
            materialEnough=false;
        else if( num*m_deveCost.normal_coin > player->getPlayerCoins() )
            moneyEnough =false;
    }
    else
    {
        if( num*m_deveCost.careful_material > getConsumeNumById(m_pei_yang_dan_id) )
            materialEnough = false;
        else if( num*m_deveCost.careful_gold > player->getPlayerGold())
            moneyEnough = false;
    }
    if(!materialEnough)
    {
        SGMainManager::shareMain()->showMessage(str_developing_officer_18);
        m_beginning = false;
        return ;
    }
    else if(!moneyEnough)
    {
        if(1==m_developingModel)
        {
            SGBaseBox * box = SGCantVisitBox::create(NULL, 2);
            SGMainManager::shareMain()->showBox(box);
            
            m_beginning = false;
            return ;
        }
        else
        {
            SGBaseBox * box = SGCantVisitBox::create(NULL, 1);
            SGMainManager::shareMain()->showBox(box);
            
            m_beginning = false;
            return ;
        }
        
    }
    //判断点数是否足够
    bool isMax = m_nowAttribute.a[0]>=m_nowAttribute.b[0] && m_nowAttribute.a[1]>=m_nowAttribute.b[1] && m_nowAttribute.a[2]>=m_nowAttribute.b[2]&& m_nowAttribute.a[3]>=m_nowAttribute.b[3];
    if(1==num)//培养一次
    {
        if(m_pointInfo.a[0] > 1 && isMax)
        {
            SGMainManager::shareMain()->showMessage(str_developing_officer_20);
        }
    }
    
    //@:其余逻辑，客户端不做判断，由返回消息状态码给出对应提示。
    
    main::DevelopOfficerRequest * request = new main::DevelopOfficerRequest();
    request->set_ssid(m_card->getSsid());
    
    if(1==num)
        request->set_count(main::DEV_ONE);
    else
        request->set_count(main::DEV_TEN);
    
    if(1==m_developingModel)
        request->set_type(main::DEV_OFF_NOR);
    else
        request->set_type(main::DEV_OFF_SPE);
    
    SGSocketClient::sharedSocketClient()->send(MSG_BEGIN_DEVELOPING, request , false);
}

void SGDevelopingOfficerLayer::developingResponse(CCObject *sender)
{
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if(sr)
    {
        main::DevelopOfficerResponse * res = (main::DevelopOfficerResponse*)sr->m_msg;
        if(res)
        {
            int state = res->state();
            if(state==1)//培养成功  、出现预览
            {
                main::NoSavedDetail noSavedDetail = res->nosaveddetail();
                m_preview.a[0] = noSavedDetail.atk();
                m_preview.a[1] = noSavedDetail.def();
                m_preview.a[2] = noSavedDetail.hp();
                m_preview.a[3] = noSavedDetail.sp();
                
                m_state = false;
                
                //在这里先更新按钮的状态
                //this->setBtnEnabled(false);
                
                CCCallFunc * callFunc1 = CCCallFunc::create(this, callfunc_selector(SGDevelopingOfficerLayer::playAnimate));
                CCCallFunc * callFunc2 = CCCallFunc::create(this, callfunc_selector(SGDevelopingOfficerLayer::refreshAfterOperator));
                CCSequence * sequence = CCSequence::create(CCDelayTime::create(1) , callFunc2 , NULL);
                CCSpawn * action = CCSpawn::create(callFunc1,sequence , NULL);
                this->runAction(action);
                
                m_card->setHasSvaedDev(false);
            }
            else if(-1==state)
            {
                SGMainManager::shareMain()->showMessage(str_developing_officer_21);
            }
            else if(2==state)
            {
                SGMainManager::shareMain()->showMessage(str_developing_officer_22);
            }
            else if(3==state)
            {
                SGMainManager::shareMain()->showMessage(str_developing_officer_18);
            }
            else if(4==state)//铜钱不足
            {
                SGBaseBox * box = SGCantVisitBox::create(NULL, 2);
                SGMainManager::shareMain()->showBox(box);
            }
            else if(5==state)//元宝不足
            {
                SGBaseBox * box = SGCantVisitBox::create(NULL, 1);
                SGMainManager::shareMain()->showBox(box);
            }
            else if(6==state)
            {
                SGMainManager::shareMain()->showMessage(str_developing_officer_24);
            }
            else if(7==state)
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_developing_officer_25 , player->getDevOfficerLimitPlayerLev())->getCString());
            }
            else if(8==state)
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_developing_officer_26 , player->getDevOfficerLimitLev())->getCString());
            }
            
            if(state!=1)
                m_beginning = false;
        }
    }
}


void SGDevelopingOfficerLayer::saveCallback()
{
    main::SaveOfficerRequest * request = new main::SaveOfficerRequest();
    request->set_ssid(m_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_SAVE_DEVELOPING, request);
}

void SGDevelopingOfficerLayer::saveDevelopingResponse(CCObject * sender)
{
    SGSocketRequest * sr = (SGSocketRequest*)sender;
    if(sr)
    {
        main::SaveOfficerResponse * res = (main::SaveOfficerResponse*)sr->m_msg;
        if(res)
        {
            if(1==res->state())
            {
                main::DevOfficerDetail devOfficerDetail = res->officerdetail();
                m_nowAttribute.a[0] = devOfficerDetail.extatk();
                m_nowAttribute.a[1] = devOfficerDetail.extdef();
                m_nowAttribute.a[2] = devOfficerDetail.exthp();
                m_nowAttribute.a[3] = devOfficerDetail.extsp();
                
                m_nowAttribute.b[0] = devOfficerDetail.maxatk();
                m_nowAttribute.b[1] = devOfficerDetail.maxdef();
                m_nowAttribute.b[2] = devOfficerDetail.maxhp();
                m_nowAttribute.b[3] = devOfficerDetail.maxsp();
                
                main::DevPointDetail devPointDetail = res->pointdetail();
                m_pointInfo.a[0] = devPointDetail.devpoint();
                m_pointInfo.a[1] = devPointDetail.adddevpoint();
                m_pointInfo.a[2] = devPointDetail.maxdevpoint();
                
                m_state = true;
                this->refreshAfterOperator();
                refreshAttribute();
                
                m_card->setHasSvaedDev(true);
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_developing_officer_27);
            }
        }
    }
}

void SGDevelopingOfficerLayer::cancelCallback()
{
    m_card->setHasSvaedDev(true);
    
    main::GiveUpOfficerRequest * request = new main::GiveUpOfficerRequest();
    request->set_ssid(m_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_CANCEL_DEVELOPING, request , false);
    
    //属性无变化 从预览页面回到培养页面
    m_state = true;
    this->refreshAfterOperator();
}

void SGDevelopingOfficerLayer::playAnimate()
{
    CCSpriterX *newRewardEffect = CCSpriterX::create("animationFile/officer_developing_effect.scml", true, true);
    newRewardEffect->setPosition(ccpAdd(this->getChildByTag(POS_FOR_ANIMATE_TAG)->getPosition() , ccp(70, 0)));
    //newRewardEffect->setScale(0.9);
    newRewardEffect->setAnimaSpeed(0.5);
    this->addChild(newRewardEffect, 512);
    newRewardEffect->play();
}

void SGDevelopingOfficerLayer::showComsumeInfo()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    CCLog("培养丹详情。");
    SGMainManager::shareMain()->showCommonInfoBoxGoToArena(BIT_CONSUMABLE , m_pei_yang_dan_id , this);
}

void SGDevelopingOfficerLayer::setIsCanTouch(bool enable)
{
    SGBaseLayer::setIsCanTouch(enable);
    m_touchLayer->setIsWorking(enable);
}

void SGDevelopingOfficerLayer::backHandler()
{
    SGMainManager::shareMain()->showGeneralInfoLayer(m_card , m_generinfoType);
}

void SGDevelopingOfficerLayer::refreshView()
{
    if(m_developingPointLab)
        m_developingPointLab->setString(CCString::createWithFormat( str_developing_officer_1, m_pointInfo.a[0])->getCString());
}


//-----------------------------file end.
