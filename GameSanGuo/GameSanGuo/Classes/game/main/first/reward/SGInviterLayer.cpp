//
//  SGInviterLayer.cpp
//  GameSanGuo
//
//  Created by 曾慧 on 13-10-10.
//
//

#include "SGInviterLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "cocos-ext.h"
#include "Invite.pb.h"
#include "SGMenu.h"
#include "SimpleAudioEngine.h"
#include "SGCantAdvanceBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGRichLabel.h"

#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGShowRewardBox.h"

USING_NS_CC_EXT;

#define REWARDFONTSIZE 14

SGInviterLayer::SGInviterLayer()
{
}
SGInviterLayer::~SGInviterLayer()
{
    _array->release();
    inviteCode->release();
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REWARD_INTIVERGOLD);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_inviterLayer);
  
    
}
SGInviterLayer *SGInviterLayer::create( CCString* inviteCode,int inviteNum,int inviteMax,CCArray *array)
{
    SGInviterLayer *rewardLayer = new SGInviterLayer();
    if (rewardLayer && rewardLayer->init(NULL, sg_levelUpGetGold))
    {
        rewardLayer->_array = array;
        rewardLayer->_array->retain();
        rewardLayer->inviteCode = CCString::createWithFormat("%s",inviteCode->getCString());
        rewardLayer->inviteCode->retain();
        rewardLayer->inviteNum = inviteNum;
        rewardLayer->inviteMax = inviteMax;
        rewardLayer->initView();
        rewardLayer->autorelease();
        return rewardLayer;
    }
    CC_SAFE_DELETE(rewardLayer);
    return NULL;
}
void SGInviterLayer::initView()
{

    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_inviterLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_inviterLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->isPostData = true;

    notification->addObserver(MSG_REWARD_INTIVERGOLD,this,callfuncO_selector(SGInviterLayer::inviteGoldListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
		
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGInviterLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    
	SGCCLabelTTF *invite_friend = SGCCLabelTTF::create(str_InviterLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//invite_friend->setColor(ccc3(0xff, 0x95, 0x0c));
	//SGCCLabelTTF *title = SGCCLabelTTF::create("对战(PVP)", FONT_XINGKAI, 36);

    //CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_getgold.png");
	
    this->addChild(invite_friend);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    invite_friend->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65/2)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, invite_friend->getPosition().y));
    
   /////////////////////////
    CCScale9Sprite *kuang1 = CCScale9Sprite::createWithSpriteFrameName("barrack_item.png");
    //float mm=0;
    kuang1->setPreferredSize(CCSizeMake(s.width*0.9, skewingY(108)));
    kuang1->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,-kuang1->getContentSize().height*0.5-title_bg->getContentSize().height*1.0-15)));
    //kuang1->setScaleY(2.8);
    this->addChild(kuang1,-5);
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    this->addChild(frame1,-5);
    frame1->setPreferredSize(CCSizeMake(kuang1->getContentSize().width, kuang1->getContentSize().height));
    frame1->setPosition(kuang1->getPosition());
    
    
    float b = SGMainManager::shareMain()->getBottomHeight();
	float h = SGMainManager::shareMain()->getHeadHeight();
    
    
    const char * strjiejian1 = str_InviterLayer_str201;
    const char * strjiejian21 =str_InviterLayer_str202;
    const char * strjiejian2 = str_InviterLayer_str203;
    const char * strjiejian3 = str_InviterLayer_str204;
    //const char * strjiejian4 = str_InviterLayer_str2;
    const char * strjiejian5  = str_InviterLayer_str205;
    const char * strjiejian6  = str_InviterLayer_str206;
    const char * strjiejian7  = str_InviterLayer_str207;

	
	int hgt = 15;
#if (PLATFORM == ANDROID)
    hgt = 35;
