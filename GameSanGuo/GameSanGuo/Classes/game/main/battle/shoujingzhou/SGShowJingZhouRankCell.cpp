//
//  SGShowJingZhouRankCell.cpp
//  GameSanGuo
//
//  Created by kanata on 13-11-14.
//
//

#include "SGShowJingZhouRankCell.h"
#include "ResourceManager.h"
#include "cocos-ext.h"
#include "SGLayout.h"
#include "cocos-ext.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGShowJingZhouRankPlayerLayer.h"
#include "SGStringConfig.h"

SGShowJingZhouRankCell::SGShowJingZhouRankCell()
:goldBorderSpr(NULL),
goldCount(NULL),
goldBg(NULL),
goldSpr(NULL),
goldCountBg(NULL)
{

}
SGShowJingZhouRankCell:: ~SGShowJingZhouRankCell()
{
ResourceManager::sharedInstance()->unBindLayerTexture(sg_sjzrank);
}


SGShowJingZhouRankCell* SGShowJingZhouRankCell::create()
{

   SGShowJingZhouRankCell *baseCell = new SGShowJingZhouRankCell();
   if (baseCell && baseCell->init()) {
      baseCell->initBackground();
      baseCell->initView();
      baseCell->autorelease();
      return baseCell;
     }
     CC_SAFE_DELETE(baseCell);
    return NULL;
}

void SGShowJingZhouRankCell::initBackground()
{
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    
    spriteBg = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    spriteBg->setScaleX(skewing(300)/spriteBg->getContentSize().width);
    spriteBg->setScaleY(skewing(65)/spriteBg->getContentSize().height);
    this->addChild(spriteBg, -10);
    spriteBg->setVisible(false);
    /////
    bgSize=CCSizeMake(skewing(300), skewing(65));
    float viewWid = bgSize.width;
    
  

    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(skewing(300), skewing(55)));
    this->addChild(frame2);
    frame2->setPosition(spriteBg->getPosition());

    ////
    CCSprite *spriteBorder = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
   // spriteBorder->setPreferredSize(CCSizeMake(skewing(300), skewing(55)));
    
    
    spriteBorder->setScaleX(frame2->getContentSize().width/spriteBorder->getContentSize().width);
    spriteBorder->setScaleY(frame2->getContentSize().height/spriteBorder->getContentSize().height);
    
    
    spriteBorder->setPosition(spriteBg->getPosition());
    this->addChild(spriteBorder, -10);
    
//    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
//    this->addChild(frame2bg, -1);
//    frame2bg->setScaleX(viewWid/frame2bg->getContentSize().width);
//    frame2bg->setScaleY(viewH/frame2bg->getContentSize().height);
//    frame2bg->setPosition(frame2->getPosition());

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


