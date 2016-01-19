//
//  SGRewardBindLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-20.
//
//

#include "SGRewardBindLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGMainLayer.h"
#include "SGRewardLayer.h"

#include <regex.h>
#include "SGRichLabel.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"

SGRewardBindLayer::SGRewardBindLayer()
:
m_pEditName(NULL),
m_pEditPassword(NULL),
m_pEditPasswordAgain(NULL),
lab_iscanget(NULL),
lab_notice(NULL),
lab_sec(NULL),
_arry(NULL),
scrollHeight(0),
scrollLayer(NULL),
scroll(NULL),
entertype(0),
registerButton(NULL),
m_pbuttonGetReward1(NULL),
m_pbuttonGetReward2(NULL),
m_nStateEmail(0),
m_nStateAccount(0),
m_pEditBoxEmail(NULL),
m_pEditBoxEmailAgain(NULL),
m_pLabelBindEmailInfo(NULL),
m_nRequestState(0)

{
    ;
}

SGRewardBindLayer::~SGRewardBindLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bindLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_BIND);
    notification->removeObserver(this, MSG_REWARD_BIND_MAILBOX_INFO);
    notification->removeObserver(this, MSG_REWARD_GET_BIND_MAILBOX);

    
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardRegPlayers();
    if (array->count()/2>=scrollHeight)
    {
        for (int i = scrollHeight*2-1 ; i>=0; i--)
        {
            array->removeObjectAtIndex(i);
        }
    }
    if(_arry)
    {
        _arry->release();
    }
}
SGRewardBindLayer *SGRewardBindLayer::create(CCArray *arry,int type,CCArray *arry2,int type2,const char * mail)
{
    SGRewardBindLayer *Layer = new SGRewardBindLayer();
    Layer->entertype=type;
    Layer->_arry=arry;
    Layer->_arry2 = arry2;
    Layer->m_nStateAccount = type;
    Layer->m_nStateEmail = type2;
    Layer->_arry->retain();
    Layer->_arry2->retain();
    Layer->m_pStrMailAddress =mail;
    if (Layer && Layer->init(NULL , sg_bindLayer))
    {
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGRewardBindLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_bindLayer);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_bindLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_bindLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_bindLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_BIND,
                              this,
                              callfuncO_selector(SGRewardBindLayer::regListener));
    notification->addObserver(MSG_REWARD_BIND_MAILBOX_INFO,
                              this,
                              callfuncO_selector(SGRewardBindLayer::bindEmailListener));
    notification->addObserver(MSG_REWARD_GET_BIND_MAILBOX,
                              this,
                              callfuncO_selector(SGRewardBindLayer::getRewardBindEmailListener));
    
     center = SGLayout::getPoint(kMiddleCenter);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

//    CCSprite *bg = CCSprite::create("sanguobigpic/storyBg.png",CCRect(60,headhgt+20,s.width,(s.height - headhgt - btmhgt)));

//    this->addChild(bg,-5);
//    bg->setAnchorPoint(ccp(0.5, 0));
//    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));

