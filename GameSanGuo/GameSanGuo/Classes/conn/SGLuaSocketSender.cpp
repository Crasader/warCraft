//
//  SGLuaSocketSender.cpp
//  GameSanGuo
//
//  Created by zenghui on 14-1-8.
//
//

#include "SGLuaSocketSender.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "TestModule.pb.h"
#include "SGMainManager.h"
#include "SGActGiftInfoBox.h"
#include "ShopModule.pb.h"
#include "SGLuaDataProto.h"
#include "LuaModule.pb.h"
#include "LuaActivityModule.pb.h"
#include "ArenaModule.pb.h"
#include "SpecialBattleModule.pb.h"
#include "FightBaseModule.pb.h"
#include "SGActivityLayer.h"
#include "SGMallLayer.h"

#define  SHOW_GM     "showGMLayer"
#define  SHOW_PVP    "showPvpLayer"
#define  SHOW_LANDGUFT    "showLandingGuiftLayer"
#define  SHOW_SOTRE       "showStoreLayer"//充值
#define  SHOW_VIP_INFO  "showVipInfo"//vip特权

#define GOTO_CHUANG_GUAN "_goto_chuang_guan" //去闯关

#define GOTO_SHOP_0 "_goto_pvp_merchant"//天梯商人
#define GOTO_SHOP_1 "_goto_jingZhou_merchant"//荆州商人
#define GOTO_SHOP_2 "_goto_travel_merchant"//云游商人
#define GOTO_SHOP_3 "_goto_luoYang_merchant"//洛阳鬼市
#define GOTO_SHOP_4 "_goto_fairyland_merchant"//幻境商人
#define GOTO_SHOP_5 "_goto_arena_merchant"//乱世商人

#define GOTO_EXOTIC_MERCHANT "_goto_exotic_merchant" //西域商人

#define GOTO_INSTANCE "_goto_instance" //副本
#define GOTO_PVP_HOME "_goto_pvp_home" //对战首页
#define GOTO_ARENA_HOME "_goto_arena_home"//竞技场首页
#define GOTO_SPECIAL_BATTLE "_goto_special_battle"//幻境
#define GOTO_BOSS_BATTLE "_goto_boss_battle"//讨魔
#define GOTO_VISIT "_goto_visit"//探访

SGLuaSocketSender* SGLuaSocketSender::s_shareLuaSender = NULL;

SGLuaSocketSender* SGLuaSocketSender::shareLuaSender(void)
{
    if (s_shareLuaSender == NULL) {
        s_shareLuaSender = new SGLuaSocketSender();
        s_shareLuaSender->init();
    }
    return s_shareLuaSender;
}

SGLuaSocketSender::SGLuaSocketSender()
{
}

SGLuaSocketSender::~SGLuaSocketSender()
{
    SGNotificationCenter * nf = SGNotificationCenter::sharedNotificationCenter();
    nf->removeObserver(this, SHOW_GM);
    nf->removeObserver(this, SHOW_PVP);
    nf->removeObserver(this, SHOW_LANDGUFT);
    nf->removeObserver(this, MSG_ACTIVITY_LUA);
    nf->removeObserver(this, SHOW_SOTRE);
    nf->removeObserver(this, SHOW_VIP_INFO);
    
    nf->removeObserver(this, GOTO_CHUANG_GUAN);
    nf->removeObserver(this, GOTO_SHOP_0);
    nf->removeObserver(this, GOTO_SHOP_1);
    nf->removeObserver(this, GOTO_SHOP_2);
    nf->removeObserver(this, GOTO_SHOP_3);
    nf->removeObserver(this, GOTO_SHOP_4);
    nf->removeObserver(this, GOTO_SHOP_5);
    
    nf->removeObserver(this , GOTO_EXOTIC_MERCHANT);
    
    nf->removeObserver(this, GOTO_INSTANCE);
    nf->removeObserver(this, GOTO_PVP_HOME);
    nf->removeObserver(this, GOTO_ARENA_HOME);
    nf->removeObserver(this, GOTO_SPECIAL_BATTLE);
    nf->removeObserver(this, GOTO_BOSS_BATTLE);
    nf->removeObserver(this, GOTO_VISIT);
}

