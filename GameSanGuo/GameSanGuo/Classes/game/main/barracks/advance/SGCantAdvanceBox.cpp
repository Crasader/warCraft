//  SGCantAdvanceBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-21.
//
//

#include "SGCantAdvanceBox.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "itemmodule.pb.h"
#include "ShopModule.pb.h"
#include "SGMsgId.h"
#include "SGStaticDataManager.h"
#include "SGMainLayer.h"
#include "SGServerListLoginGame.h"
#include "SGPlayerInfoLayer.h"
#include "SGFirstStrengBox.h"
#include "SGFightRewardLayer.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGBattleManager.h"
#include "SGSoulExchangeLayer.h"
#include "SurvivalModule.pb.h"
#include "SGStringConfig.h"
//#include "PYUserSDKLib.h"
#include "SGShouJingZhou.h"
#include "SGAIManager.h"
#include "SGComplainLayer.h"
#include "SGStaticDataManager.h"
#include "SGShareBox.h"
#include "FightBaseModule.pb.h"
#include "SGPlayerDataModel.h"
#include "RewardModule.pb.h"
#include "SGBattleMainLayer.h"
#include "SGMallLayer.h"
#include "ArenaModule.pb.h"
#if (PLATFORM == IOS)
	#include "OcCaller.h"
#endif
#include "SGRichLabel.h"

SGCantAdvanceBox::SGCantAdvanceBox()
:ok(NULL),
cancel(NULL),
store(NULL),
need(0),
_card(NULL),
_dele(NULL),
enterType(0),
warn(NULL),
_data(NULL),
neenGold(0)
{
    
}
SGCantAdvanceBox::~SGCantAdvanceBox()
{
	
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    _dele = NULL;
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    
    notification->removeObserver(this, MSG_STORE_ENTER);
    
    notification->removeObserver(this, MSG_EXCHANGE_ENTER);
    notification->removeObserver(this, MSG_PVE_FULLPOWER);
    notification->removeObserver(this, MSG_PVP_FULLPOWER);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_canAdvanceTag);
}

/** 同志们把type(即enterType)给注释加在这里吧，不然真是一头雾水。
 *1、18=武将满星不能进阶
 * 36=新手引导未完成的弹窗提示
 
 * 50=守荆州可以跳过该关卡，弹出三按钮：“跳过”“不跳”“取消”。
 * 51=军魂换礼战绩提示，弹出单按钮：“确定”
 * 52=放弃守荆州战斗的二次确认，弹出双按钮：“确定放弃”，“继续战斗”
 * 53=取回军魂的消费确认，弹出双按钮：“确定”，“取消”
 * 54=取回军魂元宝不够的储值提示，弹出按钮：”去储值“，“取消”
 * 55=军魂领奖的奖品提示界面，弹出按钮：“确定”
 * 56=守荆州每日结算奖励提示界面，弹出按钮：“确定”
 * 57= boss战结束结算提示界面
 * 41=分羹一杯
 * 60=取回军魂但三种军魂注入管的军魂值均为0
 * 100 强化选卡的二次确认
 * 101 转生武将等级不足
 * 102 转生武将材料不足
 * 103 转生装备等级不足
 * 104 转生装备材料不足
 * 105 pvp连胜奖励说明
 * 201 新商城购买物品确认
 * 219 游客账号绑定页面 增加使用老账号登入 确认放弃游客资料弹框
 * 1919 成长计划活动
 * 2014 月签奖励提示购买VIP可再度领取。
 */


