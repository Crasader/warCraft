//
//  SGBossRankItem.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-27.
//
//

#include "SGBossRankItem.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "AppMacros.h"
#include "SGStringConfig.h"


SGBossRankItem::SGBossRankItem()
:enter(0),
bgSize(CCSize(0.0f, 0.0f)),
spriteBg(NULL),
spriteIconBg(NULL),
rankrewardpng(NULL),
spriteIcon(NULL),  // 头像
labelNickName(NULL), // 昵称
labelRank(NULL),
ranknum(NULL),
labelshanghai(NULL),
detail(NULL),
listDatas(NULL),
roleId(0)
{
}
SGBossRankItem::~SGBossRankItem()
{
	
}

SGBossRankItem *SGBossRankItem::create(SGBossRankListData *listData, int enter)
{
	SGBossRankItem *rankItem = new SGBossRankItem();
	if (rankItem && rankItem->init())
	{
		rankItem->enter = enter;
		rankItem->listDatas = listData;
		rankItem->initView();
		rankItem->autorelease();
		return rankItem;
	}
	CC_SAFE_RELEASE(rankItem);
	return NULL;
}

void SGBossRankItem::initView()
{
//    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_rank);
//	ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_rank);
//	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_rank);
//    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_rank);

    spriteBg = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    spriteBg->setScaleX(skewing(300)/spriteBg->getContentSize().width);
    spriteBg->setScaleY(skewing(65)/spriteBg->getContentSize().height);
	
    bgSize=CCSizeMake(skewing(300), skewing(65));
    float viewWid = bgSize.width;
    float viewH = bgSize.height;
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(viewWid, viewH));
	frame2->setTag(33);
    this->addChild(frame2);
	
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg, -1, 32);
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
    CCString *string = CCString::create(listDatas->roleName);
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
    
    
	
    string = CCString::createWithFormat(str_BossRankItem_str1, listDatas->damage);
    labelshanghai = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelshanghai->setAnchorPoint(ccp(0, 0.5f));
    labelshanghai->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(changdu*0.6, -changdu*0.3)));
    this->addChild(labelshanghai);
    
	
    CCMenu*menu2=CCMenu::create();
    this->addChild(menu2);
    menu2->setPosition(CCPointZero);
    menu2->setTouchPriority(10);
    
	detail = SGButton::createFromLocal("unhot.png", str_BossRankItem_str2, this, menu_selector(SGBossRankItem::checkDetailEmbattle),CCPointZero,FONT_BOXINFO,ccWHITE,32);
    ////锚点换了坐标也换
	detail->setPosition(ccp(frame2->getPositionX()+frame2->getContentSize().width*0.5-detail->getContentSize().width*0.2-detail->getContentSize().height*0.5-20, spriteIcon->getPositionY()-detail->getContentSize().height*0.2));
    addBtn(detail);
	// detail->setTag(playid);
    this->addChild(detail);
    detail->setFontColor(ccc3(0xf3,0x89,00));
	// menu2->addChild(detail);
    

	string = CCString::createWithFormat(str_BossRankItem_str3, listDatas->level);

    labelRank = SGCCLabelTTF::create(string->getCString(),  fontName, fontSize);
    labelRank->setAnchorPoint(ccp(1, 0.5f));
    labelRank->setPosition(ccpAdd(detail->getPosition(), ccp(0, detail->getContentSize().height*0.7)));
    this->addChild(labelRank);
    //////前三名奖励图片
	CCSprite*   rankrewardpng=CCSprite::createWithSpriteFrameName("pvpno1.png");
    rankrewardpng->setAnchorPoint(ccp(1, 0.5));
    rankrewardpng->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(-spriteIconBg->getContentSize().width*0.3, -spriteIconBg->getContentSize().height*0.2)));
    rankrewardpng->setScale(1.2);
    this->addChild(rankrewardpng,5,10);
    rankrewardpng->setVisible(false);
    
    ranknum = SGCCLabelTTF::create(CCString::createWithFormat("%d", listDatas->loc + 1)->getCString(),  fontName, FONTSIZE(18));
    ranknum->setAnchorPoint(ccp(1, 0.5));
    ranknum->setPosition(ccpAdd(rankrewardpng->getPosition(), ccp(-rankrewardpng->getContentSize().width*0.3, spriteIconBg->getContentSize().height*0.15)));
    this->addChild(ranknum);

}

