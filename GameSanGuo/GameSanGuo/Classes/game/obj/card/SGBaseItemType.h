//
//  SGBaseCardType.h
//  GameSanGuo
//
//  Created by Fygo Woo on 10/24/14.
//
//

#ifndef GameSanGuo_SGBaseCardType_h
#define GameSanGuo_SGBaseCardType_h

/*
 * 关于所有游戏内物品的最终类型枚举。
 */
typedef enum
{
    BIT_INGOT = 0, //元宝
    BIT_GOLD = 1, //铜钱
    BIT_GRADE = 2, //PVP积分（天梯货币）
    BIT_FEAT = 3, //军功
    BIT_HORN = 4, //小喇叭
    BIT_RECIPE = 5, //炼丹秘方
    BIT_OFFICER = 6, //武将
    BIT_EQUIP = 7, //装备
    BIT_SOLDIER = 8, //士兵
    BIT_PROP = 9, //道具
    BIT_PIECE = 10, //碎片
    BIT_SOUL = 11, //军魂
    BIT_VIGOUR = 12, //体力
    BIT_GRAIN = 13, //军粮
    BIT_14 = 14, //未知
    BIT_CONSUMABLE = 15, //消耗品
    BIT_MEDAL = 16, //勋章
    BIT_MATERIAL = 17, //材料
    BIT_JADE = 18, //勾玉
    BIT_ARENA_SCORE = 19, //征服点（竞技场货币）
} SGBaseItemType;

#endif
