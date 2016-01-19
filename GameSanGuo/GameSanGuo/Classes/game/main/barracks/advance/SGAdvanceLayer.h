//
//  SGAdvanceLayer.h
//  GameSanGuo
// 武将进阶界面
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGAdvanceLayer__
#define __GameSanGuo__SGAdvanceLayer__

#include "SGBaseLayer.h"
#include "SNSTableView.h"
#include "SGOfficerCard.h"
#include "SGPropsCard.h"
#include "SGEmbattleBox.h"
class SGAdvanceLayer : public SGBaseLayer
{
private:
    CCArray *reqCards;
    CCArray *datas;
    int isfromfirst;
protected: /*SNSTableViewDataSource*/
    void setIsCanTouch(bool isTouch);
    void advanceOfficerListener(CCObject *obj);
//    void advanceEquipListener(CCObject *obj);

public:
   // CCSprite *createSpriteWithGrayColor( const std::string &strPath, CCImage::EImageFormat eImgFmt );
    SGAdvanceLayer();
    ~SGAdvanceLayer();
    static SGAdvanceLayer *create(SGBaseMilitaryCard *card,int type,int isfirst,CCArray *arry,ERI *info = NULL);//保存从布阵进入转生的信息
private:
    void initView();
    void confirmHandler();
    void backHandler();
    void showCurrInfo();
    void showAfterInfo();
    void setlab(CCPoint pos,SGBaseMilitaryCard *card);
    void setafterlab(CCPoint pos,SGBaseMilitaryCard *card);
    void showpropInfo(CCNode *node);
    void showcurrlvl();
    void showmaxlvl();
    void hidelab();
    void buttonselect(CCObject *obj);
    /////显示N维属性最大值们
    void  showmaxlabels();

private:
//    bool isenough;
    SGBaseMilitaryCard *_card;
    SGOfficerCard *officer;
    SGBaseMilitaryCard *afterCard;
    SGEquipCard *equip;
    CCDictionary *dict;
    CCArray *haveCount;
    CCArray *_arry;
    int cardType;
    int beforeitemId;
    int beforestar;
	ERI *info;//返回布阵界面的信息
    //SGButton *b[3];
    int stars[6];
    
    CCProgressTimer * m_progressTimer;
    SGCCLabelTTF * m_pieceHasNum ;
    SGCCLabelTTF * m_pieceNeedNum ;
    
    void sourceBtnClicked();
    SGButton * m_sourceBtn;
    bool m_canAdcance;
    bool m_isMaxAdv;
    
    bool m_btnCanTouch;
};

//class Advance_Item : public SNSTableViewCellItem
//{
//private:    
//    SGButton *touxiang;
//    SGPropsCard *pCard;
//private:
//    void showPropInfo();
//public:
//    CCLabelTTF *label;
//    CCLabelTTF* label2;
//    CCLabelTTF *name;
//public:
//    Advance_Item();
//    ~Advance_Item();
//	virtual bool initWithSize(int num);
//	static Advance_Item* create(int num);
//    void updateItemCard(int itemID,int count,int have);
//
//};

#endif /* defined(__GameSanGuo__SGAdvanceLayer__) */
