//
//  SGFateManager.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/9/13.
//
//

#ifndef _GameSanGuo_SGFateManager_h_
#define _GameSanGuo_SGFateManager_h_

#include "cocos2d.h"

USING_NS_CC;

class SGOfficerCard;
class SGFateBase;
class SGFateEffect;

//缘分加成属性的枚举
typedef enum
{
    FAT_MOR = 0, //血量
    FAT_ATK, //攻击
    FAT_DEF, //防御
    FAT_SPD, //速度
} FateAttrType;

//缘分加成数值的类型枚举
typedef enum
{
    FVT_PERCENT = 0, //百分比
    FVT_NUMBER, //绝对值
} FateValueType;

//缘分达成的条件类型枚举
typedef enum
{
    FCT_ERROR = -1, //错误
    FCT_OFFICER = 0, //武将
    FCT_EQUIP, //装备
    FCT_NATION, //国家
    FCT_GENDER, //性别
    FCT_TYPE, //类型（国士无双等武将标签文本）
    FCT_STAR, //星级
} FateConditionType;

//缘分品质
typedef enum
{
    FQ_WHITE = 0, //白色
    FQ_GRENN, //绿色
    FQ_BLUE, //蓝色
    FQ_PURPLE, //紫色
    FQ_ORANGE, //橙色
} FateQuality;

class SGFateManager : public CCObject
{
public: //function
    //创建
    SGFateManager();
    
    //析构
    ~SGFateManager();
    
    //单例
    static SGFateManager* sharedManager();
    
    //获得某张武将卡的数值加成。  card：武将卡  type：0=血量，1=攻击，3=防御，4=速度
    float getAddedProperty(const SGOfficerCard* card, FateAttrType type);
    
//    //检查某张武将卡缘分是否有用，即是否有该属性加成。这个用于前置判断，从而不用判断缘分条件是否达成。  card：武将卡  type：0=血量，1=攻击，3=防御，4=速度
//    //@deprecated
//    bool checkIsFateUseful(const SGOfficerCard* card, FateAttrType type);
    
    //获得某张武将卡对特定属性有加成的缘分ID。  card：武将卡  type：0=血量，1=攻击，3=防御，4=速度
    std::vector<int> getUsefulFateList(const SGOfficerCard* card, FateAttrType type);
    
    //检查某张武将卡是否达成某个缘分条件，达成则直接返回本体，不达成则为NULL。  card：武将卡，fateId：缘分id
    SGFateBase* checkIsFateGoal(const SGOfficerCard* card, int fateId);
    
    //检查某张某个缘分是否加成特定效果。  fateBase：缘分本体ID  type：0=血量，1=攻击，3=防御，4=速度
    bool checkIsFateBaseWork(int fateBase, FateAttrType type);
    
    //获得某个缘分本体的数据。  id：缘分本体ID
    SGFateBase* getFateBaseById(int id);
    
    //获得某个缘分效果的数据。  id：缘分效果ID
    SGFateEffect* getFateEffectById(int id);
    
    //将字符串拆分为数字组合，逗号分隔。形如 1001,2002
    std::vector<int>* parseString(CCString* cs);
    
    //生成数据，初始化游戏时候进行
    void gainFateData();
    
private: //function
    //解析缘分本体数据
    void gainFateBaseData();
    
    //解析缘分效果数据
    void gainFateEffectData();

private: //variable
    //静态实例
    static SGFateManager* sgfm;
    
    //缘分数据是否完全解析完毕
    bool isDataReady;
    
    //缘分本体数据字典
    CCDictionary* fateBaseData;

    //缘分效果数据字典
    CCDictionary* fateEffectData;
};


#endif
