//
//  SGGetFindCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGGetHelpCell__
#define __GameSanGuo__SGGetHelpCell__

#include "SGStoryBaseCell.h"
#include "SGStoryScrollLabel.h"
#include "SGPveHelper.h"


class Itembtndelegate
{
public:
    virtual void itemselect(SNSIndexPath* indexpath){};
};


class SGFindHelpCell : public SNSTableViewCellItem
{
private:
    void viewDidLoad();
//    void buttonClickHeadPic();
    void drawStars(int count);
public:
    SGFindHelpCell();
    ~SGFindHelpCell();
    /////////////////
    static SGFindHelpCell* create(Itembtndelegate *delegate);
    /////////////////
    void setData(SGPveHelper* hepler);
    void setIsCanTouch(bool isTouch);
    void addBtn(SGButton *button);
    void buttonClickHeadPic();
    
private:
    SGGamePlayer *_player;
    CCSize bgSize;
    CCMenu *menu1;
    SGButton *itemHeadPic1;
    int roleID;
    CCSprite *spriteIcon;       // 援军主将星级头像
    CCSprite *spriteIconBg;     // 援军主将星级头像框
//    CCSprite *spriteFriType;    // 援军类型 友军/义军
    SGCCLabelTTF *labelHLevel;    // 援军等级
    SGCCLabelTTF *labelCLevel;  // 武将等级
    SGCCLabelTTF *labelFriName;   // 援军昵称
//    SGCCLabelTTF *labelReward;    // 奖励说明
    bool _isTouch;
    CCSprite *spriteStar;   // 援军主将星级
    void buttonClickOK(CCObject *sender);
    Itembtndelegate *delegate;
    
};



#endif /* defined(__GameSanGuo__SGFindHelpCell__) */
