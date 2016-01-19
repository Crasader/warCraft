//
//  SGStrengBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-8.
//
//

#include "SGStrengBox.h"
#include "SGMainManager.h"
#include "SGFont.h"
#include "SGStrengLvlUpBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "Itemmodule.pb.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"


SGStrengBox::SGStrengBox()
:_card(NULL), delegate(NULL), _cardArray(NULL), arrayLabel(NULL), coins(0)
{
    ;
}

SGStrengBox::~SGStrengBox()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_BARRACKS_STRENG);
    notification->removeObserver(this, MSG_EQUIP_STRENG);
    notification->removeObserver(this, MSG_SOLDIER_STRENG);
    _card->release();
    _cardArray->release();
    arrayLabel->release();
    
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_strengBox);
    
}
SGStrengBox *SGStrengBox::create(SGBoxDelegate *bdg, SGBaseMilitaryCard *card, CCArray *cardArray,int needCoins)
{
    //@无处调用
    SGStrengBox *strengBox = new SGStrengBox();
    strengBox->coins = needCoins;
    strengBox->_card = card;
    strengBox->_card->retain();    
    strengBox->_cardArray = cardArray;
    strengBox->_cardArray->retain();
    strengBox->delegate = bdg;
    
    if (strengBox && strengBox->init(bdg,card))
    {       
        strengBox->autorelease();
        return strengBox;
    }
    CC_SAFE_DELETE(strengBox);
    return NULL;
}
bool SGStrengBox::init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card)
{
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303.plist", RES_TYPE_LAYER_UI, sg_strengBox);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengBox, LIM_PNG_AS_PNG);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BARRACKS_STRENG,
                              this,
                              callfuncO_selector(SGStrengBox::strengOfficerListener));
    notification->addObserver(MSG_EQUIP_STRENG,
                              this,
                              callfuncO_selector(SGStrengBox::strengEquipListener));
    notification->addObserver(MSG_SOLDIER_STRENG,
                              this,
                              callfuncO_selector(SGStrengBox::strengSoldierListener));

    arrayLabel = CCArray::create();
    arrayLabel->retain();

    delegate = bdg;
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float wid = skewing(230);
    float hgt = skewingY(227);
    if(s.height == 1136)
    {
        hgt = skewing(227);
    }
    
    
    CCSprite *title = NULL;
    CCLabelTTF* name = NULL;
    CCString *str_ = CCString::create(_card->getClassName());
    
    if(str_->isEqual(CCString::create("SGOfficerCard")))
    {
        if (!SGBaseBox::init(bdg,box_strengTag,CCSizeMake(wid, hgt + 70)))
        {
            return false;
        }
    }
    else
    {
        if (!SGBaseBox::init(bdg,box_strengTag,CCSizeMake(wid, hgt)))
        {
            return false;
        }
    }    
     
    if (str_->isEqual(CCString::create("SGSoldierCard")))
    {
        name = CCLabelTTF::create(_card->getOfficerName()->getCString(), "Arial-BoldMT", 26);
        title = CCSprite::createWithSpriteFrameName("font_strengshib.png");    }
    else if(str_->isEqual(CCString::create("SGOfficerCard")))
    {
        name = CCLabelTTF::create(_card->getOfficerName()->getCString(), "Arial-BoldMT", 26);
        title = CCSprite::createWithSpriteFrameName("font_strengwuj.png");        
        SGOfficerCard *temp = (SGOfficerCard *)_card;
       CCObject *obj = NULL;
       int rate = 0;
       int countnum = 0;
        CCARRAY_FOREACH(_cardArray, obj)
        {
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(temp->getLordSkill());

            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)obj;
            CCString *str_ = CCString::create(card->getClassName());
            if (str_->isEqual(CCString::create("SGOfficerCard"))) {
                SGSkillDataModel *lordSkill1 = SGStaticDataManager::shareStatic()->getLordSkillById(((SGOfficerCard *)card)->getLordSkill());
                if (lordSkill  &&lordSkill1 &&temp->getLordSkill() &&lordSkill->getSkillUpId() == lordSkill1->getSkillUpId()  ) {
                    rate += (card->getCurrStar() *3 + 15);
                    countnum++;
                }
            }            
        }
        if (temp->getLordSkill()) {
            
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(temp->getLordSkill());
            if (lordSkill) {                
                
                CCLabelTTF *notice = CCLabelTTF::create(CCString::createWithFormat(str_StrengCox_str1,countnum)->getCString(), FONT_BOXINFO, FONTSIZE(11));
                this->addChild(notice);
                notice->setAnchorPoint(ccp(0, 0.5));
                notice->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.45, -hgt/2 + 30)));
                CCString *lvl = CCString::create("");
                int times = 0;
                if (lordSkill->getSkillType() == 1) {
                    lvl->setString(str_chuji);
                    times = 3;
                }else if (lordSkill->getSkillType() == 2) {
                    lvl->setString(str_zhongji);
                    times = 4;
                }else if (lordSkill->getSkillType() == 3) {
                    lvl->setString(str_gaoji);
                    times = 5;
                }
                if (rate >100) {
                    rate =100;
                }
                CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat(str_StrengCox_str2,lvl->getCString(),lordSkill->getSkillName()->getCString(),lordSkill->getLevel(),times,rate)->getCString(),FONT_BOXINFO, FONTSIZE(11));
                label->setAnchorPoint(ccp(0, 0.5));
                this->addChild(label);
                label->setPosition(ccpAdd(notice->getPosition(), ccp(0, -25)));
                
                if (rate == 0) {
                    notice->setColor(ccc3(0x55, 0x55, 0x55));
                    label->setColor(ccc3(0x55, 0x55, 0x55));
                }else{
                    notice->setColor(ccWHITE);
                    label->setColor(ccWHITE);
                }
        }
    }        
 
        
    }else if(str_->isEqual(CCString::create("SGEquipCard")))
    {
        name = CCLabelTTF::create(_card->getOfficerName()->getCString(), "Arial-BoldMT", 26);

        title = CCSprite::createWithSpriteFrameName("font_strengzb.png");        
    }
    if (title) {
        this->addChild(title);
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.385)));
    }

    //头像
    char spriteName[256] = {0};
    CCString *str_nowcard = CCString::create(_card->getClassName());
    SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_strengBox);
    if (str_nowcard->isEqual(CCString::create("SGSoldierCard"))){
        sprintf(spriteName, "%d-%d.png",_card->getItemId(),SGPlayerInfo::sharePlayerInfo()->getPlayerModifyCount());}
    else{
		//12.06//////////////////////////////////////
		int itemId = _card->getItemId();
		int iconId = -1;
		if (itemId > 4000)//装备
		{
			SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(itemId);
			iconId = temp->getIconId();
		}
		else if (itemId > 1000)//武将
		{
			SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
			iconId = temp->getIconId();
		}
		else
		{
			SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(itemId);
			iconId = temp->getIconId();
		}
		//12.06//////////////////////////////////////
		sprintf(spriteName, "head%d.png",iconId);
//		sprintf(spriteName, "head%d.png",_card->getItemId());
    }

    SGButton *portrait = SGButton::create(spriteName,"jinkuang.png", this,

                                    menu_selector(SGStrengBox::showGenerInfo),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addChild(portrait);
    portrait->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.37, hgt*.23)));
    
    
    //姓名
