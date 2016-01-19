//
//  SGInviteeLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#include "SGInviteeLayer.h"
#include "SGMainManager.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "Invite.pb.h"
#include "SGCantAdvanceBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGRewardLayer.h"
#define   REWARDBTTAG 100
SGInviteeLayer::SGInviteeLayer():m_pTextField(NULL)
,btn(NULL)
{

}
SGInviteeLayer::~SGInviteeLayer()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REWARD_INTIVEEGOLD);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_inviteeLayer);
 
}
SGInviteeLayer *SGInviteeLayer::create()
{
    SGInviteeLayer *Layer = new SGInviteeLayer();
    if (Layer && Layer->init(NULL , sg_drinkWineLayer))
    {
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGInviteeLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_inviteeLayer);

    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_REWARD_INTIVEEGOLD,this,callfuncO_selector(SGInviteeLayer::inviteeGoldListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/reward_winebg.plist", RES_TYPE_LAYER_UI, sg_inviteeLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("reward_winebg.png");
    bg->setScaleY((s.height - headhgt - btmhgt)/702);
    bg->setScaleX(s.width/768);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));
    
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGInviteeLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGInviteeLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
      
//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_drink.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_InviteeLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
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
    
    
    
    CCSprite *fontbg = CCSprite::createWithSpriteFrameName("reward_hk.png");
    this->addChild(fontbg);
    fontbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft),ccp(fontbg->getContentSize().width/4,-60)));
    
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(str_InviteeLayer_str2, FONT_BOXINFO, 26);
    this->addChild(label1);
    label1->setPosition(fontbg->getPosition());
    
//    SGCCLabelTTF *label_time1 = SGCCLabelTTF::create("1.奖励只能在15级之前可以领取", FONT_BOXINFO, 26);
//      SGCCLabelTTF *label_time2 = SGCCLabelTTF::create("2.只能领取一次", FONT_BOXINFO, 26);
    SGCCLabelTTF *label_time1 = SGCCLabelTTF::create(str_InviteeLayer_str3, FONT_BOXINFO, 26);
    //"2.填写邀请码可获得“铜钱10W”,“元宝100”奖励"
    const char * strTongqian= str_InviteeLayer_str401;
    const char * strYuanbao = str_InviteeLayer_str402;
    const char * strstr =str_InviteeLayer_str403;
    SGCCLabelTTF *label_time2 = SGCCLabelTTF::create(strstr, FONT_BOXINFO, 26);
    SGCCLabelTTF *label_time21 = SGCCLabelTTF::create(strTongqian, FONT_BOXINFO, 26);

    SGCCLabelTTF *label_time22 = SGCCLabelTTF::create(strYuanbao, FONT_BOXINFO, 26);

    //CCSprite *yb = CCSprite::createWithSpriteFrameName("store_yb.png");
    //yb->setPosition(ccpAdd(font_lvl->getPosition(), ccp(100, 0)));
    //this->addChild(yb,0,14);

    CCSprite * tongqian = CCSprite::createWithSpriteFrameName("coinicon.png");
    CCSprite * yuanbao = CCSprite::createWithSpriteFrameName("store_yb.png");
    SGCCLabelTTF *label_time3 = SGCCLabelTTF::create(str_InviteeLayer_str5, FONT_BOXINFO, 26);
    SGCCLabelTTF *label_time4 = SGCCLabelTTF::create(str_InviteeLayer_str6, FONT_BOXINFO, 26 , ccRED);
    label_time1->setAnchorPoint(CCPointZero);
    label_time1->ignoreAnchorPointForPosition(false);
    label_time2->setAnchorPoint(CCPointZero);
    label_time2->ignoreAnchorPointForPosition(false);
    label_time21->setAnchorPoint(CCPointZero);
    label_time21->ignoreAnchorPointForPosition(false);
    label_time22->setAnchorPoint(CCPointZero);
    label_time22->ignoreAnchorPointForPosition(false);

    //tongqian->setAnchorPoint(ccp(0.5,1));
    //tongqian->ignoreAnchorPointForPosition(false);
    //yuanbao->setAnchorPoint(CCPointZero);
    //yuanbao->ignoreAnchorPointForPosition(false);

    label_time3->setAnchorPoint(CCPointZero);
    label_time3->ignoreAnchorPointForPosition(false);
    label_time4->setAnchorPoint(CCPointZero);
    label_time4->ignoreAnchorPointForPosition(false);
    //modify by:zyc. merge into create.
    //label_time4->setColor(ccRED);
    this->addChild(label_time1);
    this->addChild(label_time2);
    this->addChild(label_time21);
    this->addChild(label_time22);
    this->addChild(label_time3);
    this->addChild(label_time4);
    this->addChild(tongqian);
    this->addChild(yuanbao);
    label_time1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(40+label_time1->getContentSize().width*.5, -140)));
    label_time2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(40+label_time2->getContentSize().width*.5, -180)));
    tongqian->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(70+label_time2->getContentSize().width, -180)));
    label_time21->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(100+label_time2->getContentSize().width, -180)));
    
    yuanbao->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(305+label_time2->getContentSize().width*.5, -180)));
    label_time22->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(360+label_time2->getContentSize().width*.5, -180)));
    label_time3->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(40+label_time3->getContentSize().width*.5, -220)));
    label_time4->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(40+label_time4->getContentSize().width*.5, -260)));
     
    btn = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGInviteeLayer::confirmHandler),CCPointZero,false,true);
    btn->setPosition(ccpAdd(SGLayout::getPoint(KMiddleRight), ccp(-btn->getContentSize().width, btn->getContentSize().height*2)));
    this->addBtn(btn);

    m_pTextField = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pTextField->setPlaceHolder(str_InviteeLayer_str7);
    m_pTextField->setMaxLength(30);
    m_pTextField->setPosition(ccp(backBtn->getPosition().x + 160, btn->getPosition().y));
    m_pTextField->setReturnType(kKeyboardReturnTypeDone);
    m_pTextField->setInputMode(kEditBoxInputModeSingleLine);
    m_pTextField->setReturnType(kKeyboardReturnTypeDone);
    m_pTextField->setDelegate(this);
    addChild(m_pTextField,50);

}

