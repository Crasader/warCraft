//
//  SGEmbattleBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//  设置上场武将弹框
//

#include "SGEmbattleBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ArmyModule.pb.h"
#include "SGSocketClient.h"
#include "SGCCLabelTTF.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGEquipCard.h"
#include "SGArrangeLayer.h"
#include "SGGuideManager.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGEmbattleLayer.h"
#include "AppMacros.h"

#include "SGTeamgroup.h"
#include "SGGeneralInfoLayer.h"
#include "SGStringConfig.h"


SGEmbattleBox::SGEmbattleBox()
:officerCard(NULL),
_positon(0),
m_gov(0),
totleGovern(0),
emtype(0),
fromWhere(0)
{
    
}
SGEmbattleBox::~SGEmbattleBox()
{
	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_embattleBox);
    
	
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EMBATTLE_DISPATCH);
    notification->removeObserver(this, MSG_EMBATTLE_REMOVE);
    notification->removeObserver(this, MSG_EMBATTLE_SETLORD);
	//释放info
}
SGEmbattleBox *SGEmbattleBox::create(SGBoxDelegate *delegate, SGOfficerCard *card, int govern,int em,int pos, int wid, int hei, int fromWhere)
{
    SGEmbattleBox *embattleBox = new SGEmbattleBox();
	embattleBox->fromWhere = fromWhere;//来自哪个页面
    embattleBox->officerCard = card;
	embattleBox->officerCard->retain();
    embattleBox->_positon = pos;
    embattleBox->totleGovern = govern;
    embattleBox->emtype = em;
	
    
    if (embattleBox && embattleBox->init(delegate, box_embattleBox,CCSizeMake(wid, hei),true))
    {
        embattleBox->initView();
        embattleBox->autorelease();
        return embattleBox;
    }
    CC_SAFE_RELEASE(embattleBox);
    return NULL;
}

void SGEmbattleBox::initView()
{
	 
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_box_embattleBox);
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_box_embattleBox);
	ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_box_embattleBox);
	ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_box_embattleBox, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_box_embattleBox);

    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EMBATTLE_DISPATCH,
                              this,
                              callfuncO_selector(SGEmbattleBox::setMainOfficerListener));
    notification->addObserver(MSG_EMBATTLE_REMOVE,
                              this,
                              callfuncO_selector(SGEmbattleBox::offMainOfficerListener));
    notification->addObserver(MSG_EMBATTLE_SETLORD,
                              this,
                              callfuncO_selector(SGEmbattleBox::setLordOfficerListener));
    

	if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard)<= 0)
	{
		fontbg->setPreferredSize(CCSize(480, 282));
	}
	else
	{
		fontbg->setPreferredSize(CCSize(480, 452));
	}
    
    fontbg->setPosition(ccpAdd(biao->getPosition(), ccp(0,- fontbg->getContentSize().height*.5f-40)));

    
    char spriteName[256] = {0};
	//12.05
	SGMainManager::shareMain()->addHeadIconPlistByNum(officerCard->getHeadSuffixNum(),sg_emBattleBox);
    sprintf(spriteName, "head%d.png",officerCard->getHeadSuffixNum());
    
    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(officerCard->getItemId());
    int starlvl = temp->getOfficerCurrStarLevel();
    if (starlvl>1) {
        starlvl-=1;
    }
    SGButton *portrait = SGButton::create(spriteName,CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString(), this,
                                          menu_selector(SGEmbattleBox::showGeneralInfoLayer),
                                          ccp(0, 0),
                                          false,
                                          true);
//    SGButton *portrait = SGButton::create(spriteName,CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString(), this,
//                                    menu_selector(SGEmbattleBox::showGeneralInfoLayer),
//                                    ccp(0, 0),
//                                    false,
//                                    true);
    
