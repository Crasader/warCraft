//
//  SGOfficerCard.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-7.
//
//武将卡信息

#ifndef __GameSanGuo__SGOfficerCard__
#define __GameSanGuo__SGOfficerCard__
#include "SGBaseMilitaryCard.h"
#include "SGEquipCard.h"

//武将卡性别枚举
enum OfficerGender
{
    OG_SHEMALE = 0, //人妖
    OG_MALE = 1, //男
    OG_FEMALE = 2, //女
};

class SGOfficerCard : public SGBaseMilitaryCard
{
private:
    //
    CCArray *equips;
    CCArray *g_advance;
public:    
    virtual const char * getClassName(){return "SGOfficerCard";};

    //  被动技Id
    CC_SYNTHESIZE(int, g_officerSkillId, OfficerSkil);
//  主将计Id(不用了)
    CC_SYNTHESIZE(int, g_lordSkillId, LordSkill);
//  主将计Id
    CC_SYNTHESIZE(int, g_times, LordId);
//  被动技经验
    CC_SYNTHESIZE(int, g_skillExp,SkillExp);
    //  主将计经验
    CC_SYNTHESIZE(int, g_lordExp,LordExp);
    //武将性别，新增
    CC_SYNTHESIZE(OfficerGender, g_gender, Gender);
    //武将缘分列表
    CC_SYNTHESIZE_PASS_BY_REF(std::vector<int>, g_fateList, FateList);
    //OFFICER速度修正系数
    CC_SYNTHESIZE(float, m_speedFactor, SpeedFactor);
    //武将计最大等级
    CC_SYNTHESIZE(int, m_skillmaxlevel, SkillMaxLevel);
    //主将计最大等级
    CC_SYNTHESIZE(int, m_lordmaxlevel, LordMaxLevel);
    //获得攻击力。不计算缘分
    int getBaseAtk() const;
    
    //获得防御力。不计算缘分
    int getBaseDef() const;
    
    //获得血量值。不计算缘分
    int getBaseMorale() const;
    
    //获得速度值。不计算缘分
    float getBaseSpeed() const;
    
    /*武将培养新加字段。
     *@：除图鉴及类图鉴处武将攻防血速这些原生属性客户端自行计算，其他情况皆由服务器推送
     *@:请此日期后取此字段使用。2014-12-22
     */
    CC_SYNTHESIZE(int, m_realAtk, RealAtk);
    CC_SYNTHESIZE(int, m_realDef, RealDef);
    CC_SYNTHESIZE(int, m_realMorale, RealMorale);
    CC_SYNTHESIZE(int, m_realSpeed, RealSpeed);
    
    //获得攻击力。计算缘分之后
    int getAtk() const;
    
    //获得防御力。计算缘分之后
    int getDef() const;
    
    //获得血量值。计算缘分之后
    int getMorale() const;
    
    //获得速度值。计算缘分之后
    float getSpeed() const;
    //转生材料数量
    CC_SYNTHESIZE(int, m_advancestuffnum, AdvanceStuffNum);
    //转生排序权重
    CC_SYNTHESIZE(int, m_adsortnum, AdvSortNum);
    
    //MM: 仅仅为探访页面设计，如果探访获得的武将会碎裂，则由本字段表示其被转化成的碎片数量，默认为0，表示并没有碎裂。为什么？因为探访页面只接受SGOfficerCard为其数据的参数，省去额外添加奇怪的参数。
    CC_SYNTHESIZE(int, m_crashNum, CrashNum);
    
    //武将潜力点
    CC_SYNTHESIZE(int, m_devPoint, DevPoint);
    
    //是否已经保存培养
    CC_SYNTHESIZE(bool, m_hasSvaedDev, HasSvaedDev);

public:
    SGOfficerCard();
    ~SGOfficerCard();
    
    void addEquip(SGEquipCard *equipCard);
    void removeEquip(SGEquipCard *equipCard);
    void addReqitemId(int itemId);
    CCArray *getRequitemIds();    
    CCArray *getOfficerEquips();
    
    //该武将是否可以转生，条件：碎片数量不小于需求数量且未达到上限
    bool selfCanAdvance();
    //该武将可以强化
    bool selfShowStrengthNotice();
    //武将本身是否该显示红点提示，条件：可以转生、可穿戴某个装备、其装备可转身。任一
    bool canShowNotice();
    //拥有闲置 且未穿戴
    bool canWearEquip(int equipType);
    //武将是否穿戴某个装备 参数为装备类型 武器、铠甲、饰品、坐骑
    bool isWearEquip(int equipType);
    //武将身上的某个装备是否可以转生
    bool hisEquipCanAdv(int equipType);
    //武将身上的某个装备可以强化
    bool hisEquipCanStreng(int equipType);
    //武将是否显示可培养提示
    bool selfCanDeveloping();
};


#endif /* defined(__GameSanGuo__SGOfficerCard__) */