//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGRewardBindLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGRewardBindLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setTag(88);
    menu->setZOrder(10);
    
    title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 0));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt-3)));
    this->addChild(title_bg,8);
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_bind_get.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_RewardBindLayer_str12, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    
    this->addChild(title,10);
    
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
    
    CCSprite *kuang = CCSprite::createWithSpriteFrameName("bangdingkuang.png");
    kuang->setPosition(ccpAdd(center, ccp(0, -55)));
    this->addChild(kuang);
    CCSprite *kbg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    kbg->setPosition(kuang->getPosition());
    kbg->setScale(kuang->getContentSize().width/kbg->getContentSize().width);
    this->addChild(kbg);
    

    
    //提示字内容
     const char * words1=str_RewardBindLayer_str1;
     const char * words2=str_RewardBindLayer_str2;
    
    //绑定按键
    registerButton = SGButton::create("loginbtnbg.png", "reward_font_bind.png", this, menu_selector(SGRewardBindLayer::registerHandler),CCPointZero,false,true);
    this->addBtn(registerButton);
    registerButton->setPosition(ccpAdd(center,ccp(0,-275)));
    
    
       if(m_nStateAccount == 0)
    {
        CCSprite* info = SGRichLabel::getFomLabel(words1, CCSizeMake(30*16,120),FONT_PANGWA, 20,ccYELLOW);
        this->addChild(info);
        info->setAnchorPoint(ccp(0.5,1));
        info->setPosition(ccpAdd(center,ccp(0,170)));
    }
    else
    {
        if(m_nStateAccount == 1)
        {
            m_pbuttonGetReward1 =  SGButton::createFromLocal("store_lank.png", str_GoldShopLimitTimeSell_str14, this, menu_selector(SGRewardBindLayer::registerHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
            this->addBtn(m_pbuttonGetReward1);
            m_pbuttonGetReward1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(200,  -headhgt +300)));
            registerButton->setVisible(false);
            
            CCSprite* info = SGRichLabel::getFomLabel(words2, CCSizeMake(30*16,120),FONT_PANGWA, 20,ccYELLOW);
            this->addChild(info);
            info->setAnchorPoint(ccp(0.5,1));
            info->setPosition(ccpAdd(center,ccp(0,170)));


            
        }
        else if(m_nStateAccount ==2)
        {
            m_pbuttonGetReward1 =  SGButton::createFromLocal("store_huik.png", str_RewardBindLayer_str101, this, menu_selector(SGRewardBindLayer::registerHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
            this->addBtn(m_pbuttonGetReward1);
            m_pbuttonGetReward1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(200,  -headhgt +300)));
            m_pbuttonGetReward1->setEnabled(false);
            registerButton->setVisible(false);
            
            CCSprite* info = SGRichLabel::getFomLabel(words2, CCSizeMake(30*16,120),FONT_PANGWA, 20,ccYELLOW);
            this->addChild(info);
            info->setAnchorPoint(ccp(0.5,1));
            info->setPosition(ccpAdd(center,ccp(0,170)));
            registerButton->setVisible(false);
        }
        
        if(m_nStateEmail ==0)
        {
            m_pbuttonGetReward2 =  SGButton::createFromLocal("store_lank.png", str_RewardBindLayer_str102, this, menu_selector(SGRewardBindLayer::OnbindEmail),CCPointZero,FONT_PANGWA,ccWHITE,32);
            this->addBtn(m_pbuttonGetReward2);
            m_pbuttonGetReward2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(200,  -headhgt + 65*.48)));
            
             
            m_pEditBoxEmail = CCEditBox::create(CCSizeMake(320, 40), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
            m_pEditBoxEmail->setPlaceHolder(str_RewardBindLayer_str103);
            m_pEditBoxEmail->setMaxLength(30);
            m_pEditBoxEmail->setReturnType(kKeyboardReturnTypeDone);
            m_pEditBoxEmail->setInputMode(kEditBoxInputModeEmailAddr);
           // m_pEditBoxEmail->setDelegate(this);
            addChild(m_pEditBoxEmail);
            m_pEditBoxEmail->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-80,  -headhgt + 40)));
            
            
            m_pEditBoxEmailAgain = CCEditBox::create(CCSizeMake(320, 40), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
            m_pEditBoxEmailAgain->setPlaceHolder(str_RewardBindLayer_str104);
            m_pEditBoxEmailAgain->setMaxLength(30);
            m_pEditBoxEmailAgain->setReturnType(kKeyboardReturnTypeDone);
            m_pEditBoxEmailAgain->setInputMode(kEditBoxInputModeEmailAddr);
            // m_pEditBoxEmail->setDelegate(this);
            addChild(m_pEditBoxEmailAgain);
            m_pEditBoxEmailAgain->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-80,  -headhgt -10)));
            
            m_pLabelBindEmailInfo = SGCCLabelTTF::create(str_RewardBindLayer_str105,FONT_PANGWA,20, CCSizeMake(30*16,120),ccWHITE);
            addChild(m_pLabelBindEmailInfo);
            m_pLabelBindEmailInfo->cocos2d::CCNode::setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,  -headhgt +190)));
        }
        else if(m_nStateEmail ==1)
        {
            m_pbuttonGetReward2 =  SGButton::createFromLocal("store_lank.png", str_GoldShopLimitTimeSell_str14, this, menu_selector(SGRewardBindLayer::OnbindEmail),CCPointZero,FONT_PANGWA,ccWHITE,32);
            this->addBtn(m_pbuttonGetReward2);
            m_pbuttonGetReward2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(200,  -headhgt + 65*.48)));
            const char * strEmail = setTheMail(m_pStrMailAddress);
            m_pLabelBindEmailInfo = SGCCLabelTTF::create(CCString::createWithFormat(str_RewardBindLayer_str106,strEmail)->getCString(),FONT_PANGWA,20, CCSizeMake(30*16,120),ccWHITE);
            addChild(m_pLabelBindEmailInfo);
            m_pLabelBindEmailInfo->cocos2d::CCNode::setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,  -headhgt +190)));
            free((void*)strEmail);
        }
        else if(m_nStateEmail ==2)
        {
            m_pbuttonGetReward2 =  SGButton::createFromLocal("store_huik.png", str_RewardBindLayer_str101, this, menu_selector(SGRewardBindLayer::OnbindEmail),CCPointZero,FONT_PANGWA,ccWHITE,32);
            this->addBtn(m_pbuttonGetReward2);
            m_pbuttonGetReward2->setEnabled(false);
            m_pbuttonGetReward2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(200,  -headhgt + 65*.48)));
            const char * strEmail = setTheMail(m_pStrMailAddress);
            m_pLabelBindEmailInfo = SGCCLabelTTF::create(CCString::createWithFormat(str_RewardBindLayer_str106,strEmail)->getCString(),FONT_PANGWA,20, CCSizeMake(30*16,120),ccWHITE);
            addChild(m_pLabelBindEmailInfo);
            m_pLabelBindEmailInfo->cocos2d::CCNode::setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,  -headhgt +190)));
            free((void*)strEmail);
        }
        
    }
    
    if(m_nStateAccount!=0)
    {
        center = ccpAdd(center, ccp(0,30));
        initReward(_arry);
        center = ccpAdd(center, ccp(0,-200));
        initReward(_arry2);
    }
    else{
        center = ccpAdd(center, ccp(100,-100));
        initReward(_arry);
    }
    
    
}
void SGRewardBindLayer::OnbindEmail()
{
    EFFECT_PLAY(MUSIC_BTN);
    if(m_nStateEmail ==0)
    {
        bool bool_editName = this->is_email_valid(m_pEditBoxEmail->getText());
        if(!bool_editName)
        {
            SG_SHOW_WINDOW(str_RewardBindLayer_str107);
            return;
        }else
        {
            if(0==strcmp(m_pEditBoxEmail->getText(),m_pEditBoxEmailAgain->getText()))
            {
                main::BindMailRequest *request = new main::BindMailRequest();
                request->set_state(m_nRequestState);
                m_pStrMailAddress = m_pEditBoxEmail->getText();
                m_strMail = CCString::createWithData((const unsigned char *)m_pStrMailAddress, strlen(m_pStrMailAddress));
                m_strMail->retain();
                request->set_mail(m_strMail->getCString());
                SGSocketClient::sharedSocketClient()->send(MSG_REWARD_BIND_MAILBOX_INFO, request);
            }
            else
            {
                SG_SHOW_WINDOW(str_RewardBindLayer_str108);
                return;
            }
        }

        
    }
    else if(m_nStateEmail ==1)
    {
        main::RewardBindMailRequest *request = new main::RewardBindMailRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GET_BIND_MAILBOX, request);
        
    }
    else if(m_nStateEmail ==2)
    {
        
    }

}
void SGRewardBindLayer::bindEmailListener(cocos2d::CCObject *obj)
{
     SGSocketRequest *sr = (SGSocketRequest *)obj;
    if(sr)
    {
        main::BindMailResponse *response = (main::BindMailResponse *)sr->m_msg;
        int state  =   response->state();
        if(state == 0)
        {
            SG_SHOW_WINDOW(str_RewardBindLayer_str109);
        }else if(state == -1)
        {
            SG_SHOW_WINDOW(str_RewardBindLayer_str110);
        }else if(state ==2)
        {
            //SG_SHOW_WINDOW("邮箱绑定成功!");
            m_pbuttonGetReward2 ->setFont(str_GoldShopLimitTimeSell_str14);
            //m_pbuttonGetReward2->setImage("store_huik.png");
            //m_pbuttonGetReward2->setEnabled(false);
            if(m_pEditBoxEmail)
                m_pEditBoxEmail->setVisible(false);
            if(m_pEditBoxEmailAgain)
                m_pEditBoxEmailAgain->setVisible(false);
            
            const char * strEmail = setTheMail(m_pEditBoxEmail->getText());
            m_pLabelBindEmailInfo->setString(CCString::createWithFormat(str_RewardBindLayer_str111,strEmail)->getCString());
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 70, 0,0, NULL,0,NULL);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
            m_nStateEmail =1;
            free((void*)strEmail);

        }else if(state == 1 )
        {
            const char* str = m_pEditBoxEmail->getText();
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 69, 0,0, NULL,0,m_strMail->getCString());
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }
    }
}
void SGRewardBindLayer::getRewardBindEmailListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_bindLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        //弹出框显示奖励
        CCArray *data=CCArray::create();
        data->retain();
        
        
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
        main::RewardBindMailResponse *response = (main::RewardBindMailResponse *)sr->m_msg;
        int state = response->state();
        if(state==1)
        {
            
            main::RewardItemListProto rewardItemList = (main::RewardItemListProto)response->itemlist();
            int gold = rewardItemList.gold();
           // playerInfo->setPlayerGold(gold+playerInfo->getPlayerGold());
            int coin = rewardItemList.coin();
           // playerInfo->setPlayerCoins(coin+playerInfo->getPlayerCoins());
            
            int pvp = rewardItemList.pvpscore();
            //playerInfo->setplayerPvpNum(pvp+playerInfo->getplayerPvpNum());
            
//            int jg = rewardItemList.award();
//            playerInfo->setplayerAward(jg+playerInfo->getplayerAward());
            
//            int sp = rewardItemList.laba();
//            playerInfo->setSpeakerCount(playerInfo->getSpeakerCount()+sp);
            
            int ld = rewardItemList.liandan();
            //playerInfo->setPlayerliandan(ld+playerInfo->getPlayerliandan());
            
            
            //领取奖励数组初始化
            {
                //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭
                if (gold>0)
                {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(0);
                    datamodel->setCount(gold);
                    data->addObject(datamodel);
                }
                if (coin>0)
                {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(1);
                    datamodel->setCount(coin);
                    data->addObject(datamodel);
                }
                if (pvp>0)
                {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(14);
                    datamodel->setCount(pvp);
                    data->addObject(datamodel);
                }
//                if (jg>0) {
//                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
//                    datamodel->setType(3);
//                    datamodel->setCount(jg);
//                    data->addObject(datamodel);
//                }
//                if (sp>0) {
//                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
//                    datamodel->setType(4);
//                    datamodel->setCount(sp);
//                    data->addObject(datamodel);
//                }
                if (ld>0)
                {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(5);
                    datamodel->setCount(ld);
                    data->addObject(datamodel);
                }
                
            }
            
            int length1=rewardItemList.officercarditem_size();
            for (int i=0;i<length1;i++)
            {                
                if(rewardItemList.officercarditem(i).itemid()>0)
                {
                    //领奖弹出框
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(6);
                    datamodel->setCount(1);
                    datamodel->setItemId(rewardItemList.officercarditem(i).itemid());
                    data->addObject(datamodel);
                    //
                    SGOfficerCard *officerCard = new SGOfficerCard();
                    CCDictionary *dicc = SGCardAttribute::getValue(rewardItemList.officercarditem(i).currlevel(), rewardItemList.officercarditem(i).itemid());
                    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(rewardItemList.officercarditem(i).itemid());
                    int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(rewardItemList.officercarditem(i).itemid());
                    //                    if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
                    //                        type = 0;
                    //                    }
                    //                    else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
                    //                        type = 1;
                    //                    }
                    //                    else {
                    //                        type = 2;
                    //                    }
                    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, rewardItemList.officercarditem(i).currlevel()+1);
                    
                    officerCard->setSsid(rewardItemList.officercarditem(i).ssid());
                    officerCard->setItemId(rewardItemList.officercarditem(i).itemid());
                    officerCard->setCurrExp(rewardItemList.officercarditem(i).currexp());
                    officerCard->setLordId(rewardItemList.officercarditem(i).lordskillid());
                    officerCard->setIsProt(rewardItemList.officercarditem(i).isprotection());
                    //                officerCard->setIsExped(proto.isexpedition());
                    officerCard->setCurrLevel(rewardItemList.officercarditem(i).currlevel());
                    
                    if (temp->getOfficerNextId()) {
                        officerCard->setCost(temp->getOfficerCost());
                        
                        //                        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
                        //                        int len = arr->count();
                        //                        for (int j=0; j < len; j++) {
                        //                            officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
                        //                        }
                    }
                    officerCard->setExpRate(temp->getOfficerExpRate());
                    officerCard->setMaxStar(temp->getOfficerNextId());
                    officerCard->setMaxExp(expdata->getExpValue());
                    officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
                    officerCard->setOfficerName(temp->getOfficerName());
                    officerCard->setMaxLevel(temp->getOfficerMaxLevel());
                    officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                    officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                    officerCard->setRound(temp->getOfficerRound());
                    officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                    officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                    officerCard->setGoverning(temp->getOfficerGoverning());
                    officerCard->setRace(temp->getOfficerRace());
                    officerCard->setItemType(temp->getOfficerType());
                    //转生最大星级
                    officerCard->setUpgradestar(temp->getUpgradestar());
                    //转生等级
                    officerCard->setUpgradelevel(temp->getUpgradelevel());
                    //武将计最大等级
                    officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
                    //主将计最大等级
                    officerCard->setLordMaxLevel(temp->getLordMaxLevel());
                    //性别和缘分
                    officerCard->setGender((OfficerGender) temp->getOfficerGender());
                    officerCard->setFateList(temp->getOfficerFateList());
                    //转生次数
                    officerCard->setAdNum(temp->getAdvNum());
                    //武将原型ID
                    officerCard->setProtoId(temp->getOfficerProtoId());
                    //速度修正系数
                    officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
					
					officerCard->setHeadSuffixNum(temp->getIconId());//12.05
                    
                    if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
                    {
                        std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                        officerCard->setOfficerSkil(atoi(generalskill.c_str()));
                    }
                    if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
                    {
                        std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                        officerCard->setLordSkill(atoi(lordskill.c_str()));
                    }
                    //playerInfo->addOfficerCard(officerCard);
                    
                    officerCard->autorelease();
                    
                }
            }
            
            
            int length2 = rewardItemList.equipmentcarditem_size();
            for (int i=0; i<length2; i++)
            {
                
                
                if(rewardItemList.equipmentcarditem(i).itemid()>0){
                    
                    //领奖弹出框
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(7);
                    datamodel->setCount(1);
                    datamodel->setItemId(rewardItemList.equipmentcarditem(i).itemid());
                    data->addObject(datamodel);
                    
                    
                    //                    int curr = 0;
                    //                    main::EquipmentCardItemProto cardInfo = response->equipmentcard();
                    SGEquipCard *equipCard = new SGEquipCard();
                    CCDictionary *dicc = SGCardAttribute::getValue(rewardItemList.equipmentcarditem(i).currlevel(), rewardItemList.equipmentcarditem(i).itemid());
                    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(rewardItemList.equipmentcarditem(i).itemid());
                    int type = SGStaticDataManager::shareStatic()->getStrengEquipType(rewardItemList.equipmentcarditem(i).itemid());
                    //                    if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
                    //                        type = 0;
                    //                    }
                    //                    else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
                    //                        type = 1;
                    //                    }
                    //                    else {
                    //                        type = 2;
                    //                    }
                    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, rewardItemList.equipmentcarditem(i).currlevel()+1);
                    //转生等级
                    equipCard->setUpgradelevel(temp->getUpgradelevel());
                    //转生次数
                    equipCard->setAdNum(temp->getAdvNum());
                    //转生最大星级
                    equipCard->setUpgradestar(temp->getUpgradestar());
                    equipCard->setSsid(rewardItemList.equipmentcarditem(i).ssid());
                    equipCard->setItemId(rewardItemList.equipmentcarditem(i).itemid());
                    //装备原型ID
                    equipCard->setProtoId(temp->getEquipProtoId());
                    //            equipCard->setSetNum(cardInfo.setnum());
                    equipCard->setCurrLevel(rewardItemList.equipmentcarditem(i).currlevel());
                    //equipCard->setCurrExp(rewardItemList.equipmentcarditem(i).currexp());
                    equipCard->setOfficerCardId(rewardItemList.equipmentcarditem(i).officercarditemid());
                    equipCard->setIsProt(rewardItemList.equipmentcarditem(i).isprotection());
                    if (rewardItemList.equipmentcarditem(i).officercarditemid() >0)
                    {
                        SGOfficerCard *officer = playerInfo->getOfficerCard(rewardItemList.equipmentcarditem(i).officercarditemid());
                        officer->addEquip(equipCard);
                        
                    }
                    
                    equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                    equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                    equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                    equipCard->setMaxExp(expdata->getExpValue());
                    equipCard->setRound(temp->getEquipRound());
                    equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                    equipCard->setExpRate(temp->getEquipExpRate());
                    equipCard->setCurrStar(temp->getEquipCurrStarLevel());
                    equipCard->setGoverning(temp->getEquipGoverning());
                    equipCard->setItemType(temp->getEquipType());
                    equipCard->setMaxStar(temp->getEquipNextId());
                    equipCard->setOfficerName(temp->getEquipName());
                    equipCard->setMaxLevel(temp->getEquipMaxLevel());
					
					equipCard->setHeadSuffixNum(temp->getIconId());//12.06
					
                    if (temp->getEquipNextId()) {
                        equipCard->setCost(temp->getEquipCost());
                        
                        //                        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
                        //                        int len = arr->count();
                        //                        for (int j=0; j < len; j++) {
                        //                            equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
                        //                        }
                    }
                    //playerInfo->addEquipCard(equipCard);
                    equipCard->autorelease();
                }
            }
            
            int length3 = rewardItemList.propscarditem_size();
            
            for (int i=0; i<length3; i++) {
                if(rewardItemList.propscarditem(i).itemid()>0)
                {
                    
                    
                    //领奖弹出框
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(9);
                    datamodel->setCount(1);
                    datamodel->setItemId(rewardItemList.propscarditem(i).itemid());
                    data->addObject(datamodel);
                    
                    //            main::PropsCardItemProto cardInfo = response->propscard();
                    SGPropsCard *propCard = new SGPropsCard();
                    SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(rewardItemList.propscarditem(i).itemid());
                    
                    propCard->setSsid(rewardItemList.propscarditem(i).ssid());
                    propCard->setItemId(rewardItemList.propscarditem(i).itemid());
                    propCard->setIsProt(rewardItemList.propscarditem(i).isprotection());
                    
                    propCard->setSell(prop->getpropsSell());
                    propCard->setValue(prop->getpropsValue());
                    propCard->setRace(prop->getPropsCountry());
                    propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
                    propCard->setType(prop->getPropsPropsType());
                    propCard->setItemType(prop->getPropsType());
                    propCard->setCurrStar(prop->getPropsCurrStarLevel());
                    propCard->setHeadSuffixNum(prop->getIconId());
                    //playerInfo->addPropCard(propCard);
                    
                    propCard->autorelease();
                }
                
            }
            
            //CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(8);
            //str->setString(CCString::createWithFormat("%d",2)->getCString());
                        SGMainManager::shareMain()->updataUserMsg();
            registerButton->setImage("loginbtnbg_hui.png");
            //            SG_SHOW_WINDOW("奖励领取成功！");
            
            
            
            //领奖弹出框
            SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
            SGMainManager::shareMain()->showBox(rewardBox);
            
            
            
            m_nStateEmail=2;
            if(m_nStateAccount==2&&m_nStateEmail==2)
            {
                CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
                int nsum = array->count();
                for (int i = 0 ;i < nsum ;i++) {
                    SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                    if(item->gettypeItem()==Reward_Bind)
                    {
                        item->setstateItem(0);
                        break;
                    }
                }
            }
            {
                m_pbuttonGetReward2 ->setFont(str_RewardBindLayer_str101);
                m_pbuttonGetReward2->setImage("store_huik.png");
                m_pbuttonGetReward2->setEnabled(false);
            }
        }
        else
        {
            SG_SHOW_WINDOW(str_Get_fail);
            
        }
    }

}
void SGRewardBindLayer::initReward(CCArray * arr)
{
    int type = 0;
    int value = 0;
    int number = 0;
    for (int i=0; i<arr->count(); i++)
    {
        sscanf(((CCString*)arr->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
        
        CCSprite *cabg1 = CCSprite::createWithSpriteFrameName("card_bj.png");
        this->addChild(cabg1);
        if(type == 0)
        {
            
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
            CCSprite *icon1=CCSprite::createWithSpriteFrameName("big_yb.png");
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_yuanbao,number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
        }
        else if (type ==1)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_bindLayer);
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
            CCSprite *icon1=CCSprite::createWithSpriteFrameName("head9999.png");
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_tongMoney,number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
            
        }
        else if (type ==2)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_bindLayer);
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
            CCSprite *icon1=CCSprite::createWithSpriteFrameName("head9999.png");
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_pvpScore,number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
            
        }
        else if (type == 3)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9997,sg_bindLayer);
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("jinkuang.png");
            CCSprite *icon1=CCSprite::createWithSpriteFrameName("head9997.png");
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_jungong,number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
            
        }
        
        else if (type == 4)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9998,sg_bindLayer);
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("jinkuang.png");
            CCSprite *icon1=CCSprite::createWithSpriteFrameName("head9998.png");
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_smalll_laba,number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
            
        }
        else if (type == 5)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9998,sg_bindLayer);
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("jinkuang.png");
            CCSprite *icon1=CCSprite::createWithSpriteFrameName("head9996.png");
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_liandian,number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
            
        }
        
        else if (type == 6)
        {
            //                SGMainManager::shareMain()->addHeadIconPlistByNum(value[0],sg_bindLayer);
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(value);
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),sg_bindLayer);//12.06
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1) {
                starlvl-=1;
            }
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
            
            //                CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString());
            
            //                CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",value[0])->getCString());
            
            CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());//12.06
            
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setPosition(ccpAdd(icon1->getPosition(),ccp(icon1->getContentSize().width*.3,icon1->getContentSize().height*.3)));
                this->addChild(advancenumber,10);
                
            }
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
            
        }
        else if (type == 7)
        {
            //                SGMainManager::shareMain()->addHeadIconPlistByNum(value[0],sg_bindLayer);
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
            SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),sg_bindLayer);//12.06
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
            
            //                CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",value[0])->getCString());
            
            CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());//12.06
            
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setPosition(ccpAdd(icon1->getPosition(),ccp(icon1->getContentSize().width*.3,icon1->getContentSize().height*.3)));
                this->addChild(advancenumber,10);
                
            }
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
        }
        else if (type == 9)
        {
            //               SGMainManager::shareMain()->addHeadIconPlistByNum(value[0],sg_bindLayer);
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(value);
            SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),sg_bindLayer);//12.06
            
            CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName("jinkuang.png");
            
            //                CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",value[0])->getCString());
            CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
            
            
            SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            
            icon1->setPosition(ccpAdd(center,ccp(-100,40)));
            jinkuang1->setPosition(icon1->getPosition());
            lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
            cabg1->setPosition(icon1->getPosition());
            
            this->addChild(lab1);
            this->addChild(jinkuang1);
            //            this->addChild(cabg1);
            this->addChild(icon1);
        }
        else if (type == 10) //碎片
        {
            SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(value);
            
            int starLv = pieces->getStarLevel();
            if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
            {
                starLv -= 1;
            }
            
            int piecesType=pieces->getPiecesType();
            if (piecesType==0) {
                SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
                if (officerData)
                {
                    
                    SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_bindLayer);

                    CCSprite *jinkuang1 = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_%d.png",starLv, 0)->getCString());
                    
                    //                CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",value[0])->getCString());
                    CCSprite *icon1=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString());
                    
                    
                    SGCCLabelTTF * lab1 = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",pieces->getPiecesName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                    
                    icon1->setPosition(ccpAdd(center,ccp(-100,40)));
                    jinkuang1->setPosition(icon1->getPosition());
                    lab1->setPosition(ccpAdd(icon1->getPosition(),ccp(0,-70)));
                    cabg1->setPosition(icon1->getPosition());
                    
                    this->addChild(lab1);
                    this->addChild(jinkuang1);
        //            this->addChild(cabg1);
                    this->addChild(icon1);
                }
                
            }
        }
       
    }
    
    
 
    
    
    
}



