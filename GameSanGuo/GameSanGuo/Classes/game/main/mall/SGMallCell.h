//商城
//by: zyc
//date:2014-06-26


#ifndef __GameSanGuo__SGMallCell__
#define __GameSanGuo__SGMallCell__

#include "SNSTableViewCellItem.h"
#include "SGMallData.h"
#include "cocos2d.h"

class SGMallLayer;

class SGMallCell : public SNSTableViewCellItem
{
public:
    // dtor
    SGMallCell();
    
    ~SGMallCell();
    //
    static SGMallCell * create(SGMallLayer * ,CargoInformation * );
    
    //
    void initView(SGMallLayer *,CargoInformation *);
    
    //设置本控件为灰色/原色。
    void setGray(bool isGray);
    
private:
    
    //点击具体的商品的回调。显示商品详情
    void onClicked(CCObject * pSend);
    
    CargoInformation * _cargoInfo;
    SGMallLayer * _mallLayer;
    
    //底板
    SGButton* btn;
    
    //售出图片
    CCSprite * _sellOut;
    
    //容器
    CCLayer *_contentLayer;
};




#endif //the end