//    CCLabelTTF* name = CCLabelTTF::create(_card->getOfficerName()->getCString(), "Arial-BoldMT", 26);
    if (name) {
        name->setAnchorPoint(ccp(0, 1));
        name->setPosition(ccp(portrait->getPosition().x + portrait->getContentSize().width*.7, portrait->getPosition().y + portrait->getContentSize().height/2));
        this->addChild(name);
    }

    
    //当前星级    
    int starCount = this->_card->getCurrStar();
    for(int i=0;i<starCount;i++)
    {
        CCSprite *spriteStarCount = CCSprite::createWithSpriteFrameName("xx.png");
        spriteStarCount->setAnchorPoint(ccp(0, 0.5));
        spriteStarCount->setPosition(ccp(name->getPosition().x + spriteStarCount->getContentSize().width*i, portrait->getPosition().y));
        this->addChild(spriteStarCount);
        if(str_->isEqual(CCString::create("SGOfficerCard")))
        {
            spriteStarCount->setPosition(ccpAdd(spriteStarCount->getPosition(), ccp(0, 35)));
        }
    }

    //等级
    CCString *str_lvl = CCString::createWithFormat(str_Format_ji,_card->getCurrLevel());
//    CCString *str_lvl = CCString::createWithFormat("%d级临时",1);
    CCLabelTTF* lvl = CCLabelTTF::create(str_lvl->getCString(), "Arial-BoldMT", 24);
    if (lvl) {
        lvl->setColor(ccc3(0xff, 0xff, 0xff));
        lvl->setAnchorPoint(ccp(1, 0.5));
        this->addChild(lvl);
        lvl->setPosition(ccp(title->getPosition().x + lvl->getContentSize().height * 2.5, name->getPosition().y - name->getContentSize().height/2));
    }

    
    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGStrengBox::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.62-10)));
    
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGStrengBox::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.62-10)));
    
    CCSprite* redbg = CCSprite::createWithSpriteFrameName("red_bg.png");
    redbg->setAnchorPoint(ccp(0.5, 1));
    redbg->setScaleX(wid/redbg->getContentSize().width);
    this->addChild(redbg,-1);
    redbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter),ccp(0, hgt*.1)));
    redbg->setScaleY(hgt*0.355/redbg->getContentSize().height);
    
    CCSprite *tit = CCSprite::createWithSpriteFrameName("font_yxzsck.png");
    tit->setPosition(ccpAdd(redbg->getPosition(), ccp(0, - hgt*.05)));
    this->addChild(tit);

    
    CCLabelTTF *label1 = CCLabelTTF::create(str_one_star_card3, "Arial-BoldMT", 24);
    CCLabelTTF *label2 = CCLabelTTF::create(str_two_star_card3, "Arial-BoldMT", 24);
    CCLabelTTF *label3 = CCLabelTTF::create(str_three_star_card3, "Arial-BoldMT", 24);
    CCLabelTTF *label4 = CCLabelTTF::create(str_4_star_card3, "Arial-BoldMT", 24);
    CCLabelTTF *label5 = CCLabelTTF::create(str_5_star_card3, "Arial-BoldMT", 24);
    CCLabelTTF *label6 = CCLabelTTF::create(str_6_star_card3, "Arial-BoldMT", 24);
    label1->setPosition(ccpAdd(tit->getPosition(), ccp(-wid*.21, - redbg->getContentSize().height/3)));
    label4->setPosition(ccpAdd(tit->getPosition(), ccp(wid*.21, - redbg->getContentSize().height/3)));
    label2->setPosition(ccpAdd(label1->getPosition(), ccp(0, -redbg->getContentSize().height/3)));
    label5->setPosition(ccpAdd(label4->getPosition(), ccp(0, -redbg->getContentSize().height/3)));
    label3->setPosition(ccpAdd(label2->getPosition(), ccp(0, -redbg->getContentSize().height/3)));
    label6->setPosition(ccpAdd(label5->getPosition(), ccp(0, -redbg->getContentSize().height/3)));
    this->addChild(label1);
    this->addChild(label2);
    this->addChild(label3);
    this->addChild(label4);
    this->addChild(label5);
    this->addChild(label6);
    label1->setVisible(false);
    label2->setVisible(false);
    label3->setVisible(false);
    label4->setVisible(false);
    label5->setVisible(false);
    label6->setVisible(false);
    arrayLabel->addObject(label1);
    arrayLabel->addObject(label2);
    arrayLabel->addObject(label3);
    arrayLabel->addObject(label4);
    arrayLabel->addObject(label5);
    arrayLabel->addObject(label6);
    
    
    if(str_->isEqual(CCString::create("SGOfficerCard")))
    {
        title->setPosition(ccpAdd(title->getPosition(), ccp(0, 35)));
        portrait->setPosition(ccpAdd(portrait->getPosition(), ccp(0, 35)));
        redbg->setPosition(ccpAdd(redbg->getPosition(), ccp(0, 35)));
        tit->setPosition(ccpAdd(tit->getPosition(), ccp(0, 35)));
        ok->setPosition(ccpAdd(ok->getPosition(), ccp(0, -35)));
        cancel->setPosition(ccpAdd(cancel->getPosition(), ccp(0, -35)));
        lvl->setPosition(ccpAdd(lvl->getPosition(), ccp(0, 35)));
        name->setPosition(ccpAdd(name->getPosition(), ccp(0, 35)));
        label1->setPosition(ccpAdd(label1->getPosition(), ccp(0, 35)));
        label4->setPosition(ccpAdd(label4->getPosition(), ccp(0, 35)));
        label2->setPosition(ccpAdd(label2->getPosition(), ccp(0, 35)));
        label5->setPosition(ccpAdd(label5->getPosition(), ccp(0, 35)));
        label3->setPosition(ccpAdd(label3->getPosition(), ccp(0, 35)));
        label6->setPosition(ccpAdd(label6->getPosition(), ccp(0, 35)));
    }
    
    //费劲的算法......
    int star[128]={0};
    int addEXP = 0;
    int proExp = 0;    
    for(int i=0;i<_cardArray->count();i++)
    {        
        SGBaseMilitaryCard *card_ = (SGBaseMilitaryCard*)_cardArray->objectAtIndex(i);
        int cardCurrStar = card_->getCurrStar();
        CCString *str_1 = CCString::create(card_->getClassName());
        if (str_1->isEqual(CCString::create("SGPropsCard")))
        {
            proExp += (((SGPropsCard *)card_)->getValue());
            if (str_->isEqual(CCString::create("SGOfficerCard")) &&((SGPropsCard *)card_)->getRace() == ((SGOfficerCard *)_card)->getRace()) {
                proExp = proExp + proExp*0.3;
            }
            
           // CCLOG("%d",proExp);
        }
        else if(str_1->isEqual(CCString::create("SGOfficerCard")))
        {      
        
        float expRate = card_->getExpRate();
        int gov = card_->getGoverning(); 
        int lvl = card_->getCurrLevel();
        addEXP += (((gov >0?gov:-gov) + cardCurrStar) * expRate * lvl);
            if (((SGOfficerCard *)card_)->getRace() == ((SGOfficerCard *)_card)->getRace()) {
                addEXP = addEXP + addEXP*0.3;
            }
        CCLog("cardCurrStar is %d",cardCurrStar);        
        }
        else if(str_1->isEqual(CCString::create("SGEquipCard")))
        {            
            float expRate = card_->getExpRate();
            int gov = card_->getGoverning();
            int lvl = card_->getCurrLevel();
            addEXP += (((gov >0?gov:-gov) +cardCurrStar) * expRate * lvl);
            if (((SGEquipCard *)card_)->getItemType() == ((SGEquipCard *)_card)->getItemType()) {
                addEXP = addEXP + addEXP*0.3;
            }
            CCLog("cardCurrStar is %d",cardCurrStar);
        }

        if(cardCurrStar != 0 && cardCurrStar < 128)
        {
            star[cardCurrStar] ++;
        }
    }
    CCSprite *exp = CCSprite::createWithSpriteFrameName("font_yjhdexp.png");
    this->addChild(exp);
    exp->setPosition(ccpAdd(redbg->getPosition(), ccp(-wid*.27, -hgt*.4)));
    int totleExp = addEXP +proExp;
    CCLabelTTF *label_exp = CCLabelTTF::create(CCString::createWithFormat("%d",totleExp)->getCString(), "Arial-BoldMT", 26);
    label_exp->setAnchorPoint(ccp(0, 0.5));
    label_exp->setPosition(ccpAdd(exp->getPosition(), ccp(exp->getContentSize().width*.55, 0)));
    this->addChild(label_exp);

    int cardIndex = 0;
    for(int i=0;i<128;i++)
    {
        if(star[i] == 0)
        {
            continue;
        }
        
        CCLabelTTF *label = (CCLabelTTF*)arrayLabel->objectAtIndex(cardIndex);        
        CCString *string = CCString::createWithFormat(str_Format__star_card3,i,star[i]);
        label->setString(string->getCString());
        label->setVisible(true);
        if(i>=4)
        {
            label->setColor(ccRED);
        }else{
            label->setColor(ccWHITE);
        }
        
        cardIndex++;
    }
    
    int type = SGStaticDataManager::shareStatic()->getStrengCardType(str_, _card->getItemId());
    
