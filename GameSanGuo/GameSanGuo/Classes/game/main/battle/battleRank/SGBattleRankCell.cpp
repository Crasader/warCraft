//
//  SGBattleRankCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-16.
//
//

#include "SGBattleRankCell.h"
#include "SGLayout.h"
#include "cocos-ext.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
USING_NS_CC_EXT;

SGBattleRankCell::SGBattleRankCell(void)
{
    spriteBg = NULL;
    spriteIcon = NULL;
    labelNickName = NULL;
    labelRank = NULL;
    labelFTime = NULL;
    labelLevel = NULL;
    labelWinR = NULL;
    bgSize = CCSizeMake(0, 0);
    spriteBg = NULL;
}

SGBattleRankCell::~SGBattleRankCell(void)
{
}

SGBattleRankCell* SGBattleRankCell::create()
{
    SGBattleRankCell *baseCell = new SGBattleRankCell();
    if (baseCell && baseCell->init()) {
        baseCell->initBackground();
        baseCell->initView();
        baseCell->autorelease();
        return baseCell;
    }
    CC_SAFE_DELETE(baseCell);
    return NULL;
}

void SGBattleRankCell::initBackground()
{
    spriteBg = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    spriteBg->setScaleX(skewing(300)/spriteBg->getContentSize().width);
    spriteBg->setScaleY(skewing(65)/spriteBg->getContentSize().height);

    this->addChild(spriteBg, -10);
    
    CCScale9Sprite *spriteBorder = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    spriteBorder->setPreferredSize(CCSizeMake(skewing(300), skewing(65)));

    this->addChild(spriteBorder, -10);
  
    
   // bgSize = CCSizeMake(600, 125);
    bgSize=CCSizeMake(skewing(300), skewing(65));
    float viewWid = bgSize.width;
    float viewH = bgSize.height;
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(viewWid, viewH));
    this->addChild(frame2);
    frame2->setPosition(spriteBg->getPosition());
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg, -1);
    frame2bg->setScaleX(viewWid/frame2bg->getContentSize().width);
    frame2bg->setScaleY(viewH/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2l, -1);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2r, -1);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang2m, -1);
    guang2m->setScaleX((viewWid-120)/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(viewWid*.5f, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-viewWid*.5f, 0)));
}

void SGBattleRankCell::resetBgImg(int index)
{
    if (index%2 == 0) { // 重置背景
//        spriteBg->setDisplayFrame(CCSprite::create("login/server_GreenBG.png")->displayFrame());
    }
}

void SGBattleRankCell::initView()
{
    bgbgbg = NULL;
    spriteIconBg = CCSprite::createWithSpriteFrameName("jinkuang.png");
    spriteIconBg->setPosition(ccp(-bgSize.width/2+spriteIconBg->getContentSize().width/2+35, 0));
    this->addChild(spriteIconBg, 1);
    
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_rankLayer);
    spriteIcon = CCSprite::createWithSpriteFrameName("head1001.png");
    spriteIcon->setPosition(ccpAdd(spriteIconBg->getPosition(),ccp(0,-10)));//gai yuan wu add only getposition
    this->addChild(spriteIcon);
    
    // rank labels
    float fontSize = FONTSIZE(13);
    const char* fontName = FONT_BOXINFO;
    
    float iconW = spriteIcon->getContentSize().width/2 + 25;
    CCString *string = CCString::create(str_BMatchEndBox_str2);
    labelNickName = SGCCLabelTTF::create(string->getCString(),  fontName, FONTSIZE(14));
    labelNickName->setAnchorPoint(ccp(0, 0.5f));
    labelNickName->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(iconW, 32)));
    this->addChild(labelNickName);
    
    string = CCString::createWithFormat(str_BMatchEndBox_str14, 1);
    labelRank = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelRank->setAnchorPoint(ccp(0, 0.5f));
    labelRank->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(iconW, -5)));
    this->addChild(labelRank);
    
    string = CCString::createWithFormat(str_BMatchEndBox_str15, 865);
    labelFTime = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelFTime->setAnchorPoint(ccp(0, 0.5f));
    labelFTime->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(iconW, -35)));
    this->addChild(labelFTime);
    
    iconW = bgSize.width*.5f - 25;
    string = CCString::createWithFormat(str_BattleRankCell_str1, 156);
    labelLevel = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelLevel->setAnchorPoint(ccp(0, 0.5f));
    labelLevel->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(iconW, -5)));
    this->addChild(labelLevel);
    
    string = CCString::createWithFormat(str_BMatchEndBox_str10, 0.8525*100);
    labelWinR = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelWinR->setAnchorPoint(ccp(0, 0.5f));
    labelWinR->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(iconW, -35)));
    this->addChild(labelWinR);
}

void SGBattleRankCell::setData(SGRankPlayer *player)
{
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    
    CCString *string = CCString::createWithFormat("countryIconbig_%d.png", player->getHeadId());
    spriteIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    spriteIcon->cocos2d::CCNode::setScale(0.75, 0.8);
    string = CCString::createWithFormat("country_%d.png", player->getHeadId());
    spriteIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    
    labelNickName->setString(player->getName()->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str14, player->getRank());
    labelRank->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str15, player->getBcount());
    labelFTime->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BattleRankCell_str1, player->getLevel());
    labelLevel->setString(string->getCString());
    
    string = CCString::createWithFormat(str_BMatchEndBox_str10, player->getWinRate());
    labelWinR->setString(string->getCString());
}