void SGInviteeLayer::inviteeGoldListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::InviteResponse *response = (main::InviteResponse *)sr->m_msg;
        
         //1:成功     2被邀请人超过15级       3：填写自己的邀请码          4：无效邀请人              5：邀请码只能填写一次
        if (response->state() == 1)
        {
            CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat(str_InviteeLayer_str8,m_pTextField->getText())->getCString() , FONT_BOXINFO, FONTSIZE(18));
          //  label->setPosition(m_pTextField->getPosition());
            CCPoint center = SGLayout::getPoint(kMiddleCenter);
//            CCPoint center=
            label->setPosition(ccp(center.x, m_pTextField->getPosition().y));
            label->setColor(ccYELLOW);
            
            
            addChild(label);
            m_pTextField->setVisible(false);
            m_pTextField->setText(" ");
            btn->setVisible(false);

            int rewardCoin = response->coins();
            int rewardGold = /*response->*/response->gold();
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 41,rewardCoin,0,NULL,rewardGold);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
            int nsum = array->count();
            for (int i = 0 ;i < nsum ;i++) {
                SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                if(item->gettypeItem()==Reward_Invitee)
                {
                    item->setstateItem(2);
                    break;
                }
            }
        }
        else if (response->state() == 2){
            SGMainManager::shareMain()->showMessage(str_InviteeLayer_str9);
            
        }else if (response->state() == 3){
            SGMainManager::shareMain()->showMessage(str_InviteeLayer_str10);
            
        }else if (response->state() == 4){
            SGMainManager::shareMain()->showMessage(str_InviteeLayer_str11);
            
        }else if (response->state() == 5){
            SGMainManager::shareMain()->showMessage(str_InviteeLayer_str12);

        }
        else{
            SGMainManager::shareMain()->showMessage(str_InviteeLayer_str13);
        }
    }
}
void SGInviteeLayer::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    if ( m_pTextField->getText() == NULL || strlen(m_pTextField->getText()) == 0) {
         SGMainManager::shareMain()->showMessage(str_InviteeLayer_str14);
        return;
    }
    main::InviteRequest *request = new main::InviteRequest();
    request->set_invitecode(m_pTextField->getText());
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVEEGOLD, request);
}
void SGInviteeLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer( (btn != NULL) && (!btn->isVisible()));
}


#pragma mark editBox delegate

void SGInviteeLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void SGInviteeLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void SGInviteeLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void SGInviteeLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    
}