void SGRewardBindLayer::registerHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (m_nStateAccount==0) {
        SGMainManager::shareMain()->showregisterLayer(4,false);
        
    }
    else if (m_nStateAccount==1)
    {
        main::RewardBindAccountRequest *request = new main::RewardBindAccountRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_REWARD_BIND, request);

    }
    else if (m_nStateAccount==2)
    {
        SG_SHOW_WINDOW(str_RewardBindLayer_str3);
    }
}

const char * SGRewardBindLayer::setTheMail(const char* email)
{
    const char*  str = email;
    char  * strtemp = (char*)malloc(strlen(str)+1);
    strcpy(strtemp,str);
    int ncount = 0;
    for(int i = 0 ;i < strlen(str); i++)
    {
        if(strtemp[i]=='@')
        {
            ncount = i;
            break;
        }
        
    }
    if(ncount>3)
    {
         strtemp[ncount-3] = '*';
         strtemp[ncount-2] = '*';
         strtemp[ncount-1] = '*';
    }
    //const char * strtt = &strtemp;
    return strtemp;
    
}
bool SGRewardBindLayer::is_email_valid(const char* email)
{
    if (email == NULL)
    {
        return false;
    }
    //    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    //    return std::regex_match(email, pattern);
    const char *eset = "[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}";
    regmatch_t   subs[10];
    regex_t stuRT;
    
    regcomp(&stuRT, eset, REG_EXTENDED);
    int err = regexec(&stuRT, email, (size_t)10, subs, 0);
    
    if (err) {
        
        regfree   (&stuRT);
        return false;
    }
    else
    {
        regfree   (&stuRT);
        return true;
    }
}

void SGRewardBindLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}


void SGRewardBindLayer::regListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_bindLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;

    if (sr)
    {
        //弹出框显示奖励
        CCArray *data=CCArray::create();
        data->retain();

        
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
        main::RewardBindAccountResponse *response = (main::RewardBindAccountResponse *)sr->m_msg;
        int state = response->state();
        if(state==1)
        {
            
            main::RewardItemListProto rewardItemList = (main::RewardItemListProto)response->itemlist();
            int gold = rewardItemList.gold();
           // playerInfo->setPlayerGold(gold+playerInfo->getPlayerGold());
            int coin = rewardItemList.coin();
            //playerInfo->setPlayerCoins(coin+playerInfo->getPlayerCoins());
            
            int pvp = rewardItemList.pvpscore();
            //playerInfo->setplayerPvpNum(pvp+playerInfo->getplayerPvpNum());
            
//            int jg = rewardItemList.award();
//            playerInfo->setplayerAward(jg+playerInfo->getplayerAward());
//            
//            int sp = rewardItemList.laba();
//            playerInfo->setSpeakerCount(playerInfo->getSpeakerCount()+sp);
            
            int ld = rewardItemList.liandan();
           // playerInfo->setPlayerliandan(ld+playerInfo->getPlayerliandan());
            
            
            //领取奖励数组初始化
            {
                 //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭
                if (gold>0) {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(0);
                    datamodel->setCount(gold);
                    data->addObject(datamodel);
                }
                if (coin>0) {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(1);
                    datamodel->setCount(coin);
                    data->addObject(datamodel);
                }
                if (pvp>0) {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(14);
                    datamodel->setCount(pvp);
                    data->addObject(datamodel);
                }
//                if (jg>0) {
//                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
//                    datamodel->setType(3);
//                    datamodel->setCount(jg);
//                    data->addObject(datamodel);
//                }
//                if (sp>0) {
//                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
//                    datamodel->setType(4);
//                    datamodel->setCount(sp);
//                    data->addObject(datamodel);
//                }
                if (ld>0) {
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(5);
                    datamodel->setCount(ld);
                    data->addObject(datamodel);
                }
               
            }
            
            int length1=rewardItemList.officercarditem_size();
            for (int i=0;i<length1;i++) {
                
                
                if(rewardItemList.officercarditem(i).itemid()>0)
                {
                    //领奖弹出框
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(6);
                    datamodel->setCount(1);
                    datamodel->setItemId(rewardItemList.officercarditem(i).itemid());
                    data->addObject(datamodel);
//
                    SGOfficerCard *officerCard = new SGOfficerCard();
                    CCDictionary *dicc = SGCardAttribute::getValue(rewardItemList.officercarditem(i).currlevel(), rewardItemList.officercarditem(i).itemid());
                    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(rewardItemList.officercarditem(i).itemid());
                    int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(rewardItemList.officercarditem(i).itemid());
//                    if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                        type = 0;
//                    }
//                    else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                        type = 1;
//                    }
//                    else {
//                        type = 2;
//                    }
                    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, rewardItemList.officercarditem(i).currlevel()+1);
                    
                    officerCard->setSsid(rewardItemList.officercarditem(i).ssid());
                    officerCard->setItemId(rewardItemList.officercarditem(i).itemid());
                    officerCard->setCurrExp(rewardItemList.officercarditem(i).currexp());
                    officerCard->setLordId(rewardItemList.officercarditem(i).lordskillid());
                    officerCard->setIsProt(rewardItemList.officercarditem(i).isprotection());
                    //                officerCard->setIsExped(proto.isexpedition());
                    officerCard->setCurrLevel(rewardItemList.officercarditem(i).currlevel());
                    
                    if (temp->getOfficerNextId()) {
                        officerCard->setCost(temp->getOfficerCost());
                        
//                        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                        int len = arr->count();
//                        for (int j=0; j < len; j++) {
//                            officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                        }
                    }
                    officerCard->setExpRate(temp->getOfficerExpRate());
                    officerCard->setMaxStar(temp->getOfficerNextId());
                    officerCard->setMaxExp(expdata->getExpValue());
                    officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
                    officerCard->setOfficerName(temp->getOfficerName());
                    officerCard->setMaxLevel(temp->getOfficerMaxLevel());
                    officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                    officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                    officerCard->setRound(temp->getOfficerRound());
                    officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                    officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                    officerCard->setGoverning(temp->getOfficerGoverning());
                    officerCard->setRace(temp->getOfficerRace());
                    officerCard->setItemType(temp->getOfficerType());
                    //转生最大星级
                    officerCard->setUpgradestar(temp->getUpgradestar());
                    //转生等级
                    officerCard->setUpgradelevel(temp->getUpgradelevel());
                    //武将计最大等级
                    officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
                    //主将计最大等级
                    officerCard->setLordMaxLevel(temp->getLordMaxLevel());
                    //性别和缘分
                    officerCard->setGender((OfficerGender) temp->getOfficerGender());
                    officerCard->setFateList(temp->getOfficerFateList());
                    //转生次数
                    officerCard->setAdNum(temp->getAdvNum());
                    //武将原型ID
                    officerCard->setProtoId(temp->getOfficerProtoId());
                    //速度修正系数
                    officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
					
					officerCard->setHeadSuffixNum(temp->getIconId());//12.05
                    
                    if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
                    {
                        std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                        officerCard->setOfficerSkil(atoi(generalskill.c_str()));
                    }
                    if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
                    {
                        std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                        officerCard->setLordSkill(atoi(lordskill.c_str()));            
                    }
                    //playerInfo->addOfficerCard(officerCard);
                    
                    officerCard->autorelease();
                    
                }
            }
            
            
            int length2 = rewardItemList.equipmentcarditem_size();
            for (int i=0; i<length2; i++)
{
                
                
                if(rewardItemList.equipmentcarditem(i).itemid()>0){
                    
                    //领奖弹出框
                    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
                    datamodel->setType(7);
                    datamodel->setCount(1);
                    datamodel->setItemId(rewardItemList.equipmentcarditem(i).itemid());
                    data->addObject(datamodel);
                    
                    
//                    int curr = 0;
//                    main::EquipmentCardItemProto cardInfo = response->equipmentcard();
                    SGEquipCard *equipCard = new SGEquipCard();
                    CCDictionary *dicc = SGCardAttribute::getValue(rewardItemList.equipmentcarditem(i).currlevel(), rewardItemList.equipmentcarditem(i).itemid());
                    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(rewardItemList.equipmentcarditem(i).itemid());
                    int type = SGStaticDataManager::shareStatic()->getStrengEquipType(rewardItemList.equipmentcarditem(i).itemid());
//                    if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                        type = 0;
//                    }
//                    else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                        type = 1;
//                    }
//                    else {
//                        type = 2;
//                    }
                    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, rewardItemList.equipmentcarditem(i).currlevel()+1);
                    //转生等级
                    equipCard->setUpgradelevel(temp->getUpgradelevel());
                    //转生次数
                    equipCard->setAdNum(temp->getAdvNum());
                    //转生最大星级
                    equipCard->setUpgradestar(temp->getUpgradestar());
                    equipCard->setSsid(rewardItemList.equipmentcarditem(i).ssid());
                    equipCard->setItemId(rewardItemList.equipmentcarditem(i).itemid());
                    //装备原型ID
                    equipCard->setProtoId(temp->getEquipProtoId());
                    //            equipCard->setSetNum(cardInfo.setnum());
                    equipCard->setCurrLevel(rewardItemList.equipmentcarditem(i).currlevel());
                    //equipCard->setCurrExp(rewardItemList.equipmentcarditem(i).currexp());
                    equipCard->setOfficerCardId(rewardItemList.equipmentcarditem(i).officercarditemid());
                    equipCard->setIsProt(rewardItemList.equipmentcarditem(i).isprotection());
                    if (rewardItemList.equipmentcarditem(i).officercarditemid() >0)
                    {
                        SGOfficerCard *officer = playerInfo->getOfficerCard(rewardItemList.equipmentcarditem(i).officercarditemid());
                        officer->addEquip(equipCard);
                        
                    }
                    
                    equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                    equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                    equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                    equipCard->setMaxExp(expdata->getExpValue());
                    equipCard->setRound(temp->getEquipRound());
                    equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                    equipCard->setExpRate(temp->getEquipExpRate());
                    equipCard->setCurrStar(temp->getEquipCurrStarLevel());
                    equipCard->setGoverning(temp->getEquipGoverning());
                    equipCard->setItemType(temp->getEquipType());
                    equipCard->setMaxStar(temp->getEquipNextId());
                    equipCard->setOfficerName(temp->getEquipName());
                    equipCard->setMaxLevel(temp->getEquipMaxLevel());
					
					equipCard->setHeadSuffixNum(temp->getIconId());//12.06
					
                    if (temp->getEquipNextId()) {
                        equipCard->setCost(temp->getEquipCost());
                        
//                        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                        int len = arr->count();
//                        for (int j=0; j < len; j++) {
//                            equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                        }
                    }
                    //playerInfo->addEquipCard(equipCard);
                    equipCard->autorelease();
            }
}
            
    int length3 = rewardItemList.propscarditem_size();

    for (int i=0; i<length3; i++) {
        if(rewardItemList.propscarditem(i).itemid()>0)
        {
            
            
            //领奖弹出框
            SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
            datamodel->setType(9);
            datamodel->setCount(1);
            datamodel->setItemId(rewardItemList.propscarditem(i).itemid());
            data->addObject(datamodel);
            
//            main::PropsCardItemProto cardInfo = response->propscard();
            SGPropsCard *propCard = new SGPropsCard();
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(rewardItemList.propscarditem(i).itemid());
            
            propCard->setSsid(rewardItemList.propscarditem(i).ssid());
            propCard->setItemId(rewardItemList.propscarditem(i).itemid());
            propCard->setIsProt(rewardItemList.propscarditem(i).isprotection());
            
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
            propCard->setHeadSuffixNum(prop->getIconId());
            //playerInfo->addPropCard(propCard);
                
            propCard->autorelease();
        }

    }
           
            //CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(8);
            //str->setString(CCString::createWithFormat("%d",2)->getCString());
                        SGMainManager::shareMain()->updataUserMsg();
            registerButton->setImage("loginbtnbg_hui.png");
