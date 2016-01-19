//
//  SGActivityMainCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGActivityMainCell__
#define __GameSanGuo__SGActivityMainCell__

#include "SGStoryBaseCell.h"
#include <ctime>
#include "SGStoryBean.h"
#include "SGActivityMainLayer.h"

class SGActivityMainCell : public SGStoryBaseCell
{
private:
    CCSprite *spriteIsPass;
    CCLabelTTF *labelName;
    SGCCLabelTTF *labelDesc;
    SGCCLabelTTF *labelTime;
    SGCCLabelTTF *m_labelChallengeTimes;
    int activityId; // 活动ID
    int limit_time; // 剩余时间, 秒
    bool isStartActivity;
    bool isCountClose;
    /////
    SGActivityMainCellDelegate *delegate;
    SNSTableView *tableview;
    SGStoryBean *activityBean;
private:
    void viewDidLoad();
    void flashTimer(); // 刷新时间描述
    void buttonClick();
public:
    SGActivityMainCell();
    ~SGActivityMainCell();
    
    static SGActivityMainCell* create(SGActivityMainCellDelegate *del,SNSTableView*table);
    void setData(SGStoryBean *bean);
    static  long getCurrentTime();
 
};

#endif /* defined(__GameSanGuo__SGActivityMainCell__) */
