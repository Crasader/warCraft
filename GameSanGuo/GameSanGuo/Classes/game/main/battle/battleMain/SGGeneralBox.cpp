//
//  SGGeneralBox.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-23.
//
//

#include "SGGeneralBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGRichLabel.h"
#include "RewardModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGFriend.h"
#include "SGStoryRewardBox.h"
#include "SGTestSwitch.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGFightRewardLayer.h"

SGGeneralBox::SGGeneralBox(void)
:_type(0),
t_delegate(NULL),
ldIndex(-1)//炼丹数据
{

}
SGGeneralBox::~SGGeneralBox(void)
{
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_generalBox);
    if (_type == 2) {
        SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
        notification->removeObserver(this, MSG_REWARD_LIANDANENTER);
    }
}

SGGeneralBox* SGGeneralBox::create(SGBoxDelegate *delegate, int type, SGOfficerCard *card)
{
    SGGeneralBox *box = new SGGeneralBox();
    if (box && box->init(delegate, type)) {
        box->t_delegate = delegate;
        box->initView(card);
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

bool SGGeneralBox::init(SGBoxDelegate *delegate, int type)
{
 
    bool isBottom = true;
    bool isHavabg = true;
    
    CCSize csize = CCSizeMake(535, 320);
    
    if (type == 3) {
        isBottom = false;
        isHavabg = false;
        csize = CCDirector::sharedDirector()->getWinSize();
    }
    
    if (type == 2) {
        csize = CCSize(535, 340);
    }
	//援军好友界面
	if (type == 1)
	{
		csize = CCSize(550, 680);
	}
    _type = type;
    
    if (!SGBaseBox::init(delegate, box_general, csize, isHavabg, isBottom)) {
        return false;
    }
    return true;
}

void SGGeneralBox::initView(SGOfficerCard *card)
{
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this,
																  callfuncO_selector(SGGeneralBox::getFriendOfficerListener));
 
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_generalBox);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer3.plist", RES_TYPE_LAYER_UI, sg_generalBox);
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_generalBox);
	ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_generalBox);

    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    if (_type == 1)
	{ //添加好友
			
        fontbg->setContentSize(CCSizeMake(450, 450));
		fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 20)));
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_GeneralBox_str1, FONT_PANGWA, 40 ,ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc  merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(ccpAdd(biao->getPosition(),ccp(0,-10)));
        this->addChild(title);
		SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());
		int iconId = officer->getIconId();
		SGMainManager::shareMain()->addHeadIconPlistByNum(iconId, sg_generalBox);
		char headStr[256] = "\0";
		sprintf(headStr, "head%d.png", iconId);
		
		CCSprite *headIcon = CCSprite::createWithSpriteFrameName(headStr);
		this->addChild(headIcon, 1);
		headIcon->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width * .3f, fontbg->getContentSize().height * .3f)));
		
		CCSprite *cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
		this->addChild(cardBg, 0);
		cardBg->setPosition(ccpAdd(headIcon->getPosition(), ccp(0, 0)));
		
		
		int starLvl = officer->getOfficerCurrStarLevel();
		if (starLvl > 1)
			starLvl -= 1;
		char borderStr[256] = "\0";
		sprintf(borderStr, "officerPinzhi_%d_0.png", starLvl);
		CCSprite *borderIcon = CCSprite::createWithSpriteFrameName(borderStr);
		this->addChild(borderIcon, 2);
		borderIcon->setPosition(ccpAdd(headIcon->getPosition(), ccp(0, 0)));
		
		SGCCLabelTTF *level = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d", HelpFriendItemLevel)->getCString(), FONT_PANGWA, 26);
		this->addChild(level, 1);
		level->setPosition(ccpAdd(borderIcon->getPosition(), ccp(0, -borderIcon->getContentSize().height / 2
																 + level->getContentSize().height + 2)));
		
		
		if (officer->getAdvNum())
		{
            SGCCLabelTTF *adIcon = SGCCLabelTTF::create(CCString::createWithFormat("+%d",officer->getAdvNum())->getCString(), FONT_PANGWA, 24);
            adIcon->setInsideColor(ccGREEN);
			this->addChild(adIcon, 1);
			adIcon->setPosition(ccpAdd(borderIcon->getPosition(), ccp(borderIcon->getContentSize().width * .20f,
																	  borderIcon->getContentSize().height * .20f)));
		}
		
		CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",officer->getOfficerRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
		this->addChild(countryName, 2);
		countryName->setPosition(ccpAdd(borderIcon->getPosition(), ccp(-borderIcon->getContentSize().width * .155f,
																	   borderIcon->getContentSize().height * .31f)));
		
		SGCCLabelTTF *officerName = SGCCLabelTTF::create(CCString::create(officer->getOfficerName()->getCString())->getCString(),
														 FONT_BOXINFO, 32);
		officerName->setAnchorPoint(ccp(0, 0.5));
		this->addChild(officerName, 1);
		officerName->setPosition(ccpAdd(borderIcon->getPosition(), ccp(borderIcon->getContentSize().width / 1.6,
																	   borderIcon->getContentSize().height * .25f)));
		
		//画星星
		for(int i = 0; i < 3; i++)
		{
			CCSprite* xingji = CCSprite::createWithSpriteFrameName("xx.png");
			xingji->setAnchorPoint(ccp(0.2, 0.5));
			xingji->setPosition(ccp(borderIcon->getPosition().x + borderIcon->getContentSize().width / 1.8 + xingji->getContentSize().width * i,
									borderIcon->getPosition().y - xingji->getContentSize().height / 1.2));
			this->addChild(xingji,1,i);
		}
		
		
		int m_atk = card->getAtk();
		int m_def = card->getDef();
		int m_round = card->getRound();
		int m_speed= card->getSpeed();
		int m_mor = card->getMorale();
		
		CCObject *obj = NULL;
		CCARRAY_FOREACH(card->getOfficerEquips(), obj)
		{
			SGEquipCard *equipCard = (SGEquipCard *)obj;
//			m_atk += equipCard->getAtk();
//			m_def += equipCard->getDef();
			m_round += equipCard->getRound();
//			m_speed += equipCard->getSpeed();
//			m_mor += equipCard->getMorale();
		}
		
		CCSprite* tiao1 = CCSprite::createWithSpriteFrameName("login_name_line.png");
		tiao1->setScaleX(450/tiao1->getContentSize().width);
		tiao1->setPosition(ccp(biao->getPosition().x, headIcon->getPosition().y - 65));
		this->addChild(tiao1,5);
		
		CCSprite* tiao2 = CCSprite::createWithSpriteFrameName("login_name_line.png");
		tiao2->setScaleX(450/tiao1->getContentSize().width);
		tiao2->setPosition(ccpAdd(tiao1->getPosition(),ccp(0, -140)));

		
		SGCCLabelTTF *atk_ = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
        //modify by:zyc. merge into create.
		//atk_->setColor(ccc3(0xff,0xc2,0x00));
		atk_->setPosition(ccpAdd(tiao1->getPosition(), ccp(-fontbg->getContentSize().width * .35f,-30)));
		this->addChild(atk_);
		
		SGCCLabelTTF *def_ = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
        //modify by:zyc. merge into create.
		//def_->setColor(ccc3(0xff,0xc2,0x00));
		def_->setPosition(ccpAdd(atk_->getPosition(), ccp(0,-43)));
		this->addChild(def_);
		
		SGCCLabelTTF *mor_ = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
        //modify by:zyc. merge into create.
		//mor_->setColor(ccc3(0xff,0xc2,0x00));
		mor_->setPosition(ccpAdd(def_->getPosition(), ccp(0,-43)));
		this->addChild(mor_);
		
		SGCCLabelTTF *xuli_ = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
		xuli_->setPosition(ccpAdd(atk_->getPosition(), ccp(200, 0)));
		this->addChild(xuli_);
        //modify by:zyc. merge into create.
		//xuli_->setColor(ccc3(0xff,0xc2,0x00));
		
		SGCCLabelTTF *sudu_ = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
		sudu_->setPosition(ccpAdd(xuli_->getPosition(), ccp(0,-43)));
		this->addChild(sudu_);
        //modify by:zyc. merge into create.
		//sudu_->setColor(ccc3(0xff,0xc2,0x00));
		
		CCString* str_atk = CCString::createWithFormat("%d",m_atk);
		SGCCLabelTTF* atk = SGCCLabelTTF::create(str_atk->getCString(), FONT_BOXINFO, 26);
		atk->setAnchorPoint(ccp(1, 0.5));
		atk->setPosition(ccpAdd(atk_->getPosition(), ccp(105, 0)));
		this->addChild(atk);
		
		CCString* str_def = CCString::createWithFormat("%d",m_def);
		SGCCLabelTTF* def = SGCCLabelTTF::create(str_def->getCString(), FONT_BOXINFO, 26);
		def->setAnchorPoint(ccp(1, 0.5));
		def->setPosition(ccpAdd(def_->getPosition(), ccp(105, 0)));
		this->addChild(def);
		
		CCString* str_shiqi = CCString::createWithFormat("%d",m_mor);
		SGCCLabelTTF* shiqi = SGCCLabelTTF::create(str_shiqi->getCString(), FONT_BOXINFO, 26);
		shiqi->setAnchorPoint(ccp(1, 0.5));
		shiqi->setPosition(ccpAdd(mor_->getPosition(), ccp(105,0)));
		this->addChild(shiqi);
		
		CCString* strxuli = CCString::createWithFormat("%d",m_round);
		SGCCLabelTTF* xuli = SGCCLabelTTF::create(strxuli->getCString(), FONT_BOXINFO, 26);
		xuli->setAnchorPoint(ccp(1, 0.5));
		xuli->setPosition(ccp(xuli_->getPosition().x + 105, atk->getPosition().y));
		this->addChild(xuli);
		
		CCString* str_spd = CCString::createWithFormat("%d",(int)m_speed);
		SGCCLabelTTF* speed = SGCCLabelTTF::create(str_spd->getCString(), FONT_BOXINFO, 26);
		speed->setAnchorPoint(ccp(1, 0.5));
		speed->setPosition(ccp(xuli_->getPosition().x + 105, def->getPosition().y));
		this->addChild(speed);
		
		CCScale9Sprite* font_bg = CCScale9Sprite::createWithSpriteFrameName("labelbg.png");
		font_bg->setPreferredSize(CCSize(215, 24));
		font_bg->setPosition(ccp(xuli_->getPosition().x + 70, shiqi->getPosition().y));
		//    this->addChild(font_bg);

		
		///////////////////////////
		
        //CCLog("%s",HelpFriendRoleName);
        CCString *str = CCString::createWithFormat(str_GeneralBox_str2, HelpFriendRoleName);
        CCSize nsize = CCSizeMake(size.width*.72f, size.height*.5f);
        CCSprite *label = SGRichLabel::getFomLabel(str->getCString(), nsize, FONT_BOXINFO, 32);
        label->setPosition(ccpAdd(center, ccp(0, -label->getContentSize().height*0.8)));
        this->addChild(label);
        
      //  SGButton *addBtn = SGButton::create("box_btnbg.png",
       //                                      "friend_friend_1.png",
      //                                       this,
       //                                      menu_selector(SGGeneralBox::addFriendAction),
        //                                     ccp(0, 0),
         //                                    false,
         //                                    true);
        
        SGButton *addBtn = SGButton::createFromLocal("box_btn_newbg.png", str_GeneralBox_str3, this, menu_selector(SGGeneralBox::addFriendAction),CCPointZero,FONT_PANGWA,ccWHITE,40);
        this->addBtn(addBtn);
        addBtn->setPosition(ccpAdd(center, ccp(-addBtn->getContentSize().width*.75f, -size.height/2 + 70)));
