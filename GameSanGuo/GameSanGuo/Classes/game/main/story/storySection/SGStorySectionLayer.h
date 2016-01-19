//
//  SGStorySectionLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGStorySectionLayer__
#define __GameSanGuo__SGStorySectionLayer__


#include "SGStoryBean.h"
#include "SGStorySectionCell.h"
class NewScrollView;

class SGStorySectionLayer : public SGBaseTableLayer ,public SGStorySectionDelegate
{
    
public:
	virtual void onEnter();//11.20//设置menu的优先级
	
    SGStorySectionLayer(void);
    virtual ~SGStorySectionLayer(void);
    
    static SGStorySectionLayer *create(SGStoryBean* bean);
    void setStoryId(int sid);
    
    virtual void setIsCanTouch(bool isTouch);
    int getStorySectionCount(void);
    virtual void entrySectionHandler(SGStorySectionCell *section);
    virtual void showSectionInfoHandler(SGStorySectionCell *section);
    void dealFirstSectionInfo();
    void showFirstSectionInfo();
    void setSectionFirstInfo(SGStorySectionCell *section);
    void gotoFirendRequest();
	void freshStoryScenceData(CCObject *sender);
	void buyCreamListener(CCObject *sender);
	bool getBossSectionBeaten(int storyId); //boos关是不是打过,本地用户数据存储
	void backHandler(CCObject* sender);
	void sendRewardRequest(int storyId);
	
    //购买精英闯关次数时 在此页刷新
    void refreshCreamOnBuy();
private:
    void initView();
    void initData(SGStoryBean* bean);
    
    
    // 获取服务器发送的友军数据并显示
    void showHelperLayer(CCObject* sender);
    void gotoPurchPower();
    void creatSectionInfoView(SGStorySectionCell *section);
    void addSectionNormalItem(float contentHeight);
    void addSectionStarItem(float contentHeight);
    void addSectionItem(float contentHeight,bool isStarReward);
    void moveSectiontableViewDown(bool isDown);
    void hideSectiontableView();
    bool checkGovernNeed();
    void resetMovingSectiontableView();
    bool isMovingSection;
    void dealHideSingleMove();
    void rewardSuccess(CCObject *sender);
    SGStorySectionCell *m_starRewardCell;
    CCSize size;
    
    void limitMenuTouch();
    void resetMenuTouch();
    void delayShowSectionInfoHandler(SGStorySectionCell *section);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
private:
    SGStoryBean *storyBean;   // 故事－场景
    CCArray *mydatas;
    int selectIndex;    // 选中的场景
    int lastSelectIndex; //上次选中的场景
    float lastMoveDis; //上次选中场景时移动的距离
    NewScrollView *sectiontableView;
    float infoViewPosY;
    SGStorySectionCell *m_lastShowSection;
	
	int buyTimes;//今日可购买次数
    int storySecenId;
	
	//是否是自动领取的,用于确定是否回到闯关列表
	int isAutoReward;
	bool isPopRewardWindow;//是否弹出领奖窗口
    int m_nChallengeCount;
    int m_nChallengeMaxcount;
	
public:
    void showRewardBox();
	virtual int getSelectIndex();
};

class NewScrollView : public SNSScrollView
{
public:
    NewScrollView();
    virtual ~NewScrollView();
    static NewScrollView* creatNewScrollView(CCRect frames);
    bool initGSScrollViewFrame(CCRect frames);
    virtual void visit();
    CCRect parentScissorFrame;
    void setParentScissorFrame(CCRect parentScissor);
    
    void updateFrames(CCRect var);

};


//打完boss关, 出现的提示新关卡天启的提示框
class SGTipsBox : public SGBaseBox
{
public:
	SGTipsBox();
	~SGTipsBox();
	
	static SGTipsBox *create(SGBoxDelegate *del, bool canGetReward = false, int storyId = 0);
	void initView();
	
	void pressOkButton();
	void pressCancelButton();
	
private:
	bool canGetReward;
	SGBoxDelegate *del;
	int storyId;
};










#endif /* defined(__GameSanGuo__SGStorySectionLayer__) */
