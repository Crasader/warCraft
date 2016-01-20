 //
//  SGStrengLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-8.
//
//

#include "SGStrengLayer.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGCardItem.h"
#include "cocos-ext.h"
#include "SGStrengBox.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ItemModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGMenu.h"
#include "SGGeneralInfoLayer.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "SGMainLayer.h"
#include "SGStrengLvlUpBox.h"
#include "SGStrengSoldierLayer.h"
#include "SimpleAudioEngine.h"
#include "SGTeamgroup.h"
#include "SGEmbattleBox.h"
#include <algorithm>
#include "SGStrengEffectLayer.h"

#include "ResourceManager.h"
#include "SGStringConfig.h"

#define JUNYING_INFO_ENTER 10
#define HOME_INFO_ENTER 11
#define EMBATTLE_INFO_ENTER 12

#define LABEL_DEL	1
#define LABEL_ATK	2
#define LABEL_AP	3
#define LABEL_SP	4

static int SGLess(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getselectposition() < ((SGBaseMilitaryCard*)p2)->getselectposition();
}
SGStrengLayer::SGStrengLayer()
:cardCount(0)
,_card(NULL)
,add(true)
,selectposition(0)
,selectcardcount(0)
,officerNum(0)
,times(0)//11.25
,isautostrengenough(false)
,addFlags(0)
,_currentPage(0)
,unsortarrary(NULL)
,fermEffect(NULL)
,isfromfirst(0)
,leavenum(0)
,EmbattleInfo(NULL)
,onlyOfficer(false)
,Icon(NULL)
,choose(NULL)
,lab_lvl(NULL)
,lab_orilvl(NULL)
//,need(NULL)
,afterExp(NULL)
,skilluprate(NULL)
,maxe(NULL)
//,have(NULL)
,progress(NULL)
//,needCoins(0)
,officer(NULL)
,equip(NULL)
,soldier(NULL)
,_arraySelectCard(NULL)
,range(NULL)
,cardType(0)
,isSelect(0)
,okBtn(NULL)
,tabelItem(NULL)
,maxneedexp(0)
,lab_addack(NULL)
,lab_addSpeed(NULL)
,lab_adddef(NULL)
,lab_addmoral(NULL)
,strengLimitLevel(0)
,m_plabel_MaterialCount(NULL)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("strengofficersort", CCString::createWithFormat("%d",3)->getCString());
    CCUserDefault::sharedUserDefault()->setStringForKey("strengequipsort", CCString::createWithFormat("%d",3)->getCString());
    CCUserDefault::sharedUserDefault()->flush();
    
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true , false);
    
    memset(selectItem, 0, sizeof(selectItem));
}

SGStrengLayer::~SGStrengLayer()
{
     datas ->removeAllObjects();
    datas->release();
    _card->release();
    CC_SAFE_RELEASE(_arraySelectCard);
    CC_SAFE_RELEASE(unsortarrary);


    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_BARRACKS_STRENG);
    notification->removeObserver(this, MSG_EQUIP_STRENG);
    notification->removeObserver(this, MSG_SOLDIER_STRENG);
    notification->removeObserver(this, MSG_STRENG_SKILL);
    notification->removeObserver(this, MSG_STRENG_LORD);
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_strengLayer);

//    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
}
/*
 type = 1,{enter=11 阵容-武将详情-强化，enter=10背包-武将详情-强化 ， enter=4强化主动技进入 ， enter=1 强化-强化武将 ， enter=10 首页武将-强化。}
 type = 2
 type = 3,{enter=0 阵容-查看士兵-强化士兵, }
 type = 4,enter=5被动技强化
 isOnlyOfficer = true,{强化主将技能}
 */
SGStrengLayer *SGStrengLayer::create(SGBaseMilitaryCard *card, int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage)
{
    SGStrengLayer *strengLayer = new SGStrengLayer();
    
    if (strengLayer && strengLayer->init(card, type,isOnlyOfficer))
    {
		strengLayer->EmbattleInfo = info;//是否从布阵界面进入
        strengLayer->isfromfirst = enter;
        strengLayer->onlyOfficer = isOnlyOfficer;
        strengLayer->initView();
        strengLayer->_currentPage=currentPage;
		
		
        strengLayer->autorelease();
        return strengLayer;
    }
    CC_SAFE_DELETE(strengLayer);
    return NULL;
}


