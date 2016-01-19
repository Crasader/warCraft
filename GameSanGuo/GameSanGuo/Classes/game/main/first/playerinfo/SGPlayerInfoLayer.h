//
//  SGPlayerInfoLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-21.
//
//

#ifndef __GameSanGuo__SGPlayerInfoLayer__
#define __GameSanGuo__SGPlayerInfoLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"
class SGPlayerInfoLayer : public SGBaseLayer
{
private:
    CCLabelTTF *tili;           //体力
    
    SGCCLabelTTF *huifu;          //军粮回复
    SGCCLabelTTF *qhuifu;         //军粮全部回复
    SGCCLabelTTF *hf;             //体力回复
    SGCCLabelTTF *qhf;            //体力全部回复
    CCArray *_vipGifts;
    CCMenu *menu1;
    CCScrollView *scroll;
public:
    SGCCLabelTTF *havegold;
    SGCCLabelTTF *label_junliang; //军粮
private:
    void initView();
    void backHandler();
    void test();
    //整合到mainLayer
    //    void storeEnterListener(CCObject *obj);
    void exchangeEnterListener(CCObject *obj);
    void pvpexchangeEnterListener(CCObject *obj);
    void showStoreLayer();
    void showExchangeLayer();
    void showCountrysInfo();
    void changNickname();
    void recoverPower();
    void recoverPvpPower();
    void pvpexchang();
    void showvipinfo();
    int GetCurrentTYL();
    void vipGiftBagListener(cocos2d::CCObject *obj);
    
    //军魂换礼 去荆州商店
    void junHunBtnClick();
public:
    SGPlayerInfoLayer();
    ~SGPlayerInfoLayer();
    static SGPlayerInfoLayer *create();
    
    //刷新界面
    void updateInfo(const char *tiliLeftTime,const char *tiliTime,const char *junliangLeftTime,const char*junliangTime);
    virtual void setIsCanTouch(bool isTouch);
};

#endif /* defined(__GameSanGuo__SGPlayerInfoLayer__) */
