//
//  SGMsgId.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-14.
//
//

#ifndef GameSanGuo_SGMsgId_h
#define GameSanGuo_SGMsgId_h


#define MSG_HTTP_LOGIN 4  //http 登入
#define MSG_HTTP_LOGIN_ERROR 44//登入错误

#define MSG_HTTP_REGISTER 3//注册
#define MSG_HTTP_FAST_LOGIN 16//快速登入
#define MSg_HTTP_SERVERLIST 17//服务列表
#define MSg_HTTP_LASTESTSERVER 18//最后登入的服务器
#define MSg_HTTP_OPENAPP 19 //每次打开游戏app即发送消息，正文发送udit。
#define MSG_HTTP_UPDATE_SERVER_STATE 20 //更新当前选中的服务器消息
#define MSG_HTTP_GET_CHANNEL_USER_ID 21 //特殊需求，从渠道处获得保持一致的user_id

#define MSG_HTTP_BING 8//绑定
#define MSG_HTTP_PASSWORD 5//修改密码


#define MSG_MAIN_CREATEROLE  101            //创建角色
#define MSG_MAIN_LOGIN    102 //登入角色
#define MSG_CDK          1020//cdk验证
#define MSG_BATTLE_OVER   106 //登出
#define MSG_UPDATE_PLAYERINFO 107 //推送角色信息更新
#define MSG_HEARTBEAD 108      //心跳
#define MSG_UPDATEPOWER 109  //推送体力军粮
#define MSG_PVPFIGHT_LOGIN  110 //pvp匹配成功后的登入
#define MSG_GUIDE_ASK   111     //新手教学
#define MSG_VISIT_ENTER 210 //探访武将主界面
#define MSG_VISIT_PURCH 211 //探访武将操s作
#define MSG_VISIT_9LOTTERY 212 //登陆后的九宫格抽奖操作
#define MSG_VISIT_HUODONG 215 //活动探访结果
#define MSG_VISIT_HUODONGDATAS 216 //活动探访数据请求
#define MSG_BOSSBATTLE_SHOW 1128 //boss战进入界面
#define MSG_BOSSBATTLE_JOIN 1130 //讨伐魔王

#define MSG_UPDATE_MAINTASK  213 //更新主线任务
#define MSG_GETMAINTASK_REWARD 214//领取奖励

#define MSG_TONGQUE_REQUEST  217
#define MSG_TONGQUE_DOBK 218
#define MSG_TONGQUE_REFRESH 219
#define MSG_TONGQUE_YANGXIN 220


#define MSG_BATTLE_LOGON  301 // -----------
#define MSG_BATTLE_MATCH  302 //pvp开始战斗
#define MSG_BATTLE_MOVE   303 //战斗移动
#define MSG_BATTLE_DELETE 304//战斗删除
#define MSG_BATTLE_FILL   305//战斗补兵
#define MSG_BATTLE_GIVEUP 306//战斗放弃回合，切换回合
#define MSG_BATTLE_MAJORSKILL 308 //战斗发动主将计
#define MSG_BATTLE_PVESTART 310 // pve 开始战斗

#define MSG_VALIDATION 309 //战斗验证
#define MSG_RUNAWAY         312//战斗逃跑
#define MSG_LOST_CONNECTION 313//掉线通知
#define MSG_BATTLE_REVIVE   314//战场复活
#define MSG_fightLogRequest 315// 客户端给服务器发状态日志，用于调试
#define MSG_FightFixMapRequest 316 //修复地图，不知道什么地方调用。
#define MSG_PveContinuePlayRequest 317//继续pvp
#define MSG_QiangZhiFillUnitRequest 319//给对方补兵
#define MSG_PVPENTER_SEASONWAR 330//pvp入口界面
#define MSG_SEASONWAR_CDLIST   340 // pvp商店列表
#define MSG_SEASONWAR_CDCHG    341 //pvp商店物品购买
#define MSG_SEASONWAR_RANK  331	 //pvp排行
#define MSG_SEASONWAR_APPLY 332 //pvp 匹配报名
#define MSG_SEASONWAR_APPLYS 333    // apply success 没用
#define MSG_SEASONWAR_SAPPLY 334    //  退出报名 不举手
#define MSG_SEASONWAR_READYS 335    // 确认报名  举手
#define MSG_SEASONWAR_MATCHS 336    // 匹配成功 进入战场,返回对玩家的信息

