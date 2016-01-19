//
//  SGStaticDataManager.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-21.
//
//
#include "SGTestSwitch.h"
#include "SGStaticDataManager.h"
#include "SGFateManager.h"
#include "SGPlayerInfo.h"

//是否直接读取sanguo.zip，这时候的sanguo.zip直接放在Resource目录下，无需clean，默认关闭，仅供开发人员快速调试使用。
//@重要：若需上传本cpp，必须将此参数改为0，切记。
#define LOAD_DATA_WITHOUT_CLEAN 0

static SGStaticDataManager *staticData = NULL;

//根据id来转换成对应的数据表中的分组字段
const char *SGStaticDataManager::guide_Id[100]=
{
    "guide_tag_0",
    "guide_tag_1",
    "guide_tag_2",
    "guide_tag_3",
    "guide_tag_4",
    "guide_tag_5",
    "guide_tag_6",
    "guide_tag_7",
    "guide_tag_7_1",
    "guide_tag_7_2",
    "guide_tag_7_3",
    "guide_tag_8",
    "guide_tag_9",
    "guide_tag_10",
    "guide_tag_11",
    "guide_tag_12",
    "guide_tag_13",
    "guide_tag_14",
    "guide_tag_15",
    "guide_tag_16",
    "guide_tag_17",
    "guide_tag_18",
    "guide_tag_19",
    "guide_tag_20",
    "guide_tag_21",
    "guide_tag_22",
    "guide_tag_22_1",
    "guide_tag_23",
    "guide_tag_24",
    "guide_tag_25",
    "guide_tag_26",
//    "guide_tag_27",
//    "guide_tag_28",
//    "guide_tag_29",
    "guide_tag_30",
    "guide_tag_31",
    "guide_tag_32",
    "guide_tag_33",
    "guide_tag_34",
    "guide_tag_35",
    "guide_tag_36",
    "guide_tag_37",
    "guide_tag_38",
    "guide_tag_39",
    "guide_tag_40",
    "guide_tag_41",
    "guide_tag_42",
    "guide_tag_43",
    "guide_tag_44",
    "guide_tag_45",
    "guide_tag_46",
    "guide_tag_47",
    "guide_tag_48",
    "guide_tag_49",
    "guide_tag_50",
    "guide_tag_51",
    "guide_tag_52",
    "guide_tag_53",
    "guide_tag_54",
    "guide_tag_55",
    "guide_tag_56",
    "guide_tag_57",
    "guide_tag_58",
    "guide_tag_59",
    "guide_tag_60",
    "guide_tag_99",
};

const char *SGStaticDataManager::limit_Id[100]=
{
    "step_0",
    "step_1",
    "step_2",
    "step_3",
    "step_4",
    "step_5",
    "step_6",
    "step_7",
    "step_8",
    "step_9",
    "step_10",
    "step_11",
    "step_12",
    "step_13",
    "step_14",
    "step_15",
    "step_16",
    "step_17",
    "step_18",
    "step_19",
    "step_20",
    "step_21",
    "step_22",
    "step_23",
    "step_24",
    "step_25",
    "step_26",
    "step_27",
    "step_28",
    "step_29",
    "step_30",
    "step_31",
    "step_32",
    "step_33",
    "step_34",
    "step_35",
    "step_36",
    "step_37",
    "step_38",
    "step_39",
    "step_40",
    "step_41",
    "step_42",
    "step_43",
    "step_44",
    "step_45",
    "step_46",
    "step_47",
    "step_48",
    "step_49",
    "step_50",
    "step_51",
    "step_52",
    "step_53",
    "step_54",
    "step_55",
    "step_56",
    "step_57",
    "step_58",
    "step_59",
    "step_60",
    "step_99",
};


SGStaticDataManager::SGStaticDataManager()
{
    _dataMap = NULL;
    sbLocalDict = NULL;
    loadingOfficerDict = NULL;
    
    
    wujiangjiDict = NULL;
    zhujiangjiDict = NULL;
    officerDict = NULL;
    nickNameDict = NULL;
    soldierDict = NULL;
    propsDict = NULL;
    equipmentDict = NULL;
    cardTypeDict = NULL;
    battlewordDict = NULL;
    spotDict = NULL;
    activityDict = NULL;
    suitDict = NULL;
    vipDict = NULL;
    playerDict = NULL;
    expDict = NULL;
    taskDict = NULL;
    guideDict = NULL;
    fightGuideDict = NULL;
    autoGuideDict = NULL;
    fightwordDict = NULL;
    robotDict = NULL;
    tipDict = NULL;
    helpDict = NULL;
    helpinfoDict = NULL;
    fateBaseDict = NULL;
    fateEffectDict = NULL;
    protoOfficerDict = NULL;
    protoEquipmentDict = NULL;
    wujiangjicostDict = NULL;
    zhujiangjicostDict = NULL;
    limitGuideDict = NULL;
    visitViewDict = NULL;
    buffDict = NULL;
	piecesDict = NULL;
    rewardDict = NULL;
	consumeableDict = NULL;
    pvpjundutiao = NULL;
    weiOfficerDict = NULL;
    shuOfficerDict = NULL;
    wuOfficerDict = NULL;
    qunOfficerDict = NULL;
    equipViewDict = NULL;
    soldiergrowskillDict = NULL;
    soldierskillDict = NULL;
    mainTaskGroupDic=NULL;
    mainTaskSubDic=NULL;
    _controlValueDict = NULL;
    _lootWiKiDict = NULL;
    _materialLootWikiDict = NULL;
    _nationListDict = NULL;
    _skillFuzzyXml= NULL;
    materialDict = NULL;
    materialDependencyDict = NULL;
    m_bossDict = NULL;
    _soldierGrowPropertyMap = NULL;
    _baseCombatValueDict = NULL;
    _skillCombatValueDict = NULL;
    specialBattleDict = NULL;
    _fightOverNoticeDict = NULL;
    _fightOverNoticeBtnDict = NULL;
    spHelpRuleDict = NULL;
}

SGStaticDataManager::~SGStaticDataManager()
{
    CC_SAFE_DELETE(sbLocalDict);
    CC_SAFE_DELETE(loadingOfficerDict);
    
    CC_SAFE_RELEASE(wujiangjiDict);
    CC_SAFE_RELEASE(zhujiangjiDict);
    CC_SAFE_RELEASE(officerDict);
    CC_SAFE_RELEASE(nickNameDict);
    CC_SAFE_RELEASE(soldierDict);
    CC_SAFE_RELEASE(propsDict);
    CC_SAFE_RELEASE(equipmentDict);
    CC_SAFE_RELEASE(cardTypeDict);
    CC_SAFE_RELEASE(battlewordDict);
    CC_SAFE_RELEASE(spotDict);
    CC_SAFE_DELETE(m_bossDict);
    CC_SAFE_RELEASE(activityDict);
    CC_SAFE_RELEASE(suitDict);
    CC_SAFE_RELEASE(vipDict);
    CC_SAFE_RELEASE(playerDict);
    CC_SAFE_RELEASE(expDict);
    CC_SAFE_RELEASE(taskDict);
    CC_SAFE_RELEASE(guideDict);
    CC_SAFE_RELEASE(fightGuideDict);
    CC_SAFE_RELEASE(autoGuideDict);
    CC_SAFE_RELEASE(fightwordDict);
    CC_SAFE_RELEASE(robotDict);
    CC_SAFE_RELEASE(tipDict);
    CC_SAFE_RELEASE(helpDict);
    CC_SAFE_RELEASE(helpinfoDict);
    CC_SAFE_RELEASE(fateBaseDict);
    CC_SAFE_RELEASE(fateEffectDict);
    CC_SAFE_RELEASE(protoOfficerDict);
    CC_SAFE_RELEASE(protoEquipmentDict);
    CC_SAFE_RELEASE(wujiangjicostDict);
    CC_SAFE_RELEASE(zhujiangjicostDict);
    CC_SAFE_RELEASE(limitGuideDict);
    CC_SAFE_RELEASE(visitViewDict);
    CC_SAFE_RELEASE(buffDict);
    CC_SAFE_RELEASE(piecesDict);
    CC_SAFE_RELEASE(rewardDict);
    CC_SAFE_RELEASE(consumeableDict);
    CC_SAFE_RELEASE(pvpjundutiao);
    CC_SAFE_RELEASE(weiOfficerDict);
    CC_SAFE_RELEASE(shuOfficerDict);
    CC_SAFE_RELEASE(wuOfficerDict);
    CC_SAFE_RELEASE(qunOfficerDict);
    CC_SAFE_RELEASE(equipViewDict);
    CC_SAFE_RELEASE(soldiergrowskillDict);
    CC_SAFE_RELEASE(soldierskillDict);
    CC_SAFE_RELEASE(mainTaskGroupDic);
    CC_SAFE_RELEASE(mainTaskSubDic);
    CC_SAFE_RELEASE(_controlValueDict);
    CC_SAFE_RELEASE(_lootWiKiDict);
    CC_SAFE_DELETE(_materialLootWikiDict);
    CC_SAFE_RELEASE(_nationListDict);
    CC_SAFE_DELETE(_skillFuzzyXml);
    CC_SAFE_RELEASE(materialDict);
    CC_SAFE_RELEASE(materialDependencyDict);
    CC_SAFE_DELETE(_soldierGrowPropertyMap); //just delete is ok
    CC_SAFE_DELETE(_baseCombatValueDict);
    CC_SAFE_DELETE(_skillCombatValueDict);
    CC_SAFE_DELETE(specialBattleDict);
    CC_SAFE_DELETE(_fightOverNoticeBtnDict);
    CC_SAFE_DELETE(_fightOverNoticeDict);
    CC_SAFE_DELETE(spHelpRuleDict);
}

//加密码表
static unsigned char _codec[10] = {0b00010101, 0b00101010, 0b00110011, 0b00001100, 0b00101101, 0b00011101, 0b00011110, 0b00100100, 0b00100001, 0b00100010};

//解码，都采用字符串解码。
unsigned char* dehandle(unsigned char* & buffer, unsigned long bSize, bool isStr = true)
{
    int k = 0;
    for (int i = 0; i < bSize; i++)
    {
        buffer[i] ^= _codec[k];
        k++;
        if (k > 9)
        {
            k = 0;
        }
    }
    unsigned int realSize = (unsigned int)(isStr ? bSize + 1 : bSize);
    unsigned char* ko = new unsigned char[realSize];
    memset(ko, 0, realSize);
    memcpy(ko, buffer, bSize);

    buffer += bSize;
    return ko;
}

//解析数据集合文件。
void SGStaticDataManager::parseBinData()
{
    if (_dataMap)
    {
        return;
    }
    
    _dataMap = new std::map< std::string, std::pair<unsigned int, char*> >();
    //decryption
    unsigned long fsz;
    unsigned char* file;
    
    if (LOAD_DATA_WITHOUT_CLEAN)
    {
        file = CCFileUtils::sharedFileUtils()->getFileData("sanguo.zip", "rb", &fsz);
    }
    else
    {
        file = CCFileUtils::sharedFileUtils()->getFileData("sanguobigpic/sanguo.zip", "rb", &fsz);
    }
    unsigned char* fileMem = file;
    
    char* ret = (char*) dehandle(file, 2);
    int num = atoi(ret);
    CCLOG("PARSE || total file num=%d", num);
    
    for (int k = 0; k < num; k++)
    {
        //name
        char* nameLenChar = (char*) dehandle(file, 2);
        unsigned int nameLen = atoi(nameLenChar);
        
        char* nameChar = (char*) dehandle(file, nameLen);
        //file
        char* fileLenChar = (char*) dehandle(file, 10);
        unsigned int fileLen = atoi(fileLenChar);
        
        char* fileChar = (char*) dehandle(file, fileLen, false);
        CCLOG("PARSE || No.%d nameLenChar=%s, nameLen=%d, fileName=%s, fileLenChar=%s, fileLen=%d, ", k, nameLenChar, nameLen, nameChar, fileLenChar, fileLen);
        //data
        _dataMap->insert( std::pair< std::string, std::pair<unsigned int, char*> >(std::string(nameChar), std::make_pair(fileLen, fileChar)) );
        CC_SAFE_DELETE_ARRAY(nameLenChar);
        CC_SAFE_DELETE_ARRAY(fileLenChar);
        CC_SAFE_DELETE_ARRAY(nameChar);
    }
    
    CC_SAFE_DELETE_ARRAY(fileMem);
}

void SGStaticDataManager::loadPlist()
{
    //完成对数据集合文件的解析。
    this->parseBinData();
    
    sbLocalDict = readPlist(PLIST_SBLOCAL);
    sbLocalDict->retain();
    
    loadingOfficerDict = readPlist(PLIST_LOADING_OFFICER);
    loadingOfficerDict->retain();
    
    zhujiangjiDict = readPlist(PLIST_ZHUJIANGJI);
    zhujiangjiDict->retain();
    
    wujiangjiDict = readPlist(PLIST_WUJIANGJI);
    wujiangjiDict->retain();
    
    nickNameDict = readPlist(PLIST_NICKNAME);
    nickNameDict->retain();
    
    cardTypeDict = readPlist(PLIST_TYPE);
    cardTypeDict->retain();
    
    battlewordDict = readPlist(PLIST_BATTLEWORD);
    battlewordDict->retain();
    
    fightwordDict = readPlist(PLIST_FIGHTWORD);
    fightwordDict->retain();
    
    spotDict = readPlist(PLIST_SPOT);
    spotDict->retain();
    
    m_bossDict = readPlist(PLIST_CREAMINFO);
    m_bossDict->retain();
    
    activityDict = readPlist(PLIST_ACTIVITY);
    activityDict->retain();
    
    suitDict = readPlist(PLIST_SUIT);
    suitDict->retain();
    
    vipDict = readPlist(PLIST_VIP);
    vipDict->retain();
    
    playerDict = readPlist(PLIST_PLAYER);
    playerDict->retain();
    
    taskDict = readPlist(PLIST_TASK);
    taskDict->retain();
    
    fightGuideDict = readPlist(PLIST_FIGHTGUIDE);
    fightGuideDict->retain();
    
    limitGuideDict = readPlist(PLIST_LIMITUIGUIDE);
    limitGuideDict->retain();
    
    autoGuideDict = readPlist(PLIST_AUTOGUIDE);
    autoGuideDict->retain();
    
    robotDict = readPlist(PLIST_ROBOTGUIDE);
    robotDict->retain();
    
    tipDict = readPlist(PLIST_TIP);
    tipDict->retain();
    
    helpDict = readPlist(PLIST_HELP);
    helpDict->retain();
    
    helpinfoDict = readPlist(PLIST_HELPINFO);
	helpinfoDict->retain();
    
	piecesDict = readPlist(PLIST_PIECES);
    piecesDict->retain();
    
    wujiangjicostDict = readPlist(PLIST_WJJ_COST);
    wujiangjicostDict->retain();
    
    zhujiangjicostDict = readPlist(PLIST_ZJJ_COST);
    zhujiangjicostDict->retain();
    
    rewardDict = readPlist(PLIST_REWARD);
    rewardDict->retain();
    
    buffDict = readPlist(PLIST_BUFF);
    buffDict->retain();
    
    pvpjundutiao = readPlist(PLIST_JINDU);
    pvpjundutiao->retain();
    
    visitViewDict = readPlist(PLIST_VISITHUODONG);
    visitViewDict->retain();
    
    weiOfficerDict = readPlist(PLIST_WEIOFFICER);
    weiOfficerDict->retain();
    
    shuOfficerDict = readPlist(PLIST_SHUOFFICER);
    shuOfficerDict->retain();
    
    wuOfficerDict = readPlist(PLIST_WUOFFICER);
    wuOfficerDict->retain();
    
    qunOfficerDict = readPlist(PLIST_QUNOFFICER);
    qunOfficerDict->retain();
    
    equipViewDict = readPlist(PLIST_EQUPIPVIEW);
    equipViewDict->retain();
    
    _controlValueDict = readPlist(PLIST_CONTROLVALUE);
    _controlValueDict->retain();
    
    soldierskillDict = readPlist(PLIST_SOLDIER_SKILL);
    soldierskillDict->retain();
    
    soldiergrowskillDict = readPlist(PLIST_SOLDIER_GROW_SKILL);
    soldiergrowskillDict->retain();
    
    mainTaskGroupDic = readPlist(PLIST_MAINTASK);
    mainTaskGroupDic->retain();
    
    mainTaskSubDic = readPlist(PLIST_MAINTASKQUEST);
    mainTaskSubDic->retain();
    
    officerDict = readPlist(PLIST_OFFICER);
    officerDict->retain();
    
    expDict = readPlist(PLIST_EXP);
    expDict->retain();
    
    equipmentDict = readPlist(PLIST_EQUIPMENT);
    equipmentDict->retain();
    
    propsDict = readPlist(PLIST_PROPS);
    propsDict->retain();
    
    soldierDict = readPlist(PLIST_SOLDIER);
    soldierDict->retain();
    
    guideDict = readPlist(PLIST_UIGUIDE);
    guideDict->retain();
    
    fateBaseDict = readPlist(PLIST_FATE_BASE);
    fateBaseDict->retain();
    
    fateEffectDict = readPlist(PLIST_FATE_EFFECT);
    fateEffectDict->retain();
    
	consumeableDict = readPlist(PLIST_CONSUMABLE);
	consumeableDict->retain();
    
    _lootWiKiDict = readPlist(PLIST_LOOTWIKI);
	_lootWiKiDict->retain();
    
    _materialLootWikiDict = readPlist(PLIST_MATERIAL_LOOTWIKI);
	_materialLootWikiDict->retain();
    
    _nationListDict = readPlist(PLIST_NATIONLIST);
    _nationListDict->retain();
    
    _baseCombatValueDict = readPlist(PLIST_BASE_COMBAT_VALUE);
    _baseCombatValueDict->retain();
    
    _skillCombatValueDict = readPlist(PLIST_SKILL_COMBAT_VALUE);
    _skillCombatValueDict->retain();
    
    _fightOverNoticeDict = readPlist(PLIST_FIGHT_OVER_NOTICE);
    _fightOverNoticeDict->retain();
    
    _fightOverNoticeBtnDict = readPlist(PLIST_FIGHT_OVER_NOTICE_BTN);
    _fightOverNoticeBtnDict->retain();
    
    //特殊副本的规则表
    spHelpRuleDict = readPlist(PLIST_SPBATTLE_RULE);
    spHelpRuleDict->retain();
    
    
    //XML
    _skillFuzzyXml = readXml(XML_SKILL_FUZZY);
    
    
    //读取装备转生材料表
    materialDict = readPlist(PLIST_MATERIAL);
    materialDict->retain();
    
    //装备转生所需要的的材料的依赖表
    materialDependencyDict = readPlist(PLIST_MATERIALDEPENDENCY);
    materialDependencyDict->retain();
    
//    //特殊副本战斗关卡数据
//    specialBattleDict = readPlist(PLIST_SPECAIL_BATTLE);
//    specialBattleDict->retain();
    
    //生成原始ID级武将数据
    createProtoOfficerData();
    //生成原始ID级装备数据
    createProtoEquipmentData();
    //生成缘分相关数据
    createFateData();
    //生成士兵属性增量因子表
    createSoldierGrowProperty();
    
    //解析碎片
    m_allPieceDataModel = CCArray::create();
    m_allPieceDataModel->retain();
    createAllPieceDataModel();
    
    //战力
    m_allBaseCombatValueDataModel = CCArray::create();
    m_allBaseCombatValueDataModel->retain();
    m_allSkillCombatValueDataModel = CCArray::create();
    m_allSkillCombatValueDataModel->retain();
    
    this->createCombatValueDataModel();
    
    
    //解析fragmentnum 文件
    this->createFragmentNum();
        //清空数据包
    for (std::map< std::string, std::pair<unsigned int, char*> >::iterator iter = _dataMap->begin(); iter != _dataMap->end(); iter++)
    {
        CC_SAFE_DELETE_ARRAY(iter->second.second);
    }
    _dataMap->clear();
    CC_SAFE_DELETE(_dataMap);
}
//读plist文件
CCDictionary* SGStaticDataManager::readPlist(const char *filepath)
{
    CCLOG("readPlist: file=%s try", filepath);
    std::map< std::string, std::pair<unsigned int, char*> >::iterator iter = _dataMap->find(std::string(filepath));
    if (iter != _dataMap->end())
    {
        CCLOG("readPlist: file=%s ok", filepath);
        return CCFileUtils::sharedFileUtils()->createCCDictionaryWithBuffers(iter->second.second , iter->second.first);
    }
    // shall not come here.
    assert(1 == 0);
    // old codes below.
//    const char *staticDataPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(filepath);
//    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(staticDataPath);
//    return dict;
}

