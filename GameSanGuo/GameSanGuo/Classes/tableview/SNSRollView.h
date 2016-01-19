//
//  SNSRollView.h
//  CardDragBox
//	version 1.0
//  Created by orix on 28/9/12.
//
//

#ifndef CardDragBox_SNSRollView_h
#define CardDragBox_SNSRollView_h

#include "SNSView.h"

#define kSpriteZOrder 3

class SNSRollViewDelegate
{
public:
	virtual void anmitionDidStop() = 0;
};

class SNSRollView : public SNSView
{
protected:
    SNSRollView();
    virtual ~SNSRollView();
    bool initRollViewFrame(CCRect frame);
	
	void update(float dt);
	
	SNSRollViewDelegate *	m_delegate;			// delegate
	CCArray *				m_data;				// 图片数据，里边对象类型是CCSprite
	CCSprite *				m_backImage;		// 卡牌背面的图片
	int						m_stopIndex;		// 停止位置在数据中的索引
	CCPoint					m_pos;				// 位置
	float					m_speed;			// 移动速度
	float					m_halfFrameHeight;	// 半个frame的高度
	int						m_rollPointer;		// 移动指针，判断当前移动到第几个元素了
	int						m_nowPointer;		// 正面指针，判断当前是第几个元素
	bool					m_isAdd;			// 判断是否是放大卡牌操作（模拟旋转）
	bool					m_isSlowDown;		// 是否允许减速（减速的话旋转就会结束）
	
public:
    CC_DEPRECATED_ATTRIBUTE static SNSRollView* initWithFrame(CCRect frame);
	static SNSRollView* create(CCRect frames);
	
	// 设置delegate
	void setDelegate(SNSRollViewDelegate *delegate);
	// 设置数据（ccsprite数组）
	void setData(CCArray *data, CCSprite *backImage);
	// 重置基础数值
	void resetValue();
	// 开始播放动画
	void startAnimation();
	// 停止播放动画,并且停止在背面
	void stop();
	// 设置最终停止的位置并播放停止动画
	void stopAtIndex(int index);
	// 结束播放动画的回调函数
	void animationDidStop();
	
};

class SNSFlipView : public CCLayer
{
public:
	SNSFlipView();
	~SNSFlipView();

	virtual bool initWithNodes(CCNode* frontNode, CCNode* backNode, CCRect frame, CCObject* target, SEL_CallFuncN selector, float duration, bool isAutoBack, float autoBackDelay);
	static SNSFlipView* create(CCNode* frontNode, CCNode* backNode, CCRect frame, CCObject* target, SEL_CallFuncN selector, float duration = 0.3f, bool isAutoBack = false, float autoBackDelay = 3.0f);

public:
	void runFlipX(float duration, bool isAutoBack, float autoBackDelay);
	
protected:
	
	virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

private:
	CCNode*					m_frontNode;				// 前层指针
	CCNode*					m_backNode;					// 背部层指针
	CCSize					m_frontScale;				// 前景记录到的scaleX和Y
	CCSize					m_backScale;				// 背部记录到的scaleX和Y
	
	CCObject*				m_target;
	SEL_CallFuncN			m_selector;
	float					m_duration;
	bool					m_isAutoBack;
	float					m_autoBackDelay;
	
	bool					m_canRoll;					// 控制是否可以响应点击事件

private:
	void showFrontHideBack();
	void hideFrontShowBack();

};

#endif
