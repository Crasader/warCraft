//
//  SGAdvanceBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGAdvanceBox.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "Itemmodule.pb.h"
#include "SGPlayerInfo.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGAdvanceBox::SGAdvanceBox():
_card(NULL),
arrayLabel(NULL),
reqCards(NULL),
cardType(0),
officer(NULL),
equip(NULL)
{
}
SGAdvanceBox::~SGAdvanceBox()
{

	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_advanceTag);

	
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_BARRACKS_ADVANCE);
    notification->removeObserver(this, MSG_EQUIP_ADVANCE);
    arrayLabel->release();
    reqCards->release();
}
SGAdvanceBox *SGAdvanceBox::create(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,int type)
{    
    SGAdvanceBox *advanceBox = new SGAdvanceBox();
    advanceBox->_card = card;
    advanceBox->cardType = type;
    if (advanceBox && advanceBox->init(bdg,card))
    {        
        advanceBox->autorelease();
        return advanceBox;
    }
    CC_SAFE_DELETE(advanceBox);
    return NULL;
}
bool SGAdvanceBox::init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card)
{
  
	
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_box_advanceTag);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_box_advanceTag, LIM_PNG_AS_PNG);

 
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BARRACKS_ADVANCE,
                              this,
                              callfuncO_selector(SGAdvanceBox::advanceOfficerListener));
    notification->addObserver(MSG_EQUIP_ADVANCE,
                              this,
                              callfuncO_selector(SGAdvanceBox::advanceEquipListener));
    
    reqCards = CCArray::create();
    reqCards->retain();
    CCObject *obj = NULL;
    CCArray *propCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
   
    //测试plist代码
   // CCSprite *title = CCSprite::create("barrack/font_jjwj.png");
    CCSprite *title=CCSprite::createWithSpriteFrameName("font_jjwj.png");
    if (cardType ==1) {
        officer = (SGOfficerCard *)_card;
        
        CCARRAY_FOREACH(officer->getRequitemIds(), obj)
        {
            CCString *str = (CCString *)obj;
            CCObject *obj_ = NULL;
            CCARRAY_FOREACH(propCards, obj_)
            {
                SGPropsCard *card = (SGPropsCard *)obj_;
                if (card->getItemId() == str->intValue())
                {
                    reqCards->addObject(card);
                    break;
                }
            }
        }
    }
     else if(cardType == 2)
    {
        
        //测试plist代码
       // CCSprite *sprite = CCSprite::create("barrack/font_jjzb.png");
        CCSprite *sprite=CCSprite::createWithSpriteFrameName("font_jjzb.png");
        title->setDisplayFrame(sprite->displayFrame());
        equip = (SGEquipCard *)_card;
        CCARRAY_FOREACH(equip->getRequitemIds(), obj)
        {
            CCString *str = (CCString *)obj;
            CCObject *obj_ = NULL;
            CCARRAY_FOREACH(propCards, obj_)
            {
                SGPropsCard *card = (SGPropsCard *)obj_;
                if (card->getItemId() == str->intValue())
                {
                    reqCards->addObject(card);
                    break;
                }
            }
        }
    }   
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float wid = skewing(230);
    float hgt = skewingY(200);
    if (s.height == 1136) {
        hgt = skewing(200);
    }
    
    this->addChild(title,10);
    title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.37)));

    if (!SGBaseBox::init(bdg,box_advanceTag,CCSizeMake(wid, hgt)))
    {
        return false;
    }
    
    char spriteName[256] = {0};
	
	//12.06
	SGMainManager::shareMain()->addHeadIconPlistByNum(card->getHeadSuffixNum(),sg_advanceLayer);
    sprintf(spriteName, "head%d.png",card->getHeadSuffixNum());

    SGButton *portrait = SGButton::create("jinkuang.png",spriteName,
                                    this,
                                    menu_selector(SGAdvanceBox::showGeneralInfo),
                                    ccp(0, 0),
                                    false,
                                    true);
    portrait->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.37, hgt*.2)));
    this->addChild(portrait);
    
