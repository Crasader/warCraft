//
//  SGAdvanceLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGAdvanceLayer.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGAdvanceBox.h"
#include "cocos-ext.h"
#include "SGCantAdvanceBox.h"
#include "SGStaticDataManager.h"
#include "SGPropInfoLayer.h"
#include "SGGeneralInfoLayer.h"
#include "SGEquipInfoLayer.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "Itemmodule.pb.h"
#include "SGGuideManager.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "AppMacros.h"
#include "SGLootWiKiMainLayer.h"
#include "SGAdvancedEffectLayer.h"

SGAdvanceLayer::SGAdvanceLayer()
:_card(NULL)
,_arry(NULL),
reqCards(NULL),
datas(NULL),
isfromfirst(false),
officer(NULL),
afterCard(NULL),
equip(NULL),
dict(NULL),
haveCount(NULL),
cardType(0),
beforeitemId(0),
beforestar(0),
info(NULL)
, m_progressTimer(NULL)
, m_pieceHasNum(NULL)
, m_pieceNeedNum(NULL)
, m_sourceBtn(NULL)
, m_canAdcance(false)
//isenough(false)
, m_btnCanTouch(true)
, m_isMaxAdv(false)
{
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true , false);
    for (int i=0;i<6;i++) {
        stars[i]=0;
    }

}
SGAdvanceLayer::~SGAdvanceLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_advanceLayer);
//    datas->release();
    CCLOG("~SGAdvanceLayer");
    afterCard->release();
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_BARRACKS_ADVANCE);
//    notification->removeObserver(this, MSG_EQUIP_ADVANCE);
    if (_arry) {
        _arry->release();
        _arry = NULL;
    }
}

SGAdvanceLayer * SGAdvanceLayer::create(SGBaseMilitaryCard *card,int type,int isfirst,CCArray *arry, ERI *info)
{
    SGAdvanceLayer *advanceLayer = new SGAdvanceLayer();
    if (advanceLayer->init(NULL, sg_advanceLayer))
    {
		advanceLayer->info = info;//返回布阵界面所需要的信息
        advanceLayer->_arry = arry;
        if (advanceLayer->_arry != NULL) {
            advanceLayer->_arry->retain();
            }
        advanceLayer->isfromfirst = isfirst;
        advanceLayer->cardType = type;
        advanceLayer->_card = card;
        advanceLayer->initView();
        advanceLayer->autorelease();
        return advanceLayer;
    }
    CC_SAFE_DELETE(advanceLayer);
    return NULL;
}
void SGAdvanceLayer::initView()
{
  
	
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    
    //此页面不需要手型动画了。。。2014-11-28
    //ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    beforeitemId = _card->getItemId();
    beforestar = _card->getCurrStar();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BARRACKS_ADVANCE,
                              this,
                              callfuncO_selector(SGAdvanceLayer::advanceOfficerListener));
//    notification->addObserver(MSG_EQUIP_ADVANCE,
//                              this,
//                              callfuncO_selector(SGAdvanceLayer::advanceEquipListener));

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCObject *obj = NULL;
    dict = CCDictionary::create();
    CCString *str_kuang = CCString::create("");
    CCString *str_ = CCString::create(_card->getClassName());
    
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        str_kuang = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);

