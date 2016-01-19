//
//  SGCommonInfoBox.h
//  GameSanGuo
//
//  Created by Fygo Woo on 10/23/14.
//
//

#ifndef __GameSanGuo__SGCommonInfoBox__
#define __GameSanGuo__SGCommonInfoBox__

#include "SGBaseBox.h"
class SGOfficerCard;
class SGEquipCard;
class SGPropsCard;
class SGPiecesCard;
class SGConsumableCard;
class SGMaterialCard;

//myEnterType的特殊数字，说明请参考create函数。
#define CIBET_TOTAL_LOGIN_REWARD 19491001
#define CIBET_FIND_HELP 19270701
#define CIBET_ALL_TASK_LAYER 201412116

/*
 * 以后可能需要提供的回调借口，暂时无用。
 */
class SGCommonInfoBoxDelegate
{
    
};

//开启此窗口的主要方式。
typedef enum
{
    SGCIBOT_MY_EQUIP_WEAR, //我的装备（已穿戴）
    SGCIBOT_MY_EQUIP_UNWEAR, //我的装备（未穿戴）
    SGCIBOT_OTHER_EQUIP, //其他人的装备
    SGCIBOT_PROTO_ITEM, //无主物品
    SGCIBOT_MS_ORDINARY_ITEM, //针对月签奖励设置，普通物品。
    SGCIBOT_MS_VIP_ITEM, //针对月签奖励设置，VIP物品。
} SGCibOpenType;

//开启此窗口的“获取”需求。
enum SGCibSpecialMode
{
    SGCSM_NULL, //无事。
    SGCSM_GOTO_ARENA, //由消耗品培养丹去“竞技场”。
};

/*
 * 通用的物品信息简略弹窗，方便快捷。
 */
class SGCommonInfoBox : public SGBaseBox
{
//function
public:
    //ctor
    SGCommonInfoBox();
    
    //dtor
    ~SGCommonInfoBox();
    
    //create 完整版。boxDeg=窗口代理供关闭回调，main=主物品；sub=副物品；otype=本次BOX类型；myEnterType=本BOX的进入类型；otherEnterType=详情页面的进入类型，为了与以前直接进入全屏版详情页面的代码兼容；otherIsBeforeDel默认为true，即删除以前的页面，但是有些页面非常特殊，所以这个参数需要留意；extArg=预留特殊参数，deg=预留本BOX代理。
    /*myEnterType:
     19491001=从“首页领奖-登入奖励”进来，特殊处理，关闭box时候不开启mainlayer触控。
     19270701=从”找友军帮忙"头像点击进来，特殊处理，不显示“详情”按钮。
     201412116=从“任务界面”点击进来，特殊处理，不显示“详情”按钮。
     */
    static SGCommonInfoBox* create(SGBoxDelegate* boxDeg = NULL, SGBaseMilitaryCard* main = NULL, SGBaseMilitaryCard* sub = NULL, SGCibOpenType otype = SGCIBOT_PROTO_ITEM, int myEnterType = -1, int otherEnterType = -1, bool otherIsBeforeDel = true, std::string extArg = "", SGCommonInfoBoxDelegate* deg = NULL, main::SigninCell* scData = NULL, SGCibSpecialMode sm = SGCSM_NULL);
    
    //@重要 原型级物品快捷调用窗口，参数请参考上方完整版。
    static SGCommonInfoBox* create(SGBaseMilitaryCard* main, SGBoxDelegate* boxDeg = NULL, int otherEnterType = -1, bool otherIsBeforeDel = true, int myEnterType = -1, SGCibSpecialMode sm = SGCSM_NULL);
    
    //@特殊 月签奖励物品快捷调用窗口，参数请参考上方。
    static SGCommonInfoBox* create(SGBoxDelegate* boxDeg, SGBaseMilitaryCard* main, SGCibOpenType otype, main::SigninCell* scData);
    
    //initView
    void initView();
    
    //兼容处理
    virtual void boxClose();
    
    //兼容处理
    virtual void boxCloseWithOutCallBack();
    
    //阻挡其他点击。
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

private:
    //转生装备
    void advanceEquipHandler();
    
    //查看详情
    void viewDetailHandler();
    
    /* 以下为从SGGeneralEquipBox转移过来的兼容代码。*/
    
    //穿戴装备
    void wearEquipHandler();
    
    //卸下装备
    void unwearEquipHandler();
    
    //穿戴装备回调
    void wearEquipListener(CCObject *obj);
    
    //卸下装备回调
    void unwearEquipListener(CCObject *obj);
    
    //强化装备
    void strengthenEquipHandler();
    
    //特殊物品前往功能的回调
    void gotoCallback();
    
//variable
private:
    //主要物品。
    SGBaseMilitaryCard* _mainItem;
    
    //次要物品。
    SGBaseMilitaryCard* _subItem;
    
    //打开的类型。
    SGCibOpenType _opentype;
    
    //特殊的模式。
    SGCibSpecialMode _specmode;
    
    //我的enterType，可能没用。
    int _myEnterType;
    
    //传入的其他的enterType，用于回溯。
    int _otherEnterType;
    
    //传入的其他的isBeforeDel，用于回溯。
    bool _otherIsBeforeDel;
    
    //特殊参数。
    std::string _extArg;
    
    //预留按钮借口。
    SGCommonInfoBoxDelegate* _cibDelegate;
    
    //可能的物品指针。
    SGOfficerCard* officer;
    SGEquipCard* equip;
    SGPropsCard* props;
    SGPiecesCard* pieces;
    SGConsumableCard* consumable;
    SGMaterialCard* material;
    
    //特殊数据指针
    main::SigninCell* _scData;
};

#endif /* defined(__GameSanGuo__SGCommonInfoBox__) */
