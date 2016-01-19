//
//  SGLimitGuideLayer.h
//  GameSanGuo
//
//  Created by zenghui on 13-12-26.
//
//

#ifndef __GameSanGuo__SGLimitGuideLayer__
#define __GameSanGuo__SGLimitGuideLayer__

#include "SGBaseLayer.h"
#include "SGBoxDelegate.h"
#include "SGLimitGuideVO.h"
#include "SGRichLabel.h"
#include "SGAdvanceLayer.h"


class SGLimitGuideLayer : public SGBaseLayer
{
private:
    int _currGuideId;
    int _wordIndex;
    SGLimitGuideVO *_model;
    
    bool _canTouch;
    CCRect _rect; // 可响应区域
    CCPoint _touchPoint;
    
    CCSprite *label;
    CCSprite *speaker;
    CCSprite *bboard;
    CCSprite *actSprite;
    
    SNSTableView *tableview;
    SNSTableView *oldTableview;
    SGButton *btn ;
    CCNode *generalIcon ;
    int m_removeRectTag;
    SNSIndexPath *path;
    
    //scrollview与第一个sublayer的位置
    CCPoint scrollViewPos;
    CCPoint subLayerPos;
    
    CCLayerColor *color;

    
private:
    
    void skipAction(CCObject *sender);
    
    CCRect getRectByGuideId();
    void setScrollViewEnable(bool enable);
    CCAction* getActionByDirection(int direct);
public:
    SGBoxDelegate *_delegateTouch;
    void setTableViewVisible(bool visible);
    CCSprite *spriteBlackbg;
    CCSprite *spriteTag;
public:
    SGLimitGuideLayer(void);
    ~SGLimitGuideLayer(void);
    
    static SGLimitGuideLayer* create();
    void initView();
    void setDatas(SGLimitGuideVO *data, SGBoxDelegate *delegate);
    
    void registerWithTouchDispatcher();
    void showVisible(bool visible);
    
    void setHighLightRect(CCNode *node, int type,int nodeId,CCRect &rect);
    void removeHighLightRect();
    
    void showJoinBtn();
    void buttonAction(CCObject *sender);
    void resetTaskTouch();
    
    inline SGLimitGuideVO *getModel(){return _model;}
    
protected:
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__GameSanGuo__SGLimitGuideLayer__) */