//    if(str_->isEqual(CCString::create("SGOfficerCard")))
//    {
//    SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
//    if (officer->getOfficerExpType()->isEqual(CCString::create("A"))) {
//        type = 0;
//    }
//    else if (officer->getOfficerExpType()->isEqual(CCString::create("B"))) {
//        type = 1;
//    }
//    else if (officer->getOfficerExpType()->isEqual(CCString::create("C"))) {
//        type = 2;
//    }
//    else if (officer->getOfficerExpType()->isEqual(CCString::create("D"))) {
//        type = 3;
//    }
//    }else if(str_->isEqual(CCString::create("SGSoldierCard")))
//    {
//        SGSoldierDataModel *officer = SGStaticDataManager::shareStatic()->getSoliderById(_card->getItemId());
//        if (officer->getSoldierExpType()->isEqual(CCString::create("A"))) {
//            type = 0;
//        }
//        else if (officer->getSoldierExpType()->isEqual(CCString::create("B"))) {
//            type = 1;
//        }
//        else if (officer->getSoldierExpType()->isEqual(CCString::create("C"))) {
//            type = 2;
//        }
//        else if (officer->getSoldierExpType()->isEqual(CCString::create("D"))) {
//            type = 3;
//        }
//
//    }
//    else if(str_->isEqual(CCString::create("SGEquipCard")))
//    {
//        SGEquipmentDataModel *officer = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
//        if (officer->getEquipExpType()->isEqual(CCString::create("A"))) {
//            type = 0;
//        }
//        else if (officer->getEquipExpType()->isEqual(CCString::create("B"))) {
//            type = 1;
//        }
//        else if (officer->getEquipExpType()->isEqual(CCString::create("C"))) {
//            type = 2;
//        }
//        else if (officer->getEquipExpType()->isEqual(CCString::create("D"))) {
//            type = 3;
//        }
//
//    }
    int percent = 0;
    int currLvl = _card->getCurrLevel();
    SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
    
    while (totleExp >= num->getExpValue()) {
        currLvl ++;
        totleExp -= num->getExpValue();
        num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl);
        if (currLvl>_card->getMaxLevel()) {
            percent = 1;
        }
    }
    
    if (currLvl >_card->getMaxLevel()) {
        currLvl = _card->getMaxLevel();
    }    

    CCLabelTTF *afterLvl = CCLabelTTF::create(CCString::createWithFormat("Lv%d",currLvl)->getCString(), FONT_BOXINFO, FONTSIZE(12));
    afterLvl->setAnchorPoint(ccp(0, 0.5));
    this->addChild(afterLvl);
    afterLvl->setPosition(ccpAdd(exp->getPosition(), ccp(-exp->getContentSize().width*.45, -exp->getContentSize().height*1.5)));
    
    CCSprite *progresscao = CCSprite::createWithSpriteFrameName("lantiao.png");
    this->addChild(progresscao);
    progresscao->setPosition(ccpAdd(afterLvl->getPosition(), ccp(afterLvl->getContentSize().width*1.1 + progresscao->getContentSize().width/2, 0)));
    CCProgressTimer *progress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("tiao_lan.png"));
    progress->setType(kCCProgressTimerTypeBar);
    progress->setMidpoint(ccp(0,0));
    progress->setBarChangeRate(ccp(1, 0));
    addChild(progress,10);
    progress->setPosition(progresscao->getPosition());
    float rate = (float)totleExp/num->getExpValue();
    progress->setPercentage(rate*100);
    
    if (percent) {
        progress->setPercentage(100);
    }
    


    return true;
}

