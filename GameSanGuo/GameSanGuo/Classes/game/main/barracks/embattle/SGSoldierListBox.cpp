//
//  SGSoldierListBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGSoldierListBox.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCardItem.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "ArmyModule.pb.h"
#include "SGFont.h"
#include "SGSoldierItem.h"

#include "ResourceManager.h"

#include "SGTeamgroup.h"
#include "SGStringConfig.h"

SGSoldierListBox::SGSoldierListBox():
colorId(0),
_delegateLayer(NULL),
tag(0),
emtype(0)
{
    ;
}

SGSoldierListBox::~SGSoldierListBox()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EMBATTLE_DISPATCH_SOLDIER);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_soldierListBox);
//    CC_SAFE_RELEASE(soldiers); 
}
SGSoldierListBox *SGSoldierListBox::create(SGBoxDelegate *dg, int colorId_ , SGEmbattleLayer *delegateLayer,int em)
{
    SGSoldierListBox *soldierList = new SGSoldierListBox();
    soldierList->_delegateLayer = delegateLayer;
   
    if (soldierList && soldierList->init(dg, box_soldierList, CCSizeMake(535, 677),false,true,CCSizeMake(535, 130)))
    {
        soldierList->colorId = colorId_;
        soldierList->emtype=em;
        soldierList->initView();
        soldierList->autorelease();
        return soldierList;
    }
    CC_SAFE_DELETE(soldierList);
    return NULL;
}

void SGSoldierListBox::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_box_soldierListBox, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_box_soldierListBox);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_box_soldierListBox, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_box_soldierListBox);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_box_soldierListBox);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EMBATTLE_DISPATCH_SOLDIER,
                              this,
                              callfuncO_selector(SGSoldierListBox::soldiersDispatchListener));
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCArray *soldierDatas = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();  
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    SGButton *backBtn = SGButton::create("box_btnbg.png", "public_font_quxiao.png", this, menu_selector(SGSoldierListBox::boxCloseWithOutCallBack),CCPointZero,false,true);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(center, ccp(0, -677/2+5 -40)));

    CCSprite *title = CCSprite::createWithSpriteFrameName("font_choosebz.png");
    title->setPosition(biao->getPosition());
    this->addChild(title,10);
    
    int sid = 0;
    switch (SGPlayerInfo::sharePlayerInfo()->getPlayerCountry())
    {
        case 1:
            sid = 3101;
            break;
        case 2:
        {
            sid = 3201;
        }
            break;
        case 3:
        {
            sid = 3301;
        }
            break;
        default:
            break;
    }
    
    CCSprite *jinbian = CCSprite::createWithSpriteFrameName("login_name_line.png");
    this->addChild(jinbian);
    jinbian->setPosition(ccpAdd(center, ccp(0, 295)));
    
    for (int i = 1; i<4; i++)
    {
        CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
        frame2->setPreferredSize(CCSizeMake(486+20, 124));
        this->addChild(frame2,1);
        frame2->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, -frame2->getContentSize().height/2 - 25   -(i-1)*(frame2->getContentSize().height + 18))));
        
        CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
        this->addChild(frame2bg);
        frame2bg->setScaleX(508/frame2bg->getContentSize().width);
        frame2bg->setScaleY(124/frame2bg->getContentSize().height);
        frame2bg->setPosition(frame2->getPosition());
        
        CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
        this->addChild(guang2l);
        guang2l->setAnchorPoint(ccp(0, 1));
        CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
        this->addChild(guang2r);
        guang2r->setFlipX(true);
        guang2r->setAnchorPoint(ccp(1, 1));
        
        CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
        this->addChild(guang2m);
        guang2m->setScaleX(386/guang2m->getContentSize().width);
        guang2m->setAnchorPoint(ccp(0.5, 1));
        
        guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
        guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(243+10, 0)));
        guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-243-10, 0)));
        
        
        SGButton *choose = SGButton::createFromLocal("streng_btn.png", str_select, this, menu_selector(SGSoldierListBox::buttonClick),CCPointZero,FONT_PANGWA,ccWHITE,28,false,true);
        this->addBtn(choose);
        choose->setTag(i);
        choose->setPosition(ccpAdd(frame2->getPosition(), ccp(frame2->getContentSize().width*.35 -10, 0)));
        
        SGSoldierCard *card = (SGSoldierCard *)soldierDatas->objectAtIndex(i-1);
        CCString *str_lvl = CCString::createWithFormat(str_Format_ji,card->getCurrLevel());
        SGCCLabelTTF *lvl = SGCCLabelTTF::create(str_lvl->getCString(), FONT_BOXINFO, 30);
        lvl->setAnchorPoint(ccp(1, 0.5));
        lvl->setPosition(ccpAdd(frame2->getPosition(), ccp(90, 0)));
        this->addChild(lvl);
		//使用白色边equipPinzhi_1.png
        SGButton *item = SGButton::create(CCString::createWithFormat("%d-%d.png",sid+i-1,colorId)->getCString(),CCString::createWithFormat("equipPinzhi_%d_0.png",1)->getCString(), this, menu_selector(SGSoldierListBox::showSoldierInfo),CCPointZero,false,true);
        this->addBtn(item);
        item->setTag(i-1);
        item->setPosition(ccpAdd(frame2->getPosition(), ccp(-frame2->getContentSize().width*.3 - 20, 0)));
        CCSprite *bj = CCSprite::createWithSpriteFrameName("card_bj.png");
        this->addChild(bj);
        bj->setPosition(item->getPosition());

        SGCCLabelTTF *name = SGCCLabelTTF::create(card->getOfficerName()->getCString(), FONT_BOXINFO, 30);
        name->setAnchorPoint(ccp(0, 0.5));
        this->addChild(name);
        name->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*1.5 - 60, 0)));
    }
        
    SGCCLabelTTF* font1 = SGCCLabelTTF::create(str_SoiderListBox_str1,FONT_BOXINFO, 26);
    font1->setPosition(ccpAdd(backBtn->getPosition(), ccp(-15,160)));
    this->addChild(font1);
    
    SGCCLabelTTF* font2 = SGCCLabelTTF::create(str_SoiderListBox_str2, FONT_BOXINFO, 26 , COLOR_GRAY);
    font2->setAnchorPoint(ccp(0, 0.5));
    //modify by:zyc. merge into create.
    //font2->setColor(COLOR_GRAY);
    font2->setPosition(ccpAdd(font1->getPosition(), ccp(-font1->getContentSize().width*.5, -40)));
    this->addChild(font2);

    SGCCLabelTTF *fontcolor = NULL;// = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    this->addChild(fontcolor,1);
    fontcolor->setPosition(font1->getPosition());
    //modify by:zyc. merge into create.
    switch (colorId) {
        case 1:
            SGCCLabelTTF::create(str_SoiderListBox_str3, FONT_BOXINFO, 26 , COLOR_GREEN);
//            fontcolor->setString(str_SoiderListBox_str3);
//            fontcolor->setColor(COLOR_GREEN);
            break;
        case 2:
            SGCCLabelTTF::create(str_SoiderListBox_str4, FONT_BOXINFO, 26 , COLOR_BLUE);
//            fontcolor->setString(str_SoiderListBox_str4);
//            fontcolor->setColor(COLOR_BLUE);
            break;
        case 3:
            SGCCLabelTTF::create(str_SoiderListBox_str5, FONT_BOXINFO, 26 , COLOR_RED);
//            fontcolor->setString(str_SoiderListBox_str5);
//            fontcolor->setColor(COLOR_RED);
            break;
        default:
            break;
    }
        
}

