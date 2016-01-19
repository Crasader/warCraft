//
//  SGSellBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-27.
//
//

#include "SGSellBox.h"
#include "SGMainManager.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "Itemmodule.pb.h"
#include "SGPlayerInfo.h"
#include "SGMainLayer.h"
#include "SGStaticDataManager.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGSellBox::SGSellBox()
:totlesell(0),
enterType(0),
datas(NULL)

{
}
SGSellBox::~SGSellBox()
{
 
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_BARRACK_SELL);
    datas->release();
    
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_sellBox);

}
SGSellBox *SGSellBox::create(SGBoxDelegate *delegate,SGBaseMilitaryCard *card,int type)
{
    SGSellBox *sellBox = new SGSellBox();    
 
    if (sellBox)
    {
        sellBox->enterType = type;
        sellBox->_card = card;
        sellBox->initView(delegate);
        sellBox->autorelease();
        return sellBox;
    }
    CC_SAFE_DELETE(sellBox);
    return NULL;
}
        
void SGSellBox::initView(SGBoxDelegate *dele)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_sellBox);
 
    float hgt = 330;
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    datas = CCArray::create();
    datas->retain();
    datas->addObject(_card);
    
    CCString *str_ = CCString::create(_card->getClassName());
    if(str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerCard *card = (SGOfficerCard *)_card;
        if (card->getOfficerEquips())
        {
            datas->addObjectsFromArray(card->getOfficerEquips());
            for (int i = 0; i<card->getOfficerEquips()->count(); i++) {
                hgt+= 130;
            }
        }
    }


    if (!SGBaseBox::init(dele,box_sellBoxTag,CCSizeMake(535, hgt)))
    {
        return;
    }
    fontbg->setPreferredSize(CCSizeMake(480, hgt-163));
    fontbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 20)));//40

    SGCCLabelTTF *font = SGCCLabelTTF::create(str_SellBox_str1,FONT_BOXINFO, 30);
    this->addChild(font);
    font->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -fontbg->getContentSize().height * 0.85f)));
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BARRACK_SELL,
                              this,
                              callfuncO_selector(SGSellBox::sellListener));
        
    CCSprite *title = CCSprite::createWithSpriteFrameName("font_sellqr.png");
    title->setPosition(ccpAdd( biao->getPosition(), ccp(0,10)));
    this->addChild(title);

    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGSellBox::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
        
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGSellBox::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    ok->setPosition(ccpAdd(center, ccp(-145, -hgt/2+5 -40)));
    cancel->setPosition(ccpAdd(center, ccp(145, -hgt/2+5-40)));
    
    CCObject *obj = NULL;
    int base = 0;
    CCARRAY_FOREACH(datas, obj)
    {
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)obj;
    CCString *str_ = CCString::create(card->getClassName());
    if (str_->isEqual(CCString::create("SGPropsCard")))
    {
            totlesell = card->getSell();
    }
    else{
    switch (card->getCurrStar()) {
            CCLOG(".,.,., %d",card->getCurrStar());
        case 1:
            base = 50;
            break;
        case 2:
            base = 250;
            break;
        case 3:
            base = 500;
            break;
        case 4:
            base = 750;
            break;
        case 5:
            base = 1500;
            break;
        case 6:
            base = 2500;
            break;
            
        default:
            break;
            }
        int a  = base * ((card->getCurrLevel() - 1) *.1 + 1);
        totlesell += a;
        }
    }
    if (datas) {
        for (int i = 0; i<datas->count(); i++)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(i);
            Sell_Item *item = item = Sell_Item::create(card);
            if (item) {
                this->addChild(item
                               );
                item->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.5f - 80 - 130*i)));
            }
            
        }
    }

//    Sell_Item *item = (Sell_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
//    SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(indexPath->getRow());
//    if (NULL == item) {
//        item = Sell_Item::create(card);
//    }
//    else
//    {
//        item->updateOfficerCard(card,indexPath->getRow());
//    }

}

