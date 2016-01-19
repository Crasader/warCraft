//
//  SGPiecesInfoLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-26.
//
//

#include "SGPiecesInfoLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGPiecesLayer.h"
#include "SGStringConfig.h"

SGPiecesInfoLayer::SGPiecesInfoLayer():
enterType(0),
_card(NULL)
{
	
}
SGPiecesInfoLayer::~SGPiecesInfoLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_piecesinfolayer);
}

/*
 enterType:
 0=背包 碎片
 1000=背包-武将-碎片。
 
 */
SGPiecesInfoLayer *SGPiecesInfoLayer::create(SGPiecesCard *card,int type)
{
    SGPiecesInfoLayer *layer = new SGPiecesInfoLayer();
    if (layer && layer->init(NULL, sg_piecesinfolayer))
    {
       
        layer->enterType = type;
        layer->_card = card;
        layer->_card->retain();
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}
void SGPiecesInfoLayer::initView()
{
	
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
     ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
      ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist",RES_TYPE_LAYER_UI ,sg_piecesinfolayer);
	
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();
	
    //float frameheight = skewingY(155);
    float hgt = skewingY(244);
    if (s.height == 1136) {
        //frameheight = skewing(155);
        hgt = skewing(244);
    }
    
	
    //左火
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/qhhyA.scml", true, true);
    
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect, 10, 0);
    //右火
    CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/qhhyB.scml", true, true);
    fermEffect1->setCCSpriterXFlipX(true);
    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    addChild(fermEffect1, 10, 1);
    CCLOG("type%d",_card->getItemType());

    int iconId;
    char officerName[256] = "\0";
  	if (_card->getItemType() == 0)//武将
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getCallCardId());
		iconId = temp->getIconId();
        
//        SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(_card->getCallCardId());
        
        this->setCardType(100);
        this->setstar(temp->getOfficerCurrStarLevel());
        this->setTitle(_card->getPiecesName()->getCString());
	}
	else if (_card->getItemType() == 1)//装备
	{
		SGEquipmentDataModel *temp =SGStaticDataManager::shareStatic()->getEquipById(_card->getCallCardId());
		iconId = temp->getIconId();
        this->setCardType(100);
        this->setstar(temp->getEquipCurrStarLevel());
        this->setTitle(_card->getPiecesName()->getCString());
	}
	
    
	SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_piecesinfolayer);
	sprintf(officerName, "head%d.png", iconId);
    CCSprite *item = CCSprite::createWithSpriteFrameName(officerName);
    this->addChild(item);
