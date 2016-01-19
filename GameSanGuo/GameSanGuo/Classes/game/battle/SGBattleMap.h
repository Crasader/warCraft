//
//  SGBattleMap.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-20.
//
//

#ifndef __GameSanGuo__SGBattleMap__
#define __GameSanGuo__SGBattleMap__

#include "cocos2d.h"
#include "SGGridBase.h"
#include "GameConfig.h"
#include "CCSpriterX.h"
#include "SGLayout.h"

class ShiBing;
class SGAttackList;
class SGHeroLayer;

//cgpSpine
#include "cocos-ext.h"
USING_NS_CC_EXT;

using namespace cocos2d;

struct AttackBackMsg
{
    GameIndex index;
    int blood;
};
/**
 * 战场中所有的点击事件
 */
enum ETouchState
{
    eTouchNull= 0,
    eTouchMove,
    eTouchXuan,
    eTouchFang,
    eTouchEnd,
    eTouchRemove,
    eTouchRemoveIn,
    eTouchDai,
    eTouchSkills, //选择选位型技能的位置中
    eTouchSkillsFang, //技能施法中
};

class SGBattleMap : public CCLayer
{
public:
    CCSkeletonAnimation*   liansuoChar;
    
public:
    SGBattleMap();
    ~SGBattleMap();
    /*
     * 创建地图
     * soldiers 初始地图上的所有兵；
     * isme 是否是自己的地图;
     */
    static SGBattleMap *create(CCArray* soldiers, bool isme);
    /**
     * 填充指定的网格
     */
    virtual bool init(CCArray* soldiers, bool isme);
    /**
     * 注册点击事件的分发
     */
    void registerWithTouchDispatcher();
    /**
     * 事件处理
     */
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void delayChangeSbState(CCNode* psender);
    
    /**
     * 通过格子对象获取对应的士兵
     */
    ShiBing* getShiBingWithGrid(SGGridBase *grid);
    /**
     * 根据index获取网格上的士兵
     */
    ShiBing * getShiBingByIndex(int index);
    
    int getShiBingCount();
    /*
     * 通过格子获取对应的攻击队列
     */
    SGAttackList *getAttackListByGrid(SGGridBase *grid);
    /**
     * 显示闪烁的同时,显示手指指引
     */
	void displayTipsFinger(CCPoint startPos);//11.18
    /**
     * *******************移动完成（这个是核心函数）****************************
     */
    void moveEnd();
    /**
     * 每次删除或者移动士兵后检测是否形成攻击阵列或者防御墙
     */
    void deleteMoveEndDetection();
    /**
     * 初始化点击到的网格
     */
    void initTouchGrid(int list);
    /**
     * 要移动过去的网格
     */
    bool moveTouchGrid(int list);
    /**
     * 初始化兵,通过传过来的数据,初始化到地图上
     */
    void initMapSBs(CCArray *sbs);
    /**
     * 变换回合
     */
    void changeRound( bool isSetTouch, bool ismyRound, bool isFirst = false);
    /**
     * 出兵操作
     */
    void chubing();
    /**
     * 填充指定的网格
     */
    void initGrid(SGGridBase *grid);
    /**
     * 删除攻击阵列
     */
    void removeAttackList(SGAttackList *attackList, SGHeroLayer *hero = NULL, SGGridBase *grid = NULL, bool isMe = false,bool showDrops = true, bool isDelay = false);
    /**
     * 移除攻击散兵
     */
    void removeAttackedSB(ShiBing *sb, int attackv, SGHeroLayer *hero = NULL);
    /**
     * 如果是整合返回true????
     */
    bool chackAttackChain(SGAttackList *attack);
    /**
     * 刷新所有的格子
     */
    void refreshMapGrid( bool isChack);
    /**
     * 获取所有的格子列表
     */
    CCArray * getMapGrid();
    /**
     * 删除散兵对象
     */
    void deleteSbObj(int i, int j, float _t = 0.0);
    /**
     * 返回一个普通攻击阵列的最上的行列
     */
    GameIndex getNormalAttackR(CCArray *array);
    /**
     * 返回所有的士兵
     */
    CCArray *getSoldiers(){return soldiers;};
    /**
     * 英雄攻击时。。。。
     */
    void spgongJiSwapGridObj(CCArray *array);
    /**
     * 添加主将BUFF
     */
    void addHeroBuff(CCDictionary * data);
    /**
     *移除主将BUFF
     */
    void removeHeroBuff();
    /**
     * 激活主将BUFF
     */
    void activateHeroBuff(bool isadd );    
    /**
     * 刷新索引？？？
     */
    void flashIndex(GameIndex index, int flashTag = 1024 ,bool isblue = false);
    /**
     * 删除全部士兵
     */
    void removeSoldiers(bool showDrops = true);
    /**
     * 修复攻击
     */
    void repairAttack(CCDictionary *data, CCArray *array);
    /* *
     *移除散兵
     */
    void backRemoveSB();
    /**
     * 移除替身
     */
    void removeReplaceSoldier();
    /**
     * 设置选散兵位置
     */
    void setXuanSbPos();
    /**
     * 移除散兵数组
     */
    void removeSbAr(CCArray *ar);
    /**
     *
     */
    void setTouchInitSbOpacity(bool scale = true);
    /**
     * 将等待区士兵派到地图上去
     */
    void qzFillUnit(SEL_CallFuncN selector);
    /**
     * remove军旗????
     */
    void removeJQ();
    /**
     *
     */
    void qzEnd();
    /**
     *
     */
    void showMoveArrow();
    /**
     *
     */
    void removeMoveArrow();
    /**
     *
     */
    GameIndex findLastShiBin();
    /**
     *
     */
    GameIndex findFirstShiBin();
    /**
     *
     */
    GameIndex findMiddleShiBin();
	/**
     * 11.18需要在新手引导调用,改为公有
     */
	ShiBing* getShiBingWithIndex(GameIndex index);
    /**
     *
     */
    CCArray *getFourList();
    /**
     *
     */
    CCArray *getTwoList();
    /**
     *
     */
    bool getIsMe(){return isme;};
    /**
     *
     */
    void setIsMe(bool bme){isme = bme;};
    /**
     *
     */
    bool getIsNeedRemoveArrow(){return needremoveArrow;}
    /**
     *
     */
    CCArray *getAttackList();
    /**
     *
     */
    void hideRemindMe();
    /**
     *
     */
    void showDefendName(ShiBing *sb);
    /**
     *
     */
    void setStartShowFriendly(bool _needStartShowFriendly);
    /**
     *
     */
    void setIsMoveAble();
    /**
     *
     */
    void hideFriendlyHelp(bool startCounting=true);

