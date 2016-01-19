//
//  SGRewardBindLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-20.
//
//

#ifndef __GameSanGuo__SGRewardBindLayer__
#define __GameSanGuo__SGRewardBindLayer__

#include "SGBaseLayer.h"
#include "SGCCLabelTTF.h"
#include "cocos-ext.h"
#include "SGHttpClient.h"


USING_NS_CC_EXT;
class SGRewardBindLayer : public SGBaseLayer
{
private:
    CCEditBox* m_pEditName;
    CCEditBox* m_pEditPassword;
    CCEditBox* m_pEditPasswordAgain;

    SGCCLabelTTF *lab_iscanget;
    SGCCLabelTTF *lab_notice;
    SGCCLabelTTF *lab_sec;
//    SGButton *btn;
    CCArray *_arry;
    CCArray *_arry2;//邮箱奖励
    int scrollHeight;
    CCLayer *scrollLayer;
    CCScrollView *scroll;
    int entertype;
    SGButton *registerButton;
    CCPoint center;
    SGButton *m_pbuttonGetReward1;
    SGButton *m_pbuttonGetReward2;//邮箱奖励按钮
    int m_nStateEmail;//绑定邮箱状态
    int m_nStateAccount;//绑定账号状态
    CCEditBox *m_pEditBoxEmail;
    CCEditBox *m_pEditBoxEmailAgain;
    SGCCLabelTTF * m_pLabelBindEmailInfo;
    int m_nRequestState;
    const char * m_pStrMailAddress;
    CCString * m_strMail;
    
//    CCArray *scrollArray;
private:
    void initView();
    void initReward(CCArray * arr);
    void OnbindEmail();
    void bindEmailListener(cocos2d::CCObject *obj);
    void getRewardBindEmailListener(cocos2d::CCObject *obj);
    void backHandler();
//    bool checkInputStringChar(const char *inputString);
    void regListener(cocos2d::CCObject *obj);
     CCSprite *title_bg;
    void registerHandler();
    bool is_email_valid(const char* email);
    const char * setTheMail(const char* email);
//    void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
//    
//    void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
//    
//    void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    
    void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
    void requestFinished(int msgid, CCObject *data);
    void requestFailed();
public:
    virtual void setIsCanTouch(bool isTouch);
    SGRewardBindLayer();
    ~SGRewardBindLayer();
    static SGRewardBindLayer *create(CCArray *arry,int type,CCArray *arry2,int type2,const char * mail);
    
};


#endif /* defined(__GameSanGuo__SGRewardBindLayer__) */