#define MSG_PVP_LOADING     342     // PVP剧情loading
#define MSG_PVE_LOADING     343     // PVE剧情loading
#define MSG_FillUnitRevive  344     //复活
#define MSG_BARRACKS_STRENG 401 // 强化武将
#define MSG_BARRACKS_PROTECT 402// 武将设置保护
#define MSG_BARRACKS_DISPROTECT 403 //武将取消保护
#define MSG_BARRACKS_ADVANCE 404 // 武将提升星级
#define MSG_EQUIP_STRENG 405  // 装备强化
#define MSG_EQUIP_ADVANCE 406 // 装备升星
#define MSG_EQUIP_PROTECT 407 //装备保护
#define MSG_EQUIP_DISPROTECT 408 //装备取消保护

#define MSG_EQUIP_NEWSTRENG 440  // 装备强化
#define MSG_EQUIP_NEWAUTOSTRENG 441  // 装备强化

#define MSG_EQUIP_WEAR 409 //穿装备
#define MSG_EQUIP_OFF 410 //脱装备
#define MSG_BARRACK_SELL 411 //出售物品
#define MSG_SOLDIER_STRENG 412 //士兵卡强化
#define MSG_BARRACK_EXPAND 413 //扩建军营
#define MSG_PROP_PROTECT 414   //道具卡保护
#define MSG_PROP_DISPROTECT 415 //道具卡取消保护
#define MSG_ALLCARDS_INFO 416   //卡牌图鉴列表
#define MSG_STRENG_SKILL 417    //强化被动技
#define MSG_STRENG_LORD  420    //强化主将计

#define MSG_AUTOBATCHSELL 421		//批量自动卖出卡牌
#define MSG_BATCHSELL		422		//批量卖出卡牌
#define MSG_USE_CONSUMABLE	425	//使用消耗品

#define MSG_AUTO_ALLEQUIP 418   //全员自动装备
#define MSG_AUTO_EQUIP   419   //单个武将自动装备

#define MSG_CGBATTLE_START   430   //CG战场进入消息

#define MSG_MERGE_ITEM      442     //一键合成材料


#define MSG_PVP_WAITE 450//PVP等待对方玩家进入
#define MSG_PVP_CANCEL_WAITE 451//取消等待

#define MSG_ACTIVITY_LUA 500   // 活动Lua消息

#define MSG_EMBATTLE_DISPATCH 501   // 上场武将
#define MSG_EMBATTLE_REMOVE 502   // 下场武将
#define MSG_EMBATTLE_SETLORD 503   // 设置主将
#define MSG_EMBATTLE_DISPATCH_SOLDIER 504   //派遣制定颜色小兵
#define MSG_EMBATTLE_CHANG_OFFICER 505    // 更换武将位置
#define MSG_EMBATTLE_CHANGE 506 // 更换武将
#define MSG_EMBATTLE_CHANGEALL 509 //更新全部武将


#define MSG_FRIEND_ADDFRIEND    601   // 加好友
#define MSG_FRIEND_SEARCHBYNAME 602   // 搜好友by name
#define MSG_FRIEND_SEARCHBYID   603   // 搜好友by id
#define MSG_FRIEND_PLAYERINFO   604   // 好友信息
#define MSG_FRIEND_AGREEFRIEND  605   // 同意加好友
#define MSG_FRIEND_ADDBLACK     606   // 加入黑名单
#define MSG_FRIEND_UNAGREEADDFRIEND   607   // 拒绝加好友
#define MSG_FRIEND_DELETEBLACK  608   // 删除黑名单
#define MSG_FRIEND_DELETEFRIEND 609   // 删除好友
#define MSG_FRIEND_EXPANDFRIEND 610   // 扩展好友数量

