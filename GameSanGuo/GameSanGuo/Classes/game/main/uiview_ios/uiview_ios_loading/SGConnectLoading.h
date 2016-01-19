//
//  SGConnectLoading.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-6.
//
//

#ifndef __GameSanGuo__SGConnectLoading__
#define __GameSanGuo__SGConnectLoading__

#include "cocos2d.h"
#include "SGBaseLayer.h"
class SGConnectLoading:public SGBaseLayer
{
public:
    //ctor
    SGConnectLoading();
    
    static SGConnectLoading*create();
    void initView();
    void setisshow(bool isshow, float cdSecs = 0);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //延迟展示的检测函数
    virtual void update(float dt);
    
    //静态设置delayShow的时间，防止改变过多api。
    static void setDelayShowTime(float dsSec);
    
private:
    //静态delayShow的时间。
    static float _delayShowTime;
    
    //本次X秒后才显示小红人，提升频繁读条的体验。
    float _MaxCdSeconds;
    
    //本次已进行的X秒显示读条等待。
    float _curCdSeconds;
    
    //X秒读条等待的开关标志。
    bool _isDelayShow;
    
    //重置延迟展示参数。
    void _resetDelayShowParams();
};

#endif /* defined(__GameSanGuo__SGConnectLoading__) */