void SGStrengBox::showGenerInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    this->boxClose();
    CCString *str_nowcard = CCString::create(_card->getClassName());
    if (str_nowcard->isEqual(CCString::create("SGOfficerCard")))
    {        
        SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card, NULL);
    }else if (str_nowcard->isEqual(CCString::create("SGEquipCard")))
    {        
        SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card,0);
    }else{
        SGMainManager::shareMain()->showSoldierInfoLayer((SGSoldierCard *)_card,0);
    }
}
void SGStrengBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
        CCString *str_nowcard = CCString::create(_card->getClassName());
        if (str_nowcard->isEqual(CCString::create("SGOfficerCard")))
        {
            main::OfficerCompositeRequest *request = new main::OfficerCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _cardArray->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_cardArray->objectAtIndex(i);
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
            for (int i = 0; i < _cardArray->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_cardArray->objectAtIndex(i);
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
        }else{
            
            main::SoldierCompositeRequest *request = new main::SoldierCompositeRequest();
            request->set_ssid(_card->getSsid());            
            for (int i = 0; i < _cardArray->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_cardArray->objectAtIndex(i);              
                request->add_propsid(baseMilitaryCard->getSsid());
            }                
            SGSocketClient::sharedSocketClient()->send(MSG_SOLDIER_STRENG, request);
        }
      
}


