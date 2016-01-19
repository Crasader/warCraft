//
//  SGStaticDataManager.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-21.
//
//


/*
 *  各种文件的文件名
 */
#include "SGSoulExchangeLayer.h"

//原始的49个数据表，48个PLIST/1个XML BGN
#define PLIST_SBLOCAL       "sbLocal.plist"       //士兵本地数据，added by cgp
#define PLIST_LOADING_OFFICER   "loadingOfficer.plist"    //战斗loading表，added by cgp
#define PLIST_ACTIVITY      "activity.plist"        //活动表
#define PLIST_AUTOGUIDE     "autoguide.plist" // 序章演示表
#define PLIST_BATTLEWORD    "battleword.plist"      //战斗台词
#define PLIST_CREAMINFO      "boss.plist"   //精英闯关
#define PLIST_CONSUMABLE	"Consumableslist.plist"//道具（消耗品）表
#define PLIST_CONTROLVALUE     "controlvalue.plist"//controlValue即原来的displayValue
#define PLIST_EQUIPMENT     "equipment.plist"       //装备表
#define PLIST_EQUPIPVIEW       "equipView.plist"         //武将卡文件
#define PLIST_EXP           "exp.plist"             //经验表
#define PLIST_FATE_BASE     "fateBase.plist" //缘分列表
#define PLIST_FATE_EFFECT   "fateEffect.plist" //缘分效果列表
#define PLIST_FIGHTGUIDE    "fightguide.plist" // 战斗教学表
#define PLIST_FIGHTWORD     "fightword.plist"       //战场台词
#define PLIST_HELP           "help.plist"
#define PLIST_HELPINFO        "helpinfo.plist"
#define PLIST_BUFF          "junxianbuff.plist"//军衔buff说明表
#define PLIST_LIMITUIGUIDE  "limitGuide.plist"         //功能教学表
#define PLIST_LOOTWIKI      "lootWiki.plist" //图鉴来源
#define PLIST_MATERIAL_LOOTWIKI "MateriallootWiki.plist" //装备来源
#define PLIST_NATIONLIST    "nationList.plist" //国家介绍
#define PLIST_NICKNAME      "nickName.plist"        //昵称表
#define PLIST_OFFICER       "officer.plist"         //武将卡文件
#define PLIST_PIECES		  "pieces.plist"		// 碎片列表
#define PLIST_PLAYER        "player.plist"          //玩家角色表
#define PLIST_PROPS         "props.plist"           //道具表
#define PLIST_JINDU         "PvpJindutiao.plist"//pvp进度条说明表
#define PLIST_TIP           "pvpspk.plist" // 小提示
#define PLIST_MAINTASKQUEST     "quest.plist"
#define PLIST_MAINTASK      "questgroup.plist"
#define PLIST_QUNOFFICER       "qunofficer.plist"         //武将卡文件
#define PLIST_REWARD        "reward.plist" //奖励列表
#define PLIST_ROBOTGUIDE    "robotguide.plist" //
#define PLIST_SHUOFFICER       "shuofficer.plist"         //武将卡文件
#define PLIST_SOLDIER       "soldier.plist"         //士兵表
#define PLIST_SOLDIER_GROW_SKILL    "soldiergrowskill.plist"//士兵技能-等级对应表
#define PLIST_SOLDIER_SKILL "soldierskill.plist"//士兵技能表
#define PLIST_SPOT          "spot.plist"            //关卡表
#define PLIST_SUIT          "suit.plist"            //套装表
#define PLIST_TASK          "task.plist"            //日常任务
#define PLIST_TYPE          "type.plist"            //类型
#define PLIST_UIGUIDE       "uiGuide.plist"         //UI教学表
#define PLIST_VIP           "vip.plist"             //vip表
#define PLIST_VISITHUODONG      "visitview.plist"//探访活动界面配置表
#define PLIST_WEIOFFICER       "weiofficer.plist"         //武将卡文件
#define PLIST_WUJIANGJI     "wujiangji.plist"       //被动技
#define PLIST_WJJ_COST   "wujiangjicost.plist" //武将计花费列表
#define PLIST_WUOFFICER       "wuofficer.plist"         //武将卡文件
#define PLIST_ZHUJIANGJI    "zhujiangji.plist"      //主将计
#define PLIST_ZJJ_COST   "zhujiangjiexp.plist" //主将计经验列表
#define XML_SKILL_FUZZY "skillfuzzy.xml" //AI战斗数据
//原始的49个数据表，48个PLIST/1个XML BGN

