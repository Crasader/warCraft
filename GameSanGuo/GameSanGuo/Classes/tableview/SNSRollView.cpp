//
//  SNSRollView.cpp
//  CardDragBox
//	version 1.0
//  Created by orix on 28/9/12.
//
//

#include "SNSRollView.h"

SNSRollView::SNSRollView()
:m_data(NULL), m_stopIndex(-1), m_rollPointer(0), m_isAdd(false),
m_delegate(NULL), m_backImage(NULL), m_isSlowDown(false), m_nowPointer(0),
m_speed(0), m_halfFrameHeight(0)
{
	m_frame = CCRectMake(0, 0, 0, 0);
	m_pos = ccp(0, 0);
}

SNSRollView::~SNSRollView()
{
	if (NULL != m_data) {
		CC_SAFE_DELETE(m_data);
	}
	if (NULL != m_backImage) {
		CC_SAFE_DELETE(m_backImage);
	}
}

bool SNSRollView::initRollViewFrame(CCRect frame)
{
	if (!SNSView::initViewFrame(frame)) {
		return false;
	}
	//根据frame设置自己的position
	m_frame = frame;
	m_halfFrameHeight = m_frame.size.height * 0.5f;
	this->setPosition(ccp(frame.origin.x, frame.origin.y));
	//设置初始滚动的速度
	m_speed = 0.3f;
	
	return true;
}

SNSRollView* SNSRollView::initWithFrame(CCRect frame)
{
    return SNSRollView::create(frame);
}

SNSRollView* SNSRollView::create(CCRect frame)
{
    SNSRollView* pRet = new SNSRollView();
    
    if ( pRet && pRet->initRollViewFrame(frame) ) {
        pRet->autorelease();
        return pRet;
    }
	CC_SAFE_DELETE(pRet);
    return NULL;
}

#pragma mark - set data and start animation

void SNSRollView::setDelegate(SNSRollViewDelegate *delegate)
{
	m_delegate = delegate;
}

void SNSRollView::setData(CCArray *data, CCSprite *backImage)
{
	m_data = data;
	m_data->retain();
	CCAssert(NULL != backImage, "Back image should not be NULL!");
	m_backImage = backImage;
	m_backImage->retain();  // retain一下这两个元素，防止提前释放
	if (NULL == m_data) return;
	//创建一个独立的sprite，保证数据中的sprite不被修改
	CCSprite *floatSprite = CCSprite::create();
	floatSprite->setDisplayFrame(backImage->displayFrame());	
	floatSprite->setPosition(ccp(m_frame.size.width * 0.5f, m_halfFrameHeight));
	addChild(floatSprite, 0, kSpriteZOrder);
}

void SNSRollView::resetValue()
{
	// 重新设置基础数值
	m_stopIndex = -1;
	m_speed = 0.3f;
	m_rollPointer = 0;
	m_isAdd = false;
	m_isSlowDown = false;
}

void SNSRollView::startAnimation()
{
	if (NULL == m_data) return;
	this->resetValue(); // 重新设置旋转时需要的数值
	schedule(schedule_selector(SNSRollView::update));
}

void SNSRollView::update(float dt)
{
	unsigned int count = m_data->count();
	if (count < 1) return; //低于1个元素就不用滚动了，滚动也没意义
	
	//先移动元素，翻牌完毕再修改指针
	CCSprite *spr = (CCSprite *)this->getChildByTag(kSpriteZOrder);
	if (spr->getScaleX() >= 1) m_isAdd = false;
	if (spr->getScaleX() <= 0) {
		m_isAdd = true;
		//把指针往下移动
		++m_rollPointer;
		CCSprite *newSprite = NULL;
		if (m_speed <= 0) {
			//运动停止时结束循环
			unschedule(schedule_selector(SNSRollView::update));
			//获取应该移动到的图，再播放一次动画
			if (m_stopIndex == -1) {
				newSprite = m_backImage;
			} else {
				newSprite = (CCSprite *)m_data->objectAtIndex(m_stopIndex);
			}
			spr->setDisplayFrame(newSprite->displayFrame());
			// 如果是网络超时状态，那么加速结束
			int duration = (m_stopIndex == -1)?1.2f:1.2f;
			CCScaleTo *scaleChange = CCScaleTo::create(duration, 1.0f);
			CCDelayTime *delay = CCDelayTime::create(0.6f);
			CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SNSRollView::animationDidStop));
			CCAction *seq = CCSequence::create(scaleChange, delay, call, NULL);
			spr->runAction(seq);
			return;
		}
		//CCLOG("pointer:%d mod:%d", m_rollPointer, m_rollPointer % 2);
		if (m_rollPointer % 2) {
			//转到正面
			int index = (m_nowPointer >= count)?m_nowPointer % count:m_nowPointer;
			//CCLOG("count:%d index:%d", count, index);
			newSprite = (CCSprite *)m_data->objectAtIndex(index);
			++m_nowPointer;
		} else {
			//转到背面
			newSprite = m_backImage;
		}
		spr->setDisplayFrame(newSprite->displayFrame());
		if (m_isSlowDown) m_speed -= (m_stopIndex == -1)?0.15f:0.06f; // 这里如果网络超时，那么直接转到背面，否则转到前面
	}
	// 这里放一个加速以控制翻牌的速度
	float realSpeed = m_speed + 0.06f;
	spr->setScaleX(spr->getScaleX() + (m_isAdd?realSpeed:-realSpeed));
}

void SNSRollView::stop()
{
	m_isSlowDown = true;
}

void SNSRollView::stopAtIndex(int index)
{
	m_stopIndex = index;
	this->stop();
}