//    item->setScale(2);
    
    
    
    
    
    
    
    
    
    
    
    if (s.height == 960) {
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
		
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -300)));//-365
		
	}
    else if (s.height == 1136) {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*1.13 - title_bg->getContentSize().height), 768, (s.height - hgt*1.13 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
		
        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
		
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -350)));//-450
		
		
    }else
    {
        CCSprite *a = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        this->addChild(a,-100);
        a->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - a->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width*.41, a->getContentSize().height*.02 -a->getContentSize().height*.09)));
        fermEffect1->setPosition(ccpAdd(a->getPosition(), ccp(a->getContentSize().width*.41, a->getContentSize().height*.02-a->getContentSize().height*.09)));
        
        item->setPosition(ccpAdd(a->getPosition(), ccp(0, -a->getContentSize().height*.1)));//362
    }
    fermEffect->play();
    fermEffect1->play();
    menu->setZOrder(7);
	
    
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGPiecesInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    
    if(enterType == 101)
    {
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_piecesinfolayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.13)/2, s.width, hgt*1.13);
        
        //bg1->cocos2d::CCNode::setScale(r.size.width / bg1->getContentSize().width, r.size.height / bg1->getContentSize().height);
        bg1->setTextureRect(r);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
    }
    else if(enterType ==25)
    {
        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
		bg1->setAnchorPoint(ccp(0.5, 0));
		bg1->setScaleX(s.width/bg1->getContentSize().width);
		bg1->setScaleY((hgt*1.23)/bg1->getContentSize().height);
		bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 0 )));
		this->addChild(bg1,5);

    }
    else
    {
        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
		bg1->setAnchorPoint(ccp(0.5, 0));
		bg1->setScaleX(s.width/bg1->getContentSize().width);
		bg1->setScaleY((hgt*1.13-btmheight*.68)/bg1->getContentSize().height);
		bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
		this->addChild(bg1,5);

    }
    
    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setPosition(ccp(s.width/2, hgt*1.13));
    jinbian->setScaleX(s.width/jinbian->getContentSize().width);
    this->addChild(jinbian,6);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 320));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, -frame2->getContentSize().height/2 - 30)));
	
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(320/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2l,6);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2r,6);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang2m,6);
    guang2m->setScaleX(468/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(284+10, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-284-10, 0)));
    
    CCScale9Sprite *fontbg = CCScale9Sprite::createWithSpriteFrameName("box_fontbg.png");
    fontbg->setPreferredSize(CCSizeMake(538, 205));
    this->addChild(fontbg,6);
    fontbg->setPosition(ccpAdd(frame2->getPosition(), ccp(0, 26)));
    
	
    CCString* info=CCString::createWithFormat(str_PiecesInfolayer_str1,_card->getSummonNeed());
    
    //SGCCLabelTTF*label=SGCCLabelTTF::create(info->getCString(), FONT_BOXINFO, FONTSIZE(13));
    SGCCLabelTTF*label=SGCCLabelTTF::create(_card->getPiecesDesc()->getCString(), FONT_BOXINFO, FONTSIZE(13), CCSizeMake(450, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition( ccpAdd(fontbg->getPosition(), ccp(0, 20))   );
    this->addChild(label,10);
    
    
    ///////////////////////////////////////////////////////////////////////イメージを設定する　　　　　　　　　ここからはじまる
    //////////////////////////////////////////////////////////////////////
    

    

    
    
    
    
    
    //品质框
	char pinzhi[256] = "\0";
	
    //	if ( info->getCallCardId() <= 4000 && info->getCallCardId() >= 1001)
	if (_card->getItemType() == 0)
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getCallCardId());
		int starlvl = temp->getOfficerCurrStarLevel();
		if (starlvl>1) {
			starlvl-=1;
		}
		sprintf(pinzhi, "officerPinzhi_%d_0.png",starlvl);
	}
	else if (_card->getItemType() == 1)
	{
		SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(_card->getCallCardId());
		sprintf(pinzhi, "equipPinzhi_%d_0.png", equipModel->getEquipStarPinzhi());
	}
	
	CCSprite *pz = CCSprite::createWithSpriteFrameName(pinzhi);
	this->addChild(pz);
    pz->setPosition(item->getPosition());
    pz->setScale(item->getScale());
    // pz->setAnchorPoint(ccp(0, 0));
	
	
	//头像蒙版
	CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
	this->addChild(headMask);
    headMask->setPosition(ccpAdd(item->getPosition(), ccp(2, -2)));
    headMask->setScale(item->getScale());
    //headMask->setAnchorPoint(ccp(0, 0));
    

    ////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////ここまで終わる
    
    
    if(enterType==0 || 1000 == enterType)
    {
        SGButton *showButton = SGButton::createFromLocal("batch_sell.png", str_Show_, this, menu_selector(SGPiecesInfoLayer::showHandler), CCPointZero, FONT_PANGWA, ccc3(0xff, 0xc2, 00), 32);
        
        showButton->setPosition(ccpAdd(frame2bg->getPosition(), ccp(0, -210)));
        addBtn(showButton);
        showButton->setTag(100);
    }
    
    
    
}





void SGPiecesInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    //////////////////////////
	
    
    if (enterType==25) {
         SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_totalLoginReward);
        SGMainManager::shareMain()->showLayer(layer);
        return;
    }
    if (enterType==10) {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_creditCardLayer);
        SGMainManager::shareMain()->showLayer(layer);
        return;
    }
    if (enterType == CHATSHOWENTRYTAG) {
        SGMainManager::shareMain()->closeChatShowLayer();
        }
    if(enterType == 0)
    {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_piecesLayer);
        SGBaseLayer*layer2 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_equipPiecesLayer);
        if(layer)
        SGMainManager::shareMain()->showLayer(layer);
        else
        SGMainManager::shareMain()->showLayer(layer2);
        return;
    }
    //背包-武将-武将碎片
    if(1000 == enterType)
    {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_piecesBagLayer);
        if(layer)
            SGMainManager::shareMain()->showLayer(layer);
        return;
    }
    //由邀请好友界面进入
    if (enterType == 26)
    {
        main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVER, request);
    }
    
}
    
    

  void SGPiecesInfoLayer::showHandler()
{
    int starlvl;
    if (_card->getItemType() == 0)
	{
	    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getCallCardId());
            starlvl = temp->getOfficerCurrStarLevel();

	}
	else if (_card->getItemType() == 1)
	{
	    SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(_card->getCallCardId());
	    starlvl=equipModel->getEquipStarPinzhi();
	}
    
    
    
    
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getPiecesName(), 5, starlvl, _card->getSsid());
}