void SGSellBox::showGenerInfo()
{

}
void SGSellBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
    SGBaseBox::boxClose();
}
void SGSellBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::SellRoleItemRequest *request = new main::SellRoleItemRequest();
    request->set_ssid(_card->getSsid());
    request->set_itemid(_card->getItemId());
    SGSocketClient::sharedSocketClient()->send(MSG_BARRACK_SELL, request);
    
}
void SGSellBox::sellListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::SellRoleItemResponse *response = (main::SellRoleItemResponse *)sr->m_msg;
    if (sr)
    {
    if (response->state() == 1)
        {
            
            EFFECT_PLAY(MUSIC_34);
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();                     
            
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_SellBox_str2,totlesell)->getCString());

            
            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() + totlesell);
            SGMainManager::shareMain()->updataUserMsg();
            
            CCString *str_ = CCString::create(_card->getClassName());
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    playerInfo->getPropCards()->removeObject(_card);
                    
                    if (enterType == 2) {
                        this->boxCloseWithOutCallBack();
                        SGMainManager::shareMain()->showGeneralsLayer(NULL);
                    }else if (enterType == 3) {
                        this->boxCloseWithOutCallBack();
                        SGMainManager::shareMain()->showEquipsLayer(NULL,0,0);
                    }else if (enterType == 4) {
                        this->boxCloseWithOutCallBack();
                        SGMainManager::shareMain()->showSoldierslayer();
                    }else{
                        this->boxCloseWithOutCallBack();
                        SGMainManager::shareMain()->showSkillCardsLayer(10);}
                }
                else if(str_->isEqual(CCString::create("SGOfficerCard")))
                {
                    SGOfficerCard *card = (SGOfficerCard *)_card;

                    if (card->getOfficerEquips()) {
                        playerInfo->getEquipCards()->removeObjectsInArray(card->getOfficerEquips());            
                    }
                    playerInfo->getOfficerCards()->removeObject(_card);
                    this->boxCloseWithOutCallBack();
                     SGMainManager::shareMain()->showGeneralsLayer(NULL);
                }else if(str_->isEqual(CCString::create("SGEquipCard")))
                {                 
                    SGEquipCard *card = (SGEquipCard *)_card;
                    if (card->getOfficerCardId()) {
                         playerInfo->getOfficerCard(card->getOfficerCardId())->getOfficerEquips()->removeObject(_card);
                    }                   
                    playerInfo->getEquipCards()->removeObject(_card);
//                    this->boxCloseWithOutCallBack();
                    SGMainManager::shareMain()->showEquipsLayer(NULL,0,0);
                }
        }
    else{        
        SGMainManager::shareMain()->showMessage("shibai");
//        this->boxCloseWithOutCallBack();
       }
        SGMainManager::shareMain()->closeBox();
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        if (mainLayer) {
            mainLayer->setIsCanTouch(true);
        }
    }
   
}


#pragma mark - Sell_Item Class

Sell_Item::Sell_Item():
_card(NULL),
spritePlayerHead(NULL),
sellCoins(0)
{
}
Sell_Item::~Sell_Item()
{

}
void Sell_Item::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_sellBox);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_sellBox, LIM_PNG_AS_PNG);
    
    CCString *str_ = CCString::create(_card->getClassName());
    
    CCSprite * kuang = NULL;
    bgbgbg = NULL;
    
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
        if (kuang)
        {
            kuang->removeChildByTag(30);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(kuang->getContentSize().width*0.355,kuang->getContentSize().height * 0.83f));
            kuang->addChild(countryName,30,30);
        }
        
    }else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
    }else
    {
        SGPropsCard *card = (SGPropsCard *)_card;
        if (card->getType() == 1) {
            
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
            kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",temp->getPropsCountry())->getCString());
        }
        else if (card->getType() == 4) {
            kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4.png");
        }
        else
        {
            kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
        }
    }
    if (kuang) {
        this->addChild(kuang,1);
        kuang->setPosition(ccpAdd(kuang->getPosition(), ccp(-180 + kuang->getContentSize().width/2, 0)));
    }