#define MSG_FRIEND_PRESENTPOW   614   // 赠送好友体力
#define MSG_FRIEND_EATPWOWERGIFT   615   // 领取好友体力
#define MSG_FRIEND_GETPWOWERGIFT   616   // 收到好友体力
#define MSG_FRIEND_EATALLPWOWERGIFT 617  //一键领取所有可领体力


#define MSG_REWARD_INTIVEEGOLD   611       //被邀请方领奖请求
#define MSG_REWARD_INTIVER       612       //邀请方请求
#define MSG_REWARD_INTIVERGOLD   613       //邀请方领奖请求

#define MSG_STORE_ENTER 701	//充值界面
#define MSG_STORE_BUY 702	//充值操作，买元宝
#define MSG_EXCHANGE_ENTER 703	//金子换铜钱界面
#define MSG_EXCHANGE_COINS 704	//金子换铜钱操作
#define MSG_VIPGIFT_BAGS 705	//vip礼包界面

#define MSG_VIPGIFT_BUY 707	//vip礼包购买操作
#define MSG_PURCHPVEPOWE_RENTER 708         //进入购买体力
#define MSG_PURCHPVPPOWE_RENTER 709         //进入购买军粮
#define MSG_PVE_FULLPOWER     710           //pve恢复体力
#define MSG_PVP_FULLPOWER     711           //pvp恢复军粮

#define MSG_REWARD_LVLUPENTER 712      //升级领奖界面
#define MSG_REWARD_LVLUP 713          //升级领奖操作
#define MSG_REWARD_SALARYENTER 714      //领工资界面
#define MSG_REWARD_SALARY 715           //领工资操作
#define MSG_REWARD_RECOVERENTER 716      //喝酒回体力界面
#define MSG_REWARD_RECOVER 717        //喝酒回体力操作
#define MSG_REWARD_GODENTER 718      //祭天界面
#define MSG_REWARD_GOD 719           //祭天操作
#define MSG_REWARD_REGENTER 720      //拜财神界面(注册限时活动)
#define MSG_REWARD_REG 721            //拜财神操作（注册限时活动）
#define MSG_REWARD_REGPLAERS 724      //拜财神玩家滚屏（注册限时活动）
#define MSG_REWARD_LIANDANENTER 722    //炼丹列表
#define MSG_REWARD_LIANDAN 723        //获得炼丹物品
#define MSG_REWARD_BIND 725           //绑定得到奖励
#define MSG_REWARD_BIND_INFO 726      //绑定得到奖励个数
#define MSG_REWARD_BIND_MAILBOX_INFO 727      //绑定邮箱奖励
#define MSG_REWARD_GET_BIND_MAILBOX 728      //领取绑定邮箱奖励
#define MSG_REWARD_NOTICE 730        //更新奖励状态
#define MSG_REWARD_YUEKA 1120        //月卡请求
#define MSG_REWARD_YUEKAGETGOLD 1121        //月卡领奖请求


#define MSG_CHAT_MESSAGE        801         //发送聊天内容
#define MSG_CHAT_GETDATA        802         //获得聊天列表
#define MSG_CHAT_BUYSPEAKER     803         //买喇叭
#define MSG_CHAT_SHOWITEM   799   // 聊天展示物品

//邮件
#define MSG_MAIL_SENDMESSAGE    811         //发送邮件
#define MSG_MAIL_DELETEMESSAGE  812         //删除邮件
#define MSG_MAIL_READMAIL       813         //看邮件
#define MSG_MAIL_READSYSTEMMAIL 814         //查看系统邮件
#define MSG_MAIL_GETACCOSSORY   815         //接收附件
#define MSG_MAIL_DELETESYSTEMMAIL 816       //删除系统邮件
#define MSG_MAIL_REFRESHSYSTEMMAIL 817


#define MSG_STORY_CLIENTBATTLE_OVER   900        //客户端请求战斗奖励