bool SGStrengLayer::init(SGBaseMilitaryCard *card, int type,bool isOnlyOfficer)
{
    if (!SGBaseTableLayer::init(NULL, sg_strengLayer))
    {
        return false;
    }
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BARRACKS_STRENG,   this,
                              callfuncO_selector(SGStrengLayer::strengOfficerListener));
    
    notification->addObserver(MSG_EQUIP_STRENG,  this,
                              callfuncO_selector(SGStrengLayer::strengEquipListener));
    
    notification->addObserver(MSG_SOLDIER_STRENG,   this,
                              callfuncO_selector(SGStrengLayer::strengSoldierListener));
    
    notification->addObserver(MSG_STRENG_SKILL,     this,
                              callfuncO_selector(SGStrengLayer::strengSkillListener));
    
    notification->addObserver(MSG_STRENG_LORD,  this,
                              callfuncO_selector(SGStrengLayer::strengLordSkillListener));

    _card = card;
     
    //SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    
    _card->retain();
    
    cardType = type;
    
    datas = CCArray::create();
    datas->retain();
    
    _arraySelectCard = CCArray::create();
    _arraySelectCard->retain();
    
    unsortarrary = CCArray::create();
    unsortarrary->retain();
    
    strengLimitLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if (isOnlyOfficer)
    {

        officer = (SGOfficerCard *)_card;
        CCArray *prop = SGPlayerInfo::sharePlayerInfo()->getPropCards();
        CCObject *obj_ = NULL;
        CCARRAY_FOREACH(prop, obj_)
        {
            SGPropsCard *card = (SGPropsCard *)obj_;
            
           if (card && card->getType() == 5 && card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
        
    }
    else if (cardType ==1)//武将
    {

            CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(array_pro, obj)
            {
                SGPropsCard *card = (SGPropsCard *)obj;
                if (card  && card->getType() == 1 && card->getIsProt() == 0)
                {
                    datas->addObject(card);
                }
            }

    }
    else if(cardType == 2)
    {
        equip = (SGEquipCard *)_card;
        CCArray *equips = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
        CCObject *obj_ = NULL;
        CCARRAY_FOREACH(equips, obj_)
        {
            SGEquipCard *card = (SGEquipCard *)obj_;
            if (card->getSsid() != _card->getSsid() &&card->getOfficerCardId() == 0 &&card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
        CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array_pro, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card  && card->getType() == 2 && card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
    }
    else if(cardType == 3)
    {
        soldier = (SGSoldierCard *)_card;
        CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array_pro, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card  && card->getType() == 3 &&card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
    }
    else
    {
        officer = (SGOfficerCard *)_card;
        CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array_pro, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card  && card->getType() == 4 &&card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
    }
    //if(cardType == 1)
       // showSort();else{SGSortBox::sortCards(datas, 3,0);}
    SGSortBox::sortCards(datas, 3,0);
    if(cardType == 1)
        showSort();
    
    tableViewHeight = 108;
    
    return true;
}


#pragma mark "重新刷新界面"

void SGStrengLayer::freshData()
{
	//重围所有相关数据,重新刷新
	_arraySelectCard->removeAllObjects();
	unsortarrary->removeAllObjects();
	datas->removeAllObjects();
	times = 0;
	cardCount = 0;
	
	for (int i = 0; i < 300; i++)
	{
		selectItem[i] = 0;
	}
	selectposition = 0;
}

void SGStrengLayer::freshViewData()
{
    CCString *str_ = CCString::create(_card->getClassName());
    int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
    
    SGExpDataModel *maxExp=NULL;
    int currLvl = _card->getCurrLevel();
    if(currLvl  >=  strengLimitLevel)
    {
        maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
    }
    else
    {
        maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
    }
    
    float per = (float)_card->getCurrExp()/maxExp->getExpValue();
	//停止闪动的动画
   	lab_lvl->stopAllActions();
    lab_lvl->setVisible(true);
    lab_lvl->setop(255);
    lab_orilvl->setVisible(true);
    lab_orilvl->setop(255);
    
    lab_lvl->setVisible(true);
    progress->setVisible(true);
    progress->setColor(ccWHITE);
    progress->setPercentage(per*100);
     progress->stopAllActions();
    progress->setOpacity(255);
 	lab_orilvl->setString(CCString::createWithFormat("Lv %d", _card->getCurrLevel())->getCString());

	lab_lvl->setString(CCString::createWithFormat("Lv %d", _card->getCurrLevel())->getCString());
	((SGCCLabelTTF *)getChildByTag(LABEL_ATK))->setString(CCString::createWithFormat("%d", _card->getAtk())->getCString());
	((SGCCLabelTTF *)getChildByTag(LABEL_DEL))->setString(CCString::createWithFormat("%d", _card->getDef())->getCString());
	((SGCCLabelTTF *)getChildByTag(LABEL_AP))->setString(CCString::createWithFormat("%d", _card->getMorale())->getCString());
	((SGCCLabelTTF *)getChildByTag(LABEL_SP))->setString(CCString::createWithFormat("%d", (int)_card->getSpeed())->getCString());
    m_plabel_MaterialCount->setString(CCString::createWithFormat("%d",countTheMaterialNumber())->getCString());
}

//强化完成后重新刷新界面
void SGStrengLayer::refreshStrengView()
{
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
    this->setIsCanTouch(true);
    for (int i = 0; i < sizeof(selectItem) / sizeof(selectItem[0]); i++)
    {
        SNSIndexPath* indexPath = NULL;
        if (selectItem[i] == 0)
        {
            continue;
        }
        else if (selectItem[i] == 1)
        {
            indexPath = SNSIndexPath::create(i, 0, 1);
        }
        Tabel_Item *item = (Tabel_Item*)tableView->getItemByIndexPath(indexPath);
        if (!item)
        {
            continue;
        }
        SGBaseMilitaryCard *basecard = item->_card;
        
        times--;
        addFlags--;
        this->changeNum(-1);
        selectItem[i] = 0;
        
        SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
        selectposition = selectposition<basecard->getselectposition()?selectposition:basecard->getselectposition();
        cardposition->removeChildByTag(110);
        cardposition->removeChildByTag(111);
        cardposition->removeChildByTag(112);
        CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
        if (ico)
        {
            ico->setVisible(true);
        }
        _arraySelectCard->removeObject(datas->objectAtIndex(indexPath->getRow()));
    }

    
	//刷新所有数据
	this->freshData();
	CCObject *obj = NULL;

	//武将卡
	if (cardType == 1)
	{
		CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
		CCARRAY_FOREACH(array_pro, obj)
		{
			SGPropsCard *card = (SGPropsCard *)obj;
			if (card  && card->getType() == 1 && card->getIsProt() == 0)
			{
				datas->addObject(card);
			}
		}

		//刷新武将相关信息显示
		SGOfficerCard *officerCard = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(_card->getSsid());
		// 卡片已经更新
		_card = officerCard;
		_card->retain();
		
	}
	else if (cardType == 2)//装备卡
	{
		CCArray *equips = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
		
        CCARRAY_FOREACH(equips, obj)
        {
            SGEquipCard *card = (SGEquipCard *)obj;
            if (card->getSsid() != _card->getSsid() &&card->getOfficerCardId() == 0 &&card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
        CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
		
        CCARRAY_FOREACH(array_pro, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card  && card->getType() == 2 && card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
		SGEquipCard *equipCard = SGPlayerInfo::sharePlayerInfo()->getEquipCard(_card->getSsid());
		//卡片已经更新
		_card = equipCard;
		_card->retain();
	}
	else if (cardType == 3)
	{
		CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array_pro, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card  && card->getType() == 3 &&card->getIsProt() == 0)
            {
                datas->addObject(card);
            }
        }
		SGSoldierCard *soldierCard = SGPlayerInfo::sharePlayerInfo()->getSoldierCard(_card->getSsid());
		_card = soldierCard;
		_card->retain();
	}
    
        this->showSort();
   
	//重新载入tableview
	tableView->reloadData(true);
	
   
	//刷新相关金币与数量显示
	this->updateCoind();
	

	this->freshViewData();
    
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
}


void SGStrengLayer::initMsg()
{
    
}
int SGStrengLayer::countTheMaterialNumber()
{
    int nCount = 0;
    nCount = datas->count();
    return nCount;
    
}

void SGStrengLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/strengjiantoutexiao.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/strengqhqr.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-3303.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    
    
    //SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSprite* title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-5);
    
    CCSprite *titlecenter =  CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    CCSprite *redbg =  CCSprite::createWithSpriteFrameName("playerinfo_bg_red.png");
    redbg->setAnchorPoint(ccp(0.5, 0));
    redbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  - title_bg->getContentSize().height - redbg->getContentSize().height)));
    this->addChild(redbg, -2);
    
    CCSprite *guangm =  CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guangm,6);
    guangm->setAnchorPoint(ccp(0.5, 1));
    guangm->setScaleX(s.width/guangm->getContentSize().width);
    guangm->setPosition(ccpAdd(redbg->getPosition(), ccp(0, redbg->getContentSize().height)));

    CCSprite *cardbg =  CCSprite::createWithSpriteFrameName("strengcardbg.png");
    if (s.width == 768)
    {
        cardbg->setDisplayFrame( CCSprite::createWithSpriteFrameName("strengcardbg_768.png")->displayFrame());
    }
    this->addChild(cardbg,1);
    cardbg->setPosition(ccpAdd(redbg->getPosition(), ccp(0, -cardbg->getContentSize().height*.445 +3)));
   
    for (int i = 0; i<5; i++)
    {
        SGButton *kong =  SGButton::create("streng_sckk.png", NULL, this, menu_selector(SGStrengLayer::btnselected),CCPointZero,false,true);

        this->addBtn(kong);
        kong->setTag(i);
        kong->setEnabled(false);
		kong->setPosition(ccpAdd(cardbg->getPosition(), ccp(-(kong->getContentSize().width+skewing(2))*2 +(kong->getContentSize().width+skewing(2))*i - 35 , -cardbg->getContentSize().height*.09)));
        
        CCSprite *icon =  CCSprite::createWithSpriteFrameName("public_font_kong.png");
        this->addChild(icon,100,100+i);
        icon->setPosition(kong->getPosition());
    }
    
   if(!onlyOfficer && cardType!=4)
   {
       SGCCLabelTTF *maxexp = SGCCLabelTTF::create(str_StrengLayer_str1, FONT_BOXINFO, 24);
       maxexp->setAnchorPoint(ccp(0, 0.5));
       this->addChild(maxexp,2);
       maxexp->setPosition(ccpAdd(cardbg->getPosition(), ccp(10, 46)));
       
       
       
       CCString *str_ = CCString::create(_card->getClassName());
       int type= SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
       
       CCLOG("type is %d",type);
       int currLvl = _card->getCurrLevel();
       int maxlvl = strengLimitLevel;
       SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);//比如currLvl为5级 此句话获得5级升6级所需经验值
       SGExpDataModel *num2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, maxlvl);//比如currLvl为5级 此句话获得5级升6级所需经验值

       maxneedexp = num2->getexpAll()-num->getexpAll()-_card->getCurrExp();
       maxneedexp = maxneedexp  < 0 ? 0 : maxneedexp ;
       CCString* maxexpstring = CCString::createWithFormat("%d",maxneedexp);
       maxe = SGCCLabelTTF::create(maxexpstring->getCString(),  FONT_BOXINFO, 24);
       maxe->setAnchorPoint(ccp(0, 0.5));
       this->addChild(maxe,2);
       maxe->setPosition(ccpAdd(maxexp->getPosition(), ccp(maxexp->getContentSize().width*1.1, 0)));
       
       
       SGCCLabelTTF *needexplabel = SGCCLabelTTF::create(str_StrengLayer_str2, FONT_BOXINFO, 24);
       needexplabel->setAnchorPoint(ccp(0, 0.5));
       this->addChild(needexplabel,2);
       needexplabel->setPosition(ccpAdd(cardbg->getPosition(), ccp(-290, 46)));
       
       afterExp = SGCCLabelTTF::create("0",FONT_BOXINFO, 24);
       afterExp->setAnchorPoint(ccp(0, 0.5));
       this->addChild(afterExp,11);
       afterExp->setPosition(ccpAdd(needexplabel->getPosition(), ccp(needexplabel->getContentSize().width*1.1,0)));

   
   }
    
	//自动添加材料
	SGButton *autoSrgBtn = SGButton::create("player_portrait.png", "player_portrait.png",
											this, menu_selector(SGStrengLayer::autoStreng), ccp(0, 0), false, true);
	
	autoSrgBtn->setZOrder(9);
	autoSrgBtn->setScale(0.7);
	autoSrgBtn->setTag(12);
	this->addBtn(autoSrgBtn);

	autoSrgBtn->setPosition(ccp(cardbg->getPositionX() + cardbg->getContentSize().width / 2 * 0.84, cardbg->getPositionY() * 0.97));
	//添加“自动添加字体”
	SGCCLabelTTF *au = SGCCLabelTTF::create(str_zidong, FONT_PANGWA, 25);
	SGCCLabelTTF *add = SGCCLabelTTF::create(str_tianja, FONT_PANGWA, 25);
	
    au->setScale(1.5);
    add->setScale(1.5);
    autoSrgBtn->addChild(au, 100);
	autoSrgBtn->addChild(add, 100);
    au->setPosition(ccp(autoSrgBtn->getContentSize().width/2 - au->getContentSize().width/4, autoSrgBtn->getContentSize().height/2 + au->getContentSize().height * 0.2 -4 ));
	add->setPosition(ccp(autoSrgBtn->getContentSize().width/2 - add->getContentSize().width/4, autoSrgBtn->getContentSize().height/2 - add->getContentSize().height/1.5-4 ));

    CCSprite *rightbian =  CCSprite::createWithSpriteFrameName("public_bian.png");
    rightbian->setAnchorPoint(ccp(1, 1));
    rightbian->setFlipX(true);
    this->addChild(rightbian,-1);
    rightbian->setScaleY(1.5);
    rightbian->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0, -title_bg->getContentSize().height)));
    
    CCSprite *leftbian =  CCSprite::createWithSpriteFrameName("public_bian.png");
    leftbian->setAnchorPoint(ccp(0, 1));
    this->addChild(leftbian,-1);
    leftbian->setScaleY(1.5);
    leftbian->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -title_bg->getContentSize().height)));
    
    CCSprite *gonggao_3 =  CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    gonggao_3->setAnchorPoint(ccp(0.5, 0.28));
    gonggao_3->setScaleX(6.5);
    gonggao_3->setPosition(ccp(s.width/2,redbg->getPosition().y));
    this->addChild(gonggao_3);
    
    
    if (cardType == 3)
    {
        SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_strengLayer);
        int color = 1;
        switch (_card->getItemId())
        {
            case 3103:
            case 3203:
            case 3301:
            {
                color=2;
            }
                break;
            case 3102:
            case 3201:
            case 3303:
            {
                color =3;
                
            }
                break;

            default:
                break;
        }

        Icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%d-%d.png",_card->getItemId(),color)->getCString());
    }
    else if (cardType == 4)
    {
        SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getGeneralSkillById(officer->getOfficerSkil());
        SGMainManager::shareMain()->addHeadIconPlistByNum(skill->getIconId(),sg_strengLayer);
        Icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",skill->getSkillItemId())->getCString());
    }
    else
    {
		int iconId = -1;
        
		if (!strcmp(_card->getClassName(), "SGOfficerCard") && !onlyOfficer)//武将
		{
			SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
			iconId = of->getIconId();
		}
        else if (!strcmp(_card->getClassName(), "SGOfficerCard") && onlyOfficer)//主将技能强化
        {
            SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getLordSkillById(officer->getLordId());
            SGMainManager::shareMain()->addHeadIconPlistByNum(skill->getIconId(),sg_strengLayer);
            iconId = skill->getIconId();
        }
		else if(!strcmp(_card->getClassName(), "SGEquipCard"))//装备
		{
			SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
			iconId = eq->getIconId();
		}
		else if (!strcmp(_card->getClassName(), "SGPropsCard"))
		{
			SGPropsDataModel *eq = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
			iconId = eq->getIconId();
		}
		else
		{
			iconId = _card->getItemId();
		}
		
        SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_strengLayer);
        Icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png", iconId)->getCString());
    }
    this->addChild(Icon,40);
    Icon->setScale(1.23);
    Icon->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width*.377-3, -title_bg->getContentSize().height - redbg->getContentSize().height/2)));
    
    CCSprite *cardground =  CCSprite::createWithSpriteFrameName("card_bj.png");
    cardground->setScale(1.23);
    this->addChild(cardground,-1);
    cardground->setPosition(Icon->getPosition());
    
    CCString *str_ = CCString::create(_card->getClassName());
    CCSprite *jink =  CCSprite::createWithSpriteFrameName("jinkuang.png");
    int nAdvanceNumPos = 140;
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        if (cardType == 4)
        {
            jink->setDisplayFrame( CCSprite::createWithSpriteFrameName("jinengkuang.png")->displayFrame());
        }
        else
        {
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1)
            {
                starlvl-=1;
            }
            jink->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
            if (jink)
            {
                jink->removeChildByTag(30, true);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(jink->getContentSize().width*0.35,jink->getContentSize().height - countryName->getContentSize().height));
                jink->addChild(countryName,30,30);
                
                jink->removeChildByTag(250, true);
                if (temp->getAdvNum()>0)
                {
                    SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
                    advancenumber->setAnchorPoint(ccp(1,1));

                    advancenumber->ignoreAnchorPointForPosition(false);
                    advancenumber->setPosition(ccp(nAdvanceNumPos,nAdvanceNumPos));

                    jink->addChild(advancenumber,50,250);
                }

            }
        }
    }
    else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        jink->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
        
        jink->removeChildByTag(250, true);
        if (temp->getAdvNum()>0)
        {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
            advancenumber->setInsideColor(ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->ignoreAnchorPointForPosition(false);

            advancenumber->setPosition(ccp(nAdvanceNumPos,nAdvanceNumPos));

            jink->addChild(advancenumber,50,250);
        }
    }
    else if (str_->isEqual(CCString::create("SGSoldierCard")))
    {
        jink->setDisplayFrame( CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png")->displayFrame());
    }
    
    this->addChild(jink,100);
    jink->setScale(1.23);
    jink->setPosition(Icon->getPosition());
    
    //是否显示一键合成，如果不是武将合成，或者是士兵卡对应合成，就不显示
    bool isDispMergeBtn = false;
    
    const char * strTitle =NULL;
    const char * strMaterialTips =NULL;
    if(cardType==1&&onlyOfficer==false)
    {
        strTitle = str_wujiangqianghua;
        strMaterialTips = str_StrengLayer_st101;
        isDispMergeBtn = true;
    }
    else if(cardType==1&&onlyOfficer==true)
    {
        strTitle = str_wujiangjiqianghua;
        strMaterialTips = str_StrengLayer_st102;
    }
    else if(cardType == 2)
    {
        strTitle = str_zhuangbeiqianghua;
        strMaterialTips = str_StrengLayer_st101;
    }
    else if(cardType == 3)
    {
        strTitle = str_shibinqianghua;
        strMaterialTips = str_StrengLayer_st104;
        isDispMergeBtn = true;
    }
    else if(cardType == 4)
    {
        strTitle = str_beidongjiqianghua;
        strMaterialTips = str_StrengLayer_st103;
    }
    SGCCLabelTTF * LableTitle = SGCCLabelTTF::create(strTitle, FONT_PANGWA,40);
    this->addChild(LableTitle);
    LableTitle->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    
    SGCCLabelTTF *name = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_BOXINFO, 36 ,ccc3(0xff, 0xf0, 0x00));
    this->addChild(name);
    name->setPosition(ccpAdd(LableTitle->getPosition(), ccp(LableTitle->getContentSize().width/2+name->getContentSize().width , 0)));

    SGCCLabelTTF *labelMaterialTips = SGCCLabelTTF::create(strMaterialTips,FONT_PANGWA,26,ccWHITE);
    labelMaterialTips->setAnchorPoint(ccp(0, 0.5));
    this->addChild(labelMaterialTips);
    
    int leftShift = -550;
    int btnRightShift = 20;
    if (CCDirector::sharedDirector()->getWinSize().width == 768)
    {
        leftShift = -600;
        btnRightShift = -30;
    }
    
    labelMaterialTips->setPosition(ccpAdd(autoSrgBtn->getPosition(),ccp(leftShift,-120)));
    
    
    
    //一键合并 合并经验丹小到大，客户端只负责显示，合成规则在服务器，这边只接收状态
    if (isDispMergeBtn)
    {
        SGButton *hotKeyToMerge = SGButton::createFromLocal("batch_sell.png", str_StrengLayer_st105,
                                                        this, menu_selector(SGStrengLayer::strengMergeLogic),CCPointZero,FONT_PANGWA,ccWHITE,28);
    
        this->addBtn(hotKeyToMerge);
        hotKeyToMerge->setScale(0.8);
        hotKeyToMerge->setPosition(ccpAdd(autoSrgBtn->getPosition(), ccp(-hotKeyToMerge->getContentSize().width / 2 + btnRightShift, -120)));
    }
    
    m_plabel_MaterialCount = SGCCLabelTTF::create("666",FONT_PANGWA,26,COLOR_YELLOW);
    m_plabel_MaterialCount->setString(CCString::createWithFormat("%d",countTheMaterialNumber())->getCString());
    m_plabel_MaterialCount->setAnchorPoint(ccp(0, 0.5));
    this->addChild(m_plabel_MaterialCount);
    m_plabel_MaterialCount->setPosition(ccpAdd(labelMaterialTips->getPosition(),ccp(labelMaterialTips->getContentSize().width*1+10,0)));
    
    if (onlyOfficer)
    {
        SGOfficerCard *temp = (SGOfficerCard *)_card;
        if (temp->getLordId())
        {
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(temp->getLordId());
            SGCCLabelTTF*lvl = SGCCLabelTTF::create("", FONT_BOXINFO, 30);
            
            lvl->setString(CCString::createWithFormat("%s",lordSkill->getSkillName()->getCString())->getCString());
            lvl->setAnchorPoint(ccp(0, 0.5));
            lvl->setPosition(ccpAdd(jink->getPosition(), ccp(s.width*.12, jink->getContentSize().height*.3)));
            this->addChild(lvl,10);
            
            SGCCLabelTTF *b = SGCCLabelTTF::create(str_StrengLayer_str3, FONT_BOXINFO, 24);
            this->addChild(b);
            b->setAnchorPoint(ccp(0, 0.5));
           
            b->setPosition( ccpAdd(lvl->getPosition(), ccp(0, -lvl->getContentSize().height*1.0)));
            
			SGCCLabelTTF *bbb = SGCCLabelTTF::create(CCString::createWithFormat("Lv %d / Lv %d",lordSkill->getLevel(),
																				temp->getLordMaxLevel())->getCString(), FONT_BOXINFO, 24 , ccc3(0xff, 0xb4, 0x00));
            bbb->setAnchorPoint(ccp(0, 0.5));
           
            bbb->setPosition(ccpAdd(b->getPosition(), ccp(b->getContentSize().width+0, 0)));
            this->addChild(bbb);
            
            
            SGCCLabelTTF* skillup = SGCCLabelTTF::create(str_StrengLayer_str4, FONT_BOXINFO, 24);
            this->addChild(skillup);
            skillup->setAnchorPoint(ccp(0, 0.5));
            skillup->setPosition(ccpAdd(b->getPosition(), ccp(0, -b->getContentSize().height*1.3)));
            
            skilluprate = SGCCLabelTTF::create("0%", FONT_BOXINFO, 28);
            skilluprate->setAnchorPoint(ccp(0, 0.5));
            skilluprate->setPosition(ccpAdd(skillup->getPosition(),ccp(skillup->getContentSize().width*1.1,0)));
            this->addChild(skilluprate,10);
        }
        
    }
    else if (cardType !=4)
    {
        lab_lvl = SGCCLabelTTF::create(CCString::createWithFormat("Lv %d",_card->getCurrLevel())->getCString(),FONT_BOXINFO, 28 , ccc3(0x26, 0xd9, 0xff));
        lab_lvl->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_lvl);
        
        if (str_->isEqual(CCString::create("SGSoldierCard"))) {
            lab_lvl->setPosition(ccpAdd(jink->getPosition(), ccp(s.width*.12, jink->getContentSize().height*0.5-lab_lvl->getContentSize().height*0.6)));
        }
        else
            lab_lvl->setPosition(ccpAdd(jink->getPosition(), ccp(s.width*.12, jink->getContentSize().height*0.5-lab_lvl->getContentSize().height*0.6)));
        
        lab_orilvl = SGCCLabelTTF::create(CCString::createWithFormat("Lv %d",_card->getCurrLevel())->getCString(),FONT_BOXINFO, 28 , ccc3(0x26, 0xd9, 0xff));
        lab_orilvl->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_orilvl);
        
        lab_orilvl->setPosition(ccpAdd(jink->getPosition(), ccp(s.width*.12, jink->getContentSize().height*0.5-lab_lvl->getContentSize().height*0.6)));
        if (cardType == 3)
        {
            lab_orilvl->setPosition(ccpAdd(jink->getPosition(), ccp(s.width*.12, jink->getContentSize().height*0.5-lab_lvl->getContentSize().height*0.6)));
        }
        lab_orilvl->setop(0);
        
        
        CCSprite *progresscao =  CCSprite::createWithSpriteFrameName("lantiao.png");
        this->addChild(progresscao);
        progresscao->setPosition(ccpAdd(redbg->getPosition(),ccp(progresscao->getContentSize().width*.25,125)));
        progress = CCProgressTimer::create( CCSprite::createWithSpriteFrameName("tiao_lan.png"));
        progress->setType(kCCProgressTimerTypeBar);
        progress->setMidpoint(ccp(0,0));
        progress->setBarChangeRate(ccp(1, 0));
        addChild(progress,10);
        progress->setPosition(progresscao->getPosition());
        
        
        CCString *str_ = CCString::create(_card->getClassName());
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
        
        
        SGExpDataModel *maxExp=NULL;
        int currLvl = _card->getCurrLevel();
        if(currLvl  >=  strengLimitLevel)
        {
           progress->setPercentage(100);

        }
        else
        {
           maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
           float a = (float)_card->getCurrExp()/maxExp->getExpValue();
             
           progress->setPercentage(100*a);
        }
    

        CCSprite *bf_atkbg=CCSprite::createWithSpriteFrameName("font_strengexpsbg.png");
        this->addChild(bf_atkbg);
        bf_atkbg->setPosition(ccpAdd(lab_lvl->getPosition(), ccp(bf_atkbg->getContentSize().width/2+10,-bf_atkbg->getContentSize().height*2.1)));
        
        CCSprite *bf_defbg=CCSprite::createWithSpriteFrameName("font_strengexpsbg.png");
        this->addChild(bf_defbg);
        bf_defbg->setPosition(ccpAdd(bf_atkbg->getPosition(), ccp(bf_atkbg->getContentSize().width+10,0)));
        
        CCSprite *bf_moralebg=CCSprite::createWithSpriteFrameName("font_strengexpsbg.png");
        this->addChild(bf_moralebg);
        bf_moralebg->setPosition(ccpAdd(bf_atkbg->getPosition(), ccp(0,-bf_atkbg->getContentSize().height*2.2)));
        
        CCSprite *bf_speedbg=CCSprite::createWithSpriteFrameName("font_strengexpsbg.png");
        this->addChild(bf_speedbg);
        bf_speedbg->setPosition(ccpAdd(bf_moralebg->getPosition(), ccp(bf_moralebg->getContentSize().width+10,0)));
        
        CCSprite *bf_atk=CCSprite::createWithSpriteFrameName("ofc_icon_atk.png");
        this->addChild(bf_atk);
        bf_atk->setPosition(ccpAdd(bf_atkbg->getPosition(), ccp(-bf_atkbg->getContentSize().width/2,0)));
        
        CCSprite *bf_def=CCSprite::createWithSpriteFrameName("ofc_icon_def.png");
        this->addChild(bf_def);
        bf_def->setPosition(ccpAdd(bf_defbg->getPosition(), ccp(-bf_defbg->getContentSize().width/2,0)));
        
        CCSprite *bf_morale=CCSprite::createWithSpriteFrameName("ofc_icon_morale.png");
        this->addChild(bf_morale);
        bf_morale->setPosition(ccpAdd(bf_moralebg->getPosition(), ccp(-bf_moralebg->getContentSize().width/2,0)));
        
        CCSprite *bf_speed=CCSprite::createWithSpriteFrameName("ofc_icon_speed.png");
        this->addChild(bf_speed);
        bf_speed->setPosition(ccpAdd(bf_speedbg->getPosition(), ccp(-bf_speedbg->getContentSize().width/2,0)));
        

        
        //攻防血速
        SGCCLabelTTF *lab_ack = SGCCLabelTTF::create(CCString::createWithFormat("%d",_card->getAtk())->getCString(),FONT_BOXINFO, 28);
        lab_ack->setAnchorPoint(ccp(0, 0.5));
		lab_ack->setTag(LABEL_ATK);
        this->addChild(lab_ack);
        lab_ack->setPosition(ccpAdd(bf_atkbg->getPosition(), ccp(-bf_atkbg->getContentSize().width/2+20,0)));
        
        
        SGCCLabelTTF * lab_def = SGCCLabelTTF::create(CCString::createWithFormat("%d",_card->getDef())->getCString(),FONT_BOXINFO, 28);
        lab_def->setAnchorPoint(ccp(0, 0.5));
		lab_def->setTag(LABEL_DEL);
        this->addChild(lab_def);
        lab_def->setPosition(ccpAdd(bf_defbg->getPosition(), ccp(-bf_defbg->getContentSize().width/2+20,0)));
        
        SGCCLabelTTF *lab_moral = SGCCLabelTTF::create(CCString::createWithFormat("%d",_card->getMorale())->getCString(),FONT_BOXINFO, 28);
        lab_moral->setAnchorPoint(ccp(0, 0.5));
		lab_moral->setTag(LABEL_AP);
        this->addChild(lab_moral);
        lab_moral->setPosition(ccpAdd(bf_moralebg->getPosition(), ccp(-bf_moralebg->getContentSize().width/2+20,0)));
        
        SGCCLabelTTF *lab_Speed = SGCCLabelTTF::create(CCString::createWithFormat("%d",(int)_card->getSpeed())->getCString(),FONT_BOXINFO, 28);
        lab_Speed->setAnchorPoint(ccp(0, 0.5));
		lab_Speed->setTag(LABEL_SP);
        this->addChild(lab_Speed);
        lab_Speed->setPosition(ccpAdd(bf_speedbg->getPosition(), ccp(-bf_speedbg->getContentSize().width/2+25,0)));
        
        
        lab_addack = SGCCLabelTTF::create("",FONT_BOXINFO, 28 , ccGREEN);
        lab_addack->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_addack);
        lab_addack->setPosition(ccpAdd(lab_ack->getPosition(), ccp(lab_ack->getContentSize().width,0)));
        
        lab_adddef = SGCCLabelTTF::create("",FONT_BOXINFO, 28 , ccGREEN);
        lab_adddef->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_adddef);
        lab_adddef->setPosition(ccpAdd(lab_def->getPosition(), ccp(lab_def->getContentSize().width,0)));
        
        lab_addmoral = SGCCLabelTTF::create("",FONT_BOXINFO, 28 , ccGREEN);
        lab_addmoral->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_addmoral);
        lab_addmoral->setPosition(ccpAdd(lab_moral->getPosition(), ccp(lab_moral->getContentSize().width,0)));
        
        lab_addSpeed = SGCCLabelTTF::create("",FONT_BOXINFO, 28 , ccGREEN);
        lab_addSpeed->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_addSpeed);
        lab_addSpeed->setPosition(ccpAdd(lab_Speed->getPosition(), ccp(lab_Speed->getContentSize().width,0)));
    }
    else
    {
        SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getGeneralSkillById(officer->getOfficerSkil());
        name->setString(skill->getSkillName()->getCString());
        
        lab_lvl = SGCCLabelTTF::create(skill->getSkillName()->getCString(),FONT_BOXINFO, 30);
        lab_lvl->setAnchorPoint(ccp(0, 0.5));
        this->addChild(lab_lvl);
        lab_lvl->setPosition(ccpAdd(jink->getPosition(), ccp(s.width*.12, jink->getContentSize().height*.4)));
        
        afterExp = SGCCLabelTTF::create(str_StrengLayer_str3, FONT_BOXINFO, 24);
        
       // CCLOG("%d,%d",skill->getSkillMaxLevel(),officer->getSkillMaxLevel());
        afterExp->setAnchorPoint(ccp(0, 0.5));
        this->addChild(afterExp);
        afterExp->setPosition(ccpAdd(lab_lvl->getPosition(), ccp(0, -lab_lvl->getContentSize().height*1.5)));
        
        
        SGCCLabelTTF *bbb = SGCCLabelTTF::create(CCString::createWithFormat("Lv %d / Lv %d",skill->getSkillMaxLevel(),officer->getSkillMaxLevel())->getCString(), FONT_BOXINFO, 24 , ccc3(0xff, 0xb4, 0x00));
        bbb->setAnchorPoint(ccp(0, 0.5));
        
        bbb->setPosition(ccpAdd(afterExp->getPosition(), ccp(afterExp->getContentSize().width+0, 0)));
        this->addChild(bbb);
        
   
        SGCCLabelTTF * lab = SGCCLabelTTF::create(str_StrengLayer_str5,FONT_BOXINFO, 24);
        this->addChild(lab);
        lab->setAnchorPoint(ccp(0, 0.5));
        lab->setPosition(ccpAdd(afterExp->getPosition(), ccp(0, -lab_lvl->getContentSize().height)));
        
        skilluprate = SGCCLabelTTF::create("0%", FONT_BOXINFO, 24);
        skilluprate->setAnchorPoint(ccp(0, 0.5));
        skilluprate->setPosition(ccpAdd(lab->getPosition(), ccp(lab->getContentSize().width, 0)));
        this->addChild(skilluprate,10);
        
    }
    
    
    int modelTag = 0;

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGStrengLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    //不再指向返回
    //modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_18);
    backBtn->setTag(modelTag);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_strengLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30 ,bg->getContentSize().width , -112 + s.height - title_bg->getContentSize().height - redbg->getContentSize().height );
	
    bg->setTextureRect(r);
	bg->setScaleX(s.width/bg->getContentSize().width * 1.02);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-2, 112));
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_15);
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height -title_bg->getContentSize().height - redbg->getContentSize().height - cardbg->getContentSize().height - 80 -40));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, 50 + 65)));
    
    okBtn = SGButton::createFromLocal("streng_btn.png", str_Ok, this, menu_selector(SGStrengLayer::secondeconfirm),CCPointZero,FONT_PANGWA,ccWHITE,32);
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_16);
    okBtn->setTag(modelTag);
    this->addBtn(okBtn);
    okBtn->setPosition(ccp(s.width*.5f + 250, Icon->getPosition().y-20));
    tableView->setDown(-10);
    
	
	
    //箭头转弯
    fermEffect = CCSpriterX::create("animationFile/qhjm.scml", true, true);
    if (fermEffect)
    {
        fermEffect->setanimaID(0);
        fermEffect->setisloop(true);
        addChild(fermEffect, 50, 10000);
        fermEffect->setPosition(Icon->getPosition());
        fermEffect->setVisible(false);
        
        CCSprite *jiantou =  CCSprite::createWithSpriteFrameName("qhjm_1.png");
        this->addChild(jiantou,50,77);
        jiantou->setAnchorPoint(ccp(0, 0.5));
        jiantou->setPosition(ccpAdd(cardbg->getPosition(), ccp(s.width/2, -cardbg->getContentSize().height*.025)));
        CCAction *action =  CCSequence::create(CCMoveTo::create(2, ccp(Icon->getPosition().x, jiantou->getPosition().y)),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::updateposition)),CCDelayTime::create(fermEffect->getAnimaTimeLength(0, 0)),CCCallFunc::create(this, callfunc_selector(SGStrengLayer::stopjiantouAction)),NULL);
        jiantou->runAction(CCRepeatForever::create((CCActionInterval*)action));
    }

    if (datas->count() == 0)
    {
        if (onlyOfficer)
        {
            okBtn->setEnabled(false);
            okBtn->setImage("barrack_anniu_hui.png");
            okBtn->setEnabled(false);
            
            SGButton *Btn = SGButton::createFromLocal("box_btnbg.png",
                                             str_Back_,
                                             this,
                                             menu_selector(SGStrengLayer::backHandler),
                                             ccp(0, 3),
                                             FONT_PANGWA,
                                             ccWHITE,
                                             32,
                                             false,
                                             true);
            this->addBtn(Btn);
            Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, b)));
            
            SGCCLabelTTF *notice = SGCCLabelTTF::create(str_StrengLayer_str7, FONT_BOXINFO, 32);
            this->addChild(notice);
            notice->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -50)));
        }
        else
        {
            okBtn->setEnabled(false);
            okBtn->setImage("barrack_anniu_hui.png");
            
            okBtn->setEnabled(false);
            SGButton *Btn = SGButton::create("box_btnbg.png",
                                             "public_font_fanhui.png",
                                             this,
                                             menu_selector(SGStrengLayer::backHandler),
                                             ccp(0, 0),
                                             false,
                                             true);
            this->addBtn(Btn);
            Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, b)));
            
            if (cardType == 4)
            {
                SGCCLabelTTF *notice = SGCCLabelTTF::create(str_StrengLayer_str7, FONT_BOXINFO, 32);
                this->addChild(notice);
                notice->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -50)));
            }
            
            
        }

    }
	

	
}