//新增的数据表 BGN
#define PLIST_MATERIAL  "Material.plist"//装备进阶的材料表
#define PLIST_MATERIALDEPENDENCY    "MaterialDependency.plist"//装备转生所需要的材料的表
#define PLIST_SOLDIER_GROW_PROPERTY "soldierGrowProperty.plist" //士兵的属性增量因子表，按等级提升。
#define PLIST_FRAGMENT_NUM "FragmentNum.plist" //武将转生需碎片数量
#define PLIST_BASE_COMBAT_VALUE "baseCombatValue.plist" //武将、装备基础战斗力表
#define PLIST_SKILL_COMBAT_VALUE "skillCombatValue.plist" //武将技能（主动技、被动技）战斗力加成 作用加成到武将战斗力
//#define PLIST_SPECAIL_BATTLE "specialBattlePlot.plist"    //
#define PLIST_SPBATTLE_RULE      "sphelp.plist"             //特殊战斗的规则


#define PLIST_FIGHT_OVER_NOTICE "fightOverNotice.plist" //战斗结算弹窗提示数据表
#define PLIST_FIGHT_OVER_NOTICE_BTN "fightOverNoticeBtn.plist"


//新增的数据表 BGN

/*
    经验类型
 */
#define EXPTYPE_A 0
#define EXPTYPE_B 1
#define EXPTYPE_C 2
#define EXPTYPE_D 3
#define EXPTYPE_E 4
#define EXPTYPE_F 5
#define EXPTYPE_Z 6

// 目前玩家最高等级
#define MaxPlayerLevel 99


#ifndef __GameSanGuo__SGStaticDataManager__
#define __GameSanGuo__SGStaticDataManager__

// 目前最大关卡ID
#define MaxSceneId      162
#define MaxStoryId      35

#define MaxActivityId 19999
//精英最大头上
#define MaxCreamId 21000

/**
 *  UI教学Tag
 **/
typedef enum{
    guide_tag_0 = 0,
    guide_tag_1,
    guide_tag_2,
    guide_tag_3,
    guide_tag_4,
    guide_tag_5,
    guide_tag_6,
    guide_tag_7,
    guide_tag_7_1,
    guide_tag_7_2,
    guide_tag_7_3,
    guide_tag_8,
    guide_tag_9,
    guide_tag_10,
    guide_tag_11,
    guide_tag_12,
    guide_tag_13,
    guide_tag_14,
    guide_tag_15,
    guide_tag_16,
    guide_tag_17,
    guide_tag_18,
    guide_tag_19,
    guide_tag_20,
    guide_tag_21,
    guide_tag_22,
    guide_tag_22_1,
    guide_tag_23,
    guide_tag_24,
    guide_tag_25,
    guide_tag_26,
//    guide_tag_27,
//    guide_tag_28,
//    guide_tag_29,
    guide_tag_30 = 31,
    guide_tag_31,
    guide_tag_32,
    guide_tag_33,
    guide_tag_34,
    guide_tag_35,
    guide_tag_36,
    guide_tag_37,
    guide_tag_38,
    guide_tag_39,
    guide_tag_40,
    guide_tag_41,
    guide_tag_42,
    guide_tag_43,
    guide_tag_44,
    guide_tag_45,
    guide_tag_46,
    guide_tag_47,
    guide_tag_48,
    guide_tag_49,
    guide_tag_50,
    guide_tag_51,
    guide_tag_52,
    guide_tag_53,
    guide_tag_54,
    guide_tag_55,
    guide_tag_56,
    guide_tag_57,
    guide_tag_58,
    guide_tag_59,
    guide_tag_60,
    guide_tag_99 = 99,
}guideTags;
/**
 *  UI教学Tag
 **/
