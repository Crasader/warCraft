//
//  SGAdventureItem.h
//  GameSanGuo
//
//  Created by Fygo Woo on 12/26/14.
//
//

#ifndef __GameSanGuo__SGAdventureItem__
#define __GameSanGuo__SGAdventureItem__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/*
 * 奇遇页面的点击回调。
 */
class SGAdventureDelegate
{
//function
public:
    //点击了index的条目。
    virtual void selectHandler(int index) = 0;
};

/*
 * 奇遇页面顶部条目。
 */
class SGAdventureItem : public CCLayerColor
{
//function
public:
    //ctor
    SGAdventureItem();
    
    //dtor
    ~SGAdventureItem();
    
    //create。image为传进来的图片，text为文本。
    static SGAdventureItem* create(SGAdventureDelegate* deg, const std::string& image, const int index, const CCSize& vSize, const std::string& text = "");
    
    //设置触控开关。
    void setEnabled(bool bl);
    
    //设置选中状态。
    void setSelected(bool bl);
    
    //设置红点显隐。
    void setNotice(bool bl);
    
    //began
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //moved
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    //ended
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    //iv
    bool initView(const std::string& image, const CCSize& vSize, const std::string& text);
    
    //按钮回调。
    void btnHandler(CCObject* sender);
    
//variable
private:
    //本item的回调对象。
    SGAdventureDelegate* deg_;
    
    //本item代表的索引。
    int index_;
    
    //本item的size。
    CCSize viewSize_;
    
    //承载按钮的选单。
    CCMenu* menu_;
    
    //选中框。
    CCSprite* selectedSpt_;
    
    //红点。
    CCSprite* noticeSpt_;
    
    //是否触控移动中。
    bool isMoving_;
    
    //是否可触控。
    bool canTouch_;
    
    //触控开始位置。
    CCPoint touchBeginLocation_;
};


#endif /* defined(__GameSanGuo__SGAdventureItem__) */
