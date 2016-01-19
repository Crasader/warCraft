//商城首页
//by: zyc
//date:2014-06-25


#ifndef __GameSanGuo__SGMallLayer__
#define __GameSanGuo__SGMallLayer__

#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SGMallData.h"

class SGMallCell;
class PointLayer;
class SGButton;

//商城入口-》返回使用
enum ShopEnterType
{
    SHOP_LAYER,//商城列表
    PALYER_INFO_LAYER,//角色详情页
    PVP_HOME_LAYER,//对战首页
    SHOU_JING_ZHOU_LAYER,//守荆州首页
    SPECIAL_BATTLE_LAYER,//特殊战斗首页
    ARENA_LAYER//竞技场首页
};

//商店编号：0=pvp 1=荆州 2=云游商人 3=洛阳鬼市 4=幻境商城 5=竞技场商店 …
enum ShopGUID
{
    SHOPID_PVP=0,
    SHOPID_JING_ZHOU=1,
    SHOPID_YUN_YOU=2,
    SHOPID_LUO_YANG=3,
    SHOPID_SPECIAL_BATTLE=4,
    SHOPID_ARENA=5
    //如有新的商店请在下边添加：
};

class SGMallUtil
{
public:
    //商品图片
    static CCSprite * createSpriteByCargoId(int  itemId , int type,int num=0 , bool isShowNum = false);
    
    //支付方式的图片
    static CCSprite * createSpriteByPayType(int type);
    
    //标题 或者是物品的描述文本
    static SGCCLabelTTF * createLabelTitleByCargoId(int type , int id , int fontSize = 24 , bool isDescriptor=false);
    
    //背包中物品的数量
    static int getNumByCargoIdAndType(int itemType , int itemId =-1 );
    
};



class SGMallLayer : public SGBaseTableLayer , public SNSScrollViewPageDelegate
{
public:
    
    ~SGMallLayer();
    
    // static create,
    static SGMallLayer * create(SGMallData * );
    
    //加载视图。
    void initView(SGMallData * mallData);
    
    //设置、返回当前分页。滑动处理时用得到。
    inline void setPage(int page){ _nowPage = page;}
    inline int getPage(){ return _nowPage;}
    
    //根据indexPath返回tableView的cellItem
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    //返回tableView的列数
    virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section) { return 3; }
    
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section) { return this->datas->count(); }

    //override
    // 设置翻页回调 type 1:pre 2:next
    virtual void refreshPage(int type);
    
    //点击了哪一个。
    void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
    {
        EFFECT_PLAY(MUSIC_BTN);
        _selectIndex = tableView->getItemPointerWithIndexPath(indexPath);//选择哪一item
    }
    
    //刷新商品消息的响应
    void refreshDataOnResponse(CCObject * );
    
    
    //购买物品后响应服务器消息
    void buyCargoOnResponse(CCObject *);
    
    
    //保存将要显示的数据
    SGMallData * _mallData;
    
    
    
    
    //保存购买物品cargoinfo的指针 ，若购买成功则只修改其状态 已售出。
    static CargoInformation * _cargoInfoBuySuccess;
    
    //保存点击详情时的CellLayer指针 方便购买之后直接颜色变黑
    static SGMallCell * _buyMallCell;
    
    //发送刷新物品消息   金钱不足 返回false。 不刷新
    bool sendRefreshMsg();
    
    //在点击购买后   等待服务器消息返回，之后才可以继续购买。 warnning 第一次的情况  ，因此初始化为true
    static bool _isBuyResponsed ;
    
    static void setEnterTyppe(ShopEnterType  type){m_enterType = type;}
protected:
    
//    virtual void
    
    //ctor
    SGMallLayer();
    
    PointLayer * _pointLayer;
private:
    //进入商城入口
    static ShopEnterType  m_enterType;
    //返回按钮点击的回调。
    void onBackBtnClicked(CCObject *);
    
    //点击刷新物品的按钮
    void onRefreshBtnClicked(CCObject *);
    
    //可以翻页时 加上箭头。 点 还有左右滑动
    void showScrollGuid(bool bShow);
    
    //点击将 装  宝的回调。
    void onGoodsKindChanged( int kind );
    void onJiangClicked(CCObject *) ;
    void onZhuangClicked(CCObject *);
    void onBaoClicked( CCObject * );
    
    //数据分类
    void partData(SGMallData * mallData);
    
    //根据本次接收到的消息中所有商品的支付方式，加载背包中该支付方式的图标和数量
    void showMoneyInNowShop();
    
    //钱包中货币的背景。
    CCSprite * otherInfoBg;
    
    
    //总页数
    int _totalPage;
    //当前显示的分页，从 1 开始。
    int _nowPage;
    
    
    //当前分类的数据。
    CCArray * _currentData;
    //当前分页面的数据
    //CCArray * _currentPageData;
    
    //暂时没用上
    int _selectIndex;
    
    //选种物品时外边的圆圈，标识选中状态
    CCSprite * spriteSelectCircle;
    //左右滑动的箭头
    CCSprite * leftArrows;
    CCSprite * rightArrows ;
    
    //滑动页面时 下方的点阵 
    
    
    CCSprite * goodsKindBg;
    
    //天梯商城时保存 将 装 宝 的按钮。
    SGButton * sprite_jiang;
    SGButton * sprite_zhuang;
    SGButton * sprite_bao;
    
    //刷新按钮
    SGButton * refreshBtn;
    
    //保存商城类别下 需要的  背包中含所有 代币的type 和 数字label的指针
    std::map<int ,SGCCLabelTTF * > _moneyIconAndNum;
    
    

}; //SGMallLayer end.







#endif //the end