//        str_kuang = CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType());
    }
    else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
    }
    if (cardType ==1)
    {
        officer = (SGOfficerCard *)_card;
        
        int cardMaxStar = _card->getMaxStar();
        
        SGOfficerDataModel *oCard = SGStaticDataManager::shareStatic()->getOfficerById(cardMaxStar);
        SGOfficerCard *officerCard = new SGOfficerCard;
        officerCard->setOfficerName(oCard->getOfficerName());
        officerCard->setMaxLevel(oCard->getOfficerMaxLevel());
        officerCard->setCurrExp(_card->getCurrExp());//转生
        officerCard->setCurrStar(oCard->getOfficerCurrStarLevel());
        officerCard->setIsProt(_card->getIsProt());//转生
        officerCard->setCurrLevel(_card->getCurrLevel());//转生加
//        officerCard->setIsExped(0);
        officerCard->setRace(oCard->getOfficerRace());
        officerCard->setItemId(oCard->getOfficerId());
        //转生最大星级
        officerCard->setUpgradestar(oCard->getUpgradestar());
        //性别和缘分
        officerCard->setGender((OfficerGender) oCard->getOfficerGender());
        officerCard->setFateList(oCard->getOfficerFateList());
        //武将原型ID
        officerCard->setProtoId(oCard->getOfficerProtoId());
        //速度修正系数
        officerCard->setSpeedFactor(oCard->getOfficerSpeedFactor());
        //转生等级
        officerCard->setUpgradelevel(oCard->getUpgradelevel());
        //武将计最大等级
        officerCard->setSkillMaxLevel(oCard->getSkillMaxLevel());
        //主将计最大等级
        officerCard->setLordMaxLevel(oCard->getLordMaxLevel());
        //转生次数
        officerCard->setAdNum(oCard->getAdvNum());
        if (oCard->getOfficerGeneralsSkill()&&!oCard->getOfficerGeneralsSkill()->isEqual(CCString::create(""))) {
//            CCLOG("%s",oCard->getOfficerGeneralsSkill());

            officerCard->setOfficerSkil(SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(oCard->getOfficerGeneralsSkill()->getCString()));
        }
        
        if (oCard->getOfficerLordSkill() &&!oCard->getOfficerLordSkill()->isEqual(CCString::create(""))) {
            
            int lordskill = SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(oCard->getOfficerLordSkill()->getCString());
            //printf("officer lord skill is %d",lordskill);

            officerCard->setLordSkill(lordskill);

        }        
        officerCard->setAtk(oCard->getOfficerBaseAtk());
        officerCard->setDef(oCard->getOfficerBaseDef());
        officerCard->setRound(oCard->getOfficerRound());
        officerCard->setSpeed(oCard->getOfficerBaseSpeed());
        officerCard->setMorale(oCard->getOfficerMorale());
        officerCard->setGoverning(oCard->getOfficerGoverning());
		officerCard->setHeadSuffixNum(oCard->getIconId());//获取对应的head头像12.05
        
        officerCard->autorelease();
        afterCard = officerCard;
        afterCard->retain();
        
        CCArray *array =CCArray::create();
        array->retain();
        array =SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
        for(int i=0;i<array->count();i++)//统计素材每个星级的数量
        {
            SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)array->objectAtIndex(i);
            //        CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
            
            //        if (str_->isEqual(CCString::create("SGOfficerCard")))
            //        {
            SGOfficerCard *card = (SGOfficerCard *)baseMilitaryCard;
            int po1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,card);
            int po2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,card);
            int po3 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,card);
            if ( po1 == 0 && po2 ==0 && po3 == 0 && card->getIsProt() == 0)
            {
                int s = card->getCurrStar()-1;
                stars[s]+=1;
            }
            //        }
        }



    }else if(cardType == 2)
    {
        equip = (SGEquipCard *)_card;
        CCArray *reqItemIDs = equip->getRequitemIds();
        
        CCARRAY_FOREACH(reqItemIDs, obj)
        {
            CCString *itemid = (CCString *)obj;
            CCLOG("柴料id:%d",itemid->intValue());
            CCString *str = (CCString *)dict->objectForKey(itemid->getCString());
            if (str)
            {
                int value = str->intValue();
                value ++;
                str = CCString::createWithFormat("%d",value);
            }
            else
            {
                str = CCString::createWithFormat("%d",1);
            }
            dict->setObject(str, itemid->getCString());
        }
        SGEquipCard *equipCard = new SGEquipCard;
        SGEquipmentDataModel *eCard = SGStaticDataManager::shareStatic()->getEquipById(_card->getMaxStar());
        
        //原型ID
        equipCard->setProtoId(eCard->getEquipProtoId());
        equipCard->setCurrLevel(_card->getCurrLevel());//转生加
        equipCard->setCurrStar(eCard->getEquipCurrStarLevel());
        //equipCard->setCurrExp(_card->getCurrExp());//转生
        equipCard->setOfficerCardId(0);
        //转生最大星级
        equipCard->setUpgradestar(eCard->getUpgradestar());
        //转生等级
        equipCard->setUpgradelevel(eCard->getUpgradelevel());
        //转生次数
        equipCard->setAdNum(eCard->getAdvNum());
        equipCard->setItemType(eCard->getEquipType());
        equipCard->setIsProt(_card->getIsProt());//转生
        equipCard->setOfficerName(CCString::create(eCard->getEquipName()->getCString()));
        equipCard->setItemId(_card->getMaxStar());
        equipCard->setMorale(eCard->getEquipHP());
        equipCard->setAtk(eCard->getEquipmentBaseAtk());
        equipCard->setDef(eCard->getEquipBaseDef());
        equipCard->setRound(eCard->getEquipRound());
        equipCard->setSpeed(eCard->getEquipBaseSpeed());
        equipCard->setMaxLevel(eCard->getEquipMaxLevel());
        equipCard->setGoverning(eCard->getEquipGoverning());
		
		equipCard->setHeadSuffixNum(eCard->getIconId());//12.06
		
        equipCard->autorelease();
        afterCard = equipCard;
        afterCard->retain();
        
        CCArray *equips = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
//        CCObject *obj = NULL;
        
        for(int i=0;i<equips->count();i++)//统计素材每个星级的数量
        {
            SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)equips->objectAtIndex(i);
            //        CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
            
            //        if (str_->isEqual(CCString::create("SGEquipCard")))
            //        {
            SGEquipCard *card = (SGEquipCard *)baseMilitaryCard;
            
            if (card->getOfficerCardId() == 0 &&card->getIsProt() == 0)
            {
                int s = card->getCurrStar()-1;
                stars[s]+=1;
            }
            //        }
        }

    }
    
  
    CCSprite* title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);

  //  CCSprite *font_title = CCSprite::create("barrack/barrack_font_kpzssx.png");
//    CCSprite *font_title = CCSprite::createWithSpriteFrameName("barrack_font_kpzssx.png");
    SGCCLabelTTF * font_title = SGCCLabelTTF::create( str_officer_zhuan , FONT_PANGWA, 32 ,COLOR_YELLOW );
    font_title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(font_title);
    
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
    
    
    CCSprite *lanbg = CCSprite::createWithSpriteFrameName("advance_bg_lan.png");
    
    lanbg->setPosition(ccpAdd(center, ccp(0,250)));
    