//        SGButton *stopBtn = SGButton::create("box_btnbg.png",
//                                             "public_font_qixiao.png",
//                                             this,
//                                             menu_selector(SGGeneralBox::closeAction),
//                                             ccp(0, 0),
//                                             false,
//                                             true);
        SGButton *stopBtn = SGButton::createFromLocal("box_btn_newbg.png", str_Cancel, this, menu_selector(SGGeneralBox::closeAction),CCPointZero,FONT_PANGWA,ccWHITE,40);
        
        this->addBtn(stopBtn);
        stopBtn->setPosition(ccpAdd(center, ccp(stopBtn->getContentSize().width*.75f, -size.height/2 + 70)));
    }else if (_type == 2) { //炼丹炉开启
        SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
        notification->addObserver(MSG_REWARD_LIANDANENTER, this,
                                  callfuncO_selector(SGGeneralBox::liandanListener));
        

//        CCSprite *title = CCSprite::createWithSpriteFrameName("font_fxldl.png");
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips60, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));

        title->setPosition(biao->getPosition());

        this->addChild(title);
        
        SGButton *ok = SGButton::create("box_btn_newbg.png",
                                        "font_qw.png",
                                        this,
                                        menu_selector(SGGeneralBox::openLianDanAction),
                                        ccp(0, 0),
                                        false,
                                        true);
      //  this->addBtn(ok);

        ok->setPosition(ccpAdd(center, ccp(-145, -size.height/2+5 -40)));

        
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_queding.png",
                                            this,
                                            menu_selector(SGGeneralBox::closeAction),
                                            ccp(0, 0),
                                            false,
                                            true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(center, ccp(0, -size.height/2+40)));

        

        SGCCLabelTTF *a = SGCCLabelTTF::create(str_GeneralBox_str4, FONT_BOXINFO, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(-8,-90)));
        SGCCLabelTTF *b = SGCCLabelTTF::create(str_GeneralBox_str5, FONT_BOXINFO, 32);
        

        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2+8,- 50)));
    }else if (_type == 3) {
        this->initUpdateBox(deletage);
    }
}

