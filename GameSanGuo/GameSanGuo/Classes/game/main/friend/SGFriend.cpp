//
//  SGFriend.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-27.
//
//

#include "SGFriend.h"
#include "SGFriendSubLayer.h"
#include "SGMainManager.h"
#include "SGExpandFriend.h"
#include "SGMyFriend.h"
#include "SGMainManager.h"
#include "SGFriendRequest.h"
#include "SGSearchFriend.h"
#include "SGInviteFriend.h"
#include "SGExpandFriend.h"
#include "SGBlackFriend.h"
#include "SGPlayerInfo.h"
#include "SGStoryRewardBox.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGObtainTili.h"

SGFriend::SGFriend()
{
    
}

SGFriend::~SGFriend()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_friendLayer);
}

SGFriend* SGFriend::create()
{
    SGFriend *friends = new SGFriend();
    if(friends && friends->init(NULL, sg_friendLayer))
    {
        friends->construct();
        friends->autorelease();
        return friends;
    }
    
    CC_SAFE_DELETE(friends);
    return NULL;
}

void SGFriend::construct()
{
    this->initView();
}

//初始化界面
void SGFriend::initView()
{
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    float b = SGMainManager::shareMain()->getBottomHeight();
	 

    
    //我的好友
//    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("friend_myFriend.png");
    SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(str_FriendCountFull_str11, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//spriteTitle->setColor(ccc3(0xff, 0x95, 0x0c));
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(spriteTitle);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
   
    //tableview
    tableView->setFrame(CCRectMake(0, 56+101, SGLayout::getSkewing(320) , SGLayout::getSkewingY(480)-b-h));
    tableView->setZOrder(0);
    tableView->setPosition(ccpAdd(bottomCenter, ccp(-skewing(160), b)));
    this->tableViewHeight = 110;
    tableView->setDown(-18);
    //tableView -> setPageNEnable(true);
}

SNSTableViewCellItem* SGFriend::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGFriendSubLayer *cell = (SGFriendSubLayer*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if(NULL == cell)
    {
        cell = SGFriendSubLayer::create(indexPath->getRow(),this);
    }
    
    return cell;
}

unsigned int SGFriend::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return SGFriendTypeCount;
}


void SGFriend::itemselect(SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    switch (indexPath->getRow())
    {
        case SGFriendTypeMyFriend:
        {
            SGMyFriend *myFriend = SGMyFriend::create(SGPlayerInfo::sharePlayerInfo()->getFriendList());
            SGMainManager::shareMain()->showLayer(myFriend);
            EFFECT_PLAY(MUSIC_ITEM);
            break;
        }
        case SGFriendTypeSendTiLi:
        {
            SGObtainTili *expand = SGObtainTili::create(SGPlayerInfo::sharePlayerInfo()->getFriendPowerList());
            SGMainManager::shareMain()->showLayer(expand);
            EFFECT_PLAY(MUSIC_ITEM);
            break;
        }
        case SGFriendTypeFriendRequest:
        {
            SGFriendRequest *request = SGFriendRequest::create(SGPlayerInfo::sharePlayerInfo()->getApplyList());
            SGMainManager::shareMain()->showLayer(request);
            EFFECT_PLAY(MUSIC_ITEM);
            break;
        }
        case SGFriendTypeSearchFriend:
        {
            SGSearchFriend *search = SGSearchFriend::create(true);
            SGMainManager::shareMain()->showLayer(search);
            EFFECT_PLAY(MUSIC_ITEM);
            break;
        }
        case SGFriendTypeBlackFriend:
        {
            SGBlackFriend *black = SGBlackFriend::create(SGPlayerInfo::sharePlayerInfo()->getBlackList());
            SGMainManager::shareMain()->showLayer(black);
            EFFECT_PLAY(MUSIC_ITEM);
            break;
        }
            //        case SGFriendTypeInviteFriend:
            //        {
            //            SGInviteFriend *invite = SGInviteFriend::create();
            //            SGMainManager::shareMain()->showLayer(invite);
            //
            //            break;
            //        }
        case SGFriendTypeExpandFriend:
        {
            SGExpandFriend *expand = SGExpandFriend::create();
            SGMainManager::shareMain()->showLayer(expand);
            EFFECT_PLAY(MUSIC_ITEM);
            break;
        }
        default:
            break;
    }

}

void SGFriend::buttonClickBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}

