//
//  SGMainTaskDataModel.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-14.
//
//

#include "SGMainTaskDataModel.h"

SGMainTaskDataModel::SGMainTaskDataModel()
:m_rewardType(nullCard),
m_allProgress(0),
m_curProgress(0),
m_itemId(0),
m_rewardCount(0),
str_bossName(NULL),
str_taskDeprision(NULL),
b_complateState(false),
task_id(0),
taskGroup_id(0),
m_showType(0),
m_arraive(-10),
str_taskName(NULL)
, rewardModel(NULL)
{
    rewardModel=new SGRewardDataModel();
}

SGMainTaskDataModel::~SGMainTaskDataModel(){
    CC_SAFE_RELEASE(str_taskDeprision);
    CC_SAFE_RELEASE(str_bossName);
    CC_SAFE_RELEASE(str_taskName);
}
#pragma  mark 


SGMainTaskGroup::SGMainTaskGroup()
:str_taskGroupName(NULL),
taskGroup_id(0),
tabId(0),
initQuestId(0),
b_hasFinish(false),
b_hasTask(false)
{
    
}

SGMainTaskGroup::~SGMainTaskGroup(){
    CC_SAFE_RELEASE(str_taskGroupName);
    
}
SGMainTaskSubModal::SGMainTaskSubModal()
:m_rewareId(0),
taskid(0),
tabId(0),
str_taskDesprition(NULL),
str_taskName(NULL)
{
    
}

SGMainTaskSubModal::~SGMainTaskSubModal(){
//    CC_SAFE_RELEASE(str_taskGroupName);
    
}