void SGGeneralBox::openLianDanAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    //open liandan    
    main::RewardLianDanTimeRequest *request = new main::RewardLianDanTimeRequest();
    request->set_index(ldIndex);
    DDLog("q_1_send===============-----------%d", ldIndex);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LIANDANENTER, request);
}

void SGGeneralBox::addFriendAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    main::AddFriendRequest *add = new main::AddFriendRequest();
    add->set_friendroleid(HelpFriendRoleId);
    SGMainLayer *mainLayer = SGMainManager::shareMain()->getMainLayer();
    mainLayer->m_isbattelAddFriend = true;
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_ADDFRIEND, add);
    
    this->closeAction(sender);
}

void SGGeneralBox::liandanListener(CCObject *obj)
{
    this->closeAction(NULL);
    
    SGSocketRequest *req = (SGSocketRequest *)obj;
    if (req) {
        main::RewardLianDanTimeResponse *response = (main::RewardLianDanTimeResponse *)req->m_msg;
        int time = response->time();
        DDLog("q_2_send===============-----------%d-%d", ldIndex, time);
        SGMainManager::shareMain()->showLiandanLayer(time, ldIndex);
    }
}

void SGGeneralBox::getFriendOfficerListener(CCObject *sender)
{
	SGMainManager *sGMainManager = SGMainManager::shareMain();
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::FriendInfoResponse *friRes = (main::FriendInfoResponse *)req->m_msg;
		if (friRes)
		{
			SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(friRes->role());
			SGOfficerCard *officer = playerInfo->getLordOfficer();
            HelpFriendRoleName=playerInfo->getNickName()->getCString();
            HelpFriendItemLevel=playerInfo->getLevel();
            HelpFriendItemId=playerInfo->getLordid();
            HelpFriendItemLevel=playerInfo->getLevel();
            
			officer->retain();
			
			// 此处获得好友武将卡的信息
			SGGeneralBox *box = SGGeneralBox::create(deletage, 1, officer);
			box->setLdIndex(ldIndex);
			sGMainManager->closeBox();
			sGMainManager->showBox(box);
		}
	}
}

