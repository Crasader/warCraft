//
//  SGAreanItemFightLog.h
//  GameSanGuo
//  竞技场列表战斗日志item
//  Created by angel on 14-12-18.
//
//

#ifndef __GameSanGuo__SGAreanItemFightLog__
#define __GameSanGuo__SGAreanItemFightLog__

#include <iostream>
#include "SNSTableViewCellItem.h"
class SGAreanFightLogData;
class SGAreanItemFightLog: public SNSTableViewCellItem
{
public:
    SGAreanItemFightLog();
    ~SGAreanItemFightLog();
    
    static SGAreanItemFightLog *create(SGAreanFightLogData * itemFightLogData);
    void setItemFightLogData(SGAreanFightLogData * itemFightLogData);
    SGAreanFightLogData * getItemFightLogData();
    void initView();
    
    void onClickButtonLook();
    
    void resetElemnet();//根据当前的数据重新设置一下label及图片
    
    void refreshItem();//刷新一下item上面的信息
    
    CCString * calcTime(int time);
private:
    SGAreanFightLogData * m_pItemFightLogData;
    
    SGCCLabelTTF * m_pLabelName;
    SGCCLabelTTF * m_pLabelLevel;//等级
    SGCCLabelTTF * m_pLabelRank;//排名
    SGCCLabelTTF * m_pLabelTime;//时间
    SGCCLabelTTF * m_pLabelForeRank;//前排名
    SGCCLabelTTF * m_pLabelBackRank;//后排名
    SGCCLabelTTF * m_pLabelDeltaRank;//排名差
    
    CCSprite * m_pRankState;//排名上升或者下降或者不变
    CCSprite * m_pFightResult;
    
    SGButton * m_pButtonHeadIcon;
    
    
};

//
//  SGAreanFightLogData
//  GameSanGuo
//  竞技场列表战斗日志item数据
//  Created by angel on 14-12-18.
//
//

class SGAreanFightLogData: public CCObject
{

public:
    SGAreanFightLogData();
    ~SGAreanFightLogData();
    static SGAreanFightLogData * create(int roleId, CCString * nickName, int nationId,
                                        int officeItemId, int level, int foreRank,
                                        int backRank, bool isWin, bool isAttack, int time);
    
    CC_SYNTHESIZE(int , m_roleId, RoleId);
    CC_SYNTHESIZE_RETAIN(CCString * , m_nickName, NickName);
    CC_SYNTHESIZE(int , m_nationId, NationId);
    CC_SYNTHESIZE(int , m_officeItemId, OfficeItemId);
    CC_SYNTHESIZE(int , m_level, Level);
    CC_SYNTHESIZE(int , m_foreRank, ForeRank);
    CC_SYNTHESIZE(int , m_backRank, BackRank);
    CC_SYNTHESIZE(bool, m_isWin, IsWin);
    CC_SYNTHESIZE(bool, m_isAttack, IsAttack);
    CC_SYNTHESIZE(int , m_time,Time);
};
#endif /* defined(__GameSanGuo__SGAreanItemFightLog__) */