SGCantAdvanceBox *SGCantAdvanceBox::create(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,int type,int Coins,int power, CCObject *obj,int Golds,const char *showMessage)
{
    SGCantAdvanceBox *cantaAvanceBox = new SGCantAdvanceBox();
    cantaAvanceBox->_dele = bdg;
    if (cantaAvanceBox && cantaAvanceBox->init(bdg,card,type,Coins,power, obj,Golds,showMessage))
    {
        cantaAvanceBox->autorelease();
        return cantaAvanceBox;
    }
    CC_SAFE_DELETE(cantaAvanceBox);
    return NULL;
}
bool SGCantAdvanceBox::init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,int type,int Coins,int power, CCObject *obj,int Golds,const char *showMessage)
{
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_box_canAdvanceTag);
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_box_canAdvanceTag);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_box_canAdvanceTag, LIM_PNG_AS_PNG);
    
    enterType = type;
    need = Coins;
    neenGold = Coins;
    _card = card;
    _data = obj;
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EXCHANGE_ENTER,
                              this,
                              callfuncO_selector(SGCantAdvanceBox::exchangeEnterListener));
    
    notification->addObserver(MSG_STORE_ENTER,
                              this,
                              callfuncO_selector(SGCantAdvanceBox::storeEnterListener));
    
    notification->addObserver(MSG_PVE_FULLPOWER, this, callfuncO_selector(SGCantAdvanceBox::fullPvEPowerListener));
    notification->addObserver(MSG_PVP_FULLPOWER, this, callfuncO_selector(SGCantAdvanceBox::fullPvPPowerListener));
    
    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    float wid = 535;
    float hgt = 340;

    
    if (type == 13 ||type == 14 || type == 1  || type == 18||type == 6 || type == 9 || type == 10 || type == 11 || type == 17||type == 19 ||type ==100 ||type == 101 ||type == 102 ||type == 103 ||type ==104  ||type == 100100)
    {
        hgt = 340;//这个高度不要再小了
    }
    else if (type== 25)
    {
        hgt = 400;
    }
	else if (type == 105)
    {
		hgt = 510;
    }
    
    if (!SGBaseBox::init(bdg,box_canAdvanceTag,CCSizeMake(wid, hgt)))
    {
        return false;
    }
    if (type ==1 || type == 18 ||type == 101 ||type ==102 ||type ==103 ||type == 104) {
		//武将满星不能进阶
		
		ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::boxCloseWithOutCallBack),
							  ccp(0, 0),
							  false,
							  true);
		this->addBtn(ok);
		ok->setPosition(ccpAdd(center, ccp(0, -265/2+10)));
		
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_No_if_jingjie, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc  merge into create.
//        title->setOutSideColor(ccc3(143,87,0));
		title->setPosition(biao->getPosition());
		this->addChild(title);
        SGCCLabelTTF* a = NULL;
        if (type == 1) {
            a = SGCCLabelTTF::create(str_the_role_up_border, FONT_PANGWA, 32);
        }
        else if(type == 101)
        {
			a = SGCCLabelTTF::create(str_the_role_zhuansheng_no_class, FONT_PANGWA, 32);
        }
        else if (type == 102)
        {
			a = SGCCLabelTTF::create(str_no_material_zhuansheng, FONT_PANGWA, 32);
        }
        else if (type == 103)
        {
            a = SGCCLabelTTF::create(str_no_zhuangbei_class, FONT_PANGWA, 32);
        }
        else if (type == 104)
        {
            a = SGCCLabelTTF::create(str_no_material_zhuansheng_zhuangbei, FONT_PANGWA, 32);
        }
        else
        {
            a = SGCCLabelTTF::create(str_no_this_zhuangbei_star_up_border, FONT_PANGWA, 32);
        }
		this->addChild(a);
		a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
		SGCCLabelTTF*b = SGCCLabelTTF::create(str_up_star, FONT_PANGWA, 32);
		this->addChild(b);
		b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -50)));
    }
    
    else if (type == 105)
    {
        ok = SGButton::create("box_btn_newbg.png",
                              "public_font_queding.png",
                              this,
                              menu_selector(SGCantAdvanceBox::boxCloseWithOutCallBack),
                              ccp(0, 0),
                              false,
                              true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(center, ccp(0, -265/2-75)));
        
        SGCCLabelTTF* a = NULL;
		
        a = SGCCLabelTTF::create(str_lian_sheng_shuoming, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc  merge into create.
        //a->setOutSideColor(ccc3(143,87,0));
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -10)));
        for (int i=0;i<5;i++)
        {
            SGJunbuff *buff = SGStaticDataManager::shareStatic()->getjindutiaobyid(i);
            CCString *str1 = CCString::createWithFormat(str_Format_lian_sheng,buff->getMin(),buff->getMax());
            CCString *str2=NULL;
            if (buff->getScore()==0) {
                str2 = CCString::create(str_CantAdvance_tips1);
            }
            else
            {
                str2 = CCString::createWithFormat(str_CantAdvance_tips2,buff->getScore());
            }
            CCString *str = CCString::createWithFormat("%s%s",str1->getCString(),str2->getCString());
            
            SGCCLabelTTF* x = SGCCLabelTTF::create(str->getCString(), FONT_PANGWA, 25);
            x->setAnchorPoint(ccp(0,0.5));
            x->setPosition(ccpAdd(biao->getPosition(), ccp(-220, -70-i*50)));
            this->addChild(x);
            
			
        }
        SGCCLabelTTF* ts = SGCCLabelTTF::create(str_CantAdvance_tips3, FONT_PANGWA, 25);
        ts->setAnchorPoint(ccp(0,0.5));
        ts->setPosition(ccpAdd(biao->getPosition(), ccp(-220, -60-5*50-10)));
        this->addChild(ts);
        
        SGCCLabelTTF* ts2 = SGCCLabelTTF::create(str_CantAdvance_tips4, FONT_PANGWA, 25);
        ts2->setAnchorPoint(ccp(0,0.5));
        ts2->setPosition(ccpAdd(ts->getPosition(), ccp(85, -30)));
        this->addChild(ts2);
    
    }
    else if (type == 2)
    {

    }
    else if (type == 3)
    {
		// 强卡钱不够
        ok = SGButton::create("box_btn_newbg.png",
							  "btn_qdh.png",
							  this,
							  menu_selector(SGCantAdvanceBox::noCoinsHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::onlyclosebox),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -324/2+35)));
        cancel->setPosition(ccpAdd(center, ccp(145, -324/2+35)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips4_nomoney, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        CCString *str_lab = CCString::createWithFormat(str_CantAdvance_tips5,need);
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_lab->getCString(), FONT_PANGWA, 32);
        a->setAnchorPoint(ccp(0, 0.5f));
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(-215, -75)));
		
        SGCCLabelTTF* b = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips7,SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString(), FONT_PANGWA, 32);
        b->setAnchorPoint(ccp(0, 0.5f));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(0,- 50)));
        SGCCLabelTTF* c = SGCCLabelTTF::create(str_CantAdvance_tips9, FONT_PANGWA, 32);
        this->addChild(c);
        c->setAnchorPoint(ccp(0, 0.5f));
        c->setPosition(ccpAdd(b->getPosition(), ccp(0, - 50)));
		
    }
    else if (type == 4)
    {
		//   进阶钱不够
        ok = SGButton::create("box_btn_newbg.png",
							  "btn_qdh.png",
							  this,
							  menu_selector(SGCantAdvanceBox::noCoinsHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
		
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::onlyclosebox),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
        cancel->setPosition(ccpAdd(center, ccp(145, -324/2+40)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips4_nomoney, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        CCString *str_lab = CCString::createWithFormat(str_CantAdvance_tips10,need);
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_lab->getCString(), FONT_PANGWA, 32);
        a->setAnchorPoint(ccp(0, 0.5f));
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(-215, -70)));
        
        SGCCLabelTTF* b = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips7,SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString(), FONT_PANGWA, 32);
        b->setAnchorPoint(ccp(0, 0.5f));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(0,- 50)));
        SGCCLabelTTF* c = SGCCLabelTTF::create(str_CantAdvance_tips9, FONT_PANGWA, 32);
        this->addChild(c);
        c->setAnchorPoint(ccp(0, 0.5f));
        c->setPosition(ccpAdd(b->getPosition(), ccp(0, - 50)));
		
    }
    
    else if (type == 6)
    {
		//原为最后一张武将卡不能卖，现在修改为"上阵武将卡都不能卖，解除保护也不能卖。"
        
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::boxCloseWithOutCallBack),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
		ok->setPosition(ccpAdd(center, ccp(0, -265/2+5 -40)));
        
        CCSprite *title = CCSprite::createWithSpriteFrameName("font_cantsell.png");
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips11, FONT_PANGWA, 32); //这是您的最后一张上场武将卡,
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
        SGCCLabelTTF*b = SGCCLabelTTF::create(str_CantAdvance_tips12, FONT_PANGWA, 32);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -50)));
		
    }
    else if (type == 7)
    {
		//填写邀请码成功  未确定
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::boxCloseWithOutCallBack),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.7+40)));
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.7+40)));
        
        CCSprite *title = CCSprite::createWithSpriteFrameName("font_acceptinvite.png");
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.27)));
        this->addChild(title);
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips13, FONT_PANGWA, FONTSIZE(12));
        this->addChild(a);
        a->setPosition(SGLayout::getPoint(kMiddleCenter));
        SGCCLabelTTF*b = SGCCLabelTTF::create(str_CantAdvance_tips14, FONT_PANGWA, FONTSIZE(12));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(0, -a->getContentSize().height*1.5)));
    }
    else if (type == 8)
    {
		//重温新手引导
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::boxCloseWithOutCallBack),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
		
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -324/2+5 -40)));
        cancel->setPosition(ccpAdd(center, ccp(145, -324/2+5-40)));
		
        CCSprite *title = CCSprite::createWithSpriteFrameName("font_restart.png");
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        CCString *str_lab = CCString::createWithFormat(str_CantAdvance_tips15,need);
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_lab->getCString(), FONT_PANGWA, 32);
        a->setAnchorPoint(ccp(0, 0.5f));
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(-215, -90)));
        
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips16, FONT_PANGWA, 32);
        b->setAnchorPoint(ccp(0, 0.5f));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(0,- 50)));
        SGCCLabelTTF* c = SGCCLabelTTF::create(str_CantAdvance_tips17, FONT_PANGWA, 32);
        this->addChild(c);
        c->setAnchorPoint(ccp(0, 0.5f));
        c->setPosition(ccpAdd(b->getPosition(), ccp(0, - 50)));
		
    }
    else if (type == 9)
    {
        //金子换铜钱 金子不够
        ok = SGButton::createFromLocal("box_btn_newbg.png",
                                       str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::showAddGold),CCPointZero,FONT_PANGWA,ccWHITE,32);
        this->addBtn(ok);
		
        cancel = SGButton::create("box_btn_newbgHui.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips18, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
		
		
        SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips19,neenGold)->getCString(), FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
        CCString *str = CCString::createWithFormat(str_CantAdvance_tips199,SGPlayerInfo::sharePlayerInfo()->getPlayerGold());
        SGCCLabelTTF* b = SGCCLabelTTF::create(str->getCString(), FONT_PANGWA, 32);
        this->addChild(b);
        b->setAnchorPoint(ccp(0, 0.5f));
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width*.5f, -50)));
		
    }
    else if (type == 10)
    {
        //购买礼包 金子不够
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::showAddGold),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10)));
		
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips20, FONT_PANGWA, 40 , ccWHITE ,ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
		
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips21,need)->getCString(), FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
        CCString *str = CCString::createWithFormat(str_CantAdvance_tips199,SGPlayerInfo::sharePlayerInfo()->getPlayerGold());
        SGCCLabelTTF* b = SGCCLabelTTF::create(str->getCString(), FONT_PANGWA, 32);
        this->addChild(b);
        b->setAnchorPoint(ccp(0, 0.5f));
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width*.5f, -50)));
        
	}
    else if (type == 11)
    {
        //购买礼包 vip等级不够
        ok =   SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::showAddGold),CCPointZero,FONT_PANGWA,ccWHITE,32);
        

        this->addBtn(ok);
        cancel = SGButton::create("box_btn_newbgHui.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips20, FONT_PANGWA, 40 ,ccWHITE ,  ccc3(143,87,0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
		
        SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips22,need)->getCString(), FONT_PANGWA, 32);
        this->addChild(a);
        a->setAnchorPoint(ccp(0, 0.5f));
        a->setPosition(ccpAdd(biao->getPosition(), ccp(-215, -90)));
        
        
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips23, FONT_PANGWA, 32);
        this->addChild(b);
        b->setAnchorPoint(ccp(0, 0.5f));
        b->setPosition(ccpAdd(a->getPosition(), ccp(0, -50)));
		
    }
    else if (type == 12)
    {
        //暴仓
        ok = SGButton::create("box_btn_newbg.png",
							  "font_sjjuny.png",
							  this,
							  menu_selector(SGCantAdvanceBox::expandBarrack),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.34, -hgt*.52+50)));
        
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.34, -hgt*.52+50)));
        
        store = SGButton::create("box_btn_newbg.png",
								 "font_glkapian.png",
								 this,
								 menu_selector(SGCantAdvanceBox::managerBarrack),
								 ccp(0, 0),
								 false,
								 true);
        this->addBtn(store);
        store->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.52+50)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips24, FONT_PANGWA, 40 , ccWHITE ,ccc3(143,87,0));
        //modify by:zyc. merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips25, FONT_PANGWA, 28);
        this->addChild(a);
        a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 22)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips26,need,SGPlayerInfo::sharePlayerInfo()->getPlayerBagSize())->getCString(), FONT_PANGWA, 28);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -a->getContentSize().height*1.5 )));
        SGCCLabelTTF* c = SGCCLabelTTF::create(str_CantAdvance_tips27, FONT_PANGWA, 28);
        this->addChild(c);
        c->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + c->getContentSize().width/2, -a->getContentSize().height*1.5)));
    }
    else if(enterType == 13)
    {
		//扩建军营钱不够
        ok =   SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::showAddGold),CCPointZero,FONT_PANGWA,ccWHITE,32);

        this->addBtn(ok);
        
        cancel = SGButton::create("box_btn_newbgHui.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10)));
		
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
        CCString *str_gold = CCString::createWithFormat(str_CantAdvance_tips29,playerInfo->getPlayerGold());
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_gold->getCString(), FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(center, ccp(0, 0)));
    }
    else if(enterType == 14)
    {
        //强化素材卡有装备
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::strengConfirmHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
		
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGBaseBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
        // modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips30_1, FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
        SGCCLabelTTF*b = SGCCLabelTTF::create(str_CantAdvance_tips30_2, FONT_PANGWA, 32);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -40)));
		
    }
    else  if(type == 100)
    {
        //强化素材卡有装备
        ok = SGButton::create("box_btn_newbg.png",
                              "public_font_queding.png",
                              this,
                              menu_selector(SGCantAdvanceBox::strengsecondconfirm),
                              ccp(0, 0),
                              false,
                              true);
        this->addBtn(ok);
        
        cancel = SGButton::create("box_btn_newbg.png",
                                  "public_font_quxiao.png",
                                  this,
                                  menu_selector(SGBaseBox::boxClose),
                                  ccp(0, 0),
                                  false,
                                  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10 )));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10 )));
        
		//        CCSprite *title = CCSprite::createWithSpriteFrameName("font_notice.png");
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips31, FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
        SGCCLabelTTF*b = SGCCLabelTTF::create(str_CantAdvance_tips32, FONT_PANGWA, 32);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -40)));
		
    }
    else  if(type == 100100)
    {
        //强化素材卡有装备
        ok = SGButton::create("box_btn_newbg.png",
                              "public_font_queding.png",
                              this,
                              menu_selector(SGCantAdvanceBox::strengsecondconfirm),
                              ccp(0, 0),
                              false,
                              true);
        this->addBtn(ok);
        
        cancel = SGButton::create("box_btn_newbg.png",
                                  "public_font_quxiao.png",
                                  this,
                                  menu_selector(SGBaseBox::boxClose),
                                  ccp(0, 0),
                                  false,
                                  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10 )));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10 )));
        
		//        CCSprite *title = CCSprite::createWithSpriteFrameName("font_notice.png");
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40, ccc3(143,87,0));
        // modify by: zyc. merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips31_1, FONT_PANGWA, 28);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        
        SGCCLabelTTF*b = SGCCLabelTTF::create(str_CantAdvance_tips31_2, FONT_PANGWA, 28);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -40)));
		
        SGCCLabelTTF*c = SGCCLabelTTF::create(str_CantAdvance_tips31_3, FONT_PANGWA, 28);
        this->addChild(c);
        c->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + c->getContentSize().width/2, -40)));
    }
    else if (type == 15)
    {
        //统御力不符合条件
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::boxClose),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.3f, -hgt*.75-10)));
        
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.3f, -hgt*.75-10)));
        
		//        CCSprite *title = CCSprite::createWithSpriteFrameName("font_barrackfull.png");
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips24, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.275)));
        this->addChild(title);
        float titleH = title->getContentSize().height;
        
        CCLabelTTF* a = CCLabelTTF::create(str_CantAdvance_tips33, FONT_PANGWA, FONTSIZE(12), CCSizeMake(wid-skewing(50), 0), kCCTextAlignmentLeft);
        this->addChild(a);
        a->setPosition(ccpAdd(title->getPosition(), ccp(-wid/2+a->getContentSize().width/2+skewing(25), -(titleH+skewing(12.5f*2))/2)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips34, FONT_PANGWA, FONTSIZE(12));
        this->addChild(b);
        b->setPosition(ccpAdd(title->getPosition(), ccp(-wid/2+b->getContentSize().width/2+skewing(25), -(titleH+skewing(12.5f*5))/2)));
    }
    else if (type == 16)
    {
        //体力不足 金子补满
        if (_data == NULL)
            return false;
        
        ok = SGButton::create("box_btn_newbg.png",
							  "btn_fullpower.png",
							  this,
							  menu_selector(SGCantAdvanceBox::getFullPvEPower),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        if (false) ok->setEnabled(false);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.75+40)));
        
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.75+40)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips35, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
        //modify by:zyc. merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
		
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.275)));
        this->addChild(title);
        float titleH = title->getContentSize().height;
        
        const char *text = CCString::createWithFormat(str_CantAdvance_tips36, ((CCString *)_data)->intValue(), 0)->getCString();
        // labelttf字符的高度为25, 12.5 = height/2
        SGCCLabelTTF* a = SGCCLabelTTF::create(text, FONT_PANGWA, FONTSIZE(12));
        this->addChild(a);
        a->setPosition(ccpAdd(title->getPosition(), ccp(-wid/2+a->getContentSize().width/2+skewing(5), -(titleH+skewing(12.5f*1))/2)));
        
        // 储值
        CCSprite *redImg = CCSprite::createWithSpriteFrameName("bigredbg.png"); //CCRectMake(3, 0, wid-5, skewing(35))
        redImg->setScaleX((wid-2)/redImg->getContentSize().width);
        redImg->setScaleY(skewing(35)/redImg->getContentSize().height);
        redImg->setContentSize(CCSizeMake(wid, skewing(35)));
        redImg->setAnchorPoint(ccp(0.5f/redImg->getScaleX(), 0.5f/redImg->getScaleY()));
        redImg->setPosition(ccpAdd(title->getPosition(), ccp(1, -(titleH+skewing(12.5f*2))/2-redImg->getContentSize().height/2-skewing(1.5f))));
        this->addChild(redImg);
        
        store = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::showAddGold),CCPointZero,FONT_PANGWA,ccWHITE,32);
        

        this->addBtn(store);
        store->setPosition(ccpAdd(redImg->getPosition(), ccp(wid/2-store->getContentSize().width/2-3, -10)));
        
        text = CCString::createWithFormat(str_CantAdvance_tips37, Coins)->getCString();
        SGCCLabelTTF* b = SGCCLabelTTF::create(text, FONT_PANGWA, FONTSIZE(12));
        this->addChild(b);
        b->setPosition(ccpAdd(redImg->getPosition(), ccp(-wid/2+b->getContentSize().width/2+skewing(5), redImg->getContentSize().height/2-b->getContentSize().height/2-skewing(3))));
        
        int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
        SGCCLabelTTF* c = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips38, gold)->getCString(), FONT_PANGWA, FONTSIZE(12));
        this->addChild(c);
        c->setPosition(ccpAdd(redImg->getPosition(), ccp(-wid/2+c->getContentSize().width, -redImg->getContentSize().height/2+c->getContentSize().height/2+skewing(3))));
        
        warn = CCLabelTTF::create(str_CantAdvance_tips39, FONT_PANGWA, FONTSIZE(10));
        warn->setColor(ccRED);
        warn->setVisible(false);
        warn->setPosition(ccpAdd(redImg->getPosition(), ccp(-wid/2+warn->getContentSize().width/2+skewing(8), -redImg->getContentSize().height+warn->getContentSize().height/2+skewing(3))));
        this->addChild(warn);
    }
    else if (type == 17)
    {
        //炼丹炉 炼丹秘方不够
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::enterVisitHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);

        ok->setPosition(ccpAdd(center, ccp(0, -265/2+10)));

        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips40, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips41, FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -120)));

    }
    else if (type == 18)
    {
        //第一次迎财神
        ok = SGButton::create("box_btn_newbg.png",
							  "font_qw.png",
							  this,
							  menu_selector(SGCantAdvanceBox::boxCloseWithOutCallBack),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.75-10)));
		
        CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_csd.png");
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25)));
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips43, FONT_PANGWA, FONTSIZE(12));
        this->addChild(a);
        a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.05)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips44, FONT_PANGWA, FONTSIZE(12) , ccRED);
        this->addChild(b);
        //modify by:zyc. merge into create.
        //b->setColor(ccRED);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2,- a->getContentSize().height*1.5)));
        
        SGCCLabelTTF*c = SGCCLabelTTF::create(str_CantAdvance_tips45, FONT_PANGWA, FONTSIZE(12));
        this->addChild(c);
        c->setPosition(ccpAdd(b->getPosition(), ccp(b->getContentSize().width/2+c->getContentSize().width/2, 0)));
    }
    else if (type == 19)
    {
        //拜财神钱不够
        ok =  SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::showAddGold),CCPointZero,FONT_PANGWA,ccWHITE,32);
        
        this->addBtn(ok);
        cancel = SGButton::create("box_btn_newbgHui.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+5 )));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+5 )));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips46, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        CCString *str = CCString::createWithFormat(str_CantAdvance_tips199,SGPlayerInfo::sharePlayerInfo()->getPlayerGold());
        SGCCLabelTTF* b = SGCCLabelTTF::create(str->getCString(), FONT_PANGWA, 28);
        this->addChild(b);
        b->setPosition(ccpAdd(biao->getPosition(), ccp(0,-140)));
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips47,Coins)->getCString(),FONT_PANGWA, 28);
        this->addChild(a);
        a->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + a->getContentSize().width/2, 50)));
    }
    else if (type == 20 ||type == 22)
    {
        cancel = SGButton::createFromLocal("box_btn_newbg.png",
										   str_CantAdvance_tips49,//login_font_qczh.png
										   this,
										   menu_selector(SGCantAdvanceBox::gotoBindAccountLayer),
                                           CCPointZero,FONT_PANGWA,ccWHITE,32,
										   false,
										   true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.38)));
		
        
        SGCCLabelTTF * title =  SGCCLabelTTF::create(str_CantAdvance_tips50, FONT_PANGWA, 36 , ccWHITE , COLOR_TAN);
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.36)));
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips51, FONT_PANGWA, FONTSIZE(14));
        this->addChild(a);
        a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.08)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips52, FONT_PANGWA, FONTSIZE(14));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2,- a->getContentSize().height*1.5)));
        
    }
    else if (type == 21)
    {
        //无帐号登陆
        ok = SGButton::create("box_btn_newbg.png",
							  "public_font_queding.png",
							  this,
							  menu_selector(SGCantAdvanceBox::okHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(- wid*.2, -hgt*.75-10)));
        
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::cancelHandler),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.75-10)));

        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips53, FONT_PANGWA, FONTSIZE(12));
        this->addChild(a);
        a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.05)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips54, FONT_PANGWA, FONTSIZE(12));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2,- a->getContentSize().height*1.5)));
        
    }
    else if (type == 23)
    {
        //游客修改密码
        ok = SGButton::create("box_btn_newbg.png",
							  "login_font_sjzh.png",
							  this,
							  menu_selector(SGCantAdvanceBox::upAccountHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.75-10)));
        
        SGButton *buttonGoback = SGButton::create("public_btn_6.png","public_fanhui.png" , this, menu_selector(SGCantAdvanceBox::cancelHandler));
        this->addBtn(buttonGoback);
        buttonGoback->setAnchorPoint(ccp(0, 0.5));
        buttonGoback->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.43, hgt*.36)));
		
		//        CCSprite *title = CCSprite::createWithSpriteFrameName("login_font_notice.png");
        SGCCLabelTTF * title =  SGCCLabelTTF::create(str_CantAdvance_tips55, FONT_PANGWA, 36 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25)));
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips56, FONT_PANGWA, FONTSIZE(12));
        this->addChild(a);
        a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.05)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips57, FONT_PANGWA, FONTSIZE(12));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2,- a->getContentSize().height*1.5)));
    }
    else if (type == 24)
    {
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_queding.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.5-10)));
        
		//        CCSprite *title = CCSprite::createWithSpriteFrameName("font_nopower.png");
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips58, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* label = SGCCLabelTTF::create(str_CantAdvance_tips59, FONT_PANGWA, 27, CCSizeMake(wid*0.85f, hgt*0.35f));
        this->addChild(label);
        label->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 0)));
        
    }
    else if(type == 25)
    {
		//        炼丹炉开启
		//        ==================
		//        禀告主公。探马发现此地不远有一炼丹炉，是否要过去看看？
		//        （之后也可通过首页 – 领奖前往）
		//
		//        查看     取消

        ok = SGButton::create("box_btn_newbg.png",
							  "font_qw.png",
							  this,
							  menu_selector(SGCantAdvanceBox::liandanHandler),
							  ccp(0, 0),
							  false,
							  true);
        this->addBtn(ok);
        cancel = SGButton::create("box_btn_newbg.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+5 -40)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+5 -40)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips60, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips61, FONT_PANGWA, 32);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips62, FONT_PANGWA, 32);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2+3,- 50)));
		
    }
    else if (type == 27 || type == 28 || type == 29 || type == 31 || type == 32 || type == 33)//新版的体力/军粮不足
    {
        const char *file = "";
        if ( type == 28 || type == 29)
        {
            file = str_CantAdvance_tips35;
        }
        else if (type == 27)
        {
            file = str_CantAdvance_tips35_1;
        }
        else if(type == 31 )
        {
            file = str_CantAdvance_tips35_1;
        }
        else
        {
            file = str_CantAdvance_tips63;
        }
        SGCCLabelTTF* title = SGCCLabelTTF::create(file, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        file = "btn_fullpower.png";
        if (type == 27 || type == 31)
        {
            file = str_CantAdvance_tips35_2 ;//"btn_fullpower.png";
        }
        else if (type == 28 || type == 32)
        {
            if (SGPlayerInfo::sharePlayerInfo()->getShowvip())
            {
                file = "font_qw.png";
            }
            else
            {
				file = "public_font_queding.png";
            }
        }
        else if (type == 29 || type == 33)
        {
            file = str_CantAdvance_tips35_31;
        }
 
        
        
        if(type == 27 || type == 31 )
        {
            
            ok = SGButton::createFromLocal("box_btn_newbg.png", file, this, NULL,CCPointZero,FONT_PANGWA,ccWHITE,32);
            
            cancel = SGButton::createFromLocal("box_btn_newbg.png",
                                      str_CantAdvance_tips35_3,
                                      this,
                                      menu_selector(SGCantAdvanceBox::boxClose),CCPointZero,FONT_PANGWA,ccWHITE,32);
        }
        else if( type == 29 || type == 33)
        {
            ok = SGButton::createFromLocal("box_btn_newbg.png", file, this, NULL,CCPointZero,FONT_PANGWA,ccWHITE,32);
            
            cancel = SGButton::createFromLocal("box_btn_newbgHui.png",
                                               str_CantAdvance_tips35_3,
                                               this,
                                               menu_selector(SGCantAdvanceBox::boxClose),CCPointZero,FONT_PANGWA,ccWHITE,32);
        }
        else
        {
            ok = SGButton::create("box_btn_newbg.png", file, this, NULL, ccp(0, 0), false, true);
            
            cancel = SGButton::create("box_btn_newbg.png",
                                      "public_font_quxiao.png",
                                      this,
                                      menu_selector(SGCantAdvanceBox::boxClose),
                                      ccp(0, 0),
                                      false,
                                      true);
        }

        
        
        
        ok->setPosition(ccpAdd(center, ccp(-wid*.25f, -hgt*.55f+60)));
        this->addBtn(ok);
        if (type == 27)
        {
            ok->setTarget(this, menu_selector(SGCantAdvanceBox::getFullPvEPower));
        }
        else if (type == 31)
        {
            ok->setTarget(this, menu_selector(SGCantAdvanceBox::getFullPvPPower));
        }
        else if (type == 28 || type == 32 || type == 29 || type == 33)
        {
            if (SGPlayerInfo::sharePlayerInfo()->getShowvip() || type == 29 || type == 33)
            {
                ok->setTarget(this, menu_selector(SGCantAdvanceBox::showAddGold));
            }
            else
            {
				ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
            }
            
        }
        

        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(center, ccp(wid*.25f, -hgt*.55f+60)));
        
        
        const char *text = "";
        type < 30 && (file = str_CantAdvance_tips64);
        type > 30 && (file = str_CantAdvance_tips65);
        
        if (type == 27 || type == 29 || type == 31 || type == 33)
        {
            text = CCString::createWithFormat(str_CantAdvance_tips66, ((CCString *)_data)->getCString())->getCString();
            SGCCLabelTTF* a = SGCCLabelTTF::create(text, FONT_PANGWA, 28);
            a->setAnchorPoint(ccp(0, 0.5f));
            this->addChild(a);
            a->setPosition(ccpAdd(title->getPosition(), ccp(-wid*.5f + 50, -75)));
            
            float iconPx = 0;
            float factor = 0.32f;
            CCSprite *goldicon = NULL;
            
            //type < 30是体力
            if (type < 30)
            {
                //显示固定数字的体力,由服务器指定
                text = CCString::createWithFormat(str_CantAdvance_tips67_1, power)->getCString();
            }
            else if (type == 31)
            {
                text = CCString::createWithFormat(str_CantAdvance_tips67_2,power, file)->getCString();
            }
            else
            {
                text = CCString::createWithFormat(str_CantAdvance_tips67, file)->getCString();
            }
            
            int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
            SGCCLabelTTF* b = SGCCLabelTTF::create(text, FONT_PANGWA, 28);
            b->setAnchorPoint(ccp(0, 0.5f));
            this->addChild(b);
            b->setPosition(ccpAdd(a->getPosition(), ccp(0, -55)));
            
            goldicon = CCSprite::createWithSpriteFrameName("goldicon.png");
            iconPx = b->getContentSize().width + goldicon->getContentSize().width*factor;
            goldicon->setPosition(ccpAdd(b->getPosition(), ccp(iconPx, 0)));
            this->addChild(goldicon);
            
            text = CCString::createWithFormat(str_CantAdvance_tips68, Coins)->getCString();
            SGCCLabelTTF* bv = SGCCLabelTTF::create(text, FONT_PANGWA, 28 , ccYELLOW);
            //modify by:zyc . merge into create.
            //bv->setColor(ccYELLOW);
            bv->setAnchorPoint(ccp(0, 0.5f));
            this->addChild(bv);
            bv->setPosition(ccpAdd(goldicon->getPosition(), ccp(goldicon->getContentSize().width*factor, 0)));
            
            
            SGCCLabelTTF* c = SGCCLabelTTF::create(str_CantAdvance_tips69, FONT_PANGWA, 28);
            c->setAnchorPoint(ccp(0, 0.5f));
            this->addChild(c);
            c->setPosition(ccpAdd(b->getPosition(), ccp(0, -50)));
            
            goldicon = CCSprite::createWithSpriteFrameName("goldicon.png");
            iconPx = c->getContentSize().width + goldicon->getContentSize().width*factor;
            goldicon->setPosition(ccpAdd(c->getPosition(), ccp(iconPx, 0)));
            this->addChild(goldicon);
            
            text = CCString::createWithFormat(str_GoldShopLimitTimeSell_str11, gold)->getCString();
            SGCCLabelTTF* cv = SGCCLabelTTF::create(text, FONT_PANGWA, 28 , ccYELLOW);
            //modify by:zyc . merge into create.
            //cv->setColor(ccYELLOW);
            cv->setAnchorPoint(ccp(0, 0.5f));
            this->addChild(cv);
            cv->setPosition(ccpAdd(goldicon->getPosition(), ccp(goldicon->getContentSize().width*factor, 0)));
        }
        else
        {
            text = CCString::createWithFormat(str_CantAdvance_tips70, file)->getCString();
            // labelttf字符的高度为25, 12.5 = height/2
            SGCCLabelTTF* a = SGCCLabelTTF::create(text, FONT_PANGWA, 28);
            this->addChild(a);
            a->setPosition(ccpAdd(title->getPosition(), ccp(0, -100)));
            if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
                SGCCLabelTTF* b = SGCCLabelTTF::create(str_CantAdvance_tips71, FONT_PANGWA, 28);
                
                SGCCLabelTTF* b1 = SGCCLabelTTF::create(str_CantAdvance_tips72, FONT_PANGWA, 28);
                b1->setAnchorPoint(ccp(0, 0.5f));
                b1->setPosition(ccpAdd(a->getPosition(), ccp(-b->getContentSize().width*.5f, -50)));
                this->addChild(b1);
                SGCCLabelTTF* b2 = SGCCLabelTTF::create(str_CantAdvance_tips73, FONT_PANGWA, 28 , ccYELLOW);
                //modify by:zyc . merge into create.
                //b2->setColor(ccYELLOW);
                b2->setAnchorPoint(ccp(0, 0.5f));
                this->addChild(b2);
                b2->setPosition(ccpAdd(b1->getPosition(), ccp(b1->getContentSize().width, 0)));
                SGCCLabelTTF* b3 = SGCCLabelTTF::create(str_CantAdvance_tips74, FONT_PANGWA, 28);
                b3->setAnchorPoint(ccp(0, 0.5f));
                this->addChild(b3);
                b3->setPosition(ccpAdd(b2->getPosition(), ccp(b2->getContentSize().width, 0)));
            }
			
        }
    }
    else if (type == 30)//数据异常，返回主界面
    {
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips75, FONT_PANGWA, FONTSIZE(15));
        this->addChild(a);
        a->setPosition(SGLayout::getPoint(kMiddleCenter));
    }
    else if (type == 35)//失败引导
    {
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips761, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);

        SGCCLabelTTF* text = SGCCLabelTTF::create(str_CantAdvance_tips77, FONT_PANGWA, 28, CCSizeMake(470, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        text->setPosition(ccpAdd(title->getPosition(), ccp(5,  -title->getContentSize().height*2.2)));
        this->addChild(text);

        if (SGAIManager::shareManager()->isInitPve)
        {
            ok = SGButton::create("box_btn_newbg.png", "upwar_font_strgs.png", this, NULL, ccp(0, 0), false, true);
            ok->setPosition(ccpAdd(center, ccp(-wid*.34f, -hgt*.55f+60)));
            ok->setTarget(this, menu_selector(SGCantAdvanceBox::strengHandler));
            this->addBtn(ok);
            
            ok = SGButton::create("box_btn_newbg.png", "upwar_font_visit.png", this, NULL, ccp(0, 0), false, true);
            ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
            ok->setTarget(this, menu_selector(SGCantAdvanceBox::visitHandler));
            this->addBtn(ok);
            
            cancel = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, NULL, ccp(0, 0), false, true);
            cancel->setPosition(ccpAdd(center, ccp(wid*.34f, -hgt*.55f+60)));
            cancel->setTarget(this, menu_selector(SGCantAdvanceBox::cancelFromReward));
            this->addBtn(cancel);
        }
        else
        {
            ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
            ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
            ok->setTarget(this, menu_selector(SGCantAdvanceBox::cancelFromReward));
            this->addBtn(ok);
        }
    }else if (type == 36)
    { // 未完的引导
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips76, FONT_PANGWA, 40 , COLOR_TAN);
	
        title->setPosition(biao->getPosition());
        this->addChild(title);
        const char *fstr = str_CantAdvance_tips78;
        SGCCLabelTTF* a = SGCCLabelTTF::create(fstr, FONT_PANGWA, 30, CCSizeMake(boxSize.width*.86f, 100));
        a->setPosition(ccpAdd(biao->getPosition(), ccp(5, -140)));
        this->addChild(a);
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
        ok->setTarget(this, menu_selector(SGCantAdvanceBox::showConGuide));
        this->addBtn(ok);
    }
    else if (type == 37)
    { // PVP匹配超时（对手掉线）
        SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips79, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);

        title->setPosition(biao->getPosition());
        this->addChild(title);
        const char *fstr = str_CantAdvance_tips80;
        SGCCLabelTTF* a = SGCCLabelTTF::create(fstr, FONT_PANGWA, 30, CCSizeMake(boxSize.width*.86f, 100));
        a->setPosition(ccpAdd(biao->getPosition(), ccp(5, -140)));
        this->addChild(a);
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
        ok->setTarget(this, menu_selector(SGCantAdvanceBox::reConnect));
        this->addBtn(ok);
    }
    else if (type == 38)
    { // PVP匹配失败（找不到对手）
        SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips79, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);

        title->setPosition(biao->getPosition());
        this->addChild(title);
        const char *fstr = str_CantAdvance_tips81;
        SGCCLabelTTF* a = SGCCLabelTTF::create(fstr, FONT_PANGWA, 30, CCSizeMake(boxSize.width*.86f, 100));
        a->setPosition(ccpAdd(biao->getPosition(), ccp(5, -140)));
        this->addChild(a);
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
    }
    else if (type == 39)
    { // 统御力超出
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips76, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        const char *fstr = str_CantAdvance_tips82;
        SGCCLabelTTF* a = SGCCLabelTTF::create(fstr, FONT_PANGWA, 30, CCSizeMake(boxSize.width*.86f, 100));
        a->setPosition(ccpAdd(biao->getPosition(), ccp(5, -140)));
        this->addChild(a);
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
    }
    else if (type == 40)
    { // 呼朋引伴
        
        
        //这里做领取奖励的弹框显示
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips76, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
		
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
    }
    else if (type == 41)
    { // 分羹一杯
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips76, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips84,Coins,Golds)->getCString(), FONT_PANGWA, 30, CCSizeMake(boxSize.width*.86f, 100), kCCTextAlignmentCenter);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(5, -140)));
        this->addChild(a);
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, NULL, ccp(0, 0), false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
    }
    else if (type == 50) //守荆州是否跳过本次战斗
    {
        //跳过GO
        ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips85, this, menu_selector(SGCantAdvanceBox::sjzJumpGo), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.34, -hgt*.52+50)));
        this->addBtn(ok);
        
        //取消
        cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.34, -hgt*.52+50)));
        this->addBtn(cancel);
        
        //手动GO
        store = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips86, this, menu_selector(SGCantAdvanceBox::sjzRushGo), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        store->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.52+50)));
        this->addBtn(store);
        
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips87, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);

        title->setPosition(ccpAdd(biao->getPosition(), ccp(0, -ok->getContentSize().height)));
        this->addChild(title);
        
        //文本，传入的Coins就是百分比，借用数字。
        SGCCLabelTTF* text = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips88, Coins)->getCString(), FONT_PANGWA, 28, CCSizeMake(470, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        text->setPosition(ccpAdd(title->getPosition(), ccp(ok->getContentSize().width*0.1, -ok->getContentSize().height*1.3)));
        this->addChild(text);
    }
    else if (type == 51) //军魂换礼战绩提示
    {
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips89, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
		
        //文本
        SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
        SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips90, data->getBonusDamage(), data->getBonusSoul())->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(25, -100)));
        this->addChild(a);
        
        //确定
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
        
        //清除战绩状态
        data->setIsShowBonus(false);
        data->setBonusDamage(0);
        data->setBonusSoul(0);
    }
    else if (type == 52) //放弃守荆州战斗的二次确认
    {
        //确定放弃
        ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips91, this, menu_selector(SGCantAdvanceBox::sjzBattleGiveUp), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.52+50)));
        this->addBtn(ok);
        
        //继续战斗
        cancel = SGButton::createFromLocal("box_btn_newbg.png",str_CantAdvance_tips92, this, menu_selector(SGCantAdvanceBox::sjzBattleCarryOn), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.52+50)));
        this->addBtn(cancel);
		
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips93, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        //文本
        SGCCLabelTTF* text = SGCCLabelTTF::create(CCString::create(str_CantAdvance_tips94)->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        text->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
        this->addChild(text);
    }
    else if (type == 53) //取回军魂的消费确认，弹出双按钮：“确定”，“取消”
    {
        //确定
        ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::withdrawSoulCostYes), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.52+50)));
        this->addBtn(ok);
        
        //取消
        cancel = SGButton::createFromLocal("box_btn_newbg.png", str_Cancel, this, menu_selector(SGCantAdvanceBox::withdrawSoulCostNo), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.52+50)));
        this->addBtn(cancel);
        
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips95, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        //文本
        SGCCLabelTTF* text = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips96, Coins)->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        text->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
        this->addChild(text);
    }
    else if (type == 54) //取回军魂元宝不够的储值提示，“去储值”，“取消”
    {
        //去储值
        ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips97, this, menu_selector(SGCantAdvanceBox::withdrawSoulRechargeYes), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.52+50)));
        this->addBtn(ok);
        
        //取消
        cancel = SGButton::createFromLocal("box_btn_newbg.png", str_Cancel, this, menu_selector(SGCantAdvanceBox::withdrawSoulRechargeNo), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.52+50)));
        this->addBtn(cancel);
        
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips98, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        //文本
        SGCCLabelTTF* text = SGCCLabelTTF::create(CCString::create(str_CantAdvance_tips99)->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        text->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
        this->addChild(text);
    }
    else if (type == 55) //军魂领奖的奖品提示界面，弹出按钮：“确定”
    {
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips100, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        //文本
        CCString* temp = (CCString*) _data;
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(25, -80)));
        this->addChild(a);
        
        //确定
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+50)));
        //        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
    }
    else if (type == 56) //守荆州每日结算奖励的提示界面，弹出按钮：“确定”
    {
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips101, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
		//title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        //文本
        SGSjzData* sjzData = SGPlayerInfo::sharePlayerInfo()->getSjzData();
        CCString* temp = CCString::createWithFormat(str_CantAdvance_tips102, sjzData->getDailyRank(), sjzData->getDailyIngot());
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(25, -80)));
        this->addChild(a);
        
        //确定
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
        ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+30)));
        //        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
		
    }
    else if (type == 57) //守荆州每日结算奖励的提示界面，弹出按钮：“确定”
    {
        //标题
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips103, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        //文本
        
        CCString* temp = CCString::createWithFormat("%s",showMessage);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(20, -90)));
        this->addChild(a);
        
        //确定
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
        ok->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
        //        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
        this->addBtn(ok);
        SGButton * share = SGButton::createFromLocal("box_btn_newbg.png", "分享", this, menu_selector(SGCantAdvanceBox::onClickShareButton), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
        
        share->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
        if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
        {
            share->setVisible(true);

        }else
        {
            share->setVisible(false);
        }
        
        this->addBtn(share);

        
    }
	else if (type == 60) //守荆州军魂取回操作但当前注入军魂数值均为0
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips104, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips105, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		//          a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 120)));
		a->setPosition(ccpAdd(biao->getPosition(), ccp(25, -110)));
		this->addChild(a);
		
		//确定
		ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
		ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		
	}
	else if (type == 61) //守荆州开始守城时 每天免费次数用完 但玩家未达到对应VIP等级时
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_Tips, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips106, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);

		a->setPosition(ccpAdd(biao->getPosition(), ccp(25, -140)));
		this->addChild(a);
		
		//确定
		ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
		ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
		this->addBtn(ok);
		
	}
	else if (type == 62) //守荆州开始守城时 每天免费次数用完 玩家达到对应VIP等级，且VIP进入次数也用完
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips107, FONT_PANGWA, 30 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips108, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 120)));
		this->addChild(a);
		
		//确定
		ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, false, true);
		ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f-20)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		
	}
	else if (type == 63) ////守荆州开始守城时 每天免费次数用完 玩家达到对应VIP等级，且VIP进入次数还未用完
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips109, FONT_PANGWA, 30 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips110, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f +60)));
		this->addChild(a);
		
		
		CCString* temp = CCString::createWithFormat(str_CantAdvance_tips111,need);
		SGCCLabelTTF* b = SGCCLabelTTF::create(temp->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter , ccRED);
		b->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f +0)));
		this->addChild(b);
		
		//确定
		ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::chargeconfirm), CCPointZero, false, true);
		ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+50)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		
	}
	else if (type == 64) ////系统投诉建议 点删除弹出框
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips112, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips113, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::confirmdelete), CCPointZero, false, true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
        // ok->setTag(neenGold);
		
		//取消
		cancel = SGButton::createFromLocal("box_btn_newbg.png", str_Cancel, this, menu_selector(SGCantAdvanceBox::boxClose), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);
		
	}
    
	else if (type == 65) ////
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips76, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips114, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips115, this, menu_selector(SGCantAdvanceBox::strengHandler), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));

		this->addBtn(ok);
		
		//取消
		cancel = SGButton::createFromLocal("box_btn_newbg.png",str_CantAdvance_tips116, this, menu_selector(SGCantAdvanceBox::cancelFromReward), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);
		
	}else if (type == 66) ////pvp快速入口提示框
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips121, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips122,SGPlayerInfo::sharePlayerInfo()->getPvPRewardRate())->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::SGCantAdvanceBox::confirmJumptoPvp), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		// ok->setTag(neenGold);
		
		//取消
		cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::withdrawSoulRechargeNo), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);
		
	}
    else if (type == 67) ////boss快速入口提示框
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips121, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips123, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::confirmJumptoBoss), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		this->addBtn(ok);
		
		//取消
		cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::withdrawSoulRechargeNo), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);
		
	}
    
    //
    else if(201 == type)
    {
        //标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_MallLayer_str11, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* line_1 = SGCCLabelTTF::create(str_MallLayer_str12, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		line_1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(20, hgt*.25f - 32)));
		this->addChild(line_1);
		
		CCSprite * gold = SGMallUtil::createSpriteByPayType(Coins);
        gold->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-40, hgt*.25f - 85 )));
        this->addChild(gold);
        SGCCLabelTTF * goldNum = SGCCLabelTTF::create(CCString::createWithFormat("x%d",power)->getCString(), FONT_XINGKAI, 28);
        goldNum->setPosition(gold->getPosition() + ccp(60, 0));
        this->addChild(goldNum);
        
        SGCCLabelTTF * line_2 = SGCCLabelTTF::create(str_MallLayer_str13,FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        line_2->setPosition(SGLayout::getPoint(kMiddleCenter) + ccp(20 , hgt*.25f - 135));
        this->addChild(line_2);
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::shopYes), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		this->addBtn(ok);
		
		//取消
		cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::shopCancel), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);
    }
    else if (type == 68) ////更新完成提示重启
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_SGCantAdvanceBox_1001, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_SGCantAdvanceBox_1002, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::confirmReboot), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
		this->addBtn(ok);
		
		
		
	}
    else if (type == 69) ////绑定二次确认提示框
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_RewardBindLayer_str112, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		//title->setOutSideColor(ccc3(143,87,0));
		title->setPosition(biao->getPosition());
		this->addChild(title);
		mailaddress = showMessage;
        std::string straccount = CCUserDefault::sharedUserDefault()->getStringForKey("username");
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_RewardBindLayer_str113,straccount.c_str(),mailaddress)->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::confirmBindEmai), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		// ok->setTag(neenGold);
        
        cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::cancelHandler), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);
		
		
		
	}
    else if (type == 70) ////绑定邮箱成功提示
	{
		//标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_RewardBindLayer_str114, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(str_RewardBindLayer_str115, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::cancelHandler), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		// ok->setTag(neenGold);
	}
    else if (type == 71)
    {
       
        ok = SGButton::createFromLocal("box_btn_newbg.png",
                                       str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::showAddGold),CCPointZero,FONT_PANGWA,ccWHITE,32);
        this->addBtn(ok);
		
        cancel = SGButton::create("box_btn_newbgHui.png",
								  "public_font_quxiao.png",
								  this,
								  menu_selector(SGCantAdvanceBox::boxClose),
								  ccp(0, 0),
								  false,
								  true);
        this->addBtn(cancel);
        ok->setPosition(ccpAdd(center, ccp(-145, -265/2+10)));
        cancel->setPosition(ccpAdd(center, ccp(145, -265/2+10)));
        
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips128, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
		
		
        SGCCLabelTTF* a = SGCCLabelTTF::create(SGPlayerInfo::sharePlayerInfo()->getShowvip() ? str_CantAdvance_tips129 : str_CantAdvance_tips1297, FONT_PANGWA, 32,CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -120)));
        
     
    }
    //遊客綁定頁面
    else if(type==219)
    {
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_RegisterLayer_login_old, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
		title->setPosition(biao->getPosition());
		this->addChild(title);
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(showMessage, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
        
        ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGCantAdvanceBox::useOldAccountOk), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		this->addBtn(ok);
        
        cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::useOldAccountCanel), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);

    }
    else if(1919 == type)
    {
        //  type=1 vip 等级不足,type =2 元宝不足 ,type=3 购买成功
        if(need==1)
        {
            SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips130 , FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
            title->setPosition(biao->getPosition());
            this->addChild(title);
            
            SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips133, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
            this->addChild(a);
            
            cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Ok, this, menu_selector(SGCantAdvanceBox::boxCloseAtActivityLayer), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
            cancel->setPosition(ccpAdd(center, ccp( 0, -hgt*.55f+60)));
            this->addBtn(cancel);
        }
        else if(2==need)
        {
            SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips131 , FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
            title->setPosition(biao->getPosition());
            this->addChild(title);
            
            SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips134, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
            this->addChild(a);
            
            ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGCantAdvanceBox::gotoChargeLayer), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
            ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
            this->addBtn(ok);
            
            cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel_, this, menu_selector(SGCantAdvanceBox::boxCloseAtActivityLayer), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
            cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
            this->addBtn(cancel);
        }
        else if(3==need)
        {
            SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips132 , FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
            title->setPosition(biao->getPosition());
            this->addChild(title);
            
            SGCCLabelTTF* a = SGCCLabelTTF::create(str_CantAdvance_tips135, FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
            this->addChild(a);
            
            cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Ok, this, menu_selector(SGCantAdvanceBox::boxCloseAtActivityLayer), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
            cancel->setPosition(ccpAdd(center, ccp(0, -hgt*.55f+60)));
            this->addBtn(cancel);
        }
    }
    else if (2014 == type) //2014 月签奖励，提示去往充值。
    {
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_monthly_sign_str1 , FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        
        char buffer[500];
        sprintf(buffer, str_monthly_sign_str4, SGPlayerInfo::sharePlayerInfo()->getShowvip() ? showMessage : str_monthly_sign_str5);
        
        CCSprite* text = SGRichLabel::getFomLabel(buffer, CCSizeMake(450, 180), FONT_PANGWA, 28);
        text->setAnchorPoint(ccp(0.5,1));
        text->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 30)));
        this->addChild(text);
        
        ok = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCantAdvanceBox::gotoChargeLayer));
        ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
        this->addBtn(ok);
        
        cancel = SGButton::create("box_btn_newbgHui.png", "public_font_quxiao.png", this, menu_selector(SGCantAdvanceBox::boxCloseAtActivityLayer));
        cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
        this->addBtn(cancel);
    }

    else if( 2015 == type)
    {
        //标题
		SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips137, FONT_PANGWA, 40 , ccWHITE , COLOR_TAN);
        //modify by:zyc . merge into create.
		//title->setOutSideColor(ccc3(143,87,0));
		title->setPosition(biao->getPosition());
		this->addChild(title);
		
		
		SGCCLabelTTF* a = SGCCLabelTTF::create(CCString::createWithFormat(str_CantAdvance_tips138,Golds)->getCString(), FONT_PANGWA, 28, CCSizeMake(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25f - 80)));
		this->addChild(a);
		
		
		
		//确定
		ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips139, this, menu_selector(SGCantAdvanceBox::buyArenaFightCount), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		ok->setPosition(ccpAdd(center, ccp(-wid*.2, -hgt*.55f+60)));
		//        ok->setTarget(this, menu_selector(SGCantAdvanceBox::boxClose));
		this->addBtn(ok);
		// ok->setTag(neenGold);
		
		//取消
		cancel = SGButton::createFromLocal("box_btn_newbg.png",str_Cancel, this, menu_selector(SGCantAdvanceBox::cancelHandler), CCPointZero, FONT_PANGWA,ccWHITE,32,false,true);
		cancel->setPosition(ccpAdd(center, ccp(wid*.2, -hgt*.55f+60)));
		this->addBtn(cancel);

    }
    
    return true;
}

