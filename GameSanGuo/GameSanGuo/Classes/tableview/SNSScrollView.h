//
//  SNSScrollView.h
//  SNSUIKit
//	version 2.0
//  Created by yang jie on 24/11/2011.
//  Copyright 2011 ourpalm.com. All rights reserved.
//

#ifndef _SNSUIKIT_SNSSCROLLVIEW_H_
#define _SNSUIKIT_SNSSCROLLVIEW_H_

#include "cocos2d.h"

USING_NS_CC;

#include "SNSView.h"

typedef enum {
	ScrollViewChildTypeINVALID = 0,
	ScrollViewChildTypeBody,
	ScrollViewChildTypeRightSlider,
	ScrollViewChildTypeBottomSlider,
} ScrollViewChildType;

typedef enum {
	ScrollViewStatusINVALID = 0,
	ScrollViewStatusNormal,
	ScrollViewStatusCanNotMove,
	ScrollViewStatusCanNotTouch,
} ScrollViewStatus;

class SNSScrollView;
class SNSScrollViewDelegate
{
public:
    // any offset changes
    virtual void scrollViewDidScroll(SNSScrollView* scrollView){};

    // called when scrollView click event
    virtual void scrollViewDidClickBody(SNSScrollView* scrollView, CCPoint position){};
    
    //当滚动scrollView的时候发送当前滚动百分比（1～100）之间的数值，以方便自定义滚动条(ratio的x和y分别是)
    virtual void scrollViewDidScroll(SNSScrollView* scrollView, CCPoint ratio){};
    
	//scrollView轻扫的时候调用的函数
    virtual void scrollViewCellDeleteAction(SNSScrollView* scrollView, CCPoint ratio){};

};

class SNSScrollViewPageDelegate
{
public:
    int _nowPage;
    int _totalPage;
    CCArray *_allData; //所有数据
    // 设置翻页回调 type 1:pre 2:next
    virtual void refreshPage(int type){};
};

/*
 */
class SNSScrollView : public SNSView 
{
public:
	CCLayer*					m_body;
    
	CCPoint						m_scrollInertia;
	CCPoint						m_bodyRealPoint;
	CCPoint                     m_scrollStartVector;
    CCPoint                     m_scrollLastVector;
	CCPoint						m_pos;
	CCPoint						m_bodyNeedMove;
    
    //与轻扫有关的操作
    float                       lineK;
    float                       moveMaxDistance;
    float                       moveMax_x;
    float                       moveMax_y;
	
	float						m_sliderOpacity;
	
    float resetDownLimit;
	ScrollViewStatus			m_status;
	
	bool						m_isCanScroll;
    bool                        m_isDraging;
	// 是否允许弹动（是到边上就停止还是弹动）
	CC_SYNTHESIZE(bool, m_bounce, Bounce);
	
	// 横向滑动还是纵向滑动
	CC_PROPERTY(bool, m_horizontal, Horizontal);
	CC_PROPERTY(bool, m_vertical, Vertical);
	// 记录是否启用翻页的bool
	CC_PROPERTY(bool, m_isPageEnable, PageEnable);
	// 设置是否让page功能最后停止在边框位置（也就是说假设一页显示10个item，每行/列有2个元素，假设最后一页剩下3个，那么这3个是完全占一页，剩下的空着还是直接贴边对其；对齐true，不对齐false）
	CC_SYNTHESIZE(bool, m_isPageToBorder, PageToBorder);
	// 是否显示滑动条
	CC_SYNTHESIZE(bool, m_showSlider, ShowSlider);
	// 记录是否启用左右翻页的bool
	CC_SYNTHESIZE(bool, m_pageEnable, PageNEnable);
    // 记录是左右翻页还是上下移动
	CC_SYNTHESIZE(bool, m_pageVertical, PageVertical);
    CC_SYNTHESIZE(bool, m_pageHorizontal, PageHorizontal);
    
