/*
**SGPvpFightRewardLayer.h
**by:zyc
**2014-09-23 21:40
**
**pvp战斗结束结算页面
**
*/

#ifndef __GameSanGuo__SGPvpFightRewardLayer__
#define __GameSanGuo__SGPvpFightRewardLayer__

#include "cocos2d.h"
#include "SGBaseLayer.h"

class SGCCLabelTTF;


class SGPvpFightRewardData : public CCObject
{
public:
    CC_SYNTHESIZE(int , m_shengWang , ShengWang);//获得声望值
    CC_SYNTHESIZE(int , m_score , Score);//获得天梯积分
    CC_SYNTHESIZE(int , m_rank , Rank);//预计天梯排名
    CC_SYNTHESIZE(bool , m_isWin , IsWin);//战斗胜利
    CC_SYNTHESIZE(int , m_joinCount , JoinCount);//当日参加pvp次数
    
    SGPvpFightRewardData()
    : m_shengWang(0)
    , m_score(0)
    , m_rank(0)
    , m_isWin(false)
    , m_joinCount(0)
    {
    }
};



//***********************************************************************



class SGPvpFightRewardLayer : public SGBaseLayer
{
public:
    static SGPvpFightRewardLayer * create(SGPvpFightRewardData * );
    
    ~SGPvpFightRewardLayer();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
protected:
    
private:
    /*需要的数据*/
    SGPvpFightRewardData * m_rewardData;
    
    /*胜利或失败的图片*/
    CCSprite * m_winOrLost;
    
    /*6个宝箱，达到某个阶段时 播放动画后换亮图,*/
    CCSprite * m_giftBox[6];
    
    /*天梯声望*/
    SGCCLabelTTF * m_shengWangNum;
    
    /*天梯积分*/
    SGCCLabelTTF * m_scoreNum;
    
    /*天梯排名*/
    SGCCLabelTTF * m_rankNum;
    
    /*点击屏幕继续的图片字*/
    CCSprite * m_clickToContinue
    
    /*进度指示条 指示条终点位置*/;
    CCSprite * m_progressIndex;
    CCPoint m_finalPoint;
    
    
    /*进度填充图片*/
    CCSprite * m_progressInside;
    
    /*进度动画*/
    CCProgressTimer * m_updateProgress;
    
    /*存储runAction 对象的集合 当用户点击后，停止所有action*/
    CCArray * m_objectRunningAction;
    
    /*动画是否播放完毕 , 用户点击时 若未播放完毕 则停止播放所有动画，若已经播放完毕  则跳转页面*/
    bool m_isActionAllDone;
       
    /*进度填充色增长百分比*/
    float m_percent;
    float m_maxPercent;
    
    /*ctor*/
    SGPvpFightRewardLayer();
    /*初始化ui*/
    void initView(SGPvpFightRewardData *);
    /*是否显示Facebook分享 及其 按钮的回调*/
    void showFBShare(bool isShow);
    void onFBBtnClicked();
    
    /*显示最终展示的界面*/
    void showFinalUI();
    
    /*各种动作的回调 ，胜利失败最先run*/
    void labelAction();
    void updateAfterAllAction();
    void finalAciotn();
    
    /*更新宝箱状态*/
    void updateGiftBox();
    
    /*播放音效的回调*/
    void playSoundCallback(CCObject * );
    
    /*当日是否已超过15场*/
    bool m_isOverFifteen;
};



//file end.
#endif __GameSanGuo__SGPvpFightRewardLayer__