void SGGeneralBox::closeAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
    int index = ldIndex;
    
    if (_type == 3) {
        if (SGFriend::isFriendOfPlayer(HelpFriendRoleId) != SGFRIEND_FRIEND && HelpFriendRoleId > 0) { //非好友，添加好友
			
			SGMainManager::shareMain()->getFriendOfficerInfo();//发送请求
			
//            SGGeneralBox *box = SGGeneralBox::create(deletage, 1);
//            box->setLdIndex(index);
//            SGMainManager::shareMain()->closeBox();
//            SGMainManager::shareMain()->showBox(box);
            return;
        }
        if (ldIndex >= 0) { // 开启炼丹炉
            SGGeneralBox *box = SGGeneralBox::create(deletage, 2);
            box->setLdIndex(index);
            SGMainManager::shareMain()->closeBox();
            SGMainManager::shareMain()->showBox(box);
            return;
        }
    }
    if (_type == 1) {
        if (ldIndex >= 0) { // 开启炼丹炉
            SGGeneralBox *box = SGGeneralBox::create(deletage, 2);
            box->setLdIndex(index);
            SGMainManager::shareMain()->closeBox();
            SGMainManager::shareMain()->showBox(box);
            return;
        }
    }
    
//    SGStoryRewardBox::gobackToStoryLayer();
    SGFightRewardLayer::gobackToStoryLayer();
}

void SGGeneralBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}

