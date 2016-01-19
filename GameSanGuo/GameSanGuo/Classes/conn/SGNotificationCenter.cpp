//
//  SG.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-19.
//
//

#include "SGNotificationCenter.h"
#include "SGMainManager.h"

static SGNotificationCenter *s_sharedNotifCenter = NULL;

SGNotificationCenter *SGNotificationCenter::sharedNotificationCenter(void)
{
    if ( !s_sharedNotifCenter )
    {
        s_sharedNotifCenter = new SGNotificationCenter;
    }
    return s_sharedNotifCenter;
}

void SGNotificationCenter::purgeNotificationCenter(void)
{
    CC_SAFE_DELETE(s_sharedNotifCenter);
}

SGNotificationCenter::SGNotificationCenter()
:isPostData(false)
{
    pthread_mutex_init(&_postMutex, NULL);
    pthread_mutex_init(&_observerMutex, NULL);
    
    _observerVec = new std::vector<SGNotificationObserver*>();
    _observerVec->reserve(200);
    
    _postVec = new std::vector<SGNotificationPost*>();
    _postVec->reserve(20);
    
    _focusedObservers = new std::vector<SGNotificationObserver*>();
    _focusedObservers->reserve(10);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, -9999, false);
}

SGNotificationCenter::~SGNotificationCenter()
{
    for ( std::vector<SGNotificationObserver*>::iterator iter = _observerVec->begin(); iter != _observerVec->end(); iter++)
    {
        CC_SAFE_DELETE(*iter);
    }
    _observerVec->clear();
    CC_SAFE_DELETE(_observerVec);
    
    for ( std::vector<SGNotificationPost*>::iterator iter = _postVec->begin(); iter != _postVec->end(); iter++)
    {
        CC_SAFE_DELETE(*iter);
    }
    _postVec->clear();
    CC_SAFE_DELETE(_postVec);
    
    _focusedObservers->clear();
    CC_SAFE_DELETE(_focusedObservers);
    _forceMatchMsgIdSet.clear();
    
    pthread_mutex_destroy(&_postMutex);
    pthread_mutex_destroy(&_observerMutex);
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

void SGNotificationCenter::addObserver(const char *name, CCObject *target, SEL_CallFuncO selector, CCObject *obj)
{
//    pthread_mutex_lock(&_observerMutex);
    
    do {
        if (this->observerExisted(target, name))
        {
            break;
        }
        
        SGNotificationObserver *observer = new SGNotificationObserver(target, selector, name, obj);
        if (!observer)
            break;
        _observerVec->push_back(observer);
    } while (0);
    
//    pthread_mutex_unlock(&_observerMutex);
}

void SGNotificationCenter::addObserver(short msgId_, CCObject *target, SEL_CallFuncO selector, CCObject *obj)
{
    char cmdName[16];
    memset(cmdName, 0, 16);
    sprintf(cmdName, "%s%d", cmdName, msgId_);
    
    addObserver(cmdName, target, selector, obj);
}
void SGNotificationCenter::removeObserver(CCObject *target, short msgId_)
{
    char cmdName[16];
    memset(cmdName, 0, 16);
    sprintf(cmdName, "%s%d", cmdName, msgId_);
    
    removeObserver(target, cmdName);
}
void SGNotificationCenter::removeObserver(CCObject *target, const char *name)
{
//    pthread_mutex_lock(&_observerMutex);
    
    for ( std::vector<SGNotificationObserver*>::iterator iter = _observerVec->begin(); iter != _observerVec->end(); iter++)
    {
        if ( (*iter)->getTarget() == target && (NULL == name || !strcmp( (*iter)->getName(), name)) )
        {
            CC_SAFE_DELETE(*iter);
            _observerVec->erase(iter);
            break;
        }
    }
    
//    pthread_mutex_unlock(&_observerMutex);
}

void SGNotificationCenter::removeAllObserver(CCObject *target)
{
    pthread_mutex_lock(&_observerMutex);
    
    for ( std::vector<SGNotificationObserver*>::iterator iter = _observerVec->begin(); iter != _observerVec->end(); iter++)
    {
        CC_SAFE_DELETE(*iter);
    }
    _observerVec->clear();
    pthread_mutex_unlock(&_observerMutex);

}

bool SGNotificationCenter::observerExisted(CCObject *target, const char *name)
{
    for ( std::vector<SGNotificationObserver*>::iterator iter = _observerVec->begin(); iter != _observerVec->end(); iter++)
    {
        if ( (*iter)->getTarget() == target && (NULL == name || !strcmp( (*iter)->getName(), name)) )
        {
            return true;
        }
    }
    return false;
}

void SGNotificationCenter::sendNotification(const char *name, CCObject* object)
{
    //catch
    for ( std::vector<SGNotificationObserver*>::iterator iter = _observerVec->begin(); iter != _observerVec->end(); iter++)
    {
        if ( strcmp( (*iter)->getName(), name) == 0 ) //catch one obs
        {
            _focusedObservers->push_back(*iter);
        }
    }

    //dispatch
    bool shallRmvCircle = checkShouldUnlock(name);
    int rSize = (int)_focusedObservers->size();
    
    CCLOG(">>>>HandleEvent || mId=%s, state=%s, fObs=%d, rmcc=%d", name, rSize <= 1 ? "GOOD!!" : "H!O!L!Y! S!H!I!T! B!A!N!G!", rSize, shallRmvCircle);
    for ( std::vector<SGNotificationObserver*>::reverse_iterator iter = _focusedObservers->rbegin(); iter != _focusedObservers->rend(); iter++)
    {
        //MMDBG: OBSV 由701消息引发，多个obs会导致问题，暂以最新的obs为处理基准。
        (*iter)->performSelector(object);
        break;
    }
    
    SGPlatFormIos* pf = SGMainManager::shareMain()->getMainScene()->plat;
    if (shallRmvCircle /*&& !pf->getIsShowCircle()*/) //上述执行逻辑中如果没有发送消息，才可以移除转圈。暂否定。
    {
        pf->removeConnection();
    }

    _focusedObservers->clear();
}

void SGNotificationCenter::postNotification(const char *name, CCObject* object, bool noInterval)
{
    //全部延时发送，废弃noInterval参数。
    do
    {
        SGNotificationPost *post = new SGNotificationPost(name, object);
        if (!post)
            break;
        pthread_mutex_lock(&_postMutex);
        _postVec->push_back(post);
        pthread_mutex_unlock(&_postMutex);
    }
    while (0);
}

void SGNotificationCenter::postNotification(const char *name, bool noInterval)
{
    this->postNotification(name, NULL, noInterval);
}

//添加强制配对消息。
void SGNotificationCenter::setForceMatchMsgId(const char* msgId)
{
    CCLOG("setForceMatchMsgId = %s", msgId);
    _forceMatchMsgIdSet.insert(msgId);
}

//判断是否可以移除锁屏。
bool SGNotificationCenter::checkShouldUnlock(const char* msgId)
{
    //否定方面。心跳/统计/公用提示/特殊字符串事件(如连接成功,失败等)则不解除。
    if (strcmp(msgId, "108") == 0 || strcmp(msgId, "1300") == 0 || strcmp(msgId, "2015") == 0 || strlen(msgId) > 5)
    {
        return false;
    }
    
    //强制配对方面。未达成强制配对则不解除。
    if (_forceMatchMsgIdSet.size() > 0)
    {
        //轮询排查。
        for (std::set<std::string>::iterator iter = _forceMatchMsgIdSet.begin(); iter != _forceMatchMsgIdSet.end();)
        {
            if (strcmp(msgId, (*iter).c_str()) == 0) //符合要求
            {
                _forceMatchMsgIdSet.erase(iter++);
            }
            else
            {
                iter++;
            }
        }
        
        //轮询过后，如果还有未配对的msgId，则返回false。
        if (_forceMatchMsgIdSet.size() > 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

void SGNotificationCenter::onMainNotification()
{
    SGNotificationPost* post = NULL;
    //get one post
    pthread_mutex_lock(&_postMutex);
    if (_postVec->size() > 0)
    {
        post = *(_postVec->begin());
        _postVec->erase(_postVec->begin());
    }
    pthread_mutex_unlock(&_postMutex);
    //handle the post
    if (post)
    {
        sendNotification(post->getPostName(), post->getPostObject());
        delete post; //surely.
    }
}

void SGNotificationCenter::update(float dt)
{
    //dhd 游戏通知中心
    if (isPostData)
    {
        this->onMainNotification();
    }
}

#pragma mark - SGNotificationObserver
SGNotificationObserver::SGNotificationObserver(CCObject *target, SEL_CallFuncO selector, const char *name, CCObject *obj) : m_target(target), m_selector(selector), m_name(name), m_object(obj), m_markForDelete(false)
{
    ;
}

SGNotificationObserver::~SGNotificationObserver()
{
    CCLOG("~SGNotificationObserver");
}

void SGNotificationObserver::performSelector(CCObject *obj)
{
    //如果有外部传入对象就使用，没有的就使用本身自带对象（可以是NULL）
    if (m_target)
    {
		if (obj)
        {
			(m_target->*m_selector)(obj);
		}
        else
        {
			(m_target->*m_selector)(m_object);
		}
    }
}
