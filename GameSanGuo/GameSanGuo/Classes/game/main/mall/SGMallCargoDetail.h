//商城
//by: zyc
//date:2014-06-25


#ifndef __GameSanGuo__SGMallCargoDetail__
#define __GameSanGuo__SGMallCargoDetail__

#include "SGBaseBox.h"
class CargoInformation;


class SGMallCargoDetail : public SGBaseBox
{
public:
    SGMallCargoDetail();
    ~SGMallCargoDetail();
    
    static SGMallCargoDetail * create(CargoInformation * ,int shopId);
    void initView(CargoInformation *,int shopId);
    
private:
    CargoInformation * _cargoInfo;
    int _shopId; //购买时需要shopId。
    //按钮回调
    
    void onBuyClicked(CCObject *);
    void onCancelClicked(CCObject *);


}; // end.






#endif //the end