//            SG_SHOW_WINDOW("奖励领取成功！");
            
            m_nStateAccount=2;
            if(m_nStateAccount==2&&m_nStateEmail==2)
            {
                CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
                int nsum = array->count();
                for (int i = 0 ;i < nsum ;i++) {
                    SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                    if(item->gettypeItem()==Reward_Bind)
                    {
                        item->setstateItem(0);
                        break;
                    }
                }
            }

            m_pbuttonGetReward1 ->setFont(str_RewardBindLayer_str101);
            m_pbuttonGetReward1->setImage("store_huik.png");
            m_pbuttonGetReward1->setEnabled(false);
            //领奖弹出框
            SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
            SGMainManager::shareMain()->showBox(rewardBox);
            
            
            
            
            
    }
        else
        {
            SG_SHOW_WINDOW(str_Get_fail);

        }
}
}

void SGRewardBindLayer::setIsCanTouch(bool isTouch)
{
//    btn->setEnabled(isTouch);
    ((SGButton *)this->menu->getChildByTag(88))->setEnabled(isTouch);
    
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(isTouch);
    layer->setPortraitMenuCanTouch(isTouch);
}

//bool SGRewardBindLayer::checkInputStringChar(const char *inputString)
//{
//    //检查位数
//    if(!inputString || sizeof(inputString)<4 || sizeof(inputString) >17){
//        return false;
//    }
//    
//    //检查是否是字母或数字
//    for(int i=0;i<sizeof(inputString);i++)
//    {
//        int number = inputString[i];
//        if(number >= 48 && number <= 57){       //数字
//            continue;
//        }else if(number >= 65 && number <= 90){ //大写字母
//            continue;
//        }else if(number >=97 && number <= 122){ //小写字母
//            continue;
//        }else{
//            return false;
//        }
//    }
//    
//    return true;
//}