//    CCSprite *midbg = CCSprite::createWithSpriteFrameName("advance_bg_mid.png");
////    this->addChild(midbg);
//    midbg->setPosition(ccpAdd(center, ccp(0, 15)));
  

    CCSprite *redbg = CCSprite::createWithSpriteFrameName("advance_bg_hong.png");
    redbg->setFlipX(true);
    this->addChild(redbg);
    redbg->setPosition(ccpAdd(center, ccp(-6, -40)));
    
    CCSprite *jiantou = CCSprite::createWithSpriteFrameName("advance_jiantou.png");
    jiantou->setPosition(ccpAdd(lanbg->getPosition(), ccp(0,-lanbg->getContentSize().height*.35)));
    this->addChild(jiantou);
    
    this->addChild(lanbg);
    


    

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");

    CCRect re = CCRectMake(0, 30, bg->getContentSize().width, s.height - 112 - title_bg->getContentSize().height);
    bg->setTextureRect(re);
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        bg->setScaleY(1.08);
    }
    else
    {
        bg->setScaleY(1.09);
    }
    bg->cocos2d::CCNode::setScaleX(s.width/bg->getContentSize().width * 1.02);
    
    this->addChild(bg,title_bg->getZOrder() -1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-5, -30 + 133));
    
    char spriteName[256] = {0};
	
	//12.05
	SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_advanceLayer);
    sprintf(spriteName, "head%d.png",_card->getHeadSuffixNum());

    SGButton *current = SGButton::create(spriteName,str_kuang->getCString(), this,
                                    menu_selector(SGAdvanceLayer::showCurrInfo),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(current);
    current->setPosition(ccpAdd(lanbg->getPosition(), ccp(-lanbg->getContentSize().width*.3, 0)));
    
//    SGCCLabelTTF *currlvl = SGCCLabelTTF::create(CCString::createWithFormat("%d",_card->getCurrLevel())->getCString(), FONT_BOXINFO, 22);
//    this->addChild(currlvl);
//    SGCCLabelTTF *font_lvl = SGCCLabelTTF::create(str_Class, FONT_BOXINFO, 22);
//    this->addChild(font_lvl);
//    
//    SGCCLabelTTF *currmaxlvl = NULL;
//    
//    
//    currmaxlvl = SGCCLabelTTF::create(CCString::createWithFormat("/%d",_card->getMaxLevel())->getCString(), FONT_BOXINFO, 22);
//    this->addChild(currmaxlvl);
//    font_lvl->setAnchorPoint(ccp(0, 0.5));
//    currlvl->setAnchorPoint(ccp(0, 0.5));
//    currmaxlvl->setAnchorPoint(ccp(0, 0.5));
//    font_lvl->setPosition(ccpAdd(current->getPosition(), ccp(-(font_lvl->getContentSize().width + currlvl->getContentSize().width + currmaxlvl->getContentSize().width)/2, -current->getContentSize().height*.75)));
//    if (_card->getCurrLevel()<_card->getUpgradelevel()) {
//        currlvl->setInsideColor(ccRED);
//        
//        SGCCLabelTTF *labelFinal1 = SGCCLabelTTF::create(str_Class_not_enough, FONT_BOXINFO, 22);
//        this->addChild(labelFinal1);
//        labelFinal1->setAnchorPoint(ccp(0, 0.5));
//        labelFinal1->setInsideColor(ccRED);
//        labelFinal1->setPosition(ccp(font_lvl->getPosition().x ,font_lvl->getPosition().y-30));
//    }
//    currlvl->setPosition(ccpAdd(font_lvl->getPosition(), ccp(font_lvl->getContentSize().width, 0)));
//    currmaxlvl->setPosition(ccpAdd(currlvl->getPosition(), ccp(currlvl->getContentSize().width, 0)));

    
    CCSprite *jinjieq = CCSprite::createWithSpriteFrameName("font_jinjieq.png");
    jinjieq->setAnchorPoint(ccp(0, 1));
    this->addChild(jinjieq);
    jinjieq->setPosition(ccpAdd(lanbg->getPosition(), ccp(-lanbg->getContentSize().width*.48,lanbg->getContentSize().height*.46)));
    
    CCSprite *jinjieh = CCSprite::createWithSpriteFrameName("font_jinjieh.png");
    jinjieh->setAnchorPoint(ccp(0, 1));
    this->addChild(jinjieh);
    jinjieh->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.46,redbg->getContentSize().height*.46)));

    
    //前置文本字号
    int nameFontSize = 26;
    int nameOffy = 7;
    int advOffy = 0;
    
    ccColor3B color[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
    ccColor3B labelColor = ccc3(0,0,0);
    
    int starLev = _card->getCurrStar();
    if(starLev<3)
        starLev = 3;
    labelColor = color[starLev-3];
    
    SGCCLabelTTF *name1 = SGCCLabelTTF::create((_card->getOfficerName())->getCString(), FONT_PANGWA, nameFontSize , labelColor);
    this->addChild(name1);
    name1->setAnchorPoint(ccp(0,1));
    name1->setPosition(ccpAdd(jinjieq->getPosition(), ccp(jinjieq->getContentSize().width + 21,nameOffy)));
    
    //小转生次数
    if(_card->getAdNum()>0)
    {
        SGCCLabelTTF * advance1 = SGCCLabelTTF::create(CCString::createWithFormat("+%d" , _card->getAdNum())->getCString(), FONT_PANGWA, nameFontSize , ccGREEN);
        advance1->setAnchorPoint(ccp(0, 1));
        advance1->setPosition(ccpAdd(name1->getPosition(), ccp(name1->getContentSize().width + 6, advOffy)));
        this->addChild(advance1);
    }
    
    starLev = _card->getCurrStar();
    if(starLev < 3)
        starLev = 3;
    labelColor = color[starLev-3];
    SGCCLabelTTF *name2 = SGCCLabelTTF::create((afterCard->getOfficerName())->getCString(), FONT_PANGWA, nameFontSize , labelColor);
    this->addChild(name2);
    name2->setAnchorPoint(ccp(0,1));
    name2->setPosition(ccpAdd(jinjieh->getPosition(), ccp(jinjieh->getContentSize().width + 21, nameOffy)));
    
    //小转生次数
    if(afterCard->getAdNum()>0)
    {
        SGCCLabelTTF * advance2 = SGCCLabelTTF::create(CCString::createWithFormat("+%d" ,
                                                                                  afterCard->getAdNum())->getCString(), FONT_PANGWA, nameFontSize , ccGREEN);
        advance2->setAnchorPoint(ccp(0, 1));
        advance2->setPosition(ccpAdd(name2->getPosition(), ccp(name2->getContentSize().width + 6, advOffy)));
        this->addChild(advance2);
    }

    

    char spriteN[256] = {0};
	
    SGMainManager::shareMain()->addHeadIconPlistByNum(afterCard->getHeadSuffixNum(),sg_advanceLayer);//12.05
    sprintf(spriteN, "head%d.png",afterCard->getHeadSuffixNum());
    
    CCString *str2_ = CCString::create(afterCard->getClassName());
    
    if (str2_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(afterCard->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        str_kuang = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
        
        //        str_kuang = CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType());
    }
    else if (str2_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(afterCard->getItemId());
        str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
    }

    
    SGButton *advance = SGButton::create(spriteN,str_kuang->getCString(), this,
                                         menu_selector(SGAdvanceLayer::showAfterInfo),
                                         ccp(0, 0),
                                         false,
                                         true);
    
//    if (_card->getAdNum()>0) {
//        current->removeChildByTag(250);
//        SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",_card->getAdNum())->getCString(), FONT_PANGWA, 24);
//        advancenumber->setInsideColor(ccGREEN);
//        advancenumber->setAnchorPoint(ccp(1,1));
//        advancenumber->ignoreAnchorPointForPosition(false);
//        advancenumber->setPosition(ccp(current->getContentSize().width*1.16,current->getContentSize().height*1.14));
//        current->addChild(advancenumber,50,250);
//    }
//    if (afterCard->getAdNum()>0) {
//        advance->removeChildByTag(250);
//        SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",afterCard->getAdNum())->getCString(), FONT_PANGWA, 24);
//        advancenumber->setInsideColor(ccGREEN);
//        advancenumber->setAnchorPoint(ccp(1,1));
//        advancenumber->ignoreAnchorPointForPosition(false);
//        advancenumber->setPosition(ccp(advance->getContentSize().width*1.16,advance->getContentSize().height*1.14));
//        advance->addChild(advancenumber,50,250);
//    }

    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        current->removeChildByTag(30);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",_card->getRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(current->getContentSize().width*0.28,current->getContentSize().height-2));
        current->addChild(countryName,30,30);
        
        advance->removeChildByTag(30);
        countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",_card->getRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(current->getContentSize().width*0.28,current->getContentSize().height-2));
        advance->addChild(countryName,30,30);
        
    }
    
    advance->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.288, 5)));
    this->addBtn(advance);
    advance->setScale(1.2);
    
    
    this->setlab(ccpAdd(lanbg->getPosition(), ccp(-63, 70)), _card);
    this->setafterlab(ccpAdd(redbg->getPosition(), ccp(-63, 70)), afterCard);//ccp(-63, 80)
    
    SGButton* okbtn = SGButton::createFromLocal("an.png", str_zhuansheng, this,
                                                menu_selector(SGAdvanceLayer::confirmHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(okbtn);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int fixY = 0;
    if(winSize.height == 960)
        fixY = 50;
    okbtn->setPosition(ccpAdd(center, ccp(0, -380 + fixY)));
    
    //武将转生阶段引导第七步，高亮转生按钮
    int modelTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_OFFICER_ADVANCED, 7);
    okbtn->setTag(modelTag);
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this,
                                                  menu_selector(SGAdvanceLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    
    
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame1->setPreferredSize(CCSizeMake(500, 100));
    frame1->setPosition(ccpAdd(center, ccp(0,-260 + fixY * 0.75)));
    this->addChild(frame1);
    
    
    //阶段引导，武将转生第五步， 高亮对应进度条背景
    int hilightTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_OFFICER_ADVANCED, 5);
    
    CCScale9Sprite *frame2bg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuangbg.png", CCRect(15, 15, 150, 150));
    frame2bg->setOpacity(180);
    frame2bg->setPreferredSize(frame1->getContentSize());
    frame2bg->setPosition(frame1->getPosition());
    frame2bg->setTag(hilightTag);
    this->addChild(frame2bg , frame1->getZOrder()-1);
    
    CCSprite * pieceIcon = CCSprite::createWithSpriteFrameName("pieceIcon.png");
    pieceIcon->setPosition(ccp( 50 , frame2bg->getContentSize().height * 0.5 ));
    frame2bg->addChild(pieceIcon);
    
    //進度背景
    CCScale9Sprite * rateBg = CCScale9Sprite::createWithSpriteFrameName("jingyantiaobg.png");
    rateBg->setPreferredSize(CCSize(300 , rateBg->getOriginalSize().height));
//    rateBg->setScaleX(300.0/rateBg->getContentSize().width);
    rateBg->setAnchorPoint(ccp(0 , 0.5));
    rateBg->setPosition(ccp(pieceIcon->getPositionX() + pieceIcon->getContentSize().width + 15, pieceIcon->getPositionY()));
    frame2bg->addChild(rateBg);
    
    //此界面只接受 武将转生
    int pieceHasNum = 0 ;
    int pieceNeedNum = 1;
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
    
    int officerItemId = _card->getItemId();
    SGOfficerDataModel * model = SGStaticDataManager::shareStatic()->getOfficerById(officerItemId);
    pieceNeedNum = SGStaticDataManager::shareStatic()->getAdvOfficerNeedPieceNum(model->getOfficerCurrStarLevel(), model->getAdvNum());
    if(pieceNeedNum <= pieceHasNum)
        m_canAdcance = true;
    //進度
    m_progressTimer = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("pieceRate.png"));
    m_progressTimer->setScaleX(300.0/ m_progressTimer->getContentSize().width);
    m_progressTimer->setType(kCCProgressTimerTypeBar);
    m_progressTimer->setMidpoint(ccp(0,0));
    m_progressTimer->setBarChangeRate(ccp(1, 0));
    m_progressTimer->setPercentage(100.0 * pieceHasNum/pieceNeedNum);
    m_progressTimer->setAnchorPoint(rateBg->getAnchorPoint());
    m_progressTimer->setPosition(ccp(rateBg->getPositionX()  , rateBg->getPositionY() ) );
    frame2bg->addChild(m_progressTimer);
    
    ccColor3B hasNumColor = ccWHITE;
    if(pieceHasNum >= pieceNeedNum)
        hasNumColor = ccGREEN;
    m_pieceHasNum = SGCCLabelTTF::create(CCString::createWithFormat("%d" , pieceHasNum)->getCString(), FONT_BOXINFO, 24 , hasNumColor);
    m_pieceHasNum->setPosition(ccp(frame2bg->getContentSize().width*0.5 , frame2bg->getContentSize().height*0.5));
    frame2bg->addChild(m_pieceHasNum);
    
    m_pieceNeedNum = SGCCLabelTTF::create(CCString::createWithFormat("/%d" , pieceNeedNum)->getCString(), FONT_BOXINFO, 24);
    m_pieceNeedNum->setPosition(ccpAdd(m_pieceHasNum->getPosition(), ccp( m_pieceHasNum->getContentSize().width*0.5 + m_pieceNeedNum->getContentSize().width*0.5  , 0) ));
    frame2bg->addChild(m_pieceNeedNum);
    
    m_sourceBtn = SGButton::create("sourceIcon.png", NULL, this,menu_selector(SGAdvanceLayer::sourceBtnClicked) );
    m_sourceBtn->setPosition(ccp(frame2bg->getPositionX() + frame2bg->getContentSize().width*0.5 - 50 , frame2bg->getPositionY() ));
    
    //武将转生第9步，高亮转生前的来源按钮
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_OFFICER_ADVANCED, 9);
    m_sourceBtn->setTag(tag);
    
    this->addBtn(m_sourceBtn);
    
    CCMoveTo *move = CCMoveTo::create(1.0,(ccpAdd(lanbg->getPosition(), ccp(0,-lanbg->getContentSize().height*.5))));
    CCMoveTo *move2=CCMoveTo::create(0.01,(ccpAdd(lanbg->getPosition(), ccp(0,-lanbg->getContentSize().height*.35))));
    CCRepeatForever *r = CCRepeatForever::create(CCSequence::create(move,move2,NULL));
    jiantou->runAction(r);
    CCSequence*action=CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGAdvanceLayer::showcurrlvl)),NULL);
    this->runAction(action);
    
}

