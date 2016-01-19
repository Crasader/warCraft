//
//  SGFriendCellBase.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-7.
//
//

#ifndef __GameSanGuo__SGFriendCellBase__
#define __GameSanGuo__SGFriendCellBase__

#include <iostream>
#include "SNSTableViewCell.h"
#include "SNSTableViewCellItem.h"
#include "SGButton.h"
#include "cocos-ext.h"
#include "SGGamePlayer.h"
#include "SGFriendBase.h"
#include "SGCCLabelTTF.h"
#include "SGMenu.h"

USING_NS_CC_EXT;

class SGFriendBaseDelegate;
class SGFriendBase;
////////
class SGFriendCellBaseDelegate
{
public:
//  virtual  void buttonClickAgree();
//  virtual  void buttonClickRefuse();
//    virtual void entrySectionHandler(SGBattleFieldItem *giftItem){};
//    virtual void showSectionInfoHandler(SGBattleFieldItem *giftItem){};
};
////////////
class SGFriendCellBase :public SNSTableViewCellItem
{

protected:
    SGButton *itemHeadPic;
    SGCCLabelTTF *labelName;
    SGCCLabelTTF *labelLoginTime;
    SGCCLabelTTF *labelLevel;
    SGButton *buttonAgree;
    SGButton *buttonRefuse;
    SGGamePlayer *_player;
    CCMenu *menu;
    SGFriendBase *_parentLayer;
    SGFriendBaseDelegate *_friendDelegate;
   /////
	CCMenu *menu1;
private:
    
    void constructView();
    void buttonClickHeadPic();
    ////////////////
    void  clickbutnone();

public:
    SGFriendCellBase();
     ~SGFriendCellBase();
    bool viewDidLoad();
    virtual void updateCellBySGamePlayer(SGGamePlayer *player);
    void addBtn(SGButton *button);
    void setIsCanTouch(bool isTouch);
    void setParentLayer(SGFriendBase *layer);
    void setHeadTouchEnable(bool touchEnable);
    void setPlayer(SGGamePlayer *player);

};



#endif /* defined(__GameSanGuo__SGFriendCellBase__) */
