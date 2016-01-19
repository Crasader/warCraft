//
//  SGAdvancechooseLayer.h
//  GameSanGuo
// 强化界面
//  Created by gengshengcai on 13-1-8.
//
//

#ifndef __GameSanGuo__SGAdvancechooseLayer__
#define __GameSanGuo__SGAdvancechooseLayer__

#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGBaseMilitaryCard.h"
#include "SGSortBox.h"
#include "SGEquipCard.h"
#include "SGSoldierCard.h"

#pragma mark Tabel_Item1

class Tabel_Item1 : public SNSTableViewCellItem
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
    Tabel_Item1();
    ~Tabel_Item1();
    
public:
    
	void setView(int num);
	static Tabel_Item1* create(SGBaseMilitaryCard *card,int num);
};


class SGAdvancechooseLayer : public SGBaseTableLayer , public SGSortLayerDelegate
{
private:
    int _currentPage;
private:
    CCSpriterX *fermEffect;
    int selectposition;
    bool isfromfirst;
    bool onlyOfficer;
    bool add;
    int selectcardcount;
    CCSprite *Icon;
    SGCCLabelTTF *lab_lvl;
    SGCCLabelTTF *lab_orilvl;
    SGCCLabelTTF *need;
    SGCCLabelTTF *afterExp;
    SGCCLabelTTF *skilluprate;
    CCProgressTimer *progress;
    int needCoins;
    SGBaseMilitaryCard *_card;
    SGOfficerCard *officer;
    SGEquipCard *equip;
    SGSoldierCard *soldier;
    CCArray *_arraySelectCard;
    CCArray *_remain;
    CCArray *_beforecard;
    SGButton *range;
    int cardCount;
    int cardType;
    CCSprite *addprogress;
    //标识着每个cell的选中状态
    int selectItem[300];
    int beforeselect[3];
    int isSelect;
    SGButton *okBtn;
    Tabel_Item1 * tabelItem;
//    int officerNum;
    SGCCLabelTTF *lab;
    SGCCLabelTTF *lab2;
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
    void clickOK();
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
    
    void visitHandle();
    
    void labelcallback();
    
    void labelcallback1();
    virtual void showBoxCall(CCObject *obj);
    virtual void sortLayerCallBack(CCArray *cardArray);
public:
    
    SGAdvancechooseLayer();
    ~SGAdvancechooseLayer();
    static SGAdvancechooseLayer*create(SGBaseMilitaryCard *card,int type,bool enter,bool isOnlyOfficer ,int currentPage=1000,CCArray *array = NULL);
    virtual bool init(SGBaseMilitaryCard *card,int type,bool isOnlyOfficer);
    virtual void initMsg();
    virtual void initView();
    
public:
    void selectItemByIndex(int index);
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
//    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag);
};


#endif /* defined(__GameSanGuo__SGAdvancechooseLayer__) */