void SGAdvanceLayer::buttonselect(CCObject *obj)
{
    CCLOG("abcdefg");
    if (_arry && _arry->count()!=0)
        SGMainManager::shareMain()->showAdvancechooseLayer(_card, cardType,0, cardType-1,isfromfirst,_arry);
    else
        SGMainManager::shareMain()->showAdvancechooseLayer(_card, cardType,0, cardType-1,isfromfirst,NULL);
}

void SGAdvanceLayer::setafterlab(CCPoint pos,SGBaseMilitaryCard *card)
{
    int hgt = 30;
    int wid = 190;
    int wid_ = -30;
    SGCCLabelTTF *atk = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 26);
    this->addChild(atk);
    atk->setAnchorPoint(ccp(0, 0.5));
    atk->setPosition(pos);
    SGCCLabelTTF *def = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 26);
    this->addChild(def);
    def->setAnchorPoint(ccp(0, 0.5));
    def->setPosition(ccpAdd(atk->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *mor = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 26);
    this->addChild(mor);
    mor->setAnchorPoint(ccp(0, 0.5));
    mor->setPosition(ccpAdd(def->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 26);
    this->addChild(round);
    round->setAnchorPoint(ccp(0, 0.5));
    round->setPosition(ccpAdd(mor->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *spd = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 26);
    this->addChild(spd);
    spd->setAnchorPoint(ccp(0, 0.5));
    spd->setPosition(ccpAdd(round->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *gov = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 26);
    this->addChild(gov);
    gov->setAnchorPoint(ccp(0, 0.5));
    gov->setPosition(ccpAdd(spd->getPosition(), ccp(0, -hgt)));
    
    CCPoint anchor = CCPoint(0,0.5);
    CCDictionary *dicc = SGCardAttribute::getValue(afterCard->getCurrLevel(), afterCard->getItemId());
    
    SGCCLabelTTF *atkvalue = SGCCLabelTTF::create(CCString::createWithFormat("(%d)",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26 );
    atkvalue->setAnchorPoint(anchor);
    atkvalue->setPosition(ccpAdd(pos, ccp(wid*0.7 + wid_, 0)));
    this->addChild(atkvalue,1,10);
    ///////////攻击后的黄字
//    SGCCLabelTTF *atkvaluemax = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_the_max,((CCString *)dicc->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26 , COLOR_UNKNOW_TAN);
    SGCCLabelTTF *atkvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    atkvaluemax->setAnchorPoint(anchor);
//    atkvaluemax->setPosition(ccpAdd(pos, ccp(wid*1.5, 0)));
    atkvaluemax->setPosition(ccpAdd(pos, ccp(wid, 0)));
    
    this->addChild(atkvaluemax,1,100);
    ///////////
    SGCCLabelTTF *defvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26 );
    defvalue->setAnchorPoint(anchor);
    defvalue->setPosition(ccpAdd(def->getPosition(), ccp(wid*0.7 + wid_, 0)));
    this->addChild(defvalue,1,11);
    ////////////防御后的黄字
//    SGCCLabelTTF *defvaluemax = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_the_max,((CCString *)dicc->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26 , COLOR_UNKNOW_TAN);
    SGCCLabelTTF *defvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",((CCString *)dicc->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    defvaluemax->setAnchorPoint(anchor);
    defvaluemax->setPosition(ccpAdd(def->getPosition(), ccp(wid, 0)));
    this->addChild(defvaluemax,1,110);
    ///////////
    SGCCLabelTTF *morvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString(),FONT_BOXINFO, 26 );
    morvalue->setAnchorPoint(anchor);
    morvalue->setPosition(ccpAdd(mor->getPosition(), ccp(wid*0.7 + wid_, 0)));
    this->addChild(morvalue,1,12);
    ////////////血量后的黄字
    SGCCLabelTTF *morvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString(),FONT_BOXINFO, 26  , ccGREEN);
    morvaluemax->setAnchorPoint(anchor);
    morvaluemax->setPosition(ccpAdd(mor->getPosition(), ccp(wid, 0)));
    this->addChild(morvaluemax,1,120);
    
    ///////////
    SGCCLabelTTF *roundvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getRound())->getCString(),FONT_BOXINFO, 26 );
    roundvalue->setAnchorPoint(anchor);
    roundvalue->setPosition(ccpAdd(round->getPosition(), ccp(wid*0.7 + wid_, 0)));
    this->addChild(roundvalue,1,13);
    //////////蓄力后的黄字
    SGCCLabelTTF *roundvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",card->getRound())->getCString(),FONT_BOXINFO, 26 ,ccGREEN );
    roundvaluemax->setAnchorPoint(anchor);
    roundvaluemax->setPosition(ccpAdd(round->getPosition(), ccp(wid, 0)));
    this->addChild(roundvaluemax,1,130);
    
    /////////
    SGCCLabelTTF *spdvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString(),FONT_BOXINFO, 26 );
    spdvalue->setAnchorPoint(anchor);
    spdvalue->setPosition(ccpAdd(spd->getPosition(), ccp(wid*0.7 + wid_, 0)));

    this->addChild(spdvalue,1,14);
    ///////////速度后的黄字
    SGCCLabelTTF *spdvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString(),FONT_BOXINFO, 26  , ccGREEN);
    spdvaluemax->setAnchorPoint(anchor);
    spdvaluemax->setPosition(ccpAdd(spd->getPosition(), ccp(wid, 0)));
    this->addChild(spdvaluemax,1,140);
    
    //////////
    SGCCLabelTTF *govvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getGoverning())->getCString(),FONT_BOXINFO, 26 );
    govvalue->setAnchorPoint(anchor);
    govvalue->setPosition(ccpAdd(gov->getPosition(), ccp(wid*0.7+wid_, 0)));
    this->addChild(govvalue,1,15);
    ////////统御力后的黄字
    SGCCLabelTTF *govvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",card->getGoverning())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    govvaluemax->setAnchorPoint(anchor);
    govvaluemax->setPosition(ccpAdd(gov->getPosition(), ccp(wid, 0)));
    this->addChild(govvaluemax,1,150);
    
    
    ////////

}
void SGAdvanceLayer::hidelab()
{
    this->getChildByTag(10)->setVisible(false);
    this->getChildByTag(11)->setVisible(false);
    this->getChildByTag(12)->setVisible(false);
    this->getChildByTag(13)->setVisible(false);
    this->getChildByTag(14)->setVisible(false);
    this->getChildByTag(15)->setVisible(false);
//    this->getChildByTag(16)->setVisible(false);

}
void SGAdvanceLayer::showcurrlvl()
{
    CCDictionary *dicc = SGCardAttribute::getValue(afterCard->getCurrLevel(), afterCard->getItemId());
    
    ((SGCCLabelTTF *)this->getChildByTag(10))->setString(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(11))->setString(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("def"))->intValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(12))->setString(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(13))->setString(CCString::createWithFormat("%d",afterCard->getRound())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(14))->setString(CCString::createWithFormat("%d",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(15))->setString(CCString::createWithFormat("%d",afterCard->getGoverning())->getCString());
    
//    ((SGCCLabelTTF *)this->getChildByTag(16))->setString(CCString::createWithFormat(str_Format_class_d_d,afterCard->getCurrLevel(),afterCard->getMaxLevel())->getCString());

    
    this->getChildByTag(10)->setVisible(true);
    this->getChildByTag(11)->setVisible(true);
    this->getChildByTag(12)->setVisible(true);
    this->getChildByTag(13)->setVisible(true);
    this->getChildByTag(14)->setVisible(true);
    this->getChildByTag(15)->setVisible(true);
//    this->getChildByTag(16)->setVisible(true);

    //////////kanata
    /////显示N维属性们
    showmaxlabels();
    /////////
    
}

