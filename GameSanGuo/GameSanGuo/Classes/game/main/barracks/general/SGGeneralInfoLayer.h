//
//  SGGeneralInfoLayer.h
//  GameSanGuo
// 武将详细信息界面
//  Created by gengshengcai on 13-1-8.
//
//

#ifndef __GameSanGuo__SGGeneralInfoLayer__
#define __GameSanGuo__SGGeneralInfoLayer__

#include "SGBaseStuffLayer.h"
#include "SGOfficerCard.h"
#include "SGEquipCard.h"
#include "SGCCLabelTTF.h"
#include "CCSpriterX.h"
#include "SGChangeEquipEffectLayer.h"
#include "CCScale9Sprite.h"
#include "SGEmbattleBox.h"

//class cocos2d::extension::CCScale9Sprite;
class SGTouchLayer;

class SGGeneralInfoLayer : public SGBaseStuffLayer 
{
private:
    int _currentRow;
    int _currentPage;
    int officerCardlevelLimit;
public:
    CC_SYNTHESIZE(int , plotID, PlotID);
    CC_SYNTHESIZE(int , storyID, StoryID);
    CC_SYNTHESIZE(int , roldID, RoldID);
 
public:
	ERI *info;//进入更换装备后，再次返回布阵界面所需要的信息
    int isfromfirst;
    int cardId;
    int leftTimes;
    SGOfficerCard *_card;
    SGOfficerCard *_before;
    
    //强化按钮
    SGButton * strengBtn;
    //武将培养
    SGButton * developingBtn;
    //转生按钮
    SGButton *advanceBtn;
    
    //展示按钮
	SGButton *showButton;
    //换人按钮
    SGButton *changeOfficerButton;
    //下场按钮
    SGButton *offButton;
    //设为主将
    SGButton *setlordButton;
    //主将查看返回
    SGButton *lordback;
    
    SGEquipCard *equipCard;
    
    //武器栏位
    SGButton *wuqi;
    CCSprite *wuqilock;
    CCSprite * wuqiNotice;
    //铠甲栏位
    SGButton *kaijia;
    CCSprite *kaijialock;
    CCSprite * kaijiaNotice;
    //配饰栏位
    SGButton *peishi;
    CCSprite *peishilock;
    CCSprite * peishiNotice;
    //坐骑栏位
    SGButton *zuoji;
    CCSprite *zuojilock;
    CCSprite * zuojiNotice;
    
    //滑动控件
    SGTouchLayer* slideLayer;
    
    //技能控件
    SGTouchLayer* skillLayer;
    
    //将缘单独LAYER
    SGTouchLayer* fateLayer;
    //自动换装
    SGButton *autoequip;
    
    SGCCLabelTTF* governing;
    CCSprite *lock;
    CCPoint pos;
    int itemtype;
    /**
     * enterType特殊处理，0=背包武将列表LAYER的武将点击，3=首页LAYER的激活阵营的武将点击，15=阵营设置的某个阵营的武将点击。
     * 目前这三种进入方式需要初始化滑动列表，而由于历史原因，需要在这三个方式入口处获得需要参与滑动的武将列表，并将其通过static方法setMyOfficerList(CCArray* )传入，传入空即无列表。
     * 劳烦诸位顺手记录下各个ID对应的页面。
     * 7=SGStrengLayer，10000=强化页面过来 20000=从首页强化主将计过来
     *呵呵 顶楼上  enterType==100为好友相关?
     *
     */
    int enterType;
    bool isback;
    bool isbreak;
    SGCCLabelTTF *atkxiaoguo; //自动换装后攻击防御四维属性效果
    SGCCLabelTTF *defxiaoguo;
    SGCCLabelTTF *roundxiaoguo;
    SGCCLabelTTF *speedxiaoguo;
  
    
    CCDictionary *Direc;
    CCSpriterX *lvlupAct;
    bool isPlayOpenLockAnimation;
    bool isFriendGeneral;
    //用于等级和经验的显示框，记录以防纠结
    cocos2d::extension::CCScale9Sprite* frame1;
    
    //各类需要修改的文字在此
    //当前等级
    SGCCLabelTTF* labelLevel;
    //最大等级
    SGCCLabelTTF* labelMaxLevel;
    //经验
    //SGCCLabelTTF* labelExp;
    //攻击
    SGCCLabelTTF* labelAtk;
    //防御
    SGCCLabelTTF* labelDef;
    //血量
    SGCCLabelTTF* labelMorale;
    //速度
    SGCCLabelTTF* labelSpeed;
    //统御
    SGCCLabelTTF* labelGov;
    //蓄力
    SGCCLabelTTF* labelRound;
    //主将计说明文本
    SGCCLabelTTF * labelZjj;
    //被动技说明文本
    SGCCLabelTTF* labelWjj;
	