void SNSRollView::animationDidStop()
{
	//CCLOG("动画播放结束\0");
	if (NULL != m_delegate) {
		m_delegate->anmitionDidStop();
	}
}

#pragma mark - SNSFlipView -

SNSFlipView::SNSFlipView():m_frontNode(NULL), m_backNode(NULL), m_canRoll(true), m_target(NULL), m_duration(0), m_isAutoBack(false), m_autoBackDelay(0), m_selector(NULL)
{
	m_frontScale = CCSizeMake(1.0f, 1.0f);
	m_backScale = CCSizeMake(1.0f, 1.0f);
}

SNSFlipView::~SNSFlipView()
{
	
}

bool SNSFlipView::initWithNodes(CCNode* frontNode, CCNode* backNode, CCRect frame, CCObject* target, SEL_CallFuncN selector, float duration, bool isAutoBack, float autoBackDelay)
{
	if (NULL == frontNode || NULL == backNode) {
		return false;
	}
	if (!CCLayer::init()) {
		return false;
	}
	m_target = target;
	m_selector = selector;
	m_duration = duration;
	m_isAutoBack = isAutoBack;
	m_autoBackDelay = autoBackDelay;
	
	this->setContentSize(CCSizeMake(frame.size.width, frame.size.height));
	this->setPosition(ccp(frame.origin.x, frame.origin.y));
	
	m_frontNode = frontNode;
	m_backNode = backNode;
	// 记录原始scale，以防更改
	m_frontScale = CCSizeMake(m_frontNode->getScaleX(), m_frontNode->getScaleY());
	m_backScale = CCSizeMake(m_backNode->getScaleX(), m_backNode->getScaleY());
	
	this->addChild(frontNode);
	this->addChild(backNode);
	backNode->setVisible(false);
	
	this->setTouchEnabled(true);
	
	return true;
}

SNSFlipView* SNSFlipView::create(CCNode* frontNode, CCNode* backNode, CCRect frame, CCObject* target, SEL_CallFuncN selector, float duration, bool isAutoBack, float autoBackDelay)
{
	SNSFlipView* instance = new SNSFlipView();
	if (instance && instance->initWithNodes(frontNode, backNode, frame, target, selector, duration, isAutoBack, autoBackDelay)) {
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return NULL;
}

void SNSFlipView::runFlipX(float duration, bool isAutoBack/* = false*/, float autoBackDelay/* = 3.0f*/)
{
	m_canRoll = false;
	float realDuration = duration * 0.5f;
	float delayTime = duration;
	m_frontNode->runAction(CCSequence::create(CCScaleTo::create(realDuration, 0.001f, m_frontNode->getScaleY()), CCCallFuncN::create(this, callfuncN_selector(SNSFlipView::hideFrontShowBack)), NULL));
	m_backNode->runAction(CCSequence::create(CCDelayTime::create(realDuration), CCScaleTo::create(realDuration, m_backScale.width, m_backScale.height), NULL));
	if (isAutoBack) {
		// 如果需要自动翻转回来的话
		m_backNode->runAction(CCSequence::create(CCDelayTime::create(duration + autoBackDelay), CCScaleTo::create(realDuration, 0.001f, m_frontNode->getScaleY()), CCCallFuncN::create(this, callfuncN_selector(SNSFlipView::showFrontHideBack)), NULL));
		m_frontNode->runAction(CCSequence::create(CCDelayTime::create(duration + autoBackDelay + realDuration), CCScaleTo::create(realDuration, m_frontScale.width, m_frontScale.height), NULL));
		delayTime = duration * 2.0f + autoBackDelay;
	}
	if (m_target && m_selector) {
		this->runAction(CCSequence::create(CCDelayTime::create(delayTime), CCCallFuncN::create(m_target, m_selector), NULL));
	}
}

void SNSFlipView::showFrontHideBack()
{
	m_backNode->setVisible(false);
	m_frontNode->setVisible(true);
	m_frontNode->setScaleX(0.001f);
	m_canRoll = true;
}

void SNSFlipView::hideFrontShowBack()
{
	m_frontNode->setVisible(false);
	m_backNode->setVisible(true);
	m_backNode->setScaleX(0.001f);
}

#pragma mark - touch function

void SNSFlipView::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool SNSFlipView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//NSLog(@"touch start");
	
	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent()) {
        if (c->isVisible() == false) {
            return false;
        }
    }
	
	CCPoint touchLocation = pTouch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	CCPoint pos = getParent()->convertToWorldSpace(this->getPosition());
	CCRect touchEffectiveArea = CCRectMake(pos.x - this->getContentSize().width * 0.5f, pos.y - this->getContentSize().height * 0.5f, this->getContentSize().width, this->getContentSize().height);
	if(touchEffectiveArea.containsPoint(touchLocation) && m_canRoll && m_bVisible) {
		return true;
	}
	return false;
}

void SNSFlipView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	//NSLog(@"Move body");
//    CCPoint touchLocation = pTouch->getLocationInView();
//    CCPoint prevLocation = pTouch->getPreviousLocationInView();
//    
//    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
//    prevLocation = CCDirector::sharedDirector()->convertToGL(prevLocation);

}

void SNSFlipView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    //NSLog(@"移动结束");
//    CCPoint touchLocation = pTouch->getLocationInView();
//    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
//    CCPoint moveLength = ccpSub(touchLocation, m_scrollLastVector);
//    CCPoint returnPosition = ccpSub(ccpSub(touchLocation, m_pos), m_body->getPosition());
	runFlipX(m_duration, m_isAutoBack, m_autoBackDelay);
}

void SNSFlipView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	
}