    /**
     *
     */
    void addSkillDelayCount();
    
    /**
     *
     */
    void reduceSkillDelayCount();
    bool normalIsSameColor(int row,int listIndex,int colorId,bool isAtk = false);
    int testFillOne(int listIndex,int &colorId);
    CCPoint testFillTow(int listIndex);
    CCPoint testFillFour(int listIndex);
    void clearFillUnitGrids();
    void initFillUnitGrids();
    void doclientFillOne(SGGridBase * gridBase);
    bool clientFillOne(SGGridBase * gridBase);
    int getCanFillHeroPow();
    int getFillHeroColorId(int (*colorId)[3]);
    int getHeroCountWithoutFriend();

    
    //处理墙的防御力变换
    void processWallDef();
    //处理攻击阵列每回合触发技能释放
    void processPerActivitySkill(bool ismyRound);
    //处理攻击阵列上的静态buff
    void processAttackListStaticBuff();
    //作用在网格上的技能buff激活处理
    void processOnGridBuff();
    //遍历每个攻击阵列,然后计算相关加成(如融合, 锁定)
    void processAttackListChainFuse();
    //处理攻击阵列攻击力增长
    bool processAttackListAtlAdd();
    //处理攻击阵列蓄力完成是要是激活的非相互作用技能
    void processActivitySkill_UnInteract();

    
    
    /**
     * 自动战斗开启的时候，由此处开始控制地图
     */
    void autoBattleTimerStart();
    
private:
    
    /**
     *  延时显示提示信息
     */
    void dispAutoBattleInfo();
    
    /**
     *  设置提示信息是否显示完成有标记
     */
    void setAutoBattleFlag();

    
    /**
     *  对我方地图的单回合的操作结束，类似于initAiWatch函数的作用
     */
    void endSingleRoundOrAddAICheck(bool isStartTimer);
    
    /**
     *  显示信息，调用SG_SHOW_MESSAGE(),（为了动作回调）
     */
    void showInfo();
    /**
     *  先检测是不是完成了未打出去的主将技，再开始AI
     */
    void checkUnCompleteSkillAndBeginAi();
    
    /**
     *  先检测是不是完成了未打出去的主将技
     */
    void callStartSkill();
    /**
     *  检测是不是主将技完成
     */
    void skillIsComplete();

    
    