typedef enum{
    step_0 = 0,
    step_1,
    step_2,
    step_3,
    step_4,
    step_5,
    step_6,
    step_7,
    step_8,
    step_9,
    step_10,
    step_11,
    step_12,
    step_13,
    step_14,
    step_15,
    step_16,
    step_17,
    step_18,
    step_19,
    step_20,
    step_21,
    step_22,
    step_23,
    step_24,
    step_25,
    step_26,
    step_27,
    step_28,
    step_29,
    step_30,
    step_31,
    step_32,
    step_33,
    step_34,
    step_35,
    step_36,
    step_37,
    step_38,
    step_39,
    step_40,
    step_41,
    step_42,
    step_43,
    step_44,
    step_45,
    step_46,
    step_47,
    step_48,
    step_49,
    step_50,
    step_51,
    step_52,
    step_53,
    step_54,
    step_55,
    step_56,
    step_57,
    step_58,
    step_59,
    step_60,
    step_99 = 99,
}limitGuideTags;
//物品类型
enum ItemType
{
    IT_OFFICER = 6,
    IT_EQUIPMENT = 7,
    IT_SOLIDER = 8,
    IT_PROPS = 9,
    IT_PIECES = 10,
	IT_CONSUME = 15//消耗品
};


#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

#include "SGSkillDataModel.h"
#include "SGOfficerDataModel.h"
#include "SGPropsDataModel.h"
#include "SGEquipmentDataModel.h"
#include "SGSoldierDataModel.h"
#include "SGSpotDataModel.h"
#include "SGSuitDataModel.h"
#include "SGVipDataModel.h"
#include "SGPlayerDataModel.h"
#include "SGExpDataModel.h"
#include "SGDayTaskDataModel.h"
#include "SGStoryBean.h"
#include "SGStorySceneBean.h"
#include "SGGuideVO.h"
#include "SGFightGuideVO.h"
#include "SGRobotGuideVO.h"
#include "SGHelpTitleDataModel.h"

#include "SGPiecesDataModel.h"
#include "SGMaterialDataModel.h"
#include "SGMaterialDependencyDataModel.h"
#include "SGRewardDataModel.h"
#include "SGLimitGuideVO.h"

#include "SGLuaDataModel.h"
#include "SGJunbuff.h"

#include "SGConsumableDataModel.h"

#include "SGVisitViewVO.h"
#include "SGSoldierSkillDataModel.h"
#include "SGBaseCombatValueDataModel.h"
#include "SGSkillCombatValueDataModel.h"
#include "SGSpecialBattleDataModel.h"