#define MSG_STORY_SCENE_ENTER   901         //故事列表
#define MSG_STORY_BATTLE_OVER   902         //获得战斗奖励
#define MSG_STORY_BATTLE_QEND   903         //故事退出
#define MSG_STORY_MAIN_ENTER    904         //故事主界面
#define MSG_STORY_ACT_LIST      905         //限时活动
#define MSG_STORY_ACT_SCENE     906         //关卡活动
#define MSG_GUIDE_FOUR          907         //四格武将关卡
//10.28george
#define MSG_CREAM_BATTLE  909 //精英副本
#define MSG_CREAM_STORY	908 //精英副本故事ID


#define MSG_STORY_FINDHELPER    911         //援军列表
#define MSG_FRIEND          913             //好友主将上场
#define MSG_STORY_XINREWARD         914             //story星级领奖
#define MSG_BUY_CREAM			916			//购买精英副本挑战次数

//特殊战斗相关消息号
#define MSG_SPECIAL_BATTLE_CHAP        917  //章列表
#define MSG_SPECIAL_BATTLE_SEC         918  //节列表数据
#define MSG_SPECIAL_BATTLE_ENTRY       919  //特殊战斗进入消息
//次数
#define MSG_SPECIAL_BATTLE_BUY_COUNT   920     //特殊战斗购买次数消息
#define MSG_SPECIAL_BATTLE_BUY         899     //特殊战斗购买消息




#define MSG_DAYTASK_LIST 921        //日常任务列表
#define MSG_DAYTASK_STATUS 922      //日常任务列表状态
#define MSG_DYNAMIC_MOVINGMESSAGE 930       //滚动公告
#define MSG_ANNOUCEMENT_MESSAGE   931       //登入公告


#define MSG_REWARDLIST_LOGIN 942 //15天登陆奖励
#define MSG_GETREWARD_LOGIN 941 //领取奖励
#define MSG_GETCOINREWARD_LOGIN 943 //领取铜线

#define MSG_UPDATEBAGINF 940      //日常任务完成

#define MSG_UPDATETEAMINF 945  //更新队伍信息
#define MSG_DAYTASK_FINISH 950      //日常任务完成
#define MSG_ANNOUNCEMSG 998        //公告
#define MSG_FIRSTCHARGEGIFT 970        //公告

#define MSG_CHANGE_EMBATTLE 507  //换启用阵容

#define MSG_SURVIVAL_MAIN_INFO 1001 //守荆州主界面信息
#define MSG_SURVIVAL_CHALLENGE 1002 //试图继续守荆州
#define MSG_SURVIVAL_START 1003 //开始守荆州，告诉服务器我是否跳过
#define MSG_SURVIVAL_RANK_LIST 1004 //守荆州排行榜列表，请求附带页码
#define MSG_SURVIVAL_FINAL_STAT 1005 //守荆州完结之后的战绩通告，告知本次战斗的伤害和军魂收益
#define MSG_SURVIVAL_DAILY_REWARD 1006 //守荆州当日战斗奖励结算，可能随时推送，现在就设置为接收后直接弹BOX。
#define MSG_SURVIVAL_GIVE_UP 1007 //（废弃）守荆州放弃战斗的发送，服务器需要这个消息来结算守荆州

#define MSG_JUNHUN_INFO 1010 //守荆州军魂信息 ////被商城替代
#define MSG_JUNHUN_INJECT 1011 //守荆州军魂注入
#define MSG_JUNHUN_REWARD 1012 //守荆州军魂奖励
#define MSG_JUNHUN_WITHDRAW 1013 //守荆州军魂取回

#define MSG_AUTO_EQUIP 419  // 武将自动装备

#define MSG_SUMMON_CARD	508	//碎片召唤
#define MSG_EXCHANGECODE_REARD 1101 //兑换码信息
#define MSG_EXCHANGECODE_INFO 1102 //兑换码描述


#define MSG_SUBQA     1042  //客服提交QA
#define MSG_DELQA      1043  //客服删除QA 
#define MSG_UPDATEQAINFO 1040 //更新QA信息
#define MSG_CHECKQA      1041  ///查看QA
#define MSG_APPENDQUESTION  1044 //查看QA时候客户追加提问

