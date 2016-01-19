//
//  SGArrangeLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-30.
//
//

#ifndef __GameSanGuo__SGArrangeLayer__
#define __GameSanGuo__SGArrangeLayer__

#include "cocos2d.h"
#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#include "SGSortBox.h"

USING_NS_CC;


class officerCardInfo{
    int sSsid; //服务器ID
};

// 阵上换人的时候，武将确认框
class SGChangeOfficerBox : public SGBaseBox
{
private:
	SGOfficerCard *_card;
	SGOfficerCard *source;
	int m_gov;
	int emtype;
public:
	SGChangeOfficerBox();
	~SGChangeOfficerBox();
	
	static SGChangeOfficerBox *create(SGBoxDelegate *del, SGOfficerCard *card, int type, SGOfficerCard *source);
	void initView();
	virtual void boxClose();
	void changePosition();
};

class SGArrangeLayer : public SGBaseTableLayer , public SGSortLayerDelegate,  public SNSScrollViewPageDelegate
{
private:
    int position;
    SGButton *range;
    int enterType;
    int totleGovern;//当前统御力
    int allGovern;//所有的统御力
    int emtype;
    int availPosition;
    SGOfficerCard *_card;
    SGOfficerCard *source;
    SGCCLabelTTF *pageLabel;
    int preCombattle;
	//存放每个上阵的itemId,用来标识是否有重复武将上阵
	int itemIdArray[11];//最多有十个武将
    
public:
    CC_SYNTHESIZE(int , m_combattle, ComBattle);
    CC_SYNTHESIZE(int , m_speed, SpeedValue);
    CC_SYNTHESIZE(int , m_morValue, MorValue);
    CC_SYNTHESIZE(int , m_govValue, GovValue);
public:
    SGArrangeLayer();
    ~SGArrangeLayer();
	void exchangPosition();
    void initView(int pos);
    static SGArrangeLayer *create(int pos,int gov,int type,int emtype,int availPosition=0);
    void showEmbattleBox(SGOfficerCard *card, int fromWhere = 0);
    virtual void sortLayerCallBack(CCArray *cardArray);
    
    void showEmbattleBoxByIndex(int index);
	void showChangeOfficer(SGOfficerCard *card, SGOfficerCard *source);
	int GetCurrentGov();
	bool haveSameNameOfficer(int itemId);// 检查与阵上同名的武将
private:
    
    void exchangPositionListener(CCObject *obj);
    void backHandler();
    void sortHandler();
    int getgov(SGOfficerCard *m_card);

protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 设置翻页
    virtual void refreshPage(int type);
private:
    bool checkIsOutGov(SGOfficerCard *_card);//检查是否超出统御力
    
    void updateCurGov(int goving);
    void setAllGovern();
private:
    SGCCLabelTTF *govNum;
    SGCCLabelTTF *govAllNum;
    int posArr[11];//0是布阵123，1-10是卡片ID
    void initbattleArr();
    void addNewCard(int cardId, int itemId = 0);
    void deleteCard(int cardId, int itemId = 0);
    bool checkIsInEmbattle(int cardId);
private:
    void changeAllEmbatteTeamhandle();
    void changeAllEmbatteTeamListener(CCObject *obj);
    void checkUpEmbattel();//临时上场的武将放在前面
    void showEffectOfAddAttribute();
};


#endif /* defined(__GameSanGuo__SGArrangeLayer__) */
