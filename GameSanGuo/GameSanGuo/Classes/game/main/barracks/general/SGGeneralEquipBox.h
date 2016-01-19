//
//  SGGeneralEquipBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-8.
//
//

#ifndef __GameSanGuo__SGGeneralEquipBox__
#define __GameSanGuo__SGGeneralEquipBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
#include "SGEquipCard.h"

class SGGeneralEquipBox : public SGBaseBox
{
private:   
    SGOfficerCard *officerCard;
    SGEquipCard *equipCard;
    int state;
    SGBoxDelegate *dlg;
    int enterType;
    int enterTypeOther;//另外一个判断入口的变量,历史问题没有办法
private:
    void offHandler();
    void strengHandler();
    void upHandler();
    void offEquipListener(CCObject *obj);
    void wearEquipListener(CCObject *obj);
public:
    SGGeneralEquipBox();
    ~SGGeneralEquipBox();
    
    static SGGeneralEquipBox*create(SGBoxDelegate *delegate, SGOfficerCard *card,SGEquipCard *card_,int type,int enter,int enterTypeOther=0);
    void initView();
    virtual void boxClose();
    virtual void boxCloseWithOutCallBack();
};


#endif /* defined(__GameSanGuo__SGGeneralEquipBox__) */