	//武将占几格
	SGCCLabelTTF* labelCell;
    
    //小转生次数
    SGCCLabelTTF * m_advanceCountLab;
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
	
private:
    void inviterListener(cocos2d::CCObject *obj);
    void updateskill(CCNode *node);
    void initData();
    void disprotectListener(CCObject *obj);
    void soldiersDispatchListener(CCObject *obj);
    void officerprotectListener(CCObject *obj);
    void backHandler();
     void listener();
    void changeexplabel();
    //强化
    void strengHandler();
    //进阶
    void advanceHandler();
    //设置保护
    void protectHandler();
    void disProtectHandler();
    //展示
    void showHandler();
    //显示主将计能信息
    void showSkillInfo();
    //显示被动技能信息
    void showOfficerSkil();
    //显示装备信息
    void showEquipInfo();
    //设置装备
    void setEquipment(CCNode *sender);
    void showEquipTips(int type);
    //通过tag值找到相应的layer显示出来，用于返回按钮的操作
    void showLayerByTag(LayerTag layerTag);
    void showChgScoreLayer(CCObject *sender);
//    void setdic(CCDictionary *data,int a ,int b ,int c ,int d);
    //显示成功状态
    void playAction();
    void removeSpX();
    void aaaa();
    void setcanttouch();
    void setcantouch();
    void listener(CCObject *obj);
    void autoequipfunc(cocos2d::CCObject *obj); ////自动装备按钮回调函数
    void PlayOpenLockAnimation();//播放解锁动画
    
    //定制的初始化6大LABEL，ATK/DEF/MORALE/SPEED改为图片按钮。
    virtual void addLabel(stuffType type, cocos2d::CCPoint point);
    
    //anchorType为适应不同界面的文字锚点而定，默认0为(1,0.5)，1为(0,0.5)        //qianvalue strengthtype为解决强化士兵和装备设的值    //此处没有用处  只为保证父类虚函数结构 //详情见父类
    virtual void addValueLabel(stuffType type, int value1, float move,int value2 = 0, int anchorType = 0 ,int qianvalue=0, int strengthtype=0,bool ismax=false);
    
    //定制的文本动画
    virtual void labAnimation(stuffType type,int change);
    
    //定制的修改指定文本数字
    virtual void setlabelValue(stuffType type, int value,int value2=0);
    
    //定制的经验条动画回调
    virtual void progressCallback(CCNode*node,SGBaseMilitaryCard *card);
    
    //定制的经验条动画启动
    virtual void progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call);
public:
    //存储本次将要参与滑动的武将卡列表
    static void setMyOfficerList(CCArray* arr);
	
	void callBack(cocos2d::CCNode *node);

//滑动相关的所有参数都在这里
private:
    //武将卡列表
    static CCArray* m_myOfficerList;
    
    //是否支持滑动，根据enterType确定
    bool m_isSupported;
    
    /**
     * 根据索引生成一个武将原画LAYER
     */
    CCSprite* generateData(int index);

    /**
     * 左侧对象
     */
    CCSprite* m_leftObj;
    
    /**
     * 左侧对象X坐标
     */
    float m_leftX;
    
    /**
     * 左侧对象标准位置
     */
    CCPoint m_leftPos;
    
    /**
     * 当前对象
     */
    CCSprite* m_middleObj;
    
    /**
     * 当前对象X坐标
     */
    float m_middleX;
    
    /**
     * 当前对象标准位置
     */
    CCPoint m_middlePos;
    
    /**
     * 右侧对象
     */
    CCSprite* m_rightObj;
    
    /**
     * 右侧对象X坐标
     */
    float m_rightX;
    
    /**
     * 右侧对象标准位置
     */
    CCPoint m_rightPos;
    
    /**
     * 当前对象索引
     */
    int m_index;
    
    /**
     * 触控开始的点的X值
     */
    float m_beganX;
    
    /**
     * 触控移动的距离
     */
    float m_dist;
    
    /**
     * 是否可以开始移动。
     */
    bool m_canMove;
    
    /**
     * 是否被点击到了。
     */
    bool m_isTouched;
    
    /**
     * 能否被触控，一般只在动画结束后重新赋值。
     */
    bool m_canTouched;
    
    /**
     * 移动layer
     */
    void moveItemLayer();
    
    /**
     * 使用动作调整位置。-2=不调整。-1=左不调整，0=全部调整，1=右不调整。
     */
    void adjustPosition(int type);
    
    /**
     * 清理一些数据成员。
     */
    void cleanParams();
    
    /**
     * 重置LAYER使能够被触控。
     */
    void resetCanTouched();