//购买竞技场挑战次数
void SGCantAdvanceBox::buyArenaFightCount()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::ArenaBuyFightCountRequest * req = new main::ArenaBuyFightCountRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_BUY_FIGHT_COUNT, req);
    this->boxClose();
    
}
void SGCantAdvanceBox::reConnect()
{
    EFFECT_PLAY(MUSIC_BTN);
    // 重新登陆原来的IP
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    
    std::string serverip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
    short serverpost = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
    SGSocketClient::sharedSocketClient()->startConn(serverip.c_str(), serverpost);
}

void SGCantAdvanceBox::showConGuide()
{
    EFFECT_PLAY(MUSIC_BTN);
    // 显示未完成的教学
    this->boxClose();
    
    SGMainManager::shareMain()->showFirstLayer();
}

void SGCantAdvanceBox::gotoBindAccountLayer()
{
#if (PLATFORM == IOS)
    this->setVisible(false);
    this->boxClose();
    
     SGMainManager::shareMain()->showregisterLayer(2, true);
#else
    SGMainManager::shareMain()->showregisterLayer(2, true);
    
    //android 需要这样处理, 如果有问题，加入ANDROID宏来控制
    
    this->setVisible(false);
    this->boxClose();
#endif
//    SGMainManager::shareMain()->showregisterLayer(2, true);
//
////android 需要这样处理, 如果有问题，加入ANDROID宏来控制
//
//    this->setVisible(false);
//    this->boxClose();
//    
}



void SGCantAdvanceBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_dele)
    {
		//         _dele->setIsCanTouch(true);
		_dele->showBoxCall(data);
        _dele = NULL;
    }
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGBaseLayer *layer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (layer != NULL) {
        layer->setIsCanTouch(true);
    }
    
    if (enterType != 100)
    {
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        if (mainLayer != NULL) {
            mainLayer->setIsCanTouch(true);
            
        }
    }

   // SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    int tag = mainScene->nowBoxTag;
    
    if(mainScene->oldBoxTag>0)
    {
        
        SGBaseBox *mainbox = (SGBaseBox *)mainScene->getChildByTag(mainScene->oldBoxTag);
        if(mainbox&&this!=mainbox)
            mainbox->boxClose();
    }

    this->removeFromParentAndCleanup(true);
}

void SGCantAdvanceBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}

void SGCantAdvanceBox::liandanHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
    this->boxClose();
}

void SGCantAdvanceBox::storeEnterListener(cocos2d::CCObject *obj)
{
	
    if (enterType != 9) {
		
		SGSocketRequest *sr = (SGSocketRequest *)obj;
		main::ShopChargeEntryResponse *response = (main::ShopChargeEntryResponse *)sr->m_msg;
		if (sr)
		{
            //MM: 当时哪个脑残片写的“hehe"注释？这里有BUG知道吗？
            this->boxClose();//hehe 在这个地方加boxClose，不然会删除监听
            
			main::VIPInfo vip = response->vipinfo();
			int leng = response->chargeitems_size();
			CCArray *arrary = CCArray::create();
			CCArray *strary = CCArray::create();
			for (int i = 0;i<leng;i++) {
				main::ChargeItem item = response->chargeitems(i);
				CCString *str = CCString::createWithFormat("%d-%d-%d-%d-%d-%d-%d-%d",item.showtype(),item.chargetype(),item.discount(),item.gold(),item.rmb(),item.goldperday(),item.leftday(),item.rewardday());
				arrary->addObject(str);
				CCString *str2 = CCString::createWithFormat("%s",item.code().c_str());
				strary->addObject(str2);
				
			}
			
			
			
			//        SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,0);
			
			if (enterType == 11 || enterType == 10)
            {
				SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,3,strary);
				
				//            SGMainManager::shareMain()->showStoreLayer(response->currentviplevel(),response->nextviplevel(),response->updateneedrmb(),3);
			}
			else if (enterType == 13)
            {
				SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,4,strary);
				//            SGMainManager::shareMain()->showStoreLayer(response->currentviplevel(),response->nextviplevel(),response->updateneedrmb(),4);
			}
			else if (enterType == 19)
            {
				SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,5,strary,false);
				//            SGMainManager::shareMain()->showStoreLayer(response->currentviplevel(),response->nextviplevel(),response->updateneedrmb(),5,false);
				
			}
			else if (enterType == 16 || enterType == 28 || enterType == 29)
            {
				SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,6,strary,false);
				//            SGMainManager::shareMain()->showStoreLayer(response->currentviplevel(),response->nextviplevel(),response->updateneedrmb(),6,false);
			}
            else //此时701消息有多个obs，没有明确指定的话，返回到商城页面。
            {
                SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,0,strary,true);
            }
        }
		
    }
	
}