//对应的合成逻辑，向服务器以关消息
void SGStrengLayer::strengMergeLogic(CCObject *obj)
{
    CCLOG(" 开始合成 ");
    //清除所有手动选择的状态。
    for (int i = 0; i < sizeof(selectItem) / sizeof(selectItem[0]); i++)
    {
        SNSIndexPath* indexPath = NULL;
        if (selectItem[i] == 0)
        {
            continue;
        }
        else if (selectItem[i] == 1)
        {
            indexPath = SNSIndexPath::create(i, 0, 1);
        }
        Tabel_Item *item = (Tabel_Item*)tableView->getItemByIndexPath(indexPath);
        if (!item)
        {
            continue;
        }
        SGBaseMilitaryCard *basecard = item->_card;
        
        times--;
        addFlags--;
        this->changeNum(-1);
        selectItem[i] = 0;
        
        SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
        selectposition = selectposition<basecard->getselectposition()?selectposition:basecard->getselectposition();
        cardposition->removeChildByTag(110);
        cardposition->removeChildByTag(111);
        cardposition->removeChildByTag(112);
        CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
        ico->setVisible(true);
        _arraySelectCard->removeObject(datas->objectAtIndex(indexPath->getRow()));
    }
    tableView->reloadData();
    //要弹框
    
    SGMergeMaterialBox *mBox = SGMergeMaterialBox::create(this, cardType, _card);//技能是没有的，所以可以使用cardType，为1的时候只能是武将
    SGMainManager::shareMain()->showBox(mBox);
}


void SGStrengLayer::changeNum(int num)
{
    cardCount+= num;
	(cardCount < 0) && (cardCount = 0);
    
}

void SGStrengLayer::setSelectCard(Tabel_Item *item)
{
	
	SGBaseMilitaryCard *basecard = item->_card;
	CCString *str_ = CCString::create(basecard->getClassName());
	selectposition = selectposition<basecard->getselectposition()?selectposition:basecard->getselectposition();
	
	basecard->setselectposition(selectposition);
	
	SGButton *position = (SGButton *)menu->getChildByTag(basecard->getselectposition());
	while (NULL != position->getChildByTag(110))
    {
		selectposition++;
		basecard->setselectposition(selectposition);
		position = (SGButton *)menu->getChildByTag(basecard->getselectposition());
	}
	SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
	cardposition->setUserObject(item->getIndexPath());
    cardposition->setEnabled(true);

	CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
	ico->setVisible(false);
	selectposition ++;
	CCSprite *icon = NULL;
	if (!strcmp(basecard->getClassName(), "SGPropsCard"))
	{
		SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(basecard->getItemId());
		icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
	}
	else
	{
		icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",basecard->getHeadSuffixNum())->getCString());
	}
	

	icon->setPosition(ccp(cardposition->getContentSize().width/2, cardposition->getContentSize().height/2));
	cardposition->addChild(icon,1,110);
	
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
	CCSprite *kuang =  CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
	cardposition->addChild(kuang,1,111);
	kuang->setPosition(icon->getPosition());
	
	CCSprite *background =  CCSprite::createWithSpriteFrameName("card_bj.png");
	cardposition->addChild(background,0,112);
	background->setPosition(icon->getPosition());
	
	CCString *string = CCString::create(basecard->getClassName());
	if (string->isEqual(CCString::create("SGOfficerCard")))
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(basecard->getItemId());
		int starlvl = temp->getOfficerCurrStarLevel();
		if (starlvl>1)//匹配品质星级，一星二星用相同品质
        {
			starlvl-=1;
		}
		kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
		if (kuang)
		{
			kuang->removeChildByTag(3, true);
			CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
			countryName->setAnchorPoint(ccp(1,1));
			countryName->ignoreAnchorPointForPosition(false);
			countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
			kuang->addChild(countryName,3,3);
            
            kuang->removeChildByTag(250, true);
            if (temp->getAdvNum()>0)
            {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->setPosition(kuang->getContentSize()/1.2);
                kuang->addChild(advancenumber,50,250);
            }

		}
	}
    else if (str_->isEqual(CCString::create("SGEquipCard")))
	{
		SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(basecard->getItemId());
		kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
        kuang->removeChildByTag(250, true);
        if (temp->getAdvNum()>0)
        {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->setPosition(kuang->getContentSize()/1.2);
            kuang->addChild(advancenumber,50,250);
        }
	}
    else
	{
		SGPropsCard *card = (SGPropsCard *)basecard;
		if (card->getType() == 1)
        {
			SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());
            int starlvl = temp->getPropsCurrStarLevel();
            if (starlvl>1)//匹配品质星级，一星二星用相同品质
            {
                starlvl-=1;
            }
            kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
            if (kuang)
            {
                kuang->removeChildByTag(3, true);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                kuang->addChild(countryName,3,3);
            }

        }
        else
		{
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());
            int starlvl = temp->getPropsCurrStarLevel();
            if (starlvl>1)
            {
                starlvl-=1;
            }

            if (card->getType() == 4)
            {
                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                if (kuang)
                {
                    kuang->removeChildByTag(30, true);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                }
                
            }
            else if (card->getType() == 5)
            {
                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                
                if (kuang)
                {
                    kuang->removeChildByTag(30, true);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                }
                
            }
            else
            {
                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                
            }
			
		}
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//星级低排序
static int SGStarLess(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getCurrStar() < ((SGBaseMilitaryCard *)p2)->getCurrStar());
}
//星级高排序
static int SGStarMore(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getCurrStar() > ((SGBaseMilitaryCard *)p2)->getCurrStar());
}
static int SGLvlLess(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getCurrLevel() < ((SGBaseMilitaryCard *)p2)->getCurrLevel());
}
static int SGCountrySame(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getRace() < ((SGBaseMilitaryCard *)p2)->getRace());
}
static int SGItemSame(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getItemType() < ((SGBaseMilitaryCard *)p2)->getItemType());
}
static int SGPropsCardLess(const CCObject* p1, const CCObject* p2)
{
	SGPropsDataModel *propsTemp1 = SGStaticDataManager::shareStatic()->getPropsById(((SGPropsCard *)p1)->getItemId());
	SGPropsDataModel *propsTemp2 = SGStaticDataManager::shareStatic()->getPropsById(((SGPropsCard *)p2)->getItemId());
	return (propsTemp1->getpropsValue() < propsTemp2->getpropsValue());
}

//寻找到与对应的第N张卡牌相同星级的有开始始index（1为星级）
int SGStrengLayer::findPreSameIndex(int n, int which)
{
	int preIndex = n;
	//向前遍历相同星级的卡牌
	for (int i = n; i > 0; i--)
	{
		if (which == 1)
		{
			if (((SGBaseMilitaryCard *)datas->objectAtIndex(n))->getCurrStar() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i - 1))->getCurrStar() &&
				((SGBaseMilitaryCard *)datas->objectAtIndex(i))->getCurrStar() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i - 1))->getCurrStar())// 后一个星级与前一个相同
			{
				preIndex = i - 1;
			}
		}
		else if (which == 2)
		{
			if (((SGBaseMilitaryCard *)datas->objectAtIndex(n))->getCurrLevel() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i - 1))->getCurrLevel() &&
				((SGBaseMilitaryCard *)datas->objectAtIndex(n))->getCurrStar() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i - 1))->getCurrStar())// 后一个星级与前一个相同
			{
				preIndex = i - 1;
			}
		}

	}
	return preIndex;
}
//1为星级
int SGStrengLayer::findBckSameIndex(int n, int which)
{
	int bckIndex = n;
	//向前遍历相同星级的卡牌
	for (int i = n; i < datas->count() - 1; i++)
	{
		if (which == 1)//星级
		{
			if (((SGBaseMilitaryCard *)datas->objectAtIndex(n))->getCurrStar() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i + 1))->getCurrStar() &&
				((SGBaseMilitaryCard *)datas->objectAtIndex(i))->getCurrStar() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i + 1))->getCurrStar())// 后一个星级与前一个相同
			{
				bckIndex = i + 1;
			}
		}
		else if (which == 2)//等级
		{
			if (((SGBaseMilitaryCard *)datas->objectAtIndex(n))->getCurrLevel() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i + 1))->getCurrLevel() &&
				((SGBaseMilitaryCard *)datas->objectAtIndex(n))->getCurrStar() == ((SGBaseMilitaryCard *)datas->objectAtIndex(i + 1))->getCurrStar())// 后一个星级与前一个相同
			{
				bckIndex = i + 1;
			}
		}

	}
	return bckIndex;
}