//by:zyc  修改 为增加的数值
void SGAdvanceLayer::showmaxlabels()
{
    
    CCDictionary *dicc = SGCardAttribute::getValue(_card->getCurrLevel(), _card->getItemId());
    CCDictionary *diccAfter = SGCardAttribute::getValue(afterCard->getCurrLevel(), afterCard->getItemId());
    
    int add = 0;
    add = ((CCString*)diccAfter->objectForKey("atk") )->intValue() - ((CCString*)dicc->objectForKey("atk"))->intValue();
    ((SGCCLabelTTF *)this->getChildByTag(100))->setString(CCString::createWithFormat("(+%d)" ,add)->getCString() );
    
    add = ((CCString*)diccAfter->objectForKey("def") )->intValue() - ((CCString*)dicc->objectForKey("def"))->intValue();
    ((SGCCLabelTTF *)this->getChildByTag(110))->setString(CCString::createWithFormat("(+%d)" ,add)->getCString() );
    
    add = ((CCString*)diccAfter->objectForKey("mor") )->intValue() - ((CCString*)dicc->objectForKey("mor"))->intValue();
    ((SGCCLabelTTF *)this->getChildByTag(120))->setString(CCString::createWithFormat("(+%d)" ,add)->getCString() );
    
    add = afterCard->getRound() - _card->getRound();
    ((SGCCLabelTTF *)this->getChildByTag(130))->setString(CCString::createWithFormat("(+%d)" ,add)->getCString() );
    
    add = ((CCString*)diccAfter->objectForKey("speed") )->intValue() - ((CCString*)dicc->objectForKey("speed"))->intValue();
    ((SGCCLabelTTF *)this->getChildByTag(140))->setString(CCString::createWithFormat("(+%d)" ,add)->getCString() );
    
    add = afterCard->getGoverning() - _card->getGoverning();
    ((SGCCLabelTTF *)this->getChildByTag(150))->setString(CCString::createWithFormat("(+%d)" ,add)->getCString() );
    
                                                          
}