	CC_SYNTHESIZE(SNSScrollViewDelegate*, m_delegate, Delegate);
    CC_SYNTHESIZE(SNSScrollViewPageDelegate*, m_pagegate, Pagegate);
	// 触碰有效区概念，也就是说在这个区域之中的touch事件我们才响应,与剪裁区域区分开来 add by ver 2.0
	CC_SYNTHESIZE(CCRect, m_touchEffectiveArea, TouchEffectiveArea);
    //可触发翻页的距离设置，原为220不好滑，现默认采用100，可自行根据需求设置，必须是正值。
    CC_SYNTHESIZE(float, m_turnPageDist, TurnPageDist);
    
public: /*重定义父类函数*/
    virtual void setFrame(CCRect var);
    virtual void setBodySize(CCSize var);
	virtual void setPosition(CCPoint pos);
    virtual void registerWithTouchDispatcher();
    virtual void visit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void setIsTouchEnabled(bool enabled);
	
	virtual void setVisible(bool isVisible);
    
protected:
    SNSScrollView();
    virtual ~SNSScrollView();
    bool initScrollViewFrame(CCRect frames);
    
public:
	// 随着cocos2dx在进化，这个函数可能会弃用
    CC_DEPRECATED_ATTRIBUTE static SNSScrollView* initWithFrame(CCRect frames);
	static SNSScrollView* create(CCRect frames);
    
private:
    void updateBody(float delta);
	int autoLength(int length);
	long long getLongTimes();
	
	CCPoint						m_pageInertia;
	float						m_nowPageTop, m_nowPageBottom, m_nowPageLeft, m_nowPageRight;
	CCSize						m_halfWinSize;
	
	long long                   m_startMoveInterval;
    long long                   m_startDragInterval;
    
    CCLayerColor*               m_sliderRight;
    CCLayerColor*               m_sliderBottom;
	
	//page效果需要参数
	CC_SYNTHESIZE(int, m_pageCount, PageCount);						// 总页数
	int							m_nowPage;							// 当前所在页数
    bool                        m_pageControl;						// 是否显示翻页控件
	
	bool						m_swallowsTouches;					// 是否吸收点击事件
    bool isMove;
    
    float moveDistance;
protected:
    //绘制滑动条
    void drawSlider();
    //显示slider
    void showSliders();
    //隐藏slider
    void hideSliders();
    //移动slider
    void moveSliders();
	//移动body的位置
	void moveBodyToPosition(CCPoint position);
	//移动body位置的schdule
	void moveBodySchedule(float delta);
	//修正body的复用位置
	void fixPosition();
    
public:
    //当scrollView滚动的时候触发的事件
    virtual void scrollViewDidScroll();
	//当scrollView启用分页功能的时候触发的事件
	virtual void returnNowPage(int nowPage, int pageCount);
    //当scrollView点击的时候触发的事件
    virtual void scrollViewDidClick(CCPoint position, CCTouch* touch);
    //点scrollView横扫的时候触发的事件
    virtual void scrollViewCellDeleteAction(SNSScrollView *scrollView, CCPoint position);
    //scrollView内容物拖动开始的事件
    virtual void scrollViewDidStartDrag(CCPoint position);
    //scrollView内容物拖动结束的事件
    virtual void scrollViewDidEndDrag(CCPoint position);
    virtual void setBunSelected(CCPoint position, CCTouch* touch, bool isEnd = false);
	virtual bool isMoveEnd();
	//设置是否需要显示滑动条
	void setSliderVisible(bool visible);
	//设置slider的颜色以及透明度
    void setSliderColor(ccColor4B color);
    //强制停止scrollView的滚动
    void stopScroll();
    //停止自己的所有action
    void stopActions();
	// 设置是否吸收点击事件
	void setSwallowsTouches(bool isSwallow);

    //手动移动body坐标, xy坐标分别传0~100之间的值过去代表他的百分比
    void moveBody(CCPoint ratio);
    //启用/禁用拖动操作
    void setMoveEnable(bool isEnable);
	//移动到指定页面（仅在将isPageEnable启用的时候有效）
	void moveToPage(int page);
	//是否设置pageControl控件（待开发）
    void setPageControl(bool pageControl);
    
};

#endif // _SNSUIKIT_SNSSCROLLVIEW_H_