void SGStrengLayer::findSameCountry(int pos, int s, int e)
{
	int index = 0, count = 0;
	for (int i = 0; i < datas->count(); i++)
	{
		SGBaseMilitaryCard *card1 = (SGBaseMilitaryCard *)datas->objectAtIndex(0);
		SGBaseMilitaryCard *card2 = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
		if (card1->getCurrStar() == card2->getCurrStar())
		{
			index++;
		}
	}
	
	for (int j = 0; j < index; j++)
	{
		SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(j);
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());//被强化的卡牌
		int strengCountry = temp->getOfficerRace();//被强化的卡牌国家
		if (strengCountry == card->getRace())
		{
			datas->exchangeObjectAtIndex(j, count++);
		}
	}
}

//将等级相同的三个放到前面
void SGStrengLayer::findSameItem(int pos, int s, int e, int which)
{
	int index = s, count = 0;
	if (which == 1)//武将卡
	{
		//std::sort(datas->data->arr + s, datas->data->arr + e, SGCountrySame);//先按国家排一下序
		
		for (int i = s; i < e; i++)
		{
			SGBaseMilitaryCard *card1 = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
			SGBaseMilitaryCard *card2 = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
			
			if (card1->getRace() == card2->getRace())
			{
				count++;
				if (count >= pos - s + 1)
				{
					break;
				}
			}
			else
			{
				i = index;
				count = 0;
				index++;
			}
		}
	}
	else if (which == 2)//装备卡
	{
		std::sort(datas->data->arr + s, datas->data->arr + e, SGItemSame);//按装备类型排一下序
		for (int i = s; i < e; i++)
		{
			SGBaseMilitaryCard *card1 = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
			SGBaseMilitaryCard *card2 = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
			
			if (card1->getItemType() == card2->getItemType())
			{
				count++;
				if (count >= pos - s + 1)
				{
					break;
				}
			}
			else
			{
				i = index;
				count = 0;
				index++;
			}
		}
	}

	
	//移动对应个数的相同国家的卡牌到对应5张卡牌后面的位置
	for (int j = index, k = s; j < index + count && k < pos + 1; j++, k++)
	{
		datas->exchangeObjectAtIndex(j, k);
	}
	
	CCLOG("index == %d count == %d", index, count);
}

static bool NewSortRule(const CCObject* p1, const CCObject* p2)
{
    SGBaseMilitaryCard *card_1 = (SGBaseMilitaryCard*)p1;
    SGBaseMilitaryCard *card_2 = (SGBaseMilitaryCard*)p2;

    //int cardCurrStar = card_->getCurrStar();
    CCString *str_1 = CCString::create(card_1->getClassName());
    CCString *str_2 = CCString::create(card_2->getClassName());
    if(str_1->isEqual(CCString::create("SGPropsCard"))&&!str_2->isEqual(CCString::create("SGPropsCard")))
    {
        return  true;
    }else if(!str_1->isEqual(CCString::create("SGPropsCard"))&&str_2->isEqual(CCString::create("SGPropsCard")))
    {
        return false;
    }else{
        return (((SGBaseMilitaryCard *)p1)->getCurrStar() <((SGBaseMilitaryCard *)p2)->getCurrStar());

    }
    //return (((SGBaseMilitaryCard *)p1)->getCurrStar() > ((SGBaseMilitaryCard *)p2)->getCurrStar());
}
//展示前排序
void SGStrengLayer::showSort()
{
    std::sort(datas->data->arr,datas->data->arr + datas->data->num,NewSortRule);
   if(cardType != 3){
    CCArray *propsary = CCArray::create();
    propsary->retain();
    for (int i = datas->count()-1;i>=0;i--)
    {
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
        CCString *cardType = CCString::create(card->getClassName());//卡牌类型
        if (card && (cardType->isEqual(CCString::create("SGPropsCard"))))
        {
            datas->removeObject(card);//移除对应位置的对象，
            propsary->addObject(card);
            
        }
        
    }
    CCArray *arytemp = CCArray::create();
    arytemp->retain();
    arytemp ->addObjectsFromArray(datas);
    if ( this->cardType==1 && datas->count()+propsary->count()>0)
    {
        CCArray *arywei = CCArray::create();
        arywei->retain();
        CCArray *aryshu = CCArray::create();
        aryshu->retain();
        CCArray *arywu = CCArray::create();
        arywu->retain();
        CCArray *aryqun = CCArray::create();
        aryqun->retain();
        for (int i=0;i<propsary->count();i++)
        {
            SGPropsCard *card = (SGPropsCard *)propsary->objectAtIndex(i);
            switch (card->getRace()) {
                case 1:
                    aryshu->addObject(card);
                    break;
                case 2:
                    arywei->addObject(card);
                    break;
                case 3:
                    arywu->addObject(card);
                    break;
                case 4:
                    aryqun->addObject(card);
                    break;
                default:
                    break;
            }
        }
        propsary->removeAllObjects();
        if (aryshu->count()>1)
            std::sort(aryshu->data->arr, aryshu->data->arr + aryshu->data->num, SGStarLess);
        if (arywei->count()>1)
            std::sort(arywei->data->arr, arywei->data->arr + arywei->data->num, SGStarLess);
        if (arywu->count()>1)
            std::sort(arywu->data->arr, arywu->data->arr + arywu->data->num, SGStarLess);
        if (aryqun->count()>1)
            std::sort(aryqun->data->arr, aryqun->data->arr + aryqun->data->num, SGStarLess);
        int countrynum = _card->getRace();
        datas->removeAllObjects();
        datas->addObjectsFromArray(aryshu);
        datas->addObjectsFromArray(arywei);
        datas->addObjectsFromArray(arywu);
        datas->addObjectsFromArray(aryqun);
        datas->addObjectsFromArray(arytemp);
        
        aryqun->removeAllObjects();
        aryshu->removeAllObjects();
        arywei->removeAllObjects();
        arywu->removeAllObjects();
        aryqun->release();
        aryshu->release();
        arywei->release();
        arywu->release();
        aryqun->release();
        aryshu->release();
        arywei->release();
        arywu->release();

        
        CCLOG("====>:%d", datas->count());
        
    }
    
    arytemp->removeAllObjects();
    propsary->removeAllObjects();
   
    
    arytemp->release();
    propsary->release();
    arytemp->release();
    propsary->release();

   }


}
//对卡片按自动添加材料的逻辑重新排序
void SGStrengLayer::reSortCard()
{
    if (isfromfirst==4 || isfromfirst==5)//主将计武将技自动添加,先排国家,星级从高到低
    {
        if (datas->count()>5)
        {
            CCArray *arrary = CCArray::create();
            CCArray *ary1=CCArray::create();
            CCArray *ary2=CCArray::create();
            CCArray *ary3=CCArray::create();
            CCArray *ary4=CCArray::create();
            ary1->retain();
            ary2->retain();
            ary3->retain();
            ary4->retain();
            arrary->addObject(ary1);
            arrary->addObject(ary2);
            arrary->addObject(ary3);
            arrary->addObject(ary4);
            arrary->retain();
          
            for(int i=0;i<datas->count();i++)//不同国家分组
            {
                SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(i);
                int country = card->getRace();
                switch (country)
                {
                    case 1:
                        ary1->addObject(card);
                        break;
                    case 2:
                        ary2->addObject(card);
                        break;
                    case 3:
                        ary3->addObject(card);
                        break;
                    case 4:
                        ary4->addObject(card);
                        break;
                        
                    default:
                        CCAssert(false, "数据错误，没有该国家分类。");
                        break;
                }
                
            }
            int cardcountry = _card->getRace()-1;
            arrary->exchangeObjectAtIndex(cardcountry,0);//把和卡同国家的放在最前面
            int cardnum = 0;
            for (int i=0;i<4;i++)
            {
                CCArray *ary = (CCArray*)arrary->objectAtIndex(i);
                cardnum+=ary->count();
                std::sort(ary->data->arr, ary->data->arr + ary->data->num, SGStarMore);//再按星级高排序
                if (cardnum > 5)
                    break;
                
            }
            datas->removeAllObjects();
            for (int i=0;i<4;i++)
            {
                CCArray *ary = (CCArray*)arrary->objectAtIndex(i);
                datas->addObjectsFromArray(ary);
            }
            arrary->release();
            ary1->release();
            ary2->release();
            ary3->release();
            ary4->release();
            
            
       }
        
        
    }
    else if(cardType!=3)
    {
        leavenum=0;
        unsortarrary->removeAllObjects();
        
        //    CCObject *obj_ = NULL;
        for(int i = datas->count()-1;i>=0;i--)//所有现在星级为4级以上和最大星级为5级以上的放在最后,不进入自动添加
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
            CCString *cardType = CCString::create(card->getClassName());//卡牌类型
            if (card && (!cardType->isEqual(CCString::create("SGPropsCard"))) && (card->getCurrStar()>=4 || card->getUpgradestar()>=4))
            {
                datas->removeObject(card);//移除对应位置的对象，
                //            datas->insertObject(card, datas->count()-1);//将对象放在最后
                //            datas->addObject(card);
                unsortarrary->addObject(card);
                leavenum++;//不排序的个数
            }
        }
        CCLOG("leavenum=%d",leavenum);
        
        
        
        std::sort(datas->data->arr, datas->data->arr + datas->data->num, SGStarLess);//先按星级低排序
        CCArray *propsary = CCArray::create();
        propsary->retain();
        for (int i = datas->count()-1;i>=0;i--)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
            CCString *cardType = CCString::create(card->getClassName());//卡牌类型
            if (card && (cardType->isEqual(CCString::create("SGPropsCard"))))
            {
                datas->removeObject(card);//移除对应位置的对象，
                propsary->addObject(card);
                
            }

        }
        
        
        if (datas->count() >0)
        {

            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(0);//获取每用卡牌
            CCString *cardType = CCString::create(card->getClassName());//卡牌类型
            if (cardType->isEqual(CCString::create("SGOfficerCard")) || cardType->isEqual(CCString::create("SGEquipCard")))//如果为武将卡
            {
                int cards = (datas->count() > 5 ? 4 : datas->count() - 1);
                int preIndex = findPreSameIndex(cards, 1);//1为星级
                int bckIndex = findBckSameIndex(cards, 1);

                std::sort(datas->data->arr + preIndex, datas->data->arr + bckIndex + 1, SGLvlLess);// 有相同星级的，把相同星级的部分按等级排序
                
                //相同等级，再排相同国家
                int preIndexLvl = findPreSameIndex(cards, 2);//1为星级
                int bckIndexLvl = findBckSameIndex(cards, 2);

                if (cardType->isEqual(CCString::create("SGOfficerCard")))
                {
                    findSameCountry(cards, bckIndexLvl + 1, 0);
                }
                if (cardType->isEqual(CCString::create("SGEquipCard")))
                {
                    this->findSameItem(cards, preIndexLvl, bckIndexLvl + 1, 2);
                }
                
            
            }
            if ( this->cardType==1 && datas->count()+propsary->count()>5)
            {
                CCArray *arywei = CCArray::create();
                arywei->retain();
                CCArray *aryshu = CCArray::create();
                aryshu->retain();
                CCArray *arywu = CCArray::create();
                arywu->retain();
                CCArray *aryqun = CCArray::create();
                
                aryqun->retain();
                for (int i=0;i<propsary->count();i++)
                {
                    SGPropsCard *card = (SGPropsCard *)propsary->objectAtIndex(i);
                    switch (card->getRace())
                    {
                        case 1:
                            aryshu->addObject(card);
                            break;
                        case 2:
                            arywei->addObject(card);
                            break;
                        case 3:
                            arywu->addObject(card);
                            break;
                        case 4:
                            aryqun->addObject(card);
                            break;
                        default:
                            break;
                    }
                }
                propsary->removeAllObjects();
                
                if (aryshu->count()>1)
                    std::sort(aryshu->data->arr, aryshu->data->arr + aryshu->data->num, SGStarLess);
                if (arywei->count()>1)
                    std::sort(arywei->data->arr, arywei->data->arr + arywei->data->num, SGStarLess);
                if (arywu->count()>1)
                    std::sort(arywu->data->arr, arywu->data->arr + arywu->data->num, SGStarLess);
                if (aryqun->count()>1)
                    std::sort(aryqun->data->arr, aryqun->data->arr + aryqun->data->num, SGStarLess);
                
                int countrynum = _card->getRace();
                switch (countrynum)
                {
                    case 1:
                    {
                        datas->addObjectsFromArray(aryshu);
                        datas->addObjectsFromArray(arywei);
                        datas->addObjectsFromArray(arywu);
                        datas->addObjectsFromArray(aryqun);
                    }
                        break;
                    case 2:
                    {
                        datas->addObjectsFromArray(arywei);
                        datas->addObjectsFromArray(aryshu);
                        datas->addObjectsFromArray(arywu);
                        datas->addObjectsFromArray(aryqun);
                    }
                        break;
                    case 3:
                    {
                        datas->addObjectsFromArray(arywu);
                        datas->addObjectsFromArray(aryshu);
                        datas->addObjectsFromArray(arywei);
                        datas->addObjectsFromArray(aryqun);
                        
                    }
                        break;
                    case 4:
                    {
                        datas->addObjectsFromArray(aryqun);
                        datas->addObjectsFromArray(aryshu);
                        datas->addObjectsFromArray(arywei);
                        datas->addObjectsFromArray(arywu);
                    }
                        break;
                        
                    default:
                        break;
                }
                
                aryqun->removeAllObjects();
                aryshu->removeAllObjects();
                arywei->removeAllObjects();
                arywu->removeAllObjects();
                aryqun->release();
                aryshu->release();
                arywei->release();
                arywu->release();
                aryqun->release();
                aryshu->release();
                arywei->release();
                arywu->release();

                
            }
            else if(propsary->count())
            {
                std::sort(propsary->data->arr, propsary->data->arr + propsary->data->num, SGStarLess);
                datas->addObjectsFromArray(propsary);
            }

        }
        else
        {
            
            if(propsary->count())
            {
                std::sort(propsary->data->arr, propsary->data->arr + propsary->data->num, SGStarLess);
                datas->addObjectsFromArray(propsary);
            }
            else
            {
                SG_SHOW_WINDOW(str_StrengLayer_str9);
            }
        }
        datas->addObjectsFromArray(unsortarrary);
        propsary->removeAllObjects();
        propsary->release();
        propsary->release();


    }
    else
    {
        std::sort(datas->data->arr, datas->data->arr + datas->data->num, SGStarLess);//先按星级低排序
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//自动添加材料回调
void SGStrengLayer::autoStreng()
{
    
    if (addFlags < 5)
    {
        //addFlags=0;
        if (_card->getCurrLevel() >= strengLimitLevel&&onlyOfficer==false)
        {
            if (cardType == 1)
            {
                SG_SHOW_WINDOW(str_GeneralInfoLayer_str4_1);
                return;
            }
            else if (cardType == 3)
            {
                SG_SHOW_WINDOW(str_GeneralInfoLayer_str4_2);
                return;
            }
        }
        isautostrengenough=false;
        tableView->reloadData();//防止滚动之后，cell复用造成item为NULL而崩溃
        do
        {
            //第一个for作用：清除所有手动选择的状态。
            for (int i = 0; i < sizeof(selectItem) / sizeof(selectItem[0]); i++)
            {
                SNSIndexPath* indexPath = NULL ; //SNSIndexPath::create(0, 0, 1);;
                if (selectItem[i] == 0)
                {
                    continue;
                }
                else if (selectItem[i] == 1)
                {
                    indexPath = SNSIndexPath::create(i, 0, 1);
                }
                Tabel_Item *item = (Tabel_Item*)tableView->getItemByIndexPath(indexPath);
                if (!item)
                {
                    continue;
                }
                SGBaseMilitaryCard *basecard = item->_card;
			
                times--;
                addFlags--;
                this->changeNum(-1);
                selectItem[i] = 0;
                
                SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
                selectposition = selectposition<basecard->getselectposition()?selectposition:basecard->getselectposition();
                cardposition->removeChildByTag(110);
                cardposition->removeChildByTag(111);
                cardposition->removeChildByTag(112);
                CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
                ico->setVisible(true);
                _arraySelectCard->removeObject(datas->objectAtIndex(indexPath->getRow()));
            }
            IF_ERROR_BREAK(datas->count() <= 0, str_StrengLayer_st10);//重复点击自动添加,  或者没有数据
            if (addFlags < 5)
            {
                reSortCard();//按规则重新排序卡牌
            }
            //tableView->reloadData();//重新载入新的卡牌
            int tabNum = datas->count();
            SNSIndexPath *tempIndex = SNSIndexPath::create(0, 0, 1);
            /*
             leavenum:不进入排序的序列，放在最后
             sortnum :将要进行排序的序列个数。
             */
            int sortnum = ((datas->count()-leavenum)>=5) ? 5:(datas->count()-leavenum);//看真正排序的卡片数量
            for (int i = 0; i < tabNum; i++)
            {
                tempIndex = SNSIndexPath::create(i, 0, 1);
                int tabIndex = tableView->getItemPointerWithIndexPath(tempIndex);
                
                Tabel_Item *item = (Tabel_Item *)tableView->dequeueReusableCellItemForIndexPath(tempIndex);
                SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(tabIndex);
                if (item == NULL)
                {
                    item = Tabel_Item::create(card,tabIndex);
                }
                else
                {
                    item->updateOfficerCard(card,tempIndex->getRow(),selectItem[tempIndex->getRow()]);
                }
                
                if ((times < sortnum)&&!isautostrengenough)//最多加五张
                {
                    item->setItemSelect(true);
                    this->changeNum(1);
                    selectItem[tempIndex->getRow()] = 1;
                    _arraySelectCard->addObject(item->_card);
                    this->setSelectCard(item);
                    this->updateCoind();//更新数值信息
                    times++;
                    addFlags++;
                }
                else if(times>=5)
                {
                    item->setIsCanSelect(false);
                }
                if(i>10)
                    break;
            }
        }while (0);
    }
}

SNSTableViewCellItem* SGStrengLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{	
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	Tabel_Item *item = (Tabel_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(num);
   // SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
    CCLOG("indexPath->getRow()  is %d",num);
    if (item == NULL)
    {
        item = Tabel_Item::create(card,num);
    }
    else if (num < datas->count())
    {
        item->updateOfficerCard(card,indexPath->getRow(),selectItem[indexPath->getRow()]);
    }
    
    if (_arraySelectCard->count() == 5)
    {
        
        item->setIsCanSelect(_arraySelectCard->containsObject(card));
        
    }
    else
    {
        item->setIsCanSelect(true);
    }
	
    item->setTouchEnabled(true);
	
	return item;
}

void SGStrengLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag)
{
    if (_card->getCurrLevel() >= strengLimitLevel&&onlyOfficer==false)
    {
        
        if (cardType == 1)
        {
            SG_SHOW_WINDOW(str_GeneralInfoLayer_str4_1);
            return;
        }
        else if (cardType == 3)
        {
            SG_SHOW_WINDOW(str_GeneralInfoLayer_str4_2);
            return;
        }
        //SG_SHOW_WINDOW(str_GeneralInfoLayer_str4);
        
    }
    EFFECT_PLAY(MUSIC_BTN);
    
    CCLOG("btntag:%d",btnTag);
    
    if (btnTag) {
        return;
    }
    Tabel_Item *item = (Tabel_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBaseMilitaryCard *basecard = item->_card;
    
    isSelect = indexPath->getRow();
    //最多可以选择五张
    if(cardCount >= 5 && 0==selectItem[indexPath->getRow()])
    {
        SG_SHOW_WINDOW(str_StrengLayer_st11);
        return;
    }
    if(selectItem[indexPath->getRow()])
    {
        addFlags--;
		times--;//开始手动选择
		if (times <= 0)
			times = 0;
        this->changeNum(-1);
        selectItem[indexPath->getRow()] = 0;
        CCLOG("_arraySelectCard->indexOfObject(basecard) %d",_arraySelectCard->indexOfObject(basecard));
        SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
        selectposition = selectposition<basecard->getselectposition()?selectposition:basecard->getselectposition();
//        cardposition->removeAllChildrenWithCleanup(true);
//        cardposition->removeChildByTag(10);
        cardposition->removeChildByTag(110,true);
        cardposition->removeChildByTag(111,true);
        cardposition->removeChildByTag(112,true);
//        cardposition->removeAllChildren();
        CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
        ico->setVisible(true);
        _arraySelectCard->removeObject(datas->objectAtIndex(indexPath->getRow()));
        
    }
    else
    {
        
        CCString *str_ = CCString::create(basecard->getClassName());
        if (str_->isEqual(CCString::create("SGOfficerCard")))
        {
            SGOfficerCard *card = (SGOfficerCard*)item->_card;
            if (card->getOfficerEquips() && card->getOfficerEquips()->count())
            {
                SGCantAdvanceBox *haveEquip = SGCantAdvanceBox::create(this, NULL,14,0);
                SGMainManager::shareMain()->showBox(haveEquip);
                tabelItem = item;
                return;
            }
            else
            {
                addFlags++;
				times++;//开始手动选择
				if (times >= 5)
					times = 5;
                this->changeNum(1);
                selectItem[indexPath->getRow()] = 1;
                _arraySelectCard->addObject(datas->objectAtIndex(indexPath->getRow()));
            }
        }
        else
        {
            addFlags++;
			times++;//开始手动选择
			if (times >= 5)
				times = 5;
            this->changeNum(1);
            selectItem[indexPath->getRow()] = 1;
            _arraySelectCard->addObject(datas->objectAtIndex(indexPath->getRow()));
        }
		//this->setSelectCard(item);//11.25
        basecard->setselectposition(selectposition);
        
        SGButton *position = (SGButton *)menu->getChildByTag(basecard->getselectposition());

        while (NULL != position->getChildByTag(110))
        {
            selectposition++;
            basecard->setselectposition(selectposition);
            position = (SGButton *)menu->getChildByTag(basecard->getselectposition());
        }

        
        SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
        cardposition->setUserObject(indexPath);
        cardposition->setEnabled(true);
        CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
        ico->setVisible(false);
        selectposition ++;
        SGMainManager::shareMain()->addHeadIconPlistByNum(basecard->getHeadSuffixNum(), sg_strengLayer);
		CCSprite *icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",basecard->getHeadSuffixNum())->getCString());
        icon->setPosition(ccp(cardposition->getContentSize().width/2, cardposition->getContentSize().height/2));
        cardposition->addChild(icon,1,110);

        ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
         ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
        CCSprite *kuang =  CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
        cardposition->addChild(kuang,1,111);
        kuang->setPosition(icon->getPosition());
        
        CCSprite *background =  CCSprite::createWithSpriteFrameName("card_bj.png");
//        SGButton *background = SGButton::create("card_bj.png", NULL, this, menu_selector(SGStrengLayer::btnselected),CCPointZero,false,true);
        cardposition->addChild(background,0,112);
//        this->addBtn(background);
        background->setPosition(icon->getPosition());
        
        CCString *string = CCString::create(basecard->getClassName());
        if (string->isEqual(CCString::create("SGOfficerCard")))
        {
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(basecard->getItemId());
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1)
            {
                starlvl-=1;
            }
            kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
            if (kuang)
            {
                kuang->removeChildByTag(30, true);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                kuang->addChild(countryName,30,30);
                
                kuang->removeChildByTag(250, true);
                if (temp->getAdvNum()>0) {
                    SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
                    advancenumber->setAnchorPoint(ccp(1,1));
                    advancenumber->setPosition(kuang->getContentSize()/1.2);
                    kuang->addChild(advancenumber,50,250);
                }

            }
        }
        else if (str_->isEqual(CCString::create("SGEquipCard")))
        {
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(basecard->getItemId());
            kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
            
            kuang->removeChildByTag(250, true);
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->setPosition(kuang->getContentSize()/1.2);
                kuang->addChild(advancenumber,50,250);
            }

        }
        else
        {
            SGPropsCard *card = (SGPropsCard *)basecard;
            if (card->getType() == 1) {
                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());
                int starlvl = temp->getPropsCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                if (kuang)
                {
                    kuang->removeChildByTag(30, true);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                }

            }
            else
            {
                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());
                int starlvl = temp->getPropsCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                if (card->getType() == 4)
                {
                    kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                    if (kuang)
                    {
                        kuang->removeChildByTag(30, true);
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                        kuang->addChild(countryName,30,30);
                    }

                }
                else if (card->getType() == 5) {
                    //kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName("jinkuang.png")->displayFrame());
//                    kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png")->displayFrame());
                    
                    kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                    //            kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString())->displayFrame());
                    if (kuang)
                    {
                        kuang->removeChildByTag(30, true);
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                        kuang->addChild(countryName,30,30);
                    }
                    
                }
                else
                {
                    //kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName("yinkuang.png")->displayFrame());
                    kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                    
                }
                
            }
        }
        
    }
    updateCoind();
    item->setItemSelect(0 != selectItem[indexPath->getRow()]);
    tabelItem = item;
    
    tableView->reloadData(false);
    
    std::sort(_arraySelectCard->data->arr, _arraySelectCard->data->arr + _arraySelectCard->data->num, SGLess);
}