void SGCantAdvanceBox::strengsecondconfirm()
{
	EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::createWithFormat("%d",10), "type");
    data = dict;
    SGBaseBox::boxClose();
	
}
void SGCantAdvanceBox::strengConfirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::createWithFormat("%d",5), "type");
    data = dict;
    SGBaseBox::boxClose();
}
void SGCantAdvanceBox::showAddGold()
{
    EFFECT_PLAY(MUSIC_BTN);
    //if (enterType ==9) {
      //  this->boxClose();
    //}
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    int tag = mainScene->nowBoxTag;
    if(mainScene->oldBoxTag>0)
    {
        
        SGBaseBox *mainbox = (SGBaseBox *)mainScene->getChildByTag(mainScene->oldBoxTag);
        if(mainbox&&this!=mainbox)
            mainbox->boxClose();
    }

    if (SGMainManager::shareMain()->getMainLayer()) {
        
        SGMainManager::shareMain()->getMainLayer()->requestStoreLayer(this);
    }
	//    this->boxClose();//不能在这个地方加boxClose ,会把监听删除
    
}
void SGCantAdvanceBox::managerBarrack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGFirstStrengBox *box = SGFirstStrengBox::create(_dele);
    this->boxCloseWithOutCallBack();
    SGMainManager::shareMain()->showBox(box);
}
void SGCantAdvanceBox::expandBarrack()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxCloseWithOutCallBack();
    SGMainManager::shareMain()->showExpandLayer();
}
void SGCantAdvanceBox::noCoinsHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
	//    SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mailLayer);
	//    mainlayer->storyHandler();
	//    this->boxClose();
    main::ShopExchangeCoinEntryRequest *request = new main::ShopExchangeCoinEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_ENTER, request);
}
void SGCantAdvanceBox::upAccountHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showregisterLayer(2);
    
    
	//    SGRegisterBox *registerBox = SGRegisterBox::create(_dele,2);
	//    this->boxClose();
	//    SGMainManager::shareMain()->showBox(registerBox);
}
void SGCantAdvanceBox::cleanAccountHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
    if (enterType == 22) {
        
		// 移动至外围ocCaller中,集中使用cpp调用
#if (PLATFORM == IOS)
		OcCaller::getInstance()->pyUserSDKCallReq();
#else
		//android code here
#endif
        
        this->boxClose();
		/*
        id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
        [[PYUserSDKLib sharedUserSDKLib] destroyGuest:@"1"
                                            ChannelId:[sdkReqCallBack ChannelId]
									 CallBackDelegate:sdkReqCallBack FinishSelector:@selector(registFinished:) FailSelector:@selector(requestFailed:)];
        
		 */
		
        SGMainManager::shareMain()->showregisterLayer(1);
		//        SGRegisterBox *registerBox = SGRegisterBox::create(_dele,ACCOUNT_REGISTER);
		//        this->boxCloseWithOutCallBack();
		//        SGMainManager::shareMain()->showBox(registerBox);
    }else
    {
        CCLOG("SGCantAdvanceBox:enterType == 22 enterType != 22" );
		//    SGServerListLoginGame *login = SGServerListLoginGame::create(_dele);
		//    this->boxClose();
		//    SGMainManager::shareMain()->showBox(login);
    }
    
}
void SGCantAdvanceBox::okHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("CanAdvance"), "key");
    data = dict;
    this->boxClose();
    SGMainManager::shareMain()->showwelcomelayer();
    
}
void SGCantAdvanceBox::cancelHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}
void SGCantAdvanceBox::enterVisitHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    //SGMainManager::shareMain()->entryGoldMall(2);
	//    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mailLayer);
	//    layer->storyHandler();
}