//    CCSprite *background = CCSprite::create("public/card_bj.png");
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");

    this->addChild(background,-1);
    background->setPosition(portrait->getPosition());
    
    CCLabelTTF*name = CCLabelTTF::create(card->getOfficerName()->getCString(), FONT_PANGWA, FONTSIZE(12));

    name->setAnchorPoint(ccp(0, 1));
    name->setPosition(ccp(portrait->getPosition().x + portrait->getContentSize().width*.7, portrait->getPosition().y + portrait->getContentSize().height/2));
    this->addChild(name);
    
    for(int i=0; i < card->getCurrStar();i++)
    {
        CCSprite *xingji=CCSprite::createWithSpriteFrameName("xx.png");
        
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccp(name->getPosition().x + xingji->getContentSize().width, name->getPosition().y - xingji->getContentSize().height*1.4));
        this->addChild(xingji);
    }

    CCLabelTTF* lvl = CCLabelTTF::create(CCString::createWithFormat(str_Format_ji,card->getCurrLevel())->getCString(),FONT_BOXINFO, FONTSIZE(12));
    lvl->setColor(ccc3(0xff, 0xff, 0xff));
    lvl->setAnchorPoint(ccp(1, 0.5));
    this->addChild(lvl);
    lvl->setPosition(ccp(title->getPosition().x + lvl->getContentSize().height * 2.5, name->getPosition().y - name->getContentSize().height/2));
    
    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGAdvanceBox::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.62-10)));
    
    
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGAdvanceBox::boxCloseWithOutCallBack),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.62-10)));
    
    CCSprite *redbg=CCSprite::createWithSpriteFrameName("red_bg.png");
    
    redbg->setAnchorPoint(ccp(0.5, 1));
    redbg->setScaleX(wid/redbg->getContentSize().width);
    this->addChild(redbg,-1);
    redbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter),ccp(0,0)));
    redbg->setScaleY(hgt*0.4/redbg->getContentSize().height);
    
    CCSprite *tit=CCSprite::createWithSpriteFrameName("font_yxzsck.png");
    
    tit->setPosition(ccpAdd(redbg->getPosition(), ccp(0, - hgt*.05)));
    this->addChild(tit);
    
    CCLabelTTF *label1 = CCLabelTTF::create(str_one_star_card3, "TJenLei-Medium", FONTSIZE(12));
    CCLabelTTF *label2 = CCLabelTTF::create(str_two_star_card3, "TJenLei-Medium", FONTSIZE(12));
    CCLabelTTF *label3 = CCLabelTTF::create(str_three_star_card3, "TJenLei-Medium", FONTSIZE(12));
    CCLabelTTF *label4 = CCLabelTTF::create(str_4_star_card3, "TJenLei-Medium", FONTSIZE(12));
    CCLabelTTF *label5 = CCLabelTTF::create(str_5_star_card3, "TJenLei-Medium", FONTSIZE(12));
    CCLabelTTF *label6 = CCLabelTTF::create(str_6_star_card3, "TJenLei-Medium", FONTSIZE(12));
    this->addChild(label1);
    this->addChild(label2);
    this->addChild(label3);
    this->addChild(label4);
    this->addChild(label5);
    this->addChild(label6);
    label1->setPosition(ccpAdd(tit->getPosition(), ccp(-wid*.21, - redbg->getContentSize().height/3)));
    label4->setPosition(ccpAdd(tit->getPosition(), ccp(wid*.21, - redbg->getContentSize().height/3)));
    label2->setPosition(ccpAdd(label1->getPosition(), ccp(0, -redbg->getContentSize().height/3)));
    label5->setPosition(ccp(label4->getPosition().x, label2->getPosition().y));
    label3->setPosition(ccpAdd(label2->getPosition(), ccp(0, -redbg->getContentSize().height/3)));
    label6->setPosition(ccp(label4->getPosition().x, label3->getPosition().y));
    arrayLabel = CCArray::create();
    arrayLabel->retain();
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
        

    int star[10]={0};
    //CCLOG("%d",reqCards->count());
    for(int i=0;i< reqCards->count();i++)
    {
        SGBaseMilitaryCard *card_ = (SGBaseMilitaryCard*)reqCards->objectAtIndex(i);        
                    
            int cardCurrStar = card_->getCurrStar();           
            
            CCLOG("cardCurrStar is %d",cardCurrStar);
            
            if(cardCurrStar != 0 && cardCurrStar < 10)
            {
                star[cardCurrStar] ++;     
            }
    }
    int cardIndex = 0;
    for(int i=0;i<10;i++)
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
 
    return true;
}
void SGAdvanceBox::showGeneralInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    this->boxCloseWithOutCallBack();
    if (cardType == 1) {
        SGMainManager::shareMain()->showGeneralInfoLayer(officer,NULL);
    }
    if (cardType ==2) {
        SGMainManager::shareMain()->showEquipInfoLayer(equip,0);
    }
}

void SGAdvanceBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (cardType == 1) {
        main::OfficerUpgradeRequest *request = new main::OfficerUpgradeRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BARRACKS_ADVANCE, request);
    }
    else{
        main::EquipmentUpgradeRequest *request = new main::EquipmentUpgradeRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_ADVANCE, request);
    }
    
}
void SGAdvanceBox::advanceOfficerListener(CCObject *obj)
{       
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::OfficerUpgradeResponse *response = (main::OfficerUpgradeResponse *)sr->m_msg;
    if (sr)
    {
        main::OfficerCardItemProto proto = response->officercarditemproto();
        if (response->state() == 1)
        {
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            playerInfo->getPropCards()->removeObjectsInArray(reqCards);

            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - officer->getCost());
            SGMainManager::shareMain()->updataUserMsg();
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showGeneralInfoLayer(officer,NULL);
            
        }
        else {
        
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showMessage("shibai");
        }
    }    
}
void SGAdvanceBox::advanceEquipListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::EquipmentUpgradeResponse *response = (main::EquipmentUpgradeResponse *)sr->m_msg;
    if (sr)
    {
        main::EquipmentCardItemProto cardInfo = response->equipmentcarditemproto();
        if (response->state() == 1)
        {            
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            playerInfo->getPropCards()->removeObjectsInArray(reqCards);
            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - equip->getCost());
            SGMainManager::shareMain()->updataUserMsg();
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showEquipInfoLayer(equip,0);
            EFFECT_PLAY(MUSIC_35);
        }
        else {            
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showMessage("shibai");
        }
    }    
}
