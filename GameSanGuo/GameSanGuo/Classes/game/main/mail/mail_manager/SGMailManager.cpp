//
//  SGMainManager.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-12.
//
//

#include "SGMailManager.h"
#include "SGMainManager.h"
#include "SGFriendMailLayer.h"
#include "SGSystemMailLayer.h"
#include "SGFriendMailContent.h"
#include "SGMail.h"
#include "SGStaticDataManager.h"
#include "SGSoldierDataModel.h"
#include "SGFirstLayer.h"
#include "SGStringConfig.h"
SGMailManager::SGMailManager()
{
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MAIL_SENDMESSAGE, this, callfuncO_selector(SGMailManager::sendMailListener));
    
    // 好友邮件
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MAIL_DELETEMESSAGE, this, callfuncO_selector(SGMailManager::delFriMailListener));
    // 系统邮件
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MAIL_DELETESYSTEMMAIL, this, callfuncO_selector(SGMailManager::delSysMailListener));
    
    selectIndex = -1;
}

SGMailManager::~SGMailManager()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_SENDMESSAGE);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_DELETEMESSAGE);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_DELETESYSTEMMAIL);
}

void SGMailManager::delSysMailListener(CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("删除邮件失败");
        return;
    }
    SG_SHOW_WINDOW(str_FriendMailLayer_str7);
    main::DeleteSystemMailResponse *response = (main::DeleteSystemMailResponse*)request->m_msg;
    if(1 == response->state())
    {
        CCArray *datas = SGPlayerInfo::sharePlayerInfo()->getSystemMails();
        SGMailSystemDataModel *model = NULL;
        for (int ii = 0; ii < datas->count(); ii++) {
            SGMailSystemDataModel *obj = (SGMailSystemDataModel *)datas->objectAtIndex(ii);
            if (obj->getSsid() == selectIndex) {
                model = obj;
                break;
            }
        }
        if (model != NULL) { // 删除原邮件
            if (model->getAccessoryArray() && model->getAccessoryArray()->count()>0) {
                // 添加附件到军营中
                CCArray *array = model->getAccessoryArray();
                SGMailSystemAccessoryDataModel *accModel = NULL;
                for (int ii = 0; ii < array->count(); ii++) {
                    accModel = (SGMailSystemAccessoryDataModel *)array->objectAtIndex(ii);
                    SGMailManager::getAccessoryByModel(accModel);
                }
            }
            datas->removeObject(model);
        }
        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGSystemMailLayer *system = (SGSystemMailLayer *)mainScene->getChildByTag(sg_systemMail);
        if (system == NULL) {
            system = SGSystemMailLayer::create(datas);
            SGMainManager::shareMain()->showLayer(system);
        }else {
            system->recieveReloadUpdate(model);
        }
    }
}

void SGMailManager::delFriMailListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        printf("error deleteMailListener !!!  \n");
        return;
    }
    
    main::DeleteMailResponse *response = (main::DeleteMailResponse*)request->m_msg;
    if(1 == response->state())
    {
        CCArray *datas = SGPlayerInfo::sharePlayerInfo()->getFriendMails();
        SGMailDataModel *model = NULL;
        for (int ii = 0; ii < datas->count(); ii++) {
            SGMailDataModel *obj = (SGMailDataModel *)datas->objectAtIndex(ii);
            if (obj->getMailSsid() == selectIndex) {
                model = obj;
                break;
            }
        }
        if (model != NULL) datas->removeObject(model);
        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGFriendMailLayer *system = (SGFriendMailLayer *)mainScene->getChildByTag(sg_friendMail);
        if (system == NULL) {
            system = SGFriendMailLayer::create(datas);
            SGMainManager::shareMain()->showLayer(system);
        }else {
            system->recieveReloadUpdate(model);
        }
    }
}

