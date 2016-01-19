//
//  SGLiandanLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#ifndef __GameSanGuo__SGLiandanLayer__
#define __GameSanGuo__SGLiandanLayer__

#include "SGBaseLayer.h"

class SGLiandanLayer : public SGBaseLayer
{
private:
    SGCCLabelTTF *time;
    int nowTime;
    int select;
    SGButton *btn;//高级经验丹按钮
    CCLabelTTF *haveDanLable;
private:
    void updateTime();
    void initView();
    void backHandler();
    void confirmHandler();
    void okHandler();
    void liandanListener(CCObject *obj);
    void refreshDanCount();
    
public:
    SGLiandanLayer();
    ~SGLiandanLayer();
    static SGLiandanLayer *create(int time,int index);
};

#endif /* defined(__GameSanGuo__SGLiandanLayer__) */
