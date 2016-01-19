//
//  SGBattleCallFriendBox.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-5-8.
//
//

//-------------------------------------------
// 战斗界面，点击呼叫友军按钮，弹出界面
//-------------------------------------------

#ifndef __GameSanGuo__SGBattleCallFriendBox__
#define __GameSanGuo__SGBattleCallFriendBox__

#include <iostream>
#include "SGBaseBox.h"
class SGBattleCallFriendBox : public SGBaseBox
{
private:
    bool m_bCanUp;
    int m_nFriendHeadId;
    int m_nFriendLevel;
    void buttonClickCallFriend(CCObject *object);
    
public:
    //初始化
    SGBattleCallFriendBox();
    ~SGBattleCallFriendBox();
    static SGBattleCallFriendBox *create(SGBoxDelegate *delegate,int friendHeadId,int friendLevel,int canUp);
    void viewDidLoad();
    void boxClose();
    
};

#endif /* defined(__GameSanGuo__SGBattleCallFriendBox__) */
