//
//  SGStorySceneBean.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-6.
//
//

#include "SGStorySceneBean.h"

SGStorySceneBean::SGStorySceneBean(void):_storyId(0)
, _sceneId(0)
, _plotId(0)
, _needPower(0)
, _sceneName(NULL)
, _dropCoin(0)
, _dropExp(0)
, _visible(false)
, _sceneCount(0)
, _rank(0)
, dayJoinTimes(0)
, buyTimes(0)
, canJoin(false)
, BuyCompTimes(0)
, buyPrice(0)
, _power(0)
, _zhandou(0)
, _exp(0)
, _gold(0)
, _openState(0)
, _isBeatIt(false)
{

}

SGStorySceneBean::~SGStorySceneBean(void)
{
    CC_SAFE_DELETE(_sceneName);
}

SGStorySceneBean* SGStorySceneBean::create(int sid, int scid, int pid, int power, const char *sname)
{
    SGStorySceneBean *bean = new SGStorySceneBean();
    if (bean) {
        bean->initWithInfo(sid, scid, pid, power, sname);
        bean->autorelease();
        return bean;
    }
    CC_SAFE_DELETE(bean);
    return NULL;
}

void SGStorySceneBean::initWithInfo(int sid, int scid, int pid, int power, const char *sname)
{
    _plotId = pid;
    _storyId = sid;
    _sceneId = scid;
    _needPower = power;
    _sceneCount = 1; // 默认为1，暂时无用
    _sceneName = CCString::create(sname);
    _sceneName->retain();
}

void SGStorySceneBean::setDropInfo(int exp, int coin)
{
    _dropCoin = coin;
    _dropExp = exp;
}