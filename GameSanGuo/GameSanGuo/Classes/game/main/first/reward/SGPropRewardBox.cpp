//
//  SGPropRewardBox.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-31.
//
//

#include "SGPropRewardBox.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"
SGPropRewardBox::SGPropRewardBox(){}
SGPropRewardBox::~SGPropRewardBox(){

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_propRewardBox);

}

SGPropRewardBox * SGPropRewardBox::create(SGBoxDelegate *delegate, CCString *giftName,int itemID,int type){
    
    SGPropRewardBox *rewardBox=new SGPropRewardBox();
    if (rewardBox) {
        
        rewardBox->data=CCString::create("Prop");
        rewardBox->initView(delegate,giftName,itemID,type);
        rewardBox->autorelease();
        return rewardBox;
    }
    CC_SAFE_DELETE(rewardBox);
    return NULL;
}
void SGPropRewardBox::initView(SGBoxDelegate *delegate, CCString *giftName,int itemID,int type)
{
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_propRewardBox);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_propRewardBox);

    float hgt = 320;
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (!SGBaseBox::init(delegate,box_rewordCoins,CCSizeMake(535, hgt)))
    {
        return;
    }
    fontbg->setPreferredSize(CCSizeMake(480, hgt-163));
    fontbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-10, 20)));
    
    
 
  
    CCString *headStr;
    CCSprite *kuangSprite;
    if (type==0) {
        
        

        SGOfficerDataModel *card=SGStaticDataManager::shareStatic()->getOfficerById(itemID);
        SGMainManager::shareMain()->addHeadIconPlistByNum(card->getIconId(),sg_propRewardBox);
        
        headStr=CCString::createWithFormat("head%d.png",itemID);
         int starlvl=card->getOfficerCurrStarLevel();
        
        if (starlvl>1) {
            starlvl-=1;
        }
        
        kuangSprite= CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
        this->addChild(kuangSprite);
        kuangSprite->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-150, 3)));

    }else if(type ==1)
    {
        

        SGEquipmentDataModel *card=SGStaticDataManager::shareStatic()->getEquipById(itemID);
        SGMainManager::shareMain()->addHeadIconPlistByNum(card->getIconId(),sg_propRewardBox);
        
        headStr=CCString::createWithFormat("head%d.png",itemID);
        int starlvl=card->getEquipCurrStarLevel();
        
        if (starlvl>1) {
            starlvl-=1;
        }
        
        kuangSprite= CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
        this->addChild(kuangSprite);
        kuangSprite->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-150, 3)));

    }
    
    CCSprite *giftSprite=CCSprite::createWithSpriteFrameName(headStr->getCString());
    this->addChild(giftSprite);
    giftSprite->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-150, 3)));
    
    SGCCLabelTTF *rewardLabel = SGCCLabelTTF::create(str_LiandanLayer_str5,FONT_BOXINFO, 30 , ccYELLOW);
    this->addChild(rewardLabel);
    //modify by:zyc. merge into create.
    //rewardLabel->setColor(ccYELLOW);
    rewardLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-105, 80)));
    
    SGCCLabelTTF *coinLabel = SGCCLabelTTF::create(giftName->getCString(),FONT_BOXINFO, 35);
    this->addChild(coinLabel);
    coinLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 10)));
    
    
    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGPropRewardBox ::boxClose),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(center, ccp(0, -hgt/2+60 )));
}
void SGPropRewardBox::boxClose(){
    SGBaseBox::boxClose();
   // SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//    this->boxCloseWithOutCallBack();
//    mainScene->removeChildByTag(sg_totalLoginReward);
    
    
}