void SGAdvanceLayer::showmaxlvl()
{
    CCDictionary *dicc = SGCardAttribute::getValue(afterCard->getMaxLevel(), afterCard->getItemId());
    
    ((SGCCLabelTTF *)this->getChildByTag(10))->setString(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(11))->setString(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("def"))->intValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(12))->setString(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(13))->setString(CCString::createWithFormat("%d",afterCard->getRound())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(14))->setString(CCString::createWithFormat("%d",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString());
    
    ((SGCCLabelTTF *)this->getChildByTag(15))->setString(CCString::createWithFormat("%d",afterCard->getGoverning())->getCString());
    
//    ((SGCCLabelTTF *)this->getChildByTag(16))->setString(CCString::createWithFormat(str_Format_class_d_d,afterCard->getMaxLevel(),afterCard->getMaxLevel())->getCString());
    this->getChildByTag(10)->setVisible(true);
    this->getChildByTag(11)->setVisible(true);
    this->getChildByTag(12)->setVisible(true);
    this->getChildByTag(13)->setVisible(true);
    this->getChildByTag(14)->setVisible(true);
    this->getChildByTag(15)->setVisible(true);
//    this->getChildByTag(16)->setVisible(true);
//    if (this->getChildByTag(17)) {
//        this->getChildByTag(17)->setVisible(true);
//    }
}
void SGAdvanceLayer::setlab(cocos2d::CCPoint pos, SGBaseMilitaryCard *card)
{
    int hgt = 30;
//    int wid = 190;
    int wid = 140;
    SGCCLabelTTF *atk = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 26);
    this->addChild(atk);
    atk->setAnchorPoint(ccp(0, 0.5));
    atk->setPosition(pos);
    SGCCLabelTTF *def = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 26);
    this->addChild(def);
    def->setAnchorPoint(ccp(0, 0.5));
    def->setPosition(ccpAdd(atk->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *mor = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 26);
    this->addChild(mor);
    mor->setAnchorPoint(ccp(0, 0.5));
    mor->setPosition(ccpAdd(def->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 26);
    this->addChild(round);
    round->setAnchorPoint(ccp(0, 0.5));
    round->setPosition(ccpAdd(mor->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *spd = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 26);
    this->addChild(spd);
    spd->setAnchorPoint(ccp(0, 0.5));
    spd->setPosition(ccpAdd(round->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *gov = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 26);
    this->addChild(gov);
    gov->setAnchorPoint(ccp(0, 0.5));
    gov->setPosition(ccpAdd(spd->getPosition(), ccp(0, -hgt)));
    
    CCPoint anchor = ccp(0,0.5);
    CCDictionary *dicc = SGCardAttribute::getValue(card->getCurrLevel(), card->getItemId());
    
    SGCCLabelTTF *atkvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26);
//    atkvalue->setAnchorPoint(ccp(1, 0.5));
    atkvalue->setAnchorPoint(anchor);
    atkvalue->setPosition(ccpAdd(atk->getPosition(), ccp(wid, 0)));
    this->addChild(atkvalue);
    
    
    SGCCLabelTTF *defvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26);
    defvalue->setAnchorPoint(anchor);
    defvalue->setPosition(ccpAdd(def->getPosition(), ccp(wid, 0)));
    this->addChild(defvalue);
    
    
    
    
    
    SGCCLabelTTF *morvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString(),FONT_BOXINFO, 26);
    morvalue->setAnchorPoint(anchor);
    morvalue->setPosition(ccpAdd(mor->getPosition(), ccp(wid, 0)));
    this->addChild(morvalue);
    SGCCLabelTTF *roundvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getRound())->getCString(),FONT_BOXINFO, 26);
    roundvalue->setAnchorPoint(anchor);
    roundvalue->setPosition(ccpAdd(round->getPosition(), ccp(wid, 0)));
    this->addChild(roundvalue);
    SGCCLabelTTF *spdvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString(),FONT_BOXINFO, 26);
    spdvalue->setAnchorPoint(anchor);
    spdvalue->setPosition(ccpAdd(spd->getPosition(), ccp(wid, 0)));
    this->addChild(spdvalue);
    SGCCLabelTTF *govvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getGoverning())->getCString(),FONT_BOXINFO, 26);
    govvalue->setAnchorPoint(anchor);
    govvalue->setPosition(ccpAdd(gov->getPosition(), ccp(wid, 0)));
    this->addChild(govvalue);
}
void SGAdvanceLayer::showpropInfo(CCNode *node)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    CCString *obj = (CCString *)datas->objectAtIndex(node->getTag());
    int itemID = obj->intValue();
    SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(itemID);
    SGPropsCard *propCard = new SGPropsCard;
    propCard->setType(prop->getPropsPropsType());
    propCard->setItemType(prop->getPropsType());
    propCard->setIsProt(0);
    propCard->setCurrStar(prop->getPropsCurrStarLevel());
    propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
    propCard->setItemId(prop->getPropsId());
	propCard->setHeadSuffixNum(prop->getIconId());//12.05
	
    propCard->autorelease();
    SGPropInfoLayer *Layer = SGPropInfoLayer::create(propCard,1);
    SGMainManager::shareMain()->showLayer(Layer,false);
}
void SGAdvanceLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
}

