//
//  SGSystemMailCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#ifndef __GameSanGuo__SGSystemMailCell__
#define __GameSanGuo__SGSystemMailCell__

#include <iostream>
#include "SNSTableViewCellItem.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SGButton.h"
#include "SGMailDataModel.h"
#include "SGCCLabelTTF.h"
#include "SGMailSystemDataModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SGSystemMailCell : public SNSTableViewCellItem
{
private:
    SGCCLabelTTF *labelNickName;  //昵称
    SGCCLabelTTF *labelSendTime;  //发件时间
    CCSprite *spriteUnread;     //未读
    SGCCLabelTTF *labelContent;   //邮件内容缩略
    CCSprite *spriteAccessory;  //附件
    SGButton *buttonDelete;     //删除按钮
    
    SGMailSystemDataModel* _mailData;  //邮件体
    
    bool _isDeleteModel;
    
    void viewDidLoad();
    
    void buttonClickDeleteMail();
    void deleteMailListener(CCObject *object);
    
public:
    SGSystemMailCell();
    
    ~SGSystemMailCell();
    
    static SGSystemMailCell *create(bool isDeleteModel,SGMailSystemDataModel *mailData);
    
    //更新数据
    void update(bool isDeleteModel,SGMailSystemDataModel *mailData);
    
    //不管英文汉字，截图这个字符串的前size个字返回
    static std::string getFirstTenWords(const char *charBuffer,int size);

    
};

#endif /* defined(__GameSanGuo__SGSystemMailCell__) */
