//
//  SGStrengLayer.h
//  GameSanGuo
// 强化界面
//  Created by gengshengcai on 13-1-8.
//
//

#ifndef __GameSanGuo__SGStrengLayer__
#define __GameSanGuo__SGStrengLayer__

#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGBaseMilitaryCard.h"
#include "SGSortBox.h"
#include "SGEquipCard.h"
#include "SGSoldierCard.h"
#include "SGEmbattleBox.h"

#pragma mark Tabel_Item

class Tabel_Item : public SNSTableViewCellItem
{
private:
    CCSprite *item;
    SGButton *spritePlayerHead;
    CCSprite *spriteIsScelected;
    SGCCLabelTTF *labelName;
    SGCCLabelTTF *labelLevel;
    CCArray *arrayStar;
    CCSprite *sprite;
    CCArray *bluestar;
public:
    SGBaseMilitaryCard *_card;

private:
    void showPlayerStar();
    void showgenerinfo();
public:
    void updateOfficerCard(SGBaseMilitaryCard *card,int index, int isSelect);
    void setItemSelect(bool isSelect);
    void setIsCanSelect(bool isCanSelecdt);
    Tabel_Item();
    ~Tabel_Item();
    
public:
    
	void setView(int num);
	static Tabel_Item* create(SGBaseMilitaryCard *card,int num);
};


class SGStrengLayer : public SGBaseTableLayer , public SGSortLayerDelegate
{
private:
	int times;//11.25
    int addFlags;
    int _currentPage;
    int strengLimitLevel;
private:
    CCArray *unsortarrary;
    CCSpriterX *fermEffect;
    int selectposition;
    int isfromfirst;
    int leavenum;
	ERI *EmbattleInfo;//布阵界面返回信息
    bool onlyOfficer;
    bool add;
    int selectcardcount;
    CCSprite *Icon;
    SGCCLabelTTF *choose;
    SGCCLabelTTF *lab_lvl;      //强化后等级
    SGCCLabelTTF *lab_orilvl;   //强化前等级
    //SGCCLabelTTF *need;
    SGCCLabelTTF *afterExp;
    SGCCLabelTTF *skilluprate;
    SGCCLabelTTF *maxe ;//升到满级的经验
    //SGCCLabelTTF *have ;//当前拥有的钱
    CCProgressTimer *progress;
    //int needCoins;
    SGBaseMilitaryCard *_card;
    SGOfficerCard *officer;
    SGEquipCard *equip;
    SGSoldierCard *soldier;
    CCArray *_arraySelectCard;    
    SGButton *range;
    int cardCount;
    int cardType;
    //标识着每个cell的选中状态
    int selectItem[300];
    int isSelect;
    SGButton *okBtn;
    Tabel_Item * tabelItem;
    int officerNum;
    bool isautostrengenough;
    int maxneedexp;
    
    
    SGCCLabelTTF *lab_addack;
    SGCCLabelTTF *lab_addSpeed;
    SGCCLabelTTF *lab_adddef;
    SGCCLabelTTF *lab_addmoral;
    
    SGCCLabelTTF *m_plabel_MaterialCount;
    
protected:
    virtual void setIsCanTouch(bool isTouch);
    virtual void setIsLock(bool isLock);
private:
    void updateposition(CCNode *node);
    void stopjiantouAction();
    void strengCardListener(CCObject *obj);
    void loadData(int ssid);
    void updateCoind();
    void strengOfficerListener(CCObject *obj);
    void strengEquipListener(CCObject *obj);
    void strengSoldierListener(CCObject *obj);
    void strengSkillListener(CCObject *obj);
    void strengLordSkillListener(CCObject *obj);
    void changeNum(int num);

    void cardBlink(CCNode *node);
    void cardBlinkcall(CCNode *node);
    void cardBlinkMidcall(CCNode *node);
    void nextcardBlindcall(CCNode *node);
    void cleanAnotherball();
    void cleanballandrotate(CCNode *node);
    void showblinkframe();
    void sendrequest();
    //返回
    void backHandler();
    //排序
    void sortHandler();
    //确定
    void confirmHandler();
    void secondeconfirm();
    
    void visitHandle();
    
    void labelcallback();
    
    void labelcallback1();
    virtual void showBoxCall(CCObject *obj);
    virtual void sortLayerCallBack(CCArray *cardArray);
    void btnselected(SGButton *obj);
    bool strengHaveSameNameOfficer(int itemId);
    
    //对应的合成逻辑，向服务器以关消息
    void strengMergeLogic(CCObject *obj);
public:
    
    SGStrengLayer();
    ~SGStrengLayer();
	
	
	//11.25自动添加强化材料
	void autoStreng();//自动添加
    void showSort();//展示素材前按展示排序规则排序
	void setSelectCard(Tabel_Item *item);
	void reSortCard();
	int findPreSameIndex(int n, int which);
	int findBckSameIndex(int n, int which);
	void findSameItem(int pos, int s, int e, int which);
	void findSameCountry(int pos, int s, int e);
	
	void refreshStrengView();
	void freshData();
	void freshViewData();
	/////////////////////////////////////////////////
	
    static SGStrengLayer*create(SGBaseMilitaryCard *card,int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage=1000);
    virtual bool init(SGBaseMilitaryCard *card,int type,bool isOnlyOfficer);
    virtual void initMsg();
    virtual void initView();
    
    int countTheMaterialNumber();
    
public:
    void selectItemByIndex(int index);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
//    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag);
};


// 一键合成的确认框

class SGMergeMaterialBox : public SGBaseBox
{
private:
    //确定
    void btnYesHandler(SGButton *sender);
    //取消，对守荆州有二次确认。
    void btnNoHandler(SGButton *sender);
    //强化什么东西，类型
    int strengType;
    //
    SGBaseMilitaryCard *card;
    
public:
    void mergerItemListener(CCObject *sender);
    SGMergeMaterialBox();
    ~SGMergeMaterialBox();
    static SGMergeMaterialBox *create(SGBoxDelegate *target, int strengType, SGBaseMilitaryCard *card);
    void initView();
    void boxClose();
};









#endif /* defined(__GameSanGuo__SGStrengLayer__) */
