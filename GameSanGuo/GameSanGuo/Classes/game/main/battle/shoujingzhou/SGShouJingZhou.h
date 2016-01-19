//
//  SGShouJingZhou.h
//  GameSanGuo
//
//  Created by kanata on 13-11-14.
//
//  @description:守荆州主页面，由消息进入，消息返回之前会锁屏。
//

#ifndef __GameSanGuo__SGShouJingZhou__
#define __GameSanGuo__SGShouJingZhou__

#include "SGBaseLayer.h"

#endif /* defined(__GameSanGuo__SGShouJingZhou__) */

class SGShouJingZhou: public SGBaseLayer
{
public:
    SGShouJingZhou();
    ~SGShouJingZhou();
    
    //创建
    static SGShouJingZhou *create();
    
    //初始化视图
    virtual void initView();
    
    //返回按钮
    void backHandler(CCObject*obj);
    
    //开始守城
    void challengeHandler();
    
    //军魂换礼
    void exchangeHandler();
    
    //规则说明
    void ruleHandler(CCObject*obj);
    
    //战绩排行
    void rankingHandler(CCObject*obj);
    
    //战绩排行cb
    void rankingCallback(CCObject* obj);
    /////守荆州消耗VIP次数 花费元宝请求
    void sendVipCostChanceRequest(int nowchance);
    
    void challengeprocess();
    void receivebossinfo(CCObject*obj);
    void buychance(CCObject *sender);

    //重写sgbaselayer的接口,刷新最高伤害数据。问题来源-- 当第一次守荆州失败，且有伤害，从商城返回时页面未刷新
    virtual void refreshView();
 private:
    SGCCLabelTTF*labelcs;//今日参战
    
    SGCCLabelTTF* labelMax ;//最高伤害。
    
};