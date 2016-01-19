//
//  SGVisitShowCardLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-26.
//
//

#define EFFECT_VISIT_OFFICER 10000
#define EFFECT_VISIT_LIGHT   10001
#define EFFECT_VISIT_ROLL    10002


#ifndef __GameSanGuo__SGVisitShowCardLayer__
#define __GameSanGuo__SGVisitShowCardLayer__

#include "SGBaseLayer.h"
#include "SGBaseMilitaryCard.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
class SGVisitShowCardLayer: public SGBaseLayer
{
private:

    CCArray *visitItemstar;
    
    CCArray *_array;
    CCArray *_huoArray;
    //12张卡牌的id
    int m_nCardId;
    
    //12张卡牌的数组
    CCArray *m_pCardArray;
    
    //抽到卡的数量
    int m_nCardCount;
    
    ////////////////////
    SGButton * visitDoorBt;
    
    SGCCLabelTTF *visitCardName;
    SGCCLabelTTF *visitDetailInfo;

    SGButton *selectCardBt;
    
    SGButton *visitCardPng;
    
    int showCardTag;
    
    int showStarNum;
    
    float m_MoveHaHaTime;
    
    CCSize winSize;
    
    CCSprite *cardPngSprite1;
    
    CCSprite *cardPngSprite2;
    
    SGBaseMilitaryCard *_comboCard;
    SGBaseMilitaryCard *_progressCard;
    bool useGoldDoor;
    float _moveUp;
    CCLayer *la;
    float movedis;
    CCNode *_visitPiece;
    float setViewWid;
    SGCCLabelTTF *piecesName;
    
    CCScrollView *scrollview;
    CCSprite *piecesLable;
    CCSprite *givenPieceBg;
    SGButton *ok;
    SGButton *buttonShare;

    
    CCSprite *piecesCoverLeft;
    CCSprite *piecesCoverRight;
    
    CCSprite *finaleSprite;
    
    //传过来买到的物品
    SGPropsCard *propBuyCard;
    
    //武将碎裂提示文本，绝大部分情况下，这个是空文字。
    SGCCLabelTTF* _crashNoticeLabel;
    CCSprite * m_plightCheng;
    CCSprite * m_plightLv;
    CCSprite * m_plightLan;
    CCSprite * m_plightZi;
    
    
public:
    SGVisitShowCardLayer();
    ~SGVisitShowCardLayer();
    static SGVisitShowCardLayer *create(CCArray *array,CCArray *huoArray,SGBaseMilitaryCard *comboCard,
                                        SGBaseMilitaryCard *progressCard,bool useGoldDoor, SGPropsCard *buyPropCard);
    
private:
    void initView(CCArray *array,CCArray *huoArray,SGBaseMilitaryCard *comboCard,SGBaseMilitaryCard *progressCard);
    
    void showPiecesCard();
    
    void createPiecesCard();
    
    void createCardShowInfo();
    
    void createVisitCard();
    
    void showDoorBtStart();
    
    void showDoorBtEnd();
    
    void showVisitCardPre();
    
    void visitDoorBtAction(CCObject* selectBt);
    
    void showVisitCardStart();
    
    void dealShowVisitCard();
    
    void selectVisitCard(CCObject* selectBt);
    
    void refreshShowInfo(int cardTag,bool isLastEff = true);
    
    void setItemStars(int starNum ,int maxstar = 0,bool startAction = false);
    
    void backHandler();
    
    void showinfoLayer(CCObject * selectPng);
    
    void setDoorBtImg();
    
    void refreshLastInfoEnd();
    
    void addKmfgRes();
    
    void removeKmfgRes();
    
    void createCardShadowAction(CCSprite * mainCard,const char* pngName,float delaytime);
    
    void movePiecesHaHa(float dt);
    
    void startMovePieces();
    
    void showPiecesName();
    
    void onClickShareButton();
    
    //显示门上方的显示的东西
    void dispConsumableUpDoor();
    void createLightBGEffect();//创建旋转光效
    void setTheLightEffect(int nIndex);//设置旋转光效显隐
    
};

#endif /* defined(__GameSanGuo__SGVisitShowCardLayer__) */