//    portrait->setPosition(ccpAdd(biao->getPosition(),ccp(-portrait->getContentSize().width - portrait->getContentSize().width/2 - 30, -105)));
    portrait->setPosition(ccpAdd(fontbg->getPosition(),ccp(-fontbg->getContentSize().width/2+portrait->getContentSize().width/2+20, fontbg->getContentSize().height/2-portrait->getContentSize().height/2-15)));
    //if (str_1->isEqual(CCString::create("SGOfficerCard")))
    {
        portrait->removeChildByTag(30);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(portrait->getContentSize().width*0.27,portrait->getContentSize().height - countryName->getContentSize().height/7));
        portrait->addChild(countryName,30,30);
        
        if (temp->getAdvNum()>0) {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
            advancenumber->setInsideColor(ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->ignoreAnchorPointForPosition(false);
            advancenumber->setPosition(ccp(portrait->getContentSize().width*1,portrait->getContentSize().height - advancenumber->getContentSize().height*0.1));
            portrait->addChild(advancenumber,50,250);
        }

    }
    
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background);
    background->setPosition(portrait->getPosition());
    this->addChild(portrait);
    
    SGCCLabelTTF* name = SGCCLabelTTF::create(officerCard->getOfficerName()->getCString(), FONT_PANGWA, 26);
    name->setAnchorPoint(ccp(0, 1));
    name->setPosition(ccp(portrait->getPosition().x + portrait->getContentSize().width*.7, portrait->getPosition().y + portrait->getContentSize().height*.4));
    this->addChild(name);
    
	for(int i=0; i < officerCard->getUpgradestar();i++)
    {
        CCSprite* xingji = CCSprite::createWithSpriteFrameName("xx.png");
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccp(name->getPosition().x + xingji->getContentSize().width * i, name->getPosition().y - xingji->getContentSize().height*1.4));
        this->addChild(xingji,1,i);
        if (i>=officerCard->getCurrStar()) {
            xingji->setVisible(false);
        }
    }
    for(int i=officerCard->getCurrStar(); i < officerCard->getUpgradestar();i++)
    {
        CCSprite* xingji = CCSprite::createWithSpriteFrameName("bluestar.png");
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccpAdd(((CCSprite*)getChildByTag(i))->getPosition(), ccp(0,-0.5)));
        this->addChild(xingji,1,i);
        
    }
    
    int m_atk = officerCard->getAtk();
    int m_def = officerCard->getDef();
    int m_round = officerCard->getRound();
    int m_speed= officerCard->getSpeed();
    int m_mor = officerCard->getMorale();
    m_gov = officerCard->getGoverning();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(officerCard->getOfficerEquips(), obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
//        m_atk += card->getAtk();
//        m_def += card->getDef();
        m_round += card->getRound();
//        m_speed += card->getSpeed();
//        m_mor += card->getMorale();
        m_gov += card->getGoverning();
    }
    if (m_gov< 0) {
        m_gov = 0;
    }
    
    CCString* str_lvl = CCString::createWithFormat("%d级",officerCard->getCurrLevel());
    SGCCLabelTTF* lvl = SGCCLabelTTF::create(str_lvl->getCString(), FONT_PANGWA, 26);
    lvl->setAnchorPoint(ccp(1, 0.5));

    this->addChild(lvl);
    lvl->setPosition(ccp(biao->getPosition().x + lvl->getContentSize().height * 2.5, name->getPosition().y - name->getContentSize().height/2));
    
    CCSprite* tiao1 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    tiao1->setScaleX(450/tiao1->getContentSize().width);
    tiao1->setPosition(ccp(biao->getPosition().x, portrait->getPosition().y - 65));
    this->addChild(tiao1,5);
    
    CCSprite* tiao2 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    tiao2->setScaleX(450/tiao1->getContentSize().width);
    tiao2->setPosition(ccpAdd(tiao1->getPosition(),ccp(0, -140)));
    if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard)>0)
    this->addChild(tiao2,5);