private:
    //刷新武将信息！用于切换时候
    void refreshOfficerInfo();
    
    //生成缘分信息！
    void createFateList();
    void createFateListFromChatShow();
    //显示缘分信息
    void showFateInfo();
    
    //获得装备对自身的加成
    CCDictionary* getOfficerEquipAddedValue();
    
    //获得所有自身属性的加成
    CCDictionary* getOfficerTotalValue();
    
    //刷新四件装备按钮，同时刷新"转生""锁定""卖出"按钮，因为一定更换了武将。
    void renewEquipment();
    
    //转生,保护,卖出的按钮状态切换，1=转生，2=锁定+卖出。
    void changeApsButtons(int type);
    
    //装备区的背景是否加载完毕，false则需要先预置
    bool m_isEqpBkgReady;
    
    //滑动左箭头
    CCSprite* m_arrowLeft;
    
    //滑动右箭头
    CCSprite* m_arrowRight;
    
    //调整左右箭头的显隐
    void tuneArrows();
    
public:
    //刷新六大属性，用于卸下装备
    void renewSimpleInfo();
    
    //回调刷新本页面，主要是更新装备
    virtual void refreshView();
    
    //重写触控，方便控制三个组件的开关。
    virtual void setIsCanTouch(bool isTouch);

public:
    //began
    bool helloBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //moved
    void helloMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    //ended
    void helloEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    //cancelled
    void helloCancelled(CCTouch *pTouch, CCEvent *pEvent);
private:
    void enterFightAction();
    
public:
    SGGeneralInfoLayer();
    ~SGGeneralInfoLayer();
    static SGGeneralInfoLayer *create(SGOfficerCard *card,int type,ERI *info = NULL, SGOfficerCard *officer = NULL,int isfirst = 0,CCDictionary *dic = NULL,bool isFriendGeneral = false,int currentPage=1,int currentRow=1 );
    virtual void initView();
//    virtual void onEnterTransitionDidFinish();
    virtual void showBoxCall(CCObject *obj);
public:
    SGChangeEquipEffectLayer *changeEquipEffectLayer;
    
    //从布阵界面进入
public:
    void offHandler();
    void setMainOfficerHandler();
    void changeHandler();
    void setLordOfficerListener(CCObject *obj);
    void setMainOfficerListener(CCObject *obj);
    void offMainOfficerListener(CCObject *obj);
    
 
public:
    void setSpeedAndMorValue(int speedValue,int morValue,int comBattleValue);
    
public:
    int combatle;
    int morValue;
    int speedValue;
    int GovValue;
private:
    CC_SYNTHESIZE(int , totalGov, totalGov);
    CC_SYNTHESIZE(int , position, position);
    CC_SYNTHESIZE(int , availPos, availPos);
    
    //阵容编号
    static int emtype;
public:
    inline static void setEmType(int num){emtype = num ;};
public:
    void setGovAndType(int totalGov,int emType,int position,int availbPos);
    void setTheLockEquip();
    
    //展示等级、经验
    SGCCLabelTTF * m_showLev;
    //SGCCLabelTTF * m_showExp;
    SGCCLabelTTF * m_levText;
    //SGCCLabelTTF * m_expText;
    
    //碎片已拥有 、需要数量
    SGCCLabelTTF * m_pieceHasNumLab;
    SGCCLabelTTF * m_pieceNeedNumLab;
    SGCCLabelTTF * m_upMaxStarLab;
    //碎片icon 使用相同icon
    CCSprite * m_pieceIcon;
    CCScale9Sprite * m_pieceRateBg;
    CCProgressTimer * m_pieceProgress;
    //来源按钮
    SGButton * m_sourceBtn;
    //拥有 、 转生需要碎片数量
    SGCCLabelTTF * m_pieceNum;
    void showLevExp(int nowLev , int maxLevel , int nowExp , int maxExp ,bool isVisible);
    void showPieceRate(int hasNum , int needNum , bool isVisible);
    //展示碎片数量 进度
    void showPieceRate();

private:
    //点击来源按钮的回调
    void onLootSourceBtnClicked();
    CCSprite * _spriteNotice ;//可转生提示
    CCSprite * m_strengNotice;//可强化提示
    CCSprite * m_devNotice;//可培养提示
    
    //武将背景图片 3 4 5 6代表 绿 蓝 紫 橙 品质。
    CCSprite * m_officerBg[4];
    void showOfficerBg(int quality);
    
    int getOfficerPieceHasNumByOfficerProtoId(int protoId);
    
    void refreshEquipNotice(SGOfficerCard *);
    
    //武将培养按钮回调
    void developingBtnCallback();
    
    //武将培养消息回调
    void onDevelopingReponse(CCObject * sender);
};

#endif /* defined(__GameSanGuo__SGGeneralInfoLayer__) */