#endif
	
    float fontSizeN = FONTSIZE(11);
    SGCCLabelTTF*label=SGCCLabelTTF::create(strjiejian1, FONT_PANGWA, FONTSIZE(12), CCSizeMake(s.width*0.85, hgt),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
    SGCCLabelTTF*label22=SGCCLabelTTF::create(strjiejian21, FONT_PENGYOU, fontSizeN,CCSizeMake(s.width*0.85, hgt),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    SGCCLabelTTF*label2=SGCCLabelTTF::create(strjiejian2, FONT_PENGYOU, fontSizeN, CCSizeMake(s.width*0.85, hgt),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop , ccRED);

    SGCCLabelTTF*label3=SGCCLabelTTF::create(strjiejian3, FONT_PENGYOU, fontSizeN, CCSizeMake(s.width*0.85, 300),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    SGCCLabelTTF*label4=SGCCLabelTTF::create(strjiejian5, FONT_PENGYOU, fontSizeN, CCSizeMake(s.width*0.85, 300),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    SGCCLabelTTF*label5=SGCCLabelTTF::create(strjiejian6, FONT_PENGYOU, fontSizeN, CCSizeMake(s.width*0.85, 300),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    SGCCLabelTTF*label6=SGCCLabelTTF::create(strjiejian7, FONT_PENGYOU, fontSizeN, CCSizeMake(s.width*0.85, 300),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);


    
    
    label2->setAnchorPoint(ccp(0.5, 1));
    label22->setAnchorPoint(ccp(0.5, 1));
    //CCSprite * strlabel  = SGRichLabel::getFomLabel(strjiejian21,CCSizeMake(s.width*0.85, 300), FONT_BOXINFO, FONTSIZE(13));
    label3->setAnchorPoint(ccp(0.5, 1));
    label4->setAnchorPoint(ccp(0.5, 1));
    label5->setAnchorPoint(ccp(0.5, 1));
    label6->setAnchorPoint(ccp(0.5, 1));
    
    //modify by:zyc. merge into create.
    //label2->setColor(ccRED);
    
    // label->setPosition((ccp(SGLayout::getPoint(kMiddleCenter).x, label_time2->getPosition().y + skewingY(40)-20)));
    label->setPosition(ccpAdd(kuang1->getPosition(), ccp(0, kuang1->getContentSize().height*0.5-10)));
    //strlabel->setPosition(ccpAdd(kuang1->getPosition(),ccp(0, -kuang1->getContentSize().height*0.25-10)));
    label22->setPosition(ccpAdd(label->getPosition(),  ccp(0, -30) ));
    label2->setPosition(ccpAdd(label22->getPosition(),  ccp(label22->getContentSize().width*0.7,0) ));
    label3->setPosition(ccpAdd(label22->getPosition(),  ccp(0,-30) ));
    label4->setPosition(ccpAdd(label3->getPosition(),  ccp(0,-30) ));
    label5->setPosition(ccpAdd(label4->getPosition(),  ccp(0,-30) ));
    label6->setPosition(ccpAdd(label5->getPosition(),  ccp(0,-30) ));
    label6->setVisible(false);

    label->setAnchorPoint(ccp(0.5, 1));
    this->addChild(label);
    //this->addChild(strlabel);
    this->addChild(label22);
    this->addChild(label2);
    this->addChild(label3);
    this->addChild(label4);
    this->addChild(label5);
    this->addChild(label6);
   

    SGCCLabelTTF *label_time1 = SGCCLabelTTF::create(str_InviterLayer_str3, FONT_PANGWA, FONTSIZE(12));
    
    if(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel()<30)
    {
        inviteCode->setString("*******     ");
    
    }

    
    SGCCLabelTTF *label_time2 = SGCCLabelTTF::create(CCString::createWithFormat(str_InviterLayer_str4,inviteNum,inviteMax)->getCString(), FONT_PANGWA, FONTSIZE(12));
    SGCCLabelTTF *label_time3 = SGCCLabelTTF::create(CCString::createWithFormat("%s",inviteCode->getCString())->getCString(), FONT_PANGWA, FONTSIZE(12) , ccRED);
    label_time1->setAnchorPoint(ccp(0.5, 1));
    label_time2->setAnchorPoint(ccp(0.5, 1));
    label_time3->setAnchorPoint(ccp(0.5, 1));
    //modify by:zyc. merge into create.
    //label_time3->setColor(ccRED);
    this->addChild(label_time1);
    this->addChild(label_time2);
    this->addChild(label_time3);
    
    float adjustY = 0;
    if (CCDirector::sharedDirector()->getWinSize().height >= 1130) {
        adjustY = -10;
    }
    
    label_time1->setPosition(ccpAdd(kuang1->getPosition(),  ccp(-kuang1->getContentSize().width/2 + label_time1->getContentSize().width/2 + 40, -kuang1->getContentSize().height/4 + adjustY+10) ));
    label_time3->setPosition(ccpAdd(label_time1->getPosition(), ccp(label_time1->getContentSize().width*1.0-5, 0)));
    label_time2->setPosition(ccp(label_time3->getPosition().x + label_time3->getContentSize().width+25,label_time3->getPosition().y));
    
    //设置tableview
	tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - title_bg->getContentSize().height - 430));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-15);
    
}


unsigned int SGInviterLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return _array->count();
}