//
//    CCSprite* tiao3 = CCSprite::createWithSpriteFrameName("login_name_line.png");
//    tiao3->setScaleX(480/tiao1->getContentSize().width);
//    tiao3->setPosition(ccpAdd(tiao2->getPosition(),ccp(0, -150)));
//    this->addChild(tiao3,5);
//
    SGCCLabelTTF* font_zjji = SGCCLabelTTF::create(str_zhujiangji, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_zjji->setColor(COLOR_YELLOW);
    font_zjji->setAnchorPoint(ccp(0, 0.5f));
    font_zjji->setPosition(ccpAdd(portrait->getPosition(), ccp(-portrait->getContentSize().width*.5f - 2,-85)));
    //this->addChild(font_zjji);
//
    SGCCLabelTTF* font_wjji = SGCCLabelTTF::create(str_beidongji, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_wjji->setColor(COLOR_YELLOW);
    font_wjji->setAnchorPoint(ccp(0, 0.5f));
    font_wjji->setPosition(ccpAdd(font_zjji->getPosition(), ccp(0,-150)));
    //this->addChild(font_wjji);
//
//    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(officerCard->getLordSkill());
//    
//    SGCCLabelTTF* a = SGCCLabelTTF::create("该武将没有主将计", FONT_BOXINFO, 24);
//    a->setAnchorPoint(ccp(0, 1));
//    a->setHorizontalAlignment(kCCTextAlignmentLeft);
//    a->setDimensions(CCSizeMake(24*18,350));
//    this->addChild(a);
//    a->setPosition(ccpAdd(font_zjji->getPosition(), ccp(0, -font_zjji->getContentSize().height*.6)));
//    if (lordSkill) {
//        a->setString(lordSkill->getSkillInfo()->getCString());
//        
//    }
//    SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(officerCard->getOfficerSkil());
//    SGCCLabelTTF* b = SGCCLabelTTF::create("该武将没有被动技", FONT_BOXINFO, 24);
//    b->setHorizontalAlignment(kCCTextAlignmentLeft);
//    b->setAnchorPoint(ccp(0, 1));
//    b->setDimensions(CCSizeMake(24*18,350));
//    this->addChild(b);
//    b->setPosition(ccpAdd(font_wjji->getPosition(), ccp(0, -font_wjji->getContentSize().height*.6)));
//
//    if (officerSkill)
//    {
//        b->setString(officerSkill->getSkillInfo()->getCString());
//       
//    }    
    
    SGCCLabelTTF *atk_ = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
    //modify by:zyc. merge into create.
   // atk_->setColor(ccc3(0xff,0xc2,0x00));
    atk_->setPosition(ccpAdd(font_wjji->getPosition(), ccp(atk_->getContentSize().width*.5f - 5,145)));
    this->addChild(atk_);
    
    SGCCLabelTTF *def_ = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 28 , ccc3(0xff,0xc2,0x00));
    //modify by:zyc. merge into create.
    //def_->setColor(ccc3(0xff,0xc2,0x00));
    def_->setPosition(ccpAdd(atk_->getPosition(), ccp(0,-43)));
    this->addChild(def_);
    
    SGCCLabelTTF *mor_ = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 28 , ccc3(0xff,0xc2,0x00));
    //modify by:zyc. merge into create.
    //mor_->setColor(ccc3(0xff,0xc2,0x00));
    mor_->setPosition(ccpAdd(def_->getPosition(), ccp(0,-43)));
    this->addChild(mor_);
    
    SGCCLabelTTF *xuli_ = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , ccc3(0xff,0xc2,0x00));
    xuli_->setPosition(ccpAdd(atk_->getPosition(), ccp(230, 0)));
    this->addChild(xuli_);
    //modify by:zyc. merge into create.
    //xuli_->setColor(ccc3(0xff,0xc2,0x00));
    
    SGCCLabelTTF *sudu_ = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 28 , ccc3(0xff,0xc2,0x00));
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
    
    SGCCLabelTTF* font_tongyu = SGCCLabelTTF::create(str_shangzheng_need_tongyuli, FONT_BOXINFO, 26 , ccc3(0xff, 0xc2, 0x00));
    //modify by:zyc. merge into create.
    //font_tongyu->setColor(ccc3(0xff,0xc2,0x00));
    font_tongyu->setPosition(ccp(font_bg->getPosition().x - 21, font_bg->getPosition().y));
    this->addChild(font_tongyu);
    
    CCString* str_tongyu = CCString::createWithFormat("%d",m_gov);
    SGCCLabelTTF* tongyu = SGCCLabelTTF::create(str_tongyu->getCString(), FONT_BOXINFO, 26);
    tongyu->setAnchorPoint(ccp(0, 0.5));
    tongyu->setPosition(ccp(font_tongyu->getPosition().x + font_tongyu->getContentSize().width*.5 + 7, font_tongyu->getPosition().y));
    this->addChild(tongyu);
