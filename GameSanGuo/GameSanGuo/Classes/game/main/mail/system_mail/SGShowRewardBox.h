//
//  SGShowRewardBox.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-1-2.
//
//

#ifndef __GameSanGuo__SGShowRewardBox__
#define __GameSanGuo__SGShowRewardBox__

#include <iostream>
#include "SGBaseBox.h"
#include "SGShowHeadIconLayer.h"
#define		STORYREWARDTAG          111
#define		STORYREWARDAUTO         112
#define    JINGYANDAN              113
#define    SPBATTLE_PREVIEW        114

class SGShowRewardBox:public SGBaseBox {
private:
    void btncloseBox();
    void initView();
    CCArray *_datas;
    CCSize _boxSize;
    int colomNum;
    int rowNUm;
    int entryTrpe;
    bool showTaskGuide;
public:
    static SGShowRewardBox *create(SGBoxDelegate *target,CCArray *data,int num=4,int entryTrpe = 0);
    SGShowRewardBox();
    ~SGShowRewardBox();
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};




#endif /* defined(__GameSanGuo__SGShowRewardBox__) */