SNSTableViewCellItem *SGInviterLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int needNum =  0;//邀请玩家数量
    int itemId = 0;//邀请玩家数量对应的奖励金子
    int itemType = 0;//奖励条目的状态	1--未满足条件	2--满足条件未领取	3--	已领取
    int itemNum = 0;
    int rewardsState = 0;
    
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    sscanf(((CCString*)_array->objectAtIndex(num))->getCString(), "%d - %d - %d - %d - %d", &needNum, &itemType,&itemId, &itemNum, &rewardsState);
	Invite_Item *item = (Invite_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    
	if (NULL == item)
	{
        item = Invite_Item::create(this,needNum,0,rewardsState, itemId, itemNum, itemType);
	}
    else
    {
        CCLOG("Need Update Item!");
        item->updateitem(rewardsState, needNum, itemId, itemNum, itemType);
    }
	item->setTouchEnabled(false);//设置item不可触摸
    return item;

}
void SGInviterLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
}




void SGInviterLayer::getGoldHandler(Invite_Item *item)
{
    EFFECT_PLAY(MUSIC_34);
    lvlupItem = item;
    main::InviteRewardRequest *request = new main::InviteRewardRequest();
    request->set_num(item->lvl);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVERGOLD, request);
}
void SGInviterLayer::inviteGoldListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;    
    if (sr)
    {
        main::InviteRewardResponse *response = (main::InviteRewardResponse *)sr->m_msg;
        if (response->state() > 0)
        {
            //int rewardGold = response->itemnum();
            //SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 40,rewardGold);
            //SGMainManager::shareMain()->showBox(cantadvanceBox);
            
            CCArray *rewards = CCArray::create();
            rewards->retain();
            
            SGMailSystemAccessoryDataModel *temp = new SGMailSystemAccessoryDataModel();
            temp->setItemId(lvlupItem->itemId);
            temp->setType(lvlupItem->itemType);
            temp->setCount(lvlupItem->itemNum);
            rewards->addObject(temp);
            
            
            SGShowRewardBox *rewardBox = SGShowRewardBox::create(this, rewards);
            SGMainManager::shareMain()->showBox(rewardBox);
            
            delete temp;
            
            if (lvlupItem != NULL) {
                ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_inviterLayer);
                lvlupItem->btn->init("reward_font_ylq.png", NULL, lvlupItem, menu_selector(Invite_Item::getGoldHandler),CCPointZero,false,true);

                lvlupItem->state = 3;
            }
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_InviterLayer_str5);
        }
    }
}

void SGInviterLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}


Invite_Item::Invite_Item()
:
state(0),
lvl(0),
gold(0),
item(NULL),
btn(NULL),
iconName(NULL),//头像
borderName(NULL),//边框
countryName(NULL),//国家
advanceNum(0),//+1, +2, +3
icon(NULL),
borderIcon(NULL),//边框
country(NULL),//国家图标
advNum(NULL),//+1 +2 +3
nums(NULL),
itemType(0),
itemId(0),
itemNum(0)
{
	iconName = NULL;//头像
	borderName = NULL;//边框
	countryName = NULL;//国家
	advanceNum = 0;//+1, +2, +3
	advNum = NULL;
	country = NULL;
    nums = NULL;
    itemType = 0;
    itemId = 0;
    itemNum = 0;
}

Invite_Item::~Invite_Item()
{

}