void SGStrengLayer::btnselected(SGButton *obj)
{
   
    SNSIndexPath *indexPath = (SNSIndexPath *)obj->getUserObject();
     CCLOG("sagagfazgfdag%d",indexPath->getRow());
    addFlags--;
    times--;//开始手动选择
    if (times <= 0)
        times = 0;
    this->changeNum(-1);
    selectItem[indexPath->getRow()] = 0;
    obj->removeChildByTag(110,true);
    obj->removeChildByTag(111,true);
    obj->removeChildByTag(112,true);
    int base = obj->getTag();
    selectposition = selectposition<base?selectposition:base;
    CCSprite *ico = (CCSprite*)this->getChildByTag(100+base);
    ico->setVisible(true);
    
    
    Tabel_Item *item = (Tabel_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (NULL != item)
    {
        item->setItemSelect(0 != selectItem[indexPath->getRow()]);
        tabelItem = item;
    }
    if(datas->count()>0)
    _arraySelectCard->removeObject(datas->objectAtIndex(indexPath->getRow()));
    
    
    updateCoind();
   
    
    tableView->reloadData(false);
    
    std::sort(_arraySelectCard->data->arr, _arraySelectCard->data->arr + _arraySelectCard->data->num, SGLess);
    obj->setEnabled(false);

}

void SGStrengLayer::selectItemByIndex(int index)
{
    this->changeNum(1);
    selectItem[index] = 1;
    _arraySelectCard->addObject(datas->objectAtIndex(index));
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    updateCoind();
}

void SGStrengLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *type = (CCString *)dict->valueForKey("type");
        if(type->intValue() == 5)
        {
            this->changeNum(1);
            selectItem[isSelect] = 1;
            _arraySelectCard->addObject(datas->objectAtIndex(isSelect));
            tabelItem->setItemSelect(0 != selectItem[isSelect]);
            updateCoind();
            SGBaseMilitaryCard *basecard = tabelItem->_card;
            basecard->setselectposition(selectposition);
            
            SGButton *position = (SGButton *)menu->getChildByTag(basecard->getselectposition());
            while (position->getChildByTag(110))
            {
                selectposition++;
                basecard->setselectposition(selectposition);
                position = (SGButton *)menu->getChildByTag(basecard->getselectposition());
            }
            SGButton *cardposition = (SGButton *)menu->getChildByTag(basecard->getselectposition());
            cardposition->setEnabled(true);
             cardposition->setUserObject(tabelItem->getIndexPath());
            CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
            ico->setVisible(false);
            selectposition ++;
//            CCSprite *icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",basecard->getItemId())->getCString());
			CCSprite *icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",basecard->getHeadSuffixNum())->getCString());
			
            icon->setPosition(ccp(cardposition->getContentSize().width/2, cardposition->getContentSize().height/2));
            cardposition->addChild(icon,1,110);
            CCSprite *kuang =  CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
            cardposition->addChild(kuang,1,111);
            kuang->setPosition(icon->getPosition());
            CCString *string = CCString::create(basecard->getClassName());
            if (string->isEqual(CCString::create("SGOfficerCard")))
            {
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(basecard->getItemId());
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
				ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
                if (kuang)
                {
                    kuang->removeChildByTag(30);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                    
                    kuang->removeChildByTag(250, true);
                    if (temp->getAdvNum()>0) {
                        SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
                        advancenumber->setAnchorPoint(ccp(1,1));
                        advancenumber->setPosition(kuang->getContentSize()/1.2);
                        kuang->addChild(advancenumber,50,250);
                    }

                }
            }
            tableView->reloadData(false);
        }
        else if (type->intValue()==10)
        {
            confirmHandler();
        }
    }
}
void SGStrengLayer::updateCoind()
{

    
    if (onlyOfficer)
    {
        int rate = 0;   //此rate为主将计能升级概率
        int addexp = 0;
        SGOfficerCard *temp = (SGOfficerCard *)_card;
       
        int len = _arraySelectCard->count();
        for (int i = 0; i<len; i++)
        {
            SGPropsCard *card = (SGPropsCard *)_arraySelectCard->objectAtIndex(i);
            addexp += card->getValue();
            rate += (card->getCurrStar() + 1 ) * 0.5;
            if (card->getRace() == temp->getRace()) //加技能相同国家的时候 经验加倍
            {
                addexp +=  card->getValue() ;
            }
        }
        SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(temp->getLordId());
        int needexp = lordSkill->getNeedExp();
        if (needexp <= (addexp + temp->getLordExp()))
        {
            rate += 62 + 2*_arraySelectCard->count();
        }
        else
        {
            rate += 100*addexp/(needexp - temp->getLordExp())*2/3;
        }
        
        if (rate>0)
        {
            if (rate>=80&&rate<100)
            {
                rate = rate/2+40;
            }
            if (rate<=5)
            {
                rate = 5;
            }
            if(rate)
            {
                if(rate>=100)
                    rate = 90 + 10*(rate-90)/rate;
            }
            skilluprate->setString(CCString::createWithFormat("%d%%",rate)->getCString());
            skilluprate->setInsideColor(ccc3(0x0c, 0xff, 0x00));
        }
        else
        {
            skilluprate->setString("0%");
            skilluprate->setInsideColor(ccWHITE);
        }
    }
    else if (cardType != 4)//cardType=4时为被动技能
    {
        CCString *str_ = CCString::create(_card->getClassName());
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
        
        
        CCLOG("type is %d",type);
        int currLvl = _card->getCurrLevel();
        int nextLvl = currLvl;
        if(currLvl < strengLimitLevel)
            nextLvl = currLvl+1;
        SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, nextLvl);//比如currLvl为5级 此句话获得5级升6级所需经验值

        
        //刷新满级经验值
        {
            CCString *str_ = CCString::create(_card->getClassName());
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
            
 
            CCLOG("type is %d",type);
            int currLvl = _card->getCurrLevel();
            int maxlvl = strengLimitLevel;
            SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);//比如currLvl为5级 此句话获得5级升6级所需经验值
            SGExpDataModel *num2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, maxlvl);//比如currLvl为5级 此句话获得5级升6级所需经验值
            
            maxneedexp = num2->getexpAll()-num->getexpAll()-_card->getCurrExp();
            if(maxneedexp<=0)
                maxneedexp=0;
            CCString* maxexpstring = CCString::createWithFormat("%d",maxneedexp);
            maxe->setString(maxexpstring->getCString());

        }
        int addEXP = 0;
        int proExp = 0;
        for(int i=0;i<_arraySelectCard->count();i++)
        {
            SGBaseMilitaryCard *card_ = (SGBaseMilitaryCard*)_arraySelectCard->objectAtIndex(i);
            int cardCurrStar = card_->getCurrStar();
            CCString *str_1 = CCString::create(card_->getClassName());
            if (str_1->isEqual(CCString::create("SGPropsCard")))
            {
                proExp = proExp +  (((SGPropsCard *)card_)->getValue());
                if (str_->isEqual(CCString::create("SGOfficerCard")) &&((SGPropsCard *)card_)->getRace() == ((SGOfficerCard *)_card)->getRace())
                {
                    proExp = proExp + (((SGPropsCard *)card_)->getValue())*0.3;
                }
            }
            else if(str_1->isEqual(CCString::create("SGOfficerCard")))
            {
                float expRate = card_->getExpRate();
                int lvl = card_->getCurrLevel();
                addEXP += ( cardCurrStar * expRate * lvl) + 0.5;
                if (((SGOfficerCard *)card_)->getRace() == ((SGOfficerCard *)_card)->getRace())
                {
                    //addEXP = addEXP + ((gov + cardCurrStar) * expRate * lvl)*0.3;
                    //addEXP = addEXP ;//公式修改
                    addEXP += 0.3 * ( cardCurrStar * expRate * lvl);//2014-8-25 武将强化，国别相同经验加成30%。 by:zyc.
                }
            }
            else if(str_1->isEqual(CCString::create("SGEquipCard")))
            {
                float expRate = card_->getExpRate();
                int lvl = card_->getCurrLevel();
                addEXP += (cardCurrStar  * expRate * lvl)+.5;
                if (((SGEquipCard *)card_)->getItemType() == ((SGEquipCard *)_card)->getItemType())
                {
                    addEXP = addEXP ;//公式修改
                }
            }
        }
        int totleExp = addEXP +proExp;
        SGExpDataModel *maxExp=NULL;
        if(currLvl  >=  strengLimitLevel)
        {
            maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
        }
        else
        {
            maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
        }
        if (maxExp->getExpValue() == _card->getCurrExp())
        {
            progress->setPercentage(100);
            return;
        }
        if (totleExp < 0)
        {
            totleExp = 0;
        }
        afterExp->setString(CCString::createWithFormat("%d",totleExp)->getCString());
        if (totleExp <= maxneedexp)
        {
            afterExp->setInsideColor(ccWHITE);
        }
        else
        {
            afterExp->setInsideColor(ccRED);
        }
       
        totleExp += _card->getCurrExp();//
        while ((totleExp >= num->getExpValue())&&(currLvl !=strengLimitLevel))
        {
            
            totleExp -= num->getExpValue();
            currLvl += 1;
          //  CCLOG("%d",num->getExpValue());
            /////////////////kanata      ///////如果经验超了最大等级  直接设定为最大等级
             if (currLvl  >= strengLimitLevel)
             {
                 
                 currLvl = strengLimitLevel;
                 SGExpDataModel*  num2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
                // CCLOG("%d",num2->getExpValue());
                 totleExp=num2->getExpValue();//比如满级是10级  经验超了直接设经验条数值为9升10所需经验值数据
                 isautostrengenough = true;
             }
             else
             {
                 num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
             }
        }
        

        float rate = 0;//强化后经验条百分比
        float per = (float)_card->getCurrExp()/maxExp->getExpValue();//强化前经验条百分比
        if (currLvl > _card->getCurrLevel())
        {
            rate = (float)totleExp/maxExp->getExpValue();
            
            progress->setPercentage(rate*100);
            progress->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(1, 0),CCFadeTo::create(1, 255),NULL)));
            
        }
        else
        {
            rate = (float)totleExp/maxExp->getExpValue();
        }
        
        if (currLvl > strengLimitLevel)
        {
            ////////////////////不会再走的路线
            currLvl = strengLimitLevel;
            SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
            SGExpDataModel *orinum = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, _card->getCurrLevel()-1);

            int expTemp = num->getexpAll() - _card->getCurrExp();
            
            if (orinum)
            {
                expTemp -= orinum->getexpAll();
            }
            expTemp = expTemp < 0 ? 0 : expTemp;
            afterExp->setString(CCString::createWithFormat("%d", expTemp)->getCString());

            progress->setPercentage(100);
        }
        else if (currLvl == _card->getCurrLevel() && cardCount != 0)
        {
            progress->stopAllActions();
            progress->setVisible(true);
            progress->setColor(ccWHITE);
            progress->setPercentage(rate*100);
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_addexp.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
        }
        if (cardCount == 0)
        {
            progress->stopAllActions();
            progress->setOpacity(255);
            progress->setColor(ccWHITE);
            currLvl = _card->getCurrLevel();
            progress->setPercentage(per*100);
        }
        lab_orilvl->setop(0);  
        lab_lvl->labelstopact();
        lab_lvl->setop(255);
        //lab_lvl->fadin(0.1);
        lab_lvl->stopAllActions();
        lab_lvl->setString(CCString::createWithFormat("Lv %d",currLvl)->getCString());
        lab_lvl->setVisible(true);

        if (currLvl> _card->getCurrLevel())
        {
            lab_lvl->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create((CCActionInstant*)lab_lvl->fadout(1),CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(SGStrengLayer::labelcallback)),CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(SGStrengLayer::labelcallback1)),CCDelayTime::create(1),(CCActionInstant*)lab_lvl->fadin(1),CCDelayTime::create(1),NULL)));
            
            
            //为了完成功能
            CCDictionary *dicc = SGCardAttribute::getValue(currLvl, _card->getItemId());
            CCDictionary *dicc1 = SGCardAttribute::getValue(_card->getCurrLevel(), _card->getItemId());
            
            
            lab_addack->setString(CCString::createWithFormat("+%d",((CCString *)dicc->objectForKey("atk"))->intValue()
															 -((CCString *)dicc1->objectForKey("atk"))->intValue())->getCString());
			SGCCLabelTTF *lab_atk = ((SGCCLabelTTF *)getChildByTag(LABEL_ATK));
			lab_addack->setPosition(ccpAdd(lab_atk->getPosition(), ccp(lab_atk->getContentSize().width + 6, 0)));
			
            
            lab_adddef->setString(CCString::createWithFormat("+%d",((CCString *)dicc->objectForKey("def"))->intValue()-
                                                        ((CCString *)dicc1->objectForKey("def"))->intValue())->getCString());
			SGCCLabelTTF *lab_def = ((SGCCLabelTTF *)getChildByTag(LABEL_DEL));
			lab_adddef->setPosition(ccpAdd(lab_def->getPosition(), ccp(lab_def->getContentSize().width + 6, 0)));
			
            
            lab_addmoral->setString(CCString::createWithFormat("+%d",((CCString *)dicc->objectForKey("mor"))->intValue()-
															   ((CCString *)dicc1->objectForKey("mor"))->intValue())->getCString());
			SGCCLabelTTF *lab_ap = ((SGCCLabelTTF *)getChildByTag(LABEL_AP));
			lab_addmoral->setPosition(ccpAdd(lab_ap->getPosition(), ccp(lab_ap->getContentSize().width + 6, 0)));
            
          if (_card->getItemId()>2999 &&_card->getItemId() <4000)
          {
          }
          else
          {
            lab_addSpeed->setString(CCString::createWithFormat("+%d",(int)(((CCString *)dicc->objectForKey("speed"))->floatValue()-((CCString *)dicc1->objectForKey("speed"))->intValue()))->getCString());
            SGCCLabelTTF *lab_Speed = ((SGCCLabelTTF *)getChildByTag(LABEL_SP));
            lab_addSpeed->setPosition(ccpAdd(lab_Speed->getPosition(), ccp(lab_Speed->getContentSize().width +6, 0)));
          }
        }
        else
        {
            lab_addack->setString("");
            lab_adddef->setString("");
            lab_addmoral->setString("");
            lab_addSpeed->setString("");
        }
        
    }
    else
    {    //被动技比率计算
        SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getGeneralSkillById(officer->getOfficerSkil());
        
        int rate = 0;
        
        int addexp = 0;
        int len = _arraySelectCard->count();
        for (int i = 0; i<len; i++)
        {
            SGPropsCard *card = (SGPropsCard *)_arraySelectCard->objectAtIndex(i);
            addexp += card->getValue();
            rate += (card->getCurrStar()+1)*0.5;
            if (card->getRace() == officer->getRace())//加技能相同的时候
            {
                addexp+=card->getValue();
            }
        }
        
        int needexp = skill->getNeedExp();
        
        if (needexp <= (addexp + officer->getSkillExp()))
        {
            rate += 62 + 2*_arraySelectCard->count();
        }
        else
        {
            rate += 100*addexp/(needexp - officer->getSkillExp())*2/3;
        }
        if (rate>0)
        {
            
            if (rate>=80)
            {
                rate = 80;
            }
            if (rate<=5)
            {
                rate = 5;
            }
            skilluprate->setString(CCString::createWithFormat("%d%%",rate)->getCString());
            skilluprate->setInsideColor(ccc3(0x0c, 0xff, 0x00));
        }
        else
        {
            skilluprate->setString("0%");
            skilluprate->setInsideColor(ccWHITE);
            
        }
    }
}

