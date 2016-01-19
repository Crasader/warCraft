//
//  SGLogicManager.h
//  GameSanGuo
//
//  Created by Fygo Woo on 12/28/14.
//
//

#ifndef __GameSanGuo__SGLogicManager__
#define __GameSanGuo__SGLogicManager__

#include "cocos2d.h"

USING_NS_CC;

/*
 * 业务管理器，目前主要管理红点提示。
 */
class SGLogicManager : public CCObject
{
//function
public:
    //ctor
    SGLogicManager();
    
    //dtor
    ~SGLogicManager();
    
    //gi
    static SGLogicManager* gi();
    
    //启动业务定时器。
    void startUp();
    
    //红点判定
    void redSpotDetection(float dt);
    
    //设置游戏速率。（用于3倍速自动战斗/延时动作的反向调整等）
    void setGameSpeed(float dt);
    
    //获得游戏速率。
    float getGameSpeed();

private:
    //西域商人红点检查。
    void checkExoMechNotice();
    
    //首页布阵红点检查
    void checkEmbattleNotice();
    
//variable
private:
    //游戏速率，默认为1，即不加速。
    float gameSpeed_;
    
};

#endif /* defined(__GameSanGuo__SGLogicManager__) */