void SGAdvanceLayer::confirmHandler()
{
    if(!m_btnCanTouch)
        return;
    if(!m_canAdcance)
    {
        SGMainManager::shareMain()->showMessage(str_adcance_tips);
    }
    //武将转生已达到上限
//    else if(m_isMaxAdv)
//        SGMainManager::shareMain()->showMessage(str_officer_zhuan_max);
    else
    {
        
        SGMainManager::shareMain()->setWillAdvancedItemId(_card->getItemId());
        SGMainManager::shareMain()->setAfterAdvancedItemId(afterCard->getItemId());
        SGMainManager::shareMain()->setAfterAdvancedSsid(_card->getSsid());
        
        main::OfficerUpgradeRequest *request = new main::OfficerUpgradeRequest();
        request->set_officerid(_card->getSsid());
        SGSocketClient::sharedSocketClient()->send(MSG_BARRACKS_ADVANCE, request);
        
        m_btnCanTouch = false;
    }
}


void SGAdvanceLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
	
	if (info && info->isFromEmbattle == true)
	{
		CCLog("Advancing........");
		//返回布阵页面
		SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, (SGOfficerCard *)_card, info->ty, info->em);
		SGMainManager::shareMain()->showBox(embattleBox);
	}
    else if (cardType == 1) {
        if (isfromfirst == 11)//从首页-详情-转生进入
        {
            SGMainManager::shareMain()->showGeneralInfoLayer(officer, 3,NULL,true,NULL,isfromfirst);
        }
        else if (isfromfirst ==12)//从布阵-详情-转生进入
        {
           SGMainManager::shareMain()->showGeneralInfoLayer(officer, 22,NULL,true,NULL,isfromfirst);
        }
        else
        {
        SGMainManager::shareMain()->showGeneralInfoLayer(officer,NULL);
        }
    }else if(cardType == 2) {
        if (isfromfirst) {
            SGMainManager::shareMain()->showEquipsLayer(NULL, 0, 2);
        }else
        {
        SGMainManager::shareMain()->showEquipInfoLayer(equip,0);
        }
    }
}
void SGAdvanceLayer::showCurrInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (cardType == 1)
    {
        SGMainManager::shareMain()->showGeneralInfoLayer(officer, 8);}
    else{
        SGEquipInfoLayer *Layer = SGEquipInfoLayer::create(equip,1);
        SGMainManager::shareMain()->showLayer(Layer,false);
    }
}
void SGAdvanceLayer::showAfterInfo()
{
    return;
    EFFECT_PLAY(MUSIC_BTN);
    if (cardType == 1)
    {
    SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create((SGOfficerCard *)afterCard,13);
        SGMainManager::shareMain()->showLayer(Layer,false);
    }
    else{
        SGEquipInfoLayer *Layer = SGEquipInfoLayer::create((SGEquipCard *)afterCard,1);
        SGMainManager::shareMain()->showLayer(Layer,false);
    }
}