Invite_Item* Invite_Item::create(SGInvietDelegate *dlg,int num,int gold,int station, int itemId, int itemNum, int itemType)
{
	Invite_Item *instance = new Invite_Item();
	if (instance&&instance->init()) {
        instance->lvl = num ;
        instance->gold = gold ;
        instance->delegate = dlg;
        instance->state = station;
        instance->itemId = itemId;
        instance->itemNum = itemNum;
        instance->itemType = itemType;
        instance->setView(station);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

/*
 
 元宝("元宝",(byte)0),
 铜钱("铜钱",(byte)1),
 PVP积分("PVP积分",(byte)2),
 军功("军功",(byte)3),
 小喇叭("小喇叭",(byte)4),
 炼丹秘方("炼丹秘方",(byte)5),
 武将卡("武将卡",(byte)6),
 装备卡("装备卡",(byte)7),
 士兵卡("士兵卡",(byte)8),
 道具卡("道具卡",(byte)9),
 碎片("碎片",(byte)10),
 军魂("军魂",(byte)11),
 体力("体力",(byte)12),
 军粮("军粮",(byte)13),
 天梯积分("天梯积分",(byte)14),
 消耗品("消耗品",(byte)15),
 
 */

//根据类型确定各种icon
void Invite_Item::getSomeIconWithItemType(int itemType, int itemId)
{
    //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14天梯积分 15消耗品
    switch(itemType)
    {
        case 6://将领卡牌
        {
			countryName = NULL;
            SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
            if(officerData)
            {
                SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_inviterLayer);
                iconName=CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString();
                int starlvl = officerData->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                
                borderName=CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString();
                countryName=CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace())->getCString();
				advanceNum = officerData->getAdvNum();
            }
            break;
        }
        case 7://装备卡牌
        {
            //                sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",access->getItemId())->getCString());
            SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(itemId);
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(equip->getIconId(), sg_inviterLayer);
            
            int starlvl = equip->getEquipStarPinzhi();
            iconName=CCString::createWithFormat("head%d.png",equip->getIconId())->getCString();
            borderName=CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString();
            advanceNum = equip->getAdvNum();
			countryName = NULL;
            break;
        }
        case 8://士兵卡牌
        {
            SGSoldierDataModel *soldier = SGStaticDataManager::shareStatic()->getSoliderById(itemId);
            if(soldier)
            {
                ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_inviterLayer);
                iconName=CCString::createWithFormat("%d-1.png",itemId)->getCString();
                borderName=CCString::createWithFormat("country_%d.png",soldier->getSoldierRace())->getCString();
            }
            countryName = NULL;
            break;
        }
        case 9://道具卡牌
        {
            //12.06
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(itemId);
            SGMainManager::shareMain()->addHeadIconPlistByNum(prop->getIconId() , sg_inviterLayer);
            advanceNum = 0;
			countryName = NULL;
            iconName=CCString::createWithFormat("head%d.png",prop->getIconId())->getCString();
            //borderName="jinkuang.png";
			
			int starLvl = prop->getPropsCurrStarLevel();
			if (starLvl > 1)
				starLvl -= 1;
			//有国家
			if (prop->getPropsCountry())
			{
				countryName = CCString::createWithFormat("countryName%d.png",prop->getPropsCountry())->getCString();
				borderName = CCString::createWithFormat("officerPinzhi_%d_0.png", starLvl)->getCString();
			}
			else
			{
				//countryName = NULL;
				borderName = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString();
			}
            
            break;
        }
        case 1://加铜钱
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_inviterLayer);
            countryName = NULL;
			iconName="head9999.png";
            borderName="equipPinzhi_3_0.png";
            break;
        }
        case 0://金子
        {
            countryName = NULL;
            iconName="big_yb.png";
            borderName="equipPinzhi_4_0.png";
            
            break;
        }
        case 3://军功
        {
            countryName = NULL;
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_inviterLayer);
            iconName="head9997.png";
            borderName="jinkuang.png";
            break;
        }
        case 5://炼丹秘方
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_inviterLayer);
            iconName="head9996.png";
            borderName="jinkuang.png";
			countryName = NULL;
            break;
        }
        case 4://小喇叭
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_inviterLayer);
			iconName="head9998.png";
            borderName="jinkuang.png";
			countryName = NULL;
			
            break;
        }
		case 10://碎片
        {
            countryName = NULL;
            SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(itemId);
            int piecesType=pieces->getPiecesType();
            int starlvl = pieces->getStarLevel();
            if (starlvl > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
            {
                starlvl -= 1;
            }
            if (piecesType==0) {
                SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
                if (officerData)
                {
                    SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_inviterLayer);
                    iconName=CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString();
                    
                    borderName=CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString();
                    countryName=CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace())->getCString();
                }
                else
                {
                    iconName = "common_pieces_border.png";
                    borderName = "equipPinzhi_4_0.png";
                }
				
            }else if (piecesType==1)
            {
                SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId());
                
                SGMainManager::shareMain()->addHeadIconPlistByNum(equip->getIconId(), sg_inviterLayer);
                
                iconName=CCString::createWithFormat("head%d.png",equip->getIconId())->getCString();
                borderName=CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString();
            }
            
        }
            break;
        case 11://军魂,
        {
			countryName = NULL;
            iconName="jinkuang.png";
            borderName="jinkuang.png";
        }
        case 12://体力
        {
			countryName = NULL;
			iconName="mail_streng.png";
            borderName="jinkuang.png";
        }
        case 13://军粮
        {
			countryName = NULL;
            iconName="mail_food.png";
            borderName="jinkuang.png";
        }
        case 14://天梯积分
        {
			countryName = NULL;
            iconName="store_tianti_big.png";
            borderName="jinkuang.png";
        }
			
		case 15://消耗品
		{
			countryName = NULL;
			SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(itemId);
			SGMainManager::shareMain()->addHeadIconPlistByNum(consume->getIconId(), sg_inviterLayer);
			iconName = CCString::createWithFormat("head%d.png", consume->getIconId())->getCString();
			
			int starLvl = consume->getConsumeStarLvl();
			if (starLvl > 1)
				starLvl -= 1;
			borderName = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString();
		}
            break;
		default:
			break;
    }
}