//通过FriendProto 获取 SGGamePlayer 对象
SGGamePlayer* SGFriend::getSGGamePlayer(main::FriendProto _friend)
{
    SGGamePlayer *player = new SGGamePlayer();
    player->setFriendID(_friend.friendid());
    player->setFriendName(CCString::create(_friend.friendname().c_str()));
    player->setImage(_friend.image());
    player->setIsOnline(_friend.isonline());
    player->setLevel(_friend.level());
    player->setRoleID(_friend.roleid());
    player->setTime(_friend.time());
    player->setTypeID(_friend.typeid_());
    player->autorelease();
    return player;
    
}

SGGamePlayerInfo *SGFriend::getSGGamePlayerInfo(main::RoleInfo roleInfo)
{
    SGGamePlayerInfo *info = new SGGamePlayerInfo();
    
    info->setRoleId(roleInfo.roleid());
    info->setName(CCString::create(roleInfo.name()));
    info->setNickName(CCString::create(roleInfo.nickname()));
    info->setSsid(CCString::create(roleInfo.ssid()));
    info->setExp(roleInfo.exp());
    info->setLevel(roleInfo.level());
    info->setCoins(roleInfo.coins());
   // info->setSilver(roleInfo.silver());
    info->setGold(roleInfo.gold());
    info->setGoverning(roleInfo.governing());
    info->setCountry(roleInfo.country());
    info->setImage(roleInfo.image());
    info->setPower(roleInfo.power());
    info->setBagSize(roleInfo.bagsize());
    info->setModifyNickNameCount(roleInfo.modifynicknamecount());

    info->setCombatValue(roleInfo.combatvalue());
    info->setVipLevle(roleInfo.viplevel());
   ////////////////////////////////////////////
    
    info->setpvpRankName(CCString::create(roleInfo.pvprankname()));
    info->setpresTige(roleInfo.prestige());
    info->setenterPVPCount(roleInfo.enterpvpcount());
    info->setpvpWinCount(roleInfo.pvpwincount());
    info->setGoverningLimit(roleInfo.governinglimit());
    info->setSpeed(roleInfo.speed());
    info->setHp(roleInfo.hp());
    info->setpvpTop(roleInfo.pvptop());

    
    
    
    
    
    
    
    
    
    
    
  //////////////////////////////////////////////////
    //info->setMilitlity(roleInfo. )
    CCLog("***************好友的战斗力:%d",roleInfo.combatvalue());
    main::RoleTeamInfo team = roleInfo.roleteaminfo();
    info->setLordid(team.lordofficerid());
    info->setRedSoldierId(team.redsoldierid());
    info->setGreenSoldierId(team.greensoldierid());
    info->setBlueSoldierId(team.bluesoldierid());
    
    
    int kuafupvplength=team.pvpofficercarditeminfo_size();
     CCArray*array=CCArray::create();
    for(int i=0;i<kuafupvplength;i++)
    {
        
        
        
        
       
        pvpInfo*obj=new pvpInfo();
        main::PvpOfficerCardItemInfo  proto=  team.pvpofficercarditeminfo(i);
        obj->setitmeId(proto.itmeid());
        obj->setlevel(proto.level());
        info->getePVPOfficerArray()->addObject(obj);
        

    }
    
    
    
    
    
    
    int soldierlength=team.soldiercarditemproto_size();
    
    for(int i=0;i<soldierlength;i++)
    {
    
    
        main::SoldierCardItemProto card=team.soldiercarditemproto(i);
        
        
        if(team.redsoldierid()==card.itemid())
        {
        
        SGBaseMilitaryCard*basecard=NULL;
        basecard=createEmbattleSoldierCardWithInfo(card);
        
        info->getembattlesoldierArray()->addObject(basecard);
        }
    
    
    }
    
    for(int i=0;i<soldierlength;i++)
    {
        
        
        main::SoldierCardItemProto card=team.soldiercarditemproto(i);
        
        
        if(team.greensoldierid()==card.itemid())
        {
            
            SGBaseMilitaryCard*basecard=NULL;
            basecard=createEmbattleSoldierCardWithInfo(card);
            
            info->getembattlesoldierArray()->addObject(basecard);
        }
        
        
    }
    
    
    for(int i=0;i<soldierlength;i++)
    {
        
        
        main::SoldierCardItemProto card=team.soldiercarditemproto(i);
        
        
        if(team.bluesoldierid()==card.itemid())
        {
            
            SGBaseMilitaryCard*basecard=NULL;
            basecard=createEmbattleSoldierCardWithInfo(card);
            
            info->getembattlesoldierArray()->addObject(basecard);
        }
        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
 ///////////////////////////////////////////////////////
    int leng = team.teamofficercarditeminfo_size();
    for (int i = 0;i < leng; i++)
    {
//        SGBaseMilitaryCard *basecard = NULL;
        main::TeamOfficerCardItemInfo card = team.teamofficercarditeminfo(i);
        
        SGBaseMilitaryCard *basecard = NULL;
        main::OfficerCardItemProto proto = card.baseofficercardinfoproto();
        basecard = createOfficerCardWithInfo(proto);
        info->getOfficerCardItemArray()->addObject(basecard);
        int eleng = card.baseequipmentcarditemproto_size();
        for (int j = 0;j<eleng; j++)
        {
            main::EquipmentCardItemProto cardInfo = card.baseequipmentcarditemproto(j);
            SGEquipCard *equipCard = new SGEquipCard();
            equipCard->autorelease();
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type =  SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
            
//            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1 > 99 ? 100 : proto.currlevel() + 1);
            
            equipCard->setSsid(cardInfo.ssid());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(cardInfo.itemid());
            //            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(cardInfo.currlevel());
            //equipCard->setCurrExp(cardInfo.currexp());
            equipCard->setOfficerCardId(cardInfo.officercarditemid());
            equipCard->setIsProt(cardInfo.isprotection());
            if (cardInfo.officercarditemid() >0)
            {
                int arrayCount = info->getOfficerCardItemArray()->count();
                for(int i=0;i<arrayCount;i++)
                {
                    SGOfficerCard *card = (SGOfficerCard*)info->getOfficerCardItemArray()->objectAtIndex(i);
                    if(card->getSsid() == cardInfo.officercarditemid())
                    {
                        card->addEquip(equipCard);
                    }
                }
            }
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            equipCard->setMaxExp(expdata->getExpValue());
            equipCard->setRound(temp->getEquipRound());
            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            equipCard->setExpRate(temp->getEquipExpRate());
            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
            equipCard->setGoverning(temp->getEquipGoverning());
            equipCard->setItemType(temp->getEquipType());
            equipCard->setMaxStar(temp->getEquipNextId());
            equipCard->setOfficerName(temp->getEquipName());
            equipCard->setMaxLevel(temp->getEquipMaxLevel());
            if (temp->getFateDesc())
            {
                equipCard->setFateDesc(temp->getFateDesc());
            }
			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
            if (temp->getEquipNextId()) {
                equipCard->setCost(temp->getEquipCost());
                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }

            info->getEquipmentCardItemArray()->addObject(equipCard);

        }
    }

    info->autorelease();
    
    return info;
}

//获取与好友的关系
int SGFriend::isFriendOfPlayer(int roleId)
{
    SGGamePlayer *player = NULL;
    
    int friendCount = SGPlayerInfo::sharePlayerInfo()->getFriendList()->count();
    for(int i=0;i<friendCount;i++)
    {
        player = (SGGamePlayer*)SGPlayerInfo::sharePlayerInfo()->getFriendList()->objectAtIndex(i);
        if(roleId == player->getFriendID())
        {
            return SGFRIEND_FRIEND;
        }
    }
    
    int blackCount = SGPlayerInfo::sharePlayerInfo()->getBlackList()->count();
    for(int i=0;i<blackCount;i++)
    {
        player = (SGGamePlayer*)SGPlayerInfo::sharePlayerInfo()->getBlackList()->objectAtIndex(i);
        if(roleId == player->getFriendID())
        {
            return SGFRIEND_BLACK;
        }
    }
    
    int applyCount = SGPlayerInfo::sharePlayerInfo()->getApplyList()->count();
    for(int i=0;i<applyCount;i++)
    {
        player = (SGGamePlayer*)SGPlayerInfo::sharePlayerInfo()->getApplyList()->objectAtIndex(i);
        if(roleId == player->getFriendID())
        {
            return SGFRIEND_REQUEST;
        }
    }
    
    return SGFRIEND_STRANGER;
}

//检查某个玩家是不是自己
bool SGFriend::checkFriendIsSelf(int roleId)
{
    if(roleId == SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId())
    {
        return true;
    }
    
    return false;
}

void SGFriend::updateData()
{
    tableView->reloadData();
}