//读取xml文件，返回XMLDocument
tinyxml2::XMLDocument* SGStaticDataManager::readXml(const char* fileName)
{
    std::map< std::string, std::pair<unsigned int, char*> >::iterator iter = _dataMap->find(std::string(fileName));
    if (iter != _dataMap->end())
    {
        CCLOG("readXml: file=%s", fileName);
        tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
        tinyxml2::XMLError state = pDoc->Parse(iter->second.second , iter->second.first);
        if (tinyxml2::XML_NO_ERROR == state)
        {
            return pDoc;
        }
    }
    // shall not come here.
    assert(1 == 0);
}

//单例类
SGStaticDataManager* SGStaticDataManager::shareStatic()
{
    if(NULL == staticData)
    {
        staticData = new SGStaticDataManager();
    }
    return staticData;
}

CCDictionary* SGStaticDataManager::getwjjCostBylevel(int level)
{
    if(NULL == wujiangjicostDict)
    {
        wujiangjicostDict = readPlist(PLIST_WJJ_COST);
        wujiangjicostDict->retain();
    }
    CCDictionary *dict = (CCDictionary*)wujiangjicostDict->objectForKey(CCString::createWithFormat("%d",level)->getCString());
    
    return dict;
    
    
}

CCDictionary* SGStaticDataManager::getzjjCostBylevel(int level)
{
    if(NULL == zhujiangjicostDict)
    {
        zhujiangjicostDict = readPlist(PLIST_ZJJ_COST);
        zhujiangjicostDict->retain();
    }
    CCDictionary *dict = (CCDictionary*)zhujiangjicostDict->objectForKey(CCString::createWithFormat("%d",level)->getCString());
    
    return dict;
}


//通过Id获取 SGOfficerDataModel 对象
SGOfficerDataModel* SGStaticDataManager::getOfficerById(int officerId)
{
    if(NULL == officerDict)
    {
        officerDict = readPlist(PLIST_OFFICER);
        officerDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)officerDict->objectForKey(CCString::createWithFormat("%d",officerId)->getCString());
    if(!dict)
    {
        return NULL;
    }
    
    return dictToOfficer(dict);
}



//通过原型Id查找武将卡部分数据
SGOfficerDataModel* SGStaticDataManager::getOffcierByProtoId(int protoId)
{
    return (SGOfficerDataModel*) protoOfficerDict->objectForKey(protoId);
}

//通过piecesId解析碎片dict
SGPiecesDataModel *SGStaticDataManager::getPiecesDictById(int piecesId)
{
//    CCDictionary *dict = (CCDictionary *)piecesDict->objectForKey(CCString::createWithFormat("%d", piecesId)->getCString());//通过碎片id获得对应数据
//	if (!dict)
//	{
//		return NULL;
//	}
//
    SGPiecesDataModel * ret = NULL;
    SGPiecesDataModel * temp ;
    for(int i = 0 ;i < m_allPieceDataModel->count();++i)
    {
        temp = (SGPiecesDataModel *)m_allPieceDataModel->objectAtIndex(i);
        if(temp->getPiecesId() == piecesId)
        {
            ret = temp;
            break;
        }
        
    }
//	return dictToPieces(dict);
    return ret;
}

void SGStaticDataManager::createAllPieceDataModel()
{
    if (NULL == piecesDict)
	{
		piecesDict = readPlist(PLIST_PIECES);
		piecesDict->retain();
	}
    else
    {
        CCDictElement* obj = NULL; //单个原始数据对象
        CCDictionary* dict = NULL; //单个原始数据对象的值，用来初始化Model
        SGPiecesDataModel * piece = NULL; //装备原始数据
        CCDICT_FOREACH(piecesDict , obj)
        {
            dict = (CCDictionary*) obj->getObject();
            
            piece = dictToPieces(dict);
            m_allPieceDataModel->addObject(piece);
        }
    }
}



SGPiecesDataModel *SGStaticDataManager::getPiecesDictByOfficerProtoId(int officerProtoId)
{
    SGPiecesDataModel * ret = NULL;
    SGPiecesDataModel * temp ;
    for(int i=0 ; i<m_allPieceDataModel->count() ; ++i)
    {
        temp = (SGPiecesDataModel*)m_allPieceDataModel->objectAtIndex(i);
        if(officerProtoId == temp->getOfficerProtoId())
        {
            ret = temp;
            break;
        }
    }
    return ret;
}

void SGStaticDataManager::createCombatValueDataModel()
{
    if(NULL == _baseCombatValueDict)
    {
        _baseCombatValueDict = readPlist(PLIST_BASE_COMBAT_VALUE);
        _baseCombatValueDict->retain();
    }
    else
    {
        CCDictElement* obj = NULL; //单个原始数据对象
        CCDictionary* dict = NULL; //单个原始数据对象的值，用来初始化Model
        SGBaseCombatValueDataModel * model = NULL;
        CCDICT_FOREACH(_baseCombatValueDict , obj)
        {
            dict = (CCDictionary*) obj->getObject();
            
            model = dictToBaseCombatValue(dict);
            m_allBaseCombatValueDataModel->addObject(model);
        }

    }
    
    if(NULL == _skillCombatValueDict)
    {
        _skillCombatValueDict = readPlist(PLIST_SKILL_COMBAT_VALUE);
        _skillCombatValueDict->retain();
    }
    else
    {
        CCDictElement* obj = NULL; //单个原始数据对象
        CCDictionary* dict = NULL; //单个原始数据对象的值，用来初始化Model
        SGSkillCombatValueDataModel * model = NULL;
        CCDICT_FOREACH(_skillCombatValueDict , obj)
        {
            dict = (CCDictionary*) obj->getObject();
            
            model = dictToSkillCombatValue(dict);
            m_allSkillCombatValueDataModel->addObject(model);
        }
        
    }

}

SGBaseCombatValueDataModel * SGStaticDataManager::dictToBaseCombatValue(cocos2d::CCDictionary *dict)
{
    SGBaseCombatValueDataModel * ret = new SGBaseCombatValueDataModel ;
    
    ret->setType( dynamic_cast<CCString *>(dict->objectForKey("type") )->intValue() );
    ret->setStarLev( dynamic_cast<CCString *>(dict->objectForKey("star") )->intValue() );
    ret->setAdvNum( dynamic_cast<CCString *>(dict->objectForKey("smillStar") )->intValue() );
    ret->setCombatValue( dynamic_cast<CCString *>(dict->objectForKey("combatValue") )->intValue() );
    ret->setLevelRate( dynamic_cast<CCString *>(dict->objectForKey("levelRate") )->floatValue() );
    
    ret->autorelease();
    return ret;
}

SGSkillCombatValueDataModel * SGStaticDataManager::dictToSkillCombatValue(cocos2d::CCDictionary *dict)
{
    SGSkillCombatValueDataModel * ret = new SGSkillCombatValueDataModel;
    
    ret->setType( dynamic_cast<CCString *>(dict->objectForKey("type") )->intValue() );
    ret->setGrid( dynamic_cast<CCString *>(dict->objectForKey("area") )->intValue() );
    ret->setSkillLev( dynamic_cast<CCString *>(dict->objectForKey("level") )->intValue() );
    ret->setWeightA( dynamic_cast<CCString *>(dict->objectForKey("star3") )->intValue() );
    ret->setWeightB( dynamic_cast<CCString *>(dict->objectForKey("star4") )->intValue() );
    ret->setWeightC( dynamic_cast<CCString *>(dict->objectForKey("star5") )->intValue() );
    ret->setWeightD( dynamic_cast<CCString *>(dict->objectForKey("star6") )->intValue() );
    
    ret->autorelease();
    return ret;
}




//根据consumeId来取到对应的数据
SGConsumableDataModel *SGStaticDataManager::getConsumableById(int consumeId)
{
	if (NULL == consumeableDict)
	{
		consumeableDict = readPlist(PLIST_CONSUMABLE);
		consumeableDict->retain();
	}
	CCDictionary *dict = (CCDictionary *)consumeableDict->objectForKey(CCString::createWithFormat("%d", consumeId)->getCString());//通过碎片id获得对应数据
	if (!dict)
	{
		return NULL;
	}
	
	return dictToConsumable(dict);
}
//根据index获取单条规则的文本
std::string SGStaticDataManager::getSingleSpBattleRule(int index)
{
    if (spHelpRuleDict == NULL)
    {
        spHelpRuleDict = readPlist(PLIST_SPBATTLE_RULE);
        spHelpRuleDict->retain();
    }
    CCDictionary *dict = (CCDictionary *)spHelpRuleDict->objectForKey(CCString::createWithFormat("%d", index)->getCString());
    if (!dict)
    {
        return "";
    }
    
    std::string info(static_cast<CCString *>(dict->objectForKey("info"))->getCString());
    return info;
}

int SGStaticDataManager::getSpHelpInfoKeysNum()
{
    return spHelpRuleDict->count();
}

//根据materialID获取对应的material数据

SGMaterialDataModel *SGStaticDataManager::getMaterialById(int materialId)
{
    if (!this->materialDict)
    {
        materialDict = readPlist(PLIST_MATERIAL);
        materialDict->retain();
    }
    CCDictionary *dict = (CCDictionary *)materialDict->objectForKey(CCString::createWithFormat("%d", materialId)->getCString());
    if (!dict)
    {
        return NULL;
    }
    return dictToMaterial(dict);
}

//Material字典数据转换为dataModel
SGMaterialDataModel *SGStaticDataManager::dictToMaterial(CCDictionary *dict)
{
    SGMaterialDataModel *material = new SGMaterialDataModel();
    material->setMaterialId(static_cast<CCString *>(dict->objectForKey("materialId"))->intValue());
    material->setMaterialName(static_cast<CCString *>(dict->objectForKey("materialName")));
    material->setMaterialIconId(static_cast<CCString *>(dict->objectForKey("materialIconId"))->intValue());
    material->setMaterialBorderColor(static_cast<CCString *>(dict->objectForKey("materialBorderColor"))->intValue());
    material->setMaterialPileLimit(static_cast<CCString *>(dict->objectForKey("materialPileLimit"))->intValue());
    material->setMaterialSoldPrice(static_cast<CCString *>(dict->objectForKey("materialSoldPrice"))->intValue());
    material->setMaterialDesc(static_cast<CCString *>(dict->objectForKey("materialDesc")));
    material->setMaterialJokeInfo(static_cast<CCString *>(dict->objectForKey("materialMeng")));
    
    material->autorelease();
    return material;
}

SGMaterialDependencyDataModel *SGStaticDataManager::dictToMaterialDependency(CCDictionary *dict)
{
    SGMaterialDependencyDataModel *materialDepend = new SGMaterialDependencyDataModel();
    
    materialDepend->setMaterialDependId(static_cast<CCString *>(dict->objectForKey("materialDependencyId"))->intValue());
    materialDepend->setMaterialDependCondition(static_cast<CCString *>(dict->objectForKey("materialDependencyCondition")));
    
    materialDepend->autorelease();
    return materialDepend;
}


//装备转生所需要材料依赖表，根据需要材料组的id，获取对应的条件
SGMaterialDependencyDataModel *SGStaticDataManager::getMaterialDependencyById(int materialDependencyId)
{
    if (!this->materialDependencyDict)
    {
        materialDependencyDict = readPlist(PLIST_MATERIALDEPENDENCY);
        materialDependencyDict->retain();
    }
    CCDictionary *dict = (CCDictionary *)materialDependencyDict->objectForKey(CCString::createWithFormat("%d",materialDependencyId)->getCString());
    if (!dict)
    {
        return NULL;
    }
    return dictToMaterialDependency(dict);
}



//解析消耗品数据
SGConsumableDataModel *SGStaticDataManager::dictToConsumable(CCDictionary *dict)
{
	SGConsumableDataModel *consume  = new SGConsumableDataModel();
	consume->setCanUseDriving(((CCString *)dict->objectForKey("isUseDriving"))->intValue());
	consume->setIconId(((CCString *)dict->objectForKey("iconId"))->intValue());
	consume->setConsumId(((CCString *)dict->objectForKey("consumableId"))->intValue());
	consume->setLimitAndNum(((CCString *)dict->objectForKey("consumableNumAndLimit")));
	consume->setConsumeType(((CCString *)dict->objectForKey("consumableType"))->intValue());
	consume->setConsumeStarLvl(((CCString *)dict->objectForKey("consumableStarLevel"))->intValue());
	consume->setConsumePrice(((CCString *)dict->objectForKey("consumbablePrice"))->intValue());
	consume->setConsumeName(((CCString *)dict->objectForKey("consumableName")));
	consume->setConsumeDesc(((CCString *)dict->objectForKey("consumableDesc")));
	consume->setConsumePicId(((CCString *)dict->objectForKey("consumablePicId"))->intValue());
	consume->setMoneyType(((CCString *)dict->objectForKey("moneyType"))->intValue());
	consume->setConsumeJokeInfo(((CCString *)dict->objectForKey("consumableMeng")));
    
	consume->autorelease();
	return consume;
}


