//
//  SGCantVisitBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-20.
//
//

#ifndef __GameSanGuo__SGCantVisitBox__
#define __GameSanGuo__SGCantVisitBox__

#include "SGBaseBox.h"
class SGCantVisitBox : public SGBaseBox
{
private:
    int m_type;
    //需要的花费
    int m_cost;
    
    //
    SGBoxDelegate * m_delegate;
    
    
private:
    void boxClose();
    //去储值
    void gotoChargeLayer();
    //去元宝换铜钱
    void gotoExchangeLayer();
//    void noAward();
//    void visitHandler();
//    void confirmHandler();

//    void boxCloseWithOutCallBack();
//    void storeEnterListener(CCObject *obj);
public:
    SGCantVisitBox();
    ~SGCantVisitBox();
    //type: 1元宝不足，去充值    2铜钱不足，去元宝换铜钱        cost參數暂无用
    static SGCantVisitBox *create(SGBoxDelegate *bdg,int type , int cost=100000);
    void initView();
//    bool init(SGBoxDelegate *bdg,int type);
};

#endif /* defined(__GameSanGuo__SGCantVisitBox__) */
