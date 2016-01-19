//
//  SGProtocolToMsg.cpp
//  typhoonwar
//
//  Created by ding huidong on 12-11-2.
//
//

#include "SGProtocolToMsg.h"
#include "DayTaskModule.pb.h"
#include "FightBaseModule.pb.h"
#include "LoginModule.pb.h"
#include "itemmodule.pb.h"
#include "ArmyModule.pb.h"
#include "RoleModule.pb.h"
#include "LotteryModule.pb.h"
#include "ShopModule.pb.h"
#include "RewardModule.pb.h"
#include "MessageModule.pb.h"
#include "PlotModule.pb.h"
#include "PvpFight.pb.h"
#include "ServerMoving.pb.h"
#include "CooperationModule.pb.h"
#include "ActivityModule.pb.h"
#include "ActivityMessage.pb.h"
#include "Invite.pb.h"
#include <google/protobuf/descriptor.h>
#include "AutoEquip.pb.h"
#include "SurvivalModule.pb.h"
#include "MonthlyCardModule.pb.h"
#include "RankModule.pb.h"
#include "LuaModule.pb.h"
#include "YuanBaoShopModule.pb.h"
#include "LuaActivityModule.pb.h"
#include "SpecialBattleModule.pb.h"
#include "XiyuModule.pb.h"

#include "RoleShopModule.pb.h"
#include "Developing.pb.h"
#include "ArenaModule.pb.h"
using namespace google::protobuf;

#define ON_COMMAND(_CMD_, _FUNC_) case (_CMD_): msg = new _FUNC_();break;

#define ON_SEND(_CMD_) SGSocketClient::sharedSocketClient()->send(_CMD_, msg)

