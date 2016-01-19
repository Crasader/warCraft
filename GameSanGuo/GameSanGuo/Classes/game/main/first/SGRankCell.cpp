//
//  SGRankCell.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-11.
//
//

#include "SGRankCell.h"
#include "ResourceManager.h"
#include "cocos-ext.h"
#include "SGLayout.h"
#include "cocos-ext.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGRankAllPlayer.h"
#include "SGRankDetailLayer.h"
#include "SGFriend.h"
#include "SGStringConfig.h"

SGRankCell::SGRankCell()
:bgSize(CCSizeZero),
spriteBg(NULL),
spriteIconBg(NULL),
rankrewardpng(NULL),
spriteIcon(NULL),   // 头像
labelNickName(NULL),  // 昵称
labelRank(NULL),
ranknum(NULL),
labelshanghai(NULL),
typetype(0),
playid(0),
nation(0),
detail(NULL),
enterType(0)
{
    typetype=-10;
    
}
SGRankCell:: ~SGRankCell()
{
    
}


SGRankCell* SGRankCell::create(int playid, int enter)
{
    
    SGRankCell *baseCell = new SGRankCell();
    if (baseCell && baseCell->init()) {

		baseCell->enterType = enter;
        baseCell->initView(playid);
        baseCell->autorelease();
        return baseCell;
    }
    CC_SAFE_DELETE(baseCell);
    return NULL;
}




void SGRankCell::initView(int playid)
{
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_rank);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_rank);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_rank);
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    
    spriteBg = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    spriteBg->setScaleX(skewing(300)/spriteBg->getContentSize().width);
    spriteBg->setScaleY(skewing(65)/spriteBg->getContentSize().height);

    bgSize=CCSizeMake(skewing(300), skewing(65));
    float viewWid = bgSize.width;
    float viewH = bgSize.height;
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(viewWid, viewH));
    this->addChild(frame2);

    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg, -1);
    frame2bg->setScaleX(viewWid/frame2bg->getContentSize().width);
    frame2bg->setScaleY(viewH/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
    spriteIconBg = CCSprite::createWithSpriteFrameName("jinkuang.png");
    spriteIconBg->setPosition(ccp(-skewing(300)*0.32, 0));
   
    this->addChild(spriteIconBg);

    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_rankLayer);
    spriteIcon = CCSprite::createWithSpriteFrameName("head1001.png");
   
    spriteIcon->setPosition(ccpAdd(spriteIconBg->getPosition(),ccp(0,0)));
    this->addChild(spriteIcon);
    
    float fontSize = FONTSIZE(13);
    const char* fontName = FONT_BOXINFO;
    
    float changdu = spriteIconBg->getContentSize().width;
    CCString *string = CCString::create(str_MainTaskLayer_str2);
    labelNickName = SGCCLabelTTF::create(string->getCString(),  fontName, FONTSIZE(14) , ccc3(0xff, 0xc0, 0x00));
    labelNickName->setAnchorPoint(ccp(0, 0.5f));
    labelNickName->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*0.6, changdu*0.3)));
    this->addChild(labelNickName);
    //modify by:zyc. merge into create.
    //labelNickName->setColor(ccc3(0xff, 0xc0, 0x00));
    
    CCSprite*namebg=CCSprite::createWithSpriteFrameName("name_bg.png");
    namebg->setAnchorPoint(ccp(0, 0.5f));
    namebg->setPosition(labelNickName->getPosition());
    this->addChild(namebg,-5);

    string = CCString::createWithFormat(str_ShowJingZhouRankCell_str3, 0);
    labelshanghai = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelshanghai->setAnchorPoint(ccp(0, 0.5f));
    labelshanghai->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*0.6, -changdu*0.3)));
    this->addChild(labelshanghai);
    

    CCMenu*menu2=CCMenu::create();
    this->addChild(menu2);
    menu2->setPosition(CCPointZero);
    menu2->setTouchPriority(10);
    
   detail = SGButton::createFromLocal("unhot.png", str_BossRankItem_str2, this, menu_selector(SGRankCell::sendrequest),CCPointZero,FONT_BOXINFO,ccWHITE,32);

    
    ////锚点换了坐标也换
     detail->setPosition(ccp(frame2->getPositionX()+frame2->getContentSize().width*0.5-detail->getContentSize().width*0.2-detail->getContentSize().height*0.5-20, spriteIcon->getPositionY()-detail->getContentSize().height*0.2));
    addBtn(detail);
   // detail->setTag(playid);
    this->addChild(detail,999);
    detail->setFontColor(ccc3(0xf3,0x89,00));
   // menu2->addChild(detail);
    
    
    
    if (enterType == 3)//boss战进入
	{
		string = CCString::createWithFormat(str_BossRankItem_str3, 50);
	}
	else
	{
		string = CCString::createWithFormat(str_pvpLastListLayer_str5, 123456,str_ShowJingZhouRankCell_str1);
	}