SGPiecesDataModel *SGStaticDataManager::dictToPieces(CCDictionary *dict)
{
	SGPiecesDataModel *pieces = new SGPiecesDataModel();
	
	pieces->setPiecesId(((CCString *)dict->objectForKey("itemId"))->intValue());//碎片对应的id
	pieces->setCallingCardId(((CCString *)dict->objectForKey("callCardId"))->intValue());//碎片合成对应之后的卡牌id
	pieces->setPiecesName(((CCString *)dict->objectForKey("name")));//碎片名称
	pieces->setPiecesPrice(((CCString *)dict->objectForKey("sellPrice"))->floatValue());//碎片价格

    pieces->setPiecesDesc(static_cast<CCString *>(dict->objectForKey("piecesDesc")));
	pieces->setcallNeedPieces(((CCString *)dict->objectForKey("summonNeedNumber"))->intValue());//合成总共需要的碎片数
	pieces->setPiecesType(((CCString *)dict->objectForKey("piecesType"))->intValue());//碎片的类型
	pieces->setStarLevel(((CCString *)dict->objectForKey("starLevel"))->intValue());//星级

	pieces->setMaxPiecesNum(((CCString *)dict->objectForKey("maxNumber"))->intValue());//持有的最大值
    pieces->setOfficerProtoId(((CCString *)dict->objectForKey("baseItemId"))->intValue());//对应武将的原型id
	pieces->setPiecesJokeInfo(static_cast<CCString *>(dict->objectForKey("piecesMeng")));
	pieces->autorelease();
	return pieces;
}

//返回一个 SGOfficerDataModel 对象
SGOfficerDataModel * SGStaticDataManager::dictToOfficer(CCDictionary *dict)
{
    SGOfficerDataModel *officer = new SGOfficerDataModel();
    
    officer->setOfficerId(((CCString*)dict->objectForKey("id"))->intValue());
    officer->setOfficerGoverning(((CCString*)dict->objectForKey("governing"))->intValue());
    officer->setOfficerBaseSpeed(((CCString*)dict->objectForKey("baseSpeed"))->floatValue());
    officer->setOfficerMaxLevel(((CCString*)dict->objectForKey("maxLevel"))->intValue());
    officer->setOfficerMorale(((CCString*)dict->objectForKey("morale"))->floatValue());
    officer->setOfficerType(1);//写成1在军营里才可以滑动
    officer->setOfficerBaseDef(((CCString*)dict->objectForKey("baseDef"))->floatValue());
    officer->setOfficerRace(((CCString*)dict->objectForKey("race"))->intValue());
    officer->setOfficerRound(((CCString*)dict->objectForKey("baseRound"))->intValue());
    officer->setOfficerName(((CCString*)dict->objectForKey("roleName")));
    officer->setOfficerBaseAtk(((CCString*)dict->objectForKey("baseAtk"))->floatValue());
    officer->setOfficerGeneralsSkill((CCString*)dict->objectForKey("generalsSkill"));
    officer->setOfficerCurrStarLevel(((CCString*)dict->objectForKey("currStarLevel"))->intValue());
    officer->setOfficerLordSkill((CCString*)dict->objectForKey("lordSkill"));
    officer->setOfficerExpType((CCString*)dict->objectForKey("expType"));
    officer->setOfficerAtkMode(((CCString*)dict->objectForKey("atkMode"))->intValue());
    officer->setOfficerAtkMode2(((CCString*)dict->objectForKey("atkMode2"))->intValue());
    officer->setOfficerLastHit(((CCString*)dict->objectForKey("lastHit"))->intValue());
    officer->setOfficerHead(((CCString*)dict->objectForKey("head"))->intValue());
    officer->setOfficerFileId(((CCString*)dict->objectForKey("fileId"))->intValue());
    officer->setOfficerPair(((CCString*)dict->objectForKey("pair"))->intValue());
    officer->setOfficerNum(-1);
    officer->setOfficerMorRate(((CCString*)dict->objectForKey("morrate"))->floatValue());
    officer->setOfficerAtkRate(((CCString*)dict->objectForKey("atkrate"))->floatValue());
    officer->setOfficerExpRate(((CCString*)dict->objectForKey("exprate"))->floatValue());
    officer->setOfficerGrowValue(((CCString*)dict->objectForKey("growvalue"))->floatValue());
    officer->setAttackEffect(-1);
    officer->setDeathEffect(-1);
    officer->setOfficerNextId(((CCString*)dict->objectForKey("nextid"))->intValue());
    officer->setSoundId(((CCString*)dict->objectForKey("soundId"))->intValue());
    officer->setSkillHead(((CCString*)dict->objectForKey("skill_Head"))->intValue());
    //武将计最大等级
    officer->setSkillMaxLevel(((CCString*)dict->objectForKey("generalsSkillMaxlevel"))->intValue());
    //主将计最大等级
    officer->setLordMaxLevel(((CCString*)dict->objectForKey("lordSkillMaxlevel"))->intValue());
    //转生最大星级
    officer->setUpgradestar(((CCString*)dict->objectForKey("maxStarLevel"))->intValue());
    //转生等级
    officer->setUpgradelevel(((CCString*)dict->objectForKey("upgradeMinLevel"))->intValue());
    if (dict->objectForKey("sbSkill")) {
        officer->setSkill(/*((CCString*)dict->objectForKey("sbSkill"))->intValue()*/-1);
    }
    if (officer->getOfficerNextId()) {
        officer->setReqItems(CCString::create(""));
        
        officer->setOfficerCost(((CCString*)dict->objectForKey("cost"))->intValue());
        
    }
    officer->setOfficerVisitType(1/*((CCString*)dict->objectForKey("visittype"))->intValue()*/);
	//iconId//12.06
	officer->setIconId(((CCString*)dict->objectForKey("iconId"))->intValue());
	
    officer->setAdvNum(((CCString*)dict->objectForKey("advancenum"))->intValue());
	
    //性别
    officer->setOfficerGender(((CCString*)dict->objectForKey("gender"))->intValue());
    //解析缘分开始>>
    CCString* fate = (CCString*) dict->objectForKey("fateList");
    if (fate)
    {
        std::vector<int>* vec = parseFateString(fate);
        officer->setOfficerFateList(*vec);
        CC_SAFE_DELETE(vec);
    }
    //<<解析缘分结束
    //武将原型ID
    officer->setOfficerProtoId(((CCString*)dict->objectForKey("protoId"))->intValue());
    //速度修正系数
    officer->setOfficerSpeedFactor(((CCString*)dict->objectForKey("speedFactor"))->floatValue());
	//武将占格子数
	officer->setArea(((CCString*)dict->objectForKey("area"))->intValue());
	
    officer->autorelease();;
    return officer;
}

//解析武将挂载缘分字符串，逗号分隔。形如 1001,2002
std::vector<int>* SGStaticDataManager::parseFateString(CCString* cs)
{
    std::string source = cs->m_sString; //"1,102,889,773,  ---";
    char spt = ',';
    std::string temp;
    int pos;
    int ak; //实际数字
    pos = (int)source.find(spt);
    std::vector<int>* vec = new std::vector<int>();
    while (pos != -1)
    {
        temp = source.substr(0, pos);
        source = source.substr(pos + 1); //+1为跳过一个','符号
        pos = (int)source.find(spt);
        ak = atoi(temp.c_str());
        if (ak != 0)
            vec->push_back(ak);
    }
    ak = atoi(source.c_str());
    if (ak != 0)
        vec->push_back(ak);
    return vec;
}

//讲出 Luamodel 对象
SGLuaDataModel * SGStaticDataManager::dictToLua(CCDictionary *dict)
{
    SGLuaDataModel *lua = new SGLuaDataModel();
    lua->setBtntag(((CCString*)dict->objectForKey("BtnTag"))->intValue());
    lua->setChildtag(((CCString*)dict->objectForKey("ChildTag"))->intValue());
    lua->setBtnname((CCString*)dict->objectForKey("BtnName"));
    lua->setLuaname((CCString*)dict->objectForKey("LuaName"));
    
    
    lua->autorelease();
    return lua;
    
}




//读出 SGSkillDataModel 对象
SGSkillDataModel * SGStaticDataManager::dictToSkill(CCDictionary *dict)
{
    SGSkillDataModel *skill = new SGSkillDataModel();
    
    skill->setBeanId((CCString*)dict->objectForKey("beanId"));
    skill->setSkillName((CCString*)dict->objectForKey("skillName"));
    skill->setSkillId(((CCString*)dict->objectForKey("id"))->intValue());
    skill->setSkillInfo((CCString*)dict->objectForKey("skillInfo"));
//    skill->setRoleName((CCString*)dict->objectForKey("roleName"));
    skill->setRoleName(CCString::create(""));
    //当被动技能时 此skillMaxLevel字段表示当前技能等级 坑爹。。。
    skill->setSkillMaxLevel(((CCString*)dict->objectForKey("skillLevel"))->intValue());
    skill->setNextId(((CCString*)dict->objectForKey("nextId"))->intValue());
    skill->setIconId(((CCString*)dict->objectForKey("iconID"))->intValue());
    
    //被动技表才有的数据
    CCObject *clientLevelObj = dict->objectForKey("clientLevel");
    if(clientLevelObj)
    {
        skill->setSkillClientLevel((CCString*)clientLevelObj);
    }
    
    CCObject *itemid = dict->objectForKey("itemid");
    if (itemid) {
        skill->setSkillItemId(((CCString*)dict->objectForKey("itemid"))->intValue());
    }
    
    CCObject *itemPriority = dict->objectForKey("priority");
    if (itemPriority) {
        skill->setSkillPriority(((CCString*)dict->objectForKey("priority"))->intValue());
    }
    
    //主将计表才有的数据
    CCObject *roundObj = dict->objectForKey("cdRound");
    if(roundObj)
    {
        skill->setRound(((CCString*)roundObj)->intValue());
        
        //2014.5.13
        skill->setValue1(((CCString*)dict->objectForKey("value1"))->floatValue());
        skill->setValue2(((CCString*)dict->objectForKey("value2"))->floatValue());
        skill->setEffectRound(((CCString*)dict->objectForKey("effectRound"))->intValue());
        //skill->setClientType(((CCString*)dict->objectForKey("clientType"))->intValue());
        //skill->setAtkModeSkill(((CCString*)dict->objectForKey("atkModeSkill"))->intValue());
        //skill->setBelongId(((CCString*)dict->objectForKey("belongId"))->intValue());
        skill->setBuff(((CCString*)dict->objectForKey("buff"))->intValue());
    }
    
    //主将计升级编码
    CCObject *objectUpId = dict->objectForKey("upId");
    if(objectUpId)
    {
        skill->setSkillUpId(((CCString*)objectUpId)->intValue());
    }
    
    //主将计技能级别
    CCObject *objectType = dict->objectForKey("levelType");
    if(objectType)
    {
        skill->setSkillType(((CCString*)objectType)->intValue());
    }
    //被动技星级
    CCObject *objectStar = dict->objectForKey("star");
    if(objectStar)
    {
        skill->setSkillStarLvl(((CCString*)objectStar)->intValue());
        
        CCDictionary* dic=getwjjCostBylevel(skill->getSkillMaxLevel());
        CCString *str = CCString::createWithFormat("cost%dstar",skill->getSkillStarLvl());
        skill->setNeedCoin(((CCString*)dic->objectForKey(str->getCString()))->intValue());
        str = CCString::createWithFormat("exp%dstar",skill->getSkillStarLvl());
        skill->setNeedExp(((CCString*)dic->objectForKey(str->getCString()))->intValue());
    }
    
    //主将计才有的level
    CCObject *levelObj = dict->objectForKey("level");
    if(levelObj)
    {
        skill->setLevel(((CCString*)levelObj)->intValue());
        
        CCDictionary* dic=getzjjCostBylevel(skill->getLevel());
        CCString *str = CCString::createWithFormat("ProExpLeveltype%d",skill->getSkillType());
        skill->setProExp(((CCString*)dic->objectForKey(str->getCString()))->intValue());
        str = CCString::createWithFormat("NeedExpLeveltype%d",skill->getSkillType());
        skill->setNeedExp(((CCString*)dic->objectForKey(str->getCString()))->intValue());
        str = CCString::createWithFormat("costLeveltype%d",skill->getSkillType());
        skill->setNeedCoin(((CCString*)dic->objectForKey(str->getCString()))->intValue());
        
    }
    
    skill->autorelease();
    
    return skill;
    
}

//读出 士兵技能对象
SGSoldierSkillDataModel * SGStaticDataManager::dictToSoldierSkill(CCDictionary *dict)
{
    SGSoldierSkillDataModel *skill = new SGSoldierSkillDataModel();
    
    skill->setSoldierBeanId((CCString*)dict->objectForKey("beanId"));
    skill->setSkillName((CCString*)dict->objectForKey("skillName"));
    skill->setSkillID(((CCString*)dict->objectForKey("id"))->intValue());
    skill->setSkillInfo((CCString*)dict->objectForKey("skillInfo"));
    skill->setSkillName((CCString*)dict->objectForKey("skillName"));
    skill->setSkillLevel(((CCString*)dict->objectForKey("skillLevel"))->intValue());
    skill->setNextId(((CCString*)dict->objectForKey("nextId"))->intValue());
    //    skill->setIconId(((CCString*)dict->objectForKey("iconID"))->intValue());
    
    
    skill->autorelease();
    
    return skill;
    
}


//通过beanId获取id
int SGStaticDataManager::getAnyoneIdByBeanId(const char *beanId)
{
    if(!strcmp(beanId, ""))
    {
        return 0;
    }
    
    // sk/10000 把sk/去掉
    char beanIdChar[128] = {0};
    for(int i=0;i<strlen(beanId);i++)
    {
        if(i>=3)
        {
            beanIdChar[i-3] = beanId[i];
        }
    }
    
    return atoi(beanIdChar);
    
}

int SGStaticDataManager::getJunxianSize()
{
    if(NULL == buffDict)
    {
        buffDict = readPlist(PLIST_BUFF);
        if(NULL == buffDict)
        {
            return 0;
        }
        buffDict->retain();
    }
    return buffDict->count();
    
}

SGSkillDataModel* SGStaticDataManager::getLordSkillById(int skillId)
{
    //从主将计表中读数据
    if(NULL == zhujiangjiDict)
    {
        zhujiangjiDict = readPlist(PLIST_ZHUJIANGJI);
        if(NULL == zhujiangjiDict)
        {
            return NULL;
        }
        
        zhujiangjiDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)zhujiangjiDict->objectForKey(CCString::createWithFormat("%d",skillId)->getCString());
    
    if(NULL == dict)
    {
        return NULL;
    }
    
    return dictToSkill(dict);
    
}

int SGStaticDataManager::getSoldierGrowSkillid(int level,int soldierid)
{
    if(NULL == soldiergrowskillDict)
    {
        soldiergrowskillDict = readPlist(PLIST_SOLDIER_GROW_SKILL);
        if(NULL == soldiergrowskillDict)
        {
            return 0;
        }
        
        soldiergrowskillDict->retain();
    }
    CCDictionary *dict = (CCDictionary*)soldiergrowskillDict->objectForKey(CCString::createWithFormat("%d",level)->getCString());
    
    if(NULL == dict)
    {
        return 0;
    }
    CCString *str = CCString::createWithFormat("soldier-%d",soldierid);
    int id = ((CCString*)dict->objectForKey(str->getCString()))->intValue();
    return id;
    
    
}

SGSoldierSkillDataModel* SGStaticDataManager::getSoldierSkillById(int skillId)
{
    //从主将计表中读数据
    if(NULL == soldierskillDict)
    {
        soldierskillDict = readPlist(PLIST_SOLDIER_SKILL);
        if(NULL == soldierskillDict)
        {
            return NULL;
        }
        
        soldierskillDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)soldierskillDict->objectForKey(CCString::createWithFormat("%d",skillId)->getCString());
    
    if(NULL == dict)
    {
        return NULL;
    }
    
    return dictToSoldierSkill(dict);
    
}


SGJunbuff * SGStaticDataManager::getbuffbyid(int buffid)
{
    if(NULL == buffDict)
    {
        buffDict = readPlist(PLIST_BUFF);
        if(NULL == buffDict)
        {
            return NULL;
        }
        buffDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)buffDict->objectForKey(CCString::createWithFormat("%d",buffid)->getCString());
    
    if(NULL == dict)
    {
        return NULL;
    }
    
    return dictToBuff(dict,0);
    
}

SGJunbuff * SGStaticDataManager::getjindutiaobyid(int buffid)
{
    if(NULL == buffDict)
    {
        pvpjundutiao = readPlist(PLIST_JINDU);
        if(NULL == pvpjundutiao)
        {
            return NULL;
        }
        pvpjundutiao->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)pvpjundutiao->objectForKey(CCString::createWithFormat("%d",buffid)->getCString());
    
    if(NULL == dict)
    {
        return NULL;
    }
    
    return dictToBuff(dict,1);
    
}





SGJunbuff *SGStaticDataManager::dictToBuff(CCDictionary *dict,int type)
{
    SGJunbuff *buff = new SGJunbuff();
    if (type ==0) {
        buff->setJunName((CCString*)dict->objectForKey("name"));
        buff->setBuffid(((CCString*)dict->objectForKey("junid"))->intValue());
        buff->setJunCondition((CCString*)dict->objectForKey("condition"));
        buff->setJunbuff((CCString*)dict->objectForKey("buff"));
        buff->autorelease();
        
    }
    else if (type==1)
    {
        buff->setScore(((CCString*)dict->objectForKey("score"))->intValue());
        buff->setMax(((CCString*)dict->objectForKey("max"))->intValue());
        buff->setMin(((CCString*)dict->objectForKey("min"))->intValue());
    }
    
    return buff;
}