void SGMailManager::sendMailListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        printf("error sendMailListener !!! \n");
        return;
    }
    
    main::SendMailResponse *response = (main::SendMailResponse*)request->m_msg;
    
    main::MailProto mailproto = response->mailproto();
    
    SGMailDataModel *dataModel = getMailDataModelByMailProto(mailproto);
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCArray *array = NULL;
    if(1 == dataModel->getMailType())
    {
        int senderId = dataModel->getMailOtherId();
        SGGamePlayer *player = NULL;
        CCArray *blist = SGPlayerInfo::sharePlayerInfo()->getBlackList();
        for (int ii = 0; ii < blist->count(); ii++) {
            player = (SGGamePlayer *)blist->objectAtIndex(ii);
            
            if (player->getFriendID() == senderId) {
                return;
            }
        }
        array = playerInfo->getFriendMails();
    }else if(2 == dataModel->getMailType())
    {
        array = playerInfo->getSystemMails();
    }else
    {
        array = playerInfo->getGMMails();
    }
    
    //用于指向新发来的这个邮件的主体
    SGMailDataModel *newDataModel = NULL;
    
    bool haveIt = false;
    CCObject *object = NULL;
    if (array) {
        CCARRAY_FOREACH(array, object)
        {
            SGMailDataModel *dataModelObj = (SGMailDataModel*)object;
            if(dataModelObj->getMailSsid() == dataModel->getMailSsid())
            {
                //已经有了这个邮件主体，那么我就把它的新消息加进去，然后设为未读
                haveIt = true;
                dataModelObj->getMailContent()->addObject(dataModel->getMailContent()->lastObject());
                dataModelObj->setMailIsRead(dataModel->getMailIsRead());
                newDataModel = dataModelObj;
                break;
            }else if(dataModelObj->getMailSsid() == -1){
                //这个邮件主体是我临时新建的，是个空的，现在需要替换它
                dataModelObj->setMailSsid(dataModel->getMailSsid());
                dataModelObj->setMailIsRead(dataModel->getMailIsRead());
                dataModelObj->setMailOneSelfId(dataModel->getMailOneSelfId());
                dataModelObj->setMailOneselfNickname(dataModel->getMailOneselfNickname());
                dataModelObj->setMailOtherId(dataModel->getMailOtherId());
                dataModelObj->setMailOtherNickname(dataModel->getMailOtherNickname());
                dataModelObj->setMailType(dataModel->getMailType());
                dataModelObj->getMailContent()->addObject(dataModel->getMailContent()->lastObject());
                dataModelObj->setMailItemId(dataModel->getMailItemId());
                newDataModel = dataModelObj;
                haveIt = true;
                break;
            }
        }
        
        //如果没有这么一个邮件体，那就把它当做一个新的邮件加进数组
        if(!haveIt)
        {
            array->addObject(dataModel);
            newDataModel = dataModel;
        }
    }
    
    //flush layer
    CCArray *arrayLayer = SGMainManager::shareMain()->getMainScene()->getChildren();
    
    CCObject *objectLayer = NULL;
    CCARRAY_FOREACH(arrayLayer, objectLayer)
    {
        SGBaseLayer *layer = (SGBaseLayer*)objectLayer;

        if(layer->getTag() == sg_mailLayer)
        {
            SGMail *mail = (SGMail*)layer;
            mail->recieveMailUpdate();
        }
        if(layer->getTag() == sg_friendMail)
        {
            SGFriendMailLayer *friends = (SGFriendMailLayer*)layer;
            friends->recieveReloadUpdate();
        }
        if(layer->getTag() == sg_systemMail)
        {
            SGSystemMailLayer *system = (SGSystemMailLayer*)layer;
            system->recieveReloadUpdate();
        }
        if(layer->getTag() == sg_friendMailContent)
        {
            SGFriendMailContent *content = (SGFriendMailContent*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendMailContent);
            content->setDataModel(newDataModel);
            content->constructScrollView();
        }
    }
//    updateData();
}

SGMailDataModel *SGMailManager::getMailDataModelByMailProto(main::MailProto mailProto)
{
    SGMailDataModel *dataModel = new SGMailDataModel();
    dataModel->setMailIsRead(mailProto.isread());
    dataModel->setMailOneSelfId(mailProto.oneselfid());
    dataModel->setMailOneselfNickname(CCString::create(mailProto.oneselfnickname()));
    dataModel->setMailOtherId(mailProto.otherid());
    dataModel->setMailOtherNickname(CCString::create(mailProto.othernickname()));
    dataModel->setMailSsid(mailProto.ssid());
    dataModel->setMailType(mailProto.type());
    dataModel->setMailItemId(mailProto.image());
    
    int mailDetailCount = mailProto.maildetailsproto_size();
    for(int i=0;i<mailDetailCount;i++)
    {
        dataModel->getMailContent()->addObject(getMailDetailDataModelByMailProto(mailProto.maildetailsproto(i)));
    }
    
    dataModel->autorelease();
    
    return dataModel;
}

