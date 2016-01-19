//
//  SGCoinRewardBox.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-31.
//
//

#include "SGCoinRewardBox.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGCoinRewardBox::SGCoinRewardBox(){}
SGCoinRewardBox::~SGCoinRewardBox(){

     ResourceManager::sharedInstance()->unBindLayerTexture(sg_coinRewardBox);

}

SGCoinRewardBox * SGCoinRewardBox::create(SGBoxDelegate *delegate, int coinNum){
    
    SGCoinRewardBox *rewardBox=new SGCoinRewardBox();
    if (rewardBox) {
         rewardBox->data=CCString::create("coin");
        rewardBox->initView(delegate,coinNum);
        rewardBox->autorelease();
        return rewardBox;
    }
    CC_SAFE_DELETE(rewardBox);
    return NULL;
}
void SGCoinRewardBox::initView(SGBoxDelegate *delegate, int coinNum){
    
    
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_coinRewardBox, LIM_PNG_AS_PNG);
    
    float hgt = 320;
    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (!SGBaseBox::init(delegate,box_rewordCoins,CCSizeMake(535, hgt)))
    {
        return;
    }
    fontbg->setPreferredSize(CCSizeMake(480, hgt-153));
    fontbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 35)));
    
    CCSprite *coinSprite=CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(coinSprite);
    coinSprite->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-100, 50)));
    
    SGCCLabelTTF *rewardLabel = SGCCLabelTTF::create(str_CoinRewardBox_str1,FONT_PANGWA, 30 , ccYELLOW);
    this->addChild(rewardLabel);
    //modify by:zyc. merge into create.
    //rewardLabel->setColor(ccYELLOW);
    rewardLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-95, 100)));
    
    SGCCLabelTTF *coinLabel = SGCCLabelTTF::create(CCString::createWithFormat("%d",coinNum)->getCString(),FONT_PANGWA, 35);
    this->addChild(coinLabel);
    coinLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 50)));
    
    SGCCLabelTTF *font = SGCCLabelTTF::create(str_CoinRewardBox_str2,FONT_PANGWA, 40);
    this->addChild(font);
    font->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -fontbg->getContentSize().height*.5f +30)));
    SGButton *ok = SGButton::create("box_btn_newbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGCoinRewardBox ::boxClose),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
     ok->setPosition(ccpAdd(center, ccp(0, -hgt/2+60 )));
}
void SGCoinRewardBox::boxClose(){
     SGBaseBox::boxClose();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGTotalLoginRewardLayer *loginLayer = (SGTotalLoginRewardLayer *)mainScene->getChildByTag(sg_totalLoginReward);
    if (loginLayer) {
        loginLayer->setIsCanTouch(true);
    }
}