void SGStrengBox::strengOfficerListener(CCObject *obj)
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
            officerCard->setSsid(proto.ssid());
//            officerCard->setItemId(proto.itemid());
//            officerCard->setOfficerName(CCString::create(proto.name()));
//            officerCard->setPositionId(proto.position());
//            officerCard->setMaxLevel(proto.maxlevel());
//            officerCard->setCurrExp(proto.currexp());
//            officerCard->setAtk(proto.atk());
//            officerCard->setDef(proto.def());
//            officerCard->setRound(proto.round());
//            officerCard->setSpeed(proto.speed());
//            officerCard->setMaxExp(proto.maxexp());
//            officerCard->setMorale(proto.morale());
//            officerCard->setGoverning(proto.governing());
//            officerCard->setSell(proto.sell());            
//            officerCard->setExpRate(proto.exprate());
//            officerCard->setCurrLevel(proto.currlevel());
//            officerCard->setCurrStar(proto.currstarlevel());
//            officerCard->setIsProt(proto.isprotection());
////            officerCard->setIsExped(proto.isexpedition());
//            officerCard->setMaxStar(proto.nextid());
//            officerCard->setRace(proto.race());
//            officerCard->setItemType(proto.itemtype());
//            officerCard->setOfficerSkil(proto.generalsskillid());
//            officerCard->setLordSkill(proto.lordskillid());            
//            officerCard->setCost(proto.cost());            
//            officerCard->setLordTimes(proto.lordskilllevel());
//            
//            int length = proto.reqitems_size();
//            for (int j=0; j < length; j++) {
//                officerCard->addReqitemId(proto.reqitems(j));
//            }
            int len = _cardArray->count();
            for (int i = 0; i < len; i++) {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_cardArray->objectAtIndex(i);
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
            
            //服务器直接推送更新，不需要自动操作
            //playerInfo->getOfficerCards()->removeObject(_card);
            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - coins);
            SGMainManager::shareMain()->updataUserMsg();
            //playerInfo->addOfficerCard(officerCard);
            
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_StrengCox_str3,response->upgradestate(),response->allexp())->getCString());
            if (officerCard->getCurrLevel() > _card->getCurrLevel()) {
                delegate->setIsLock(true);
                SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(delegate, officerCard,_card,1);
                SGMainManager::shareMain()->showBox(LvlUpBox);
                EFFECT_PLAY(MUSIC_BTN);
                this->boxCloseWithOutCallBack(); 
            }
            else
            {
                SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,NULL);
                EFFECT_PLAY(MUSIC_35);
                this->boxClose();
            }

            officerCard->autorelease();         
        }
        else
        {
            
            SGMainManager::shareMain()->showMessage(str_fail);
            
            this->boxClose();
        }

    }
}