SGMailDetailDataModel *SGMailManager::getMailDetailDataModelByMailProto(main::MailDetailsProto mailDetailProto)
{
    SGMailDetailDataModel *detailModel = new SGMailDetailDataModel();
    detailModel->setMailDeContent(CCString::create(mailDetailProto.content()));
    detailModel->setMailDeId(mailDetailProto.mailid());
    detailModel->setMailDeReceiverId(mailDetailProto.receiverid());
    detailModel->setMailDeReceiverNickName(CCString::create(mailDetailProto.receivernickname()));
    detailModel->setMailDeSendDate(mailDetailProto.senddate());
    detailModel->setMailDeSenderId(mailDetailProto.senderid());
    detailModel->setMailDeSenderNickName(CCString::create(mailDetailProto.sendernickname()));
    detailModel->autorelease();

    return detailModel;
}

//系统邮件
SGMailSystemDataModel* SGMailManager::getSystemMailByProto(main::SystemMailProto systemProto)
{
    SGMailSystemDataModel *system = new SGMailSystemDataModel();
    system->setIsRead(systemProto.isread());
    system->setIsReceive(systemProto.isreceive());
    system->setRoleId(systemProto.roleid());
    system->setSendTime(systemProto.senddate());
    system->setSsid(systemProto.ssid());
    system->setcontent(CCString::create(systemProto.content()));
    
    int accessoryCount = systemProto.attachmentproto_size();
    for(int i=0;i<accessoryCount;i++)
    {
        SGMailSystemAccessoryDataModel *acc = new SGMailSystemAccessoryDataModel();

        main::AttachmentProto att = systemProto.attachmentproto(i);
        acc->setCount(att.count());
        acc->setItemId(att.itemid());
        acc->setLevel(att.level());
        acc->setSsid(att.ssid());
        acc->setSystemMailId(att.systemmailid());
        acc->setType(att.type());
        
        //把附件添加进邮件的数组里面
        system->getAccessoryArray()->addObject(acc);
        acc->release();

    }
    system->autorelease();
    return system;
    
}