//通过Id获得被动技技能数据
SGSkillDataModel* SGStaticDataManager::getGeneralSkillById(int skillId)
{
    //从被动技表中读数据
    if(NULL == wujiangjiDict)
    {
        wujiangjiDict = readPlist(PLIST_WUJIANGJI);
        if(NULL == wujiangjiDict)
        {
            return NULL;
        }
        wujiangjiDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)wujiangjiDict->objectForKey(CCString::createWithFormat("%d",skillId)->getCString());
    
    if(NULL == dict)
    {
        return NULL;
    }
    
    return dictToSkill(dict);
}

//起名器，谁家有孩子，可以试试啊
std::string SGStaticDataManager::getRandomNickName()
{
    //读plist表
    if(NULL == nickNameDict)
    {
        nickNameDict = readPlist(PLIST_NICKNAME);
        nickNameDict->retain();
    }
    
    //没有读到表
    if(NULL == nickNameDict)
    {
        printf("nickName.plist 表没有读出数据 ... ");
        return NULL;
    }
    
    //名字
    CCString *firstName = NULL;;
    int firstNameRandom = 0;
    int dictSize = nickNameDict->count();
    
    do{
        firstNameRandom = arc4random()/dictSize;
        if(firstNameRandom < dictSize && firstNameRandom > 0)
        {
            CCDictionary *dict = (CCDictionary*)nickNameDict->objectForKey(CCString::createWithFormat("%d",firstNameRandom)->getCString());
            firstName = CCString::create(((CCString*)dict->objectForKey("firstName"))->getCString());
            //CCLog("fisrtName -id = %d" ,firstNameRandom );
        }
    }while (!firstName);
    
    //姓
    CCString *familyName = NULL;
    int familyNameRandom = 0;
    
    do{
        
        familyNameRandom = arc4random()%291;
        if(familyNameRandom <= 290 && familyNameRandom > 0)
        {
            CCDictionary *dict = (CCDictionary*)nickNameDict->objectForKey(CCString::createWithFormat("%d",familyNameRandom)->getCString());
            familyName = CCString::create(((CCString*)dict->objectForKey("familyName"))->getCString());
            //CCLog("familyNameRandom -id = %d" ,familyNameRandom );
        }
    }while(!familyName);
    
    //地名
    CCString *areaName = NULL;
    int areaNameRandom = 0;
    do{
        areaNameRandom = arc4random()%112;
        if(areaNameRandom <= 110 && areaNameRandom > 0)
        {
            if(areaNameRandom%2 == 0)
            {
                areaName = CCString::create("");
                break;
            }
            
            CCDictionary *dict = (CCDictionary*)nickNameDict->objectForKey(CCString::createWithFormat("%d",areaNameRandom)->getCString());
            areaName = CCString::create(((CCString*)dict->objectForKey("areaName"))->getCString());
            //CCLog("areaNameRandom -id = %d" ,areaNameRandom );
        }
        
    }while(!areaName);
    
    //组装名字，返回名字
    //CCLog("nickName =%s%s%s" , areaName->getCString() , familyName->getCString() , firstName->getCString());
    std::string nickname;
    nickname.append(areaName->getCString());
    nickname.append(familyName->getCString());
    nickname.append(firstName->getCString());
    return nickname;
}

//通过id获取静态装备数据
SGEquipmentDataModel* SGStaticDataManager::getEquipById(int equipId)
{
    if(NULL == equipmentDict)
    {
        equipmentDict = readPlist(PLIST_EQUIPMENT);
        equipmentDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)equipmentDict->objectForKey((CCString::createWithFormat("%d",equipId))->getCString());
    if(NULL == dict)
    {
        printf("getEquipById error ... ");
        return NULL;
    }
    
    return dictToEquipment(dict);
}

//通过原型Id查找装备卡部分数据
SGEquipmentDataModel* SGStaticDataManager::getEquipByProtoId(int protoId)
{
    return (SGEquipmentDataModel*) protoEquipmentDict->objectForKey(protoId);
}


//从dict中读出相应的属性
SGEquipmentDataModel* SGStaticDataManager::dictToEquipment(CCDictionary *dict)
{
    SGEquipmentDataModel *equipment = new SGEquipmentDataModel();
    
    equipment->setEquipmentBaseAtk(((CCString*)dict->objectForKey("baseAtk"))->floatValue());
    equipment->setEquipBaseDef(((CCString*)dict->objectForKey("baseDef"))->floatValue());
    equipment->setEquipBaseSpeed(((CCString*)dict->objectForKey("baseSpeed"))->floatValue());
    equipment->setEquipCurrStarLevel(((CCString*)dict->objectForKey("currStarLevel"))->intValue());
    equipment->setEquipGoverning(((CCString*)dict->objectForKey("governing"))->intValue());
    equipment->setEquipHP(((CCString*)dict->objectForKey("hp"))->floatValue());
     CCLOG("%s",((CCString*)dict->objectForKey("hp"))->getCString());
   // CCLOG("%f",((CCString*)dict->objectForKey("hp"))->floatValue());
    
    equipment->setEquipMaxLevel(((CCString*)dict->objectForKey("maxLevel"))->intValue());
    equipment->setEquipName(((CCString*)dict->objectForKey("name")));
    
    
    equipment->setEquipmentId(((CCString*)dict->objectForKey("itemId"))->intValue());


    
    equipment->setEquipRound(((CCString*)dict->objectForKey("round"))->intValue());
    equipment->setEquipType(((CCString*)dict->objectForKey("itemType"))->intValue());
    equipment->setEquipExpType((CCString*)dict->objectForKey("exptype"));
    equipment->setEquipNum(-1);
    equipment->setUpgradelevel(((CCString*)dict->objectForKey("upgradeMinLevel"))->intValue());
    equipment->setEquipGrowValue(((CCString*)dict->objectForKey("growvalue"))->floatValue());
    equipment->setUpgradestar(((CCString*)dict->objectForKey("maxStarLevel"))->intValue());
    
    CCString *a = (CCString*)dict->objectForKey("nextid");
    if (a) {
        equipment->setEquipNextId(((CCString*)dict->objectForKey("nextid"))->intValue());
        
    }
    equipment->setEquipVisitType(1);
    if (equipment->getEquipNextId()) {
        equipment->setEquipCost(((CCString*)dict->objectForKey("cost"))->intValue());
        equipment->setReqItems(CCString::create(""));
    }
    equipment->setEquipExpRate(((CCString*)dict->objectForKey("exprate"))->floatValue());
    //装备原型ID
    equipment->setEquipProtoId(((CCString*)dict->objectForKey("protoId"))->intValue());
	equipment->setIconId(((CCString*)dict->objectForKey("iconId"))->intValue());//12.06
	equipment->setAdvNum(((CCString*)dict->objectForKey("advancenum"))->intValue());
    equipment->setMaterialDependencyId(((CCString*)dict->objectForKey("MaterialDependency"))->intValue());
    equipment->setFateDesc(((CCString*)dict->objectForKey("fateDesc")));
    
    equipment->autorelease();
    
    return equipment;
}

//通过fileId获取小兵的时间特效数据  //added by cgp
SGSbLocalDataModel* SGStaticDataManager::getSbLocalByFileId(int fileId)
{
    if(NULL == sbLocalDict)
    {
        //GPCCLOG("nu== SoliderDict");
        sbLocalDict = readPlist(PLIST_SBLOCAL);
        sbLocalDict->retain();
    }
    CCDictionary *dict = (CCDictionary*)sbLocalDict->objectForKey((CCString::createWithFormat("%d",fileId))->getCString());
    if (!dict)
    {
        //GPCCLOG("fileId====%d",fileId);
    }
    return dictToSbLocal(dict);
    
}

//从dict中读出小兵的的时间特效数据
SGSbLocalDataModel* SGStaticDataManager::dictToSbLocal(cocos2d::CCDictionary *dict)
{
    if (dict != NULL) {
        SGSbLocalDataModel *sbLocal = new SGSbLocalDataModel();
        
        sbLocal->setFileId(((CCString*)dict->objectForKey("FileId"))->intValue());
        sbLocal->setAllTime(((CCString*)dict->objectForKey("allTime"))->floatValue());
        sbLocal->setEffTime(((CCString*)dict->objectForKey("effTime"))->floatValue());
        sbLocal->setWallEffId(((CCString*)dict->objectForKey("wallEffId"))->intValue());
        sbLocal->setBottomEffId(((CCString*)dict->objectForKey("bottomEffId"))->intValue());
        sbLocal->setFlyId(((CCString*)dict->objectForKey("flyId"))->intValue());
        sbLocal->setFlyPosX(((CCString*)dict->objectForKey("flyPosX"))->intValue());
        sbLocal->setFlyPosY(((CCString*)dict->objectForKey("flyPosY"))->intValue());
        sbLocal->autorelease();
        return sbLocal;
    }
    return NULL;
    
}




//通过randomId获取随机武将信息  //added by cgp
SGLoadingOfficerDataModel* SGStaticDataManager::getLoadingOfficerByRandomId(int randomId)
{
    if(NULL == loadingOfficerDict)
    {
        //GPCCLOG("nu== SoliderDict");
        loadingOfficerDict = readPlist(PLIST_LOADING_OFFICER);
        loadingOfficerDict->retain();
    }
    CCDictionary *dict = (CCDictionary*)loadingOfficerDict->objectForKey((CCString::createWithFormat("%d",randomId))->getCString());
    if (!dict)
    {
        //GPCCLOG("fileId====%d",fileId);
    }
    return dictToLoadingOfficer(dict);
    
}

//从dict中读出loading
SGLoadingOfficerDataModel* SGStaticDataManager::dictToLoadingOfficer(cocos2d::CCDictionary *dict)
{
    if (dict != NULL) {
        SGLoadingOfficerDataModel *loadingOfficer = new SGLoadingOfficerDataModel();
        
        loadingOfficer->setRandomId(((CCString*)dict->objectForKey("randomId"))->intValue());
        loadingOfficer->setFileId(((CCString*)dict->objectForKey("fileId"))->intValue());
        loadingOfficer->setBigIcon(((CCString*)dict->objectForKey("bigIcon"))->intValue());
        loadingOfficer->setName((CCString*)dict->objectForKey("name"));
        loadingOfficer->setDescription((CCString*)dict->objectForKey("description"));

        loadingOfficer->autorelease();
        return loadingOfficer;
    }
    return NULL;
    
}

//通过Id获取小兵的静态数据
SGSoldierDataModel* SGStaticDataManager::getSoliderById(int soldierId)
{
    //    CCLOG(">>>>%d",soldierId);
    if(NULL == soldierDict)
    {
        CCLog("nu== SoliderDict");
        soldierDict = readPlist(PLIST_SOLDIER);
        soldierDict->retain();
    }
    
    
    
    CCDictionary *dict = (CCDictionary*)soldierDict->objectForKey((CCString::createWithFormat("%d",soldierId))->getCString());
    if (!dict) {
        CCLOG("soldierId====%d",soldierId);
    }
    return dictToSoldier(dict);
    
}

//从dict中读出小兵的的数据
SGSoldierDataModel* SGStaticDataManager::dictToSoldier(cocos2d::CCDictionary *dict)
{
    if (dict != NULL) {
        SGSoldierDataModel *soldier = new SGSoldierDataModel();
        
        soldier->setSoldierBeanId((CCString*)dict->objectForKey("beanId"));
        soldier->setSoldierId(((CCString*)dict->objectForKey("id"))->intValue());
        soldier->setSoldierName((CCString*)dict->objectForKey("name"));
        soldier->setSoldierRace(((CCString*)dict->objectForKey("race"))->intValue());
        soldier->setSoldierBaseAtk(((CCString*)dict->objectForKey("baseAtk"))->floatValue());
        soldier->setSoldierBaseDef(((CCString*)dict->objectForKey("baseDef"))->floatValue());
        soldier->setSoldierMorale(((CCString*)dict->objectForKey("morale"))->intValue());
        soldier->setSoldierWallDef(((CCString*)dict->objectForKey("wallDef"))->intValue());
        soldier->setSoldierRound(((CCString*)dict->objectForKey("round"))->intValue());
        soldier->setSoldierExpType((CCString*)dict->objectForKey("expType"));
        soldier->setSoldierNum(((CCString*)dict->objectForKey("num"))->intValue());
        soldier->setSoldierAtkMode(((CCString*)dict->objectForKey("atkMode"))->intValue());
        soldier->setSoldierAtkMode2(((CCString*)dict->objectForKey("atkMode2"))->intValue());
        soldier->setSoldierHead(((CCString*)dict->objectForKey("head"))->intValue());
        soldier->setSoldierFileId(((CCString*)dict->objectForKey("fileId"))->intValue());
        soldier->setSoldierPair(((CCString*)dict->objectForKey("pair"))->intValue());
        soldier->setSoldierLastHit(((CCString*)dict->objectForKey("lastHit"))->intValue());
        soldier->setSoldierAtkRate(((CCString*)dict->objectForKey("atkrate"))->floatValue());
        soldier->setSoldierMorRate(((CCString*)dict->objectForKey("morrate"))->floatValue());
        soldier->setSoldierMaxLevel(((CCString*)dict->objectForKey("maxlevel"))->intValue());
        soldier->setSoldierStarLevel(((CCString*)dict->objectForKey("starlvl"))->intValue());
        soldier->setSoldierGrowValue(((CCString*)dict->objectForKey("growvalue"))->floatValue());
        soldier->setAttackEffect(((CCString*)dict->objectForKey("attackEffect"))->intValue());
        soldier->setDeathEffect(((CCString*)dict->objectForKey("deathEffect"))->intValue());
        soldier->setSkill(((CCString*)dict->objectForKey("sbSkill"))->intValue());
//        soldier->setGrowSkill(((CCString*)dict->objectForKey("GrowSkill"))->intValue());
        soldier->setTalentSkill(((CCString*)dict->objectForKey("TalentSkill"))->intValue());
        soldier->autorelease();
        
        return soldier;
    }
    return NULL;
    
}

//通过id获得道具的静态数据
SGPropsDataModel* SGStaticDataManager::getPropsById(int propsId)
{
    if(NULL == propsDict)
    {
        propsDict = readPlist(PLIST_PROPS);
        propsDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)propsDict->objectForKey((CCString::createWithFormat("%d",propsId))->getCString());
    return dictToProps(dict);
}

//从dict中读出道具的数据
SGPropsDataModel* SGStaticDataManager::dictToProps(cocos2d::CCDictionary *dict)
{
    SGPropsDataModel *props = new SGPropsDataModel();
    
    props->setPropsBeanId((CCString*)dict->objectForKey("beanId"));
    props->setPropsCurrStarLevel(((CCString*)dict->objectForKey("currStarLevel"))->intValue());
    props->setPropsInfo((CCString*)dict->objectForKey("propsInfo"));
    props->setPropsId(((CCString*)dict->objectForKey("itemId"))->intValue());
    props->setPropsName((CCString*)dict->objectForKey("name"));
    props->setPropsType(((CCString*)dict->objectForKey("type"))->intValue());
    props->setPropsPropsType(((CCString*)dict->objectForKey("propsType"))->intValue());
    //props->setpropsNum(((CCString*)dict->objectForKey("num"))->intValue());
    props->setpropsSell(((CCString*)dict->objectForKey("sell"))->intValue());
    CCString *valueStr = (CCString*)dict->objectForKey("value");
    //props->setpropVisitType(((CCString*)dict->objectForKey("visittype"))->intValue());
	props->setPropJokeInfo(static_cast<CCString *>(dict->objectForKey("propsMeng")));
	//
	props->setIconId(((CCString*)dict->objectForKey("iconId"))->intValue());//head%d，索引
    if (valueStr) {
        props->setpropsValue(valueStr->intValue());
        
    }
    CCString *countryStr = (CCString*)dict->objectForKey("country");
    if(countryStr)
    {
        
        props->setPropsCountry(countryStr->intValue());
    }
    
    props->autorelease();
    
    return props;
    
}

//获取卡牌类型
CCString *SGStaticDataManager::getCardType(int type)
{
    if(NULL == cardTypeDict)
    {
        cardTypeDict = readPlist(PLIST_TYPE);
        cardTypeDict->retain();
    }
    
    CCString *cardType = (CCString*)cardTypeDict->objectForKey((CCString::createWithFormat("%d",type))->getCString());
    
    return cardType;
    
}

CCArray* SGStaticDataManager::getArrayById(int plotId)
{
    if(NULL == battlewordDict)
    {
        battlewordDict = readPlist(PLIST_BATTLEWORD);
        battlewordDict->retain();
    }
    
    const char* key = CCString::createWithFormat("%d", plotId)->getCString();
    CCDictionary *dict = (CCDictionary*)battlewordDict->objectForKey(key);
    
    return dictToBattleword(dict);
    
}

CCString* SGStaticDataManager::getPKWordById(int plotId)
{
    if(NULL == fightwordDict)
    {
        fightwordDict = readPlist(PLIST_FIGHTWORD);
        fightwordDict->retain();
    }
    
    const char* key = CCString::createWithFormat("%d", plotId)->getCString();
    CCDictionary *dict = (CCDictionary*)fightwordDict->objectForKey(key);
    
    CCArray *array = dictToBattleword(dict);
    
    if (array->count() > 0) {
        CCString *word = (CCString *)array->objectAtIndex(0);
        return word;
    }
    return NULL;
}

