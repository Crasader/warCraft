//商城
//by: zyc
//date:2014-06-25


#ifndef __GameSanGuo__SGMallData__
#define __GameSanGuo__SGMallData__

#include "RoleShopModule.pb.h"
#include "cocos2d.h"

//接收服务器消息，RoleShopProto. 商品详情
class CargoInformation : public CCObject
{
public:
    int cargoId;//商品id
    //std::string name;//商品名称
    int payKind;//支付方式
    int oldPrice;//原价
    int currentPrice;//现价
    //int remainsCount;//该角色剩余购买数量
    
    int itemKind;//物品类型
    int itemId;//物品id
    int itemCount;//物品数量
    int cargoTag; //页签归属  标识 讲、装、宝。
    int cargoState; //物品状态 。  0 正常，1售出 ， 2 限制购买中
};




class SGMallData : public CCObject
{
public:
    //商城类型
    int _mallId;
    //商店标题
    std::string _mallTitle;
    //当是天梯商店时。显示的商品分类，包含：将、装、宝,1=显示当前分类。-1=商城没有该分类。初始化为-1.
    int _goodsKind[3];
    //当前商店显示的可用代币支付方式，每种方式都有2中状态， -1=没有该种支付方式。 1=代表有。初始化为0.
    //当前商店所有物品需要的支付方式
    std::vector<int> _vecPayKind;
    //每日更新商品时间。
    std::string  _everydayUpdateTime;
    
    //主动刷新需要代币和数量
    int _refreshNeedMoney;
    int _refreshNeedPayKind;
    
    //存储所有商品，指向 CargoInfo结构
    CCArray * _allCargo;
    
    
    //ctor 初始化数组。
    SGMallData():_mallTitle(""),
    _everydayUpdateTime(""),
    _allCargo(NULL),
    _refreshNeedMoney(0),
    _refreshNeedPayKind(0),
    _mallId(0)
    {
        for ( int i=0 ; i< 3 ; ++i)
            _goodsKind[i] = -1;
        
        _allCargo = CCArray::createWithCapacity(50);
        _allCargo->retain();
    }
    
    ~SGMallData()
    {
        _allCargo->release();
    }
    
}; //SGMallData end.






#endif //the end