void SGLuaSocketSender::init()
{
    SGNotificationCenter * nf = SGNotificationCenter::sharedNotificationCenter();
    nf->addObserver(SHOW_GM,this,callfuncO_selector(SGLuaSocketSender::showGMLayer));
    nf->addObserver(SHOW_PVP,this,callfuncO_selector(SGLuaSocketSender::showPvpLayer));
    nf->addObserver(SHOW_LANDGUFT,this,callfuncO_selector(SGLuaSocketSender::showLandingGuiftLayer));
    nf->addObserver(MSG_ACTIVITY_LUA,this,callfuncO_selector(SGLuaSocketSender::dealLuaMsg));
    nf->addObserver(SHOW_SOTRE,this,callfuncO_selector(SGLuaSocketSender::showStoreLayer));
    nf->addObserver(SHOW_VIP_INFO,this,callfuncO_selector(SGLuaSocketSender::showVipInfoLayer));
    
    nf->addObserver(GOTO_CHUANG_GUAN ,this,callfuncO_selector(SGLuaSocketSender::gotoChuangGuan));
    
    nf->addObserver(GOTO_SHOP_0,this,callfuncO_selector(SGLuaSocketSender::gotoShop0));
    nf->addObserver(GOTO_SHOP_1,this,callfuncO_selector(SGLuaSocketSender::gotoShop1));
    nf->addObserver(GOTO_SHOP_2,this,callfuncO_selector(SGLuaSocketSender::gotoShop2));
    nf->addObserver(GOTO_SHOP_3,this,callfuncO_selector(SGLuaSocketSender::gotoShop3));
    nf->addObserver(GOTO_SHOP_4,this,callfuncO_selector(SGLuaSocketSender::gotoShop4));
    nf->addObserver(GOTO_SHOP_5,this,callfuncO_selector(SGLuaSocketSender::gotoShop5));
    
    nf->addObserver(GOTO_EXOTIC_MERCHANT,this,callfuncO_selector(SGLuaSocketSender::gotoExoticMerchant));
    
    nf->addObserver(GOTO_INSTANCE,this,callfuncO_selector(SGLuaSocketSender::gotoInstance));
    nf->addObserver(GOTO_PVP_HOME,this,callfuncO_selector(SGLuaSocketSender::gotoPvpHome));
    nf->addObserver(GOTO_ARENA_HOME,this,callfuncO_selector(SGLuaSocketSender::gotoArenaHome));
    nf->addObserver(GOTO_SPECIAL_BATTLE,this,callfuncO_selector(SGLuaSocketSender::gotoSpecialBattle));
    nf->addObserver(GOTO_BOSS_BATTLE,this,callfuncO_selector(SGLuaSocketSender::gotoBossBattle));
    nf->addObserver(GOTO_VISIT,this,callfuncO_selector(SGLuaSocketSender::gotoVisit));
}

void SGLuaSocketSender::dealLuaMsg(CCObject *obj)
{
    SGSocketRequest *msg = (SGSocketRequest*)obj;
    main::LuaCommonResponse *respone = (main::LuaCommonResponse*)msg->m_msg;
    respone->cmd();
   // CCLOG("%s",(respone->response()).c_str());
    SGLuaDataProto::shareStatic()->setresponseDate((char*)(respone->response()).c_str());
    
}
    
void SGLuaSocketSender::sendLuaMsg(const char* luaMsg,int msgid,int length)
{
    main::LuaCommonRequest *request = new main::LuaCommonRequest();
    request->set_cmd(msgid);
//     char * x=SGLuaDataProto::shareStatic()->getRequestData();
//    int m = SGLuaDataProto::shareStatic()->getRequestLength();
    request->set_request(luaMsg,length);
    SGSocketClient::sharedSocketClient()->send(MSG_ACTIVITY_LUA, request);
}

void SGLuaSocketSender::sendLuaLayerMsg(const char* luaMsg)
{
    main::TestRequest *request = new main::TestRequest();
    CCString * msgNum = CCString::createWithFormat("%s",luaMsg);
    int cmdId = msgNum->intValue();
    SGSocketClient::sharedSocketClient()->send(cmdId, request);
}

void SGLuaSocketSender::sendLuaBoxMsg(int activityid,int btnid)
{
   

}

void SGLuaSocketSender::showGMLayer()
{
    SGMainManager::shareMain()->showliuyanlayer();
}

void SGLuaSocketSender::showPvpLayer()
{
    SGMainManager::shareMain()->getMainLayer()->battleHandler();
}