google::protobuf::MessageLite* parseMessage(unsigned int cmd, SGPacketHeader* pack)
{
    
    MessageLite* msg = NULL;
    
    switch ( cmd )
    {
			
			ON_COMMAND(MSG_GET_BOSS_BUFF, main::GetBossBuffResponse);//购买Boss战中的buff
			ON_COMMAND(MSG_FLUSH_BOSS_INFO, main::BossCurrStateResponse);//Boss当前的状态，刷新页面用
			ON_COMMAND(MSG_ENTER_BOSS_RANK, main::BossDamageRankResponse);//boss战排行界面
			ON_COMMAND(MSG_ENTER_BOSS, main::EnterBossResponse);//进入boss参战消息
			ON_COMMAND(MSG_BUY_MALL_GOODS, main::GoodsBuyResponse);//购买元宝商城里的商品
			ON_COMMAND(MSG_ENTRY_TODAY_MALL, main::ShopEntryResponse);//进入今日特价
			ON_COMMAND(MSG_USE_CONSUMABLE, main::UseConsumableItemResponse);//使用消耗品
			ON_COMMAND(MSG_BUY_CREAM, main::BossPlotBuyCountResponse);//精英副本购买
			ON_COMMAND(MSG_AUTOBATCHSELL, main::SellRoleItemAllAutoResponse);//自动批量卖出卡牌
			ON_COMMAND(MSG_BATCHSELL, main::SellRoleItemAllResponse);//批量卖出卡牌
			ON_COMMAND(MSG_GUIDE_STEP, main::NewComerRecordResponse);
			//12.3
			ON_COMMAND(MSG_SUMMON_CARD, main::SummonCardResponse);//碎片召唤
			//10.29
			ON_COMMAND(MSG_CREAM_STORY, main::BossPlotListResponse);//关卡
			ON_COMMAND(MSG_CREAM_BATTLE, main::BossStoryListResponse);//章节
			
			
            ON_COMMAND(MSG_CDK, main::CDKEYResponse);//cdk验证
            
            ON_COMMAND(MSG_MAIN_LOGIN, main::LoginResponse);
            ON_COMMAND(MSG_BATTLE_LOGON, main::FightMatrixResponse);
            ON_COMMAND(MSG_BATTLE_MATCH, main::StartResponse);
            ON_COMMAND(MSG_BATTLE_PVESTART, main::PveStarResponse);
            ON_COMMAND(MSG_CGBATTLE_START, main::PlotPrologueStartResponse);
            ON_COMMAND(MSG_BATTLE_MOVE, main::MoveResponse);
            ON_COMMAND(MSG_BATTLE_DELETE, main::RemoveResponse);
            ON_COMMAND(MSG_BATTLE_FILL, main::FillUnitResponse);
            ON_COMMAND(MSG_BATTLE_GIVEUP, main::GiveUpResponse);
            ON_COMMAND(MSG_BATTLE_MAJORSKILL, main::MajorSkillResponse);
            ON_COMMAND(MSG_BARRACKS_STRENG, main::OfficerCompositeResponse);
            ON_COMMAND(MSG_BARRACKS_PROTECT, main::OfficerProtectionResponse);
            ON_COMMAND(MSG_BARRACKS_DISPROTECT, main::OfficerUnProtectionResponse);
            ON_COMMAND(MSG_BARRACKS_ADVANCE, main::OfficerUpgradeResponse);
            ON_COMMAND(MSG_BARRACK_EXPAND, main::ExpansionResponse);
            ON_COMMAND(MSG_EQUIP_STRENG, main::EquipmentCompositeResponse);
            ON_COMMAND(MSG_EQUIP_NEWSTRENG, main::EquipmentStrengthResponse);
            ON_COMMAND(MSG_EQUIP_NEWAUTOSTRENG, main::EquipmentStrengthAutoResponse);
            ON_COMMAND(MSG_EQUIP_ADVANCE, main::EquipmentUpgradeResponse);
            ON_COMMAND(MSG_EQUIP_PROTECT,   main::EquipmentProtectionResponse);
            ON_COMMAND(MSG_EQUIP_DISPROTECT, main::EquipmentUnProtectionResponse);
            ON_COMMAND(MSG_EQUIP_WEAR,   main::WearEquipmentResponse);
            ON_COMMAND(MSG_EQUIP_OFF, main::OffEquipmentResponse);
            ON_COMMAND(MSG_BARRACK_SELL, main::SellRoleItemResponse);//可能还会有用
            ON_COMMAND(MSG_VALIDATION, main::VerifyResponse);
            ON_COMMAND(MSG_EMBATTLE_DISPATCH, main::DispatchOfficerResponse);
            ON_COMMAND(MSG_EMBATTLE_DISPATCH_SOLDIER, main::DispatchSoldierResponse);
            ON_COMMAND(MSG_EMBATTLE_CHANG_OFFICER, main::ChangePositionResponse);
            ON_COMMAND(MSG_EMBATTLE_CHANGE, main::ChangeOfficerResponse);
            ON_COMMAND(MSG_EMBATTLE_CHANGEALL, main::BatchChangeOfficerResponse);
            ON_COMMAND(MSG_SOLDIER_STRENG, main::SoldierCompositeResponse);
            ON_COMMAND(MSG_PROP_DISPROTECT, main::PropsUnProtectionResponse);
            ON_COMMAND(MSG_PROP_PROTECT, main::PropsProtectionResponse);
            ON_COMMAND(MSG_STRENG_SKILL, main::SkillCompositeResponse);
            ON_COMMAND(MSG_STRENG_LORD, main::LordSkillCompositeResponse);
            ON_COMMAND(MSG_EMBATTLE_REMOVE, main::RemoveOfficerResponse);
            ON_COMMAND(MSG_EMBATTLE_SETLORD, main::SetLordResponse);
            ON_COMMAND(MSG_MAIN_CREATEROLE, main::CreateRoleResponse);
            ON_COMMAND(MSG_QiangZhiFillUnitRequest, main::QiangZhiFillUnitResponse);
            //卡牌图鉴
            ON_COMMAND(MSG_ALLCARDS_INFO, main::CardStatesResponse);

            //好友系统
            ON_COMMAND(MSG_FRIEND_ADDFRIEND, main::AddFriendResponse);
            ON_COMMAND(MSG_FRIEND_SEARCHBYNAME, main::SearchFriendByNameResponse);
            ON_COMMAND(MSG_FRIEND_SEARCHBYID, main::SearchFriendByInviteCodeResponse);
            ON_COMMAND(MSG_FRIEND_PLAYERINFO, main::FriendInfoResponse);
            ON_COMMAND(MSG_FRIEND_AGREEFRIEND, main::AgreeFriendResponse);
            ON_COMMAND(MSG_FRIEND_ADDBLACK, main::AddBlockResponse);
            ON_COMMAND(MSG_FRIEND_UNAGREEADDFRIEND, main::UnagreeFriendResponse);
            ON_COMMAND(MSG_FRIEND_DELETEBLACK, main::DeleteBlockResponse);
            ON_COMMAND(MSG_FRIEND_DELETEFRIEND, main::DeleteFriendResponse);
            ON_COMMAND(MSG_FRIEND_EXPANDFRIEND, main::expandFriendResponse);
            ON_COMMAND(MSG_FRIEND_PRESENTPOW, main::PowerGiftResponse);
            
            ON_COMMAND(MSG_FRIEND_EATPWOWERGIFT, main::GetPowerGiftResponse);
            ON_COMMAND(MSG_FRIEND_GETPWOWERGIFT, main::AcceptPowerGiftResponse);
            ON_COMMAND(MSG_FRIEND_EATALLPWOWERGIFT, main::GetAllPowerGiftResponse);
            
            
            ON_COMMAND(MSG_REWARD_INTIVEEGOLD, main::InviteResponse);//被邀请方领奖请求
            ON_COMMAND(MSG_REWARD_INTIVER, main::InviteDetailResponse);//邀请方请求
            ON_COMMAND(MSG_REWARD_INTIVERGOLD, main::InviteRewardResponse);//邀请方领奖请求
     
            //更新玩家信息
            ON_COMMAND(MSG_UPDATE_PLAYERINFO, main::updateRoleResponse);
            //更新队伍信息
            ON_COMMAND(MSG_UPDATETEAMINF, main::TeamInfo);
            //换启用阵容
            ON_COMMAND(MSG_CHANGE_EMBATTLE, main::ChangeCurrentTeamResponse);
            
            //商城
            ON_COMMAND(MSG_STORE_ENTER, main::ShopChargeEntryResponse);
            ON_COMMAND(MSG_VIPGIFT_BAGS, main::ShopVipGiftBagEntryResponse);
            ON_COMMAND(MSG_VIPGIFT_BUY, main::ShopVipGiftBagPurchResponse);
            ON_COMMAND(MSG_STORE_BUY, main::ShopChargeResponse);
            ON_COMMAND(MSG_EXCHANGE_ENTER, main::ShopExchangeCoinEntryResponse);
            ON_COMMAND(MSG_EXCHANGE_COINS, main::ShopExchangeCoinResponse);
            
            //领奖
            ON_COMMAND(MSG_REWARD_LVLUPENTER, main::RewardLevelUpEntryResponse);
            ON_COMMAND(MSG_REWARD_LVLUP, main::RewardLevelUpPurchResponse);
            ON_COMMAND(MSG_REWARD_SALARYENTER, main::RewardSalaryEntryResponse);
            ON_COMMAND(MSG_REWARD_SALARY, main::RewardGetSalaryResponse);
            ON_COMMAND(MSG_REWARD_RECOVERENTER, main::RewardRecoverPowerEntryResponse);
            ON_COMMAND(MSG_REWARD_RECOVER, main::RewardRecoverPowerPurchResponse);
            ON_COMMAND(MSG_REWARD_GODENTER, main::RewardGodPrayEntryResponse);
            ON_COMMAND(MSG_REWARD_GOD, main::RewardGodPrayAddResponse);
            ON_COMMAND(MSG_REWARD_LIANDANENTER, main::RewardLianDanTimeResponse);
            ON_COMMAND(MSG_REWARD_LIANDAN, main::RewardLianDanResponse);
            ON_COMMAND(MSG_REWARD_REGENTER, main::RewardRegLotteryEntryResponse);
            ON_COMMAND(MSG_REWARD_REG, main::RewardRegLotteryGetResponse);
            ON_COMMAND(MSG_REWARD_NOTICE, main::RewardNoticeMessage);
            ON_COMMAND(MSG_REWARD_REGPLAERS, main::RewardRegLotteryPush);
            ON_COMMAND(MSG_REWARD_BIND_INFO, main::RewardBindAccountInfoResponse);
            ON_COMMAND(MSG_REWARD_BIND_MAILBOX_INFO, main::BindMailResponse);
            ON_COMMAND(MSG_REWARD_GET_BIND_MAILBOX, main::RewardBindMailResponse);
            
            ON_COMMAND(MSG_REWARD_BIND, main::RewardBindAccountResponse);
            //聊天系统 
            ON_COMMAND(MSG_CHAT_MESSAGE, main::SendMessageResponse);
            ON_COMMAND(MSG_CHAT_GETDATA, main::ShowMessageResponse);
            //ON_COMMAND(MSG_CHAT_BUYSPEAKER, main::BuySpeakerResponse);
            ON_COMMAND(MSG_CHAT_SHOWITEM, main::ViewSpectacularItemResponse);
            
            //探访
            ON_COMMAND(MSG_VISIT_ENTER, main::LotteryEntryResponse);
            ON_COMMAND(MSG_VISIT_PURCH, main::LotteryPurchResponse);
            //活动探访
            ON_COMMAND(MSG_VISIT_HUODONG, main::ActivityLotteryResponse);
             ON_COMMAND(MSG_VISIT_HUODONGDATAS, main::ActivityLotteryEntryResponse);
            
            //九宫格
            ON_COMMAND(MSG_VISIT_9LOTTERY, main::Lottery9PurchResponse);
            //当日任务
            ON_COMMAND(MSG_DAYTASK_LIST, main::DayTaskListResponse);
            ON_COMMAND(MSG_DAYTASK_STATUS, main::DayTaskGetRewardResponse);
            //任务完成
 
            ON_COMMAND(MSG_DAYTASK_FINISH, main::DayTaskListResponse);
 
            //ON_COMMAND(MSG_DAYTASK_FINISH, main::DayTaskFinishResponse);
 
            //邮件系统
            ON_COMMAND(MSG_MAIL_SENDMESSAGE, main::SendMailResponse);
            ON_COMMAND(MSG_MAIL_DELETEMESSAGE, main::DeleteMailResponse);
            ON_COMMAND(MSG_MAIL_READMAIL, main::ShowMailResponse);
            ON_COMMAND(MSG_MAIL_DELETESYSTEMMAIL, main::DeleteSystemMailResponse);
            ON_COMMAND(MSG_MAIL_GETACCOSSORY, main::ReceiveAttachmentResponse);
            ON_COMMAND(MSG_MAIL_READSYSTEMMAIL, main::ShowSystemMailResponse);
            ON_COMMAND(MSG_MAIL_REFRESHSYSTEMMAIL, main::UpdateRoleMailResponse)
            
            //故事系统
            ON_COMMAND(MSG_STORY_SCENE_ENTER, main::PlotListResponse);
            ON_COMMAND(MSG_STORY_MAIN_ENTER, main::StoryListResponse);
            ON_COMMAND(MSG_STORY_BATTLE_OVER, main::FightRewardResponse);
            ON_COMMAND(MSG_STORY_BATTLE_QEND, main::PlotQuitEndResponse);
            ON_COMMAND(MSG_STORY_ACT_LIST, main::ActivityStoryListResponse);
            ON_COMMAND(MSG_STORY_ACT_SCENE, main::ActivityListResponse);
            
            ON_COMMAND(MSG_STORY_FINDHELPER, main::CooperationResponse);
            //恢复军粮和体力
            ON_COMMAND(MSG_PVE_FULLPOWER, main::ShopPurchPvEPowerResponse);
            ON_COMMAND(MSG_PVP_FULLPOWER, main::ShopPurchPvPPowerResponse);
            ON_COMMAND(MSG_PURCHPVEPOWE_RENTER, main::ShopPurchPvEPowerEntryResponse);
            ON_COMMAND(MSG_UPDATEPOWER, main::updateRolePowerResponse);
            
            //讨伐系统
            ON_COMMAND(MSG_PVPENTER_SEASONWAR, main::PvpEntryResponse);
            ON_COMMAND(MSG_SEASONWAR_RANK, main::PvpTopResponse);
            ON_COMMAND(MSG_SEASONWAR_APPLY, main::PvpApplyResponse);
            ON_COMMAND(MSG_SEASONWAR_SAPPLY, main::PvpExitApplyResponse);
            ON_COMMAND(MSG_SEASONWAR_APPLYS, main::CheckReadyResponse);
            ON_COMMAND(MSG_SEASONWAR_READYS, main::PvpGetReadyResponse);
            ON_COMMAND(MSG_SEASONWAR_MATCHS, main::PvpMatchingResponse);
            ON_COMMAND(MSG_SEASONWAR_CDLIST, main::PvpShopListResponse);
            ON_COMMAND(MSG_SEASONWAR_CDCHG, main::PvpShopPurchResponse);
            ON_COMMAND(MSG_FightFixMapRequest, main::FightFixMapResponse);
            ON_COMMAND(MSG_RUNAWAY, main::FightRunAwayResponse);
            ON_COMMAND(MSG_FRIEND, main::CooperationFillResponse);
            ON_COMMAND(MSG_BATTLE_REVIVE, main::FightReviveResponse);
            ON_COMMAND(MSG_FillUnitRevive, main::FillUnitReviveResponse);
            // pvp fighting //剧情loading
            ON_COMMAND(MSG_PVPFIGHT_LOGIN, main::StartResponse);
            //全服动态
            ON_COMMAND(MSG_DYNAMIC_MOVINGMESSAGE, main::ServerMovingResponse);
            
            //公告
            ON_COMMAND(MSG_ANNOUCEMENT_MESSAGE, main::AnnouncementResponse);
            //停服通知
            ON_COMMAND(MSG_STOP_SERVICE, main::ServerNoticeResponse);
            //分享奖励
            ON_COMMAND(MSG_SHARE_REWARD, main::ShareRewardResponse);

            //教学
            ON_COMMAND(MSG_GUIDE_ASK, main::BeginnersGuideResponse);
            
            // 剧情loading
            ON_COMMAND(MSG_PVE_LOADING, main::StartStatePVEResponse);
            ON_COMMAND(MSG_PVP_LOADING, main::StartStatePVPResponse);
            ON_COMMAND(MSG_GUIDE_FOUR, main::NewHandProssResponse);
            ON_COMMAND(MSG_ANNOUNCEMSG, main::ActivityMessageInfosResponse);
            ON_COMMAND(MSG_UPDATEBAGINF, main::UpdateBagInfo);
 
            ON_COMMAND(MSG_GETREWARD_LOGIN, main::RewardLoginAwardResponse);
            ON_COMMAND(MSG_REWARDLIST_LOGIN, main::LoginAwardListResponse);
            ON_COMMAND(MSG_GETCOINREWARD_LOGIN, main::RewardLoginAwardGoldResponse);
            ON_COMMAND(MSG_AUTO_EQUIP, main::AutoEquipResponse);
            ON_COMMAND(MSG_HEARTBEAD, main::HeartBeatResponse);
            
            //守荆州相关
            ON_COMMAND(MSG_SURVIVAL_MAIN_INFO, main::SurvivalMainInfoResponse);
            ON_COMMAND(MSG_SURVIVAL_CHALLENGE, main::SurvivalChallengeResponse);
            ON_COMMAND(MSG_SURVIVAL_START, main::SurvivalStartResponse);
            ON_COMMAND(MSG_SURVIVAL_RANK_LIST, main::SurvivalRankListResponse);
            ON_COMMAND(MSG_SURVIVAL_FINAL_STAT, main::SurvivalFinalStatResponse); //守荆州完结之后的战绩通告，告知本次战斗的伤害和军魂收益
            ON_COMMAND(MSG_SURVIVAL_DAILY_REWARD, main::SurvivalDailyRewardResponse); //守荆州每日奖励结算通告，告知当时排名和获得元宝。

            //军魂相关
//            ON_COMMAND(MSG_JUNHUN_INFO, main::JunhunInfoResponse); //被商城替代
            ON_COMMAND(MSG_JUNHUN_INJECT, main::JunhunInjectResponse);
            ON_COMMAND(MSG_JUNHUN_REWARD, main::JunhunRewardResponse);
            ON_COMMAND(MSG_JUNHUN_WITHDRAW, main::JunhunWithdrawResponse); //守荆州军魂取回

            ON_COMMAND(MSG_STORY_XINREWARD, main::StoryRewardResponse);
            ON_COMMAND(MSG_PURCHPVPPOWE_RENTER, main::ShopPurchPvPPowerEntryResponse);
 
            ON_COMMAND(MSG_EXCHANGECODE_REARD, main::ExchangeCodeInfoResponse);
            ON_COMMAND(MSG_EXCHANGECODE_INFO, main::ExchangeCodeEntryResponse);
            ON_COMMAND(MSG_REWARD_YUEKA, main::MonthlyCardShowResponse);//月卡奖励请求
            ON_COMMAND(MSG_REWARD_YUEKAGETGOLD, main::MonthlyCardRewardResponse);//月卡奖励领取
            
            /////客服相关
             ON_COMMAND(MSG_SUBQA, main::SubQAResponse); //提交问题
             ON_COMMAND(MSG_DELQA, main::DelQAResponse); //删除QA问题
             ON_COMMAND(MSG_UPDATEQAINFO, main::UpdateQAResponse);//更新QA信息
             ON_COMMAND(MSG_CHECKQA, main::CheckQAResponse);//查看QA
             ON_COMMAND(MSG_APPENDQUESTION, main::AppendQuestionResponse);//查看QA时候客户追加提问
             ON_COMMAND(MSG_RANK, main::RankResponse);//新排行榜
            ON_COMMAND(MSG_LUA_ACTIVITY, main::LuaActivityDetailResponse);//LUA活动请求
            
             ON_COMMAND(MSG_PVP_EVERYDAY, main::PvPDayAwardGiftEntryResponse);//pvp每日奖励
            ON_COMMAND(MSG_PVP_SEASON, main::PvPSeasonAwardGiftEntryResponse);//pvp赛季奖励
            ON_COMMAND(MSG_PVP_EVERYDAY_GIFTGET, main::PvPDayAwardGiftGetResponse);//pvp每日奖励领取
            ON_COMMAND(MSG_PVP_SEASON_GIFTGET, main::PvPSeasonAwardGiftGetResponse);//pvp赛季奖励领取
            ON_COMMAND(MSG_PVP_LAST_LIST, main::PvpOldRankResponse);//pvp上届榜单
            ON_COMMAND(MSG_ACTIVITY_LUA, main::LuaCommonResponse);//lua活动页面通用

            ON_COMMAND(MSG_UPDATE_MAINTASK, main::UpdateRoleQuest);
            ON_COMMAND(MSG_GETMAINTASK_REWARD, main::ReceiveQuestRewardResponse);
            
            
            ON_COMMAND(MSG_FIRSTCHARGEGIFT, main::FirstPayRewardResponse);//显示首充礼包
    
            ON_COMMAND(MSG_LUA_GETGIFT, main::ActivityGetRewardResponse);//活动lua领奖
            ON_COMMAND(MSG_LUA_SET_ELEMENT, main::ActivityOpretionResponse);//活动中lua设置元素的
            ON_COMMAND(MSG_LUA_ACTIVITY_PAGE, main::ActivityDetailResponse);//活动lua单个活动页面

            
            ON_COMMAND(MSG_TONGQUE_REQUEST, main::TongQueLotteryDropResponse);//进入铜雀台界面
            ON_COMMAND(MSG_TONGQUE_DOBK, main::LotteryResponse);//铜雀抽奖
            ON_COMMAND(MSG_TONGQUE_REFRESH, main::TongQueRefreshResponse);//铜雀刷新
            ON_COMMAND(MSG_TONGQUE_YANGXIN, main::TongQueYangXinResponse);//铜雀刷新
            
            ON_COMMAND(MSG_BOSSBATTLE_SHOW, main::AtkBossResponse);//boss战界面;
            ON_COMMAND(MSG_BOSSBATTLE_JOIN, main::MainBossResponse);//进入讨伐魔王界面
            ON_COMMAND(MSG_STARTBOSSTIME, main::BossStartSateResponse) //boss 战是否开启
            ON_COMMAND(MSG_BOSSEND_REWARD, main::BossEndRewardResponse) //boss 战结束
            
            
            //新商城
            ON_COMMAND(MSG_CARGO_LIST, main::ShopCargoListResponse) ;//请求所有商品的响应
            ON_COMMAND(MSG_CARGO_TRY_TO_BUY, main::CargoBuyResponse); //尝试购买商品
            ON_COMMAND(MSG_CARGO_REFRESH, main::ShopCargoFreshResponse) ; //刷新商品列表
            //新商城end
            
            ON_COMMAND(MSG_PVP_WAITE, main::PvPEntryMapRequest) ;
            ON_COMMAND(MSG_PVP_CANCEL_WAITE, main::PvPActionReadyResponse) ;
            
            //pvp 战斗奖励。
            ON_COMMAND(MSG_PVP_FIGHT_REWARD , main::FightPvpRewardResponse);
            //敲醒客户端。
            ON_COMMAND(MSG_ALAN_WAKE , main::AlanWake);
            //武将碎裂
            ON_COMMAND(MSG_OFFICER_HOLY_CRASH , main::OfficerCardItemProto);
            
            //成长计划活动
            ON_COMMAND(MSG_ACTIVITY_BUY_GROW_PLAN , main::ShopGrowPlanResponse);
            
            //月签奖励
            ON_COMMAND(MSG_MONTHLY_SIGN_MAIN_INFO, main::SigninDetailResponse);
            ON_COMMAND(MSG_MONTHLY_SIGN_GET_REWARD, main::SigninRewardResponse)

            ON_COMMAND(MSG_EXO_MECH_MAIN_INFO, main::XiyuInfoResponse);
            ON_COMMAND(MSG_EXO_MECH_BUY_GOODS, main::XiyuBuyResponse);
            
            //一键合成
            ON_COMMAND(MSG_MERGE_ITEM, main::MergeItemResponse);
            
            //特殊战斗章
            ON_COMMAND(MSG_SPECIAL_BATTLE_CHAP, main::SpecialBattleStoryListResponse);
            //特殊战斗节列表
            ON_COMMAND(MSG_SPECIAL_BATTLE_SEC, main::SpecialBattlePlotListResponse);
            //特殊战斗进入战场消息
            ON_COMMAND(MSG_SPECIAL_BATTLE_ENTRY, main::SpecialBattleCheckFightResponse);
            
            //特殊战斗购买次数与购买消息
            ON_COMMAND(MSG_SPECIAL_BATTLE_BUY, main::SpecialBattleBuyCountResponse);
            ON_COMMAND(MSG_SPECIAL_BATTLE_BUY_COUNT, main::SpecialBattleBuyCountEntryResponse);
            

            
            //武将培养
            ON_COMMAND(MSG_OFFICER_DEVELOPING, main::DevelopOfficerDetailResponse);
            ON_COMMAND(MSG_BEGIN_DEVELOPING, main::DevelopOfficerResponse);
            ON_COMMAND(MSG_SAVE_DEVELOPING, main::SaveOfficerResponse);
            ON_COMMAND(MSG_ADD_DEVELOPING_POINT, main::UseDevelopItemResponse);
            
            ///竞技场
            ON_COMMAND(MSG_ARENA_MAIN_INFO, main::ArenaMainInfoResponse);
            ON_COMMAND(MSG_FIGHT_REPORT, main::ArenaFightReportResponse);
            ON_COMMAND(MSG_ARENA_RANK_LIST, main::ArenaRanksResponse);
            ON_COMMAND(MSG_ARENA_START, main::ArenaStartResponse);
            ON_COMMAND(MSG_ARENA_END_GIGHT, main::ArenaEndFightResponse);
            ON_COMMAND(MSG_ARENA_GET_REWARD, main::ArenaGetRewardResponse);
            ON_COMMAND(MSG_ARENA_BUY_FIGHT_COUNT, main::ArenaBuyFightCountResponse);
            
            
            //服务器通用提示消息
            ON_COMMAND(MSG_COMMON_SHOW_MESSAGE, main::ShowCommonMessage);

    }

    
    if ( msg )
    {
//        CCLOG("length:%d",pack->getPackData()->length);
//        CCLOG("size:%ld",sizeof(pack->getPackData()->data));
        msg->ParseFromArray(pack->getPackData()->data, pack->getPackData()->length);
    }
    else
    {
#ifdef DEBUG
        printf("parseMessage:接收到没有的命令 %d", cmd);
#endif
    }
    
    return msg;
}

void sendMatchPlayer()
{
    main::StartRequest *sr = new main::StartRequest();
    sr->set_enemyroleid(10);
    SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_MATCH, sr);
}
