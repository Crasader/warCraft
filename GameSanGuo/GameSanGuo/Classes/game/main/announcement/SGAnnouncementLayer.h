//
//  SGAnnouncementLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-8.
//
//

#ifndef __GameSanGuo__SGAnnouncementLayer__
#define __GameSanGuo__SGAnnouncementLayer__

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "ServerMoving.pb.h"
#include "SGBaseBox.h"

USING_NS_CC_EXT;


class SGAnnouncementLayer : public SGBaseBox
{
private:
	
	
    CCScrollView *m_scrollview;
    
    SGButton *buttonOK;
    SGBoxDelegate *_delegate;
	CCMenu *menu;
    
    CCArray *m_arrayAnn;
    
    void viewDidLoad(main::AnnouncementResponse &response);
    
    void buttonClickOK();
    
//    void constructScrollView();
    void pressMask();
    bool iscanmove;
public:
    
    SGAnnouncementLayer();
    
    ~SGAnnouncementLayer();
	
	void playFlowerAnim(CCSprite *spriteFlower);
    void restartFlower( cocos2d::CCNode *s );
	void flowerEffect();
    
    static SGAnnouncementLayer *create(main::AnnouncementResponse &response, SGBoxDelegate * delegate);
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//
    virtual void onEnter();
//    
    CCSize stringInLabelRect(const char *content);

    void initScrollView();
	
	CCLayerColor *initContentWindow(CCString *content, bool isHaveBtn);//初始公告内部内容窗口
    
    void AnnounceButton(CCObject *obj);
public:
    CCPoint m_clickPos;
    float scaley;
    
};

#endif /* defined(__GameSanGuo__SGAnnouncementLayer__) */
