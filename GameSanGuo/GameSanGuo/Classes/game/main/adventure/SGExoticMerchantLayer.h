//
//  SGExoticMerchantLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 12/26/14.
//
//

#ifndef __GameSanGuo__SGExoticMerchantLayer__
#define __GameSanGuo__SGExoticMerchantLayer__

#include "SGBaseLayer.h"
#include "XiyuModule.pb.h"

//最大商品数量。
const static int MAX_GOODS_NUM = 4;

class SGExoticMerchantLayer : public SGBaseLayer
{
//function
public:
    //ctor
    SGExoticMerchantLayer();
    
    //dtor
    ~SGExoticMerchantLayer();
    
    //create
    static SGExoticMerchantLayer* create(main::XiyuInfoResponse* data);
    
    //iv
    bool initView();
    
    //upd
    virtual void updateClock(float dt);
    
    //修改触控。
    virtual void setIsCanTouch(bool isTouch);
    
    /* --红点相关-- */
    
    static void setNoticeTime(int time);
    
    static int& getNoticeTime();
    
    static void setIsForcedOpen(bool bl);
    
    static bool getIsForcedOpen();
    
    static bool getNoticeState();
    
    /* --红点相关-- */
    
//variable
private:
    
    //西域数据。
    main::XiyuInfoResponse* data_;
    
    //刚刚点击的“购买”索引。
    int lastBuyIndex_;
    
    //剩余时间。
    int timeLeft_;
    
    //倒计时文本。
    SGCCLabelTTF* clockLabel_;
    
    //购买按钮。
    SGButton* buyBtnArray_[MAX_GOODS_NUM];
    
    //售罄+遮罩。
    CCLayerColor* maskLayerArray_[MAX_GOODS_NUM];
    
    //查看详情。
    void viewGoods(CCObject* obj);
    
    //购买物品。
    void buyGoods(CCObject* obj);
    
    //购买物品resp。
    void buyGoodsResponse(CCObject *sender);
};


#endif /* defined(__GameSanGuo__SGExoticMerchantLayer__) */