void Invite_Item::setView(int station)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    
    item = CCSprite::createWithSpriteFrameName("item_bg.png");
    bgbgbg =item;
    this->addChild(item,-1);
    this->setContentSize(item->getContentSize());
    
    SGCCLabelTTF *lab_up = SGCCLabelTTF::create(str_InviterLayer_str6,FONT_BOXINFO,28 , COLOR_YELLOW);
    this->addChild(lab_up,0,10);
    //modify by:zyc. merge into create.
    //lab_up->setColor(COLOR_YELLOW);
    lab_up->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width *.4, 0)));

    SGCCLabelTTF *lvl = SGCCLabelTTF::create("",FONT_PANGWA,28 , COLOR_ORANGE);
    this->addChild(lvl,0,11);
    //modify by:zyc. merge into create.
    //lvl->setColor(COLOR_ORANGE);
    lvl->setPosition(ccpAdd(lab_up->getPosition(), ccp(70, 0)));
    
    SGCCLabelTTF *font_lvl = SGCCLabelTTF::create(str_InviterLayer_str7,FONT_BOXINFO,28 , COLOR_YELLOW);
    this->addChild(font_lvl,0,12);
    //modify by:zyc. merge into create.
    //font_lvl->setColor(COLOR_YELLOW);
    font_lvl->setPosition(ccpAdd(lvl->getPosition(), ccp(95, 0)));
    

	
    
    /*
    
    CCSprite *yb = CCSprite::createWithSpriteFrameName("store_yb.png");
    yb->setPosition(ccpAdd(font_lvl->getPosition(), ccp(140, 0)));
    this->addChild(yb,0,14);
 
    SGCCLabelTTF *font_num = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    this->addChild(font_num,0,15);
    font_num->setAnchorPoint(ccp(0, 0.5));
    font_num->setPosition(ccpAdd(yb->getPosition(), ccp(yb->getContentSize().width/2+3, 0)));
     */
    
    btn = SGButton::create("reward_lingq.png", NULL, this, menu_selector(Invite_Item::getGoldHandler),CCPointZero,false,true);
    
    SGMenu *menu1 = SGMenu::create(CCRectMake(0, btmhgt, s.width, s.height - btmhgt - headhgt));
    menu1->setTag(111);
    menu1->setPosition(CCPointZero);
    this->addChild(menu1,5);
    menu1->addChild(btn);
    btn->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.4, 0)));
    
    this->getSomeIconWithItemType(itemType, itemId);
    
    //    //这里做对应显示
    icon = SGButton::create(iconName, NULL, this, menu_selector(Invite_Item::showCardDetail), CCPointZero,false, true);
    icon->setScale(0.9);
	this->addBtn(icon);
	this->addChild(icon, 2);
	icon->setPosition(ccpAdd(btn->getPosition(), ccp(-btn->getContentSize().width * 2.4, 0)));

	CCSprite *cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
    cardBg->setScale(0.9);
	this->addChild(cardBg, 1);
	cardBg->setPosition(ccpAdd(icon->getPosition(), ccp(0, -2)));
	
    borderIcon = CCSprite::createWithSpriteFrameName(borderName);
    borderIcon->setScale(0.9);
    this->addChild(borderIcon, 2);
    borderIcon->setPosition(ccpAdd(icon->getPosition(), ccp(0, 0)));
    
    //如果是碎片需要在头像的部分加上蒙版
    if (itemType == 10)
    {
        ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_inviterLayer);
        //头像蒙版
        CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
        headMask->setScale(0.9);
        this->addChild(headMask, 5, 22);
        headMask->setPosition(icon->getPosition());
    }
	
	
	//如果是武将有国家
	if(countryName != NULL)
	{
		country = CCSprite::createWithSpriteFrameName(countryName);
		this->addChild(country, 3);
		country->setPosition(ccpAdd(borderIcon->getPosition(), ccp(-borderIcon->getContentSize().width / 2 + country->getContentSize().width / 2 + 27,
																   borderIcon->getContentSize().height / 2 - 39)));
	}
	if (advanceNum != 0)
	{
        SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",advanceNum)->getCString(), FONT_PANGWA, 24);
        advancenumber->setInsideColor(ccGREEN);
        advancenumber->setAnchorPoint(ccp(1,1));
        advancenumber->ignoreAnchorPointForPosition(false);
        this->addChild(advancenumber, 3);
		advancenumber->setPosition(ccpAdd(borderIcon->getPosition(), ccp(borderIcon->getContentSize().width / 2 - advNum->getContentSize().width / 2 - 13,
																  borderIcon->getContentSize().height / 2 - 25)));
	}
    
    nums = SGCCLabelTTF::create(CCString::createWithFormat("X %d", itemNum)->getCString(), FONT_PANGWA, 16 , ccGREEN);
    //modify by:zyc. merge into create.
    //nums->setColor(ccGREEN);
    this->addChild(nums, 2);
    nums->setPosition(ccpAdd(icon->getPosition(), ccp(icon->getContentSize().width * 0.86, 0)));
    
    updateitem(station, this->lvl, itemId, itemNum, itemType);
}

