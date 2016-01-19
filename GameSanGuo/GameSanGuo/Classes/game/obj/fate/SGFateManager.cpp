//
//  SGFateManager.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/9/13.
//
//

#include "SGFateManager.h"
#include "SGOfficerCard.h"
#include "SGStaticDataManager.h"
#include "SGTeamgroup.h"
#include "SGFateBase.h"
#include "SGFateEffect.h"

//静态变量初始化
SGFateManager* SGFateManager::sgfm = NULL;

//创建
SGFateManager::SGFateManager() : isDataReady(false), fateBaseData(NULL), fateEffectData(NULL)
{
    ;
}

//析构
SGFateManager::~SGFateManager()
{
    ;
}

//单例
SGFateManager* SGFateManager::sharedManager()
{
    if (sgfm == NULL)
    {
        sgfm = new SGFateManager();
    }
    return sgfm;
}

//生成数据，初始化游戏时候进行
void SGFateManager::gainFateData()
{
    if (!isDataReady)
    {
        gainFateBaseData();
        gainFateEffectData();
        isDataReady = true;
    }
}

//获得某张武将卡的数值加成。  card：武将卡  type：0=血量，1=攻击，3=防御，4=速度
float SGFateManager::getAddedProperty(const SGOfficerCard* card, FateAttrType type)
{
    float ret = 0;
    std::vector<int> usefulVec = getUsefulFateList(card, type);
    if (usefulVec.size() >= 1) //若存在有用的缘分
    {
        std::vector<int>::iterator iter = usefulVec.begin();
        for (; iter != usefulVec.end(); iter++)
        {
            SGFateBase* sgfb = checkIsFateGoal(card, *iter);
            if ( sgfb ) //若该缘分条件满足，则将其添加到属性数值里
            {
                std::vector<int> efcVec = sgfb->getEffectList();
                std::vector<int>::iterator efcIter = efcVec.begin();
                for (; efcIter != efcVec.end(); efcIter++)
                {
                    SGFateEffect* sgfe = getFateEffectById(*efcIter); //缘分效果
                    if (sgfe)
                    {
                        if ( type == sgfe->getAttrType() ) //若缘分效果为所需求的加成
                        {
                            switch (sgfe->getValueType())
                            {
                                case FVT_PERCENT: //百分比添加
                                    {
                                        if (type == FAT_ATK)
                                            ret = (float) card->getBaseAtk() * sgfe->getValue() / 100.f;
                                        else if (type == FAT_DEF)
                                            ret = (float) card->getBaseDef() * sgfe->getValue() / 100.f;
                                        else if (type == FAT_MOR)
                                            ret = (float) card->getBaseMorale() * sgfe->getValue() / 100.f;
                                        else if (type == FAT_SPD)
                                            ret = (float) card->getBaseSpeed() * sgfe->getValue() / 100.f;
                                    }
                                    break;
                                case FVT_NUMBER: //数值添加
                                    {
                                        if (type == FAT_ATK)
                                            ret = (float) sgfe->getValue();
                                        else if (type == FAT_DEF)
                                            ret = (float) sgfe->getValue();
                                        else if (type == FAT_MOR)
                                            ret = (float) sgfe->getValue();
                                        else if (type == FAT_SPD)
                                            ret = (float) sgfe->getValue();
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
        
}

////检查某张武将卡缘分是否有用，即是否有该属性加成。这个用于前置判断，从而不用判断缘分条件是否达成。  card：武将卡  type：0=血量，1=攻击，3=防御，4=速度
////@deprecated
//bool SGFateManager::checkIsFateUseful(const SGOfficerCard* card, FateAttrType type)
//{
//    bool ret = false;
//    //检查该武将是否有缘分列表
//    std::vector<int> vec = card->getFateList();
//    if (vec.size() == 0) //没有则不管
//    {
//        ;
//    }
//    else //判断开始
//    {
//        std::vector<int>::iterator iter = vec.begin();
//        for (; iter != vec.end(); iter++)
//        {
//            if (checkIsFateBaseWork(*iter, type)) //有一个效果满足，则退出循环。
//            {
//                ret = true;
//                break;
//            }
//        }
//    }
//    return ret;
//}

//获得某张武将卡对特定属性有加成的缘分ID。  card：武将卡  type：0=血量，1=攻击，3=防御，4=速度
std::vector<int> SGFateManager::getUsefulFateList(const SGOfficerCard* card, FateAttrType type)
{
    std::vector<int> retVec;
    //检查该武将是否有缘分列表
    std::vector<int> vec = card->getFateList();
    if (vec.size() == 0) //没有则不管
    {
        ;
    }
    else //循环该武将的缘分ID列表
    {
        std::vector<int>::iterator iter = vec.begin();
        for (; iter != vec.end(); iter++)
        {
            if (checkIsFateBaseWork(*iter, type)) //将所有满足效果的缘分ID记录
            {
                retVec.push_back(*iter);
            }
        }
    }
    return retVec;
}

//检查某张某个缘分是否加成特定效果。  fateBase：缘分本体ID  type：0=血量，1=攻击，3=防御，4=速度
bool SGFateManager::checkIsFateBaseWork(int fateBase, FateAttrType type)
{
    bool ret = false;
    SGFateBase* sgfb = getFateBaseById(fateBase); //缘分本体
    if (sgfb && sgfb->getIsOn()) //必须该缘分开关打开才可继续
    {
        std::vector<int> efcVec = sgfb->getEffectList();
        std::vector<int>::iterator efcIter = efcVec.begin();
        for (; efcIter != efcVec.end(); efcIter++)
        {
            SGFateEffect* sgfe = getFateEffectById(*efcIter); //缘分效果
            if (sgfe)
            {
                if ( type == sgfe->getAttrType() ) //有一个效果满足则通过
                {
                    ret = true;
                    break;
                }
            }
        }
    }
    return ret;
}

//检查某张武将卡是否达成某个缘分条件，达成则直接返回本体，不达成则为NULL。  card：武将卡，fateId：缘分id
SGFateBase* SGFateManager::checkIsFateGoal(const SGOfficerCard* card, int fateId)
{
    SGFateBase* ret = NULL; //最终是否满足总标志
    SGFateBase* sgfb = getFateBaseById(fateId);
    //是否OK
    if (sgfb->getIsOn()) //首要条件必须开启
    {
        std::vector<int>* vec = NULL;
        std::vector<int>::iterator iter;
        CCObject* pobj = NULL;
        SGTeamgroup* sgtg = NULL;
        int team = 0;
        CCArray* folks; //阵列武将列表
        int amount = -1; //数量类的条件之需求数量
        int type = -1; //数量类的条件之需求类型，如国别/性别/类别/星级
        int many = 0; //数量类条件当前数量，用于累加
        switch (sgfb->getConditionType()) //根据条件类型判断，略复杂的业务。
        {
            case FCT_ERROR: //错误条件
                break;
            case FCT_OFFICER: //检查武将存在条件
            {
                //①检查是否在阵列里，不在则返回。
                sgtg = SGTeamgroup::shareTeamgroupInfo();
                team = sgtg->isOnEmbattle(card);
                if (0 == team)
                    return NULL;
                //②判断武将阵列。
                //即时解析武将相关条件
                vec = parseString( sgfb->getConditionString() );
                iter = vec->begin();
                pobj = NULL;
                ret = sgfb; //总标志，默认为符合最终条件
                //获得武将列表
                folks = sgtg->getEmbattleOfficers(team);
                for (; iter != vec->end(); iter++)
                {
                    bool ok = false; //副标志，是否含有条件武将，默认没有
                    CCARRAY_FOREACH(folks, pobj)
                    {
                        if ( ( (SGOfficerCard*) pobj )->getProtoId() == *iter ) //若队伍中有条件武将，副标志通过。
                        {
                            ok = true;
                            break;
                        }
                    }
                    if (!ok) //只要有一个副标志不通过，则最终不通过。
                    {
                        ret = NULL;
                        break;
                    }
                }
            }
                break;
            case FCT_EQUIP: //检查武将装备条件
            {
                //即时解析装备相关条件，无需在阵列里。
                vec = parseString( sgfb->getConditionString() );
                iter = vec->begin();
                pobj = NULL;
                ret = sgfb; //总标志，默认为符合最终条件
                //获得武将装备列表
                CCArray* equips = ( (SGOfficerCard*) card )->getOfficerEquips();
                for (; iter != vec->end(); iter++)
                {
                    bool ok = false; //副标志，是否含有条件装备，默认没有
                    CCARRAY_FOREACH(equips, pobj)
                    {
                        if ( ( (SGEquipCard*) pobj )->getProtoId() == *iter ) //若装备中有条件装备，副标志通过。
                        {
                            ok = true;
                            break;
                        }
                    }
                    if (!ok) //只要有一个副标志不通过，则最终不通过。
                    {
                        ret = NULL;
                        break;
                    }
                }
            }
                break;
            case FCT_NATION: //检查武将国家条件
            {
                //①检查是否在阵列里，不在则返回。
                sgtg = SGTeamgroup::shareTeamgroupInfo();
                team = sgtg->isOnEmbattle(card);
                if (0 == team)
                    return NULL;
                //②判断武将阵列。
                //即时解析武将国家相关条件，规则保证是两个逗分数字，如1,5。
                vec = parseString( sgfb->getConditionString() );
                iter = vec->begin();
                pobj = NULL;
                ret = NULL; //总标志，默认为不符合条件
                //获得武将列表
                folks = sgtg->getEmbattleOfficers(team);
                //获得所需国家类型&数量两参数
                type = (*vec)[0];
                amount = (*vec)[1];
                CCARRAY_FOREACH(folks, pobj)
                {
                    if ( ( (SGOfficerCard*) pobj )->getRace() == type ) //若相同国别，则判定++
                        many++;
                }
                if (many >= amount)
                    ret = sgfb;
                break;
            }
            case FCT_GENDER: //检查武将性别条件
            {
                //①检查是否在阵列里，不在则返回。
                sgtg = SGTeamgroup::shareTeamgroupInfo();
                team = sgtg->isOnEmbattle(card);
                if (0 == team)
                    return NULL;
                //②判断武将阵列。
                //即时解析武将性别相关条件，规则保证是两个逗分数字，如1,5。
                vec = parseString( sgfb->getConditionString() );
                iter = vec->begin();
                pobj = NULL;
                ret = NULL; //总标志，默认为不符合条件
                //获得武将列表
                folks = sgtg->getEmbattleOfficers(team);
                //获得所需性别类型&数量两参数
                type = (*vec)[0];
                amount = (*vec)[1];
                CCARRAY_FOREACH(folks, pobj)
                {
                    if ( ( (SGOfficerCard*) pobj )->getGender() == type ) //若相同性别，则判定++
                        many++;
                }
                if (many >= amount)
                    ret = sgfb;
            }
                break;
            case FCT_TYPE: //检查武将类型条件
            {
                //①检查是否在阵列里，不在则返回。
                sgtg = SGTeamgroup::shareTeamgroupInfo();
                team = sgtg->isOnEmbattle(card);
                if (0 == team)
                    return NULL;
                //②判断武将阵列。
                //即时解析武将类别相关条件，规则保证是两个逗分数字，如1,5。
                vec = parseString( sgfb->getConditionString() );
                iter = vec->begin();
                pobj = NULL;
                ret = NULL; //总标志，默认为不符合条件
                //获得武将列表
                folks = sgtg->getEmbattleOfficers(team);
                //获得所需类别类型&数量两参数
                type = (*vec)[0];
                amount = (*vec)[1];
                CCARRAY_FOREACH(folks, pobj)
                {
                    if ( ( (SGOfficerCard*) pobj )->getItemType() == type ) //若相同类别，则判定++
                        many++;
                }
                if (many >= amount)
                    ret = sgfb;
            }
                break;
            case FCT_STAR: //检查武将星级条件
            {
                //①检查是否在阵列里，不在则返回。
                sgtg = SGTeamgroup::shareTeamgroupInfo();
                team = sgtg->isOnEmbattle(card);
                if (0 == team)
                    return NULL;
                //②判断武将阵列。
                //即时解析武将类别相关条件，规则保证是两个逗分数字，如1,5。
                vec = parseString( sgfb->getConditionString() );
                iter = vec->begin();
                pobj = NULL;
                ret = NULL; //总标志，默认为不符合条件
                //获得武将列表
                folks = sgtg->getEmbattleOfficers(team);
                //获得所需星级类型&数量两参数
                type = (*vec)[0];
                amount = (*vec)[1];
                CCARRAY_FOREACH(folks, pobj)
                {
                    if ( ( (SGOfficerCard*) pobj )->getCurrStar() == type ) //若相同星级，则判定++
                        many++;
                }
                if (many >= amount)
                    ret = sgfb;
                break;
            }
            default:
                break;
        }
        //最后删除vec
        CC_SAFE_DELETE(vec);
    }
    return ret;
}

//解析缘分本体数据
void SGFateManager::gainFateBaseData()
{
    if (!fateBaseData)
    {
        fateBaseData = new CCDictionary();
    }
    
    CCDictionary* td = SGStaticDataManager::shareStatic()->getFateBaseData();
    CCDictElement* obj = NULL; //单个原始数据对象
    CCDictionary* real = NULL; //单个原始数据对象的值，用来初始化SGFateBase
    SGFateBase* sgfb = NULL; //单个缘分本体数据
    CCDICT_FOREACH(td, obj)
    {
        sgfb = new SGFateBase();
        //原始数据对象
        real = (CCDictionary*) obj->getObject();
        //设置
        sgfb->setId(  ((CCString*) real->objectForKey("id"))->intValue()  ); //ID
        sgfb->setName(  (CCString*) real->objectForKey("name") ); //名称
        sgfb->setConditionType( (FateConditionType) ((CCString*) real->objectForKey("cndType"))->intValue() ); //条件类型
        sgfb->setConditionString(((CCString*) real->objectForKey("cndStr"))); //条件文本
        //解析效果文本开始>>
        CCString* effect = (CCString*) real->objectForKey("effectStr");
        std::vector<int>* vec = parseString(effect);
        sgfb->setEffectList(*vec);
        CC_SAFE_DELETE(vec);
        //<<解析效果文本结束
        sgfb->setQuality( (FateQuality) ((CCString*) real->objectForKey("quality"))->intValue() ); //品质
        sgfb->setIsOn( ((CCString*) real->objectForKey("quality"))->boolValue() ); //是否开启
        //缘分相关的信息
        CCString *srcInfo = ((CCString*) real->objectForKey("sourceInfo"));
        if (srcInfo)
        {
            sgfb->setSourceFateInfo( srcInfo ); // 缘分相关的来源
        }
        //永久存储
        fateBaseData->setObject(sgfb, obj->getStrKey());
    }
}

//解析缘分效果数据
void SGFateManager::gainFateEffectData()
{
    if (!fateEffectData)
    {
        fateEffectData = new CCDictionary();
    }
    
    CCDictionary* td = SGStaticDataManager::shareStatic()->getFateEffectData();
    CCDictElement* obj = NULL; //单个原始数据对象
    CCDictionary* real = NULL; //单个原始数据对象的值，用来初始化SGFateEffect
    SGFateEffect* sgfe = NULL; //单个缘分效果数据
    CCDICT_FOREACH(td, obj)
    {
        sgfe = new SGFateEffect();
        //原始数据对象
        real = (CCDictionary*) obj->getObject();
        //设置
        sgfe->setId(  ((CCString*) real->objectForKey("id"))->intValue()  ); //ID
        sgfe->setAttrType(  (FateAttrType) ((CCString*) real->objectForKey("attrType"))->intValue()  ); //属性类型
        sgfe->setValueType(  (FateValueType) ((CCString*) real->objectForKey("valueType"))->intValue()  ); //值类型
        sgfe->setValue(  ((CCString*) real->objectForKey("value"))->intValue()  ); //值
        //永久存储
        fateEffectData->setObject(sgfe, obj->getStrKey());
    }
}

//将字符串拆分为数字组合，逗号分隔。形如 1001,2002
std::vector<int>* SGFateManager::parseString(CCString* cs)
{
    std::string source = cs->m_sString; //"1,102,889,773,  ---";
    char spt = ',';
    std::string temp;
    int pos;
    int ak; //实际数字
    pos = source.find(spt);
    std::vector<int>* vec = new std::vector<int>();
    while (pos != -1)
    {
        temp = source.substr(0, pos);
        source = source.substr(pos + 1); //+1为跳过一个','符号
        pos = source.find(spt);
        ak = atoi(temp.c_str());
        if (ak != 0)
            vec->push_back(ak);
    }
    ak = atoi(source.c_str());
    if (ak != 0)
        vec->push_back(ak);
    return vec;
}


//获得某个缘分本体的数据。  id：缘分本体ID
SGFateBase* SGFateManager::getFateBaseById(int id)
{
    //char str[10];
    //sprintf(str, "%d", id);
    
    CCString * str  = CCString::createWithFormat("%d",id);
    return (SGFateBase*) fateBaseData->objectForKey( str->getCString() );
}

//获得某个缘分效果的数据。  id：缘分效果ID
SGFateEffect* SGFateManager::getFateEffectById(int id)
{
    char str[10];
    sprintf(str, "%d", id);
    return (SGFateEffect*) fateEffectData->objectForKey( str );
}