void SGShowJingZhouRankCell::initView()
{
    spriteIconBg = CCSprite::createWithSpriteFrameName("jinkuang.png");
    spriteIconBg->setPosition(ccp(-bgSize.width/2+spriteIconBg->getContentSize().width/2+35, 0));
    this->addChild(spriteIconBg, 1);
    
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_rankLayer);
    spriteIcon = CCSprite::createWithSpriteFrameName("head1001.png");
    spriteIcon->setPosition(ccpAdd(spriteIconBg->getPosition(),ccp(0,0)));
    this->addChild(spriteIcon);
    
    float fontSize = FONTSIZE(13);
    const char* fontName = FONT_BOXINFO;
    
    float changdu = spriteIconBg->getContentSize().width;
    CCString *string = CCString::create(str_ShowJingZhouRankCell_str1);
    labelNickName = SGCCLabelTTF::create(string->getCString(),  fontName, FONTSIZE(14));
    labelNickName->setAnchorPoint(ccp(0, 0.5f));
    labelNickName->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*0.6, changdu*0.3)));
    this->addChild(labelNickName);
    
    string = CCString::createWithFormat(str_BMatchEndBox_str14, 123456);
    labelRank = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelRank->setAnchorPoint(ccp(0, 0.5f));
    labelRank->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*3.5, changdu*0.3)));
    this->addChild(labelRank);
    
    string = CCString::createWithFormat(str_ShowJingZhouRankCell_str2, 1);
    labelboshu = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelboshu->setAnchorPoint(ccp(0, 0.5f));
    labelboshu->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*0.6, changdu*0.0)));
    this->addChild(labelboshu);
    
    string = CCString::createWithFormat(str_ShowJingZhouRankCell_str3, 19910214);
    labelshanghai = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelshanghai->setAnchorPoint(ccp(0, 0.5f));
    labelshanghai->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*0.6, -changdu*0.3)));
    this->addChild(labelshanghai);
	
	//元宝显示
	
	
	goldSpr = CCSprite::createWithSpriteFrameName("big_yb.png");
	goldBorderSpr = CCSprite::createWithSpriteFrameName("equipPinzhi_5_1.png");
	goldBg = CCSprite::createWithSpriteFrameName("card_bj.png");
	
	goldBg->setScale(0.5);
	goldBorderSpr->setScale(0.5);
	goldSpr->setScale(0.5);
	
	this->addChild(goldBorderSpr);
	this->addChild(goldBg);
	this->addChild(goldSpr);
	
	goldBorderSpr->setPosition(ccpAdd(labelRank->getPosition(), ccp(goldBorderSpr->getContentSize().width / 4 - 40, -goldBorderSpr->getContentSize().height / 2.5)));
	goldSpr->setPosition(ccpAdd(goldBorderSpr->getPosition(), ccp(0, 0)));
	goldBg->setPosition(ccpAdd(goldBorderSpr->getPosition(), ccp(0, 0)));
	
	//数量
	goldCount = SGCCLabelTTF::create("X 10", FONT_PANGWA, 20 , ccc3(29, 225, 34));
    //modify by:zyc. merge into create.
	//goldCount->setColor(ccc3(29,225,34));
	goldCount->setAnchorPoint(ccp(0, 0.5));
	this->addChild(goldCount, 0);
	goldCount->setPosition(ccpAdd(goldBorderSpr->getPosition(), ccp(goldBorderSpr->getContentSize().width / 3.2, -10)));

	goldCountBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
	goldCountBg->setScaleX(0.8);
	goldCountBg->setAnchorPoint(ccp(0, 0.5));
	this->addChild(goldCountBg, -1);
	goldCountBg->setPosition(ccpAdd(goldCount->getPosition(), ccp(-10, 0)));
	
}

void SGShowJingZhouRankCell::setData(SGShowJingZhouRankPlayerLayer *player)
{
    CCString *string = CCString::createWithFormat("countryIcon_%d.png", player->getHeadId());
    spriteIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    spriteIcon->cocos2d::CCNode::setScale(0.9, 0.95);
    spriteIcon->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(spriteIcon->getContentSize().width*0.00, spriteIcon->getContentSize().height*0.05)) );
    //////////
    string = CCString::createWithFormat("country_%d.png", player->getHeadId());
    spriteIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    
    ////////////////////

     CCString *name= CCString::createWithFormat("%s", player->getName()->getCString());
     labelNickName->setString(name->getCString());
    
     CCString* paiming = CCString::createWithFormat(str_BMatchEndBox_str14, player->getRank());
     labelRank->setString(paiming->getCString());
    
     CCString* boshu = CCString::createWithFormat(str_ShowJingZhouRankCell_str2, player->getWave());
     labelboshu->setString(boshu->getCString());
    
     CCString* shanghai = CCString::createWithFormat(str_ShowJingZhouRankCell_str3, player->getDamage());
     labelshanghai->setString(shanghai->getCString());
	
	
	//更元宝显示, 是否能得到元宝
	//if (player->getGoldNum())
	{
		//可获得的奖励数量小于可获取奖励的人数时才显示
		goldCount->setString(CCString::createWithFormat("X %d", player->getGoldNum())->getCString());
	}
/*	else//没有元宝就什么都不显示
	{
		goldCount->setVisible(false);
		goldSpr->setVisible(false);
		goldBorderSpr->setVisible(false);
		goldBg->setVisible(false);
	}
	
	*/
	
	
	
 
}
