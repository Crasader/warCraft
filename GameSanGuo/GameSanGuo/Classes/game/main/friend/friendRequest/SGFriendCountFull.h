//
//  SGFriendCountFull.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#ifndef __GameSanGuo__SGFriendCountFull__
#define __GameSanGuo__SGFriendCountFull__

#include <iostream>
#include "SGBaseBox.h"
class SGFriendCountFull : public SGBaseBox
{
private:

    void constructView();
    
    void buttonClickExpand();
    void buttonClickManage();
    void buttonClickCancel();
    
public:
    SGFriendCountFull();
    ~SGFriendCountFull();
    static SGFriendCountFull *create(SGBoxDelegate *boxDelegate);

};

#endif /* defined(__GameSanGuo__SGFriendCountFull__) */