//六大button
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard)>0)//不是点击空的位置
    {
		int left_shift = 0;
		if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
            ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)//ipad
		{
			
            left_shift = 60;
		}
        if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)//ipad2
		{
			left_shift = 60;
		}
		
		//换人button
		SGButton *exchange = SGButton::createFromLocal("unhot.png", str_huanren, this, menu_selector(SGEmbattleBox::changeHandler), ccp(0, 5), FONT_PANGWA, ccWHITE, 35);
		exchange->setScaleX(0.9);
		exchange->setPosition(ccp(exchange->getContentSize().width - 15 + left_shift, tiao2->getPositionY() - exchange->getContentSize().height / 2 - 7));
		this->addBtn(exchange);
		//下场button
		SGButton *leave = SGButton::createFromLocal("unhot.png", str_xiachang, this, menu_selector(SGEmbattleBox::offHandler), ccp(0, 5), FONT_PANGWA, ccWHITE, 35);
		leave->setScaleX(0.9);
		leave->setPosition(ccp(exchange->getPositionX() + leave->getContentSize().width - 20, exchange->getPositionY()));
		this->addBtn(leave);
		//设为主将button
		SGButton *setMain = SGButton::createFromLocal("unhot.png", str_Set_to_main_role, this, menu_selector(SGEmbattleBox::setMainOfficerHandler), ccp(0, 5), FONT_PANGWA, ccWHITE, 30);
		setMain->setScaleX(0.9);
		setMain->setPosition(ccp(leave->getPositionX() + setMain->getContentSize().width - 30, exchange->getPositionY()));
        int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_41);
        setMain->setTag(modelTag);
		this->addBtn(setMain);
		//强化button
		SGButton *streng = SGButton::createFromLocal("unhot.png", str_qianghua, this, menu_selector(SGEmbattleBox::strengOfficer), ccp(0, 5	), FONT_PANGWA, ccWHITE, 35);
		streng->setScaleX(0.9);
		streng->setPosition(ccp(exchange->getPositionX(), exchange->getPositionY() - streng->getContentSize().height + 10));
		this->addBtn(streng);
		//转生button
		SGButton *advance = SGButton::createFromLocal("unhot.png", str_zhuansheng, this, menu_selector(SGEmbattleBox::advanceOfficer), ccp(0, 5), FONT_PANGWA, ccWHITE, 35);
		advance->setScaleX(0.9);
		advance->setPosition(ccp(leave->getPositionX(), leave->getPositionY() - advance->getContentSize().height + 10));
		this->addBtn(advance);
		//换装button
		SGButton *exchangeEq = SGButton::createFromLocal("unhot.png", str_huanzhuang, this, menu_selector(SGEmbattleBox::exchangeEquip), ccp(0, 5), FONT_PANGWA, ccWHITE, 35);
		exchangeEq->setScaleX(0.9);
		exchangeEq->setPosition(ccp(setMain->getPositionX(), setMain->getPositionY() - exchangeEq->getContentSize().height + 10));
		this->addBtn(exchangeEq);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	int modelTag = 0;
	//取消button
	int dis = -265;
	int left = 5;
	if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard)<=0)
	{
		left = 115;
		dis = -185;
		SGButton *upBtn = SGButton::create("box_btnbg.png",
										   "font_pqshangzhen.png",
										   this,
										   menu_selector(SGEmbattleBox::upHandler),
										   ccp(0, 0),
										   false,
										   true);
		
		this->addBtn(upBtn);
        upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-110, dis)));
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_38);
        upBtn->setTag(modelTag);
	}
	
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGEmbattleBox::cancelAction),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
	cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(left, dis)));
    
    if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard)>0)
    {
//        CCSprite* title = CCSprite::createWithSpriteFrameName("font_glszdwj.png");
//        title->setPosition(biao->getPosition());
        //this->addChild(title,5);
        
//         SGButton *setman = SGButton::createFromLocal("unhot.png", "设为主将", this, menu_selector(SGEmbattleBox::setMainOfficerHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
//         this->addBtn(setman);
//        setman->setPosition(ccpAdd(portrait->getPosition(), ccp(325, -portrait->getContentSize().height*.3)));
//
//        setman->setVisible(!(SGTeamgroup::shareTeamgroupInfo()->islord(emtype, officerCard)));
//        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_41);
//        setman->setTag(modelTag);
        
//        SGButton *offBtn = SGButton::create("box_btnbg.png",
//                                            "font_xiachang.png",
//                                            this,
//                                            menu_selector(SGEmbattleBox::offHandler),
//                                            ccp(0, 0),
//                                            false,
//                                            true);
        //this->addBtn(offBtn);
        //offBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -410)));
        
//        SGCCLabelTTF* label2 = SGCCLabelTTF::create("要让这名武将下场休息吗？", FONT_BOXINFO, 32);
//       // label2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -285)));
//        label2->setPosition(ccpAdd(font_tongyu->getPosition(),ccp(-offBtn->getContentSize().width*0.5,-offBtn->getContentSize().height*1.2)));
//        this->addChild(label2);
        
        //cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(182, -410)));
        
//        SGButton *huanren = SGButton::create("box_btnbg.png",
//                                             "barrack_font_huanren.png",
//                                             this,
//                                             menu_selector(SGEmbattleBox::changeHandler),
//                                             ccp(0, 0),
//                                             false,
//                                             true);
        //this->addBtn(huanren);
        //huanren->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-182, -410)));

//    }
//    if(SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard)<=0)
//    {
////        CCSprite* title1 = CCSprite::createWithSpriteFrameName("font_xzszzj.png");
////        title1->setPosition(biao->getPosition());
////        this->addChild(title1,5);
//        SGButton *upBtn = SGButton::create("box_btnbg.png",
//                                           "font_pqshangzhen.png",
//                                           this,
//                                           menu_selector(SGEmbattleBox::upHandler),
//                                           ccp(0, 0),
//                                           false,
//                                           true);
        //this->addBtn(upBtn);
        //upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-145, -410)));
        //modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_38);
        //upBtn->setTag(modelTag);
        
        //SGCCLabelTTF* label1 = SGCCLabelTTF::create("可以派遣这名武将上阵", FONT_BOXINFO, 32);
        //label1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -320)));//-285
        //this->addChild(label1);
        
        //cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(145, -410)));
    
    }
  