void SGStrengBox::strengEquipListener(cocos2d::CCObject *obj)
{
//    SGEquipCard* equipCard = NULL;
//    SGSocketRequest *sr = (SGSocketRequest *)obj;    
//    if (sr)
//    {
//        main::EquipmentCompositeResponse *response = (main::EquipmentCompositeResponse *)sr->m_msg;
//        main::EquipmentCardItemProto cardInfo = response->equipmentcarditemproto();
//        if (response->state() == 1)
////        {
////            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
////            equipCard = new SGEquipCard;
////            equipCard->setSsid(cardInfo.ssid());
////            equipCard->setItemId(cardInfo.itemid());
////            equipCard->setMorale(cardInfo.hp());
////            equipCard->setAtk(cardInfo.atk());
////            equipCard->setDef(cardInfo.def());
////            equipCard->setRound(cardInfo.round());
////            equipCard->setSpeed(cardInfo.speed());
////            equipCard->setExpRate(cardInfo.exprate());
////            equipCard->setGoverning(cardInfo.governing());
////            equipCard->setSetNum(cardInfo.setnum());
////            equipCard->setSell(cardInfo.sell());
////            equipCard->setMaxExp(cardInfo.maxexp());
////            equipCard->setCurrLevel(cardInfo.currlevel());
////            equipCard->setCurrStar(cardInfo.currstarlevel());
////            equipCard->setCurrExp(cardInfo.currexp());
////            equipCard->setOfficerCardId(cardInfo.officercarditemid());
////            equipCard->setItemType(cardInfo.itemtype());
////            equipCard->setMaxStar(cardInfo.nextid());
////            equipCard->setMaxLevel(cardInfo.maxlevel());
////            equipCard->setGoverning(cardInfo.governing());
////            equipCard->setIsProt(cardInfo.isprotection());
////            equipCard->setOfficerName(CCString::create(cardInfo.name()));            
////            equipCard->setCost(cardInfo.cost()); 
////            int length = cardInfo.reqitems_size();
////            for (int j=0; j < length; j++) {
////                equipCard->addReqitemId(cardInfo.reqitems(j));
////            }
////            int len = _cardArray->count();
////            for (int i = 0; i < len; i++) {
////                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_cardArray->objectAtIndex(i);
////                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
////                if (str_->isEqual(CCString::create("SGPropsCard")))
////                {
////                    playerInfo->getPropCards()->removeObject(baseMilitaryCard);
////                }
////                
////                else if(str_->isEqual(CCString::create("SGEquipCard")))
////                {
////                    playerInfo->getEquipCards()->removeObject(baseMilitaryCard);
////                }
////            }
////            playerInfo->getEquipCards()->removeObject(_card);
////            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - coins);
////            SGMainManager::shareMain()->updataUserMsg();
////            playerInfo->addEquipCard(equipCard);
//            SGMainManager::shareMain()->showMessage(CCString::createWithFormat("%d增加经验%d",response->upgradestate(),response->allexp())->getCString());
//            
//            if (equipCard->getCurrLevel() > _card->getCurrLevel()) {
//                delegate->setIsLock(true);
//                SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(delegate, equipCard,_card,2);
//                SGMainManager::shareMain()->showBox(LvlUpBox);
//                
//                this->boxCloseWithOutCallBack();
//            }
//            else
//            {
//                
//                SGMainManager::shareMain()->showEquipInfoLayer(equipCard,0);
//                
//                this->boxClose();
//            }
//            
//            equipCard->autorelease();
//        }
//        else
//        {
//            
//            SGMainManager::shareMain()->showMessage("失败");
//            
//            this->boxClose();
//        }
//
//    }
//    
    }
