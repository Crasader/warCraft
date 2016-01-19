//
//  SGFirstSubLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-19.
//
//

#ifndef __GameSanGuo__SGFirstSubLayer__
#define __GameSanGuo__SGFirstSubLayer__

#include <iostream>
#include "cocos-ext.h"
#include "SGButton.h"
#include "SGOfficerCard.h"
#include "SGScrollViewSubview.h"
#include "SGBaseShow.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class SGFirstSubLayer :public SGScrollViewSubview{
    
private:
    CCSize _viewSize;
    SGOfficerCard *_officerCard;
    int enterType;
    void constructView();
    int index ;
    void buttonClickOK();
    CCPoint touchBeginLocation;
    bool isMove;
public:
    SGFirstSubLayer();
    ~SGFirstSubLayer();
	void selectEmbattle();
    bool iscantouch;
    virtual void selectSubview();
    void setiscantouch(bool iscan);
    static SGFirstSubLayer *create(SGOfficerCard *officerCard,CCPoint position, CCSize viewSize,int type);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__GameSanGuo__SGFirstSubLayer__) */
