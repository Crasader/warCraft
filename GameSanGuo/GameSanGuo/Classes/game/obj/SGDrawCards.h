//
//  SGDrawCards.h
//  GameSanGuo
//
//  Created by dongboqian on 14-3-13.
//
//

#ifndef __GameSanGuo__SGDrawCards__
#define __GameSanGuo__SGDrawCards__
#include "cocos2d.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGGamePlayer.h"
#include "SGVipGifts.h"
#include "SGGamePlayerInfo.h"
#include "SGRankPlayer.h"
#include "SGLottery9ViewItem.h"
#include "SGSjzData.h"
#include "SGPiecesCard.h"
#include "SGConsumableCard.h"
#include "SGMaterialCard.h"
#include "SGStaticDataManager.h"
#include "SGBaseLayer.h"
#include "SGMainManager.h"
#include "ItemModule.pb.h"


USING_NS_CC;
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"
USING_NS_CC_EXT;

/*
 * ICON绘制相关的完整信息。
 */
struct SGIconInfo
{
    SGIconInfo() : itemType(BIT_GOLD), itemId(0), quality(2), haveNameBg(false), havePriceBg(false), priceQuality(0)
    {
        ;
    }
    
    //备用的数据参数。
    SGBaseItemType itemType; //物品类型。
    int itemId; //物品ID。
    int quality; //物品品质，用于取颜色。
    //图标。
    std::string iconImage; //物品图标本尊。
    std::string frameImage; //物品边框。
    std::string maskImage; //物品遮罩，碎片专属。
    std::string nationImage; //国家从属，武将专属。
    //右上角转生。
    std::string advNumLabel; //物品转生等级。
    //正下方数量。
    std::string amountLabel; //物品数量。
    //下框外名称。
    std::string nameLabel; //物品名称。
    bool haveNameBg; //物品名称背板。
    //下框外价格。
    int priceQuality; //货币品质。
    std::string priceImage; //货币图形
    std::string priceLabel; //价格文本
    bool havePriceBg; //价格背板。
};

/*
 * 图标绘制的形式。
 * 纯粹版：单图。（无任何边框等多余信息。）
 * 简略版：图标。
 * 普通版：图标/右上角转生。
 * 复杂版：图标/右上角转生/正下方数量。
 * 详情版：图标/右上角转生/正下方数量/下框外名称。
 * 商品版：图标/右上角转生/正下方数量/下框外价格。
 *
 * PS：特殊信息主要是转生次数的"+?"。
 */
enum SGIconType
{
    SGIT_PURE = 0,
    SGIT_SIMPLE,
    SGIT_NORMAL,
    SGIT_COMPLEX,
    SGIT_DETAILED,
    SGIT_GOODS,
};

class SGDrawCards : public CCObject
{
public:
    SGDrawCards();
    ~SGDrawCards();
    //以下方法全部废弃>>
    static  CCSprite *drawofficercard(int itemid,LayerTag tag, int num = 0);//画武将
    static  CCSprite *drawcoinetccard(int itemid,LayerTag tag, int num = 0);//画铜钱\元宝\军功\小喇叭\炼丹 且输入itemid即iconid,元宝id为10000
    static  CCSprite *drawpropcard(int itemid,LayerTag tag, int num = 0);//画道具
    static  CCSprite *drawequipcard(int itemid,LayerTag tag, int num = 0);//画装备
    static  CCSprite *drawpiececard(int itemid,LayerTag tag, int num = 0);//画碎片
    static  CCSprite *drawconsumecard(int itemid,LayerTag tag, int num = 0);//画消耗器
    static  CCSprite *drawmaterialcard(int itemid,LayerTag tag, int num = 0);//画装备转生材料
    static  CCSprite *drawallcards(int itemid,int type,LayerTag tag, int num = 0);//给出itemid,类型id,layertag可以直接画所有的card,但元宝还是item要是10000,type与服务器给的type一致
    //以上方法全部废弃<<
    
    /* 说明：
     * Pure：纯粹，只有单图。（无任何边框等多余信息。）
     * Simple：简略，有图标。
     * Normal：普通，有图标，有右上角转生。
     * Complex：复杂，有图标，有右上角转生，也有正下方数量。
     * Detailed：详情，由图标，由右下角转生，也有正下方数量，还有下框外的物品名称。haveNameBg可以控制是否添加名称的半透明底框。
     * 必读：货币类型（元宝/铜钱/炼丹秘方/军魂/体力/军粮/PVP积分/勋章/勾玉）不需要传入itemId，写0即可。
     **/
    
    /* 这里的两个接口是通过消息类ItemDataProto或者自建类型SGItemDataProro(暂未实用)创建ICON，以后可能会规范到只调用这两个接口。 */
    //奥义图标。
    static CCSprite* createSprite(const main::ItemDataProto& idp, LayerTag laTag, SGIconType iconType, bool haveNameBg = true);
    //奥义按钮。
    static SGButton* createButton(const main::ItemDataProto& idp, LayerTag laTag, SGIconType iconType, CCObject *target, SEL_MenuHandler selector, bool haveNameBg = true);
    //商品按钮
    static SGButton* createButtonInGoods(const main::GoodsDataProto& gdp, LayerTag laTag, SGIconType iconType, CCObject *target, SEL_MenuHandler selector, bool havePriceBg = true);
    
    /* 以下为图标。 */
    static CCSprite* createPureSprite(int itemid, int itemType, LayerTag laTag);
    static CCSprite* createSimpleSprite(int itemid, int itemType, LayerTag laTag);
    static CCSprite* createNormalSprite(int itemid, int itemType, LayerTag laTag);
    static CCSprite* createComplexSprite(int itemid, int itemType, int itemNum, LayerTag laTag);
    static CCSprite* createDetailedSprite(int itemid, int itemType, int itemNum, LayerTag laTag, bool haveNameBg = true);
    
    /* 以下为按钮。*/
    static SGButton* createSimpleButton(int itemid, int itemType, LayerTag laTag, CCObject *target, SEL_MenuHandler selector);
    static SGButton* createNormalButton(int itemid, int itemType, LayerTag laTag, CCObject *target, SEL_MenuHandler selector);
    static SGButton* createComplexButton(int itemid, int itemType, int itemNum, LayerTag laTag, CCObject *target, SEL_MenuHandler selector);
    static SGButton* createDetailedButton(int itemid, int itemType, int itemNum, LayerTag laTag, CCObject *target, SEL_MenuHandler selector, bool haveNameBg = true);
    
private:
    //计算图标相关信息，并且绑定资源。
    static void figureAndBind(int itemid, int itemType, int itemNum, LayerTag laTag, SGIconType iconType, SGIconInfo& info, bool haveNameBg = true);
    
    //计算商品相关信息，并且绑定资源。
    static void figureAndBindInGoods(int priceType, int priceValue, LayerTag laTag, SGIconInfo& info, bool havePriceBg = true);
    
    //生成一个图标。
    static CCSprite* generateSprite(SGIconInfo& info);
};

#endif /* defined(__GameSanGuo__SGDrawCards__) */

//备案，原来的货币类型对应的itemId
//现用：元宝=10000 PVP积分/天梯积分=9995 军魂=9994 体力=9993 军粮=9992 勋章=9991 金币=9999 秘方=9996
//废弃：军功=9997 小喇叭=9998