void Invite_Item::showCardDetail(CCObject *obj)
{
    //CCLOG("Show Detail!");
    //只有装备,武将, 碎片, 消耗品需要显示详情
    SGStaticDataManager* sgsdm = SGStaticDataManager::shareStatic();
    SGMainManager* sgmm = SGMainManager::shareMain();
    SGBaseMilitaryCard* bmc = NULL;
    //注意，由于showBox会关闭当前layer的触控，所以这里提前获得SGInviterLayer的指针并传入。
    SGBoxDelegate* boxDeg = sgmm->getNowShowLayer();
    switch (itemType)
    {
        case 6: //武将
        {
            bmc = sgsdm->officerDataModeToOfficer(itemId);
            sgmm->showCommonInfoBoxInProto(bmc, boxDeg, 26);
//            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)officerCard,26,NULL,true,NULL,10,NULL,0,0); //勿删
        }
            break;
        case 7: //装备
        {
            bmc = sgsdm->equipDataModeToequip(itemId);
            sgmm->showCommonInfoBoxInProto(bmc, boxDeg, 27);
//            SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)equipCard,27,true,NULL,false, NULL, 0, 0); //勿删
        }
            break;
        case 10: //碎片
        {
            bmc = sgsdm->piecesDataModelToPiece(itemId);
            sgmm->showCommonInfoBoxInProto(bmc, boxDeg, 26);
//            SGMainManager::shareMain()->showPiecesInfoLayer(piecesCard, 26, true); //勿删
        }
            break;
        case 15: //消耗品
        {
            bmc = sgsdm->consumableDataModelToConsumable(itemId);
            sgmm->showCommonInfoBoxInProto(bmc, boxDeg, 26);
//            SGMainManager::shareMain()->showConsumableInfoLayer(consumeCard, true, 26, -1); //勿删
        }
            break;
        case 9: //道具（新增）
            bmc = sgsdm->propDataModelToProp(itemId);
            sgmm->showCommonInfoBoxInProto(bmc, boxDeg, 26);
        default:
            break;
    }
}