    /**
     * 判断是否形成防御阵列
     */
    CCArray * chackdefendList(SGGridBase *grid);
    /**
     * 判断是否形成攻击阵列
     */
    CCArray * chackNormalAttackList(SGGridBase *grid);
    /**
     *
     */
    CCArray * chack2AttackList(SGGridBase *grid);
    /**
     *
     */
    void indexAddToArray(SGGridBase *gird, CCArray *array);
    /**
     *
     */
    void removeGrids(CCArray *array);//remove array 中对象
    /**
     *显示菜单
     */
    void showRemoveMenu();//
    /**
     *
     */
    void removeSB();
    /**
     * 删除当前兵对象
     */
    void removeSbObj(ShiBing *sb);
    /**
     *
     */
    void showRemoveEff(cocos2d::CCObject *obj);
    /**
     *
     */
    void callRemoveSb(CCObject * obj, SGGridBase *grid);
    /**
     *
     */
    void deleteSoldiersObj(ShiBing *sb);
    /**
     *
     */
    void detectionAllGrid();
    /**
     *
     */
    bool chackSPMove(ShiBing *bing);
    /**
     * 检测防御时可以动的是否存的
     */
    bool chackFangYuObj(CCArray * array, ShiBing *bing);
    /**
     * 防御时调换对象
     */
    void fangYuSwapGridObj(CCArray *array, bool isRefresh);
    /**
     * 攻击时.....
     */
    void gongJiSwapGridObj(CCArray *array);
    /**
     * 删除时....
     */
    void deleteSwapGridObj(CCArray *array);
    /**
     * 合成防御
     */
    void synthesisDefen(CCArray *array);
    /**
     *
     */
    void  shackDrop(CCArray *array);
    /**
     * 防御时
     */
    CCArray *chackSPDownMove(SGGridBase *gridBegin, SGGridBase * gridEnd, ShiBing *bing, int lie, int rand, SGGridBase* fangSbGrid);
    /**
     * 当前是否可以向下移动rand行 用来换墙用
     */
    CCArray *chackDownMoveIndex(ShiBing *sb, ShiBing *bing, GameIndex index, int rand, int row);
    /**
     * 当前是否可以向下移动rand行 用来换攻击阵列
     */
    CCArray *chackDownMoveIndex2(ShiBing *sb, ShiBing *bing, GameIndex index, int rand, int row);
    /**
     * 当前4格是否可以向下移动动多行
     */
    CCArray *chackSPDownMoveIndex(ShiBing *sp, ShiBing *bing, int lie, int rand);
    /**
     * 向下移动rand行
     */
    void downMoveSBRow(ShiBing *bing, int rand);
    /**
     *
     */
    void addJointlyGrid( ShiBing *bing, CCArray* array);
    /**
     *
     */
    void removeJointlyGrid(ShiBing *bing);
    /**
     *
     */
    void touchGridToList();
    /**
     *
     */
    void moveToBottomEnd();
    /**
     *
     */
    void addBingEnd(CCObject *obj);
    /**
     *
     */
    void chubingEnd(CCObject *obj);
    /**
     *
     */
    void activateSkills4(int num);
    /**
     *
     */
    void upRemoveT();
    /**
     *
     */
    void deleteSbMap();
    /**
     *
     */
    void beginAi();
    /**
     *
     */
    void aiCanStart();
    /**
     *
     */
    void initAiWatch(bool startNew = true);
    /**
     *
     */
    bool aiCanTouchMap();
    /**
     *
     */
    void showRemindMsg();
    /**
     *
     */
    void showFriendlyHelp();
    /**
     *
     */
    CC_SYNTHESIZE(SGHeroLayer*, hero_obj, Hero_Obj);
    /**
     *  给士兵添加可移动标签
     */
    void addMoveFlag(ShiBing *sb);
    /**
     *   检测士兵是不是这列最后一个
     */
    bool detectionIsLastSoldier(ShiBing *sb);
    /**
     *   添加觸摸士兵的特效
     */
    void  addTouchSoldierEffects();