#include "SGSbLocalDataModel.h"
#include "SGLoadingOfficerDataModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SGStaticDataManager :public CCObject
{
private:
    CCDictionary *sbLocalDict;      //士兵和武将的攻击动作时间，特效ID等，主键为iFileID , added by cgp
    
    CCDictionary *loadingOfficerDict;      //战斗登陆界面随机人物 added by cgp
    
    CCDictionary *officerDict;      //主将卡静态数据 PLIST_OFFICER
    
    CCDictionary *protoOfficerDict; //由原型ID对应的主将卡静态数据，附属于officerDict。
    
    CCDictionary *zhujiangjiDict;   //主将计静态数据 PLIST_ZHUJIANGJI
    
    CCDictionary *wujiangjiDict;    //被动技静态数据 PLIST_WUJIANGJI
    
    CCDictionary *nickNameDict;     //昵称静态数据 PLIST_NICKNAME
    
    CCDictionary *soldierDict;      //士兵静态数据 PLIST_SOLDIER
    
    CCDictionary *propsDict;        //道具静态数据 PLIST_PROPS
    
    CCDictionary *equipmentDict;    //装备静态数据 PLIST_EQUIPMENT
    
    CCDictionary *protoEquipmentDict; //由原型ID对应的装备卡静态数据，附属于equipmentDict。
    
    CCDictionary *cardTypeDict;     //卡牌类型静态数据 PLIST_TYPE
    
    CCDictionary *battlewordDict;   //战斗台词静态数据 PLIST_BATTLEWORD
    
    CCDictionary *fightwordDict;    //战场台词静态数据 PLIST_FIGHTWORD
    
    CCDictionary *spotDict;         //关卡的静态数据 PLIST_SPOT
    
    CCDictionary *activityDict;     //活动的静态数据 PLIST_ACTIVITY
    
    CCDictionary *suitDict;         //套装的静态数据 PLIST_SUIT
    
    CCDictionary *vipDict;          //vip静态数据 PLIST_VIP
    
    CCDictionary *playerDict;       //玩家角色静态数据 PLIST_PLAYER
    
    CCDictionary *expDict;          //经验静态数据 PLIST_EXP
    
    CCDictionary *taskDict;          //经验静态数据 PLIST_TASK
    
    CCDictionary *guideDict;        //UI教学表 PLIST_UIGUIDE
    
    CCDictionary *fightGuideDict;//战斗教学表 PLIST_FIGHTGUIDE
    
    CCDictionary *limitGuideDict;//战斗教学表 PLIST_LIMITUIGUIDE
    
    CCDictionary *autoGuideDict;//序章演示表 PLIST_AUTOGUIDE
    
    CCDictionary *robotDict; //PLIST_ROBOTGUIDE
    
    CCDictionary *tipDict; //PLIST_TIP
    
    CCDictionary *helpDict; //PLIST_HELP
    
    CCDictionary *helpinfoDict; //PLIST_HELPINFO
	
	CCDictionary *m_bossDict;//精英闯关数据 PLIST_CREAMINFO
    
    CCDictionary* fateBaseDict; //缘分本体数据 PLIST_FATE_BASE
    
    CCDictionary* fateEffectDict; //缘分效果数据 PLIST_FATE_EFFECT
	
	CCDictionary *piecesDict; //碎片数据 PLIST_PIECES

    CCDictionary *wujiangjicostDict; //武将计花费数据 PLIST_WJJ_COST
    
    CCDictionary *zhujiangjicostDict; //主将计经验数据 PLIST_ZJJ_COST
    
    CCDictionary* rewardDict; //奖励数据 PLIST_REWARD
    
    CCDictionary* buffDict; //军衔说明数据 PLIST_BUFF
	
	CCDictionary *consumeableDict;//消耗品（道具）数据 PLIST_CONSUMABLE

    CCDictionary* pvpjundutiao;//进度条数据 PLIST_JINDU
    
    CCDictionary *visitViewDict;//探访活动界面表 PLIST_VISITHUODONG
    
    CCDictionary *weiOfficerDict; //魏国主将卡静态数据 PLIST_WEIOFFICER
    
    CCDictionary *shuOfficerDict; //蜀国主将卡静态数据 PLIST_SHUOFFICER
    
    CCDictionary *wuOfficerDict; //吴国主将卡静态数据 PLIST_WUOFFICER
    
    CCDictionary *qunOfficerDict; //群英主将卡静态数据 PLIST_QUNOFFICER
    
    CCDictionary *equipViewDict; //PLIST_EQUPIPVIEW
    
    CCDictionary *_controlValueDict; //显隐控制信息dict PLIST_CONTROLVALUE
    
    CCDictionary *soldierskillDict; //士兵技能表 PLIST_SOLDIER_SKILL
    
    CCDictionary *soldiergrowskillDict; //士兵技能-等级对应表 PLIST_SOLDIER_GROW_SKILL
    
    CCDictionary *mainTaskGroupDic; //主线任务的表 PLIST_MAINTASK
    
    CCDictionary *mainTaskSubDic; //子任务表 PLIST_MAINTASKQUEST
    
    CCDictionary* _lootWiKiDict; //来源百科dict PLIST_LOOTWIKI
    
    CCDictionary * _materialLootWikiDict ;// 材料来源百科 PLIST_EQUIP_LOOTWIKI
    
    CCDictionary* _nationListDict; //国家介绍信息dict PLIST_NATIONLIST
    
    tinyxml2::XMLDocument* _skillFuzzyXml; // AI战斗XML数据
    
    CCDictionary *materialDict;//装备转生的材料表
    
    CCDictionary *materialDependencyDict;//装备转生所需要的材料的依赖表
    
    CCDictionary * _baseCombatValueDict; //武将基础战力
    
    CCDictionary * _skillCombatValueDict; //武将技能战力加成
    CCDictionary  * specialBattleDict;//特殊副本数据
    
    CCDictionary *spHelpRuleDict;//特殊副本规则表
    
    CCDictionary * _fightOverNoticeDict;//战斗结算页面弹窗提示优化对应的配置数据
    CCDictionary * _fightOverNoticeBtnDict;
    
    std::map< int, std::map<int, int>* >* _soldierGrowPropertyMap; //士兵的属性增量因子表，按等级提升。key为等级level，value为<key:士兵id,value:增量因子int>
    std::map< int , std::map<int , int > >_fragmentNumMap; //碎片召唤需要数量
private:
    //加密文件流解码后的map
    std::map< std::string, std::pair<unsigned int, char*> >* _dataMap;

public:
    SGStaticDataManager();
    
    ~SGStaticDataManager();
    
    //解析数据集合文件。
    void parseBinData();
    
    //加载各种Plist。
    void loadPlist();
    
    //读取plist文件，返回字典数据。
    CCDictionary* readPlist(const char *filepath);
    
    //读取xml文件，返回XMLDocument
    tinyxml2::XMLDocument* readXml(const char* fileName);
    
    //单例类
    static SGStaticDataManager *shareStatic();
    
    //返回AI战斗XML数据
    tinyxml2::XMLDocument* getSkillFuzzyXml();
    
    //通过Id查找武将卡数据
    SGOfficerDataModel* getOfficerById(int officerId);
    //通过被动技等级去找花费和经验
    CCDictionary* getwjjCostBylevel(int level);
    //通过主将计等级去找经验
    CCDictionary* getzjjCostBylevel(int level);
    
    //通过原型Id查找武将卡部分数据
    SGOfficerDataModel* getOffcierByProtoId(int protoId);
    
    //通过Id获得主将计技能数据
    SGSkillDataModel* getLordSkillById(int skillId);
    
    //通过Id获得被动技技能数据
    SGSkillDataModel* getGeneralSkillById(int skillId);
    
    //去掉beanId的 "sk/"
    int getAnyoneIdByBeanId(const char *beanId);
    
    //把字典数据解析成武将卡数据
    SGOfficerDataModel * dictToOfficer(CCDictionary *dict);
    
    //把字典数据解析成技能数据
    SGSkillDataModel * dictToSkill(CCDictionary *dict);
    //把字典数据解析成lua页面数据
    SGLuaDataModel * dictToLua(CCDictionary *dict);

    
    //特殊副本战斗数据
    SGSpecialBattleDataModel *getSpecialBattleDataByPlotId(int plotId);
    SGSpecialBattleDataModel *dictToSpecialBattle(CCDictionary *dict);
    
    std::string getSingleSpBattleRule(int index);
    int getSpHelpInfoKeysNum();
    
    //获取一个随机生成的名字
    std::string getRandomNickName();
    
    //通过id得到一个装备卡
    SGEquipmentDataModel *getEquipById(int equipId);
    
    //通过原型Id查找装备卡部分数据
    SGEquipmentDataModel* getEquipByProtoId(int protoId);
    
    //把字典数据解析成装备数据
    SGEquipmentDataModel *dictToEquipment(CCDictionary* dict);
    
    //通过Id获取士兵卡
    SGSoldierDataModel *getSoliderById(int soldierId);
    
    //把字典数据解析成士兵数据
    SGSoldierDataModel *dictToSoldier(CCDictionary *dict);
    
    //通过FileId获取士兵时间特效        //added by cgp
    SGSbLocalDataModel *getSbLocalByFileId(int fileId);
    
    //把字典数据解析成士兵时间特效数据    //added by cgp
    SGSbLocalDataModel *dictToSbLocal(CCDictionary *dict);
    
    
    //通过randomId获取随机人物        //added by cgp
    SGLoadingOfficerDataModel *getLoadingOfficerByRandomId(int randomId);
    
    //把字典数据解析成随机人物数据    //added by cgp
    SGLoadingOfficerDataModel *dictToLoadingOfficer(CCDictionary *dict);
    
    
    //通过Id获取道具
    SGPropsDataModel *getPropsById(int propsId);
    
    //把字典数据解析成道具
    SGPropsDataModel *dictToProps(CCDictionary *dict);
    
    //军衔说明读取
    SGJunbuff *getbuffbyid(int buffid);
    
    //进度条说明读取
    SGJunbuff *getjindutiaobyid(int buffid);
    
    //通过士兵技能号读取技能内容
    SGSoldierSkillDataModel* getSoldierSkillById(int skillId);
    
    //把字典数据解析成士兵技能数据
    SGSoldierSkillDataModel * dictToSoldierSkill(CCDictionary *dict);
    
    //根据士兵ID和LV获得其属性增量因子。
    int getSoldierGrowProperty(int sid, int slevel);

    
    //获取type的具体含义，返回描述文字
    CCString *getCardType(int type);
    
    //通过关卡Id获取所有的对话
    CCArray *getArrayById(int plotId);
    
    //通过关卡Id获取战场对话
    CCString *getPKWordById(int plotId);
    
    //把dict解析成对话的数组
    CCArray *dictToBattleword(CCDictionary *dict);
    
    //通过关卡Id获取关卡数据或活动数据
    SGSpotDataModel *getSpotById(int spotId, bool spot = true);

    //获取所有关卡数据
    CCArray* getAllStoryDatas();
    
    //获取所有活动数据
    CCArray* getAllActivityDatas();
    
    //通过引导ID获取引导Tag
    int getGuideTagById(int gid);
    
    //通过士兵等级和士兵类型找到成长技的id
    int getSoldierGrowSkillid(int level,int soldierid);
    
	//精英副本
	CCArray *getAllCreamDatas();
    
    //通过引导ID获取引导数据模型
    SGGuideVO *getGuideModel(int gid);
    
    //把dict解析成引导的数据模型
    SGGuideVO *dictToGuide(CCDictionary *dict,int gid);
    
    
    
    SGFightGuideVO *getFightGuideModel(int story, int scene, int gid);
    SGFightGuideVO *getAutoGuideModel(int gid);
    SGFightGuideVO *dictToFightGuide(CCDictionary *dict);
    
    SGRobotGuideVO *getRobotGuideModel(int scene,int story,int step);
    SGRobotGuideVO *getCgRobotGuideModel(int step);
    SGRobotGuideVO *dictToRobotGuide(CCDictionary *dict);
    //通过关卡ID获取关卡数据,修正版 或活动数据
    SGStoryBean* getStoryDataById(int storyId, bool spot = true);
    
    //通过场景主ID获取场景所在关卡 或活动
    SGStoryBean* getStoryDataByPlotId(int plotId, bool spot = true);
    
    //把dict解析成 SGSpotDataModel
    SGSpotDataModel *dictToSpot(CCDictionary *dict);
    
    SGJunbuff *dictToBuff(CCDictionary *dict,int type);
    
    SGRewardDataModel* getRewardById(int rewardId, int rewardType =1);
    
    SGRewardDataModel* dictToReward(cocos2d::CCDictionary *dict);
    //通过Id获取套装数据
    SGSuitDataModel *getSuitById(int suitId);
    
    //把dict解析成套装对象
    SGSuitDataModel *dictToSuit(CCDictionary *dict);
    
    //通过id获取vip数据
    SGVipDataModel *getVipById(int vipId);
    
    //把dict解析成vip对象
    SGVipDataModel *dictToVip(CCDictionary *dict);
    
    //通过等级获取玩家数据对象
    SGPlayerDataModel *getPlayerByLevel(int playerLevel);
    
    //把dict解析成玩家角色数据对象
    SGPlayerDataModel* dictToPlayer(CCDictionary *dict);
    
    //通过经验id获取经验对象
    SGExpDataModel *getExpById(const char *expBeanId);
    
    //通过经验类型和id获取经验对象
    SGExpDataModel *getExpByTypeAndId(int expType,int expId);
    
    //通过ID获得任务对象
    SGDayTaskDataModel *getTaskById(int taskId);    
    SGDayTaskDataModel *dictToTasks(CCDictionary *dict);
    
    SGLimitGuideVO* dictToLimitGuide(CCDictionary *dict,int gid);
    SGLimitGuideVO* getLimitGuideModel(int limitId, int gid);
    int getimitTagById(int limitId, int gid);
    
    //获得小提示
    CCArray *gettips();
    
    //获得帮助手册标题
    CCArray *gethelptitle();
    
    //获得帮助信息
    CCArray *gethelpinfo(int begin,int end);

    //把dict解析成经验数据对象
    SGExpDataModel *dictToExp(CCDictionary *dict);
    
    //获得国家信息dict
    CCDictionary* getNationListPlist();
    
    CCDictionary *getOfficerDict();
    CCDictionary *getSoldierDict();
    CCDictionary *getPropsDict();
    CCDictionary *getEquipmentDict();
    
    CCDictionary *getWeiOfficerDict();
    CCDictionary *getShuOfficerDict();
    CCDictionary *getWuOfficerDict();
    CCDictionary *getQunOfficerDict();
    CCDictionary *getEquipViewDict();
    CCDictionary *getDisplayValueDict();
    //通过武将id获取所属国家
    int getCountryIdByOfficerId(int officerId);
	
    static const char* guide_Id[100];
    
    static const char* limit_Id[100];
    
    //解析武将挂载缘分字符串，逗号分隔。形如 1001,2002
    std::vector<int>* parseFateString(CCString* cs);
    
    //获得缘分本体数据原始字典
    CCDictionary* getFateBaseData();
    
    //获得缘分效果数据原始字典
    CCDictionary* getFateEffectData();
    
    CCDictionary* getRewardData();
    //生成原型ID级主将卡数据
    void createProtoOfficerData();
    
    //生成原型ID级装备卡数据
    void createProtoEquipmentData();
    
    //生成缘分相关数据
    void createFateData();
    
    //生成士兵属性增量因子表
    void createSoldierGrowProperty();
    
    //根据类型获得物品的原始数据，目前有将领=0/装备=1/士兵=2/道具=3/武将卡碎片=4
    CCObject* getItemByTypeAndId(int type, int id);
    
    //根据类型获得物品的名称，目前有将领=0/装备=1/士兵=2/道具=3/武将卡碎片=4
//    CCString* getItemNameByTypeAndId(int type, int id);
    SeRewardTypeId* getItemNameByTypeAndId(int type, int id);
	
	//根据碎片ID获取碎片dict
	SGPiecesDataModel *getPiecesDictById(int piecesId);
    //根据武将卡protoID获取对应碎片dict
	SGPiecesDataModel *getPiecesDictByOfficerProtoId(int officerProtoId);
	//根据碎片dict获取碎片数据
	SGPiecesDataModel *dictToPieces(CCDictionary *piecesDict);
    //持有所有pieceDataModel
    void createAllPieceDataModel();
    CCArray * m_allPieceDataModel;
	
	//解析消耗品数据
	SGConsumableDataModel *dictToConsumable(CCDictionary *dict);
	
	//根据consumeId来取到对应的数据
	SGConsumableDataModel *getConsumableById(int id);
    //军衔内容个数
    int getJunxianSize();
    
    //根据材料id获取材料dataModel
    SGMaterialDataModel *getMaterialById(int materialId);
    //根据字典转换为datamodel
    SGMaterialDataModel *dictToMaterial(CCDictionary *dict);

    //根据id装备转生所需要的材料组的
    SGMaterialDependencyDataModel *getMaterialDependencyById(int materialDependencyId);
    //读取装备进阶依赖材料的表 字典转dataModel
    SGMaterialDependencyDataModel *dictToMaterialDependency(CCDictionary *dict);
 
    CCArray *getMainGroupTask();
    SGMainTaskGroup * dicToMainTaskGroup(CCDictionary *dic);
    SGMainTaskSubModal *getMainTaskSubByTaskId(int taskId);
    SGMainTaskSubModal *dicToMainSubModal(CCDictionary *dic);
    
    SGVisitViewVO* dictToVisitView(CCDictionary *dict);

    SGVisitViewVO* getVisitViewModel(int viewId);
    
    SGOfficerCard *officerDataModeToOfficer(int itemId);
    SGEquipCard *equipDataModeToequip(int itemId);
    SGPropsCard *propDataModelToProp(int itemId);
    SGPiecesCard *piecesDataModelToPiece(int itemId);
    SGConsumableCard  *consumableDataModelToConsumable(int itemId);
    SGMaterialCard *materialDataModelToMaterial(int itemId);

    //根据itemType和itemId返回一个基类卡牌对象，主要用于CIB，附带itemNum参数。
    SGBaseMilitaryCard* getOneBmcByTypeAndId(int itemType, int itemId, int itemNum = 0);
    
    //图鉴来源
    inline CCDictionary * getLootWiKiDict(){    return _lootWiKiDict ;}
    inline CCDictionary * getMaterialLootWiKiDict() {  return  _materialLootWikiDict ;}

    
    int getStrengOfficerType( int officerId);
    int getStrengEquipType(int equipId);
    int getStrengSoldierType( int soldierId);
    int getStrengCardType(CCString *cardClassName ,int cardId);
    
    //根据polotId 返回数据  精英闯关次数使用。
    SGSpotDataModel * getCreamByPlotId(int plotID);
    
    //解析 fragmentNum.plist 
    void createFragmentNum();
    
    //根据武将卡的星级 和小转生次数确定需要转生的碎片数量 失败返回-1；
    int getAdvOfficerNeedPieceNum(int starLev , int advNum);
    
    
    //----武将战斗力计算
    CCArray * m_allBaseCombatValueDataModel;
    CCArray * m_allSkillCombatValueDataModel;
    void createCombatValueDataModel();
    SGBaseCombatValueDataModel * dictToBaseCombatValue(CCDictionary *dict);
    SGSkillCombatValueDataModel * dictToSkillCombatValue(CCDictionary *dict);
    
    /*参数 lev=武将或装备自身等级    type=6 武将    type=7装备    starlev星级品质    advnum小转生次数
     返回武将（不包含技能战力）或装备战力
     */
    int getBaseCombatValue(int type, int lev ,int starLev , int advNum );
    /* 主将技能 和被动技能    grid所占格子      lordId 主将技id     passivitySkillId被动技 id
     返回主将技 和 被动技能 战力和
    */
    int getSkillCombatValue(int grid  , int lordId , int passivitySkillId);
    int getSkillCombatAssistFunc(int type , int grid , int lev , int skillWeight);
    //战斗力计算end.
    
    CCDictionary * getFightOverNoticeDict(){return _fightOverNoticeDict;}
    CCDictionary * getFightOverNoticeBtnDict(){return _fightOverNoticeBtnDict;}
    
    CCDictionary * getNickNameDict(){return nickNameDict;}
};

#endif /* defined(__GameSanGuo__SGStaticDataManager__) */
