//
//  SG.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-19.
//
//

#ifndef __GameSanGuo__SGNotificationCenter__
#define __GameSanGuo__SGNotificationCenter__

#include "cocos2d.h"
USING_NS_CC;

#define NC_NETWORK_LINK_SUCCESS  "network_link_success"
#define NC_NETWORK_LINK_FAIL     "network_link_fail"
#define NC_NETWORK_RECEIVE_ERROR "network_link_receive_error"
#define NC_NETWORK_RECEIVE_BREAK "network_link_receive_break"


#define SGNC_ADD_OBSERVER(_NAME_,_TARGET_,_SELECTOR_) (SGNotificationCenter::sharedNotificationCenter()->addObserver(_NAME_,_TARGET_,_SELECTOR_))
#define SGNC_ADD_OBSERVER_OBJ(_NAME_,_TARGET_,_SELECTOR_,_OBJ_) (SGNotificationCenter::sharedNotificationCenter()->addObserver(_NAME_,_TARGET_,_SELECTOR_,_OBJ_))

#define SGNC_REMOVE_OBSERVER(_TARGET_,_NAME_) (SGNotificationCenter::sharedNotificationCenter()->removeObserver(_TARGET_,_NAME_))
#define SGNC_REMOVE_OBSERVER_ALL(_TARGET_) (SGNotificationCenter::sharedNotificationCenter()->removeAllObserver(_TARGET_))

#define SGNC_POST_NOTIFICATION(_NAME_) (SGNotificationCenter::sharedNotificationCenter()->postNotification(_NAME_))
#define SGNC_POST_NOTIFICATION_OBJ(_NAME_,_OBJ_) (SGNotificationCenter::sharedNotificationCenter()->postNotification(_NAME_,_OBJ_))

class SGNotificationPost;
class SGNotificationObserver;

class CC_DLL SGNotificationCenter : public CCNode
{
public:
    static SGNotificationCenter *sharedNotificationCenter(void);
    static void purgeNotificationCenter(void);
    virtual ~SGNotificationCenter();
public:
    //添加监听
    void addObserver(const char *name, CCObject *target, SEL_CallFuncO selector, CCObject *obj=NULL);
    void addObserver(short msgId_, CCObject *target, SEL_CallFuncO selector, CCObject *obj=NULL);
    //移除监听
    void removeObserver(CCObject *target, const char *name);
    void removeObserver(CCObject *target, short msgId_);
    //移除所有该对象的监听
    void removeAllObserver(CCObject *target);
public:
    //通知命令，附带对象，是否立即发送（默认否）
    void postNotification(const char* name, CCObject* obj, bool noInterval=false);
    void postNotification(const char* name, bool noInterval=false);
public:
    //事件分发
    virtual void update(float dt);
    //等待通知中心对象处理
    void onMainNotification();
    //添加强制配对消息。
    void setForceMatchMsgId(const char* msgId);
protected:
    //observer集合
    std::vector<SGNotificationObserver*>* _observerVec;
//    CCArray *m_observers;
    //post集合
    std::vector<SGNotificationPost*>* _postVec;
    //临时observer众
    std::vector<SGNotificationObserver*>* _focusedObservers;
    //强制配对消息记录。
    std::set<std::string> _forceMatchMsgIdSet;
    //需要删除的
    //post锁
    pthread_mutex_t _postMutex;
    //observer锁
    pthread_mutex_t _observerMutex;
public:
    //是否发送数据
    bool isPostData;
protected:
    SGNotificationCenter();
    //检测对象是否符合要求
    bool observerExisted(CCObject *target, const char *name);
    
    //通知对象
    void sendNotification(const char *name, CCObject* object);
    
    //判断是否可以移除锁屏。
    bool checkShouldUnlock(const char* msgId);
};
//监听对象
class CC_DLL SGNotificationObserver : public CCObject {
public:
    SGNotificationObserver(CCObject *target,
                           SEL_CallFuncO selector,
                           const char *name,
                           CCObject *obj);
    ~SGNotificationObserver();
    
    void performSelector(CCObject *obj);
    
private:
    std::string m_name;
    CCObject* m_target;
    SEL_CallFuncO m_selector;
    CCObject* m_object;
    
public:
    const char* getName() const { return m_name.c_str(); }
    CCObject* getTarget() const { return m_target; }
    SEL_CallFuncO getSelector() const { return m_selector; }
    CCObject* getObject() const { return m_object; }
    CC_SYNTHESIZE(bool, m_markForDelete, MarkForDelete);
};
//发出对象
class CC_DLL SGNotificationPost : public CCObject
{
public:
    SGNotificationPost(const char* name, CCObject* object)
    {
        m_postName = name;
        m_postObject = object;
    }
    
    ~SGNotificationPost()
    {
//        CCLOG("~SGNotificationPost");
        CC_SAFE_DELETE(m_postObject);
    }
    
protected:
    std::string m_postName;
    CCObject* m_postObject;
    
public:
    const char* getPostName() const { return m_postName.c_str(); }
    CCObject* getPostObject() const { return m_postObject; }
    
};

#endif /* defined(__GameSanGuo__SG__) */
