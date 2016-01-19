//
//  SGSoldier.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-6.
//
//

#include "SGSoldier.h"


SGSoldier::SGSoldier():sb_ap(0)
,sg_RoundV(0)//回合数
,sb_def(0)//防御力
,sb_Init_Def(0)//小兵初始防御力
,sb_Def_Rend(0)//防御加成系数
, sb_Id(0)//ID
, sb_Add_Ap(0)   //每回合加多少
, sb_Add_RoundV(0)//多少回合
, sb_Add_Chain(0)//连锁次数
, sb_Max_Ap(0)  //最大ap
, sb_Sp_Add_Ap(0)//其它影响的ap
, sb_SkillsId(0)//技能id 为0无
, sb_AttributeNum(0)
, sb_AttributeNum1(0)
, sb_AttributeNum2(0)//不读表参数
, sb_buffId(0)     //buff id
, sb_buffRound(0)//持续回合
, sb_buffAttribute(0)//buff的伤害
, buff_Type(0)
, effectType(0)
, sb_AddDef(0)//国家加成防御系数
, sb_Chain_Num(0)//连锁融合加成系数
, sb_RoundV_AddDef(0)//每回合加成
, sb_Atkmode(0)//后的攻击方式 0为近程 1为远程
, sb_AtkMode2(0)//远程的用的飞行的光效
, sb_AtkModeSkill(0)//技能远程用的飞行光效
, sb_HeadId(0)//被动技头像ID可能是武将vs头像？？、
, sb_SkillHead(0)//被动技头像ID
, sb_FileId(0)//人物 图片ID
, sb_Pair(0)//是否画2列箭  1为2列
, sb_Hit_Eff(0)//最后的打击效果 （近程 远程）
, sb_FriendOff(false)//是否是好友武将
, is_Def_Type(false)//是否是防御
, m_attackEffect(0)
, m_deathEffect(0)
, sb_resetInitDef(0)
, sb_resurrectionScale(0)//复活时血比例
, sb_skLevel(0)//技能=级
, sb_DefeatPlus(0)
{
    
}

SGSoldier::~SGSoldier()
{
    
}

void SGSoldier::setAp(int appVal)
{
    sb_ap = appVal;
}

SGSBObj::SGSBObj()
:p_mSid(0)
,P_mColorId(0)
,p_mtype(0)
,p_ap(0)
,p_def(0)
,sb_Init_Def(0)
,sb_Def_Rend(0)
,p_round(0)
,sb_SkillsId(0)//技能id 为0无
,sb_AttributeNum(0)// 技能伤害
,sb_AttributeNum1(0)//。。。
,sb_AttributeNum2(0)//。。。
,sb_buffId(0)         //buffid
,sb_buffRound(0)    //buff回合
,sb_buffAttribute(0)//buff 伤害
,buff_Type(0)    //buff类型
,effectType(0) //特效类型
,sb_AddDef(0)    //每回合加的防
,sb_Add_Ap(0)   //每回合加的攻
,sb_Chain_Num(0)//连锁融合加成系数
,sb_RoundV_AddDef(0)//每回合加成
,sb_Atkmode(0)//攻击方式
,sb_AtkMode2(0)//弓箭ID
,sb_AtkModeSkill(0)//技能箭id
,sb_HeadId(0)//被动技头像ID//可能是vs 图像
,sb_skillHead(0)// 武将计头像
,sb_FileId(0)//人物 图片ID
,sb_Pair(0)//是否画2列箭
,sb_Hit_Eff(0)//最的打击效果 （近程 远程）
,sb_FriendOff(false)//是否是好友武将
,is_Def_Type(false)//是否是防御
,_currentdef(0)//当前防御
,m_attackEffect(0)
,m_deathEffect(0)
,sb_resetInitDef(0)
,sb_resurrectionScale(0)//复活比例
,sb_skLevel(0)//技能=级
,sb_baseAttack(0)//未加成的攻击
,sb_baseDefend(0)//未加成的防御
{
    
}
SGSBObj::~SGSBObj()
{
    
}