    /**
     *   检查士兵能不能下移
     */
    void  checkSBCanDown(SGGridBase *gridBegin, SGGridBase *gridEnd,SGGridBase * grid,ShiBing *bing,CCArray *sbarray);
private:
    
    
    /**
     * 剩余兵数量
     */
    int chubingnum;
    /**
     *
     */
    bool isme;
    /**
     *
     */
    bool m_battleInitpve;
    /**
     *
     */
    int battleType;
    /**
     *
     */
    int m_battleCurstoryId ;
    /**
     *
     */
    bool m_isGuide;
    /**
     *
     */
    int myGridsReady[mapRow][mapList];
    /**
     *
     */
    bool needStartShowFriendly;
    /**
     * 点击的位置
     */
    CCPoint m_clickPos;//
    /**
     * 点击间隔时间
     */
    int m_clickTime;//
    /**
     *
     */
    bool isRemovebegin;
    /**
     *
     */
    bool needremoveArrow;
    /**
     * 当前点击的选择的格子
     */
    SGGridBase *m_touchGrid;//
    /**
     * 点击位置行列
     */
    GameIndex m_touchPosIndex;//
    /**
     *
     */
    GameIndex m_touchSkillsIndex;
    /**
     *
     */
    GameIndex m_touchInitIndex;
    /**
     *
     */
    ShiBing *selectSB;
    /**
     *场景上的所有士兵
     */
    CCArray *soldiers;
    /**
     * 等待区的所有士兵（未上场）
     */
    CCArray *waitSoldiers;
    /**
     * 形成的攻击阵列
     */
    CCArray *attackLists;
    /**
     * 点的行列
     */
    GameIndex m_touchIndex;
    /**
     *
     */
    ShiBing *touchInitSb;
    /**
     * 防御阵列上面要移动的兵
     */
    CCArray *m_fangYuUpBingArray;
    /**
     * 攻击阵列上面的移动的兵
     */
    CCArray *m_nomalAttackUpBingArray;
    /**
     * 英雄阵列上的兵
     */
    CCArray *m_spAttackUpArray;
    /**
     * 存储删除块以下的兵
     */
    CCArray *m_deleteDownArray;
    /**
     * 影响的所有格子
     */
    CCArray *m_moveAllGrid;
    /**
     * 共用格子
     */
    CCArray *m_jointlySbArray;
    /**
     * 复制共用格
     */
    CCArray *m_jointlySbArrarCopy;
    /**
     * 要合成的格子
     */
    CCArray *m_synthesisGrid;
    /**
     *组成4格英雄的兵
     */
    CCArray *m_spAttackGrid;
    /**
     *
     */
    CCArray *m_moveSanGrid;
    /**
     *
     */
    CCArray *m_removeSbObj;
    /**
     *
     */
    CCArray *m_removeSbDef;
    /**
     * 是否显示非活动士兵蒙版
     */
    bool   m_isShowMask;
    /**
     *
     */
    CCSprite *sbCopy;
    /**
     *
     */
    bool isAddGrid;
    /**
     *
     */
    bool isAddGrid1;
    /**
     *
     */
    bool isAddGrid2;
    /**
     *
     */
    bool isTouch;
    /**
     *
     */
    bool isMoveOperation;
    /**
     *
     */
    bool isDeleteOperation;
    /**
     *
     */
    bool isSynthesis;
    /**
     *
     */
    bool isOperation;
    /**
     *
     */
    int moveToX;
    /**
     *
     */
    int moveToY;
    /**
     *
     */
    int fromX;
    /**
     *
     */
    int tox;
    /**
     *
     */
    
    int moveNum;
    /**
     *
     */
    int addNum;
    /**
     *
     */
    float _dlEfTime;
    /**
     *
     */
    CCSpriterX *touchSpXbj;
    /**
     *
     */
    CCSpriterX *touchSpXbj_;
    /**
     *
     */
    CCProgressTimer *left;
    /**
     *
     */
    CCSpriterX *spTouch;
    /**
     *
     */
    CCSprite *bboard;
    /**
     *
     */
    int m_lastClickj;
    /**
     *
     */
    bool isFirstRound;
    /**
     *
     */
    bool isTouchInit;
    /**
     *
     */
    bool m_showDeletPop;
    
    /**
     *
     */
    int  m_skillDelayCount;
    /**
     *
     */
    int  m_aiSkillDelayWatch;
    
    /**
     *  自动战斗提示信息的显示延时标记
     */
    bool m_autoBattleInfoDisp;
    
public:
    /**
     * 处于非防御攻击阵列的兵
     */
    CCArray *fourList;
    /**
     *
     */
    CCArray *twoList;
    /**
     *
     */
    CCArray *oneList;
    /**
     * 当前的touch状态;
     */
    ETouchState m_touchState;
    /**
     * 当前形成的英雄阵列
     */
    CCArray *m_spAttackArray;
    /**
     *
     */
    SGGridBase *myGrids[mapRow][mapList];
    /**
     *
     */
    SGGridBase *fillUnitGrids[mapRow][mapList];
    /**
     *
     */
    int goalList;
    /**
     *
     */
    bool isUpdateEnd;
    /**
     *
     */
    bool isPlayerOperation;
    /**
        援兵
     */
    bool isAddBingIn;
    /**
     *
     */
    bool _isRefreshMap;
    /**
     *
     */
    bool closeMap;
    /**
     *
     */
    SGGridBase *m_seveBuffGrid[mapRow][mapList];
    /**
     * y uqf r ynqy buff
     */
    CCArray *m_heroBuff;
    /**
     *
     */
    int princeNum;
    /**
     *
     */
    int operationNum;
    /**
     *
     */
    int roundNum;
    //是否在补兵
    bool m_isStartFillUpSoldier;
    //补兵是记录处在最前位置士兵的i值,
    int m_nTheFormerPosI;
    CCArray *m_sameTimeSB;//存储同事形成的SB阵列数组
    CCArray *m_sameTimeWJ;//存储同事形成的WJ阵列数组
    int m_nAItips_time;//ai友情提示
    
};
#endif /* defined(__GameSanGuo__SGBattleMap__) */
