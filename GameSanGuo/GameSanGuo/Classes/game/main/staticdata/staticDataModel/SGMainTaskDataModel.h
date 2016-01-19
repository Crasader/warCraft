//
//  SGMainTaskDataModel.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-14.
//
//

#ifndef __GameSanGuo__SGMainTaskDataModel__
#define __GameSanGuo__SGMainTaskDataModel__

#include <iostream>
#include "cocos2d.h"
#include "SGMailManager.h"
#include "SGRewardDataModel.h"

USING_NS_CC;



class SGMainTaskDataModel:public CCObject  {
    CC_SYNTHESIZE(int  ,task_id, taskId);//任务iD
    CC_SYNTHESIZE(int  ,taskGroup_id, taskGroupId);//任务组iD
    CC_SYNTHESIZE(int  ,m_rewardType, rewardType);//任务类型
    CC_SYNTHESIZE_RETAIN(CCString *, str_taskDeprision, taskDeprision);//任务描述
    CC_SYNTHESIZE_RETAIN(CCString *, str_taskName, taskName);//任务名字
    CC_SYNTHESIZE(int, m_rewardCount, rewardCount);// 奖励数量
    CC_SYNTHESIZE(int, m_itemId, ItemId); //物品id
    CC_SYNTHESIZE_RETAIN(CCString *, str_bossName, bossName);//boss 名字
    CC_SYNTHESIZE(int , b_complateState, complateState); // 完成状态
    CC_SYNTHESIZE(int, m_allProgress, allProgress);//完成进度
    CC_SYNTHESIZE(int, m_curProgress, curProgress);//完成量
    CC_SYNTHESIZE(int, m_showType, showType);//2 代表的是星级
    CC_SYNTHESIZE(int, m_arraive, Arrivement);//入口
    
    SGRewardDataModel *rewardModel;
    
public:
    SGMainTaskDataModel();
    ~SGMainTaskDataModel();
};

class SGMainTaskGroup:public CCObject  {
    CC_SYNTHESIZE(int  ,taskGroup_id, taskGroupId);//任务iD
    CC_SYNTHESIZE_RETAIN(CCString *, str_taskGroupName, taskGroupName);//任务组 名字
    
    CC_SYNTHESIZE(int  ,initQuestId, initQuestId);//
    CC_SYNTHESIZE(int  ,tabId, tabId);//
    CC_SYNTHESIZE(bool, b_hasFinish, hasFinish);
    CC_SYNTHESIZE(bool, b_hasTask, hasTask);//分组下面有任务；
    
public:
    SGMainTaskGroup();
    ~SGMainTaskGroup();
};

class SGMainTaskSubModal:public CCObject  {
    CC_SYNTHESIZE(int  ,taskid, taskId);//任务iD
    CC_SYNTHESIZE(int  ,m_rewareId, RewareId);//
    CC_SYNTHESIZE(int  ,tabId, tabId);//
    CC_SYNTHESIZE_RETAIN(CCString *, str_taskDesprition, taskDesprition);
    CC_SYNTHESIZE_RETAIN(CCString *, str_taskName, taskName);
    
public:
    SGMainTaskSubModal();
    ~SGMainTaskSubModal();
};


#endif /* defined(__GameSanGuo__SGMainTaskDataModel__) */
