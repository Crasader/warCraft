//
//  SGBaseTags.h
//  GameSanGuo
//
//  Created by Fygo Woo on 8/14/14.
//
//

#ifndef GameSanGuo_SGBaseTags_h
#define GameSanGuo_SGBaseTags_h

//————————————在这个加layerTag 的时候。别忘了在ResourceManager.cpp中数组的相同位置添加呀！！！！！！！---------------
//Layers cgp
//所有Layer(UI)的唯一Tag。
enum LayerTag
{
    sg_battleRevivelayer = 0,
    sg_LayerNull,
    sg_chatinputbox,
    sg_playerInfoLayer,
    sg_conloadingLayer,
    sg_introLayer,
    sg_countryLayer,
    sg_cgMovieLayer,
    sg_countryInfoLayer,
    sg_countrysInfo,
    sg_welComeLayer,//登陆
    sg_registerLayer,//注册
    sg_changepassword, //修改密码
    sg_modifypassword,
    sg_serverListLayer,//服务器列表
    sg_firstLayer,
    sg_mainLayer,
    sg_loadLayer,
    sg_battleMatchLayer,
    sg_battleLayer,
    sg_battleFieldLayer,//hkfn
    sg_battleMainLayer, //天梯赛界面
    sg_battlePreLayer,  //战前场景Loading
    sg_batttleLoadingLayer,  //added by cgp
    sg_sliceEffectLayer,    //切图层
    
    
    sg_barrackslayer,
    sg_showAllLayer,
    sg_taskLayer,//当日任务
    sg_liandanLayer,//炼丹炉界面
    sg_jitianLayer,//祭天界面
    sg_drinkWineLayer,//喝酒界面
    sg_getsalaryLayer,//领工资界面
    sg_inviterLayer,
    sg_inviteeLayer,
    sg_yuekaRewardLayer,
    sg_regLotteryLayer,//拜财神
    sg_bindLayer,//绑定帐号
    sg_levelUpGetGold,//升级领元宝
    sg_rewardsLayer,//领奖界面
    sg_landingRewardLayer,//九宫格
    sg_embattleLayer,//布阵
    sg_generalsLayer,//武将列表界面 除背包入口
    sg_generalInfoLayer,//武将详细信息界面
    sg_generalInfoScrollLayer,//武将详情信息滑动界面
    sg_equipsLayer,//装备列表界面
    sg_soldiersLayer,//士兵列表界面
    sg_skillCardsLayer,//被动技素材卡界面
    sg_strengsoldier,
    sg_chooseSkillCard,
    sg_skillcardInfoLayer,
    sg_equipInfoLayer,
    sg_soldierInfoLayer,
    sg_propInfoLayer,
    sg_propsLayer,//道具列表界面
    sg_stuffLayer,
    sg_equipstrengLayer,
    sg_strengLayer,
    sg_loginLayer,
    sg_listLoginLayer,
    sg_advanceLayer,
    sg_advancetexiao,
    sg_shopLayer,
    sg_vipGiftLayer,
    sg_storeLayer,
    sg_exchangCoin,
    sg_storyLayer,      // 故事模式列表
    sg_storySectionLayer, // 故事模式-场景详细列表
    sg_storyHelpLayer,  // 寻求好友帮忙界面
    sg_activityLayer,   // 每日活动界面
    sg_activityDetail,  // 详细活动界面
    sg_fhelpSortLayer,  // 寻求好友-排序界面
    sg_pvpMainLayer,    // 讨伐战界面
    sg_rankLayer,       // 排行榜界面
    sg_creditCardLayer, // 积分兑换界面
    sg_uiguideLayer,    // UI教学
    sg_limitGuideLayer,    // 功能教学
    sg_storyspeakLayer, // 故事章节对话
    sg_blackLayer,
    sg_expandLayer,
    sg_arrangeLayer,
    sg_soulCardLayer,
    sg_helpLayer,
    sg_helpBook,
    sg_visitLayer,
    sg_tongQueLayer,
    sg_allCardsLayer,
    sg_cardsLayer,
    sg_vipInfoLayer,
    sg_sortLayer,          //排序界面
    sg_friendLayer,         //好友主界面
    sg_myFriend,            //我的好友
    sg_obtainTili,          //领取体力
    sg_friendRequest,       //好友请求
    sg_searchFriend,        //搜索好友
    sg_expandFriend,        //扩充好友位置
    sg_inviteFriend,        //邀请好友
    sg_blackFriend,         //黑名单界面
    sg_searchFriendFinish,  //搜索好友界面
    sg_chatLayer,           //聊天界面
    sg_setLayer,
    sg_inviteNum,
    sg_test,
    sg_helpinfolayer,
    sg_updateLayer,         //更新界面
    sg_mailLayer,           //邮件界面
    sg_friendMail,          //好友邮件
    sg_systemMail,          //系统邮件
    sg_GMMail,              //GM邮件
    sg_systemMailContent,   //官方邮件内容
    sg_friendMailContent,   //好友邮件内容
    sg_visitShowCard,
    sg_announcementLayer,   //公告界面
    sg_battleSpeedLayer,    //战斗计算速度
    sg_fightrewardLayer,    //战斗结算界面
    sg_levelUpLayer,         //角色升级界面
    sg_announcemsg,
	sg_creamLayer,			//精英副本界面
	sg_autoguideLayer,
	sg_fightguideLayer,
	sg_changeroundLayer,
	sg_fillupLayer,
	sg_messageLayer,
    sg_totalLoginReward, //累计登陆界面
    sg_herolayer, //战斗界面主体，新增
    sg_shoujingzhou,        //守荆州
    sg_sjzrank,             //守荆州排名
    sg_soulExchangeLayer, //军魂换礼UI
    sg_reward_subLayer,
    sg_rewarPropBox,
    sg_rewarCoinBox,
    sg_otherPlayerInfo,
    sg_giftInfoBox,
    sg_glodChangeCoin,
    sg_generalBox,
    sg_mainScene,
	sg_baseBox,
    sg_firstAdvanceBox,
    sg_advancechooselayer,
    sg_otherPlayerInfoBox,
	sg_piecesLayer,//武将碎片
	sg_equipPiecesLayer,//装备碎片
    sg_attackListLayer,
    sg_battleHelpLayer,
    sg_bindSuccessBox,
    sg_bMatchAlertBox,
    sg_bMatchEndBox,
    sg_bMatchStartBox,
    sg_cantVisitBox,// 探访代币不足弹出框。
    sg_yangXinBox,
    sg_chatInputBox,
    sg_coinRewardBox,
    sg_creditCardBox,
    sg_creditCardCell,
    sg_embattleBox,
    sg_emBattleBox,
    sg_emBattleItem,
    sg_exchangeEquipBox,
    sg_fateInfoLayer,
    sg_firstStrengBox,
    sg_generalEquipBox,
    sg_commonInfoBox,
    sg_goldChangeCoin,
    sg_nickNameBox,
    sg_popWord,
    sg_propRewardBox,
    sg_registerBox,
    sg_registerSuccess,
    sg_rewardLdBox,
    sg_shareGameBox,
    sg_rewardSubLayer,
    sg_sellBox,
    sg_serverListNewServerBox,
    sg_skillInfoBox,
    sg_strengBox,
    sg_ttMainLayer,
    sg_battleLeaderSkillBox,
    sg_overBattleBox,
    sg_cgmovelayer,
    sg_complain,///系统投诉 建议 客服留言主页面
    sg_liuyan,//留言界面
    sg_chatwithgm,
    sg_chaatwithgminputbox,////追加发言输入框口
    sg_exchangeCodeLayer,
    sg_findHelplayer,
    sg_jingzhouprocess,
    sg_rank,  //综合排行榜
	sg_batchselllayer,			//批量卖出页面
    sg_junxianlayer,
    sg_pvpeverydaylayer,
	sg_consumablelayer,			//消耗品页面
    sg_seasonlistlayer,
    sg_pvplastlistlayer,
    sg_mainTasklayer,
	sg_comsumableInfoLayer, //消耗品详情页面
    sg_luaactivity,  //活动页面
	sg_bossbattledetailLayer,//boos战详情
	sg_bossbattleranklayer,//boss战的排行榜
	sg_goldshoptodysppricelayer,//元宝商城今日特价页面
	sg_goldshoplimittimelayer,//限制出售
	sg_goldshopnormallayer,//商城普通
	sg_buygoodsbox,//
    sg_bossBattleJoinInLayer,
    sg_bossBattleLayer,
    sg_bossBattleNjoinInlayer,
    sg_piecesinfolayer,
    sg_bossRewardLayer,
    sg_alltasklayer,
	sg_strengEffLayer,//强化效果页面
    sg_mallLayer,//test用。by：zyc  新版商店。
    sg_lastLayer, //因商城外部多处有直接跳转至具体商店接口，返回按钮需要返回原来的layer。添加lastLayer用以记录，从哪里进入商城，以便返回原处。
    sg_lootWikiMainLayer , //图鉴来源
    sg_lootWiKiLayer,
    sg_cardItem,//为了使用ResourceManager
    sg_mailGiftPopBox, //新增独立boxTag
    sg_showHeadIconLayer, //新增独立layer
    //为统一资源管理，boxTag统一纳入LayerTag
    sg_box_null,
    sg_box_alertTag,
    sg_box_registerTag,
    sg_box_loginTag,
    sg_box_strengTag,//强化武将弹框
    sg_box_advanceTag,//强化武将弹框
    sg_box_sortTag,
    sg_box_canVisitTag,
    sg_box_canAdvanceTag,
    sg_box_exchangeEquipTag,
    sg_box_firstAdvanceTag,
    sg_box_noExpStrengTag,
    sg_box_rewardLdTag,
    sg_box_sellBoxTag,
    sg_box_skillInfo,
    sg_box_yangxingTag,
    sg_box_modification,
    sg_box_changeAccount,
    sg_box_sellTag,
    sg_box_equip,
    sg_box_otherPlayerInfo,
    sg_box_btrewardTag,
    sg_box_matchStart,
    sg_box_matchAlert,
    sg_box_matchEnd,
    sg_box_quickfight,
    sg_box_overbattle,
    sg_box_help,
    sg_box_battleGuide,
    sg_box_serverList,
    sg_box_uplevel,
    sg_box_general,
    sg_box_input,
    sg_box_backFight,
    sg_box_fillUp,
    sg_box_rewordCoins,
    sg_box_shareGame,
    sg_box_nikeNameBox,
    sg_box_connTip,
    sg_box_confirm,
    sg_box_batchSell,//批量卖出
    sg_box_consumableSell,// 消耗品卖出
    sg_box_rank,
    sg_box_sellconfirm,//卖出消耗品二次确认
    sg_box_detial,//礼包详情
    sg_box_buygoods,//元宝商城购买商品
    sg_box_shade,
    sg_box_shade2,
    sg_box_cargoDetail, //zyc ..商品详情
    sg_box_lootWikiMain,     //zyc ..图鉴来源
    sg_box_autoEquip,
    sg_box_battleCallFriend,
    sg_box_battleHelp,
    sg_box_battleRoundPrompt,
    sg_box_battleRoundSurrender,   //added by cgp for surrender.
    
