//
//  SGGuideManager.h
//  GameSanGuo
//
//  Created by geng on 13-5-6.
//
//

#ifndef __GameSanGuo__SGGuideManager__
#define __GameSanGuo__SGGuideManager__

#include "cocos2d.h"
#include "SGUIGuideLayer.h"

#include "SGFightGuideLayer.h"
#include "SGStorySpeakerLayer.h"
#include "SGFourGuideLayer.h"
#include "SGStoryBean.h"
#include "SGAutoGuideLayer.h"
#include "SGLimitGuideLayer.h"



#define NEEDALERT               (0)    //1需要验证弹框，0不需要
#define NEEDENEMYVALIDATION     (1)    //1需要对方战斗数据，0不需要
#define NEEDFIGHTLOG            (1)    //1需要发送315验证log，0不需要

#define UIG_MAX1 54                    //目前最多到达53，强制引导
#define UIG_MAX2 60
#define UIG_BTNS 47

USING_NS_CC;


enum LIMIT_GUIDE
{
    LIT_GUID_SOLDIER_STRENG   = 9,
    LIT_GUID_SUMMON_OFFICER   = 10,
    LIT_GUID_OFFICER_ADVANCED = 11,
    LIT_GUID_EUIQP_ADVANCED   = 12,
    LIT_GUID_ACTIVITY_CHARGE  = 16,
    LIT_GUID_ENETR_AREAN      = 18
};






class SGGuideManager: public CCObject
{
private:
    SGFightGuideLayer *fightGuideLayer;
    SGAutoGuideLayer *autoGuideLayer;
    SGStorySpeakerLayer *storySpeaker;
    SGFourGuideLayer *fourGuide;
    SGStoryBean *_story;

private:
    SGGuideManager(void);
	~SGGuideManager();
    void initData();
    void openGuide(bool open);
    
    CCRect getRectByGuideId(int ggid);
    GameIndex gameIndex;
    std::vector<int> m_limitGuideList;  // -1默认,1限时副本,2精英副本,3对战,4守荆州
public:
    bool isGuide;
    int step;
    int autoStep;
    int actionType;
    bool isFightDesc;
    SGUIGuideLayer *uilayer;
    SGLimitGuideLayer *limitlayer;
    int limitStartStep;
    int limitStep;
    bool isWanXDL;    //行动力位置指在完按钮上

    //判定阶段引导的达成条件是否足够
    //@checkId 对应triggerdata中的id（哪个limitGuide引导）
    bool conditionEnoughForLimitGuide(int checkId);
    
public:
    int getCurrentGuideId();
    
    static SGGuideManager *shareManager();
    void showSpeaker(int index, SGBoxDelegate *delegate);
    SGUIGuideLayer* getGuideLayer();
    void changeRound(bool isme);
    void checkIsDone(int gid);
    void closeLimitGuide();
    void closeGuide();
    void dismissLayer();
    
    void startFightGuide(int from = 0);
    void startAutoGuide(int from = 0);
    void setFightGuideLayer(SGFightGuideLayer *layer);
    void setAutoGuideLayer(SGAutoGuideLayer *layer);
	
	//检测进度并向服务器发送消息
	void checkAndSendProcessMsg(int guideTag);
	
	void stepListener();

    SGAutoGuideLayer* getAutoGuideLayer();
    SGFightGuideLayer* getFightGuideLayer();
    CCPoint getPoint();
    bool getIsChangeRound();
    // story speaker
    void closeSpeaker();
    void showStorySpeaker(int plotId, SGBoxDelegate *delegate, SGStoryBean *bean);
    void resetGuideId(int gid);
    
    void startUiGuide(int guideId);
    void showFourGuide();
    void setMainTouchEnable(bool enable);
    
    void showPKWords(CCString *word, SGBattleLayer *layer);
    
    void showTaskWork();//显示任务闪烁
    
    int getLimitGuideSize();
    void setLimitGuideTag(int limitStart);
    void startLimitGuide(int startStep = 11);
    void clearGuideLayer();
    //检查大小包&标准资源更新是否完成。
    void checkIsAllResouce();
public:
    bool isDisMiss;
    bool isShowTask;
};
#endif /* defined(__GameSanGuo__SGGuideManager__) */
