//
//  SGBMatchEndBox.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#ifndef __GameSanGuo__SGBMatchEndBox__
#define __GameSanGuo__SGBMatchEndBox__

/**
 * 速配成功后显示的PK双方信息
 **/

#include "SGBaseBox.h"
#include "SGRankPlayer.h"

class SGBMatchEndBox : public SGBaseBox
{
public:
    SGBMatchEndBox(void);
    ~SGBMatchEndBox(void);
    
    static SGBMatchEndBox* create(SGBoxDelegate *delegate);
    bool init(SGBoxDelegate *delegate);
    
private:
    void initView();
    void cancelAction(); // 取消响应事件
    void resetMainTouch(bool touch);
    void resetTime(); //刷新时间
    
public:
    void setSelfData(SGRankPlayer *player); // 设置自己的数据
    void setArmyData(SGRankPlayer *player); // 设置对手的数据
    
private:
    SGBoxDelegate *t_delegate;
    
    SGCCLabelTTF *title;
    
    CCSprite *armyIcon;
    CCSprite *selfIcon;
    CCSprite *selfIconBg;
    CCSprite *armyIconBg;
    
    SGCCLabelTTF *armyName;
    SGCCLabelTTF *selfName;
    SGCCLabelTTF *armyRank;
    SGCCLabelTTF *selfRank;
    SGCCLabelTTF *armyFTime;
    SGCCLabelTTF *selfFTime;
    SGCCLabelTTF *armyLevel;
    SGCCLabelTTF *selfLevel;
    SGCCLabelTTF *armyWinR;
    SGCCLabelTTF *selfWinR;
    SGCCLabelTTF *selfservice;
    SGCCLabelTTF *armyservice;
    
    SGCCLabelTTF *label;
    SGCCLabelTTF *timeLabel;
    int cTime;
};

#endif /* defined(__GameSanGuo__SGBMatchEndBox__) */
