//
//  SGFirstLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#ifndef __GameSanGuo__SGFirstLayer__
#define __GameSanGuo__SGFirstLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "CCMenu.h"
class CCSpriterX;
class SGTaskObj : public CCObject
{
    CC_SYNTHESIZE(int, m_taskid, taskId);
    CC_SYNTHESIZE(int, m_taskcount, taskCount);
    CC_SYNTHESIZE(int, m_gold, gold);
    CC_SYNTHESIZE(int, m_isAward,isAward);
public:
    SGTaskObj();
    ~SGTaskObj();
};
using namespace cocos2d::extension;

class SGFirstLayer : public SGBaseLayer,public CCMenuMoveDelegate
{
private:
    //下面一排按钮
//    CCSprite *spriteFriendCount;
//    CCLabelTTF *labelFriendCount;
    CCSprite *spriteMailCount;
    CCLabelTTF *labelMailCount;
//    CCSprite *spriteChatCount;
//    CCLabelTTF *labelChatCount;
    CCPoint firstTouchPos;
    //6个按钮
    
    SGButton *currTaskButton;
    SGButton *limitActiveButton;
    SGButton *strengthButton;
    SGButton *advanceButton;
    SGButton *daguanButton;
    SGButton *netbattleButton;
    SGButton *visitButton;
    SGButton *lingjiangButton;
    SGButton *mailButton;
    SGButton *fubenButton;
    SGButton *shouJinZhouButton;
//    SGButton *buzhenBt;
    //原布阵按钮位置更改为武将按钮-2014-11-95
    SGButton * officerBtn;
    SGButton * rankBtn;
    SGButton *bossButton;
    //特殊战斗
    SGButton *specialBattleButton;
    
//    SGButton *friendButton;
//    SGButton *chatButton;
//    SGButton *helpButton;
    
//	CCSprite *embattleNotice;//有新武将格子,叹号提示
	
    //8个对号
    CCSprite *spriteEverydayFinish_1;
    CCSprite *spriteEverydayFinish_2;
    CCSprite *spriteEverydayFinish_3;
    CCSprite *spriteEverydayFinish_4;
    CCSprite *spriteEverydayFinish_5;
    CCSprite *spriteEverydayFinish_6;
    CCSprite *spriteEverydayFinish_7;
    
    CCSprite *spr_notice;
    //每日任务的红点提示
    CCSprite *spr_noticeDayTask;
    CCSprite *spr_noticeBuZhen;
    CCSprite *spr_noticactivity_gift;
    CCSprite *left;
    CCSprite *rgt;
    //武将相关
    CCSprite * spr_notice_officer;
    
    int realwidth;
    
    long long  m_startMoveInterval;
    long long  m_startDragInterval;
    CCPoint  m_scrollStartVector;
    CCPoint  m_scrollLastVector;
    float m_nowPageTop, m_nowPageBottom, m_nowPageLeft, m_nowPageRight;
    float ItemShowRect;
    float ItemShowPosX;
    float ItemInitPosX;
    CCSize s;
    float moveDist;
    bool isMoveRight;
    float moveAddDist;
    float moveStartPosX;
    CCSprite *liangDian_1;
    CCSprite *liangDian_2;
    CCSprite *liangDian_3;
    CCSprite *liangDian_4;
    CCSprite *liangDian_5;
    CCSprite *liangDian_6;
    CCSprite *liangDian_7;
    float showScale;
    float layerRate;
    
 
private:
    void buttonClickChatAndMakeFriend();
    void taskHandlerSend();
    void buttonClickStrengthenShow();
    void buttonClickOfficer();
    void buttonClickGeneralsShow();
    void buttonClickMailShow();
    void showRewardsLayer();
    void fubenHandler();
    
   

    void moveFirstItems(CCPoint & newPos);
    void updateItemPos(float dt);
    
    void activityanimat();
    void setMoveBtAbles(bool enable = true);
public:
    CCScrollView *scrollview;
public:
    SGFirstLayer();
    ~SGFirstLayer();
    
    static SGFirstLayer *create(const char *bgFile, LayerTag tag_);
    virtual void initMsg();
    void setMailCount(int count);
    virtual void setIsCanTouch(bool isTouch);
    void setFriendCount(int count);
    virtual void dealMenuMove(CCTouch *pTouch,CCNode *selectedMenuItem);
    virtual void dealMenuMoveEnd(CCTouch *pTouch,CCNode *selectedMenuItem);
    
    void pressMask();
    void gotoEmbattleLayer();

    //试图获取守荆州主界面信息
    void sjzInfoRequest();
    
    void resetMoveBtns(int buttonId);
//    void rankListener(CCObject*obj);
    
    void showAddAttributeEffect(int blood, int speed, int compbatle);
    void showEffectLabel();
    void removeEffectLabel();
    void updateNoticeBuZhen();
    void bossHandler();
    
    //特殊副本战斗入口
    void specialBattleHandler(CCObject *obj);
//    void updateBestEquipShow(bool isvisible);
//    void updateCanStrengSoldier();
protected:
    void scrollViewDidScroll();
    void testStoryLayer();
    
    
    //商城和排行互换位置
    void onShopBtnClicked();
public:
    void showFirstByTag(int tag);//tag=0 ,闯关，1 副本，2讨魔...
    
private:
    //10级之前在闯关按钮上播放动画。
    void animateOverCallback();
    void animateRePlay();
    
    void rankBtnClicked();
};
#endif /* defined(__GameSanGuo__SGFirstLayer__) */
