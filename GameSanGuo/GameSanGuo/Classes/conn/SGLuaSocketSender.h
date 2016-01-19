//
//  SGLuaSocketSender.h
//  GameSanGuo
//
//  Created by zenghui on 14-1-8.
//
//

#ifndef __GameSanGuo__SGLuaSocketSender__
#define __GameSanGuo__SGLuaSocketSender__

#include "cocos2d.h"

USING_NS_CC;
class SGLuaSocketSender : public CCObject
{
    
public:
    static SGLuaSocketSender* shareLuaSender(void);
    void sendLuaMsg(const char* luaMsg,int msgid,int length);
    void sendLuaLayerMsg(const char* luaMsg);
    void dealLuaMsg(CCObject *obj);
    void sendLuaBoxMsg(int activityid,int btnid);
protected:
    SGLuaSocketSender();
    ~SGLuaSocketSender();
    static SGLuaSocketSender *s_shareLuaSender;
    void init();
    
    void showGMLayer();
    void showPvpLayer();
    void showLandingGuiftLayer();
    void showStoreLayer(CCObject *obj);
    void showVipInfoLayer(CCObject *);

    void sendShopRequest(int shopId);
    
    //各种lua消息回调
    void gotoChuangGuan();
    //0 1 2 3 4 5  天梯、荆州、云游 、洛阳、幻境、乱世（竞技场）商店
    void gotoShop0();
    void gotoShop1();
    void gotoShop2();
    void gotoShop3();
    void gotoShop4();
    void gotoShop5();

    void gotoExoticMerchant();//西域商人
    
    void gotoInstance();
    void gotoPvpHome();
    void gotoArenaHome();
    void gotoSpecialBattle();
    void gotoBossBattle();
    void gotoVisit();
    
    
};

#endif /* defined(__GameSanGuo__SGLuaSocketSender__) */