    sg_box_changeNickName,
    sg_box_changeOfficer,
    sg_box_consumableSellBox,
    sg_box_consumableSellConfirmBox,
    sg_box_creditCostBox,
    sg_box_embattleBox,
    sg_box_exchangeEquipBox,
    sg_box_expandBox,
    sg_box_forbidChat,
    sg_box_friendCountFull,
    sg_box_generalinfoEffect,
    sg_box_mallGiftPopBox,
    sg_box_noExpStrengBox,
    sg_box_qFightBox,
    sg_box_rankDetailLayer,
    sg_box_soldierinfoEffect,
    sg_box_soldierListBox,
    sg_box_sortBox,
    sg_box_strengBox,
    sg_box_strengLvlUpBox,
    sg_box_tipsBox,
    sg_box_storyRewardBox,
    sg_box_actGiftInfoBox,
    sg_box_giftInfoBox,
    sg_box_pvpGiftInfoBox,
    sg_box_equipBox,
    sg_box_strengEff,
    sg_box_announcementLayer,
    sg_box_battleLeaderSkillBox,
    sg_box_battleReviveLayer,
    sg_box_battleRoundPromptLayer,
    sg_box_bindBox,
    sg_box_bindSuccessBox,
    sg_box_bossTips,
    sg_box_countryInfoLayer,
    sg_box_creamBattleBox,
    sg_box_firstStrengBox,
    sg_box_generalEquip,
    sg_box_soldierList,
    sg_box_strengEffectLayer,
    sg_box_updateClientBox,
    //MM: 针对新手引导新增的layerTag
    sg_fourGuideLayer,
    sg_fightDescLayer,
    sg_pvpFightRewardLayer,//pvp战斗奖励
    sg_generalsBagLayer, //背包-》武将入口
    sg_piecesBagLayer,
    sg_materialLayer,//材料
    sg_equipAdvancedLayer,//装备转生
    sg_advancedEffectLayer,
    sg_monthlySignLayer, //月签奖励界面。
    sg_areanLayer,//竞技场界面
    sg_vsLayer,//点击对战进入的那个界面
    sg_developingOfficerLayer, //武将培养页面
    sg_specialBattleLayer,//特殊副本战斗layer
    sg_specialBattleSecionLayer, //特殊副本进入节列表
    sg_arenaFightRewardLayer,//竞技场战斗结算界面
    sg_fightOverNoticeBox,  //除pvp外，战斗结算页面新弹出框提示 2014-12-25
    sg_adventureLayer, //奇遇界面
    sg_exoticMerchantLayer, //西域商人界面
    
    
    
    
    
    
    //cocostudio layerTag startHero
    cgp_strengAdvLayer,    //强化进阶界面，added by cgp， 从这个界面开始是cocostudio界面
    SG_LAYER_TAG_TOTAL_NUM, //@NOTICE! 所有格新增LayerTag请在这个枚举值之前填写！
};

