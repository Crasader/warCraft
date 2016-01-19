//
//  SGSoldierListBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGSoldierListBox__
#define __GameSanGuo__SGSoldierListBox__
#include "SGBaseBox.h"
#include "SNSTableView.h"
#include "SGSoldierCard.h"
#include "SGEmbattleLayer.h"

class SGSoldierListBox : public SGBaseBox
{
private:
    int colorId;
    SGEmbattleLayer *_delegateLayer;
    int tag;
    int emtype;
    
private:
    void showSoldierInfo(CCNode *node);
    void initView();    

    void buttonClick(CCNode *node);
    void callBackForSoldier(int index);
    void soldiersDispatchListener(CCObject *obj);
public:
    SGSoldierListBox();
    ~SGSoldierListBox();
    static SGSoldierListBox *create(SGBoxDelegate *dg, int colorId_ , SGEmbattleLayer *delegateLayer,int emtype);
    
};

#endif /* defined(__GameSanGuo__SGSoldierListBox__) */