CCArray * SGStaticDataManager::dictToBattleword(CCDictionary *dict)
{
    int speaker = dict->count()-1; // key
    CCArray *array = CCArray::create();
    
    for(int i=1; i<=speaker; i++) {
        CCString *str = (CCString*)dict->objectForKey(CCString::createWithFormat("speak%d", i)->getCString());
        if(str) {
            array->addObject(str);
        }
    }
    
    return array;
}

SGSpotDataModel * SGStaticDataManager::getCreamByPlotId(int plotId)
{
    CCDictionary * dict = NULL;
    if (m_bossDict == NULL)
    {
        m_bossDict = readPlist(PLIST_CREAMINFO);
        m_bossDict->retain();
    }
    std::string idStr(CCString::createWithFormat("%d", plotId)->getCString());
    dict = (CCDictionary *)m_bossDict->objectForKey(idStr);
    if(dict)
    {
        SGSpotDataModel *spot = new SGSpotDataModel();
        spot->setSpotBeanId((CCString*)dict->objectForKey("beanId"));
        spot->setSpotId(((CCString*)dict->objectForKey("plotId"))->intValue());
        spot->setSpotRequestPower(((CCString*)dict->objectForKey("requestPower"))->intValue());
        spot->setSpotSectionIndex(((CCString*)dict->objectForKey("sectionIndex"))->intValue());
        spot->setSpotSectionName((CCString*)dict->objectForKey("sectionName"));
        spot->setSpotSpotIndex(((CCString*)dict->objectForKey("spotIndex"))->intValue());
        spot->setSpotStoryName((CCString*)dict->objectForKey("storyName"));
        spot->setSpotDropExp(((CCString*)dict->objectForKey("exp"))->intValue());
        spot->setSpotDropCoins(((CCString*)dict->objectForKey("coins"))->intValue());
        spot->setCreamDayTimes( ( (CCString*)dict->objectForKey("dayJoinTimes"))->intValue());
        
        spot->autorelease();
        return spot;
    }
    else
        return NULL;
    
    
}

SGSpotDataModel* SGStaticDataManager::getSpotById(int spotId, bool spot)
{
    CCDictionary *dict = NULL;
    if (spotId >= 10000) spot = false;
    
    if(NULL == activityDict)
    {
        activityDict = readPlist(PLIST_ACTIVITY);
        activityDict->retain();
    }
    if(NULL == spotDict)
    {
        spotDict = readPlist(PLIST_SPOT);
        spotDict->retain();
    }
    if (spot == true)
    {
        dict = (CCDictionary*)spotDict->objectForKey(CCString::createWithFormat("%d",spotId)->getCString());
    }
    else
    {
        dict = (CCDictionary*)activityDict->objectForKey(CCString::createWithFormat("%d",spotId)->getCString());
    }
    //精英副本
    if (spotId >= 20000)
    {
        return this->getCreamByPlotId(spotId);
    }
    //end
	if (dict == NULL)//如果对应id的dict为空，直接返回，避免造成不必要的崩溃
	{
		return NULL;
	}
    
    return dictToSpot(dict);
}

SGSpotDataModel* SGStaticDataManager::dictToSpot(cocos2d::CCDictionary *dict)
{
    SGSpotDataModel *spot = new SGSpotDataModel();
    spot->setSpotBeanId((CCString*)dict->objectForKey("beanId"));
    spot->setSpotId(((CCString*)dict->objectForKey("plotId"))->intValue());
    spot->setSpotRequestPower(((CCString*)dict->objectForKey("requestPower"))->intValue());
    spot->setSpotSectionIndex(((CCString*)dict->objectForKey("sectionIndex"))->intValue());
    spot->setSpotSectionName((CCString*)dict->objectForKey("sectionName"));
    spot->setSpotSpotIndex(((CCString*)dict->objectForKey("spotIndex"))->intValue());
    spot->setSpotStoryName((CCString*)dict->objectForKey("storyName"));
    spot->setSpotDropExp(((CCString*)dict->objectForKey("exp"))->intValue());
    spot->setSpotDropCoins(((CCString*)dict->objectForKey("coins"))->intValue());
    
    if (dict->objectForKey("minLevel"))
    {
         spot->setMinLevel(((CCString*)dict->objectForKey("minLevel"))->intValue());
    }
    spot->autorelease();
    return spot;
}

//特殊副本战斗数据
SGSpecialBattleDataModel *SGStaticDataManager::getSpecialBattleDataByPlotId(int plotId)
{
//    if (specialBattleDict == NULL)
//    {
//        specialBattleDict = readPlist(PLIST_SPECAIL_BATTLE);
//        specialBattleDict->retain();
//    }
    
    CCDictionary *dict = (CCDictionary*)specialBattleDict->objectForKey(CCString::createWithFormat("%d",plotId)->getCString());
    
    return dictToSpecialBattle(dict);
}

SGSpecialBattleDataModel *SGStaticDataManager::dictToSpecialBattle(CCDictionary *dict)
{
    SGSpecialBattleDataModel *spModel = new SGSpecialBattleDataModel();
    spModel->autorelease();
    
    spModel->setBeanId(((CCString*)dict->objectForKey("beanId"))->getCString());
    spModel->setChapId(((CCString*)dict->objectForKey("chapId"))->intValue());
    spModel->setChapName(((CCString*)dict->objectForKey("chapName"))->getCString());
    spModel->setDetailLimits(((CCString*)dict->objectForKey("detailLimits"))->intValue());
    spModel->setDropItemsId(((CCString*)dict->objectForKey("DropItemsId"))->intValue());
    spModel->setLimitLevel(((CCString*)dict->objectForKey("openLevel"))->intValue());
    spModel->setLimitOfJoin(((CCString*)dict->objectForKey("limitsOfJoin"))->intValue());
    spModel->setNeedPower(((CCString*)dict->objectForKey("needPower"))->intValue());
    spModel->setPlotId(((CCString*)dict->objectForKey("plotId"))->intValue());
    spModel->setSectionIndex(((CCString*)dict->objectForKey("sectionIndex"))->intValue());
    spModel->setStoryDesc(((CCString*)dict->objectForKey("storyDesc"))->getCString());
    spModel->setStoryName(((CCString*)dict->objectForKey("storyName"))->getCString());
    spModel->setSuggestPower(((CCString*)dict->objectForKey("suggestPower"))->intValue());
    spModel->setTimeOfDay(((CCString*)dict->objectForKey("timesOfDay"))->intValue());
    return spModel;
}



SGRewardDataModel* SGStaticDataManager::getRewardById(int rewardId, int rewardType)
{
    CCDictionary *dict = NULL;
    
    if(NULL == rewardDict)
    {
        rewardDict = readPlist(PLIST_REWARD);
        rewardDict->retain();
    }
    dict = (CCDictionary*)rewardDict->objectForKey(CCString::createWithFormat("%d",rewardId)->getCString());
    if (dict) {
        int findRewardType = ((CCString*)dict->objectForKey("rewardType"))->intValue();
        if (findRewardType == rewardType) {
            return dictToReward(dict);
        }
    }
    return NULL;
}

SGMainTaskSubModal * SGStaticDataManager::getMainTaskSubByTaskId(int taskID){
    
    CCDictionary *dict = NULL;
    
    if(NULL == mainTaskSubDic)
    {
        mainTaskSubDic = readPlist(PLIST_MAINTASKQUEST);
        mainTaskSubDic->retain();
    }
    dict = (CCDictionary*)mainTaskSubDic->objectForKey(CCString::createWithFormat("%d",taskID)->getCString());
    if (dict) {
        return dicToMainSubModal(dict);
    }
    return NULL;
    
}
SGMainTaskSubModal * SGStaticDataManager::dicToMainSubModal(CCDictionary *dic){
    
    SGMainTaskSubModal *modal=new SGMainTaskSubModal();
    modal->settaskId(((CCString *)dic->objectForKey("id"))->intValue());
    modal->setRewareId(((CCString *)dic->objectForKey("rewardId"))->intValue());
    modal->settaskDesprition(((CCString *)dic->objectForKey("description")));
    modal->settaskName(((CCString *)dic->objectForKey("questName")));
    return modal;
}

SGRewardDataModel* SGStaticDataManager::dictToReward(cocos2d::CCDictionary *dict)
{
    SGRewardDataModel *reward = new SGRewardDataModel();
    reward->setRewardId(((CCString*)dict->objectForKey("rewardId"))->intValue());
    reward->setRewardType(((CCString*)dict->objectForKey("rewardType"))->intValue());
    
    size_t pos = 0;
    int type=0;
    int value=0;
    int number=0;
    CCString *string = NULL;
    reward->rewardArray->removeAllObjects();
    std::string rewardItem(((CCString*)dict->objectForKey("rewardInfo"))->getCString());
    while (pos != std::string::npos) {
        pos = rewardItem.find_first_of(';');
        if (pos == std::string::npos) {
            sscanf( rewardItem.c_str(), "%d,%d,%d", &type,&value,&number);
        }
        else if (pos == rewardItem.size()-1)
        {
            std::string subuse = rewardItem.substr(0,pos);
            sscanf( subuse.c_str(), "%d,%d,%d", &type,&value,&number);
            pos = std::string::npos;
        }
        else{
            std::string subuse = rewardItem.substr(0,pos);
            sscanf( subuse.c_str(), "%d,%d,%d", &type,&value,&number);
            rewardItem = rewardItem.substr(pos+1,rewardItem.size()-1);
            
        }
        
        string = CCString::createWithFormat("%d-%d-%d",type,value,number);
        reward->rewardArray->addObject(string);
    }
    return reward;
}

SGSuitDataModel* SGStaticDataManager::getSuitById(int suitId)
{
    if(NULL == suitDict)
    {
        suitDict = readPlist(PLIST_SUIT);
        suitDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)suitDict->objectForKey((CCString::createWithFormat("%d",suitId))->getCString());
    if(dict)
        return dictToSuit(dict);
    return NULL;
}

SGSuitDataModel *SGStaticDataManager::dictToSuit(cocos2d::CCDictionary *dict)
{
    SGSuitDataModel *suit = new SGSuitDataModel();
    suit->setSuitArmsName((CCString*)dict->objectForKey("arms"));
    suit->setSuitCount(((CCString*)dict->objectForKey("count"))->intValue());
    suit->setSuitId(((CCString*)dict->objectForKey("id"))->intValue());
    suit->setSuitName((CCString*)dict->objectForKey("name"));
    
    suit->autorelease();
    return suit;
    
}

SGVipDataModel *SGStaticDataManager::getVipById(int vipId)
{
    if(NULL == vipDict)
    {
        vipDict = readPlist(PLIST_VIP);
        vipDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)vipDict->objectForKey(CCString::createWithFormat("%d",vipId)->getCString());
    
    return dictToVip(dict);
}

SGVipDataModel* SGStaticDataManager::dictToVip(cocos2d::CCDictionary *dict)
{
    SGVipDataModel *vip = new SGVipDataModel();
    
    vip->setVipId(((CCString*)dict->objectForKey("vipId"))->intValue());
    vip->setVipName((CCString*)dict->objectForKey("vipLevel"));
    int count = ((CCString*)dict->objectForKey("vipCount"))->intValue();
    vip->setVipCount(count);
    
    for (int i = 0; i < count; i++) {
        CCString *rightKey = CCString::createWithFormat("rights%d",i+1);
        if (dict->objectForKey(rightKey->getCString()))
        {
            vip->getVipRightsList()->addObject(dict->objectForKey(rightKey->getCString()));
        }
        
    }
    
    vip->setneedRMB(((CCString*)dict->objectForKey("rmb"))->intValue());
    vip->autorelease();
    return vip;
    
}

SGPlayerDataModel* SGStaticDataManager::getPlayerByLevel(int playerLevel)
{
    //    测试调等级 等级限制
    if (playerLevel > 99) {
        playerLevel = 99;
    }
    else if (playerLevel <= 0)
    {
        playerLevel = 1;
    }
    
    if(NULL == playerDict)
    {
        playerDict = readPlist(PLIST_PLAYER);
        playerDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary*)playerDict->objectForKey(CCString::createWithFormat("%d",playerLevel)->getCString());
    
    return dictToPlayer(dict);
}

SGPlayerDataModel *SGStaticDataManager::dictToPlayer(cocos2d::CCDictionary *dict)
{
    SGPlayerDataModel *player = new SGPlayerDataModel();
    
    player->setPlayerBaseFriendSize(((CCString*)dict->objectForKey("baseFriendSize"))->intValue());
    player->setPlayerBeanId(((CCString*)dict->objectForKey("beanId")));
    player->setPlayerExp(((CCString*)dict->objectForKey("exp"))->intValue());
    player->setPlayerGoverning(((CCString*)dict->objectForKey("governing"))->intValue());
    player->setPlayerLevel(((CCString*)dict->objectForKey("level"))->intValue());
    player->setPlayerPower(((CCString*)dict->objectForKey("power"))->intValue());
    player->setPlayerPowerOld(((CCString*)dict->objectForKey("powerOld"))->intValue());
    player->setPlayerSalary(((CCString*)dict->objectForKey("salary"))->intValue());
    player->setPlayerOfficerMax(((CCString*)dict->objectForKey("officerUpMax"))->intValue());
    player->setPlayerUnlockTip((CCString*)dict->objectForKey("unlockTip"));
    player->setPlayerUnlockLevel(((CCString*)dict->objectForKey("unlockLevel")));
    player->setGrantPower(((CCString*)dict->objectForKey("grantpower"))->intValue());
    player->autorelease();
    
    return player;
    
}


SGExpDataModel *SGStaticDataManager::getExpById(const char *expBeanId)
{
    if(NULL == expDict)
    {
        expDict = readPlist(PLIST_EXP);
        expDict->retain();
        
    }
    
    CCDictionary *dict = (CCDictionary*)expDict->objectForKey(expBeanId);
    if (!dict) {
        //CCLOG("expBeanId-=====%d",expBeanId);
    }
    
    
    return dictToExp(dict);
}

SGDayTaskDataModel *SGStaticDataManager::getTaskById(int taskId)
{
    if(NULL == taskDict)
    {
        taskDict = readPlist(PLIST_TASK);
        taskDict->retain();
    }
    char buf[32] = {0};
    sprintf(buf, "DayTask/%d",taskId);
    std::string keyStr;
    keyStr.append(buf);
    CCDictionary *dict = (CCDictionary*)taskDict->objectForKey(keyStr);
    return dictToTasks(dict);
}
SGDayTaskDataModel *SGStaticDataManager::dictToTasks(cocos2d::CCDictionary *dict)
{
    SGDayTaskDataModel *task = new SGDayTaskDataModel();
    task->settaskbeanid((CCString*)dict->objectForKey("beanId"));
    task->settaskcount(((CCString*)dict->objectForKey("count"))->intValue());
    task->settaskid(((CCString*)dict->objectForKey("taskId"))->intValue());
    task->settaskinfo(((CCString*)dict->objectForKey("taskinfo")));
    task->settaskname(((CCString*)dict->objectForKey("taskname")));
    task->settasklvl(((CCString*)dict->objectForKey("level"))->intValue());
    task->autorelease();
    return task;
}

