//
//  SGGeneralBox.h
//  GameSanGuo
//
//  Created by lizi on 13-5-23.
//
//

#ifndef __GameSanGuo__SGGeneralBox__
#define __GameSanGuo__SGGeneralBox__

#include "SGBaseBox.h"

class SGGeneralBox : public SGBaseBox
{
public:
    SGGeneralBox(void);
    ~SGGeneralBox(void);
    
    static SGGeneralBox* create(SGBoxDelegate *delegate, int type, SGOfficerCard *card = NULL);
    bool init(SGBoxDelegate *delegate, int type);
    
    void setLdIndex(int index);
    
private:
    void initView(SGOfficerCard *card);
    void resetMainTouch(bool touch); // 重置地层Layer的响应
    
    void initUpdateBox(SGBoxDelegate *delegate); //升级界面
    
    void closeAction(CCObject *sender);         // 关闭box
    void addFriendAction(CCObject *sender);     // 添加好友
    void openLianDanAction(CCObject *sender);   // 开启炼丹
    void liandanListener(CCObject *obj);        // 炼丹界面
    void getFriendOfficerListener(CCObject *sender);
private:
    int _type;
    SGBoxDelegate *t_delegate;
    
    int ldIndex; //炼丹数据
};

#endif /* defined(__GameSanGuo__SGGeneralBox__) */
