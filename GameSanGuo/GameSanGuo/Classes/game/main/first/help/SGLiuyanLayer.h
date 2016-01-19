//
//  SGLiuyanLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-1-6.
//
//

#ifndef __GameSanGuo__SGLiuyanLayer__
#define __GameSanGuo__SGLiuyanLayer__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGCCTextFieldTTF.h"
#include "cocos-ext.h"
#include "MYtextfieldttf.h"
USING_NS_CC_EXT;





class SGLiuyanLayer:public SGBaseLayer,public CCEditBoxDelegate
{
    
    
public:
    SGLiuyanLayer();
    ~SGLiuyanLayer();
    static SGLiuyanLayer *create();
    void initView();
    void backHandler();
    //设置四个点哪个亮哪个不亮
    void setWhiteAndDark(CCNode*node);
    ////触摸也有效而使用的
    void setWhiteAndDarktouch(int btntype);
    /////设置选中按钮外观的来回切换
    void changestate(int tag);
    void submit();
    void sbumitsuccess(CCObject*obj);
    long getCurrentTime();

    
    
    //edit box
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    


    
private:
    /////四个按钮
    SGButton* quexian;
    SGButton* jianyi;
    SGButton*jubao ;
    SGButton* chongzhi;
    
    CCEditBox*  m_pEditTitle;
    SGCCTextFieldTTF*m_pTextField;
    
    MYtextfieldttf *m_pTextFieldqq;
    
    
    int now_tag;
    
    ////四种问题本体
    SGCCLabelTTF*label1;
    SGCCLabelTTF*label2;
    SGCCLabelTTF*label3;
    SGCCLabelTTF*label4;
    
    CCTextFieldTTF*ttf;
  
    
protected:
   // virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
    
};
#endif /* defined(__GameSanGuo__SGLiuyanLayer__) */
