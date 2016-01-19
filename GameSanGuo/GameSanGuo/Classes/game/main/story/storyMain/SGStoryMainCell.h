//
//  SGStoryMainCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGStoryMainCell__
#define __GameSanGuo__SGStoryMainCell__

#include "SGStoryBaseCell.h"
#include "SGLayout.h"
#include "SGStoryBean.h"
#include "SGCCLabelTTF.h"
#include "SGStoryMainLayer.h"

class SGStoryMainCell : public SGStoryBaseCell
{
private:
    CCSprite *spriteIsPass;
    CCSprite * spriteLock; //图片  ”锁“
    CCPoint _lockPos; //记录锁的位置，和中间一个黑星星重合。
    
    CCLabelTTF *labelName;
    SGCCLabelTTF *labelDisc_1;
    SGCCLabelTTF *labelDisc_2;
    CCSprite *rankRewardPop;
    int _conTime;
    ////////
    SGStoryMainCellDelegate *delegate;
    SNSTableView *tableview;
//  virtual  void registerWithTouchDispatcher();
//   virtual void onEnter();
//   virtual bool ccTouchBegan(CCTouch *touches, CCEvent *pevent);
//   
    ///////
    
private:
    void viewDidLoad();
    void resetTime();
    CCSpriterX * m_spTouch;
public:
    SGStoryMainCell();
    ~SGStoryMainCell();
//    static SGStoryMainCell *create();
 static   SGStoryMainCell*create(SGStoryMainCellDelegate*del,SNSTableView*table);
    
    void setData(int index, SGStoryBean* bean);
    ////////
    void buttonClick(CCObject*obj);
    //////////
};

#endif /* defined(__GameSanGuo__SGStoryMainCell__) */