//    this->addLabel(atk->getPosition(), m_atk);
//    this->addLabel(def->getPosition(), m_def);
//    this->addLabel(shiqi->getPosition(), m_mor);
//    this->addLabel(xuli->getPosition(), m_round);
//    this->addLabel(speed->getPosition(), m_speed);

}
static ERI *info = NULL;//进入其他界面返回所需要的信息
ERI* SGEmbattleBox::shareEriInfo(void)
{
    if (!info) {
        info = new ERI();
    }
    return info;
}
//强化回调
void SGEmbattleBox::strengOfficer(CCObject *obj)
{
	//设置对应的返回界面所需要的信息
	ERI *info = shareEriInfo();
	info->em = emtype;
	info->ty = 0;
	info->govern = totleGovern;
	info->isFromEmbattle = true;
	
	officerCard->retain();//保留
    this->boxClose();
	SGMainManager::shareMain()->showStrengLayer(officerCard, 1, false, false, 1000, info);
    //officerCard->release();
	//delete info;
}
//转生回调
void SGEmbattleBox::advanceOfficer(CCObject *obj)
{
	//设置对应的返回界面所需要的信息
	ERI *info = shareEriInfo();
	info->em = emtype;
	info->ty = 0;
	info->govern = totleGovern;
	info->isFromEmbattle = true;
	
	if (officerCard->getMaxStar() != 0)
	{
		this->boxClose();
		SGMainManager::shareMain()->showAdvanceLayer(officerCard, 1, false, NULL, info);
	}
	else
	{
		SGMainManager::shareMain()->showMessage(str_EmbattlBox_str1);
	}
	//delete info;
}
//换装备回调
void SGEmbattleBox::exchangeEquip(CCObject *obj)
{
	//设置对应的返回界面所需要的信息
	ERI *info = shareEriInfo();
	info->em = emtype;
	info->ty = 0;
	info->govern = totleGovern;
	info->isFromEmbattle = true;
	
	SGTeamgroup* sgtg = SGTeamgroup::shareTeamgroupInfo();
	CCArray* ofcList = sgtg->getEmbattleOfficers(this->emtype);
	SGGeneralInfoLayer::setMyOfficerList(ofcList);
	this->boxClose();

//	SGMainManager::shareMain()->showEquipsLayer(officerCard, 0, 1);
	SGMainManager::shareMain()->showGeneralInfoLayer(officerCard, NULL, info);
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
}