CCArray *SGStaticDataManager::gettips()
{
    CCArray *data = CCArray::create();
    if(NULL == tipDict)
    {
        tipDict = readPlist(PLIST_TIP);
        tipDict->retain();
    }
    
    for (int i = 1; i<=tipDict->allKeys()->count(); i++) {
        CCDictionary *dict = (CCDictionary*)tipDict->objectForKey(CCString::createWithFormat("tip%d",i)->getCString());
        data->addObject(((CCString*)dict->objectForKey("info")));
    }
    return data;
}
CCArray *SGStaticDataManager::gethelptitle()
{
    CCArray *data = CCArray::create();
    if(NULL == helpDict)
    {
        helpDict = readPlist(PLIST_HELP);
        helpDict->retain();
    }
    if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
        for (int i = 1; i<=helpDict->allKeys()->count(); i++) {
            CCDictionary *dict = (CCDictionary*)helpDict->objectForKey(CCString::createWithFormat("num%d",i)->getCString());
            SGHelpTitleDataModel *tit = new SGHelpTitleDataModel();
            tit->setnum(i);
            tit->setbeg(((CCString*)dict->objectForKey("begin"))->intValue());
            tit->setend(((CCString*)dict->objectForKey("end"))->intValue());
            tit->settitle(((CCString*)dict->objectForKey("title")));
            data->addObject(tit);
            tit->autorelease();
        }
        
    }
    else
    {
        for (int i = 1; i<=helpDict->allKeys()->count(); i++)
        {
            if (i != 4 && i != 3 && i!=2) {
                CCDictionary *dict = (CCDictionary*)helpDict->objectForKey(CCString::createWithFormat("num%d",i)->getCString());
                SGHelpTitleDataModel *tit = new SGHelpTitleDataModel();
                tit->setnum(i);
                tit->setbeg(((CCString*)dict->objectForKey("begin"))->intValue());
                tit->setend(((CCString*)dict->objectForKey("end"))->intValue());
                tit->settitle(((CCString*)dict->objectForKey("title")));
                data->addObject(tit);
                tit->autorelease();
                
            }
        }
        
    }
    return data;
}
CCArray *SGStaticDataManager::gethelpinfo(int begin,int end)
{
    CCArray *data = CCArray::create();
    if(NULL == helpinfoDict)
    {
        helpinfoDict = readPlist(PLIST_HELPINFO);
        helpinfoDict->retain();
    }
    
    for (int i = begin; i<=end; i++) {
        CCDictionary *dict = (CCDictionary*)helpinfoDict->objectForKey(CCString::createWithFormat("id%d",i)->getCString());
        SGHelpTitleDataModel *tit = new SGHelpTitleDataModel();
        tit->setnum(i);
        tit->setbeg(((CCString*)dict->objectForKey("id"))->intValue());
        tit->setend(((CCString*)dict->objectForKey("type"))->intValue());
        if ((CCString*)dict->objectForKey("cont")) {
            tit->settitle(((CCString*)dict->objectForKey("cont")));
        }
        data->addObject(tit);
        tit->autorelease();
    }
    return data;
}
CCArray *SGStaticDataManager::getAllStoryDatas()
{
    CCArray *data = CCArray::create();
    // 场景ID：1-MaxSceneId
    for (int ii = 0; ii < MaxSceneId; ii++)
    {
        SGSpotDataModel *story = SGStaticDataManager::shareStatic()->getSpotById(ii+1);
        
        int index = -1;
        for (int jj = 0; jj < data->count(); jj++)
        {
            SGStoryBean *bean = (SGStoryBean *)data->objectAtIndex(jj);
            if (bean->getStoryId() == story->getSpotSectionIndex())
            {
                index = jj;
                break;
            }
        }
        
        // 初始化场景Bean
        int var3 = ii + 1;
        int var1 = story->getSpotSectionIndex();
        int var2 = story->getSpotSpotIndex();
        int var4 = story->getSpotRequestPower();
        const char *var5 = story->getSpotStoryName()->getCString();

        SGStorySceneBean *storyScene = SGStorySceneBean::create(var1, var2, var3, var4, var5);
        storyScene->setDropInfo(story->getSpotDropExp(), story->getSpotDropCoins());
        
        // 初始化故事Bean    故事ID：1-35
        if (index >= 0)
        {
            SGStoryBean *storyBean = (SGStoryBean *)data->objectAtIndex(index);
            storyBean->getStorySections()->addObject(storyScene);
            //zyc debug
            storyBean->setMinLevel(story->getMinLevel());
        }
        else
        {
            int var1 = story->getSpotSectionIndex();
            const char* var2 = story->getSpotSectionName()->getCString();
            CCArray *var3 = CCArray::create();
            SGStoryBean *storyBean = SGStoryBean::create(var1, var2, var3);
            storyBean->getStorySections()->addObject(storyScene);
            data->addObject(storyBean);
        }
    }
    return data;
}
//获取本在精英副本数据
CCArray *SGStaticDataManager::getAllCreamDatas()
{
    CCArray *data = CCArray::create();
    // 场景ID：1-MaxSceneId
    for (int ii = 20000; ii <= MaxCreamId; ii++) {
        SGSpotDataModel *story = SGStaticDataManager::shareStatic()->getSpotById(ii);
        if (!story)
        {
            continue;
        }
        
        int index = -1;
        for (int jj = 0; jj < data->count(); jj++) {
            SGStoryBean *bean = (SGStoryBean *)data->objectAtIndex(jj);
            if (bean->getStoryId() == story->getSpotSectionIndex()) {
                index = jj;
                break;
            }
        }
        // 初始化场景Bean
        int var3 = ii;
        int var1 = story->getSpotSectionIndex();
        int var2 = story->getSpotSpotIndex();
        int var4 = story->getSpotRequestPower();
        const char *var5 = story->getSpotStoryName()->getCString();
        //create(int sid, int scid, int pid, int power, const char* sname);
        SGStorySceneBean *storyScene = SGStorySceneBean::create(var1, var2, var3, var4, var5);
        storyScene->setDropInfo(story->getSpotDropExp(), story->getSpotDropCoins());
        
        // 初始化故事Bean    故事ID：1-35
        if (index >= 0) {
            SGStoryBean *storyBean = (SGStoryBean *)data->objectAtIndex(index);
            storyBean->getStorySections()->addObject(storyScene);
        }else {
            int var1 = story->getSpotSectionIndex();
            const char* var2 = story->getSpotSectionName()->getCString();
            CCArray *var3 = CCArray::create();
            SGStoryBean *storyBean = SGStoryBean::create(var1, var2, var3);
            storyBean->getStorySections()->addObject(storyScene);
            data->addObject(storyBean);
        }
    }
    return data;
}


CCArray *SGStaticDataManager::getAllActivityDatas()
{
    CCArray *data = CCArray::create();
    for (int ii = 10000; ii <= MaxActivityId; ii++) {
        SGSpotDataModel *story = SGStaticDataManager::shareStatic()->getSpotById(ii, false);
		if (!story)
			continue;
        DDLog("tt-------------------%s", story->getSpotStoryName()->getCString());
        int index = -1;
        for (int jj = 0; jj < data->count(); jj++) {
            SGStoryBean *bean = (SGStoryBean *)data->objectAtIndex(jj);
            if (bean->getStoryId() == story->getSpotSectionIndex()) {
                index = jj;
                break;
            }
        }
        
        // 初始化场景Bean
        int var3 = ii;
        int var1 = story->getSpotSectionIndex();
        int var2 = story->getSpotSpotIndex();
        int var4 = story->getSpotRequestPower();
        const char *var5 = story->getSpotStoryName()->getCString();
        //create(int sid, int scid, int pid, int power, const char* sname);
        SGStorySceneBean *storyScene = SGStorySceneBean::create(var1, var2, var3, var4, var5);
        storyScene->setDropInfo(story->getSpotDropExp(), story->getSpotDropCoins());
        
        // 初始化活动Bean    活动ID：10000-10009
        if (index >= 0) {
            SGStoryBean *storyBean = (SGStoryBean *)data->objectAtIndex(index);
            storyBean->getStorySections()->addObject(storyScene);
        }else {
            int var1 = story->getSpotSectionIndex();
            const char* var2 = story->getSpotSectionName()->getCString();
            CCArray *var3 = CCArray::create();
            SGStoryBean *storyBean = SGStoryBean::create(var1, var2, var3);
            storyBean->getStorySections()->addObject(storyScene);
            data->addObject(storyBean);
        }
    }
	
    return data;
}

SGExpDataModel *SGStaticDataManager::getExpByTypeAndId(int expType,int expId)
{
    if(NULL == expDict)
    {
        expDict = readPlist(PLIST_EXP);
        expDict->retain();
    }
    
    char ch_expType = 'A';
    switch(expType)
    {
        case EXPTYPE_A:
            ch_expType = 'A';
            break;
        case EXPTYPE_B:
            ch_expType = 'B';
            break;
        case EXPTYPE_C:
            ch_expType = 'C';
            break;
        case EXPTYPE_D:
            ch_expType = 'D';
            break;
        case EXPTYPE_E:
            ch_expType = 'E';
            break;
        case EXPTYPE_F:
            ch_expType = 'F';
            break;
        case EXPTYPE_Z:
            ch_expType = 'Z';
            break;
            
        default:
            ch_expType = 'D';
            break;
    }
    
    char buf[32] = {0};
    sprintf(buf, "itemExp/%c/%d",ch_expType,expId);
    std::string keyStr;
    keyStr.append(buf);
    CCDictionary *dict = (CCDictionary*)expDict->objectForKey(keyStr);
    
    if (!dict) {
        CCLOG("expBeanId-=====%s",keyStr.c_str());
        return  NULL;
    }
    
    
    return dictToExp(dict);
}
SGStoryBean* SGStaticDataManager::getStoryDataById(int storyId, bool spot)
{
    CCArray *data = NULL;
    if (spot == true) {
        data = SGStaticDataManager::shareStatic()->getAllStoryDatas();
    }else {
        if (storyId >= 20000)
        {
            data = SGStaticDataManager::shareStatic()->getAllCreamDatas();
        }
        else
        {
            data = SGStaticDataManager::shareStatic()->getAllActivityDatas();
        }
    }
    
    
    for (int ii = 0; ii < data->count(); ii++) {
        SGStoryBean *storyBean = (SGStoryBean *)data->objectAtIndex(ii);
        if (storyBean->getStoryId() == storyId) {
            return storyBean;
        }
    }
    return NULL;
}
//把dict解析成经验数据对象
SGExpDataModel *SGStaticDataManager::dictToExp(CCDictionary *dict)
{
    if (dict != NULL) {
        SGExpDataModel *exp = new SGExpDataModel();
        exp->setExpId((CCString*)dict->objectForKey("beanId"));
        exp->setExpLevel(((CCString*)dict->objectForKey("level"))->intValue());
        exp->setexpAll(((CCString*)dict->objectForKey("expAll"))->intValue());
        exp->setExpValue(((CCString*)dict->objectForKey("exp"))->intValue());
        exp->setExpReGold(((CCString*)dict->objectForKey("reGold"))->intValue());
        
        if (dict->objectForKey("equipCost"))
        {
            exp->setEquipCost(((CCString*)dict->objectForKey("equipCost"))->intValue());
        }
        exp->autorelease();
        
        return exp;
        
    }
    return NULL;
}

SGStoryBean* SGStaticDataManager::getStoryDataByPlotId(int plotId, bool spot)
{
    CCArray *data = NULL;
    if (plotId >= 10000) spot = false;
    
    if (spot == true) //默认
    {
        data = SGStaticDataManager::shareStatic()->getAllStoryDatas();
    }
    else
    {
        data = SGStaticDataManager::shareStatic()->getAllActivityDatas();
    }
    //精英副本
    if (plotId >= 20000)
    {
        data = SGStaticDataManager::shareStatic()->getAllCreamDatas();
    }
    for (int ii = 0; ii < data->count(); ii++)
    {
        SGStoryBean *storyBean = (SGStoryBean *)data->objectAtIndex(ii);
        for (int jj = 0; jj < storyBean->getStorySections()->count(); jj++)
        {
            SGStorySceneBean *sceneBean = (SGStorySceneBean *)storyBean->getStorySections()->objectAtIndex(jj);
            if (sceneBean->getPlotId() == plotId)
            {
                return storyBean;
            }
        }
    }
    return NULL;
}



int SGStaticDataManager::getGuideTagById(int gid)
{
    int tag = 0;
    SGGuideVO *model = this->getGuideModel(gid);
    if (model != NULL) {
        tag = model->getModelTag();
    }
    DDLog("ttag-------------------%d", tag);
    return tag;
}

SGGuideVO* SGStaticDataManager::getGuideModel(int gid)
{
    //数据来源 ==> uiGuide.plist
    if (NULL == guideDict) {
        guideDict = readPlist(PLIST_UIGUIDE);
        guideDict->retain();
    }
    //目前最大的引导步数
    if (gid > guide_tag_60) {
        gid = guide_tag_60;
    }
    //const char *key = CCString::createWithFormat("%d", gid)->getCString();
    
    //大分组都是由类似 "guide_tag_XX"格式，一组为引导中的一步，下面获取的是每一步引导的数据对象字典再转换成SGGuideVO对象
    CCDictionary *dict = (CCDictionary*)guideDict->objectForKey(guide_Id[gid]);
    return dictToGuide(dict,gid);
}

SGGuideVO* SGStaticDataManager::dictToGuide(CCDictionary *dict,int gid)
{
    //id由上一步guide_Id中转换而来
    int modelId = gid; //((CCString *)dict->objectForKey("modelId"))->intValue();
    int modelTag = ((CCString *)dict->objectForKey("modelTag"))->intValue();
    int modelType = ((CCString *)dict->objectForKey("modelType"))->intValue();
    //由表中的数据填充 引导所有的数据
    SGGuideVO *model = SGGuideVO::create(modelId, modelType, modelTag);
    
    CCArray *array = (CCArray *)dict->objectForKey("modelMsg");
    //modelType == 1 有引导对话， modelType == 2 无引导对话
    if(modelType == 1)
        model->setModelMsg(array);
    else if (modelType == 2)
        model->setModelMsg(NULL);
    
    return model;
}

int SGStaticDataManager::getimitTagById(int limitId, int gid)
{
    int tag = 0;
    SGLimitGuideVO *model = this->getLimitGuideModel(limitId,gid);
    if (model != NULL) {
        tag = model->getHighlightTag();
    }
    return tag;
}

SGLimitGuideVO* SGStaticDataManager::dictToLimitGuide(CCDictionary *dict,int gid)
{
    
    if (dict == NULL)
    {
        return NULL;
    }
    
    int mid = ((CCString *)dict->objectForKey("modelId"))->intValue();
    int modelType = ((CCString *)dict->objectForKey("modelType"))->intValue();
    int layerTag = 0;//((CCString *)dict->objectForKey("layerTag"))->intValue();
    int parentTag = 0;//((CCString *)dict->objectForKey("parentTag"))->intValue();
    int highlightTag = ((CCString *)dict->objectForKey("highlightTag"))->intValue();
    
    SGLimitGuideVO *model = SGLimitGuideVO::create(mid,modelType,layerTag,parentTag,highlightTag);
    
    CCArray *array = (CCArray *)dict->objectForKey("modelMsg");
    if(modelType == 1|| modelType == 3)
        model->setModelMsg(array);
    else if (modelType == 2)
        model->setModelMsg(NULL);
    
    return model;
}

SGLimitGuideVO* SGStaticDataManager::getLimitGuideModel(int limitId, int gid)
{
    if (NULL == limitGuideDict)
    {
        limitGuideDict = readPlist(PLIST_LIMITUIGUIDE);
        limitGuideDict->retain();
    }
    const char *key = CCString::createWithFormat("%d", 1)->getCString();
    CCDictionary *dict = (CCDictionary*)limitGuideDict->objectForKey(key);
    
    const char *key2 = CCString::createWithFormat("%d", limitId)->getCString();
    CCDictionary *dict2 = (CCDictionary *)dict->objectForKey(key2);
    
    const char *key3 = CCString::createWithFormat("%d", gid)->getCString();
    CCDictionary *guide = (CCDictionary *)dict2->objectForKey(key3);
    
    return dictToLimitGuide(guide,gid);
}


SGVisitViewVO* SGStaticDataManager::dictToVisitView(CCDictionary *dict)
{
    
    SGVisitViewVO *model = NULL;
    
    CCArray *array = (CCArray *)dict->objectForKey("visitDetail");
    
    if (array->count() > 0 ) {
        model = SGVisitViewVO::create();
        model->viistViewPic = (CCString*)dict->objectForKey("visitPic");
        model->setModelMsg(array);
    }
    return model;
}

SGVisitViewVO* SGStaticDataManager::getVisitViewModel(int viewId)
{
    if (NULL == visitViewDict)
    {
        visitViewDict = readPlist(PLIST_VISITHUODONG);
        visitViewDict->retain();
    }
    
    CCDictionary *dict = (CCDictionary *)visitViewDict->objectForKey(CCString::createWithFormat("%d", viewId)->getCString());
    
    if (dict) {
        return dictToVisitView(dict);
    }
    else
    {
        return NULL;
    }
    
}