#define cocostudioLayerStart  cgp_strengAdvLayer
//所有Box的标识Tag。
enum BoxTag
{
    box_null = 1000,
    box_alertTag,
    box_registerTag,
    box_loginTag,
    box_strengTag,//强化武将弹框
    box_advanceTag,//强化武将弹框
    box_sortTag,
    box_canVisitTag,
    box_canAdvanceTag,
    box_exchangeEquipTag,
    box_firstAdvanceTag,
    box_noExpStrengTag,
    box_rewardLdTag,
    box_sellBoxTag,
    box_skillInfo,
    box_yangxingTag,
    box_modification,
    box_changeAccount,
    box_sellTag,
    box_equip,
    box_otherPlayerInfo,
    box_btrewardTag,
    box_matchStart,
    box_matchAlert,
    box_matchEnd,
    box_quickfight,
    box_overbattle,
    box_help,
    box_battleGuide,
    box_serverList,
    box_uplevel,
    box_general,
    box_input,
    box_backFight,
    box_fillUp,
    box_rewordCoins,
    box_shareGame,
    box_nikeNameBox,
    box_connTip,
    box_confirm,
    box_batchSell,//批量卖出
    box_consumableSell,// 消耗品卖出
    box_rank,
    box_sellconfirm,//卖出消耗品二次确认
    box_detial,//礼包详情
    box_buygoods,//元宝商城购买商品
    box_shade,
    box_shade2,
    box_cargoDetail, //zyc ..商品详情
    box_lootWikiMain,     //zyc ..图鉴来源
    box_autoEquip,
    box_battleCallFriend,
    box_battleHelp,
    box_battleRoundPrompt,
    box_changeNickName,
    box_changeOfficer,
    box_consumableSellBox,
    box_consumableSellConfirmBox,
    box_creditCostBox,
    box_embattleBox,
    box_exchangeEquipBox,
    box_expandBox,
    box_forbidChat,
    box_friendCountFull,
    box_generalinfoEffect,
    box_mallGiftPopBox,
    box_noExpStrengBox,
    box_qFightBox,
    box_rankDetailLayer,
    box_soldierinfoEffect,
    box_soldierListBox,
    box_sortBox,
    box_strengBox,
    box_strengLvlUpBox,
    box_tipsBox,
    box_storyRewardBox,
    box_actGiftInfoBox,
    box_giftInfoBox,
    box_pvpGiftInfoBox,
    box_equipBox,
    box_strengEff,
    box_announcementLayer,
    box_battleLeaderSkillBox,
    box_battleReviveLayer,
    box_battleRoundPromptLayer,
    box_battleRoundSurrenderLayer,   //added by cgp
    
    box_bindBox,
    box_bindSuccessBox,
    box_bossTips,
    box_countryInfoLayer,
    box_creamBattleBox,
    box_firstStrengBox,
    box_soldierInfoEffect,
    box_generalEquip,
    box_commonInfoBox,
    box_soldierList,
    box_strengEffectLayer,
    box_updateClientBox,
    box_showRewardBox,
    box_cantVisitBox,
    box_strengMergeBox,
    box_addDevelopingPointBox,//增加武将潜力点的box tag
    box_buySpBattleTimes,
    box_spRuleBox,//特殊副本的规则显示弹窗
    box_fightOverNoticeBox //除pvp外，战斗结算页面新弹出框提示 2014-12-25
};

#endif