void SGLuaSocketSender::showLandingGuiftLayer()
{
    if (!SGMainManager::shareMain()->getMainLayer()->openByActivity) {
        SGMainManager::shareMain()->getMainLayer()->openByActivity = true;
        SGMainManager::shareMain()->getMainLayer()->loginAwardList();
    }
}

void SGLuaSocketSender::showStoreLayer(CCObject *obj)
{

//    if(!SGPlayerInfo::sharePlayerInfo()->getHaveCharged())
//    {
//        
//        SGActivityLayer *layer = (SGActivityLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_luaactivity);
//        if (layer)
//        {
//            layer->showFirstLuaLayer();
//        }
//    }
//    else
//    {
        main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
//    }

//    main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
//    SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
}

void SGLuaSocketSender::showVipInfoLayer(cocos2d::CCObject * obj)
{
    main::ShopVipGiftBagEntryRequest *request = new main::ShopVipGiftBagEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VIPGIFT_BAGS, request);
}

void SGLuaSocketSender::gotoChuangGuan()
{
    if (SGPlayerInfo::sharePlayerInfo()->getIspvp()==true)
    {
        SG_SHOW_WINDOW(str_FirstLayer_str15);
    }
    else
    {
        SGMainManager::shareMain()->getMainLayer()->storyHandler();
    }

}

void SGLuaSocketSender::sendShopRequest(int shopId)
{
    ShopGUID sguid = ShopGUID (shopId);
    SGMainManager::shareMain()->sendEnterMallLayerRequest(sguid , SHOP_LAYER);
}

void SGLuaSocketSender::gotoShop0()
{
    sendShopRequest(SHOPID_PVP);
}

void SGLuaSocketSender::gotoShop1()
{
    sendShopRequest(SHOPID_JING_ZHOU);
}

void SGLuaSocketSender::gotoShop2()
{
    sendShopRequest(SHOPID_YUN_YOU);
}

void SGLuaSocketSender::gotoShop3()
{
    sendShopRequest(SHOPID_LUO_YANG);
}

void SGLuaSocketSender::gotoShop4()
{
    sendShopRequest(SHOPID_SPECIAL_BATTLE);
}

void SGLuaSocketSender::gotoShop5()
{
    sendShopRequest(SHOPID_ARENA);
}

void SGLuaSocketSender::gotoExoticMerchant()
{
    SGMainManager::shareMain()->exoMechMainInfoRequest();
}

void SGLuaSocketSender::gotoInstance()
{
    SGMainManager::shareMain()->getMainLayer()->sendActivityHandler();
}

void SGLuaSocketSender::gotoPvpHome()
{
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    
    if (!player->canBreakLimitById(limitPvpBattle))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str14,player->getLimitLevelById(limitPvpBattle))->getCString() );
    }
    else
    {
        SGMainManager::shareMain()->showPvpMainLayer();
    }

}

void SGLuaSocketSender::gotoArenaHome()
{
//    main::ArenaMainInfoRequest * request = new main::ArenaMainInfoRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_MAIN_INFO, request);
    SGMainManager::shareMain()->goToArenaLayerRequest();
}

void SGLuaSocketSender::gotoSpecialBattle()
{
    SGPlayerInfo *player=SGPlayerInfo::sharePlayerInfo();
    if (!player->canBreakLimitById(limitSpecailBattle))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str23,
                                                                           player->getLimitLevelById(limitSpecailBattle))->getCString() );
        return;
    }
    
    main::SpecialBattleStoryListRequest *storyList = new main::SpecialBattleStoryListRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_CHAP, storyList);

}

void SGLuaSocketSender::gotoBossBattle()
{
    SGPlayerInfo *player=SGPlayerInfo::sharePlayerInfo();
    if (!player->canBreakLimitById(limitBossBattle))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str12,player->getLimitLevelById(limitBossBattle))->getCString() );
        return;
    }
    if (SGPlayerInfo::sharePlayerInfo()->getIspvp()==true) {
        SG_SHOW_WINDOW(str_FirstLayer_str13);
        return;
    }
    
    main::MainBossRequest *request=new main::MainBossRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
}

void SGLuaSocketSender::gotoVisit()
{
    SGMainManager::shareMain()->getMainLayer()->visitHandler();
}
