SGFightGuideVO *SGStaticDataManager::getFightGuideModel(int story, int scene, int gid)
{
    if (NULL == fightGuideDict)
    {
        fightGuideDict = readPlist(PLIST_FIGHTGUIDE);
        fightGuideDict->retain();
    }
    const char *key = CCString::createWithFormat("%d", story)->getCString();
    CCDictionary *dict = (CCDictionary*)fightGuideDict->objectForKey(key);
    if (!dict)
    {
        return NULL;
    }
    
    const char *key2 = CCString::createWithFormat("%d", scene)->getCString();
    CCDictionary *dict2 = (CCDictionary *)dict->objectForKey(key2);
    if (!dict2)
    {
        return NULL;
    }
    
    const char *key3 = CCString::createWithFormat("%d", gid)->getCString();
    CCDictionary *guide = (CCDictionary *)dict2->objectForKey(key3);
    
    if (!guide)
    {
        return NULL;
    }
    
    return dictToFightGuide(guide);
}
SGFightGuideVO *SGStaticDataManager::getAutoGuideModel(int gid)
{
    if (NULL == autoGuideDict)
    {
        autoGuideDict = readPlist(PLIST_AUTOGUIDE);
        autoGuideDict->retain();
    }
    const char *key = CCString::createWithFormat("%d", 1)->getCString();
    CCDictionary *dict = (CCDictionary*)autoGuideDict->objectForKey(key);
    
    const char *key3 = CCString::createWithFormat("%d", gid)->getCString();
    CCDictionary *guide = (CCDictionary *)dict->objectForKey(key3);
    
    return dictToFightGuide(guide);
}
SGFightGuideVO *SGStaticDataManager::dictToFightGuide(cocos2d::CCDictionary *dict)
{
    if (!dict)
    {
        return NULL;
    }
    int modelId = ((CCString *)dict->objectForKey("modelId"))->intValue();
    int modelTag = 0;
    int modelType = ((CCString *)dict->objectForKey("modelType"))->intValue();
    
    
    CCArray *array = (CCArray *)dict->objectForKey("modelMsg");
    
    SGFightGuideVO *model = SGFightGuideVO::create(modelId, modelType, modelTag);
    model->setModelMsg(array);
    
    int from = 0;
    CCString *fromstr = (CCString *)dict->objectForKey("from");
    if (fromstr)
    {
        from = fromstr->intValue();
    }
    model->setFrom(from);
    
    int touch = 0;
    CCString *touchPos = (CCString *)dict->objectForKey("btnType");
    if (touchPos) {
        touch = touchPos->intValue();
    }
    model->setTouch(touch);
    
    CCString *tips = (CCString *)dict->objectForKey("tips");
    if (tips)
    { // 初始化tips
        const char* ctip = tips->getCString();
        int csize = (int)strlen(ctip);
        for (int ii = 0; ii < csize; ii+=3)
        {
            int count = ctip[ii]-'0';
            int type = ctip[ii+1]-'0';
            model->tips.push_back(count);
            model->tips.push_back(type);
        }
    }
    
    CCString *point = (CCString *)dict->objectForKey("point");
    if (point)
    {
        model->setPoint(CCPointFromString(point->getCString()));
    }
    CCString * pst = (CCString *)dict->objectForKey("pst");
    if (pst)
    {
        model->setPlace(pst->intValue());
    }
    return model;
}
SGRobotGuideVO *SGStaticDataManager::getRobotGuideModel(int scene, int story,int step)
{
    if (NULL == robotDict)
    {
        robotDict = readPlist(PLIST_ROBOTGUIDE);
        robotDict->retain();
    }
    const char *key = CCString::createWithFormat("%d", scene)->getCString();
    CCDictionary *sceneDict = (CCDictionary*)robotDict->objectForKey(key);
    if (!sceneDict)
    {
        return NULL;
    }
    CCArray *storyArr = (CCArray *)sceneDict->objectForKey( CCString::createWithFormat("%d",story)->getCString());
    
    //    CCArray *arr = (CCArray *)storyDict->objectForKey();
    
    if (storyArr == NULL ) {
        return NULL;
    }
    if (storyArr->count() <= step-1) {
        return NULL;
    }
    CCDictionary *value = (CCDictionary*)storyArr->objectAtIndex(step-1);
    if (!value)
    {
        return NULL;
    }
    CCString *type = (CCString *)value->objectForKey("type");
    CCString *point = (CCString *)value->objectForKey("point");
    
    SGRobotGuideVO *vo = SGRobotGuideVO::create(type->intValue(), CCPointFromString(point->getCString()));
    return vo;
}

SGRobotGuideVO *SGStaticDataManager::getCgRobotGuideModel(int step)
{
    if (NULL == robotDict)
    {
        robotDict = readPlist(PLIST_ROBOTGUIDE);
        robotDict->retain();
    }
    const char *key = CCString::createWithFormat("1000")->getCString();
    CCDictionary *sceneDict = (CCDictionary*)robotDict->objectForKey(key);
    if (!sceneDict)
    {
        return NULL;
    }
    CCArray *storyArr = (CCArray *)sceneDict->objectForKey( CCString::createWithFormat("1")->getCString());
    
    if (storyArr->count() <= step-1) {
        return NULL;
    }
    CCDictionary *value = (CCDictionary*)storyArr->objectAtIndex(step-1);
    if (!value)
    {
        return NULL;
    }
    CCString *type = (CCString *)value->objectForKey("type");
    CCString *point = (CCString *)value->objectForKey("point");
    
    SGRobotGuideVO *vo = SGRobotGuideVO::create(type->intValue(), CCPointFromString(point->getCString()));
    return vo;
}

SGRobotGuideVO *SGStaticDataManager::dictToRobotGuide(cocos2d::CCDictionary *dict)
{
    return NULL;
}
CCDictionary *SGStaticDataManager::getOfficerDict()
{
    if(NULL == officerDict)
    {
        officerDict = readPlist(PLIST_OFFICER);
        officerDict->retain();
    }
    return officerDict;
}

CCDictionary *SGStaticDataManager::getWeiOfficerDict()
{
    if(NULL == weiOfficerDict)
    {
        weiOfficerDict = readPlist(PLIST_WEIOFFICER);
        weiOfficerDict->retain();
    }
    return weiOfficerDict;
}
CCDictionary *SGStaticDataManager::getShuOfficerDict()
{
    if(NULL == shuOfficerDict)
    {
        shuOfficerDict = readPlist(PLIST_SHUOFFICER);
        shuOfficerDict->retain();
    }
    return shuOfficerDict;
}
CCDictionary *SGStaticDataManager::getWuOfficerDict()
{
    if(NULL == wuOfficerDict)
    {
        wuOfficerDict = readPlist(PLIST_WUOFFICER);
        wuOfficerDict->retain();
    }
    return wuOfficerDict;
}
CCDictionary *SGStaticDataManager::getQunOfficerDict()
{
    if(NULL == qunOfficerDict)
    {
        qunOfficerDict = readPlist(PLIST_QUNOFFICER);
        qunOfficerDict->retain();
    }
    return qunOfficerDict;
}

CCDictionary *SGStaticDataManager::getEquipViewDict()
{
    if(NULL == equipViewDict)
    {
        equipViewDict = readPlist(PLIST_EQUPIPVIEW);
        equipViewDict->retain();
    }
    return equipViewDict;
}

CCDictionary *SGStaticDataManager::getDisplayValueDict()
{
    if(NULL == _controlValueDict)
    {
        _controlValueDict = readPlist(PLIST_CONTROLVALUE);
        _controlValueDict->retain();
    }
    return _controlValueDict;
}


CCDictionary *SGStaticDataManager::getSoldierDict()
{
    
    if(NULL == soldierDict)
    {
        soldierDict = readPlist(PLIST_SOLDIER);
        soldierDict->retain();
    }
    
    return soldierDict;
}
CCDictionary *SGStaticDataManager::getPropsDict()
{
    if(NULL == propsDict)
    {
        propsDict = readPlist(PLIST_PROPS);
        propsDict->retain();
    }
    return propsDict;
}
CCDictionary *SGStaticDataManager::getEquipmentDict()
{
    if(NULL == equipmentDict)
    {
        equipmentDict = readPlist(PLIST_EQUIPMENT);
        equipmentDict->retain();
    }
    return equipmentDict;
}

int SGStaticDataManager::getCountryIdByOfficerId(int officerId)
{
    SGOfficerDataModel *officerData = getOfficerById(officerId);
    return officerData->getOfficerRace();
}

//获得缘分本体数据原始字典
CCDictionary* SGStaticDataManager::getFateBaseData()
{
    return fateBaseDict;
}

//获得缘分效果数据原始字典
CCDictionary* SGStaticDataManager::getFateEffectData()
{
    return fateEffectDict;
}

//获得奖励数据原始字典
CCDictionary* SGStaticDataManager::getRewardData()
{
    return rewardDict;
}

//生成原型ID级主将卡数据
void SGStaticDataManager::createProtoOfficerData()
{
    if(NULL == officerDict)
    {
        officerDict = readPlist(PLIST_OFFICER);
        officerDict->retain();
    }
    
    if (!protoOfficerDict)
    {
        //初始化字典
        protoOfficerDict = CCDictionary::create();
        protoOfficerDict->retain();
        //循环键值获得protoOfficerDict
        CCDictElement* obj = NULL; //单个原始数据对象
        CCDictionary* dict = NULL; //单个原始数据对象的值，用来初始化Model
        SGOfficerDataModel* officer = NULL; //武将原始数据
        CCDICT_FOREACH(officerDict , obj)
        {
            dict = (CCDictionary*) obj->getObject();
            
            officer = dictToOfficer(dict);
            
            //添加到原型列表
            protoOfficerDict->setObject(officer, officer->getOfficerProtoId());
        }
    }
}

//生成原型ID级装备卡数据
void SGStaticDataManager::createProtoEquipmentData()
{
    if(NULL == equipmentDict)
    {
        equipmentDict = readPlist(PLIST_EQUIPMENT);
        equipmentDict->retain();
    }
    
    if (!protoEquipmentDict)
    {
        //初始化字典
        protoEquipmentDict = CCDictionary::create();
        protoEquipmentDict->retain();
        //循环键值获得protoEquipmentDict
        CCDictElement* obj = NULL; //单个原始数据对象
        CCDictionary* dict = NULL; //单个原始数据对象的值，用来初始化Model
        SGEquipmentDataModel* equipment = NULL; //装备原始数据
        CCDICT_FOREACH(equipmentDict , obj)
        {
            dict = (CCDictionary*) obj->getObject();
            
            equipment = dictToEquipment(dict);
            /*
             equipment = new SGEquipmentDataModel();
             equipment->setEquipmentBaseAtk(((CCString*)dict->objectForKey("baseAtk"))->intValue());
             equipment->setEquipBaseDef(((CCString*)dict->objectForKey("baseDef"))->intValue());
             equipment->setEquipBaseSpeed(((CCString*)dict->objectForKey("baseSpeed"))->intValue());
             equipment->setEquipBeanId(((CCString*)dict->objectForKey("beanId")));
             equipment->setEquipCurrStarLevel(((CCString*)dict->objectForKey("currStarLevel"))->intValue());
             equipment->setEquipGoverning(((CCString*)dict->objectForKey("governing"))->intValue());
             equipment->setEquipHP(((CCString*)dict->objectForKey("hp"))->intValue());
             equipment->setEquipMaxLevel(((CCString*)dict->objectForKey("maxLevel"))->intValue());
             equipment->setEquipName(((CCString*)dict->objectForKey("name")));
             equipment->setEquipmentId(((CCString*)dict->objectForKey("itemId"))->intValue());
             equipment->setEquipRound(((CCString*)dict->objectForKey("round"))->intValue());
             equipment->setEquipType(((CCString*)dict->objectForKey("itemType"))->intValue());
             equipment->setEquipExpType((CCString*)dict->objectForKey("exptype"));
             equipment->setEquipNum(((CCString*)dict->objectForKey("num"))->intValue());
             equipment->setEquipGrowValue(((CCString*)dict->objectForKey("growvalue"))->floatValue());
             equipment->setUpgradelevel(((CCString*)dict->objectForKey("upgradeMinLevel"))->intValue());
             equipment->setUpgradestar(((CCString*)dict->objectForKey("maxStarLevel"))->intValue());
             CCString *a = (CCString*)dict->objectForKey("nextid");
             if (a) {
             equipment->setEquipNextId(((CCString*)dict->objectForKey("nextid"))->intValue());
             
             }
             equipment->setEquipVisitType(((CCString*)dict->objectForKey("visittype"))->intValue());
             if (equipment->getEquipNextId()) {
             equipment->setEquipCost(((CCString*)dict->objectForKey("cost"))->intValue());
             equipment->setReqItems((CCString*)dict->objectForKey("reqitem"));
             }
             equipment->setEquipExpRate(((CCString*)dict->objectForKey("exprate"))->floatValue());
             //装备原型ID
             equipment->setEquipProtoId(((CCString*)dict->objectForKey("protoId"))->intValue());
             
             equipment->setIconId(((CCString*)dict->objectForKey("iconId"))->intValue());//head%d，索引
             
             equipment->setAdvNum(((CCString*)dict->objectForKey("advancenum"))->intValue());
             equipment->autorelease();
             */
            
            
            
            //添加到装备原型列表
            protoEquipmentDict->setObject(equipment, equipment->getEquipProtoId());
        }
    }
}

//生成缘分相关数据
void SGStaticDataManager::createFateData()
{
    SGFateManager* sgfm = SGFateManager::sharedManager();
    sgfm->gainFateData();
}

//生成士兵属性增量因子表
void SGStaticDataManager::createSoldierGrowProperty()
{
    if (_soldierGrowPropertyMap == NULL)
    {
        _soldierGrowPropertyMap = new std::map< int, std::map<int, int>* >();
        CCDictionary* plistDict = this->readPlist(PLIST_SOLDIER_GROW_PROPERTY);
        //循环键值
        CCDictElement* obj = NULL; //单个原始数据对象
        CCDictionary* dict = NULL; //单个原始数据对象的值，用来初始化Model
        
        const int SD_SIZE = 9;
        int soldier[SD_SIZE] = {3103,3102,3101,  3203,3201,3202,  3301,3303,3302};
        char name[20];
        CCDICT_FOREACH(plistDict , obj)
        {
            int level = atoi(obj->getStrKey());
            std::map< int, std::map<int, int>* >::iterator iter = _soldierGrowPropertyMap->find(level);
            if (iter == _soldierGrowPropertyMap->end())
            {
                std::map<int, int>* sdMap = new std::map<int, int>();
                dict = (CCDictionary*) obj->getObject();
                for (int k = 0; k < SD_SIZE; k++)
                {
                    sprintf(name, "soldiergrow-%d", soldier[k]);
                    int ppAdd = ((CCString*) dict->objectForKey(name))->intValue();
                    sdMap->insert(std::pair<int, int>(soldier[k], ppAdd));
                }
                _soldierGrowPropertyMap->insert(std::pair<int, std::map<int, int>* >(level, sdMap));
            }
        }
    }
}

//根据士兵ID和LV获得其属性增量因子。
int SGStaticDataManager::getSoldierGrowProperty(int sid, int slevel)
{
    std::map< int, std::map<int, int>* >::iterator iter = _soldierGrowPropertyMap->find(slevel);
    if (iter != _soldierGrowPropertyMap->end())
    {
        std::map<int, int>::iterator sIter =  iter->second->find(sid);
        if (sIter != iter->second->end())
        {
            return sIter->second;
        }
    }
    //SNCH
    assert(1 == 0);
}


//根据类型获得物品的原始数据，目前有将领=0/装备=1/士兵=2/道具=3/武将卡碎片=4/消耗品=5
CCObject* SGStaticDataManager::getItemByTypeAndId(int type, int id)
{
    if (IT_OFFICER == type)
    {
        return getOfficerById(id);
    }
    else if (IT_EQUIPMENT == type)
    {
        return getEquipById(id);
    }
    else if (IT_SOLIDER == type)
    {
        return getSoliderById(id);
    }
    else if (IT_PROPS == type)
    {
        return getPropsById(id);
    }
    else if (IT_PIECES == type)
    {
        return getPiecesDictById(id);
    }
	else if (IT_CONSUME == type)//消耗品
	{
		return getConsumableById(id);
	}
    else
    {
        return NULL;
    }
}






//根据类型获得物品的名称，目前有将领=0/装备=1/士兵=2/道具=3/武将卡碎片=4
SeRewardTypeId* SGStaticDataManager::getItemNameByTypeAndId(int type, int id)
{
    
    SeRewardTypeId*temp=new SeRewardTypeId;
    if (IT_OFFICER == type)
    {
        SGOfficerDataModel* obj = (SGOfficerDataModel*) getOfficerById(id);
        //            return obj->getOfficerName();
        temp->idid=id;
        temp->typetype=type;
        if (obj)
            
            return temp;
    }
    else if (IT_EQUIPMENT == type)
    {
        SGEquipmentDataModel* obj = (SGEquipmentDataModel*) getEquipById(id);
        //        if (obj)
        //            return obj->getEquipName();
        temp->idid=id;
        temp->typetype=type;
        if (obj)
            
            return temp;
    }
    else if (IT_SOLIDER == type)
    {
        SGSoldierDataModel* obj = (SGSoldierDataModel*) getSoliderById(id);
        //        if (obj)
        //            return obj->getSoldierName();
        temp->idid=id;
        temp->typetype=type;
        if (obj)
            
            return temp;
    }
    else if (IT_PROPS == type)
    {
        SGPropsDataModel* obj = (SGPropsDataModel*) getPropsById(id);
        //        if (obj)
        //            return obj->getPropsName();
        temp->idid=id;
        temp->typetype=type;
        if (obj)
            
            return temp;
    }
    else if (IT_PIECES == type)
    {
        SGPiecesDataModel* obj = (SGPiecesDataModel*) getPiecesDictById(id);
        //        if (obj)
        //            return obj->getPiecesName();
        temp->idid=id;
        temp->typetype=type;
        if (obj)
            
            return temp;
    }
	else if (IT_CONSUME == type)//消耗品
	{
		SGConsumableDataModel *obj = (SGConsumableDataModel *)getConsumableById(id);
		temp->idid = id;
		temp->typetype = type;
		if (obj)
			return temp;
	}
	
	
    return NULL;
}

CCArray * SGStaticDataManager::getMainGroupTask(){
    
    CCArray *groupArray=CCArray::create();
//    groupArray->retain();
    
    if (!mainTaskGroupDic) {
        mainTaskGroupDic=readPlist(PLIST_MAINTASK);
        mainTaskGroupDic->retain();
    }
    
    for (int ii=0; ii<mainTaskGroupDic->count(); ii++) {
        CCDictionary *dic=(CCDictionary *)mainTaskGroupDic->objectForKey(CCString::createWithFormat("%d",ii+1)->getCString());
        dic->retain();
        SGMainTaskGroup *obj=dicToMainTaskGroup(dic);
        groupArray->addObject(obj);
    }
    
    return groupArray;
}

SGMainTaskGroup * SGStaticDataManager::dicToMainTaskGroup(CCDictionary *dic){
    
    SGMainTaskGroup *group=new SGMainTaskGroup();
    group->autorelease();
    int id=((CCString *)dic->objectForKey("id"))->intValue();
    int initQuestId=((CCString *)dic->objectForKey("initQuestId"))->intValue();
    int tabId=((CCString *)dic->objectForKey("tabId"))->intValue();
    CCString * groupName=((CCString *)dic->objectForKey("groupName"));
    group->settaskGroupId(id );
    group->setinitQuestId(initQuestId);
    group->settabId(tabId);
    group->settaskGroupName(groupName);
    
    return group;
    
    
}