void SGMailManager::getAccessoryByProto(main::AttachmentProto att)
{
    
    //1:将领卡牌 2:装备卡牌 3:士兵卡牌 4:道具卡牌 5:铜钱 6:金子 7:军功 8:炼丹秘方 9:小喇叭
    switch(att.type())
    {
        case 1://将领卡牌
        {
            SGOfficerCard *officerCard = new SGOfficerCard();
            main::OfficerCardItemProto proto = (main::OfficerCardItemProto)att.officercarditemproto();
            
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
            
//            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
            
            officerCard->setSsid(proto.ssid());
            officerCard->setItemId(proto.itemid());
            officerCard->setCurrExp(proto.currexp());
            officerCard->setLordId(proto.lordskillid());
            officerCard->setIsProt(proto.isprotection());
            //            officerCard->setIsExped(proto.isexpedition());
            officerCard->setCurrLevel(proto.currlevel());
            officerCard->setOfficerSkil(proto.generalsskillid());
            officerCard->setSkillExp(proto.generalsskillcurrexp());
            officerCard->setLordExp(proto.lordskillcurrexp());
            //proto.generalsskillid()
            
            //武将攻防血速差异性，这几个属性改由服务器发送
            officerCard->setRealAtk(proto.atk());
            officerCard->setRealDef(proto.def());
            officerCard->setRealMorale(proto.hp());
            officerCard->setRealSpeed(proto.sp());
            
            if (temp->getOfficerNextId()) {
                officerCard->setCost(temp->getOfficerCost());
                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
            officerCard->setMaxExp(expdata->getExpValue());
            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
            officerCard->setOfficerName(temp->getOfficerName());
            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            officerCard->setRound(temp->getOfficerRound());
            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            officerCard->setGoverning(temp->getOfficerGoverning());
            officerCard->setRace(temp->getOfficerRace());
            officerCard->setItemType(temp->getOfficerType());
            officerCard->setUpgradestar(temp->getUpgradestar());
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            officerCard->setFateList(temp->getOfficerFateList());
            //转生次数
            officerCard->setAdNum(temp->getAdvNum());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            
            //武将原型ID
            officerCard->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
			
			officerCard->setHeadSuffixNum(temp->getIconId());//12.05
            
            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
            {
                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                officerCard->setLordSkill(atoi(lordskill.c_str()));
            }

            SGPlayerInfo::sharePlayerInfo()->getOfficerCards()->addObject(officerCard);
            officerCard->release();
            
            break;
        }
        case 2://装备卡牌
        {
            main::EquipmentCardItemProto cardInfo = (main::EquipmentCardItemProto)att.equipmentcarditemproto();
            SGEquipCard *equipCard = new SGEquipCard();
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
            
//            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
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
                SGOfficerCard *officer = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(cardInfo.officercarditemid());
                officer->addEquip(equipCard);
            }
            
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
            }			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
			
            if (temp->getEquipNextId()) {
                equipCard->setCost(temp->getEquipCost());
                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }

            
            SGPlayerInfo::sharePlayerInfo()->getEquipCards()->addObject(equipCard);
            equipCard->release();

            break;
        }
        case 3://士兵卡牌
        {
            main::SoldierCardItemProto proto = (main::SoldierCardItemProto)att.soldiercarditemproto();
            SGSoldierCard *soldierCard = new SGSoldierCard();
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
            
            soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
            soldierCard->setSsid(proto.ssid());
            soldierCard->setItemId(proto.itemid());
            soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            soldierCard->setMaxExp(expdata->getExpValue());
            soldierCard->setOfficerName(temp->getSoldierName());
            soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            soldierCard->setRound(temp->getSoldierRound());
            soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            soldierCard->setCurrLevel(proto.currlevel());
            soldierCard->setCurrStar(temp->getSoldierStarLevel());
            soldierCard->setCurrExp(proto.currexp());
            soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
            soldierCard->setsoldiergrowskill(temp->getGrowSkill());

            SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->addObject(soldierCard);
            soldierCard->release();
            
            
            break;
        }
        case 4://道具卡牌
        {
            main::PropsCardItemProto cardInfo = (main::PropsCardItemProto)att.propscarditemproto();
            SGPropsCard *propCard = new SGPropsCard();
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
            
            propCard->setSsid(cardInfo.ssid());
            propCard->setItemId(cardInfo.itemid());
            propCard->setIsProt(cardInfo.isprotection());
            
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
			propCard->setHeadSuffixNum(prop->getIconId());//12.05

            SGPlayerInfo::sharePlayerInfo()->getPropCards()->addObject(propCard);
            propCard->release();
            
            break;
        }
        case 5://铜钱
        {
            SGPlayerInfo::sharePlayerInfo()->changeCoinsCount(att.count());
            break;
        }
        case 6://金子
        {
            SGPlayerInfo::sharePlayerInfo()->changeGoldCount(att.count());
            break;
        }
//        case 7://军功
//        {
//            SGPlayerInfo::sharePlayerInfo()->changeAwardCount(att.count());
//            break;
//        }
        case 8://炼丹秘方
        {
            SGPlayerInfo::sharePlayerInfo()->changeLiandanCount(att.count());
            break;
        }
//        case 9://小喇叭
//        {
//            SGPlayerInfo::sharePlayerInfo()->changeSpeakerCount(att.count());
//            break;
//        }
    }
}