#pragma mark - --TODO--
void SGCantAdvanceBox::getFullPvEPower()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::ShopPurchPvEPowerRequest *request = new main::ShopPurchPvEPowerRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_PVE_FULLPOWER, request);
    return;
    
    int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
    if ((gold-neenGold) < 0) {
        warn->setVisible(true);
    }else {
        // getfull power
        main::ShopPurchPvEPowerRequest *request = new main::ShopPurchPvEPowerRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_PVE_FULLPOWER, request);
    }
}

void SGCantAdvanceBox::fullPvEPowerListener(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::ShopPurchPvEPowerResponse *response = (main::ShopPurchPvEPowerResponse *)req->m_msg;
    this->boxClose();
    
    if (response && response->succ() == 1) {
        SGMainManager::shareMain()->showMessage(CCString::create(str_CantAdvance_tips117)->getCString());
    } else {
        SGMainManager::shareMain()->showMessage(CCString::create(str_CantAdvance_tips118)->getCString());
    }
}

void SGCantAdvanceBox::getFullPvPPower()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::ShopPurchPvPPowerRequest *request = new main::ShopPurchPvPPowerRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_PVP_FULLPOWER, request);
    return;
    
    int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
    if ((gold-neenGold) < 0) {
        warn->setVisible(true);
    }else {
        // getfull pvp power
        main::ShopPurchPvPPowerRequest *request = new main::ShopPurchPvPPowerRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_PVP_FULLPOWER, request);
    }
}