void SGBossRankItem::initData(SGBossRankListData *data, int order, bool isHavaKillBoss)
{
	roleId = data->roleId;
	
	
    CCString *string = CCString::createWithFormat("countryIcon_%d.png", data->country);
    spriteIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    spriteIcon->cocos2d::CCNode::setScale(0.9, 0.95);
    spriteIcon->setPosition(ccpAdd(spriteIconBg->getPosition(), ccp(spriteIcon->getContentSize().width*0.00, spriteIcon->getContentSize().height*0.05)) );
    
	
    string = CCString::createWithFormat("country_%d.png", data->country);
    spriteIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(string->getCString())->displayFrame());
    
    labelRank->setString(CCString::createWithFormat(str_BossRankItem_str3, data->level)->getCString());
    CCString *name= CCString::createWithFormat("%s", data->roleName.c_str());
    labelNickName->setString(name->getCString());
	
    labelshanghai->setString(CCString::createWithFormat(str_BossRankItem_str1, data->damage)->getCString());
	
	//有击杀者
	if(order<=4 && isHavaKillBoss)
    {
		CCSprite *sp= (CCSprite*)getChildByTag(10);
		CCSprite *bg = (CCSprite *)getChildByTag(32);
		//最后一个击杀的
		if (order == 1)
		{
			sp->setDisplayFrame(CCSprite::createWithSpriteFrameName("kill_icon.png")->displayFrame());
			bg->setDisplayFrame(CCSprite::createWithSpriteFrameName("pur_bg.png")->displayFrame());
            
            
			bg->setScaleX(1.02);
			bg->setScaleY(1);
			if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width || CCDirector::sharedDirector()->getWinSize().height == 1024
                ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
			{
				bg->setScaleX(1.23);
				bg->setScaleY(1.25);
			}
			if ((CCSprite *)getChildByTag(33))
				((CCSprite *)getChildByTag(33))->setVisible(false);//将背景边框隐藏，紫色的自带
		}
		else
		{
			sp->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("pvpno%d.png", order - 1)->getCString())->displayFrame());
		}
        
		sp->setVisible(true);
    }
    else if (order <= 4 && !isHavaKillBoss)//没有击杀者
	{
		CCSprite *sp= (CCSprite*)getChildByTag(10);
		sp->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("pvpno%d.png", order - 1)->getCString())->displayFrame());
		sp->setVisible(true);
	}
	else
    {
		ranknum->setString(CCString::createWithFormat("%d", order - 1)->getCString());
        CCSprite*sp= (CCSprite*)getChildByTag(10);
        sp->setVisible(false);
		
		float viewWid = bgSize.width;
		float viewH = bgSize.height;
		
		//每次删除掉再加上,不然item重用会使用后面的也变成紫色.或者没有边框
		this->removeChildByTag(33);
		CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
		frame2->setPreferredSize(CCSizeMake(viewWid, viewH));
		this->addChild(frame2, 33);
		
		CCSprite *bg = (CCSprite *)getChildByTag(32);
		CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
		bg->setDisplayFrame(frame2bg->displayFrame());
		bg->setScaleX(viewWid/frame2bg->getContentSize().width);
		bg->setScaleY(viewH/frame2bg->getContentSize().height);
    }

}

void SGBossRankItem::checkDetailEmbattle()
{
	main::FriendInfoRequest *info = new main::FriendInfoRequest();
    info->set_friendid(roleId);
	info->set_type(0);
//    if(typetype==0)
//    {
//        info->set_type(1);
//    }
//    else
//    {
//        info->set_type(0);
//    }
    
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);

}