void SGEmbattleBox::setMainOfficerListener(CCObject *obj)
{
    CCLOG("setMainOfficerListener");
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::DispatchOfficerResponse *response = (main::DispatchOfficerResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCLOG("上场成功");
            //设置武将保护
            officerCard->setIsProt(1);
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            this->boxClose();
            SGMainManager::shareMain()->showEmbattleLayer(0,(response->teamid()+1));
            
        }
        else
        {
            CCLOG("上场失败");
        }
        
    }
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
    SGMainManager::shareMain()->updateEmbattleInfo();
}
void SGEmbattleBox::offMainOfficerListener(CCObject *obj)
{
    CCLOG("offMainOfficerListener");
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        
        main::RemoveOfficerResponse *response = (main::RemoveOfficerResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCLOG("下场成功%d",officerCard->retainCount());            
            officerCard->setIsProt(0);
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_EmbattlBox_str2,officerCard->getOfficerName()->getCString())->getCString());
            emtype = (response->teamid()+1);

            if (SGTeamgroup::shareTeamgroupInfo()->islord(emtype,officerCard)) {

                CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
                SGOfficerCard *card1 = (SGOfficerCard*)array->objectAtIndex(0);
                for (int i = 1; i<array->count(); i++) {
                    SGOfficerCard* card2 = (SGOfficerCard*)array->objectAtIndex(i);
                    int p1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype, card1);
                    int p2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype, card2);
                    card1 = p1 <p2 ?card1:card2;

            
                }
                //card1->setIsLord(true);
                //officerCard->setIsLord(false);
            }
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            this->boxClose();
        }
        else
        {            
            CCLOG("下场失败");
        }
    }
    SGMainManager::shareMain()->updateEmbattleInfo();
}
void SGEmbattleBox::setLordOfficerListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj; 
    if (sr)
    {
        main::SetLordResponse *response = (main::SetLordResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::createWithFormat("%d",officerCard->getItemId()), "lordid");
            dict->setObject(CCString::create("lord"), "type");
            data = dict;
            
//            SGTeamgroup::shareTeamgroupInfo()->setlordId(emtype,officerCard->getSsid());
//            CCLog("@@@%d",SGTeamgroup::shareTeamgroupInfo()->array[emtype].getLordSkill());
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_EmbattlBox_str3,officerCard->getOfficerName()->getCString())->getCString());
            this->boxClose();
            
            CCLOG("设为主将成功");
            SGGuideManager::shareManager()->checkIsDone(guide_tag_42);
        }
        else
        {
            CCLOG("设为主将失败");
        }
    }
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
    SGMainManager::shareMain()->updateEmbattleInfo();
}

void SGEmbattleBox::setMainOfficerHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
	
    if (SGTeamgroup::shareTeamgroupInfo()->islord(emtype,officerCard)) {
        return;
    }
    main::SetLordRequest *request = new main::SetLordRequest();
    request->set_ssid(officerCard->getSsid());
    request->set_teamid(emtype-1);
    SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_SETLORD, request);
}
void SGEmbattleBox::upHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel());
//    if (playerInfo->getPlayerCountry() == 1) {
//        if (playerInfo->getEmbattleOfficers()->count() < player->getPlayerOfficerMax() && (m_gov + totleGovern)<= player->getPlayerGoverning()) {
//            main::DispatchOfficerRequest *request = new main::DispatchOfficerRequest();
//            request->set_ssid(officerCard->getSsid());
//            request->set_position(_positon);
//            SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_DISPATCH, request);
//        }else{
//            SGMainManager::shareMain()->showMessage("统御力超了");
//        }
//
//    }else
//    {
    if (SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype)->count() < player->getPlayerOfficerMax()&& (m_gov + totleGovern)<= player->getPlayerGoverning() + playerInfo->getAddPlayerGovering())
        {
            main::DispatchOfficerRequest *request = new main::DispatchOfficerRequest();
            request->set_ssid(officerCard->getSsid());
            request->set_position(_positon);
            request->set_teamid(emtype-1);
            SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_DISPATCH, request);
        }else{
            SGMainManager::shareMain()->showMessage(str_ArrangeLayer_str6);
        }