void SGCantAdvanceBox::fullPvPPowerListener(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::ShopPurchPvPPowerResponse *response = (main::ShopPurchPvPPowerResponse *)req->m_msg;
    
    if (response && response->succ() == 1)
    {
        //SGPlayerInfo::sharePlayerInfo()->setPvpPower(30); // 回满军粮//5改30
        
        //int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold(); // 减元宝
        //SGPlayerInfo::sharePlayerInfo()->setPlayerGold(gold-neenGold);
        
        //CCString *str = CCString::createWithFormat("%d", (gold-neenGold));
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        //角色信息页  购买成功时 更新ui
        SGPlayerInfoLayer *layer = (SGPlayerInfoLayer *)mainScene->getChildByTag(sg_playerInfoLayer);
        //pvp购买体力时  更新ui
        SGBattleMainLayer * battleMainLayer = (SGBattleMainLayer * )mainScene->getChildByTag(sg_battleMainLayer);
        if (layer != NULL)
        {
            layer->havegold->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerGold() )->getCString());
            layer->label_junliang->setString(CCString::createWithFormat("%d" , SGPlayerInfo::sharePlayerInfo()->getPvpPower() )->getCString());
        }
        if (battleMainLayer)
        {
            char str[20];
            sprintf(str,"%d/%d",SGPlayerInfo::sharePlayerInfo()->getPvpPower() , 10 );
            battleMainLayer->updateJunLiang(str);
        }
        
        SGMainManager::shareMain()->showMessage(CCString::create(str_CantAdvance_tips119)->getCString());
    }
    else
    {
        SGMainManager::shareMain()->showMessage(CCString::create(str_CantAdvance_tips120)->getCString());
    }
    this->boxClose();
}