void SGRewardBindLayer::requestFinished(int msgid, CCObject *data)
{
    if(msgid == MSG_HTTP_REGISTER)
    {
        CCString *backString = (CCString*)data;
        if(backString->intValue() == 1)
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("username", m_pEditName->getText());
            CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditPassword->getText());
            CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", true);
            CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
            CCUserDefault::sharedUserDefault()->flush();
            
            //            SGRegisterSuccess *su = SGRegisterSuccess::create(this);
            ////            this->boxCloseWithOutCallBack();
            //            SGMainManager::shareMain()->showBox(su);
            SG_SHOW_WINDOW(str_RewardBindLayer_str4);
            SGMainManager::shareMain()->showwelcomelayer();
            
        }else{
            SG_SHOW_WINDOW(str_RewardBindLayer_str5);
            //            labelAccountError->setVisible(true);
            //            labelAccountError->setString("已经有人使用了这个帐号");
            //            this->processErrorState(FLAG_ERROR, FLAG_NODISPLAY, FLAG_NODISPLAY);
        }
    }else if(msgid == MSG_HTTP_BING)
    {
        CCDictionary *dict = (CCDictionary*)data;
        
        CCString *string1 = (CCString*)dict->objectForKey("string1");
        CCString *string2 = (CCString*)dict->objectForKey("string2");
        
        if(1 == string1->intValue())
        {
            //绑定成功
            SG_SHOW_WINDOW(str_RegisterBox_str11);
            
            CCUserDefault::sharedUserDefault()->setStringForKey("username", m_pEditName->getText());
            CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditPassword->getText());
            CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
            CCUserDefault::sharedUserDefault()->flush();
//            
//            SGWelComeLayer *layer = SGWelComeLayer::create();
//            layer->loginstart();
//            SGMainManager::shareMain()->getMainScene()->addShowLayer(layer);
//            layer->showbox();
        }else
        {
            if(string2->intValue() == 1)
            {
                //绑定失败
                SG_SHOW_WINDOW(str_RegisterBox_str12);
            }
            else if(2 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str13);
            }else if(3 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str14);
            }else if(4 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str24);
            }
        }
    }
}

void SGRewardBindLayer::requestFailed()
{
    CCLOG("注册接收数据失败");
    SG_SHOW_WINDOW(str_RegisterBox_str15);
    
}
