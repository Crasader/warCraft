//
//  SGSoldier.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-6.
//
//

#ifndef __GameSanGuo__SGSoldier__
#define __GameSanGuo__SGSoldier__
#include "GameConfig.h"
#include "CCSpriterX.h"

//cgpSpine
#include "cocos-ext.h"
USING_NS_CC_EXT;

USING_NS_CC;

typedef enum
{
    kcolorRed,
    kcolorGreen,
    kcolorBlue,
    
}sgColorType;
class SGSoldier : public CCNode
{
public:
    SGSoldier();
    ~SGSoldier();

    CC_SYNTHESIZE(CCSkeletonAnimation*, mp_SbSpine, SbSpine)
    CC_SYNTHESIZE(bool, bIsSpine, IsSpine);//cgpSpine
    
    CC_SYNTHESIZE(int, sg_RoundV, RoundV);//回合数
    CC_SYNTHESIZE_READONLY(int, sb_ap, Ap);//攻击力
    CC_SYNTHESIZE(int, sb_def, Def);//防御力
    CC_SYNTHESIZE(int, sb_Init_Def, InitDef);//小兵初始防御力
    CC_SYNTHESIZE(float, sb_Def_Rend, DefRend);//防御加成系数
    CC_SYNTHESIZE(sgColorType, sbColor, SbColor);//士兵的颜色属性 0, 1, 2 : R G B
    CC_SYNTHESIZE(int, sb_Id, ID);//ID
    CC_SYNTHESIZE(int, sb_Add_Ap, AddAp);   //每回合加多少
    CC_SYNTHESIZE(int, sb_Add_RoundV, AddRoundV);//多少回合
    CC_SYNTHESIZE(int, sb_Add_Chain, AddChain);//连锁次数
    CC_SYNTHESIZE(int, sb_Max_Ap, Max_Ap);  //最大ap
    CC_SYNTHESIZE(int, sb_Sp_Add_Ap, SpAddAp);//其它影响的ap
    CC_SYNTHESIZE(int, sb_SkillsId, SkillsId);//技能id 为0无
    CC_SYNTHESIZE(float, sb_AttributeNum, AfftributeNum);
    CC_SYNTHESIZE(float, sb_AttributeNum1, AfftributeNum1);
    CC_SYNTHESIZE(int , sb_AttributeNum2, AfftributeNum2);//不读表参数
    CC_SYNTHESIZE(int , sb_buffId, BuffId);     //buff id
    CC_SYNTHESIZE(int, sb_buffRound, BuffRound);//持续回合
    CC_SYNTHESIZE(float, sb_buffAttribute, BuffAfftribute);//buff的伤害
    CC_SYNTHESIZE(int, buff_Type, BuffType);
    CC_SYNTHESIZE(int, effectType, EffectType);
    
    CC_SYNTHESIZE(float, sb_AddDef, AddDef);//国家加成防御系数
    CC_SYNTHESIZE(float, sb_Chain_Num, ChainNUm);//连锁融合加成系数
    CC_SYNTHESIZE(float, sb_RoundV_AddDef, RoundVAddDef)//每回合加成
    
    CC_SYNTHESIZE(std::string, sb_Name, Name);//兵的名字
    CC_SYNTHESIZE(int , sb_Atkmode, Atkmode);//后的攻击方式 0为近程 1为远程
    CC_SYNTHESIZE(int , sb_AtkMode2, AtkMode2);//远程的用的飞行的光效
    CC_SYNTHESIZE(int , sb_AtkModeSkill, AtkModeSkill);//技能远程用的飞行光效
    CC_SYNTHESIZE(int , sb_HeadId, HeadId);//被动技头像ID可能是武将vs头像？？、
    CC_SYNTHESIZE(int , sb_SkillHead, Skill_Head);//被动技头像ID
    CC_SYNTHESIZE(int, sb_FileId, FileId);//人物 图片ID
    
    
    //added by cgp
    CC_SYNTHESIZE(int, sb_flyId, FlyId);      //远程道具Id
    CC_SYNTHESIZE(int, sb_flyPosX, FlyPosX);   //远程道具偏移位置
    CC_SYNTHESIZE(int, sb_flyPosY, FlyPosY);
    
    CC_SYNTHESIZE(int, sb_Pair, Pair);//是否画2列箭  1为2列
    CC_SYNTHESIZE(int, sb_Hit_Eff, HitEff);//最后的打击效果 （近程 远程）
    
    CC_SYNTHESIZE(bool, sb_FriendOff, FriendOff);//是否是好友武将
    CC_SYNTHESIZE(bool, is_Def_Type, DefType);//是否是防御
    CC_SYNTHESIZE(int, m_attackEffect, AttackEffect);
    CC_SYNTHESIZE(int, m_deathEffect, DeathEffect);
    