void SGAdvanceLayer::advanceOfficerListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::OfficerUpgradeResponse *response = (main::OfficerUpgradeResponse *)sr->m_msg;
    if (sr)
    {
        main::OfficerCardItemProto proto = response->officercarditemproto();
        if (response->state() == 1)
        {
            SGMainManager::shareMain()->updataUserMsg();
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
            
            officer->setSsid(proto.ssid());
            officer->setItemId(proto.itemid());
            officer->setCurrExp(proto.currexp());
            officer->setLordId(proto.lordskillid());
            officer->setIsProt(proto.isprotection());
            officer->setCurrLevel(proto.currlevel());
            officer->setSkillExp(proto.generalsskillcurrexp());
            officer->setLordExp(proto.lordskillcurrexp());
            officer->setOfficerSkil(proto.generalsskillid());
            
            //武将攻防血速差异性，这几个属性改由服务器发送
            officer->setRealAtk(proto.atk());
            officer->setRealDef(proto.def());
            officer->setRealMorale(proto.hp());
            officer->setRealSpeed(proto.sp());
            
            if (temp->getOfficerNextId())
            {
                officer->setCost(temp->getOfficerCost());
            }
            officer->setExpRate(temp->getOfficerExpRate());
            officer->setMaxStar(temp->getOfficerNextId());
            officer->setMaxExp(expdata->getExpValue());
            officer->setCurrStar(temp->getOfficerCurrStarLevel());
            officer->setOfficerName(temp->getOfficerName());
            officer->setMaxLevel(temp->getOfficerMaxLevel());
            officer->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            officer->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            officer->setRound(temp->getOfficerRound());
            officer->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            officer->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            officer->setGoverning(temp->getOfficerGoverning());
            officer->setRace(temp->getOfficerRace());
            officer->setItemType(temp->getOfficerType());
            //转生最大星级
            officer->setUpgradestar(temp->getUpgradestar());
            //性别和缘分
            officer->setGender((OfficerGender) temp->getOfficerGender());
            officer->setFateList(temp->getOfficerFateList());
            //武将计最大等级
            officer->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officer->setLordMaxLevel(temp->getLordMaxLevel());
            //转生等级
            officer->setUpgradelevel(temp->getUpgradelevel());
            //武将原型ID
            officer->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officer->setSpeedFactor(temp->getOfficerSpeedFactor());
            officer->setAdNum(temp->getAdvNum());
			
			officer->setHeadSuffixNum(temp->getIconId());//12.05
            
//            SGMainManager::shareMain()->showAdvanceTexiao(beforeitemId,officer,cardType,beforestar,isfromfirst);
            
            SGAdvancedEffectLayer *eff = SGAdvancedEffectLayer::create(BIT_OFFICER,
                                                                       SGMainManager::shareMain()->getWillAdvancedItemId(),
                                                                       SGMainManager::shareMain()->getAfterAdvancedItemId()  , 1);
            
            SGMainManager::shareMain()->getMainScene()->addShowLayer(eff);
            
        }
        else
            SGMainManager::shareMain()->showMessage(str_zhuansheng_fail);
    }
    m_btnCanTouch = true;
}

void SGAdvanceLayer::sourceBtnClicked()
{
    int pieceId = -1;
    SGPiecesDataModel * piece = SGStaticDataManager::shareStatic()->getPiecesDictByOfficerProtoId(officer->getProtoId());
    if(piece)
        pieceId = piece->getPiecesId();
    
    SGLootWiKiMainLayer * lootWiKiMainLayer = SGLootWiKiMainLayer::create( pieceId ,-1);
    SGMainManager::shareMain()->showBox(lootWiKiMainLayer);

}

