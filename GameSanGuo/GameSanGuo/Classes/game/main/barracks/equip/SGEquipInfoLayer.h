//
//  SGEquipInfoLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#ifndef __GameSanGuo__SGEquipInfoLayer__
#define __GameSanGuo__SGEquipInfoLayer__

#include "SGBaseStuffLayer.h"
#include "SGEquipCard.h"
#include "CCSpriterX.h"
class SGEquipInfoLayer : public SGBaseStuffLayer
{
private:
    int _currentPage;
    int _currentRow;
private:
    //CCProgressTimer *expprog;
    int leftTimes;
    bool isfromfirst;
    SGEquipCard *_card;
    SGEquipCard *_before;
    SGButton *advanceBtn;
    SGButton *strengBtn;
    SGButton *sellBtn;
    CCSprite *lock;
    int enterType;
    CCPoint pos;
    bool isback;
    bool isbreak;
    
    CCDictionary *Direc;
    CCSpriterX *lvlupAct;
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
    
private:
    void initView();
    void backHandler();
    void strengHandler();
    void advanceHandler();
    void protectHandler();
    void sellHandler();
    void showEquipsInfo();
    void disprotectListener(CCObject *obj);
    void equipprotectListener(CCObject *obj);

    void disProtectHandler();
    void showLayerByTag(LayerTag layerTag);
    void aaaa();
    void setcanttouch();
    void setcantouch();
    void removeSpX();
    void playAction();
    
    /////////////////
    void setTitle(const char *title_);

    void changeexplabel();
    void progressCallback(CCNode*node,SGBaseMilitaryCard *card);
    void progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call);
    void showjitian();

    ///////////////////
    
    //材料相关，获取依赖条件，与生成几个icon的layer
    CCArray *getMaterialDependency(int *needNum);
    CCLayerColor *getMaterialLayer(CCArray *materialModelArray, int *needNumArray);
    
public:
    
    SGEquipInfoLayer();
    ~SGEquipInfoLayer();
    virtual void showBoxCall(CCObject *obj);
    static SGEquipInfoLayer *create(SGEquipCard* card,int type,SGEquipCard *before = NULL,bool isfirst=false,CCDictionary *dic = NULL,int currentPage=1000,int currentRow=0);
    virtual void addValueLabel(stuffType type, int value1, float move,int value2 = 0, int anchorType = 0,int qianvalue=0 ,int strengtype=0,bool ismax=false);

    //by:zyc 添加装备来源
private:
    //来源按钮的回调
    void onLootSourceBtnClicked(CCObject *  );
};

#endif /* defined(__GameSanGuo__SGEquipInfoLayer__) */
