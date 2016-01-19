//
//  SGJitianLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#include "SGJitianLayer.h"
#include "SGMainManager.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGRewardLdBox.h"
#include "ResourceManager.h"
#include "SGMailSystemDataModel.h"
#include "SGGeneralInfoLayer.h"
#include "SGShowRewardBox.h"
#include "AppMacros.h"
#include "SGStringConfig.h"
#include "SGRewardLayer.h"
#include "SGCommonInfoBox.h"

SGJitianLayer::SGJitianLayer()
:
curr(0),
maxx(0),
iscan(0),
label1(NULL),
jitianBtn(NULL),
backBtn(NULL),
_rewardList(NULL),
redBg(NULL)
{
    jitianBtn = NULL;
}
SGJitianLayer::~SGJitianLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_GOD);
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_jitianLayer);
    CC_SAFE_RELEASE(_rewardList);
}

SGJitianLayer *SGJitianLayer::create(int current, int max,int cando,CCArray*array)
{
    SGJitianLayer *Layer = new SGJitianLayer();
    if (Layer && Layer->init(NULL , sg_jitianLayer))
    {
        Layer->iscan = cando;
        Layer->curr = current;
        Layer->maxx = max;
        Layer->initView(array);
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGJitianLayer::initView(CCArray*array)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_jitianLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_GOD,
                              this,
                              callfuncO_selector(SGJitianLayer::godListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/reward_reg.plist", RES_TYPE_LAYER_UI, sg_jitianLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("reward_reg.png");
    bg->setScaleY((s.height - headhgt - btmhgt)/702);
    bg->setScaleX(s.width/768);
    this->addChild(bg,-5,55);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));

//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGJitianLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGJitianLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    
       
//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_jitian.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_JitianLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    
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
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65*.48)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));

    
   
    
    CCSprite *hd = CCSprite::createWithSpriteFrameName("hd.png");
	hd->setOpacity(255 * .8f);
    this->addChild(hd,-1);
    hd->setScaleY(1.12);
    hd->setPosition(ccp(s.width/2,195));


    
    if (iscan) {
//        jitianBtn = SGButton::create("jitian_btn.png", "reward_font_jitian.png", this, menu_selector(SGJitianLayer::confirmHandler),CCPointZero,false,true);
         jitianBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_JitianLayer_str1, this, menu_selector(SGJitianLayer::confirmHandler),ccp(0,2),FONT_XINGKAI,ccc3(0xff, 0xff, 0xff), 36);
    }
    else
    {
       jitianBtn = SGButton::createFromLocal("barrack_anniu_hui.png", str_JitianLayer_str1, this, menu_selector(SGJitianLayer::confirmHandler),ccp(0,2),FONT_XINGKAI,ccc3(0xff, 0xff, 0xff), 36);
    }
    this->addBtn(jitianBtn);
    jitianBtn->setPosition(ccpAdd(hd->getPosition(), ccp(0, -2)));
    ///////////////////////可能获得的奖励们
	
	
	int left_shift = 0;
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		left_shift = 55;
	}
	
	label1 = CCLabelTTF::create(str_JitianLayer_str2, FONT_PANGWA, 20);
    this->addChild(label1);
    label1->setAnchorPoint(ccp(0, 0.5));
    label1->setPosition(ccpAdd(jitianBtn->getPosition(), ccp(jitianBtn->getContentSize().width * 1.06, -2)));
	
	CCLabelTTF *counts = CCLabelTTF::create(CCString::createWithFormat(" %d/%d ", curr, maxx)->getCString(), FONT_PANGWA, 24);
	counts->setColor(ccRED);
	this->addChild(counts, 1, 11);
	counts->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width + counts->getContentSize().width / 2, 0)));
    
	CCLabelTTF *counts1 = CCLabelTTF::create(str_JitianLayer_str3, FONT_PANGWA, 20);
	this->addChild(counts1);
	counts1->setPosition(ccpAdd(counts->getPosition(), ccp(counts->getContentSize().width / 2 + counts1->getContentSize().width / 2, 0)));
	
	//上行提示信息部分
	SGCCLabelTTF *getTips = SGCCLabelTTF::create(str_JitianLayer_str4, FONT_PANGWA, 20);
	getTips->setAnchorPoint(ccp(0, 0.5));
	this->addChild(getTips, 1);
	
	//连续祭祀%d天#F3F100
	SGCCLabelTTF *golds = SGCCLabelTTF::create(str_JitianLayer_str5, FONT_PANGWA, 20 , ccc3(0xf3, 0xf1, 0x00));
	//golds->setColor(ccc3(0xf3, 0xf1, 0x00));
	golds->setAnchorPoint(ccp(0, 0.5));
	this->addChild(golds, 1);
	
	SGCCLabelTTF *rewardTips = SGCCLabelTTF::create(str_JitianLayer_str6, FONT_PANGWA, 20);
	rewardTips->setAnchorPoint(ccp(0, 0.5));
	this->addChild(rewardTips, 1);
	
	
	redBg = CCSprite::createWithSpriteFrameName("jisi_bottom.png");

	
	this->addChild(redBg, 0);
	redBg->setPosition(ccpAdd(hd->getPosition(), ccp(0, hd->getContentSize().height / 2 + redBg->getContentSize().height / 2)));
	
	getTips->setPosition(ccpAdd(redBg->getPosition(), ccp(10 - redBg->getContentSize().width * 0.5f, redBg->getContentSize().height * 0.5f - 0.8f * getTips->getContentSize().height ))); 
	//getTips->setPosition(ccpAdd(redBg->getPosition(), ccp(-redBg->getContentSize().width / 2.08 * scale,
	//													  redBg->getContentSize().height / 2 - getTips->getContentSize().height * 0.8)));
	
	golds->setPosition(ccpAdd(getTips->getPosition(), ccp(getTips->getContentSize().width, 0)));
	rewardTips->setPosition(ccpAdd(golds->getPosition(), ccp(golds->getContentSize().width, 0)));
	
	
	
	SGCCLabelTTF *tipInfo = SGCCLabelTTF::create(str_JitianLayer_str7, FONT_PANGWA, 20);
	tipInfo->setAnchorPoint(ccp(0, 0.5));
	this->addChild(tipInfo, 1);
	tipInfo->setPosition(ccpAdd(rewardTips->getPosition(), ccp(rewardTips->getContentSize().width * 1.5, 0)));
	
	SGCCLabelTTF *counts2 = SGCCLabelTTF::create(CCString::createWithFormat(str_JitianLayer_str8, maxx)->getCString(), FONT_PANGWA, 20 , ccc3(0 , 255, 60));
	//counts2->setColor(ccc3(0, 255, 60));
	counts2->setAnchorPoint(ccp(0, 0.5));
	this->addChild(counts2, 1, 10);
	counts2->setPosition(ccpAdd(tipInfo->getPosition(), ccp(tipInfo->getContentSize().width, 0)));
	
	SGCCLabelTTF *tailInfo = SGCCLabelTTF::create(str_JitianLayer_str9, FONT_PANGWA, 20);
	tailInfo->setAnchorPoint(ccp(0, 0.5));
	this->addChild(tailInfo, 1);
	tailInfo->setPosition(ccpAdd(counts2->getPosition(), ccp(counts2->getContentSize().width, 0)));
    
    initReward(array);
    
    //////////////////////
}
void SGJitianLayer::godListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_jitianLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
        int nsum = array->count();
        for (int i = 0 ;i < nsum ;i++)
        {
            SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
            if(item->gettypeItem()==Reward_Hope)
            {
                item->setstateItem(0);
                break;
            }
        }


        const char *msg= str_JitianLayer_str10;
        main::RewardGodPrayAddResponse *response = (main::RewardGodPrayAddResponse *)sr->m_msg;
        EFFECT_PLAY(MUSIC_57);
        
        if (response->result() == 1)
        {
            curr++;
            
			CCArray *array=CCArray::create();
			array->retain();
            
            for (int i=0; i<response->godrewardinfo_size(); i++)
            {
                main::GodRewardInfo info =response->godrewardinfo(i);
                
                SGMailSystemAccessoryDataModel* data=new SGMailSystemAccessoryDataModel();
                data->setCount(info.count());
                data->setItemId(info.itemid());
                data->setType(info.type());
                
                
                array->addObject(data);
            }
            
            SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, array);
            SGMainManager::shareMain()->showBox(rewardBox);
        }
        else
        {
            msg = str_JitianLayer_str12;
            EFFECT_PLAY(MUSIC_BTN);
        }
        CCLabelTTF * label = ((CCLabelTTF *)this->getChildByTag(11));
		if (label)
        {
            label->setString(CCString::createWithFormat("%d/%d", curr, maxx)->getCString());
            if(curr == maxx)
                label->setColor(ccGREEN);
        }
        
