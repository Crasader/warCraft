//
//  SGDynamicCirculateLayer.h
//  Test
//
//  Created by 江南岸 on 13-4-8.
//
//

#ifndef __Test__SGDynamicCirculateLayer__
#define __Test__SGDynamicCirculateLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ServerMoving.pb.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"

using namespace cocos2d;
using namespace extension;

class SGDynamicCirculateLayer : public CCLayerColor
{
private:
    //存放所有label，label池
    CCArray *labelArray;
    //动态数组
    CCArray *stringArray;
    //移动中的数组
    CCArray *moveArray;
    //没有公告时的公告信息
    CCArray *localArray;
    //是否在移动中
    bool _isMoving;
    //是否在屏幕中间
    bool _isInMiddle;
    //屏幕中间计数器
    int _inMiddleCounter;
    //初始化界面
    void viewDidLoad();
    //定时器
    void updateTimer();
    //构建一个新的滚动条    
    void constructDynamic();
    //服务器发来的数据
    void dynamicListener(CCObject *object);
    //颜色字体
    ccColor3B FontColor(CCString * str);
    
    int localCount;
public:
    SGDynamicCirculateLayer();
    
    ~SGDynamicCirculateLayer();
    
    static SGDynamicCirculateLayer *create();
    
};

#endif /* defined(__Test__SGDynamicCirculateLayer__) */
