//
//  SGArenaFightRewardLayer.h
//  GameSanGuo
//
//  Created by angel on 14-12-26.
//
//

#ifndef __GameSanGuo__SGArenaFightRewardLayer__
#define __GameSanGuo__SGArenaFightRewardLayer__

#include "cocos2d.h"
#include "SGBaseLayer.h"


class SGCCLabelTTF;


class SGArenaFightRewardData : public CCObject
{
public:
    CC_SYNTHESIZE(int , m_arenaCoin , ArenaCoin);//
    CC_SYNTHESIZE(int , m_beforeRank ,BeforeRank);//
    CC_SYNTHESIZE(int , m_afterRank ,AfterRank);//
    CC_SYNTHESIZE(bool , m_isWin , IsWin);//战斗胜利
    CC_SYNTHESIZE(int , m_count , Count);//次数
    CC_SYNTHESIZE(int , m_maxCount , MaxCount);//最大次数
    
    SGArenaFightRewardData()
    : m_arenaCoin(0)
    , m_beforeRank(0)
    , m_afterRank(0)
    , m_isWin(false)
    , m_count(0)
    ,m_maxCount(0)
    {
    }
};




class SGArenaFightRewardLayer:  public SGBaseLayer
{
public:
    SGArenaFightRewardLayer();
    ~SGArenaFightRewardLayer();
    static SGArenaFightRewardLayer * create( SGArenaFightRewardData * data);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void initView();
    void playSoundCallback(CCObject * obj);
    void labelAction();
private:
    SGArenaFightRewardData * m_rewardData;
    
    CCSprite  *m_winOrLost;
};

#endif /* defined(__GameSanGuo__SGArenaFightRewardLayer__) */