void SGGeneralBox::initUpdateBox(SGBoxDelegate *delegate)
{
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    
    int currLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    SGPlayerDataModel *playPreInfo = SGStaticDataManager::shareStatic()->getPlayerByLevel(HelpFriendPlayLev);
    SGPlayerDataModel *playCurrInfo = SGStaticDataManager::shareStatic()->getPlayerByLevel(currLev);
    
    // init the layer background
    CCRect rect = CCRectMake(stdSize.width/2-size.width/2, stdSize.height/2-size.height/2,
                             size.width, size.height);
    
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_generalBox);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("storyBg.png");
    bgImg->setTextureRect(rect);
    bgImg->setPosition(SGLayout::getPoint(kMiddleCenter));
    this->addChild(bgImg, -5);
    
    CCSprite *bgColorImg = CCSprite::createWithSpriteFrameName("uplevel_colorbg_n.png");
    bgColorImg->setPosition(ccpAdd(upCenter, ccp(0, -bgColorImg->getContentSize().height*.5f)));
    this->addChild(bgColorImg, -4);
    
    CCSprite *bgLevel = CCSprite::createWithSpriteFrameName("uplevel_clevel_n.png");
    bgLevel->setPosition(ccpAdd(upCenter, ccp(0, -bgLevel->getContentSize().height*.15f)));
    this->addChild(bgLevel, -3);
    
    // init the handsome girl
    CCSprite *spMM = CCSprite::createWithSpriteFrameName("uplevel_mm_n.png");
    spMM->setPosition(ccp(size.width*.5f, size.height-spMM->getContentSize().height*.5f-175));
    spMM->setVisible(false);
    this->addChild(spMM, -2);
    
    // init the level rect
    CCScale9Sprite *frame = CCScale9Sprite::createWithSpriteFrameName("uplevel_fontbg_n.png");
    frame->setOpacity(205);
    this->addChild(frame, -1);
