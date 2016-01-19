//
//  SGShowAllLayer.h
//  GameSanGuo
// 武将展开界面
//  Created by gengshengcai on 13-1-14.
//
//

#ifndef __GameSanGuo__SGShowAllLayer__
#define __GameSanGuo__SGShowAllLayer__

#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#include "SGEmbattleItem.h"
#include "SGEmbattleBox.h"
class SGShowAllLayer : public SGBaseLayer,  public MoveDelegate
{
private:
    void sortHandler();
    void backHandler();
    int colorId;
    CCArray *cardArray;
    SNSScrollView *scrollView;
    bool isCanCrade;
    int tradeIndex;
    int selectIndex;
private:
    //显示要
    void showEmbattleBox(SGOfficerCard * card);
    //设置上场士兵
    void setSolider(CCNode *sender);
    
protected:
    virtual void showBoxCall(CCObject *obj);
    virtual void moveToPoint(CCPoint point, int index);
    virtual void setIsSelect(bool isSelect);
    virtual bool trade();
    virtual void selectedIndex(int index);

public:
    SGShowAllLayer();
    static CCRect scrollviewRect;
    
    CREATE_FUNC(SGShowAllLayer);
    ~SGShowAllLayer();
    virtual bool init();
    virtual void initView();
};
#endif /* defined(__GameSanGuo__SGShowAllLayer__) */
