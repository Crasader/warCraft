//
//  SGSystemMailContent.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-28.
//
//

#ifndef __GameSanGuo__SGSystemMailContent__
#define __GameSanGuo__SGSystemMailContent__

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGCCLabelTTF.h"
#include "SGMailManager.h"
#include "SGCCLabelTTF.h"
#include "SGShowHeadIconLayer.h"
#include "SGShowRewardBox.h"
USING_NS_CC_EXT;

class SGSystemMailContent : public SGBaseLayer
{
private:
    CCLabelTTF *labelSendTime;      //发送时间
    SGCCLabelTTF *labelAccessory;     //附件标题
    CCScrollView *scrollAccessory;  //附件列表
    SGButton *buttonReceiveAcc;     //收取附件按钮
//    SGButton *buttonDelete;         //删除
    CCLabelTTF *labelSender;        //发件人
    
    //邮件内容
    SGCCLabelTTF *labelContent;       
    CCScale9Sprite *sprite_scrollview_angle;
    CCScale9Sprite *sprite_scrollview_bg;
    
    //附件
    CCScale9Sprite *sprite_content_bg_angle;
    CCScale9Sprite *sprite_content_bg_bg;
//    CCScrollView *scrollview;       //滑动控件
    
    SGShowHeadIconLayer *showHeadIconlayer;

    //是否有附件
    bool m_bIsReceived;
    //数据
    SGMailSystemDataModel *m_pSystemMail;
    
    //初始化
    void viewDidLoad();
    //返回
    void buttonClickGoBack();
    //删除
    void buttonClickDelete();
    //收取附件
    void receiveAccessory();
    //收取附件按钮
    void buttonClickAccessory();
    //获取一个sprite
    CCArray* getSpriteByType();
    //监听读取邮件的请求
    void readMailListener(CCObject *object);
    //监听删除邮件请求
    void deleteMailListener(CCObject *object);
    //收取附件监听
    void receiveAccessoryListener(CCObject *object);
    //返回上层界面
    void backToLastLayer();
    
    
public:
    
    SGSystemMailContent();
    ~SGSystemMailContent();
    static SGSystemMailContent *create(SGMailSystemDataModel *systemMail);
    
};

#endif /* defined(__GameSanGuo__SGSystemMailContent__) */