//    }
    
}
void SGEmbattleBox::offHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
	//SGEmbattleBox::boxClose();
//    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getShowOfficerCards(true);
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    if (array->count() == 1) {
        SGMainManager::shareMain()->showMessage(str_EmbattlBox_str4);
    }else{
		main::RemoveOfficerRequest *request = new main::RemoveOfficerRequest();
        request->set_position(SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard));
        request->set_teamid(emtype-1);
		SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_REMOVE, request);
    }
}
void SGEmbattleBox::showGeneralInfoLayer()
{
    //获得本地武将列表大全。
    SGTeamgroup* sgtg = SGTeamgroup::shareTeamgroupInfo();
    CCArray* ofcList = sgtg->getEmbattleOfficers(emtype);
    SGGeneralInfoLayer::setMyOfficerList(ofcList);
    EFFECT_PLAY(MUSIC_BTN);
    SGEmbattleBox::boxClose();
    //特殊初始化
    SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,15);
}
void SGEmbattleBox::changeHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard);
	SGArrangeLayer *layer = SGArrangeLayer::create(po,totleGovern,11,emtype);
    this->boxCloseWithOutCallBack();
    SGMainManager::shareMain()->showLayer(layer);
}
void SGEmbattleBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    //SGBaseBox::boxClose();//11.28
    //如果是统御力超过提示之后再取消，则返回选择武将界面
	
	if (this->fromWhere == 100)//从选择武将界面来
	{
		int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard);
		SGArrangeLayer *layer = SGArrangeLayer::create(po,totleGovern,11,emtype);
		this->boxCloseWithOutCallBack();
		SGMainManager::shareMain()->showLayer(layer);
	}
	else
	{
		SGBaseBox::boxClose();
		 
	}
}

void SGEmbattleBox::cancelAction()
{
	EFFECT_PLAY(MUSIC_BTN);
    //SGBaseBox::boxClose();//11.28
    //如果是统御力超过提示之后再取消，则返回选择武将界面
	
	if (this->fromWhere == 100)//从选择武将界面来
	{
		SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
		SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
		if (mainLayer) {
			mainLayer->setIsCanTouch(true,false);
		}
		int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,officerCard);
		SGArrangeLayer *layer = SGArrangeLayer::create(po,totleGovern,11,emtype);
		this->boxClose();
		SGMainManager::shareMain()->showLayer(layer);
	}
	else
	{
		 SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
		 SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
		 if (mainLayer) {
			 mainLayer->setIsCanTouch(true,false);
		 }
		 // 为了进入其他界面后再原路返回
		 SGEmbattleLayer *embattleLayer = (SGEmbattleLayer *)mainScene->getChildByTag(sg_embattleLayer);
		 if (!embattleLayer)
		 {
		 embattleLayer = SGEmbattleLayer::create(0,emtype);
		 }
		 mainLayer->showblack();
		 //mainLayer->setPortraitMenuCanTouch(false);
		 SGBaseBox::boxClose();//11.28
		 //this->boxCloseWithOutCallBack();//11.28
		 mainScene->addShowLayer(embattleLayer);
		
	}
}
void SGEmbattleBox::addLabel(cocos2d::CCPoint ponit, int m_value)
{
    if (m_value >0) {
        SGCCLabelTTF *a = SGCCLabelTTF::create(CCString::createWithFormat("+%d",m_value)->getCString(), FONT_BOXINFO, 28 , COLOR_GREEN);
        this->addChild(a);
        a->setAnchorPoint(ccp(0, 0.5));
        //modify by:zyc. merge into create.
        //a->setColor(COLOR_GREEN);
        a->setPosition(ponit);
    }
    
}