SGOfficerCard *SGStaticDataManager::officerDataModeToOfficer(int itemId){
    
    SGOfficerDataModel *oCard = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
    
    
    SGOfficerCard *officerCard = new SGOfficerCard;
    
    officerCard->setOfficerName(oCard->getOfficerName());
    officerCard->setMaxLevel(oCard->getOfficerMaxLevel());
    officerCard->setCurrExp(0);
    officerCard->setCurrStar(oCard->getOfficerCurrStarLevel());
    officerCard->setCurrLevel(1);
    officerCard->setRace(oCard->getOfficerRace());
    officerCard->setItemId(oCard->getOfficerId());
    //转生最大星级
    officerCard->setUpgradestar(oCard->getUpgradestar());
    //转生等级
    officerCard->setUpgradelevel(oCard->getUpgradelevel());
    //武将计最大等级
    officerCard->setSkillMaxLevel(oCard->getSkillMaxLevel());
    //主将计最大等级
    officerCard->setLordMaxLevel(oCard->getLordMaxLevel());
    //性别和缘分
    officerCard->setGender((OfficerGender) oCard->getOfficerGender());
    officerCard->setFateList(oCard->getOfficerFateList());
    //武将原型ID
    officerCard->setProtoId(oCard->getOfficerProtoId());
    //速度修正系数
    officerCard->setSpeedFactor(oCard->getOfficerSpeedFactor());
    //转生次数
    officerCard->setAdNum(oCard->getAdvNum());
    officerCard->setHeadSuffixNum(oCard->getIconId());//12.05
    
    if (oCard->getOfficerGeneralsSkill()&& !oCard->getOfficerGeneralsSkill()->isEqual(CCString::create(""))) {
        CCLOG("%s",oCard->getOfficerGeneralsSkill()->getCString());
        
        officerCard->setOfficerSkil(SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(oCard->getOfficerGeneralsSkill()->getCString()));
    }
    
    if (oCard->getOfficerLordSkill() &&!oCard->getOfficerLordSkill()->isEqual(CCString::create(""))) {
        
        int lordskill = SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(oCard->getOfficerLordSkill()->getCString());
        
        
        officerCard->setLordSkill(lordskill);
        
    }
    officerCard->setAtk(oCard->getOfficerBaseAtk());
    officerCard->setDef(oCard->getOfficerBaseDef());
    officerCard->setRound(oCard->getOfficerRound());
    officerCard->setSpeed(oCard->getOfficerBaseSpeed());
    officerCard->setMorale(oCard->getOfficerMorale());
    officerCard->setGoverning(oCard->getOfficerGoverning());
    officerCard->setHeadSuffixNum(oCard->getIconId());//12.05
    
    officerCard->autorelease();
    
    return officerCard;
}
SGEquipCard * SGStaticDataManager::equipDataModeToequip(int ItemId){
    
    SGEquipmentDataModel *equipData = SGStaticDataManager::shareStatic()->getEquipById(ItemId);
    SGEquipCard *equipCard = new SGEquipCard;
    
    //填充装备卡数据/////////////////////////////////////////////////////////////////////
    
    equipCard->setProtoId(equipData->getEquipProtoId());
    equipCard->setCurrStar(equipData->getEquipCurrStarLevel());
    equipCard->setCurrExp(0);
    equipCard->setCurrLevel(1);
    equipCard->setOfficerCardId(0);
    equipCard->setItemType(equipData->getEquipType());
    equipCard->setIsProt(0);
    equipCard->setOfficerName(CCString::create(equipData->getEquipName()->getCString()));
    equipCard->setMorale(equipData->getEquipHP());
    equipCard->setAtk(equipData->getEquipmentBaseAtk());
    equipCard->setDef(equipData->getEquipBaseDef());
    equipCard->setRound(equipData->getEquipRound());
    equipCard->setSpeed(equipData->getEquipBaseSpeed());
    equipCard->setMaxLevel(equipData->getEquipMaxLevel());
    //转生等级
    equipCard->setUpgradelevel(equipData->getUpgradelevel());
    //转生次数
    equipCard->setAdNum(equipData->getAdvNum());
    //转生最大星级
    equipCard->setUpgradestar(equipData->getUpgradestar());
    //装备原型ID
    equipCard->setProtoId(equipData->getEquipProtoId());
    equipCard->setItemId(equipData->getEquipmentId());
    //装备转生材料ID
    equipCard->setMaterialDependencyId(equipData->getMaterialDependencyId());
    //装备与武将缘分
    equipCard->setFateDesc(equipData->getFateDesc());
    
    equipCard->autorelease();
    return equipCard;
    
}
SGPropsCard *SGStaticDataManager::propDataModelToProp(int itemId){

    SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(itemId);
    SGPropsCard *propCard = new SGPropsCard();
    propCard->setItemId(itemId);
    //           propCard->setIsProt(cardInfo.isprotection());
    
    propCard->setSell(prop->getpropsSell());
    propCard->setValue(prop->getpropsValue());
    propCard->setRace(prop->getPropsCountry());
    propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
    propCard->setType(prop->getPropsPropsType());
    propCard->setItemType(prop->getPropsType());
    propCard->setCurrStar(prop->getPropsCurrStarLevel());
    propCard->setHeadSuffixNum(prop->getIconId());//12.05
    propCard->autorelease();
    
    return propCard;
}
SGPiecesCard *SGStaticDataManager::piecesDataModelToPiece(int itemId){
    
    SGPiecesCard *piecesCard = new SGPiecesCard();
    SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(itemId);
   
    piecesCard->setItemId(itemId);//碎片id
    
    piecesCard->setSell(pieces->getPiecesPrice());//价格
    piecesCard->setOfficerName(pieces->getPiecesName());//名称
    piecesCard->setPiecesName(pieces->getPiecesName());
    piecesCard->setItemType(pieces->getPiecesType());//类型
    piecesCard->setCurrStar(pieces->getStarLevel());//星级
//    piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
    piecesCard->setPiecesDesc(CCString::create(""));
    piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
    piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
    piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
    piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
    
    //新添 武将碎片对应武将protoId
    piecesCard->setOfficerProtoId(pieces->getOfficerProtoId());
    piecesCard->autorelease();

    return piecesCard;
}
SGConsumableCard *SGStaticDataManager::consumableDataModelToConsumable(int itemId){

    SGConsumableCard *consumeCard = new SGConsumableCard();
    
    SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(itemId);
    
    consumeCard->setItemId(itemId);
    
    
    consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
    consumeCard->setSell(consumeModel->getConsumePrice());
    consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
    consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
    consumeCard->setOfficerName(consumeModel->getConsumeName());
    consumeCard->setMoneyType(consumeModel->getMoneyType());
    consumeCard->setItemType(consumeModel->getConsumeType());
    consumeCard->setHeadSuffixNum(consumeModel->getIconId());
    consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
    consumeCard->setPicId(consumeModel->getConsumePicId());
    
    consumeCard->autorelease();
    return consumeCard;
}

SGMaterialCard *SGStaticDataManager::materialDataModelToMaterial(int itemId)
{
    SGMaterialCard *materialCard = new SGMaterialCard();
    SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(itemId);

    materialCard->setItemId(itemId);
    
    materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
    materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
    materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
    materialCard->setOfficerName(materialModel->getMaterialName());
    materialCard->setSell(materialModel->getMaterialSoldPrice());
    materialCard->autorelease();
    
    return materialCard;
}

//根据itemType和itemId返回一个基类卡牌对象，主要用于CIB，附带itemNum参数。
SGBaseMilitaryCard* SGStaticDataManager::getOneBmcByTypeAndId(int itemType, int itemId, int itemNum)
{
    SGStaticDataManager* sdm = this;
    SGBaseMilitaryCard* bmc = NULL;
    SGBaseItemType type = (SGBaseItemType) itemType;
    switch (type)
    {
        case BIT_OFFICER: //武将
        {
            bmc = sdm->officerDataModeToOfficer(itemId);
        }
            break;
        case BIT_EQUIP: //装备
        {
            bmc = sdm->equipDataModeToequip(itemId);
        }
            break;
        case BIT_PROP: //道具
        {
            bmc = sdm->propDataModelToProp(itemId);
        }
            break;
        case BIT_PIECE: //碎片
        {
            bmc = sdm->piecesDataModelToPiece(itemId);
        }
            break;
        case BIT_CONSUMABLE: //消耗品
        {
            bmc = sdm->consumableDataModelToConsumable(itemId);
        }
            break;
        case BIT_MATERIAL: //材料
        {
            bmc = sdm->materialDataModelToMaterial(itemId);
        }
            break;
        default:
            CCLOG("wrong one item type!!");
            break;
    }
    if (bmc)
    {
        bmc->setItemNum(itemNum);
    }
    
    return bmc;
}

int SGStaticDataManager::getStrengOfficerType( int officerId)
{
    SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(officerId);
    
    int type = 0;
    
    if (officer)
    {
        if (officer->getOfficerExpType()->isEqual(CCString::create("A"))) {
            type = 0;
        }
        else if (officer->getOfficerExpType()->isEqual(CCString::create("B"))) {
            type = 1;
        }
        else if (officer->getOfficerExpType()->isEqual(CCString::create("C"))) {
            type = 2;
        }
        else if (officer->getOfficerExpType()->isEqual(CCString::create("D"))) {
            type = 3;
        }
        else if (officer->getOfficerExpType()->isEqual(CCString::create("E"))) {
            type = 4;
        }
        else if (officer->getOfficerExpType()->isEqual(CCString::create("F"))) {
            type = 5;
        }
        else if (officer->getOfficerExpType()->isEqual(CCString::create("Z"))) {
            type = 6;
        }
    }
    
    return type;
}


int SGStaticDataManager::getStrengEquipType(int equipId)
{
    int type = 0;
    SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(equipId);
    
    if (equip)
    {
        if (equip->getEquipExpType()->isEqual(CCString::create("A"))) {
            type = 0;
        }
        else if (equip->getEquipExpType()->isEqual(CCString::create("B"))) {
            type = 1;
        }
        else if (equip->getEquipExpType()->isEqual(CCString::create("C"))) {
            type = 2;
        }
        else if (equip->getEquipExpType()->isEqual(CCString::create("D"))) {
            type = 3;
        }
        else if (equip->getEquipExpType()->isEqual(CCString::create("E"))) {
            type = 4;
        }
        else if (equip->getEquipExpType()->isEqual(CCString::create("F"))) {
            type = 5;
        }
        else if (equip->getEquipExpType()->isEqual(CCString::create("Z"))) {
            type = 6;
        }
    }
    return type;
}

int SGStaticDataManager::getStrengSoldierType( int soldierId)
{
    int type = 6;
    SGSoldierDataModel *soldier = SGStaticDataManager::shareStatic()->getSoliderById(soldierId);
    if (soldier)
    {
//        if (soldier->getSoldierExpType()->isEqual(CCString::create("A"))) {
//            type = 0;
//        }
//        else if (soldier->getSoldierExpType()->isEqual(CCString::create("B"))) {
//            type = 1;
//        }
//        else if (soldier->getSoldierExpType()->isEqual(CCString::create("C"))) {
//            type = 2;
//        }
//        else if (soldier->getSoldierExpType()->isEqual(CCString::create("D"))) {
//            type = 3;
//        }
//        else if (soldier->getSoldierExpType()->isEqual(CCString::create("E"))) {
//            type = 4;
//        }
//        else if (soldier->getSoldierExpType()->isEqual(CCString::create("F"))) {
//            type = 5;
//        }
//        else if (soldier->getSoldierExpType()->isEqual(CCString::create("Z"))) {
//            type = 6;
//        }
        type = 6;
    }

    return type;
}

int  SGStaticDataManager::getStrengCardType(CCString *cardClassName ,int cardId)
{
    int type = 0;
    if(cardClassName->isEqual(CCString::create("SGOfficerCard")))
    {
        type = SGStaticDataManager::shareStatic()->getStrengOfficerType(cardId);
    }
    else if(cardClassName->isEqual(CCString::create("SGEquipCard")))
    {
        type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardId);
    }
    else if(cardClassName->isEqual(CCString::create("SGSoldierCard")))
    {
        type = SGStaticDataManager::shareStatic()->getStrengSoldierType(cardId);
    }
    return type;
}


//获得国家信息dict
CCDictionary* SGStaticDataManager::getNationListPlist()
{
    if (_nationListDict == NULL)
    {
        _nationListDict = this->readPlist(PLIST_NATIONLIST);
        _nationListDict->retain();
    }
    return _nationListDict;
}

//返回AI战斗XML数据
tinyxml2::XMLDocument* SGStaticDataManager::getSkillFuzzyXml()
{
    if (_skillFuzzyXml == NULL)
    {
        _skillFuzzyXml = this->readXml(XML_SKILL_FUZZY);
    }
    return _skillFuzzyXml;
}

void SGStaticDataManager::createFragmentNum()
{
    int starLev = 0;
    int advNum = 0;
    int needNum = 0;
    CCDictionary * dict = readPlist(PLIST_FRAGMENT_NUM);
    CCDictElement * ele = NULL;
    CCDictionary * row=NULL;
    std::map<int , std::map<int , int> >::iterator iter;
    std::map<int , int>::iterator iterIn;
    CCDICT_FOREACH(dict, ele)
    {
        row = (CCDictionary *)ele->getObject();
        starLev = ( (CCString *)row->objectForKey("currStarLevel"))->intValue();
        advNum  = ( (CCString *)row->objectForKey("smillStar"))->intValue();
        needNum = ( (CCString *)row->objectForKey("fragmentNum"))->intValue();
        
        if((iter = _fragmentNumMap.find(starLev)) == _fragmentNumMap.end())
        {
            std::map<int , int> mapV;
            mapV.insert(std::pair<int, int>(advNum , needNum));
            _fragmentNumMap.insert(std::pair<int,std::map<int, int> >(starLev , mapV) );
        }
        else
        {
            std::pair<int , int > pairV(advNum , needNum);
            iter->second.insert(pairV);
        }
        
    }
    
}

int SGStaticDataManager::getAdvOfficerNeedPieceNum(int starLev, int advNum)
{
    int ret = 0;
    std::map<int , std::map<int ,int> >::iterator iter;
    iter = _fragmentNumMap.find(starLev);
    if(iter == _fragmentNumMap.end())
        return ret;
    
    std::map<int , int>::iterator iterIn = iter->second.find(advNum);
    if(iterIn == iter->second.end())
        return ret;
    
    ret = iterIn->second;
    return ret;
}


int SGStaticDataManager::getBaseCombatValue(int type, int lev , int starLev, int advNum)
{
    int ret = 0;
    SGBaseCombatValueDataModel * model = NULL;
    for(int i=0; i<m_allBaseCombatValueDataModel->count() ; ++i)
    {
        model = dynamic_cast<SGBaseCombatValueDataModel *>(m_allBaseCombatValueDataModel->objectAtIndex(i));
        if(type == model->getType()  &&  starLev==model->getStarLev()  &&  advNum==model->getAdvNum())
        {
            ret = model->getCombatValue() * (1 + model->getLevelRate()*lev );
            break;
        }
    }
    return ret;
}

int SGStaticDataManager::getSkillCombatValue(int grid, int lordId, int passivitySkillId)
{
    int ret =0;
    int lev = 0;
    int skillWeight=0;
    SGSkillDataModel * skillModel = NULL;
    
    //主将技  判断目的 某些武将无主将技能、被动技
    if(0 < lordId)
    {
        skillModel = getLordSkillById(lordId);
        lev = skillModel->getLevel();
        skillWeight = skillModel->getSkillType();
        ret += getSkillCombatAssistFunc(1, grid, lev, skillWeight);
    }
    //被动技
    if(0 < passivitySkillId)
    {
        skillModel = getGeneralSkillById(passivitySkillId);
        //这是前人解析数据表时巨大的坑。。
        lev = skillModel->getSkillMaxLevel();
        skillWeight = skillModel->getSkillStarLvl();
        ret += getSkillCombatAssistFunc(2, grid, lev, skillWeight);
    }
    
    
    return ret;
}


int SGStaticDataManager::getSkillCombatAssistFunc(int type, int grid, int lev, int skillWeight)
{
    int ret = 0;
    SGSkillCombatValueDataModel * model = NULL;
    for(int i =0; i < m_allSkillCombatValueDataModel->count() ; ++i)
    {
        model = dynamic_cast<SGSkillCombatValueDataModel *>(m_allSkillCombatValueDataModel->objectAtIndex(i));
        if(type == model->getType()  &&   grid==model->getGrid()  &&  lev==model->getSkillLev())
        {
            switch(skillWeight)
            {
                case 3:
                    ret = model->getWeightA();
                    break;
                case 4:
                    ret = model->getWeightB();
                    break;
                case 5:
                    ret = model->getWeightC();
                    break;
                case 6:
                default:
                    ret =0;
                    break;
            }
            break;
        }
    }
    return ret;
}