void SGSoldierListBox::showSoldierInfo(CCNode *node)
{
    EFFECT_PLAY(MUSIC_BTN);
    SGSoldierCard *card = (SGSoldierCard*)SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->objectAtIndex(node->getTag());
    SGSoldierListBox::boxClose();
    SGMainManager::shareMain()->showSoldierInfoLayer(card,1);
}
void SGSoldierListBox::buttonClick(cocos2d::CCNode *node)
{
    tag = node->getTag();
    main::DispatchSoldierRequest *request = new main::DispatchSoldierRequest();
    if (colorId == 3) {
        request->set_color(0);
    }else if(colorId == 1)
    {
        request->set_color(1);
    }else if(colorId == 2)
    {
        request->set_color(2);
    }
    
    CCArray *soldierDatas = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
        SGSoldierCard *card = (SGSoldierCard *)soldierDatas->objectAtIndex(tag-1);
        request->set_ssid(card->getSsid());
    request->set_teamid(emtype-1);
    SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_DISPATCH_SOLDIER, request);

}
void SGSoldierListBox::callBackForSoldier(int index)
{
    CCString *nameStr = CCString::createWithFormat("3%d0%d-%d.png",SGPlayerInfo::sharePlayerInfo()->getPlayerCountry(),index,colorId);
    CCSprite *sprite = CCSprite::createWithSpriteFrameName(nameStr->getCString());
    _delegateLayer->callBackToSetSoldierSkin(sprite);
    this->boxCloseWithOutCallBack();
}
void SGSoldierListBox::soldiersDispatchListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::DispatchSoldierResponse *response = (main::DispatchSoldierResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            CCString *nameStr = CCString::createWithFormat("3%d0%d-%d.png",SGPlayerInfo::sharePlayerInfo()->getPlayerCountry(),tag,colorId);
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(nameStr->getCString());
            _delegateLayer->callBackToSetSoldierSkin(sprite);
//            int index = SGTeamgroup::shareTeamgroupInfo()->getteamindex(emtype);
//            SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//            if (colorId == 3) {
////                SGTeamgroup::shareTeamgroupInfo()->setsoldierred(index, CCString::createWithFormat("3%d0%d",player->getPlayerCountry(),tag)->intValue());
////                player->setRedSoldierId(CCString::createWithFormat("3%d0%d",player->getPlayerCountry(),tag)->intValue());
//            }else if(colorId == 1)
//            {
////                SGTeamgroup::shareTeamgroupInfo()->setsoldiergreen(index, CCString::createWithFormat("3%d0%d",player->getPlayerCountry(),tag)->intValue());
////                player->setGreenSoldierId(CCString::createWithFormat("3%d0%d",player->getPlayerCountry(),tag)->intValue());
//            }else if(colorId == 2)
//            {
////                SGTeamgroup::shareTeamgroupInfo()->setsoldierblue(index, CCString::createWithFormat("3%d0%d",player->getPlayerCountry(),tag)->intValue());
////                player->setBlueSoldierId(CCString::createWithFormat("3%d0%d",player->getPlayerCountry(),tag)->intValue());
//            }
            
            this->boxCloseWithOutCallBack();

            SGMainManager::shareMain()->updateEmbattleInfo();
//            SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
            CCLOG("成功");
        }
        else{
            SGMainManager::shareMain()->showMessage(str_SoiderListBox_str6);
            this->boxCloseWithOutCallBack();
        }
    }
}