//    frame->setPreferredSize(CCSizeMake(size.width*.78f, size.height*.17f));
    frame->setPosition(ccpAdd(upCenter, ccp(0, -frame->getContentSize().height*.5f-475)));
    
    CCSprite *frameBg = CCSprite::createWithSpriteFrameName("uplevel_levbg.png");
    frameBg->setScaleX(frame->getContentSize().width/frameBg->getContentSize().width);
    frameBg->setScaleY(frame->getContentSize().height/frameBg->getContentSize().height);
    frameBg->setPosition(frame->getPosition());
    this->addChild(frameBg, -2);
    
    // 上框线框
    float posX = frame->getPositionX()-frame->getContentSize().width*.51f;
    float posY = frame->getPositionY()+frame->getContentSize().height*.53f;
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setFlipY(true);
    titlecenter->setAnchorPoint(ccp(0.5f, 1));
    titlecenter->setPosition(ccp(frame->getPositionX(), posY));
    this->addChild(titlecenter);
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setFlipY(true);
    title_bg_l->setAnchorPoint(ccp(0, 1));
    title_bg_l->setPosition(ccp(posX, posY));
    this->addChild(title_bg_l);
    posX = frame->getPositionX()+frame->getContentSize().width*.51f;
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setFlipY(true);
    title_bg_r->setPosition(ccp(posX, posY));
    title_bg_r->setAnchorPoint(ccp(1, 1));
    this->addChild(title_bg_r);
    
    // 下框线框
    posX = frame->getPositionX()-frame->getContentSize().width*.51f;
    posY = frame->getPositionY()-frame->getContentSize().height*.53f;
    titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5f, 0));
    titlecenter->setPosition(ccp(frame->getPositionX(), posY));
    this->addChild(titlecenter);
    title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccp(posX, posY));
    this->addChild(title_bg_l);
    posX = frame->getPositionX()+frame->getContentSize().width*.51f;
    title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setPosition(ccp(posX, posY));
    title_bg_r->setAnchorPoint(ccp(1, 0));
    this->addChild(title_bg_r);
    
    // 升级信息
    CCSprite *spLevelFont = CCSprite::createWithSpriteFrameName("uplevel_flevel_n.png");
    spLevelFont->setAnchorPoint(ccp(0, 0.5f));
    spLevelFont->setPosition(ccpAdd(frame->getPosition(), ccp(-235, 125)));
    this->addChild(spLevelFont);
    
    const char* levCh = CCString::createWithFormat("%d", HelpFriendPlayLev)->getCString();
    CCLabelAtlas *labelLeft = CCLabelAtlas::create(levCh, "sanguobigpic/uplevel_lnum_n.png", 45, 48, '0');
    labelLeft->setAnchorPoint(ccp(0, 0.5f));
    labelLeft->setPosition(ccpAdd(spLevelFont->getPosition(), ccp(spLevelFont->getContentSize().width, 0)));
    this->addChild(labelLeft);
    CCSprite *labelTag = CCSprite::createWithSpriteFrameName("uplevel_btip.png");
    labelTag->setAnchorPoint(ccp(0, 0.5f));
    labelTag->setPosition(ccpAdd(labelLeft->getPosition(), ccp(labelLeft->getContentSize().width, 0)));
    this->addChild(labelTag);
    levCh = CCString::createWithFormat("%d", currLev)->getCString();
    CCLabelAtlas *labelRight = CCLabelAtlas::create(levCh, "sanguobigpic/uplevel_rnum_n.png", 62, 67, '0');
    labelRight->setAnchorPoint(ccp(0, 0.5f));
    labelRight->setPosition(ccpAdd(labelTag->getPosition(), ccp(labelTag->getContentSize().width, 0)));
    this->addChild(labelRight);
    
    CCSprite *slineU = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
    slineU->setPosition(ccpAdd(frame->getPosition(), ccp(0, 77)));
    this->addChild(slineU);
    CCSprite *slineD = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
    slineD->setPosition(ccpAdd(frame->getPosition(), ccp(0, -100)));
    this->addChild(slineD);
    
    
    // old data
    int iVar1 = playPreInfo->getPlayerBaseFriendSize();
    int iVar2 = playPreInfo->getPlayerOfficerMax();
    int iVar3 = playPreInfo->getPlayerPower();
    int iVar4 = playPreInfo->getPlayerGoverning();
    // currlev data
    int iVar5 = playCurrInfo->getPlayerBaseFriendSize();
    int iVar6 = playCurrInfo->getPlayerOfficerMax();
    int iVar7 = playCurrInfo->getPlayerPower();
    int iVar8 = playCurrInfo->getPlayerGoverning();
    // 文字Label
    int culev[4] = {iVar1, iVar2, iVar3, iVar4};
    int uplev[4] = {iVar5, iVar6, iVar7, iVar8};
    const char *desc[4] = {str_GeneralBox_str6, str_GeneralBox_str7, str_GeneralBox_str8, str_GeneralBox_str9};
    float fontSize = FONTSIZE(16);
    const char* fontName = FONT_BOXINFO;
    float discX = frame->getContentSize().width;
    for (int ii = 0; ii < 4; ii++) {
        SGCCLabelTTF *label = SGCCLabelTTF::create(desc[ii], fontName, fontSize);
        label->setAnchorPoint(ccp(0, 0.5f));
        label->setPosition(ccpAdd(frame->getPosition(), ccp(-frame->getContentSize().width*.37f, 53-43.75f*ii)));
        this->addChild(label);
        
        const char *cnum = CCString::createWithFormat("%d", culev[ii])->getCString();
        SGCCLabelTTF *levC = SGCCLabelTTF::create(cnum, fontName, fontSize);
        levC->setAnchorPoint(ccp(1, 0.5f));
        levC->setPosition(ccpAdd(label->getPosition(), ccp(discX*.43f, 0)));
        this->addChild(levC);
        
        cnum = CCString::createWithFormat("%d", uplev[ii])->getCString();
        SGCCLabelTTF *levL = SGCCLabelTTF::create(cnum, fontName, fontSize);
        levL->setAnchorPoint(ccp(1, 0.5f));
        levL->setPosition(ccpAdd(label->getPosition(), ccp(discX*.7f, 0)));
        this->addChild(levL);
        if (culev[ii] < uplev[ii]) levL->setInsideColor(ccGREEN);
        
        CCSprite *sline = CCSprite::createWithSpriteFrameName("uplevel_stip.png");
        sline->setPosition(ccpAdd(levC->getPosition(), ccp(discX*.11f, 0)));
        this->addChild(sline);
    }
    
    // 体力回满提示
    CCSprite *spPower = CCSprite::createWithSpriteFrameName("uplevel_powerbg_n.png");
    spPower->setPosition(ccpAdd(frame->getPosition(), ccp(0, -137)));
    this->addChild(spPower);
    CCLabelTTF *levL = CCLabelTTF::create(str_GeneralBox_str10, fontName, fontSize);
    levL->setPosition(spPower->getPosition());
    this->addChild(levL);
    
    CCSprite *spCon = CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
    spCon->setPosition(ccpAdd(frame->getPosition(), ccp(0, -237)));
    this->addChild(spCon);
    
    // 结束后reset playLev
    HelpFriendPlayLev = currLev;
}


void SGGeneralBox::setLdIndex(int index)
{
    ldIndex = index;
}