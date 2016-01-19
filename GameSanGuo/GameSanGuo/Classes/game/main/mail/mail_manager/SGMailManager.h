//
//  SGMailManager.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-12.
//
//

#define MAIL_LAYER_FRIEND       0    //朋友
#define MAIL_LAYER_SYSTEM       1    //系统
#define MAIL_LAYER_CONTENT      2    //内容
#define MAIL_LAYER_MAINLAYER    3    //邮件主界面


#ifndef __GameSanGuo__SGMailManager__
#define __GameSanGuo__SGMailManager__

#include <iostream>
#include "cocos2d.h"
#include "SGSocketClient.h"
#include "MailModule.pb.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGMailDataModel.h"
#include "SGMailSystemDataModel.h"
 
USING_NS_CC;
enum RewardType {
    nullCard,
    officerCard,
    equipCard,
    soliderCard,
    propCard,
    coinCard,
    goldCard,
    militaryCard,
    alchemyCard,
    honkedCard,
    pieceCard,
    soulCard,
    foodCard,
    strengCard,
    scoreCard,
	consumableCard,
    medalCard,
    pieceOfficeCard,
    pieceEquipcard,
    materialCard,
    jadeCard,
    arenaScoreCard,
};

struct HeadInfo {
    HeadInfo(){
        type = nullCard;
        fileName = "";
        kuangName = "";
        fontName = "";
        reward_name = "";
        reward_count = "";
        countryName = "";
        advanceNum = 0;
    }
    RewardType type ;
    const char *fileName;
    const char *kuangName;
    const char *fontName;
    const char *reward_name;
    const char *reward_count;
    const char *countryName;
    int advanceNum;
};
 
class SGMailManager : public CCObject
{
private:
    void sendMailListener(CCObject *obj);
    //刷新数据
//    void updateData();
    
public:
    int selectIndex; //选中的邮件
    
public:
    SGMailManager();
    
    ~SGMailManager();
    
    //获取邮件所有 主体 + 分体
    static SGMailDataModel *getMailDataModelByMailProto(main::MailProto mailProto);
    //获取邮件分体
    static SGMailDetailDataModel *getMailDetailDataModelByMailProto(main::MailDetailsProto mailDetailProto);
    //系统邮件protobuf转换为数据模型
    static SGMailSystemDataModel *getSystemMailByProto(main::SystemMailProto systemProto);
    //解析附件
    static void getAccessoryByProto(main::AttachmentProto att);
    //解析附件
    static void getAccessoryByModel(SGMailSystemAccessoryDataModel *model);
    
    void delFriMailListener(CCObject *object);
    void delSysMailListener(CCObject *object);
    
   static HeadInfo getAccessoryInfoByData(SGMailSystemAccessoryDataModel *access);

};


#endif /* defined(__GameSanGuo__SGMainManager__) */