//        if (((SGCCLabelTTF*)this->getChildByTag(10)))
//			((SGCCLabelTTF*)this->getChildByTag(10))->setString(CCString::createWithFormat(str_JitianLayer_str8,response->max())->getCString());
        SGMainManager::shareMain()->showMessage(msg);
        
        if (jitianBtn != NULL)
            jitianBtn->setImage("barrack_anniu_hui.png");
    }
}



void SGJitianLayer::confirmHandler()
{
    
    if (iscan) {
        main::RewardGodPrayAddRequest *request = new main::RewardGodPrayAddRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GOD, request);
        EFFECT_PLAY(MUSIC_37);
		
    }else
    {
        SGMainManager::shareMain()->showMessage(str_JitianLayer_str12);
        EFFECT_PLAY(MUSIC_BTN);
    }    
}
void SGJitianLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}

 void SGJitianLayer::showequipinfo(CCNode*node)
{
    int index=node->getTag();
    if (index < 0 || index >= _rewardList->count())
    {
        CCLOG("jitian reward index error!");
        return;
    }
    
    SGMailSystemAccessoryDataModel* data = dynamic_cast<SGMailSystemAccessoryDataModel*>(_rewardList->objectAtIndex(index));
    if (!data)
    {
        CCLOG("jitian reward item error!");
        return;
    }
    
    int type = (SGBaseItemType) data->getType();
    int tag = data->getItemId();
    
    SGBaseMilitaryCard* bmc = NULL;
    SGStaticDataManager *sgsdm = SGStaticDataManager::shareStatic();
    SGMainManager* sgmm = SGMainManager::shareMain();
    
    int otherEt = 0;
    bool isDel = true;
    
    switch (type)
    {
        case BIT_OFFICER: //武将
        {
            bmc = sgsdm->officerDataModeToOfficer(tag);
            otherEt = 23;
            isDel = false;
            /*SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create((SGOfficerCard *)officerCard,23);
             SGMainManager::shareMain()->showLayer(Layer,false);*/ //勿删
        }
            break;
        case BIT_EQUIP: //装备
        {
            bmc = sgsdm->equipDataModeToequip(tag);
            otherEt = 20;
            /*SGMainManager::shareMain()->showEquipInfoLayer(equipCard,20);*/ //勿删
        }
            break;
        case BIT_PIECE: //碎片
        {
            bmc = sgsdm->piecesDataModelToPiece(tag);
            otherEt = 26;
            //                SGMainManager::shareMain()->showPiecesInfoLayer(piecesCard, 26, true); //勿删
        }
            break;
        case BIT_CONSUMABLE: //消耗品
        {
            bmc = sgsdm->consumableDataModelToConsumable(tag);
            otherEt = 26;
            //                SGMainManager::shareMain()->showConsumableInfoLayer(consumeCard, true, 26, -1); //勿删
        }
            break;
        case BIT_MATERIAL: //材料
        {
            bmc = sgsdm->materialDataModelToMaterial(tag);
            otherEt = 9;
            /*SGMainManager::shareMain()->showPropInfoLayer(propCard, 9);*/ //勿删
        }
            break;
        case BIT_PROP: //道具
        {
            bmc = sgsdm->propDataModelToProp(tag);
            otherEt = 9;
            /*SGMainManager::shareMain()->showPropInfoLayer(propCard, 9);*/ //勿删
        }
            break;
        case BIT_SOLDIER: //士兵
        {
            SGSoldierDataModel *oCard = SGStaticDataManager::shareStatic()->getSoliderById(tag);
            SGSoldierCard *soldierCard = new SGSoldierCard();
            soldierCard->setItemId(oCard->getSoldierFileId());
            soldierCard->setAtk(oCard->getSoldierBaseAtk());
            soldierCard->setSsid(oCard->getSoldierNum());
            //            soldierCard->setMaxExp(oCard->getsoldier);
            soldierCard->setOfficerName(oCard->getSoldierName());
            soldierCard->setDef(oCard->getSoldierBaseDef());
            soldierCard->setRound(oCard->getSoldierRound());
            soldierCard->setMorale(oCard->getSoldierMorale());
            soldierCard->setCurrLevel(1);
            //            soldierCard->setCurrStar(3);
            soldierCard->setCurrExp(0);
            //            soldierCard->setItemType();
            soldierCard->setRace(oCard->getSoldierRace());
            soldierCard->autorelease();
            //显示物品
            bmc = soldierCard;
            otherEt = 8;
            /*SGMainManager::shareMain()->showSoldierInfoLayer(soldierCard,8);*/
        }
            break;
        default:
            break;
    }
    
    if (bmc && otherEt != 0)
    {
        sgmm->showCommonInfoBoxInProto(bmc, this, otherEt, isDel);
    }
}


 void SGJitianLayer::initReward(CCArray*array)
{
    _rewardList = array;
    _rewardList->retain();
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    int num=array->count();
    float temp = (redBg->getContentSize().width - 140)/4.0;
    for (int i=0; i<num; i++)
    {
        SGMailSystemAccessoryDataModel*data = (SGMailSystemAccessoryDataModel*) array->objectAtIndex(i);
        HeadInfo headInfo=SGMailManager::getAccessoryInfoByData(data);
        ///////
        
        //防御，如果没东西，就别往下继续构造显示item了
        if (headInfo.type == nullCard)
        {
            continue;
        }
        
        SGButton *bt = SGButton::create(headInfo.fileName,headInfo.kuangName, this, menu_selector(SGJitianLayer::showequipinfo),CCPointZero,false,true);
        
        float mmm=winsize.width-bt->getContentSize().width*(num+1.2);
        float xxx=bt->getContentSize().width*1.2*(i/float(num-1))+bt->getContentSize().width*i;
        
//        if(num==1)
//        {
//			bt->setAnchorPoint(ccp(0.5, 0));
//			CCPoint center=SGLayout::getPoint(kMiddleCenter);
//			bt->setPosition(ccp(center.x, jitianBtn->getPosition().y+jitianBtn->getContentSize().height*1));
//        }
//        else
//        {
//			bt->setPosition(ccp(mmm*0.5+xxx, jitianBtn->getPosition().y+jitianBtn->getContentSize().height*1));
//			bt->setAnchorPoint(ccp(0, 0));
//        }

        bt->setAnchorPoint(ccp( 0 , 0));
        bt->setPosition(ccp( redBg->getPositionX() - redBg->getContentSize().width*0.5 + 20 + temp*i, jitianBtn->getPosition().y+jitianBtn->getContentSize().height*1));
        
        addBtn(bt);
        bt->setTag(i);
        
        CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
        this->addChild(background,-1);
        background->setAnchorPoint(ccp(0, 0));
        background->setPosition(bt->getPosition());
    }
}
