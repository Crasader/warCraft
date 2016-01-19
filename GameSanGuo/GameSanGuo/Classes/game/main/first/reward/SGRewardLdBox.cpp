//
//  SGRewardLdBox.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-6.
//
//

#include "SGRewardLdBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGRewardLdBox *SGRewardLdBox::create(SGBoxDelegate *bdg,SGBaseMilitaryCard *card,int type)
{
    SGRewardLdBox *noExpBox = new SGRewardLdBox();
    if (noExpBox && noExpBox->init(bdg,card))
    {
        noExpBox->entertype = type;
        noExpBox->autorelease();
        return noExpBox;
    }
    CC_SAFE_DELETE(noExpBox);
    return NULL;
}
SGRewardLdBox::SGRewardLdBox()
:entertype(0)
{
    
}
SGRewardLdBox::~SGRewardLdBox(){
  
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_rewardLdBox);
    
}

bool SGRewardLdBox::init(SGBoxDelegate *bdg,SGBaseMilitaryCard *card)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_rewardLdBox);
    
    if (!SGBaseBox::init(bdg,box_rewardLdTag,CCSizeMake(535, 400),true))//295
    {
        return false;
    }
    
    SGButton *ok = SGButton::create("box_btn_newbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGRewardLdBox::boxCloseWithOutCallBack),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,-150)));
	
	//12.06
	SGMainManager::shareMain() -> addHeadIconPlistByNum(card->getHeadSuffixNum(),sg_rewardLdBox);
    CCSprite *icon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",card->getHeadSuffixNum())->getCString());
	
    this->addChild(icon,101);
    icon->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -23)));//-10原来
    
    CCSprite *cardbj = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(cardbj,50);
    cardbj->setPosition(icon->getPosition());
    
    if (entertype == 1)
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        CCSprite *kuang = CCSprite::create(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
        this->addChild(kuang,101);
        kuang->setPosition(icon->getPosition());
        
        CCSprite *boxtitle = CCSprite::createWithSpriteFrameName("font_lzcg.png");
        this->addChild(boxtitle,102);
        boxtitle->setPosition(biao->getPosition());

        //if (str_1->isEqual(CCString::create("SGOfficerCard")))
        {
            kuang->removeChildByTag(30);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(kuang->getContentSize().width*0.3,kuang->getContentSize().height - countryName->getContentSize().height/4));
            kuang->addChild(countryName,30,30);
            if (temp->getAdvNum()>0)
            {
                kuang->removeChildByTag(250);
                SGCCLabelTTF *ad = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                ad->setInsideColor(ccGREEN);
                ad->setAnchorPoint(ccp(1,1));
                ad->ignoreAnchorPointForPosition(false);
                ad->setPosition(ccp(kuang->getContentSize().width*0.8,kuang->getContentSize().height - ad->getContentSize().height));
                kuang->addChild(ad,250,250);
            }
        }
        
    }
    else
    {
        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());

        CCSprite *kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getPropsCurrStarLevel()-1)->getCString());
        this->addChild(kuang,101);
        kuang->setPosition(icon->getPosition());
    }
    SGCCLabelTTF*label = NULL;
   
    label = SGCCLabelTTF::create(str_SoulExchangeLayer_str10, FONT_PANGWA, 40 ,ccWHITE , ccc3(143, 87, 0));
    label->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 150)));
    this->addChild(label);
  
    SGCCLabelTTF *a = SGCCLabelTTF::create(CCString::createWithFormat(str_RewardBindLayer_str16,card->getOfficerName()->getCString())->getCString(), FONT_BOXINFO, 32);
    this->addChild(a,101);
    a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 60)));//80原来
    
        
    return true;
}

void SGRewardLdBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_34);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
    }
    SGBaseBox::boxCloseWithOutCallBack();
    SGMainManager::shareMain()->showRewardsLayer();
}