void SGMailManager::getAccessoryByModel(SGMailSystemAccessoryDataModel *model)
{
    //1:将领卡牌 2:装备卡牌 3:士兵卡牌 4:道具卡牌 5:铜钱 6:金子 7:军功 8:炼丹秘方 9:小喇叭
    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
    switch(model->getType())
    {
        case 1://将领卡牌
        {
            SGOfficerDataModel *oCard = manager->getOfficerById(model->getItemId());
            SGOfficerCard *officerCard = new SGOfficerCard;
            officerCard->setOfficerName(oCard->getOfficerName());
            officerCard->setMaxLevel(oCard->getOfficerMaxLevel());
            officerCard->setCurrExp(0);
            officerCard->setCurrStar(oCard->getOfficerCurrStarLevel());
            officerCard->setIsProt(0);
            //        officerCard->setIsExped(0);
            officerCard->setRace(oCard->getOfficerRace());
            officerCard->setItemId(oCard->getOfficerId());
            
            if (oCard->getOfficerGeneralsSkill()&&!oCard->getOfficerGeneralsSkill()->isEqual(CCString::create(""))) {
                officerCard->setOfficerSkil(manager->getAnyoneIdByBeanId(oCard->getOfficerGeneralsSkill()->getCString()));
            }
            if (oCard->getOfficerLordSkill() &&!oCard->getOfficerLordSkill()->isEqual(CCString::create(""))) {
                int lordskill = manager->getAnyoneIdByBeanId(oCard->getOfficerLordSkill()->getCString());
                officerCard->setLordSkill(lordskill);
            }
            
            officerCard->setAtk(oCard->getOfficerBaseAtk());
            officerCard->setDef(oCard->getOfficerBaseDef());
            officerCard->setRound(oCard->getOfficerRound());
            officerCard->setSpeed(oCard->getOfficerBaseSpeed());
            officerCard->setMorale(oCard->getOfficerMorale());
            officerCard->setGoverning(oCard->getOfficerGoverning());
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
            //转生次数
            officerCard->setAdNum(oCard->getAdvNum());
            officerCard->setFateList(oCard->getOfficerFateList());
            
            //武将原型ID
            officerCard->setProtoId(oCard->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(oCard->getOfficerSpeedFactor());
			
			officerCard->setHeadSuffixNum(oCard->getIconId());//12.06
            
            SGPlayerInfo::sharePlayerInfo()->getOfficerCards()->addObject(officerCard);
            officerCard->release();
            
            break;
        }
        case 2://装备卡牌
        {
            SGEquipCard *equipCard = new SGEquipCard;
            SGEquipmentDataModel *eCard = manager->getEquipById(model->getItemId());
            
            //装备原型ID
            equipCard->setProtoId(eCard->getEquipProtoId());
            equipCard->setCurrStar(eCard->getEquipCurrStarLevel());
            equipCard->setCurrExp(0);
            equipCard->setOfficerCardId(0);
            equipCard->setItemType(eCard->getEquipType());
            equipCard->setIsProt(0);
            equipCard->setOfficerName(CCString::create(eCard->getEquipName()->getCString()));
            equipCard->setItemId(model->getItemId());
            equipCard->setMorale(eCard->getEquipHP());
            equipCard->setAtk(eCard->getEquipmentBaseAtk());
            equipCard->setDef(eCard->getEquipBaseDef());
            equipCard->setRound(eCard->getEquipRound());
            equipCard->setSpeed(eCard->getEquipBaseSpeed());
            equipCard->setMaxLevel(eCard->getEquipMaxLevel());
            //转生等级
            equipCard->setUpgradelevel(eCard->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(eCard->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(eCard->getUpgradestar());
            //装备原型ID
            equipCard->setProtoId(eCard->getEquipProtoId());
			
			equipCard->setHeadSuffixNum(eCard->getIconId());//12.06
            
            SGPlayerInfo::sharePlayerInfo()->getEquipCards()->addObject(equipCard);
            equipCard->release();
            
            break;
        }
        case 3://士兵卡牌
        {
            SGSoldierCard *soldierCard = new SGSoldierCard();
            CCDictionary *dicc = SGCardAttribute::getValue(model->getLevel(), model->getItemId());
            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(model->getItemId());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, model->getLevel()+1);
            
            if (temp) {
                soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
                soldierCard->setSsid(model->getSsid());
                soldierCard->setItemId(model->getItemId());
                soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                soldierCard->setMaxExp(expdata->getExpValue());
                soldierCard->setOfficerName(temp->getSoldierName());
                soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                soldierCard->setRound(temp->getSoldierRound());
                soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                soldierCard->setCurrLevel(model->getLevel());
                soldierCard->setCurrStar(temp->getSoldierStarLevel());
                soldierCard->setCurrExp(0);
                soldierCard->setsoldiergrowskill(temp->getGrowSkill());//士兵增加
                soldierCard->setsoldiertalentskill(temp->getTalentSkill());
                
                
                SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->addObject(soldierCard);
            }

            soldierCard->release();
            
            
            break;
        }
        case 4://道具卡牌
        {
            SGPropsDataModel *prop = manager->getPropsById(model->getItemId());
            SGPropsCard *propCard = new SGPropsCard;
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setIsProt(0);
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setItemId(prop->getPropsId());
            propCard->setHeadSuffixNum(prop->getIconId());//12.05
			
            SGPlayerInfo::sharePlayerInfo()->getPropCards()->addObject(propCard);
            propCard->release();
            
            break;
        }
        case 5://铜钱
        {
            SGPlayerInfo::sharePlayerInfo()->changeCoinsCount(model->getCount());
            break;
        }
        case 6://金子
        {
            SGPlayerInfo::sharePlayerInfo()->changeGoldCount(model->getCount());
            break;
        }
//        case 7://军功
//        {
//            SGPlayerInfo::sharePlayerInfo()->changeAwardCount(model->getCount());
//            break;
//        }
        case 8://炼丹秘方
        {
            SGPlayerInfo::sharePlayerInfo()->changeLiandanCount(model->getCount());
            break;
        }
//        case 9://小喇叭
//        {
//            SGPlayerInfo::sharePlayerInfo()->changeSpeakerCount(model->getCount());
//            break;
//        }
    }
}

//void SGMailManager::updateData()
//{
//    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//    SGFirstLayer *firstLayer = (SGFirstLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
//    int count = 0;
//    if(firstLayer)
//    {
//            for(int i=0;i<playerInfo->getFriendMails()->count();i++)
//            {
//                SGMailDataModel *mailData = (SGMailDataModel*)playerInfo->getFriendMails()->objectAtIndex(i);
//                if(!mailData->getMailIsRead())
//                {
//                    count ++;
//                }
//            }
//   
//            int systemCount = playerInfo->getSystemMails()->count();
//            for(int i=0;i<systemCount;i++)
//            {
//                SGMailSystemDataModel *mailData = (SGMailSystemDataModel*)playerInfo->getSystemMails()->objectAtIndex(i);
//                if(!mailData->getIsRead())
//                {
//                    count ++;
//                }
//            }
//   
//            for(int i=0;i<playerInfo->getGMMails()->count();i++)
//            {
//                SGMailDataModel *mailData = (SGMailDataModel*)playerInfo->getGMMails()->objectAtIndex(i);
//                if(!mailData->getMailIsRead())
//                {
//                    count ++;
//                }
//            }
//        firstLayer->setMailCount(count);
//    }
//}

HeadInfo SGMailManager:: getAccessoryInfoByData(SGMailSystemAccessoryDataModel *access)
{
//    access->setType(16);
//    access->setCount(2);
    
    HeadInfo headInfo;
    headInfo.fontName=FONT_PANGWA;
    headInfo.advanceNum = 0;
    //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 2天梯积分  3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14号(废弃) 15消耗品 16 勋章 17 材料 18 勾玉
    SGBaseItemType bit = (SGBaseItemType) access->getType();
    switch(bit)
    {
        case BIT_OFFICER://将领卡牌
        {
            SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(access->getItemId());
            if(officerData)
            {
                headInfo.type=officerCard;
                SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_systemMailContent);
                headInfo.fileName=CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString();
                int starlvl = officerData->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                
                headInfo.kuangName=CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString();
                headInfo.countryName=CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace())->getCString();
                headInfo.fontName=FONT_PANGWA;
                headInfo.reward_name=CCString::createWithFormat("%s",officerData->getOfficerName()->getCString())->getCString();
                headInfo.reward_count=CCString::createWithFormat("X%d",access->getCount())->getCString();
                headInfo.advanceNum = officerData->getAdvNum();
                
            }
            break;
        }
        case BIT_EQUIP://装备卡牌
        {
            //                sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",access->getItemId())->getCString());
            SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(access->getItemId());
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(equip->getIconId(), sg_systemMailContent);
            
            int starlvl = equip->getEquipStarPinzhi();
//            if (starlvl>1) {s
//                starlvl-=1;
//            }
            headInfo.type=equipCard;
            headInfo.fileName=CCString::createWithFormat("head%d.png",equip->getIconId())->getCString();
            headInfo.kuangName=CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString();
            headInfo.reward_name=CCString::createWithFormat("%s",equip->getEquipName()->getCString())->getCString();
            headInfo.advanceNum = equip->getAdvNum();
            
            break;
        }
        case BIT_SOLDIER://士兵卡牌
        {
            SGSoldierDataModel *soldier = SGStaticDataManager::shareStatic()->getSoliderById(access->getItemId());
            if(soldier)
            {
                ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
                headInfo.type=soliderCard;
                headInfo.fileName=CCString::createWithFormat("%d-1.png",access->getItemId())->getCString();
                headInfo.kuangName=CCString::createWithFormat("country_%d.png",soldier->getSoldierRace())->getCString();
                headInfo.reward_name=CCString::createWithFormat("%s",soldier->getSoldierName()->getCString())->getCString();
            }
            
            break;
        }
        case BIT_PROP://道具卡牌
        {
            //12.06
            
            
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(access->getItemId());
            SGMainManager::shareMain()->addHeadIconPlistByNum(prop->getIconId() , sg_systemMailContent);
            
            headInfo.type=propCard;
            headInfo.fileName=CCString::createWithFormat("head%d.png",prop->getIconId())->getCString();
			
			if (prop->getPropsPropsType() == 3 || prop->getPropsPropsType() == 4 || prop->getPropsPropsType() == 5)//如果是兵魂卡或者是技能卷轴
			{
				int starLvl = prop->getPropsCurrStarLevel();
				if (starLvl > 1)
					starLvl -= 1;
				headInfo.kuangName = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString();
			}
			else
			{
                int starLvl = prop->getPropsCurrStarLevel();
				if (starLvl > 1)
					starLvl -= 1;
				headInfo.kuangName = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString();
			}
            
            headInfo.reward_name=CCString::createWithFormat("%s",prop->getPropsName()->getCString())->getCString();
            
            break;
        }
        case BIT_GOLD://金币(铜钱)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
            
            headInfo.type=coinCard;
            headInfo.fileName="head9999.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_Tong_money;
            break;
        }
        case BIT_INGOT://元宝
        {
            
            headInfo.type=goldCard;
            headInfo.fileName="big_yb.png";
            headInfo.kuangName="equipPinzhi_4_0.png";
            headInfo.reward_name=str_yuanbao;
            
            break;
        }
        case BIT_FEAT://军功
        {
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
            headInfo.type=militaryCard;
            headInfo.fileName="head9997.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_jungong;
            break;
        }
        case BIT_RECIPE://炼丹秘方
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
            headInfo.type=alchemyCard;
            headInfo.fileName="head9996.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_liandianmifang;
            break;
        }
        case BIT_HORN://小喇叭
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
            headInfo.type=honkedCard;
            headInfo.fileName="head9998.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_Small_laba;
           
            break;
        }
        case BIT_PIECE://碎片
        {
            headInfo.type=pieceCard;
            
            ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
            SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(access->getItemId());
            
            int starLv = pieces->getStarLevel();
            if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
            {
                starLv -= 1;
            }
            
            int piecesType=pieces->getPiecesType();
            if (piecesType==0) {
                SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
                if (officerData)
                {
                    headInfo.type=pieceOfficeCard;
                    SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_systemMailContent);
                    headInfo.fileName=CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString();
                    headInfo.kuangName=CCString::createWithFormat("officerPinzhi_%d_0.png", starLv)->getCString();
                    headInfo.countryName=CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace())->getCString();
                    headInfo.fontName=FONT_PANGWA;
                    headInfo.reward_name=CCString::createWithFormat("%s",pieces->getPiecesName()->getCString())->getCString();
                    headInfo.reward_count=CCString::createWithFormat("X %d",access->getCount())->getCString();
                }

            }else if (piecesType==1)
            {
                SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId());
                
                SGMainManager::shareMain()->addHeadIconPlistByNum(equip->getIconId(), sg_systemMailContent);
                