void Invite_Item::updateitem(int station, int num, int itemId, int itemNum, int itemType)
{
    this->lvl = num;
    
    this->itemNum = itemNum;
    this->itemType = itemType;
    this->itemId = itemId;
    
    this->getSomeIconWithItemType(itemType, itemId);

    state = station;
    SGCCLabelTTF *levl = (SGCCLabelTTF *)this->getChildByTag(11);
    SGCCLabelTTF *font_lvl = (SGCCLabelTTF *)this->getChildByTag(12);
    CCSprite *yb = (CCSprite *)this->getChildByTag(14);
    SGCCLabelTTF *font_num =  (SGCCLabelTTF *)this->getChildByTag(15);

    levl->setString(CCString::createWithFormat("%d",num)->getCString());
    item->setVisible(true);
    btn->setVisible(true);

    levl->setVisible(true);
    font_lvl->setVisible(true);
    
    
    //    //这里做对应显示
    icon->setImage(iconName);
    icon->setCallBack(this, menu_selector(Invite_Item::showCardDetail));
    
    borderIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(borderName)->displayFrame());
	
	
	//如果是武将有国家
	if(countryName != NULL )
	{
        if (!country)
		{
			// 如果没有国家标签,就再创建一个国家标签
			country = CCSprite::createWithSpriteFrameName(countryName);
			this->addChild(country, 3);
			country->setPosition(ccpAdd(borderIcon->getPosition(), ccp(-borderIcon->getContentSize().width / 2 + country->getContentSize().width / 2 + 27,
                                                                borderIcon->getContentSize().height / 2 - 39)));
		}
        else
        {
            country->setDisplayFrame(CCSprite::createWithSpriteFrameName(countryName)->displayFrame());
            country->setVisible(true);
        }
	}
    else
	{
		//如果没有国家标签,那就直接隐藏
		if (country)
		{
			country->setVisible(false);
		}
	}
    
	//如果有+1 +2 +3

        if (itemType == 6 && advanceNum != 0)
        {
            //如果有品阶
            if (advNum)
            {
                advNum->setVisible(true);
                advNum->setString(CCString::createWithFormat("+%d", advanceNum)->getCString());
            }
            else
            {
                advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d", advanceNum)->getCString(), FONT_PANGWA, 24, ccGREEN);
                advNum->setAnchorPoint(ccp(1,1));
                advNum->ignoreAnchorPointForPosition(false);
                this->addChild(advNum, 3);
                advNum->setPosition(ccpAdd(borderIcon->getPosition(), ccp(borderIcon->getContentSize().width / 2 - advNum->getContentSize().width / 2 - 13,
                                                                          borderIcon->getContentSize().height / 2 - 25)));
            }
        }
        else
        {
            if (advNum)
            {
                //如果没有,就直接隐藏
                advNum->setVisible(false);
            }
        }

    nums->setString(CCString::createWithFormat("X %d", itemNum)->getCString());
    
    //如果是碎片需要在头像的部分加上蒙版
    if (itemType == 10)
    {
        ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_inviterLayer);
        //头像蒙版
        CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
        headMask->setScale(0.9);
        this->addChild(headMask, 5);
        headMask->setPosition(icon->getPosition());
    }
	else
    {
        if (this->getChildByTag(22))
        {
            this->getChildByTag(22)->removeFromParentAndCleanup(true);
        }
    }
    
    
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_inviterLayer);
    if (station == 3 )
    {
        btn->init("reward_font_ylq.png", NULL, this, menu_selector(Invite_Item::getGoldHandler),CCPointZero,false,true);
    }
    else if(station == 1)
    {
        btn->init("reward_lingq_hui.png", NULL, this, menu_selector(Invite_Item::getGoldHandler),CCPointZero,false,true);
    }
    else
    {
        btn->init("reward_lingq.png", NULL, this, menu_selector(Invite_Item::getGoldHandler),CCPointZero,false,true);
    }
}

void Invite_Item::getGoldHandler(cocos2d::CCNode *sender)
{
    if (state == 3)
    {
        SGMainManager::shareMain()->showMessage(str_InviterLayer_str9);
        EFFECT_PLAY(MUSIC_BTN);
    }else if (state ==1)
    {
        SGMainManager::shareMain()->showMessage(str_InviterLayer_str10);
        EFFECT_PLAY(MUSIC_BTN);
    }else if(delegate) {
        delegate->getGoldHandler(this);
    }
}
