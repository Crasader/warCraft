//
//  SGUIGuideLayer.h
//  GameSanGuo
//
//  Created by lizi on 13-5-6.
//
//

#ifndef __GameSanGuo__SGUIGuideLayer__
#define __GameSanGuo__SGUIGuideLayer__

#include "SGBaseLayer.h"
#include "SGBoxDelegate.h"
#include "SGGuideVO.h"
#include "SGRichLabel.h"
#include "SGEmbattleItem.h"
#include "CCSpriterX.h"

enum tipDirect
{
    tip_up = 1,
    tip_down,
    tip_left,
    tip_right,
};

class SGUIGuideLayer : public SGBaseLayer
{
private:
    int _currGuideId;
    int _wordIndex;
    tipDirect _direction;
    SGGuideVO *_model;
    SGBoxDelegate *_delegateTouch;
    
    bool _canTouch;
    CCRect _rect; // 可响应区域
    CCPoint _touchPoint;
    
    CCSprite *label;
    CCSprite *speaker;
    CCSprite *bboard;
    CCSprite *spriteTag;
    CCSpriterX *spTouch;
    CCSprite *popClick;
    //SGButton *skipBtn;
    CCSprite *spriteBlackbg;
    CCSprite *actSprite;
    
    SNSTableView *tableview;
    int m_EmbaTimer;
    
    //scrollview与第一个sublayer的位置
    CCPoint scrollViewPos;
    CCPoint subLayerPos;
    
    
private:
    void skipAction(CCObject *sender);
    
    CCRect getRectByGuideId(int ggid);
    void setScrollViewEnable(bool enable);
    SGEmbattleItem* embattleItem;
public:
    void setTableViewVisible(bool visible);
    
public:
    SGUIGuideLayer(void);
    ~SGUIGuideLayer(void);
    
    static SGUIGuideLayer* create();
    void initView();
    void setDatas(SGGuideVO *data, SGBoxDelegate *canTouch);
    
    void registerWithTouchDispatcher();
    void showVisible(bool visible);
    
    void setHighLightRect(CCNode *node, int type, CCArray *data=NULL);
    void removeHighLightRect();
    
    void showJoinBtn();
    void buttonAction(CCObject *sender);
    void resetTaskTouch();
    
    static CCAction* getActionByDirection(tipDirect direct);
    
protected:
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void addTimer();
};

#endif /* defined(__GameSanGuo__SGUIGuideLayer__) */
