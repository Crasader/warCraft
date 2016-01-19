//
//  SGOverBattleBox.h
//  GameSanGuo
//
//  Created by lizi on 13-5-17.
//
//

#ifndef __GameSanGuo__SGOverBattleBox__
#define __GameSanGuo__SGOverBattleBox__

#include "SGBaseLayer.h"
#include "SGStoryReward.h"
#include "CCSpriterX.h"

class SGPvpFightRewardData;
class SGArenaFightRewardData;
 

class SGOverBattleBox : public SGBaseLayer
{
public:
    SGOverBattleBox(void);
    ~SGOverBattleBox(void);
    
    static SGOverBattleBox* create(SGBoxDelegate *delegate, SGStoryReward *reward,int isRunAway = 0 , bool isShowExoticMerchant=false);
    
    /*新 pvp战斗结束*/
    static SGOverBattleBox * create(SGBoxDelegate *delegate , SGPvpFightRewardData * pvpRewardData);
    
    //竞技场战斗结束
    static SGOverBattleBox * create(SGBoxDelegate *delegate , SGArenaFightRewardData * arenaRewardData);
private:
    SGPvpFightRewardData * m_pvpRewardData;
    SGArenaFightRewardData * m_arenaRewardData;

private:
    void initView(bool isWin);
    void closeBox();
    void playAnims();
    void resetMainTouch(bool touch);
    void goonAction();
	
	void playWinAnim();//胜利动画
    void setWinBgVisible();//设置背景光效可见
	
    //播放循环胜利动画
    void playAnim98();
    
private:
    SGStoryReward *_reward;
    SGBoxDelegate *t_delegate;
    
	CCSprite *winLetter;//“胜利“字体
	CCSprite *winBg;//背景光特效
	
    //循环播放的胜利/失败动画
    CCSpriterX* anim98;
    int m_isRunAway;
    
    //是否提示西域商人
    bool m_isShowExoticMerchant;
};

#endif /* defined(__GameSanGuo__SGOverBattleBox__) */
