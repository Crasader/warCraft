//
//  SGUpdateManager.h
//  GameSanGuo
//
//  Created by Fygo Woo on 8/20/14.
//
//

#ifndef __GameSanGuo__SGUpdateManager__
#define __GameSanGuo__SGUpdateManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <stdio.h>
#include "pthread.h"

//判定是否已有本地配置记录的key。value=string
#define SGUM_MICHAEL_JACKSON "sgum_michael_jackson_local"

//是否完成大包组装后的首次重启，防止多次重启。
#define SGUM_LOCAL_IS_RELAUNCH_DONE_KEY "sgum_local_is_relaunch_done_key"

//是否完成大包组装的key。value=bool
#define SGUM_LOCAL_IS_BIG_READY_KEY "sgum_local_is_big_ready"

//本地资源版本的key。value=string
#define SGUM_LOCAL_RES_VER_KEY "sgum_local_res_ver"

//本地客户端版本的key。value=string
#define SGUM_LOCAL_CLIENT_VER_KEY "sgum_local_client_ver"

using cocos2d::CCObject;
using cocos2d::extension::CCHttpClient;
using cocos2d::extension::CCHttpResponse;

//资源更新管理器当前的状态
enum SGUM_UPDATE_STATE
{
    SGUM_UPDATE_SUCCESS, //完成。
    SGUM_UPDATE_WRITE_AND_UNZIP, //写入&解压。
    SGUM_UPDATE_FIRST_DEPLOY, //首次解压缩。（暂定抛弃首次解压缩这个状态，字段保留。）
    SGUM_UPDATE_RUNNING, //空转。
};

/**
 * 主要用于资源的更新，包含由两大方面，①小包到大包更新，②标准流程更新。如果下载过程中有错误出现，处理方式是继续请求，《直到世界的尽头》。
 * 注：客户端的更新提示其实也在这里进行，如果客户端的版本号过期，则会提示去下载新端，则与资源更新就没有关系了。
 * Res=资源，Client=客户端。
 */
class SGUpdateManager : public cocos2d::CCObject
{
//functions
public:
    //ctor
    SGUpdateManager();
    
    //dtor
    ~SGUpdateManager();
    
    //single
    static SGUpdateManager* gi();
    
    //开启更新线程。
    bool start();
    
    //执行一次检定。isContinue表示是否继续下载，用来标示首次Client和Res版本。
    void trigger(bool isContinue = true);
    
    //检查Client和Res版本。
    void checkClientAndRes(CCHttpClient* client, CCHttpResponse* response);
    
    //索要下载文件列表。
    void getDownloadList(CCHttpClient* client, CCHttpResponse* response);
    
    //下载一个压缩包。
    void downloadOneZip(CCHttpClient* client, CCHttpResponse* response);
    
    //写入本地资源版本号，由游戏线程调用。
    void writeLocalResVer(CCObject* obj);
    
    //写入大包组装完成标识，由游戏线程调用。
    void writeLocalIsBigReadyTrue(CCObject* obj);
    
    //获得更新进度的状态。
    SGUM_UPDATE_STATE getUpdateState();
    
    //获得本地是否已完成重启。
    bool getLocalIsRelaunchDone();
    
    //获得本地是否已完成小包->大包组装。
    bool getLocalIsBigReady();
    
    //剩余需要更新的文件数量。
    int getRemainingResNum();

    //已经完成更新的文件数量。
    int getDoneResNum();
    
    //开启或关闭更新UI，统一由本管理器控制。
    void switchUpdateLayer(bool isOpen);
    
    //进入游戏。
    void autoLogin();
    
    //获得触发资源更新开始的状态。
    bool getIsAlreadyTriggeredTrue();
    
    //触发资源更新开始，除非更新完成或者6次出错，否则不会停止。
    void setIsAlreadyTriggeredTrue();
    
    //@T 写入与解压。
    static void* writeAndUnzip(void* arg);
    
private:
    //检查http resp的有效性。
    bool verifyResponse(CCHttpResponse* resp);
    
    //将http resp流读成string。
    bool parseResponse(CCHttpResponse* resp, std::string& buffer);
    
    //检查请求次数，返回true=可以继续，返回false=网络错误。
    bool examineRequest();
    
    //把str按sep进行划分。
    std::vector<std::string>* splitString(const char* str, const char* sep);
    
    //写入文件。
    bool writeFile(const char* pszFileName, char* pData, unsigned long len);
    
    //解压文件。
    bool unzipFile(const char* pszFileName);
    
    //创建文件夹。
    bool createDirectory(const char* path);
    
//fields
private:
    //工作线程。
    pthread_t _workThread;
    
    //本地读写目录。
    std::string _writablePath;
    
    //是否已经trigger防止反复的trigger。
    bool _isAlreadyTriggerd;
    
    //是否进行了首次检查。
    bool _isFirstChecking;
    
    //是否已经开启UPDATELAYER的poster。
    bool _isUpdPosterRunning;
    
    //管理器当前的状态。
    SGUM_UPDATE_STATE _updateState;
    
    //一个流水线资源请求中错误累计的次数，超过6次则弹窗提示网络错误。
    int _errorReqNum;
    
    //建议更新client的弹窗是否已展示过的标志。如果玩家选择不更新client，则此为true，同时继续资源更新的标准流程。
    bool _isRucShowed;
    
    //本地是否已完成重启。
    bool _localIsRelaunchDone;
    
    //本地是否已完成小包->大包。
    bool _localIsBigReady;
    
    //本地资源版本号。
    std::string _localResVer;
    
    //本地客户端版本号。
    std::string _localClientVer;

    //本次Res更新的目标版本。
    std::string _targetResVer;
    
    //本次Client更新的目标版本。
    std::string _targetClientVer;
    
    //本次Res更新的检查URL。（形如"http:/resdir/0/1.0.0/0.0.1/client"，其中0=大小包标识，1.0.0=客户端版本，0.0.1=资源版本。用于检查客户端和服务端的版本匹配。）
    std::string _targetCheckUrl;
    
    //本次Res更新的列表URL。（形如“http:/resdir/list.csv”，其中list.csv文件里包含“0.0.2.zip”这样的文件名，最终下载地址则是去除list.csv之后的拼接，即"http:/resdir/list.csv"->"http:/resdir/0.0.2.zip"。）
    std::string _targetCsvUrl;
    
    //本次Res更新的文件URL。（形如“http:/resdir/0.0.2.zip”。）
    std::string _targetZipUrl;
    
    //本次Res更新的文件Name。（形如“0.0.2.zip"。）
    std::string _targetZipName;
    
    //本次Res剩余需要更新的文件数量。
    int _remainingResNum;
    
    //本次Res已经完成更新的文件数量。
    int _doneResNum;
    
    //本次下载ZIP文件的字节流，可能会比较大，需要存储。
    std::string* _zipBuffer;
};

#endif /* defined(__GameSanGuo__SGUpdateManager__) */
