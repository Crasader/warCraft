//
//  SGTouchLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/12/13.
//
//  @description:用于处理触控和滑动事件的类，伪控件。
//
//

#ifndef GameSanGuo_SGTouchLayer_h
#define GameSanGuo_SGTouchLayer_h

#include "cocos2d.h"

USING_NS_CC;

//五个回调
typedef bool (CCObject::*PFBegan) (CCTouch *, CCEvent *);
typedef void (CCObject::*PFMoved) (CCTouch *, CCEvent *);
typedef void (CCObject::*PFEnded) (CCTouch *, CCEvent *);
typedef void (CCObject::*PFCancelled) (CCTouch *, CCEvent *);
typedef void (CCObject::*PFCallBack) ();

//回调使用
#define pf_began_selector(_MJ) (PFBegan) (&_MJ)
#define pf_moved_selector(_MJ) (PFMoved) (&_MJ)
#define pf_ended_selector(_MJ) (PFEnded) (&_MJ)
#define pf_cancelled_selector(_MJ) (PFCancelled) (&_MJ)
#define pf_callback_selector(_MJ) (PFCallBack) (&_MJ)

//形态枚举
enum SGTouchMode
{
    SGTM_BUTTON = 0, //按钮
    SGTM_CONTROL, //自控
};

//触控状态枚举
enum SGTouchState
{
    SGTS_NULL = -1, //无状态
    SGTS_TOUCHED, //响应了began
//    SGTS_CANWORK //暂时不可用
};

class SGTouchLayer : public CCLayerColor
{
public:
    //构造
    SGTouchLayer();
    
    //析构
    ~SGTouchLayer();
    
    //创建。 width/height：本layer的触控范围，即ContentSize。  prior：触控级别，默认-100。  color：默认透明无色。
    static SGTouchLayer* create(float width = 10, float height = 10, int prior = -100, ccColor4B color = ccc4(0, 0, 0, 0));
    
    //①变身为按钮形态，注册回调函数。
    void setButtonMode(CCObject* obj, PFCallBack pf);
    
    //②变身为触控形态，注册四种触控函数。
    void setControlMode(CCObject* obj, PFBegan began, PFMoved moved, PFEnded ended, PFCancelled cancelled);
    
    //began
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //moved
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    //ended
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    //cancelled
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
//    //形态枚举
//    CC_SYNTHESIZE(SGTouchMode, m_mode, Mode);
    
    //触控层级，谨慎设置，别覆盖了其他的LAYER。
    CC_SYNTHESIZE(int, m_priority, Priority);
    
    //是否工作，即接受触控。总开关。
    CC_SYNTHESIZE(bool, m_isWorking, IsWorking);
    
    //进场，注册触控。
    void onEnter();
    
    //退场，移除触控。
    void onExit();
    
private:
    //形态枚举
    SGTouchMode m_mode;
    
    //当前回调对象
    CCObject* m_targetObj;
    
    //当前触控状态
    SGTouchState m_state;
    
    //callback func
    PFCallBack m_pfCallBack;
    
    //began func
    PFBegan m_pfBegan;
    
    //move func
    PFMoved m_pfMoved;
    
    //ended func
    PFEnded m_pfEnded;
    
    //cancelled func
    PFCancelled m_pfCancelled;

};

#endif
