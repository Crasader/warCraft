//
//  SGFriendMailCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#ifndef __GameSanGuo__SGFriendMailCell__
#define __GameSanGuo__SGFriendMailCell__

#include <iostream>
#include "SNSTableViewCellItem.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SGButton.h"
#include "SGMailDataModel.h"
#include "SGCCLabelTTF.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SGFriendMailCell : public SNSTableViewCellItem
{
private:
    SGCCLabelTTF *labelNickName;  //昵称
    SGCCLabelTTF *labelSendTime;  //发件时间
    SGCCLabelTTF *labelContent;   //邮件内容缩略
    SGButton *spriteHead;       //好友头像
    CCSprite *spriteUnread;     //未读
    SGButton *buttonDelete;     //删除按钮
    
    SGMailDataModel *_mailData;

    bool _isDeleteModel;
    
    void viewDidLoad();
    
    void buttonClickDeleteMail();
    
    
public:
    SGFriendMailCell();
    
    ~SGFriendMailCell();
    
    static SGFriendMailCell *create(bool isDeleteModel,SGMailDataModel *mailData);
    
    //更新数据
    void update(bool isDeleteModel,SGMailDataModel *mailData);
    
};


#endif /* defined(__GameSanGuo__SGFriendMailCell__) */
