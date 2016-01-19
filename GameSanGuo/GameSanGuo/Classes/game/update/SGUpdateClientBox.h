//
//  SGUpdateClientBox.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-18.
//
//

#ifndef __GameSanGuo__SGUpdateClientBox__
#define __GameSanGuo__SGUpdateClientBox__

#include <iostream>
#include "SGBaseBox.h"

//本BOX的类型。
enum SG_UPDATE_CLIENT_BOX_TYPE
{
    SGUCB_FORCE_UPDATE_CLIENT,
    SGUCB_RECOMMEND_UPDATE_CLIENT,
    SGUCB_NETWORK_BUSY,
    SGUCB_RESPONSE_ERR,
    SGUCB_EXIT_GAME, //增加退出游戏的功能。
};

/*
 * 这个BOX现在用作提示强制更新和建议更新，其中强制更新会跳转至APP STORE/GOOGLE PLAY，建议更新则会询问玩家。
 */
class SGUpdateClientBox :public SGBaseBox
{
public:
    //ctor
    SGUpdateClientBox();
    //dtor
    ~SGUpdateClientBox();
    //create
    static SGUpdateClientBox* create(SG_UPDATE_CLIENT_BOX_TYPE type, std::string extArg = "");
    //阻挡其他点击。
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void viewDidLoad();
    
    void buttonClickOK();
    void buttonClickSkipUpdate(CCObject *obj);
    //本次更新是何种类型。
    SG_UPDATE_CLIENT_BOX_TYPE updateType;
    //本次特殊参数。
    std::string _extArg;
};

#endif /* defined(__GameSanGuo__SGUpdateClientBox__) */
