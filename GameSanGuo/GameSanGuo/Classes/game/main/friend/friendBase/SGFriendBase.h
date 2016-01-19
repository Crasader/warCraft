//
//  SGFriendBase.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-7.
//
//

#ifndef __GameSanGuo__SGFriendBase__
#define __GameSanGuo__SGFriendBase__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGGamePlayer.h"
#include "SGGiftPower.h"
#include "SGMainManager.h"
#include "SGFriend.h"
#include "SGPlayerInfo.h"
#include "SGFriendCellBase.h"
#include "SGCCLabelTTF.h"

class SGFriendBaseDelegate
{
public:
    virtual void cellCallBack(SGGamePlayer *player , bool type){};
    virtual void giftCellCallBack(SGGiftPower *player , bool type){};
    virtual void updateData()=0;
};


class SGFriendBase :public SGBaseTableLayer , public SGFriendBaseDelegate {
    
protected:
    CCSprite *spriteTitle;
    SGCCLabelTTF *labelOnline;
    SGButton *buttonBack;
    SGCCLabelTTF *labelCenterTip;
    bool _touch;
 
    
private:
    
    void constructView();
    void buttonClickGoBack();
    
    void addFriendListener(CCObject *obj);
    void showPlayerInfoListener(CCObject *obj);
    
public:
    void agreeFriendListener(CCObject *obj);
    void refuseAddFriendListener(CCObject *obj);
    
    virtual void refreshData(int roleId); //刷新数据
public:
    SGFriendBase();
    ~SGFriendBase();
    
    virtual bool init(const char *fileName_, LayerTag tag_, bool isSpriteFrame = true);
    
    virtual void cellCallBack(int roleId , bool type){};
    
    void shownoFriendTip(const char *tips);
    
    virtual void updateData(){};
    
    virtual void setIsCanTouch(bool isTouch);
    
    //记录添加的好友id
    static int addRoleId;
    
    int _selectRoleId;
};

#endif /* defined(__GameSanGuo__SGFriendBase__) */