    CC_SYNTHESIZE(int , sb_resetInitDef, ResetInitDef);
    CC_SYNTHESIZE(float, sb_resurrectionScale, ResurrectionScale);//复活时血比例
    CC_SYNTHESIZE(int, sb_skLevel, sbSkLevel);//技能=级
//    技能名称
    CC_SYNTHESIZE(std::string, sb_skName, sbSkName);
    
    
    CC_SYNTHESIZE(float, sb_DefeatPlus, sbDefeatPlus);
    CC_SYNTHESIZE(int, sb_BaseAttack, sbBaseAttack);//未加成的攻击
    CC_SYNTHESIZE(int, sb_BaseDefend, sbBaseDefend);//未加成的防御
public:
  //  virtual void draw();
    virtual void setAp(int appVal);
    
};
class SGSBObj :public cocos2d::CCObject
{
public:
    SGSBObj();
    ~SGSBObj();
    CC_SYNTHESIZE(int, p_mSid,Sid); //      人物id
    CC_SYNTHESIZE(int, P_mColorId, ColorId);//  人物色id
    CC_SYNTHESIZE(int, p_mtype, Type);//    人物类型
    CC_SYNTHESIZE(int, p_ap, Ap);// 当前攻击力
    CC_SYNTHESIZE(int, p_def, Def); //当前防
    CC_SYNTHESIZE(int, sb_Init_Def, InitDef);//小兵初始防御力
    CC_SYNTHESIZE(float, sb_Def_Rend, DefRend);//防御加成系数
    CC_SYNTHESIZE(int, p_round, Round);         //攻击回合
    CC_SYNTHESIZE(GameIndex, p_index, Index);   //人物位置
    CC_SYNTHESIZE(int, sb_SkillsId, SkillsId);//技能id 为0无
    CC_SYNTHESIZE(float, sb_AttributeNum, AfftributeNum);// 技能伤害
    CC_SYNTHESIZE(float, sb_AttributeNum1, AfftributeNum1);//。。。
    CC_SYNTHESIZE(int, sb_AttributeNum2, AfftributeNum2);//。。。
    CC_SYNTHESIZE(int , sb_buffId, BuffId);         //buffid
    CC_SYNTHESIZE(int, sb_buffRound, BuffRound);    //buff回合
    CC_SYNTHESIZE(float, sb_buffAttribute, BuffAfftribute);//buff 伤害
    CC_SYNTHESIZE(int, buff_Type, BuffType);    //buff类型
    CC_SYNTHESIZE(int, effectType, EffectType); //特效类型
    
    CC_SYNTHESIZE(float, sb_AddDef, AddDef);    //防上限
    CC_SYNTHESIZE(int, sb_Add_Ap, AddAp);   //每回合加的攻
    CC_SYNTHESIZE(float, sb_Chain_Num, ChainNUm);//连锁融合加成系数
    CC_SYNTHESIZE(float, sb_RoundV_AddDef, RoundVAddDef)//每回合加成
    CC_SYNTHESIZE(std::string, sb_Name, Name);//人物名字
    
    CC_SYNTHESIZE(int , sb_Atkmode, Atkmode);//攻击方式
    CC_SYNTHESIZE(int , sb_AtkMode2, AtkMode2);//弓箭ID
    CC_SYNTHESIZE(int , sb_AtkModeSkill, AtkModeSkill);//技能箭id
    CC_SYNTHESIZE(int , sb_HeadId, HeadId);//被动技头像ID//可能是vs 图像
    CC_SYNTHESIZE(int ,sb_skillHead,Skill_Head);// 武将计头像
    CC_SYNTHESIZE(int, sb_FileId, FileId);//人物 图片ID
    CC_SYNTHESIZE(int, sb_Pair, Pair);//是否画2列箭
    CC_SYNTHESIZE(int, sb_Hit_Eff, HitEff);//最的打击效果 （近程 远程）
    CC_SYNTHESIZE(bool, sb_FriendOff, FriendOff);//是否是好友武将
    CC_SYNTHESIZE(bool, is_Def_Type, DefType);//是否是防御
    CC_SYNTHESIZE(int, _currentdef, Currentdef);//当前防御
    CC_SYNTHESIZE(int, m_attackEffect, AttackEffect);
    CC_SYNTHESIZE(int, m_deathEffect, DeathEffect);
    CC_SYNTHESIZE(int , sb_resetInitDef, ResetInitDef);
    CC_SYNTHESIZE(float, sb_resurrectionScale, ResurrectionScale);//复活比例
    CC_SYNTHESIZE(int, sb_skLevel, sbSkLevel);//技能=级
    
    CC_SYNTHESIZE(int, sb_baseAttack, sbBaseAttack);//未加成的攻击
    CC_SYNTHESIZE(int, sb_baseDefend, sbBaseDefend);//未加成的防御
    CC_SYNTHESIZE(float, sb_baseDefendPlus, sbBaseDefendPlus);//未加成的防御
public:
void print()
{
    CCLOG("Id%d",this->getSid());
    CCLOG("colorId%d",this->getColorId());
    CCLOG("type%d",this->getType());
    CCLOG("ap:%d", this->getAp());
};
};
#endif /* defined(__GameSanGuo__SGSoldier__) */