void SGCantAdvanceBox::exchangeEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopExchangeCoinEntryResponse *response = (main::ShopExchangeCoinEntryResponse *)sr->m_msg;
    
    if (sr)
    {
        int vipLvl = response->viplevel();
        int useChance = response->usedchance();
        int max = response->maxchance();
        int gold = response->gold();
        int coin = response->coin();
        this->boxClose();
        SGMainManager::shareMain()->showExchangLayer(vipLvl,max,useChance,gold,coin);
    }
}

void SGCantAdvanceBox::strengHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxCloseWithOutCallBack();
    
	//    if (SGPlayerInfo::sharePlayerInfo()->getBattleType() != 1 &&
	//        SGPlayerInfo::sharePlayerInfo()->getBattleType() != 2) {
	SGMainManager::shareMain()->showFirstLayer();
	
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGFirstLayer *firstLayer = (SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer);
	
	SGFirstStrengBox *strengBox = SGFirstStrengBox::create(firstLayer);
	SGMainManager::shareMain()->showBox(strengBox);
	//    }else{
	//        SGFightRewardLayer::gobackToStoryLayer();
	//    }
}

void SGCantAdvanceBox::visitHandler()
{
    
    EFFECT_PLAY(MUSIC_BTN);
    this->boxCloseWithOutCallBack();
    
	//    if (SGPlayerInfo::sharePlayerInfo()->getBattleType() != 1 &&
	//        SGPlayerInfo::sharePlayerInfo()->getBattleType() != 2) {
	main::LotteryEntryRequest *request = new main::LotteryEntryRequest();
	SGSocketClient::sharedSocketClient()->send(MSG_VISIT_ENTER, request);
	//    }else
	//    {
	//        SGFightRewardLayer::gobackToStoryLayer();
	//    }
}

void SGCantAdvanceBox::cancelFromReward()
{
    EFFECT_PLAY(MUSIC_BTN);
	if( enterType==35)
	{
		SGPlayerInfo::sharePlayerInfo()->setisshowed(true);
	}
    SGFightRewardLayer::gobackToStoryLayer();
}
void SGCantAdvanceBox::onlyclosebox()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseBox::boxCloseWithOutCallBack();
}

//守荆州跳过关卡
void SGCantAdvanceBox::sjzJumpGo()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBattleManager::sharedBattle()->sjzStartRequest(true);
    this->boxClose();
}

//守荆州手动闯关
void SGCantAdvanceBox::sjzRushGo()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBattleManager::sharedBattle()->sjzStartRequest(false);
    this->boxClose();
}

//守荆州战斗二次确认-放弃
void SGCantAdvanceBox::sjzBattleGiveUp()
{
    EFFECT_PLAY(MUSIC_BTN);
	//    //发送新的放弃守荆州消息
	//    main::SurvivalStopRequest* req = new main::SurvivalStopRequest();
	//    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_GIVE_UP, req);
    //以下为老逻辑
    CCDictionary *obj = CCDictionary::create();
    obj->setObject(CCString::createWithFormat("%d",2), "isback");
    data = obj;
    SGBaseBox::boxClose(); //不知道这个类为什么要重写boxClose()而不用父类的。
}

//守荆州战斗二次确认-继续
void SGCantAdvanceBox::sjzBattleCarryOn()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *obj = CCDictionary::create();
    obj->setObject(CCString::createWithFormat("%d",1), "isback");
    data = obj;
    SGBaseBox::boxClose();  //不知道这个类为什么要重写boxClose()而不用父类的。
}

//取回军魂消费确认-yes
void SGCantAdvanceBox::withdrawSoulCostYes()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGSoulExchangeLayer* sgsel = (SGSoulExchangeLayer*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_soulExchangeLayer);
    if (sgsel)
        sgsel->sendWithdrawSoulRequest();
    this->boxClose();
}

//取回军魂消费确认-no
void SGCantAdvanceBox::withdrawSoulCostNo()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}

//取回军魂储值确认-yes
void SGCantAdvanceBox::withdrawSoulRechargeYes()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showShopLayer();
    this->boxClose();
}

//取回军魂储值确认-no
void SGCantAdvanceBox::withdrawSoulRechargeNo()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}

//守荆州消耗VIP次数花费元宝确认
void SGCantAdvanceBox::chargeconfirm()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGShouJingZhou* layer = (SGShouJingZhou*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_shoujingzhou);
    if (layer)
        layer->sendVipCostChanceRequest(need);
    this->boxClose();
    
    
}


void SGCantAdvanceBox::confirmdelete()

{
    EFFECT_PLAY(MUSIC_BTN);
    
    int m_id=neenGold;
	
    main::DelQARequest *del = new main::DelQARequest();
    del->set_id(m_id);////////////////////////////
    SGSocketClient::sharedSocketClient()->send(MSG_DELQA, del);
    
    // CC_SYNTHESIZE(int , m_idid,checkid); //存储所查看问题的id
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    player->setcheckid(m_id);
    
    
	//    SGComplainLayer* layer = (SGComplainLayer*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_complain);
	//    layer->setidid(m_id);
    
    this->boxClose();
    
    
}
void SGCantAdvanceBox::onClickShareButton()
{
    SGBossBattleLayer* layer = (SGBossBattleLayer*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossBattleLayer);

    SGShareBox *box = SGShareBox::create(layer,SGShareTypeBossBattel);
    SGMainManager::shareMain()->showBox(box);
}
//直接进入pvp战斗页面
void SGCantAdvanceBox::confirmJumptoPvp()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showPvpMainLayer();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    int tag = mainScene->nowBoxTag;
    if(mainScene->oldBoxTag>0)
    {
   
    SGBaseBox *mainbox = (SGBaseBox *)mainScene->getChildByTag(mainScene->oldBoxTag);
    if(mainbox&&this!=mainbox)
        mainbox->boxClose();
    }
    this->boxClose();
}
//直接进入boss战斗页面
void SGCantAdvanceBox::confirmJumptoBoss()
{
    main::MainBossRequest *request=new main::MainBossRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    int tag = mainScene->nowBoxTag;
    
    if(mainScene->oldBoxTag>0)
    {
        
        SGBaseBox *mainbox = (SGBaseBox *)mainScene->getChildByTag(mainScene->oldBoxTag);
        if(mainbox&&this!=mainbox)
            mainbox->boxClose();
    }
    this->boxClose();
}
void SGCantAdvanceBox::confirmBindEmai()
{
    main::BindMailRequest *request = new main::BindMailRequest();
    request->set_state(1);
    request->set_mail(mailaddress);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_BIND_MAILBOX_INFO, request);
    this->boxClose();
}
void SGCantAdvanceBox::confirmReboot()
{
    //目前用在大包组装更新完成后的重启。
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    if(mainScene->oldBoxTag>0)
    {
        SGBaseBox *mainbox = (SGBaseBox *)mainScene->getChildByTag(mainScene->oldBoxTag);
        if(mainbox&&this!=mainbox)
            mainbox->boxClose();
    }
    this->boxClose();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#else
    CCDirector::sharedDirector()->end();
#endif
}

//商店添加  刷新二次确认
void SGCantAdvanceBox::shopYes()
{
   if ( ((SGMallLayer * )_data )->sendRefreshMsg() )
       this->shopCancel();
}

void SGCantAdvanceBox::shopCancel()
{
    this->boxClose();
    //禁用头像 和首充 的触摸。 析构中撤销。
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);

}

void SGCantAdvanceBox::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT_MAX, true);//11.17
}

bool SGCantAdvanceBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}


void SGCantAdvanceBox::useOldAccountCanel()
{
    this->boxClose();
}

void SGCantAdvanceBox::useOldAccountOk()
{
    SGMainManager::shareMain()->showloginLayer();
    this->boxClose();
}

void SGCantAdvanceBox::gotoChargeLayer()
{
    main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
    
    this->boxClose();
}

void SGCantAdvanceBox::boxCloseAtActivityLayer()
{
    this->boxClose();
    
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
}