//                int starlvl = equip->getEquipStarPinzhi();

                headInfo.type=pieceEquipcard;
                headInfo.fileName=CCString::createWithFormat("head%d.png",equip->getIconId())->getCString();
                headInfo.kuangName=CCString::createWithFormat("equipPinzhi_%d_0.png", starLv)->getCString();
                headInfo.reward_name=CCString::createWithFormat("%s",pieces->getPiecesName()->getCString())->getCString();
                headInfo.reward_count=CCString::createWithFormat("X %d",access->getCount())->getCString();

            }
            
        }
            break;
        case BIT_SOUL://军魂,
        {
            headInfo.type=pieceCard;
            headInfo.fileName="soul_icon.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_FriendMailLayer_str8;
            break;
        }
        case BIT_VIGOUR://体力
        {
            headInfo.type=pieceCard;
            headInfo.fileName="mail_streng.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_CantAdvance_tips64;
            break;
        }
        case BIT_GRAIN://军粮
        {
            headInfo.type=pieceCard;
            headInfo.fileName="mail_food.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_CantAdvance_tips65;
            break;
        }
        case BIT_GRADE://天梯积分
        {
            headInfo.type=pieceCard;
            headInfo.fileName="store_tianti_big.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_FriendMailLayer_str9;
            break;
        }

		case BIT_CONSUMABLE://消耗品
		{
			headInfo.type = consumableCard;
			SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(access->getItemId());
			SGMainManager::shareMain()->addHeadIconPlistByNum(consume->getIconId(), sg_systemMailContent);
			headInfo.fileName = CCString::createWithFormat("head%d.png", consume->getIconId())->getCString();
			
			int starLvl = consume->getConsumeStarLvl();
			if (starLvl > 1)
				starLvl -= 1;
			headInfo.kuangName = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString();
			
			headInfo.reward_name = CCString::createWithFormat("%s", consume->getConsumeName()->getCString())->getCString();
			headInfo.reward_count=CCString::createWithFormat("X %d",access->getCount())->getCString();
		}
            break;
        case BIT_MEDAL://勋章
		{
            ResourceManager::sharedInstance()->bindTexture("sgtongquelayer/sgtongquelayer.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
            headInfo.type=medalCard;
            headInfo.fileName="tongqueXunBig.png";
            headInfo.kuangName="equipPinzhi_3_0.png";
            headInfo.reward_name=str_Format_xunzhang__;
            break;
			 
		}
        case BIT_MATERIAL://材料
        {
            //获取材料的原型
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(access->getItemId());
            
            if (materialModel)
            {
                SGMainManager::shareMain()->addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_systemMailContent);
                
                headInfo.type = materialCard;
                headInfo.fileName = CCString::createWithFormat("head%d.png", materialModel->getMaterialIconId())->getCString();
                //边框颜色
                int borderColor = materialModel->getMaterialBorderColor();
                if (borderColor > 1)
                {
                    borderColor -= 1;
                }
                headInfo.kuangName = CCString::createWithFormat("equipPinzhi_%d_0.png", borderColor)->getCString();
                headInfo.reward_name = materialModel->getMaterialName()->getCString();
                
                headInfo.reward_count=CCString::createWithFormat("X %d",access->getCount())->getCString();
            }
            
            break;
        }
        case BIT_JADE: //勾玉
        {
            headInfo.type = jadeCard;
            headInfo.fileName = "big_gouyu.png";
            //边框颜色
            headInfo.kuangName = "equipPinzhi_3_0.png";
            headInfo.reward_name = str_chinese_jade;
            break;
        }
        case BIT_ARENA_SCORE: //征服點
        {
            headInfo.type = arenaScoreCard;
            headInfo.fileName = "arean_coin_big.png";
            //边框颜色
            headInfo.kuangName = "equipPinzhi_3_0.png";
            headInfo.reward_name = str_arena_score;
        }
            break;
		default:
			break;
    }
    //在这里添加的数量，威武。
    if (access->getCount() > 0)
    {
        headInfo.reward_count=CCString::createWithFormat("X %d",access->getCount())->getCString();
    }
    return headInfo;
    
}


	