//    SGMainManager::shareMain() -> addHeadIconPlistByNum(_card->getItemId(),sg_sellBox);
//      
//    spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",_card->getItemId())->getCString());
	//12.06
	SGMainManager::shareMain() -> addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_sellBox);
	
    spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString());
	
	
    this->addChild(spritePlayerHead);
    if (kuang && spritePlayerHead) {
        spritePlayerHead->setPosition(kuang->getPosition());
    }
    
    SGCCLabelTTF* labelName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_BOXINFO, 32);
    labelName->setAnchorPoint(ccp(0, 1));
    this->addChild(labelName);    
    
    SGCCLabelTTF* labelLevel = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,_card->getCurrLevel())->getCString(), FONT_PANGWA, 24);
    this->addChild(labelLevel,10);

    if (str_->isEqual(CCString::create("SGPropsCard")))
    {
        labelLevel->setVisible(false);
    }
    
    CCSprite *cardbj = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(cardbj,-1);
    
    if (kuang) {
        
        labelName->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width * 0.6f, kuang->getContentSize().height * 0.45f)));
        labelLevel->setPosition(ccpAdd(kuang->getPosition(), ccp(0,-35)));
        cardbj->setPosition(kuang->getPosition());
    }
    
    this->showPlayerStar();
}

//设置星星数量
void Sell_Item::showPlayerStar()
{
     ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_sellBox, LIM_PNG_AS_PNG);
    for(int i=0; i < _card->getCurrStar();i++)
    {
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("xx.png");
        sprite->setAnchorPoint(ccp(0, 0.5));
        sprite->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(108*.6 + sprite->getContentSize().width*i,0)));
        this->addChild(sprite);        
    }
    int base = 0;
    CCString *str_ = CCString::create(_card->getClassName());
    if (str_->isEqual(CCString::create("SGPropsCard")))
    {
        sellCoins = _card->getSell();
    }else{
        switch (_card->getCurrStar()) {
        case 1:
            base = 50;
            break;
        case 2:
            base = 250;
            break;
        case 3:
            base = 500;
            break;
        case 4:
            base = 750;
            break;
        case 5:
            base = 1500;
            break;            
        case 6:
            base = 2500;
            break;
            
        default:
            break;
    }
        sellCoins = base * ((_card->getCurrLevel() - 1) *.1 + 1);
    }
    SGCCLabelTTF *font = SGCCLabelTTF::create(str_jiage, FONT_BOXINFO, 32 , COLOR_YELLOW);
    font->setAnchorPoint(ccp(0, 0));
    //modify by:zyc. merge into create.
    //font->setColor(COLOR_YELLOW);
    font->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(108*.6, -108/2)));
    this->addChild(font);
    
    CCSprite *sp = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(sp);
    sp->setPosition(ccpAdd(font->getPosition(), ccp(1+font->getContentSize().width + sp->getContentSize().width*.5f, sp->getContentSize().height*.5f)));
    
    SGCCLabelTTF *sell = SGCCLabelTTF::create(CCString::createWithFormat("%d",sellCoins)->getCString(), FONT_BOXINFO, 32 , COLOR_YELLOW);
    sell->setAnchorPoint(ccp(0, 0.5f));
    //modify by:zyc. merge into create.
    //sell->setColor(COLOR_YELLOW);
    sell->setPosition(ccpAdd(sp->getPosition(), ccp(10+sp->getContentSize().width*.5f,0)));
    this->addChild(sell);
    
}

Sell_Item* Sell_Item::create(SGBaseMilitaryCard *card)
{
	Sell_Item *instance = new Sell_Item();
    instance->_card = card;
	if (instance && instance->init()) {
        
        instance->initView();
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