void SGStrengLayer::sortHandler()
{
}
void SGStrengLayer::strengOfficerListener(CCObject *obj)
{
    SGOfficerCard* officerCard = NULL;
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::OfficerCompositeResponse *response = (main::OfficerCompositeResponse *)sr->m_msg;
        main::OfficerCardItemProto proto = response->officercarditemproto();
        if (response->state() == 1)
        {
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            officerCard = new SGOfficerCard;
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
            
            
            /////如果未满级   /////kanata
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
            ////如果满级了
            SGExpDataModel *expdata2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel());
            /////
            
            officerCard->setSsid(proto.ssid());
            officerCard->setItemId(proto.itemid());
            /////////////kanata   //如果未满级 会给经验数值 如果满级了  服务器会下发数值为0
            officerCard->setCurrExp(proto.currexp());
            //////////////////////
            officerCard->setLordId(proto.lordskillid());
            officerCard->setIsProt(proto.isprotection());
            //            officerCard->setIsExped(proto.isexpedition());
            officerCard->setCurrLevel(proto.currlevel());
            officerCard->setLordExp(proto.lordskillcurrexp());
            
            //武将攻防血速差异性，这几个属性改由服务器发送
            officerCard->setRealAtk(proto.atk());
            officerCard->setRealDef(proto.def());
            officerCard->setRealMorale(proto.hp());
            officerCard->setRealSpeed(proto.sp());
            
            if (temp->getOfficerNextId())
            {
                officerCard->setCost(temp->getOfficerCost());
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
            
            
            //////////kanata
            if(proto.currlevel() >= strengLimitLevel)
            {
            officerCard->setMaxExp(expdata2->getExpValue());
            officerCard->setCurrExp(expdata2->getExpValue());//满级了 把经验条顶满  因为如果满级了  服务器会下发数值为0
            }
            else
            {
                officerCard->setMaxExp(expdata->getExpValue());
            
            }
           // officerCard->setMaxExp(expdata->getExpValue());
            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
            officerCard->setOfficerName(temp->getOfficerName());
            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            officerCard->setRound(temp->getOfficerRound());
            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            officerCard->setGoverning(temp->getOfficerGoverning());
            officerCard->setRace(temp->getOfficerRace());
            officerCard->setItemType(temp->getOfficerType());
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            officerCard->setFateList(temp->getOfficerFateList());
            //转生次数
            officerCard->setAdNum(temp->getAdvNum());
            //武将原型ID
            officerCard->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
			
			officerCard->setHeadSuffixNum(temp->getIconId());//12.05
            
            
            if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
            {
                std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                officerCard->setOfficerSkil(atoi(generalskill.c_str()));
            }
            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
            {
                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                officerCard->setLordSkill(atoi(lordskill.c_str()));
            }
            officerCard->setOfficerSkil(proto.generalsskillid());
            int len = _arraySelectCard->count();
            for (int i = 0; i < len; i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    playerInfo->getPropCards()->removeObject(baseMilitaryCard);
                }
                
                else if(str_->isEqual(CCString::create("SGOfficerCard")))
                {
                    SGOfficerCard *card = (SGOfficerCard *)baseMilitaryCard;
                    if (card->getOfficerEquips())
                    {
                        for (int i = 0 ; i<card->getOfficerEquips()->count(); i++)
                        {
                            SGEquipCard *equip =(SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
                            playerInfo->getEquipCard(equip->getSsid())->setOfficerCardId(0);
                        }
                    }
                    playerInfo->getOfficerCards()->removeObject(baseMilitaryCard);
                }
            }
            SGOfficerCard *before = (SGOfficerCard *)_card;
            CCObject *object = NULL;
            CCARRAY_FOREACH(before->getOfficerEquips(), object)
            {
                SGEquipCard *card = (SGEquipCard *)object;
                officerCard->addEquip(card);
            }
            //服务器直接推送更新消息

            SGMainManager::shareMain()->updataUserMsg();

            
            CCDictionary *dict = CCDictionary::create();
            CCString *str = CCString::createWithFormat(str_StrengLayer_st12,response->allexp());
            dict->setObject(CCString::createWithFormat("%d",response->upgradestate()), "upstate");
            dict->setObject(str, "skillup");
			
			//强化成功出现的提示
			CCPoint pos;
			CCSize s = CCDirector::sharedDirector()->getWinSize();
			if (s.height == 960)
			{
				pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -401));
			}
			else if (s.height == 1136)
			{
				pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -537));
			}

			
			//效果layer直接显示在本页面
			SGStrengEffectLayer *eff = SGStrengEffectLayer::create(1, response->upgradestate(), officerCard);
			SGMainManager::shareMain()->showBox(eff);

            officerCard->autorelease();
			
        }
        else
        {
            SGMainManager::shareMain()->showGeneralInfoLayer(officer, 10001);
            
        }
    }
}



void SGStrengLayer::strengLordSkillListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        int ori = ((SGOfficerCard*)_card)->getLordId();
        main::SkillCompositeResponse *response = (main::SkillCompositeResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            ((SGOfficerCard*)_card)->setLordExp(response->skillexp());
            ((SGOfficerCard*)_card)->setLordId(response->skillid());
            if (isfromfirst)
            {
                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 12,true,ori==response->skillid());
            }
            else
            {
                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 11,true,ori==response->skillid());
            }
            
        }
        else
        {
            SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 11);
        }
    }

}



void SGStrengLayer::strengSkillListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        int ori = ((SGOfficerCard*)_card)->getOfficerSkil();
        main::SkillCompositeResponse *response = (main::SkillCompositeResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            ((SGOfficerCard*)_card)->setSkillExp(response->skillexp());
            ((SGOfficerCard*)_card)->setOfficerSkil(response->skillid());

            if (isfromfirst)
            {
                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 3,true,ori==response->skillid());
            }
            else
            {
                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 4,true,ori==response->skillid());
            }
            
        }
        else
        {
            SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 10);