#define MSG_ENTER_BOSS	1127		//进入boss参战页
#define MSG_ENTER_BOSS_RANK	1131		//进入boss战排行
#define MSG_FLUSH_BOSS_INFO	1129		//服务器主推消息,刷新Boss界面消息
#define MSG_GET_BOSS_BUFF		1132		//购买Boss战中的buff

#define MSG_MONTHLY_SIGN_MAIN_INFO 1200 //月签主界面信息
#define MSG_MONTHLY_SIGN_GET_REWARD 1201 //月签领奖

#define MSG_EXO_MECH_MAIN_INFO 1505 //西域商人总览
#define MSG_EXO_MECH_BUY_GOODS 1506 //西域商人购买商品

#define MSG_GUIDE_STEP	1300		//新手玩家新引导至哪一步，服务器记录日志
#define MSG_RANK    1014            //14.2.13新排行榜
#define MSG_LUA_ACTIVITY	1400		//lua活动请求

#define MSG_PVP_EVERYDAY	1122		//pvp每日奖励
#define MSG_PVP_SEASON	1123		//pvp赛季奖励
#define MSG_PVP_EVERYDAY_GIFTGET	1124		//pvp每日奖励领取
#define MSG_PVP_SEASON_GIFTGET	1125		//pvp赛季奖励领取
#define MSG_PVP_LAST_LIST	1126		//pvp上届榜单

#define MSG_LUA_GETGIFT     1402        //活动中lua领奖
#define MSG_LUA_SET_ELEMENT     1403        //活动中lua设置元素的
#define MSG_LUA_ACTIVITY_PAGE     1404        //活动中lua单个活动页面的req

#define MSG_ENTRY_TODAY_MALL	1150//进入今日特价
#define MSG_BUY_MALL_GOODS	1151//购买对应的商品

#define MSG_STARTBOSSTIME 150 //BOSS 战是否开启
#define MSG_BOSSEND_REWARD 1133 // boss战结束提醒

#define MSG_STOP_SERVICE 932 //停服消息推送

#define MSG_SHARE_REWARD 1310//ShareReward

#define MSG_CARGO_LIST 1500 //新版商城消息，请求所有商品列表
#define MSG_CARGO_TRY_TO_BUY 1501//尝试购买商品
#define MSG_CARGO_REFRESH 1502 //刷新商品列表

#define MSG_PVP_FIGHT_REWARD 1510 //PVP战斗奖励 新、

#define MSG_OFFICER_HOLY_CRASH 1520 //武将碎裂动画，当获得已有武将时候转化为碎片。

#define MSG_ALAN_WAKE 2014 //敲醒客户端，解除Loading等待。

#define MSG_ACTIVITY_BUY_GROW_PLAN 731//成长计划活动


#define MSG_OFFICER_DEVELOPING 1210//武将详情页点击武将培养
#define MSG_BEGIN_DEVELOPING 1211//在武将培养页面 点击培养一次或十次
#define MSG_SAVE_DEVELOPING 1212 //保存培养的结果
#define MSG_CANCEL_DEVELOPING 1213 //放弃培养
#define MSG_ADD_DEVELOPING_POINT 1214//使用潜力酒增加潜力点


#define MSG_ARENA_MAIN_INFO  1600 //进入竞技场的消息
#define MSG_FIGHT_REPORT 1601 //进入竞技场的战斗日志
#define MSG_ARENA_RANK_LIST 1602 //查看竞技场排名
#define MSG_ARENA_START 1603//进入竞技场战斗
#define MSG_ARENA_END_GIGHT 1604 //结束竞技场战斗请求
#define MSG_ARENA_GET_REWARD 1605 //领取竞技场奖励
#define MSG_ARENA_BUY_FIGHT_COUNT 1606 //购买竞技场挑战次数

#define MSG_COMMON_SHOW_MESSAGE 2015 //服务器通用提示消息

#endif