void SGStrengBox::strengSoldierListener(cocos2d::CCObject *obj)
{
//    SGSoldierCard* soldierCard = NULL;
//    SGSocketRequest *sr = (SGSocketRequest *)obj;    
//    if (sr)
//    {
//        main::SoldierCompositeResponse *response = (main::SoldierCompositeResponse *)sr->m_msg;
//        main::SoldierCardItemProto proto = response->soldiercarditemproto();
//        if (response->state() == 1)
//        {
//            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//            soldierCard = new SGSoldierCard;
//            SGSoldierDataModel *oCard = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
//            soldierCard->setOfficerName(oCard->getSoldierName());
//            soldierCard->setMaxLevel(proto.maxlevel());
//            soldierCard->setSsid(proto.ssid());
//            soldierCard->setItemId(proto.itemid());
//            soldierCard->setAtk(proto.atk());
//            soldierCard->setDef(proto.def());
//            soldierCard->setRound(proto.round());
//            soldierCard->setMorale(proto.morale());
//            soldierCard->setCurrLevel(proto.currlevel());
//            soldierCard->setCurrStar(proto.currstarlevel());
//            soldierCard->setCurrExp(proto.currexp());
//            soldierCard->setItemType(proto.itemtype());
//            soldierCard->setMaxExp(proto.maxexp());
//            playerInfo->addSoldierCard(soldierCard);            
//            
//            int len = _cardArray->count();
//            for (int i = 0; i < len; i++)
//            {
//                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_cardArray->objectAtIndex(i);
//        
//                    playerInfo->getPropCards()->removeObject(baseMilitaryCard);
//            }
//            playerInfo->getSoldierCards()->removeObject(_card);
//            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - coins);
//            SGMainManager::shareMain()->updataUserMsg();
//            playerInfo->addSoldierCard(soldierCard);
//            if (soldierCard->getCurrLevel() > _card->getCurrLevel()) {
//                delegate->setIsLock(true);
//                SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(delegate, soldierCard,_card,3);
//                SGMainManager::shareMain()->showBox(LvlUpBox);
//                this->boxCloseWithOutCallBack();
//            }
//            else
//            {
//                
//                SGMainManager::shareMain()->showSoldierInfoLayer(soldierCard,0);
//                this->boxClose();
//            }
//            SGMainManager::shareMain()->showMessage(CCString::createWithFormat("%d增加经验%d",response->upgradestate(),response->allexp())->getCString());
//            soldierCard->autorelease();
//        }
//        else
//        {
//            
//            SGMainManager::shareMain()->showMessage("失败");
//            
//            this->boxClose();
//        }
//
//    }    
    
    
}