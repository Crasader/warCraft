//
//  SGYangXinBox.h
//  GameSanGuo
//
//  Created by zenghui on 14.3.1.
//
//

#ifndef __GameSanGuo__SGYangXinBox__
#define __GameSanGuo__SGYangXinBox__

#include "SGBaseBox.h"
#include "SGActivityData.h"
class SGYangXinBox : public SGBaseBox
{
private:
    void boxClose();
    void boxCloseWithOutCallBack();
    void confirmHandler(CCObject *obj);
    void lingQuHandler(CCObject *obj);
    bool m_canGot;
    CCNode * creatYangXinItem(SGItemInfo &yangXinInfo);
public:
    SGYangXinBox();
    ~SGYangXinBox();
    static SGYangXinBox *create(SGBoxDelegate *bdg,SGItemInfo &yangXinInfo,bool canGot);
    bool init(SGBoxDelegate *bdg,SGItemInfo &yangXinInfo,bool canGot);
};

#endif /* defined(__GameSanGuo__SGYangXinBox__) */