//    string = CCString::createWithFormat("%d服-%s", 123456,"呵呵");
    labelRank = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelRank->setAnchorPoint(ccp(1, 0.5f));
    labelRank->setPosition(ccpAdd(detail->getPosition(), ccp(0, detail->getContentSize().height*0.7)));
    this->addChild(labelRank);
    //////前三名奖励图片
    rankrewardpng=CCSprite::createWithSpriteFrameName("pvpno1.png");
    rankrewardpng->setAnchorPoint(ccp(1, 0.5));
    rankrewardpng->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(-spriteIconBg->getContentSize().width*0.3, -spriteIconBg->getContentSize().height*0.2)));
    rankrewardpng->setScale(1.2);
    this->addChild(rankrewardpng,5);
    rankrewardpng->setVisible(false);
    
    ranknum = SGCCLabelTTF::create("0",  fontName, FONTSIZE(18));
    ranknum->setAnchorPoint(ccp(1, 0.5));
    ranknum->setPosition(ccpAdd(rankrewardpng->getPosition(), ccp(-rankrewardpng->getContentSize().width*0.3, spriteIconBg->getContentSize().height*0.15)));
    this->addChild(ranknum);

}

void SGRankCell::setData(SGRankAllPlayer *player,int type,int rank)
{
    
    typetype=type;
    nation=player->getHeadId();
    detail->setTag(player->getplayerId());
    ////////////////////
    
    
   // CCLog("%d",player->getplayerId());
   // CCLog("%d",player->getplayerId());
   // CCLog("%d",player->getplayerId());
   // CCLog("%d",player->getplayerId());
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
    ////服务器名称
    CCString *servername= CCString::createWithFormat("%s", player->getServerName()->getCString());
   
    
    if(type==0)
    {
    CCString* serverlabel = CCString::createWithFormat(str_pvpLastListLayer_str5,player->getServerNum(), servername->getCString());
    //CCString* paiming = CCString::createWithFormat("排名：%d", player->getRank());
    labelRank->setString(serverlabel->getCString());
    }
   else
   {
       labelRank->setVisible(false);
   }
   
    CCString* shanghai;
    if(type==0||type==1)
    {
        shanghai = CCString::createWithFormat(str_MainTaskLayer_str3,  player->getRank());
    }

    else if(type==2)
    {
        shanghai = CCString::createWithFormat(str_MainTaskLayer_str4,  player->getRank());
    }
    else if(type==3)
    {
        shanghai = CCString::createWithFormat(str_MainTaskLayer_str5,  player->getRank());
    }
    else if(type==4)
    {
        shanghai = CCString::createWithFormat(str_MainTaskLayer_str6,  player->getRank());
    }
    else if(type==5)
    {
        shanghai = CCString::createWithFormat(str_MainTaskLayer_str7,  player->getRank());
    }
    labelshanghai->setString(shanghai->getCString());
    
    CCString*rankrank= CCString::createWithFormat("%d", rank);
    ranknum->setString(rankrank->getCString());
    if(rank<=3)
    {
        ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_rank);
  
        rankrewardpng->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("pvpno%d.png", rank)->getCString())->displayFrame());
        rankrewardpng->setVisible(true);
    }
    else
    {
        rankrewardpng->setVisible(false);
    }
}

void SGRankCell::sendrequest(CCNode*node)
{
    
    if(typetype==0)
    {
        SGMainManager::shareMain()->showMessage(str_MainTaskLayer_str8);
        return;
    }
    playid=node->getTag();
    
    //////send  message
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    player->setplayerID(playid);

    main::FriendInfoRequest *info = new main::FriendInfoRequest();
    info->set_friendid(playid);
    if(typetype==0)
    {
        info->set_type(1);
    }
    else
    {
        info->set_type(0);
    }

    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);

}









