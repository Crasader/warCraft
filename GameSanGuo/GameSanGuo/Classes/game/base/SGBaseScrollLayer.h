//
//  SGBaseScrollLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/6/13.
//
//

#ifndef __GameSanGuo_SGBaseScrollLayer_h_
#define __GameSanGuo_SGBaseScrollLayer_h_

#include "SGBaseLayer.h"

class SGGeneralInfoLayer;

class SGBaseScrollLayer : public SGBaseLayer
{
public:
    /**
     * 创建
     */
    SGBaseScrollLayer();
    
    /**
     * 析构
     */
    ~SGBaseScrollLayer();
    
    /**
     * 创建一个武将详情列表
     */
    static SGBaseScrollLayer* create(SGGeneralInfoLayer *sgil);
    
    /**
     * 进场，注册触控。
     */
    void onEnter();
    
    /**
     * 退场，移除触控。
     */
    void onExit();
    
    /**
     * 添加layer，并判定初始化。
     */
    bool initView(SGGeneralInfoLayer* sgbl);
    
public:
    const static bool removeBool = true;

private:
    /**
     * 生成一个武将详情LAYER
     */
    SGGeneralInfoLayer* generatedData(SGGeneralInfoLayer* midLayer, int index);
    
    /**
     * 设置多个layer
     */
    void setItemLayer(CCArray* arr, int amnt);
    
    /**
     * 移动layer之后的事件回调
     */
    void handleEvent();
    
    /**
     * 移动layer
     */
    void moveItemLayer();
    
    /**
     * 使用动作调整位置。-2=不调整。-1=左不调整，0=全部调整，1=右不调整。
     */
    void adjustPosition(int type);
    
    /**
     * 清理一些数据成员。
     */
    void cleanParams();
    
    /**
     * 重置LAYER使能够被触控。
     */
    void resetCanTouched();
    
private:
    /**
     * 左侧对象
     */
    SGGeneralInfoLayer* m_leftObj;
    
    /**
     * 左侧对象X坐标
     */
    float m_leftX;
    
    /**
     * 左侧对象标准位置
     */
    CCPoint m_leftPos;
    
    /**
     * 当前对象
     */
    SGGeneralInfoLayer* m_middleObj;
    
    /**
     * 当前对象X坐标
     */
    float m_middleX;
    
    /**
     * 当前对象标准位置
     */
    CCPoint m_middlePos;
    
    /**
     * 右侧对象
     */
    SGGeneralInfoLayer* m_rightObj;
    
    /**
     * 右侧对象X坐标
     */
    float m_rightX;
    
    /**
     * 右侧对象标准位置
     */
    CCPoint m_rightPos;
    
    /**
     * 当前对象索引
     */
    int m_index;
    
    /**
     * 触控开始的点的X值
     */
    float m_beganX;
    
    /**
     * 触控移动的距离
     */
    float m_dist;
    
    /**
     * 是否可以开始移动。
     */
    bool m_canMove;
    
    /**
     * 是否被点击到了。
     */
    bool m_isTouched;
    
    /**
     * 能否被触控，一般只在动画结束后重新赋值。
     */
    bool m_canTouched;
    
public:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    // optional
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

#endif
