//
//  SGQFightBox.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-16.
//
//

#ifndef __GameSanGuo__SGQFightBox__
#define __GameSanGuo__SGQFightBox__

#include "SGBaseBox.h"

class SGQFightBox : public SGBaseBox
{
public:
    SGQFightBox(void);
    ~SGQFightBox(void);
    
    static SGQFightBox* create(SGBoxDelegate *delegate, int index);
    void setDatas(int pid, int rid, int sid,bool canJumpSection,int power,int times);
    
private:
    void initView();
    void skipFightAction();   // 跳过战斗
     void tenSkipFightAction();   // 十次跳过战斗
    void twoSkipFightAction();   // 两次跳过战斗
    void enterFightAction();  // 进入战斗
    void resetMainTouch(bool touch); // 重置地层Layer的响应
    void closeBox(); // 关闭box
    
private:
    int index;  // cell index
    int plotId;
    int storyId;
    int roleId;
    bool canJump;
    int m_nPerTimePower;
    int m_nTimes;//可以挑战的次数,精英关卡需要
    SGBoxDelegate *t_delegate;
    int m_nSaodangVipLevel;
    int m_nTenSaodangVipLevel;
    int m_nTenSaodangPlayerLevel;
};
class SGShadeBox : public SGBaseBox
{
public:
    SGShadeBox(void);
    ~SGShadeBox(void);
    
    static SGShadeBox* create(SGBoxDelegate *delegate, int index);
    //void setDatas(int pid, int rid, int sid,bool canJumpSection);
    
private:
    void initView();
    void resetMainTouch(bool touch);
    void showTitleDot();
    
private:
        SGBoxDelegate *t_delegate;
        SGCCLabelTTF* titleDot1;
        SGCCLabelTTF* titleDot2;
        SGCCLabelTTF* titleDot3;
        int m_showDotCount;
};

class SGShadeBox2 : public SGBaseBox
{
public:
    SGShadeBox2(void);
    ~SGShadeBox2(void);
    
    static SGShadeBox2* create(SGBoxDelegate *delegate, int index);
    //void setDatas(int pid, int rid, int sid,bool canJumpSection);
    
public:
    void initView();
    void resetMainTouch(bool touch);
    void showTitleDot();
    
private:
    SGBoxDelegate *t_delegate;
    SGCCLabelTTF* titleDot1;
    SGCCLabelTTF* titleDot2;
    SGCCLabelTTF* titleDot3;
    int m_showDotCount;
};
#endif /* defined(__GameSanGuo__SGQFightBox__) */
