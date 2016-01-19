//
//  SGStoryBean.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-6.
//
//

#include "SGStoryBean.h"

SGStoryBean::SGStoryBean(void):_storyId(0)
,_storyName(NULL)
,_storySections(NULL)
, _minLevel(0)
, _visible(false)
, _iswin(0)
, _rank(0)
, _coinRate(0)
, _expRate(0)
, _conTime(0)
, _activityState(1)
, _startTime(0)
, _endTime(0)
, dayJoinTimes(0)
, buyTimes(0)
, canJoin(false)
, BuyCompTimes(0)
, buyPrice(0)
, _storyStars(0)
, _canReward(0)
, _maxStar(0)
, _rewardId(0)
, _rankStars(0)
, _rankReward(0)
, m_nChallengeCount(0)
, m_nMaxChallengeCount(0)
{
   
}

SGStoryBean::~SGStoryBean(void)
{
    if (_storySections->count()) {
        _storySections->removeAllObjects();
    }
    CC_SAFE_RELEASE(_storySections);
    _storyName->release();
}

SGStoryBean* SGStoryBean::create(int sid, const char* sname, cocos2d::CCArray *data)
{
    SGStoryBean *bean = new SGStoryBean();
    if (bean)
    {
        bean->autorelease();
        bean->initWithInfo(sid, sname, data);
        return bean;
    }
    CC_SAFE_DELETE(bean);
    return NULL;
}

void SGStoryBean::initWithInfo(int sid, const char *sname, cocos2d::CCArray *data)
{
    _storyId = sid;
    _storyName = CCString::create(sname);
    _storyName->retain();
    _storySections = data;
    _storySections->retain();
    
     _rankStars = 0;
     _rankReward = 0;
}

SGStorySceneBean* SGStoryBean::getSceneBeanById(int plotId)
{
    if (_storySections == NULL) return NULL;
    
    for (int ii = 0; ii < _storySections->count(); ii++)
    {
        SGStorySceneBean *sceneBean = (SGStorySceneBean *)_storySections->objectAtIndex(ii);
        if (plotId == sceneBean->getPlotId())
        {
            return sceneBean;
        }
    }
    return NULL;
}

void SGStoryBean::setSceneVisibleById(int plotId, bool visible)
{
    SGStorySceneBean *sceneBean = this->getSceneBeanById(plotId);
    if(sceneBean) sceneBean->setVisible(visible);
    this->setVisible(visible);
}