//            SGMainManager::shareMain()->showMessage("失败");
        }
    }
    
}
void SGStrengLayer::strengEquipListener(cocos2d::CCObject *obj)
{
    //SGEquipCard* equipCard = NULL;
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::EquipmentCompositeResponse *response = (main::EquipmentCompositeResponse *)sr->m_msg;
        main::EquipmentCardItemProto cardInfo = response->equipmentcarditemproto();
        if (response->state() == 1)
        {
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            //equipCard = new SGEquipCard;
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            ////如果满级了
            SGExpDataModel *expdata2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel());
            
            
            
            SGEquipCard *equipCard = new SGEquipCard();
            equipCard->setSsid(cardInfo.ssid());
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(cardInfo.itemid());
            //            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(cardInfo.currlevel());

            ///////////////////////////////////////////////////////////
     
            if(cardInfo.currlevel() >= strengLimitLevel)
            {
                equipCard->setMaxExp(expdata2->getExpValue());
                //equipCard->setCurrExp(expdata2->getExpValue());//满级了 把经验条顶满  因为如果满级了  服务器会下发数值为0
            }
            else
            {
                equipCard->setMaxExp(expdata->getExpValue());
                
            }

            ////////////////////////////////////////////////////////////
            equipCard->setOfficerCardId(cardInfo.officercarditemid());
            equipCard->setIsProt(cardInfo.isprotection());
            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
//            equipCard->setMaxExp(expdata->getExpValue());
            equipCard->setRound(temp->getEquipRound());
            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            equipCard->setExpRate(temp->getEquipExpRate());
            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
            equipCard->setGoverning(temp->getEquipGoverning());
            equipCard->setItemType(temp->getEquipType());
            equipCard->setMaxStar(temp->getEquipNextId());
            equipCard->setOfficerName(temp->getEquipName());
            equipCard->setMaxLevel(temp->getEquipMaxLevel());
            if (temp->getFateDesc())
            {
                equipCard->setFateDesc(temp->getFateDesc());
            }			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
			
            if (temp->getEquipNextId())
            {
                equipCard->setCost(temp->getEquipCost());
            }
            int len = _arraySelectCard->count();
            for (int i = 0; i < len; i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    playerInfo->getPropCards()->removeObject(baseMilitaryCard);
                }
                
                else if(str_->isEqual(CCString::create("SGEquipCard")))
                {
                    playerInfo->getEquipCards()->removeObject(baseMilitaryCard);
                }
            }
            if (equipCard->getOfficerCardId())
            {
                SGOfficerCard *card = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(equipCard->getOfficerCardId());
                card->removeEquip((SGEquipCard*)_card);
                card->addEquip(equipCard);
            }
            
            //服务器直接推送更新背包消息
            //playerInfo->getEquipCards()->removeObject(_card);
            //playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - needCoins);
            SGMainManager::shareMain()->updataUserMsg();
            //playerInfo->addEquipCard(equipCard);
            
            
            CCDictionary *dict = CCDictionary::create();
            
            CCString *str = CCString::createWithFormat(str_StrengLayer_st12,response->allexp());
            dict->setObject(CCString::createWithFormat("%d",response->upgradestate()), "upstate");
            dict->setObject(str, "skillup");
            
			//装备强化, 成功直接显示在本页
			SGStrengEffectLayer *eff = SGStrengEffectLayer::create(cardType, response->upgradestate(), equipCard);
			SGMainManager::shareMain()->showBox(eff);
			
            equipCard->autorelease();
        }
        else
        {
             SGMainManager::shareMain()->showEquipInfoLayer(equip,10001);
           // SGMainManager::shareMain()->showMessage("失败");
            
        }
    }
    
}
void SGStrengLayer::strengSoldierListener(cocos2d::CCObject *obj)
{
    SGSoldierCard* soldierCard = NULL;
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SoldierCompositeResponse *response = (main::SoldierCompositeResponse *)sr->m_msg;
        main::SoldierCardItemProto proto = response->soldiercarditemproto();
        if (response->state() == 1)
        {
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            soldierCard = new SGSoldierCard;
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
            /////////////////////
            
            //SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            ////如果满级了
            SGExpDataModel *expdata2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
            
            /////////////////////
            
            soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
            soldierCard->setSsid(proto.ssid());
            soldierCard->setItemId(proto.itemid());
            soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            soldierCard->setMaxExp(expdata->getExpValue());
            soldierCard->setOfficerName(temp->getSoldierName());
            soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            soldierCard->setRound(temp->getSoldierRound());
            soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            soldierCard->setCurrLevel(proto.currlevel());
            soldierCard->setCurrStar(temp->getSoldierStarLevel());
            soldierCard->setCurrExp(proto.currexp());
            ////////////
            if(proto.currlevel() >= strengLimitLevel)
            {
                soldierCard->setMaxExp(expdata2->getExpValue());
                soldierCard->setCurrExp(expdata2->getExpValue());//满级了 把经验条顶满  因为如果满级了  服务器会下发数值为0
            }
            else
            {
                soldierCard->setMaxExp(expdata->getExpValue());
                
            }
            soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
            soldierCard->setsoldiergrowskill(temp->getGrowSkill());

            
            /////////////
            
            int len = _arraySelectCard->count();
            for (int i = 0; i < len; i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                
                playerInfo->getPropCards()->removeObject(baseMilitaryCard);
            }
            
            playerInfo->getSoldierCards()->removeObject(_card);
            //playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - needCoins);
            SGMainManager::shareMain()->updataUserMsg();
            playerInfo->addSoldierCard(soldierCard);
            
            CCDictionary *dict = CCDictionary::create();
            
            CCString *str = CCString::createWithFormat(str_StrengLayer_st12,response->allexp());
            dict->setObject(CCString::createWithFormat("%d",response->upgradestate()), "upstate");
            dict->setObject(str, "skillup");
            
            //SGMainManager::shareMain()->showSoldierInfoLayer(soldierCard,10000,true,(SGSoldierCard*)_card,isfromfirst,dict);
			
			//士兵页面直接在本页显示
			SGStrengEffectLayer *eff = SGStrengEffectLayer::create(cardType, response->upgradestate(), _card);
			SGMainManager::shareMain()->showBox(eff);
			
            soldierCard->autorelease();
        }
        else
        {
            SGMainManager::shareMain()->showSoldierInfoLayer(soldier,10001);
        }
    }
}

//回调函数
void SGStrengLayer::sortLayerCallBack(CCArray *cardArray)
{
}

void SGStrengLayer::secondeconfirm()
{
    
    if (_card->getCurrLevel() >= strengLimitLevel&&onlyOfficer==false)
    {
        
        if (cardType == 1)
        {
            SG_SHOW_WINDOW(str_GeneralInfoLayer_str4_1);
            return;
        }
        else if (cardType == 3)
        {
            SG_SHOW_WINDOW(str_GeneralInfoLayer_str4_2);
            return;
        }
        //SG_SHOW_WINDOW(str_GeneralInfoLayer_str4);
        
    }
    if ((cardType ==1 && !onlyOfficer)|| cardType ==2)
    {
        CCObject *obj_ = NULL;
//        int x=0;
        CCARRAY_FOREACH(_arraySelectCard, obj_)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)obj_;
            CCString *str_nowcard = CCString::create(card->getClassName());
            if (card->getCurrStar()>=4 && (str_nowcard->isEqual(CCString::create("SGOfficerCard")) || str_nowcard->isEqual(CCString::create("SGEquipCard"))))
            {
                SGCantAdvanceBox *box = SGCantAdvanceBox::create(this,NULL,100,0);
                 SGMainManager::shareMain()->showBox(box);
                return;
            }
        }
        
        if (strengHaveSameNameOfficer(_card->getItemId()))
        {
            SGCantAdvanceBox *box = SGCantAdvanceBox::create(this,NULL,100100,0);
            SGMainManager::shareMain()->showBox(box);
            return;
        }
    
        confirmHandler();
    }
   else
   {
       confirmHandler();
   }
}

void SGStrengLayer::confirmHandler()
{

    EFFECT_PLAY(MUSIC_BTN);
    if (cardType == 2 ||cardType == 3)
    {
        
        CCString *str_ = CCString::create(_card->getClassName());
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
        
        SGExpDataModel *maxExp=NULL;
        int currLvl = _card->getCurrLevel();
        if(currLvl  >=  strengLimitLevel)
        {
           maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
        }
        else
        {
            maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
        }
        
        
        if (_card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
        {
            SGMainManager::shareMain()->showMessage(str_qianghua_tips1);
            return;
        }
    }
    
    if (cardCount>0 )//&& needCoins < SGPlayerInfo::sharePlayerInfo()->getPlayerCoins()
    {
        selectcardcount = _arraySelectCard->count()-1;
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(selectcardcount);
        SGButton *a = (SGButton*)menu->getChildByTag(card->getselectposition());
        SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(false);
        cardBlink(a);
        this->setIsCanTouch(false);
        if (fermEffect)
        {
//            fermEffect->removeAllChildrenWithCleanup(true);
            fermEffect->removeFromParentAndCleanup(true);
//            this->removeChild(fermEffect, true);
            fermEffect = NULL;
        }
        this->removeChildByTag(77, true);
    }
    else if(cardCount == 0){
        SGMainManager::shareMain()->showMessage(str_one_card_need);
    }
    
	/*  //removed by cgp
	if (CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") <= guide_tag_17)
	{
		main::BeginnersGuideRequest *request = new main::BeginnersGuideRequest();
        request->set_id(guide_tag_17);
        SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_ASK, request, false);
        if (SGGuideManager::shareManager()->getGuideLayer())
        {
            SGGuideManager::shareManager()->getGuideLayer()->setVisible(false);
            SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
        }
    }
     */
    
}
void SGStrengLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
	
	//如果从布阵界面进入
	if (EmbattleInfo && EmbattleInfo->isFromEmbattle)
	{
		CCLog("Back To Embattle");
		//返回布阵页面
        _card->retain();
		SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, (SGOfficerCard *)_card, EmbattleInfo->ty, EmbattleInfo->em);
		SGMainManager::shareMain()->showBox(embattleBox);
	}
    else if (isfromfirst==4)
    {
      SGMainManager::shareMain()->showGeneralsLayer(4,true,0,_currentPage);
    }
    else if(isfromfirst==5)
    {
      SGMainManager::shareMain()->showGeneralsLayer(5,true,0,_currentPage);
    }
    else if (isfromfirst==1)
	{
        if (cardType == 1)
        {
            SGMainManager::shareMain()->showGeneralsLayer(2,true,0,_currentPage);
        }
        else if (cardType == 2)
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL,0,1,true,0 ,_currentPage,0);
        }
        else if (cardType == 3)
        {
            SGStrengSoldierLayer *layer = SGStrengSoldierLayer::create();
            SGMainManager::shareMain()->showLayer(layer);
        }
        else
        {
            SGMainManager::shareMain()->showGeneralsLayer(5,true,0,_currentPage);
        }
    }
    else if (isfromfirst == JUNYING_INFO_ENTER)
    {
      SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card,0,NULL,true,NULL,false,NULL,1,0);
    }
    else if (isfromfirst == HOME_INFO_ENTER)
    {
      SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card, 3,NULL,true,NULL,11);
    }
    else if (isfromfirst == EMBATTLE_INFO_ENTER)
    {
      SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card, 22,NULL,true,NULL,12);
    }
    else if(isfromfirst==3)
    {
       //int id =  (SGEquipCard*)_card->getOfficerCardId();
        SGEquipCard * equip =(SGEquipCard*)_card;
        int id = equip->getOfficerCardId();
        
        SGMainManager::shareMain()->showGeneralInfoLayer(SGPlayerInfo::sharePlayerInfo()->getOfficerCard(id), 3,NULL,true,NULL,12);

    }
    else
    {
        if (cardType == 1)
        {
            SGMainManager::shareMain()->showGeneralsLayer(0,true,0,_currentPage);
        }
        else if (cardType == 2)
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL,0,0,true,0 ,_currentPage,0);
        }else if (cardType == 3){
            SGMainManager::shareMain()->showSoldierslayer(true,3);
        }else
        {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard*)_card, 0);
        }
    }
}
void SGStrengLayer::setIsCanTouch(bool isTouch)
{
    tableView->setIsTouchEnabled(isTouch);
    SGBaseLayer::setIsCanTouch(isTouch);
}
void SGStrengLayer::setIsLock(bool isLock)
{
    this->isLock = isLock;
}

void SGStrengLayer::updateposition(cocos2d::CCNode *node)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    node->setPosition(ccp(s.width, node->getPosition().y));
    fermEffect->setVisible(true);
    fermEffect->play();
}
void SGStrengLayer::stopjiantouAction()
{
    fermEffect->stop();
    fermEffect->setVisible(false);
}
void SGStrengLayer::cardBlink(cocos2d::CCNode *node)
{
    //素材卡闪烁
    CCSpriterX *texiao = CCSpriterX::create("animationFile/qhqr.scml", true, true);
    texiao->setanimaID(0);
    addChild(texiao, 105);
    texiao->setisloop(false);
    texiao->setPosition(node->getPosition());
    texiao->play();
    float dela = texiao->getAnimaTimeLength(0, 0);
    node->runAction(CCSequence::create(
                                       CCDelayTime::create((float)dela/4),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::nextcardBlindcall)),
                                       CCDelayTime::create((float)dela/4),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::cardBlinkMidcall)),
                                       CCDelayTime::create((float)dela/4),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::cardBlinkcall)),NULL));
}
void SGStrengLayer::cleanAnotherball()
{
    for (int i = 0; i<_arraySelectCard->count()-1; i++)
    {
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
        this->removeChildByTag(5000+card->getselectposition(), true);
        CCLOG("e23234234 %d",5000 + card->getselectposition());
    }
    CCScaleTo *scale  = CCScaleTo::create(0.1, 0.2);
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(_arraySelectCard->count()-1);
    CCSpriterX *ball = (CCSpriterX *)this->getChildByTag(5000+card->getselectposition());
    if (ball)
    {
        ball->setScale(1.23);
        ball->runAction(CCSequence::create(CCDelayTime::create(0.3),CCMoveBy::create(0.1, ccp(0, -30)),CCMoveTo::create(0.2, Icon->getPosition()),scale,CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::showblinkframe)),CCDelayTime::create(0.05),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::cleanballandrotate)),NULL));
    }
    else
    {
        SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
    }

}
void SGStrengLayer::showblinkframe()
{
    CCSprite *spriteBig = CCSprite::createWithSpriteFrameName("qhqr_7.png");
    this->addChild(spriteBig,105,789);
    spriteBig->setPosition(Icon->getPosition());
    
}
void SGStrengLayer::sendrequest()
{
    if (cardType != 4)
    {
        CCString *str_nowcard = CCString::create(_card->getClassName());
        if (onlyOfficer)
        {
           
            main::LordSkillCompositeRequest *request = new main::LordSkillCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                request->add_propsid(baseMilitaryCard->getSsid());
            }
            SGSocketClient::sharedSocketClient()->send(MSG_STRENG_LORD, request);

            
        }
        else if (str_nowcard->isEqual(CCString::create("SGOfficerCard")))
        {
            main::OfficerCompositeRequest *request = new main::OfficerCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
                
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    request->add_propsid(baseMilitaryCard->getSsid());}
                
                else if(str_->isEqual(CCString::create("SGOfficerCard")))
                {
                    request->add_materialid(baseMilitaryCard->getSsid());
                }
            }
            SGSocketClient::sharedSocketClient()->send(MSG_BARRACKS_STRENG, request);
            
        }
        else if (str_nowcard->isEqual(CCString::create("SGEquipCard")))
        {
            main::EquipmentCompositeRequest *request = new main::EquipmentCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    request->add_propsid(baseMilitaryCard->getSsid());}
                
                else if(str_->isEqual(CCString::create("SGEquipCard")))
                {
                    request->add_materialid(baseMilitaryCard->getSsid());
                }
            }
            SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_STRENG, request);
        }
        else
        {
            
            main::SoldierCompositeRequest *request = new main::SoldierCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                request->add_propsid(baseMilitaryCard->getSsid());
            }
            SGSocketClient::sharedSocketClient()->send(MSG_SOLDIER_STRENG, request);
        }
    }
    else
    {
        main::SkillCompositeRequest *request = new main::SkillCompositeRequest();
        request->set_ssid(_card->getSsid());
        for (int i = 0; i < _arraySelectCard->count(); i++)
        {
            SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
            request->add_propsid(baseMilitaryCard->getSsid());
        }
        SGSocketClient::sharedSocketClient()->send(MSG_STRENG_SKILL, request);
    }
	
	_arraySelectCard->removeAllObjects();//11.28
    addFlags = 0;
    
}
void SGStrengLayer::cleanballandrotate(cocos2d::CCNode *node)
{
    this->removeChild(node, true);
    this->removeChildByTag(789, true);
    CCSpriterX *texiao2 = CCSpriterX::create("animationFile/qhqr_001.scml", true, true);
    texiao2->setanimaID(0);
    texiao2->setisloop(false);
    addChild(texiao2, 10);
    texiao2->setPosition(Icon->getPosition());
    texiao2->play();
    this->runAction(CCSequence::create(CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::sendrequest)),NULL));
}
void SGStrengLayer::nextcardBlindcall(cocos2d::CCNode *node)
{
    selectcardcount -- ;
    CCLOG("wegdasdf");
    if (selectcardcount >= 0){
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(selectcardcount);
        SGButton *kuang = (SGButton*)menu->getChildByTag(card->getselectposition());//sprite
        
        cardBlink(kuang);
    }
}
void SGStrengLayer::cardBlinkcall(cocos2d::CCNode *node)
{
    //光球
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSpriterX *texiao1 = CCSpriterX::create("animationFile/qhqr_000.scml", true, true);
    texiao1->setanimaID(0);
    texiao1->setisloop(true);
    addChild(texiao1,106,5000+node->getTag());
    CCLOG("texiao tag is %d",5000+node->getTag());
    texiao1->setPosition(node->getPosition());
    texiao1->play();
    texiao1->runAction(CCMoveTo::create(0.5, ccp(s.width/2-s.width*.377-3, node->getPosition().y)));
    
    if (selectcardcount == 0)
    {
        runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::cleanAnotherball)),NULL));
    }
    if (selectcardcount == 1 || _arraySelectCard->count() == 2)
    {
        runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::cleanAnotherball)),NULL));
    }
    if (selectcardcount == -1 || _arraySelectCard->count() == 1)
    {        
        runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFuncN::create(this, callfuncN_selector(SGStrengLayer::cleanAnotherball)),NULL));
    }
}
void SGStrengLayer::cardBlinkMidcall(CCNode *node)
{
    this->getChildByTag(node->getTag() +100)->setVisible(true);
//    node->removeAllChildrenWithCleanup(true);//改成button之后,去掉上面的人物头像\框等是110,111,112三个,不能全去掉
    node->removeChildByTag(110);
    node->removeChildByTag(111);
    node->removeChildByTag(112);
}
#pragma mark - Tabel_Item Classr

Tabel_Item::Tabel_Item() : item(NULL), spritePlayerHead(NULL), spriteIsScelected(NULL), labelName(NULL), labelLevel(NULL), arrayStar(NULL), sprite(NULL), bluestar(NULL), _card(NULL)
{
    ;
}

Tabel_Item::~Tabel_Item()
{
    arrayStar->release();
    bluestar->release();
}

void Tabel_Item::setView(int num)
{
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    SGMenu *menu1 = SGMenu::create(CCRectMake(0, 50, s.width, s.height - 180 - 284 - 50));
    menu1->setTouchPriority(1);
    menu1->setTag(101);
    menu1->setPosition(CCPointZero);
    this->addChild(menu1,5);
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(menu1);
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(menu1, 1, true);
    arrayStar = CCArray::create();
    arrayStar->retain();
    bluestar = CCArray::create();
    bluestar->retain();

    item =  CCSprite::createWithSpriteFrameName("item_smallbg.png");
    bgbgbg = item;
    this->addChild(item);
	
	//强化士兵的时候使用itemid
	if (strcmp(_card->getClassName(), "SGSoldierCard"))
	{
		if (!strcmp(_card->getClassName(), "SGPropsCard"))
		{
			SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
			SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),sg_strengLayer);
			spritePlayerHead = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(), "jinkuang.png", this, menu_selector(Tabel_Item::showgenerinfo),ccp(0, 0),false,true);
		}
		else
		{
			SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_strengLayer);
			spritePlayerHead = SGButton::create(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString(), "jinkuang.png", this, menu_selector(Tabel_Item::showgenerinfo),ccp(0, 0),false,true);
		}

	}
	else if (!strcmp(_card->getClassName(), "SGSoldierCard"))
	{
		SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_strengLayer);
		spritePlayerHead = SGButton::create(CCString::createWithFormat("head%d.png",_card->getItemId())->getCString(), "jinkuang.png", this, menu_selector(Tabel_Item::showgenerinfo),ccp(0, 0),false,true);
	}

	
    item->setPosition(ccpAdd(item->getPosition(), ccp(spritePlayerHead->getContentSize().width/2, 0)));
    menu1->addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width/2 - spritePlayerHead->getContentSize().width/2, 2)));
    
    CCSprite *background =  CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background,-1);
    background->setPosition(spritePlayerHead->getPosition());
    
    labelName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_BOXINFO, 24);
    
    labelName->setAnchorPoint(ccp(0, 0.5));
    labelName->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6, labelName->getContentSize().height/2+3)));
    this->addChild(labelName);
    
    spriteIsScelected =  CCSprite::createWithSpriteFrameName("baikuang.png");
    this->addChild(spriteIsScelected);
    spriteIsScelected->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.359,0)));
    this->setItemSelect(false);
    
    labelLevel = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
    this->addChild(labelLevel);
    labelLevel->setAnchorPoint(ccp(0, 0.5));
    labelLevel->setPosition(ccpAdd(labelName->getPosition(), ccp(labelName->getContentSize().width + 10, 0)));
    if (_card->getCurrLevel())
    {
        labelLevel->setString(CCString::createWithFormat("%d级",_card->getCurrLevel())->getCString());
    }
    
    CCString *str_ = CCString::create(_card->getClassName());
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1)
        {
            starlvl-=1;
        }
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
        if (spritePlayerHead)
        {
            spritePlayerHead->removeChildByTag(30, true);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spritePlayerHead->getContentSize().width*0.3-2,spritePlayerHead->getContentSize().height - countryName->getContentSize().height/4 +6));
            spritePlayerHead->addChild(countryName,30,30);
            
            spritePlayerHead->removeChildByTag(250, true);
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24 , ccGREEN);
                advancenumber->setAnchorPoint(ccp(1, 1));
                advancenumber->setPosition( spritePlayerHead->getContentSize()/1.2);
                spritePlayerHead->addChild(advancenumber,50,250);
            }

            
        }
    }
    else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
        spritePlayerHead->removeChildByTag(250, true);
        if (temp->getAdvNum()>0)
        {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->setPosition(spritePlayerHead->getContentSize()/1.2);
            spritePlayerHead->addChild(advancenumber,50,250);
        }
    }
    else
    {
        SGPropsCard *card = (SGPropsCard *)_card;
        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
        int starlvl = temp->getPropsCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        
        if (card->getType() == 1 || card->getType() == 4 ||card->getType() == 5)
        {
            

            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
            if (spritePlayerHead)
            {
                spritePlayerHead->removeChildByTag(30, true);
                spritePlayerHead->removeChildByTag(250, true);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(spritePlayerHead->getContentSize().width*0.3-2,spritePlayerHead->getContentSize().height - countryName->getContentSize().height/4 +6));
                spritePlayerHead->addChild(countryName,30,30);
                
            }

        }
        else
        {
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName((CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl))->getCString())->displayFrame());
        }
    }
    spritePlayerHead->setTag(11);
    //暂定强化时不让其查看物品详情。
    spritePlayerHead->setEnabled(false);
    this->showPlayerStar();
}

//设置星星数量
void Tabel_Item::showPlayerStar()
{
    for(int i=0;i<arrayStar->count();i++)
    {
        CCSprite *sprite = (CCSprite*)arrayStar->objectAtIndex(i);
        sprite->removeFromParentAndCleanup(true);
    }
//    for(int i=0;i<bluestar->count();i++)
//    {
//        CCSprite *sprite = (CCSprite*)bluestar->objectAtIndex(i);
//        sprite->removeFromParentAndCleanup(true);
//    }
    
    int starCount = _card->getCurrStar();
//    for(int i=0; i < 6;i++)
    for(int i=0; i < 3;i++)
    {
        CCSprite *sprite =  CCSprite::createWithSpriteFrameName("xx.png");
        sprite->setAnchorPoint(ccp(0.5, 1));
        sprite->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width+sprite->getContentSize().width*i-17,4)));
        arrayStar->addObject(sprite);
        this->addChild(sprite);
        if (i >= starCount) {
            sprite->setVisible(false);
        }
    }
//    if(strcmp(_card->getClassName(), "SGOfficerCard") || strcmp(_card->getClassName(), "SGEquipCard"))
//    {
//        for(int i=starCount; i < _card->getUpgradestar();i++)
//        {
//            CCSprite *sprite = CCSprite::createWithSpriteFrameName("bluestar.png");
//            sprite->setAnchorPoint(ccp(0.5, 1));
//            sprite->setPosition(ccpAdd(((CCSprite *)arrayStar->objectAtIndex(i))->getPosition(), ccp(0,-3.5)));
//            bluestar->addObject(sprite);
//            this->addChild(sprite);
//        }
//    }

}

Tabel_Item* Tabel_Item::create(SGBaseMilitaryCard *card,int num)
{
	Tabel_Item *instance = new Tabel_Item();
    instance->_card = card;
	if (instance->init())
    {
        instance->setView(num);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}
void Tabel_Item::setIsCanSelect(bool isCanSelecdt)
{
    item->setColor(ccWHITE);
    if (!isCanSelecdt)
    {
        item->setColor(ccc3(80, 80, 80));
    }
}
//设置选中或非选中
void Tabel_Item::setItemSelect(bool isSelect)
{
    if(isSelect)
    {
        CCSprite *spriteSelectOK =  CCSprite::createWithSpriteFrameName("streng_choose.png");
        spriteIsScelected->setDisplayFrame(spriteSelectOK->displayFrame());
    }
    else
    {
        CCSprite *spriteSelectNO =  CCSprite::createWithSpriteFrameName("baikuang.png");
        spriteIsScelected->setDisplayFrame(spriteSelectNO->displayFrame());
    }
}

//刷新
void Tabel_Item::updateOfficerCard(SGBaseMilitaryCard *card,int index, int isSelect)
{
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer);
    _card = card;
    labelName->setString(card->getOfficerName()->getCString());
    CCString *str_ = CCString::create(_card->getClassName());
    SGMainManager::shareMain()->addHeadIconPlistByNum(card->getHeadSuffixNum(), sg_strengLayer);
    spritePlayerHead->setImage(CCString::createWithFormat("head%d.png",card->getHeadSuffixNum())->getCString());
    
    labelLevel->setString("");
    if (card->getCurrLevel())
    {
        labelLevel->setString(CCString::createWithFormat("%d级",card->getCurrLevel())->getCString());
        labelLevel->setPosition(ccpAdd(labelName->getPosition(), ccp(labelName->getContentSize().width + 10, 0)));
    }
    
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1)
        {
            starlvl-=1;
        }
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());

        if (spritePlayerHead)
        {
            spritePlayerHead->removeChildByTag(30, true);     
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spritePlayerHead->getContentSize().width*0.3-2,spritePlayerHead->getContentSize().height - countryName->getContentSize().height/4+6));
            spritePlayerHead->addChild(countryName,30,30);
            
            spritePlayerHead->removeChildByTag(250, true);
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->setPosition(spritePlayerHead->getContentSize()/1.2);
                spritePlayerHead->addChild(advancenumber,50,250);
            }
        }
    }
    else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
        
        spritePlayerHead->removeChildByTag(250, true);
        if (temp->getAdvNum()>0) {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->setPosition(spritePlayerHead->getContentSize()/1.2);
            spritePlayerHead->addChild(advancenumber,50,250);
        }
    }
    else
    {
        SGPropsCard *card = (SGPropsCard *)_card;
        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
        int starlvl = temp->getPropsCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        if (card->getType() == 1 ||card->getType() == 4 ||card->getType() == 5)
        {
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",temp->getPropsCountry())->getCString())->displayFrame());
            
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
            
            if (spritePlayerHead)
            {
                spritePlayerHead->removeChildByTag(30, true);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(spritePlayerHead->getContentSize().width*0.3-2,spritePlayerHead->getContentSize().height - countryName->getContentSize().height/4+6));
                spritePlayerHead->addChild(countryName,30,30);
                spritePlayerHead->removeChildByTag(250, true);
            }

        }

        else
        {
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
            spritePlayerHead->removeChildByTag(250, true);
        }
    }
    this->setItemSelect(isSelect != 0);
    showPlayerStar();
}
void Tabel_Item::showgenerinfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCString *str_ = CCString::create(_card->getClassName());
    if (str_->isEqual(CCString::create("SGPropsCard")))
    {
        if (((SGPropsCard *)_card)->getType() ==4 || ((SGPropsCard *)_card)->getType() ==5)
        {
            SGMainManager::shareMain()->showSkillCardInfoLayer((SGPropsCard *)_card, 1,false);
        }
        else
        {
            SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)_card, 5,false);
        }
    }
    else if(str_->isEqual(CCString::create("SGOfficerCard")) )
    {
        SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create((SGOfficerCard *)_card, 7);
        SGMainManager::shareMain()->showLayer(Layer,false);
        
    }
    else if(str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 3,false);
    }
}
void SGStrengLayer::labelcallback()
{
    lab_lvl->setop(0);
    runAction(lab_orilvl->fadin(1));
}
void SGStrengLayer::labelcallback1()
{
    runAction(lab_orilvl->fadout(1));
    // runAction(CCSequence::create((CCActionInstant*)lab_orilvl->fadout(1),CCDelayTime::create(3),(CCActionInstant*)lab_orilvl->fadin(1),CCDelayTime::create(3),NULL));
}
void SGStrengLayer::visitHandle()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
    if (onlyOfficer )
    {
        if (cardType == 1)
        {
            main::LotteryEntryRequest *request = new main::LotteryEntryRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_ENTER, request);
        }
        else
        {
            CCLOG("test visit");
        }
    }

}


//检查与阵上同名的武将
bool SGStrengLayer::strengHaveSameNameOfficer(int itemId)
{
	SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
	int protoId = officer->getOfficerProtoId();
	
    CCObject *officerCardObj = NULL;
    if (_arraySelectCard)
    {
        CCARRAY_FOREACH(_arraySelectCard, officerCardObj)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)officerCardObj;
            
            if (card->getItemType() == 1) {
                int onEmbattleProtoId = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId())->getOfficerProtoId();
                if (onEmbattleProtoId == protoId)
                {
                    return true;
                }
            }

        }
    }
    
	return false;
}


SGMergeMaterialBox::SGMergeMaterialBox()
:strengType(-1)
,card(NULL)
{
}
SGMergeMaterialBox::~SGMergeMaterialBox()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MERGE_ITEM);
}
SGMergeMaterialBox *SGMergeMaterialBox::create(SGBoxDelegate *target, int strengType, SGBaseMilitaryCard *card)
{
    SGMergeMaterialBox *mergeBox = new SGMergeMaterialBox();
    if (mergeBox->init(target, box_strengMergeBox, CCSizeMake(535,350)))
    {
        mergeBox->autorelease();
        mergeBox->strengType = strengType;
        mergeBox->card = card;
        mergeBox->initView();
        return mergeBox;
    }
    CC_SAFE_DELETE(mergeBox);
    return NULL;
}
void SGMergeMaterialBox::initView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MERGE_ITEM,
                              this,
                              callfuncO_selector(SGMergeMaterialBox::mergerItemListener));
    SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png",
                                          this, menu_selector(SGMergeMaterialBox::btnYesHandler),CCPointZero,false,true);
    okButton->setTag(1);
    SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png",
                                              this, menu_selector(SGMergeMaterialBox::btnNoHandler),CCPointZero,false,true);
    cancelButton->setTag(2);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);

    SGCCLabelTTF *title = SGCCLabelTTF::create(str_MergeItemTitle, FONT_PANGWA, 33 , ccWHITE , ccc3(143, 87, 0));
    this->addChild(title);
    title->setPosition(ccpAdd(center, ccp(0, 120)));
    
    const char *info = str_MergeItemoffTips;
    if (strengType == 3)
    {
        info = str_MergeItemsolTips;
    }
    
    
    SGCCLabelTTF *label = SGCCLabelTTF::create(info, FONT_PANGWA, 26, CCSizeMake(32*13, 200),
                                               kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    label->setAnchorPoint(ccp(.5, 1));
    label->setPosition(ccpAdd(biao->getPosition(), ccp(0, -75)));
    this->addChild(label);
    this->addBtn(okButton);
    this->addBtn(cancelButton);
    okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+40)));

}

void SGMergeMaterialBox::mergerItemListener(CCObject *sender)
{
    //receive msg
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    
    if (sr)
    {

        main::MergeItemResponse *mir = static_cast<main::MergeItemResponse *>(sr->m_msg);
        if (mir->state() == 0)
        {
            SGMainManager::shareMain()->showMessage(str_MergeItemMsg_Failed);
        }
        else if (mir->state() == 1)
        {
            SGMainManager::shareMain()->showStrengLayer(card, strengType);
            if (mir->type() == main::OFFICER)
            {
                SGMainManager::shareMain()->showMessage(str_MergeItemMsg_offSuccess);
            }
            else if (mir->type() == main::SOLDIER)
            {
                SGMainManager::shareMain()->showMessage(str_MergeItemMsg_solSuccess);
            }
        }
        else if (mir->state() == 2)
        {
            if (mir->type() == main::OFFICER)
            {
                SGMainManager::shareMain()->showMessage(str_MergeItemMsg_offNone);
            }
            else if (mir->type() == main::SOLDIER)
            {
                SGMainManager::shareMain()->showMessage(str_MergeItemMsg_solNone);
            }        }
    }
    this->boxClose();

}

void SGMergeMaterialBox::btnYesHandler(SGButton *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    //开始向服务器发送消息
    main::MergeItemRequest *mir = new main::MergeItemRequest();

    if (strengType == 1)//武将卡
    {
        mir->set_type(main::OFFICER);
    }
    else if (strengType == 3)//士兵卡
    {
        mir->set_type(main::SOLDIER);
    }
    SGSocketClient::sharedSocketClient()->send(MSG_MERGE_ITEM, mir);
}
void SGMergeMaterialBox::boxClose()
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
    SGBaseBox::boxClose();
}
//取消，对守荆州有二次确认。
void SGMergeMaterialBox::btnNoHandler(SGButton *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}













