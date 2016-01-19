//
//  SGUpdateManager.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 8/20/14.
//
//

#include "SGUpdateManager.h"
#include "../../conn/http/SGHttpClient.h"
#include "HttpClientTool.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGShowString.h"
#include "SGUpdateClientBox.h"
#include "UpdateLayer.h"
#include "unzip.h"
#include <fstream>
#include "GlobalConfig.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
//cgp_mod
#include <unistd.h>
#endif
#include "GlobalConfig.h"

//文件名最大长度。
#define MAX_FILENAME 512

//解压缩缓冲区字节大小。
#define BUFFER_SIZE 8192

//最大综合尝试HTTP次数。
#define MAX_TRY_HTTP_TIME 10

//写入本地资源版本号OBS-TAG
#define OBS_TAG_WRITE_LOCAL_RES_VER "writeLocalResVer"

//写入大包组装完成OBS-TAG
#define OBS_TAG_WRITE_LOCAL_IS_BIG_READY_TRUE "writeLocalIsBigReadyTrue"

USING_NS_CC;

static SGUpdateManager* sgum_instance = NULL;

//ctor
SGUpdateManager::SGUpdateManager() : _localIsBigReady(false), _localIsRelaunchDone(false), _updateState(SGUM_UPDATE_RUNNING), _workThread(NULL), _doneResNum(0), _remainingResNum(0), _isRucShowed(false), _errorReqNum(0), _isFirstChecking(false), _isAlreadyTriggerd(false), _isUpdPosterRunning(false)
{
    ;
}

//dtor
SGUpdateManager::~SGUpdateManager()
{
    if (_workThread)
    {
//        pthread_cancel(_workThread);
    }
}

//single
SGUpdateManager* SGUpdateManager::gi()
{
    if (sgum_instance == NULL)
    {
        sgum_instance = new SGUpdateManager();
    }
    return sgum_instance;
}

//开启更新线程。
bool SGUpdateManager::start()
{
    SGNotificationCenter* sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(OBS_TAG_WRITE_LOCAL_RES_VER, this,  callfuncO_selector(SGUpdateManager::writeLocalResVer));
    sgnc->addObserver(OBS_TAG_WRITE_LOCAL_IS_BIG_READY_TRUE, this,  callfuncO_selector(SGUpdateManager::writeLocalIsBigReadyTrue));
    //检查本地配置。
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    std::string token = ccud->getStringForKey(SGUM_MICHAEL_JACKSON, "MJ");
    if (strcmp(token.c_str(), SGUM_MICHAEL_JACKSON) == 0) //已有config，直接使用之。
    {
        _localIsBigReady = ccud->getBoolForKey(SGUM_LOCAL_IS_BIG_READY_KEY);
        _localIsRelaunchDone = ccud->getBoolForKey(SGUM_LOCAL_IS_RELAUNCH_DONE_KEY);
        _localResVer = ccud->getStringForKey(SGUM_LOCAL_RES_VER_KEY);
        _localClientVer = BUNDLE_VERSION;
    }
    else //没有config，此时以自身信息写入。
    {
        GlobalConfig* ohgc = GlobalConfig::gi();
        _localIsBigReady = ohgc->getPackageVersion() ? true : false;
        _localIsRelaunchDone = ohgc->getPackageVersion() ? true : false;
        _localResVer = ohgc->getResourceVersion();
        _localClientVer = BUNDLE_VERSION;

        ccud->setBoolForKey(SGUM_LOCAL_IS_BIG_READY_KEY, _localIsBigReady);
        ccud->setBoolForKey(SGUM_LOCAL_IS_RELAUNCH_DONE_KEY, _localIsRelaunchDone);
        ccud->setStringForKey(SGUM_LOCAL_RES_VER_KEY, _localResVer);
        //!写入标识
        ccud->setStringForKey(SGUM_MICHAEL_JACKSON, SGUM_MICHAEL_JACKSON);
        ccud->flush();
    }
    
    
    _updateState = SGUM_UPDATE_RUNNING;
    _writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    
    //开启写解。
    int errCode = 0;
    do
    {
        pthread_attr_t tAttr;
        errCode = pthread_attr_init(&tAttr);
//        errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
        if ( 0 != errCode ) {
            pthread_attr_destroy(&tAttr);
            return false;
        }
        errCode = pthread_create(&_workThread, &tAttr, SGUpdateManager::writeAndUnzip, this);
    }
    while (0);
    
//    //MMDEBUG: TEMP 暂时设置true
//    _localIsBigReady = true;
//    _localResVer = "0";

    //启动游戏时候，若组装为大包的这个过程已经完成，则必须走更新UI。同时需要设置SGUM_LOCAL_IS_RELAUNCH_DONE_KEY为true，因为这时候APP必然已经完成过更新和重启。
    if (_localIsBigReady)
    {
        _localIsRelaunchDone = true;
    }
    //开启更新界面。
    this->switchUpdateLayer(true);
    //触发一次检定。
    _isFirstChecking = true;
    
    //需要跳过更新的话，将下面两行代码各自反向注释即可。
//    _updateState = SGUM_UPDATE_SUCCESS; //开启此，跳过更新。
    trigger(false); //注释此，跳过更新。
    
    return true;
}

//执行一次检定。
void SGUpdateManager::trigger(bool isContinue /*= true*/)
{
    _targetCheckUrl = CCString::createWithFormat("%s/resources/%d/%d/%s/%s/client", GlobalConfig::gi()->getAuthUrl().c_str(), GAME_CHANNEL_TYPE, GlobalConfig::gi()->getPackageVersion(), _localClientVer.c_str(), _localResVer.c_str())->m_sString;
    CCLOG("checkClientAndRes request=%s", _targetCheckUrl.c_str());
    HttpClientTool::sharedHttpClient()->HttpGet(this, _targetCheckUrl.c_str(), httpresponse_selector(SGUpdateManager::checkClientAndRes), "checkClientAndRes");
}

//检查http resp的有效性。
bool SGUpdateManager::verifyResponse(CCHttpResponse* resp)
{
    CCLOG("%s response state=%d", resp->getHttpRequest()->getTag(), resp->getResponseCode());
    if ( !resp || !resp->isSucceed() || !(resp->getResponseCode() == 200) )
    {
        //http的错误处理，由上层统一继续请求。
        return false;
    }
    else //ok
    {
        return true;
    }
}

//将http resp流读成string。
bool SGUpdateManager::parseResponse(CCHttpResponse* resp, std::string& buffer)
{
    if (verifyResponse(resp))
    {
        std::vector<char>* _vecbuf = resp->getResponseData();
        const char* s = &(*_vecbuf->begin());
        buffer = std::string(s, _vecbuf->size());
        if (buffer.length() <= 0)
        {
            CCLOG("%s error! buffer is empty. respones=%s", __FUNCTION__, resp->getHttpRequest()->getTag());
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        CCLOG("%s error! verifyResponse failed. respones=%s", __FUNCTION__, resp->getHttpRequest()->getTag());
        return false;
    }
}

//检查请求次数，返回true=可以继续，返回false=网络错误。
bool SGUpdateManager::examineRequest()
{
    _errorReqNum++;
    if (_errorReqNum > MAX_TRY_HTTP_TIME)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//把str按sep进行划分。
std::vector<std::string>* SGUpdateManager::splitString(const char* str, const char* sep)
{
    std::vector<std::string>* vec = new std::vector<std::string>();
    vec->reserve(5);
    char* newStr = const_cast<char*>(str);
	char *p = strtok(newStr, sep);
	while(p != NULL)
    {
        vec->push_back(std::string(p));
		p = strtok(NULL, sep);
	}
	return vec;
}

//检查Res和Client版本。
void SGUpdateManager::checkClientAndRes(CCHttpClient* client, CCHttpResponse* response)
{
    std::string buffer;
    if (!parseResponse(response, buffer))
    {
        if (examineRequest())
        {
            //!!!: 即刻重试。
            CCLOG("checkClientAndRes RETRY! request=%s", _targetCheckUrl.c_str());
            this->trigger(true);
            return;
        }
        else
        {
            CCLOG("SGUpdateClientBox=SGUCB_NETWORK_BUSY open!");
            SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_NETWORK_BUSY);
            SGMainManager::shareMain()->showBox(update);
            _isRucShowed = true;
            return;
        }
    }
    
    //vec = 1.0.0 | 2 | http:/resdir/list.csv | 11
    //      Client Res         CSV URL          REMAINING RES
    std::vector<std::string>* vec = splitString(buffer.c_str(), "\r\n");
    if (vec->size() < 4) //资源版本信息不全，请联系服务器人员。
    {
        CCLOG("SGUpdateClientBox=SGUCB_RESPONSE_ERR open!");
        SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_RESPONSE_ERR);
        SGMainManager::shareMain()->showBox(update);
        _isRucShowed = true;
        return;
    }
    else //解析Client和Res信息。
    {
        _targetClientVer = vec->at(0);
        _targetResVer = vec->at(1);
        _targetCsvUrl = vec->at(2);
        _remainingResNum = atoi(vec->at(3).c_str());
        //1号下载URL
        std::string clientUpdateUrl = vec->at(4).c_str();
        //设置客户端下载地址，fb分享需要使用。
        GlobalConfig::gi()->setClientUpdateUrl(clientUpdateUrl);
        //2号特殊URL
        std::string extStr = vec->at(5).c_str();
        //如果是台湾go2play的gp包，特殊处理，增加第三方渠道引流。
        if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY)
        {
            GlobalConfig::gi()->setTaiWanGpTo3rdLeadingUrl(extStr);
        }
        
        CCLOG("checkClientAndRes info: _localClientVer=%s, _localResVer=%s, _targetClientVer=%s, _targetResVer=%s, _targetCsvUrl=%s, _remainingResNum=%d, clientUpdateUrl=%s, extStr=%s", _localClientVer.c_str(), _localResVer.c_str(), _targetClientVer.c_str(), _targetResVer.c_str(), _targetCsvUrl.c_str(), _remainingResNum, clientUpdateUrl.c_str(), extStr.c_str());
        /*
         * 比较ClientVer，规则：
         * 第一位不同，必须更新。
         * 第二位不同，强制更新。
         * 第三位不同，建议更新。
         */
        
        std::string tempTargetStr(_targetClientVer.c_str());
        std::string tempLocalStr(_localClientVer.c_str());
        
        std::vector<std::string>* targetCVec = splitString(tempTargetStr.c_str(), "."); //1.0.0
        std::vector<std::string>* localCVec = splitString(tempLocalStr.c_str(), "."); //0.0.0
        
        if (strcmp(targetCVec->at(0).c_str(), localCVec->at(0).c_str()) == 0) //第一位通过。
        {
            if (strcmp(targetCVec->at(1).c_str(), localCVec->at(1).c_str()) == 0) //第二位通过，
            {
                if (strcmp(targetCVec->at(2).c_str(), localCVec->at(2).c_str()) == 0 || _isRucShowed) //第三位通过，或者已展示过建议更新ClientBox。
                {
                    //比较ResVer
                    if (strcmp(_localResVer.c_str(), _targetResVer.c_str()) == 0)
                    {
                        //XXX: post资源更新完成。
                        SGNotificationCenter::sharedNotificationCenter()->postNotification(OBS_TAG_WRITE_LOCAL_IS_BIG_READY_TRUE);
                    }
                    else
                    {
                        if (!_isUpdPosterRunning)
                        {
                            UpdateLayer* updLa = dynamic_cast<UpdateLayer*>(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_updateLayer));
                            if (updLa)
                            {
                                updLa->switchUpdFrame(true);
                            }
                            _isUpdPosterRunning = true;
                        }
//                    //首次检查判断大小包，小包直接放过，大包强制更新。
//                    if (_isFirstChecking)
//                    {
                        //启动游戏时候，若组装为大包的这个过程已经完成，则必须走更新UI。同时需要设置SGUM_LOCAL_IS_RELAUNCH_DONE_KEY为true，因为这时候APP必然已经完成过更新和重启。
                        if (_localIsBigReady)
                        {
                            CCLOG("updateLaunch=full package!");
                            _localIsRelaunchDone = true;
                            //MMDEBUG: UPDATE 更新启动
                            this->setIsAlreadyTriggeredTrue();
                            //准备下载CSV文件。
                            CCLOG("getDownloadList request=%s", _targetCsvUrl.c_str());
                            HttpClientTool::sharedHttpClient()->HttpGet(this, _targetCsvUrl.c_str(), httpresponse_selector(SGUpdateManager::getDownloadList), "getDownloadList");
                        }
                        else //MMDEBUG: NOTICE @非常重要!!如果未完成大包的组装，则直接进入游戏，后台会“推迟到”进入游戏主界面后继续更新，否则会阻塞登陆流程！
                        {
                            if (this->getIsAlreadyTriggeredTrue())
                            {
                                //准备下载CSV文件。
                                CCLOG("getDownloadList request=%s", _targetCsvUrl.c_str());
                                HttpClientTool::sharedHttpClient()->HttpGet(this, _targetCsvUrl.c_str(), httpresponse_selector(SGUpdateManager::getDownloadList), "getDownloadList");
                            }
                            else
                            {
                                CCLOG("autoLogin=mini package!");
                                UpdateLayer* updLa = dynamic_cast<UpdateLayer*>(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_updateLayer));
                                if (updLa)
                                {
                                    updLa->setUpdateProgress(100);
                                }
                                autoLogin();
                            }
                        }
//                    }
//                    else
//                    {
//                        //MMDEBUG: UPDATE 更新启动
//                        this->setIsAlreadyTriggeredTrue();
//                        //准备下载CSV文件。
//                        CCLOG("getDownloadList request=%s", _targetCsvUrl.c_str());
//                        HttpClientTool::sharedHttpClient()->HttpGet(this, _targetCsvUrl.c_str(), httpresponse_selector(SGUpdateManager::getDownloadList), "getDownloadList");
//                    }
                    }
                }
                else //建议更新。
                {
                    CCLOG("SGUpdateClientBox=SGUCB_RECOMMEND_UPDATE_CLIENT open!");
                    SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_RECOMMEND_UPDATE_CLIENT);
                    SGMainManager::shareMain()->showBox(update);
                    _isRucShowed = true;
                }
            }
            else //强制更新。
            {
                CCLOG("SGUpdateClientBox=SGUCB_FORCE_UPDATE_CLIENT open!");
                SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_FORCE_UPDATE_CLIENT);
                SGMainManager::shareMain()->showBox(update);
            }
            
        }
        else //必须更新。
        {
            CCLOG("SGUpdateClientBox=SGUCB_FORCE_UPDATE_CLIENT open!");
            SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_FORCE_UPDATE_CLIENT);
            SGMainManager::shareMain()->showBox(update);
        }
        
        _isFirstChecking = false;
        
        CC_SAFE_DELETE(localCVec);
        CC_SAFE_DELETE(targetCVec);
    }
    CC_SAFE_DELETE(vec);
}

//索要下载文件列表。
void SGUpdateManager::getDownloadList(CCHttpClient* client, CCHttpResponse* response)
{
    //temp: 修改提示文本為“系統更新中”
    UpdateLayer* updLa = dynamic_cast<UpdateLayer*>(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_updateLayer));
    if (updLa)
    {
        updLa->setLabelInfo(str_UpdateFail_str6);
    }
    
    std::string buffer;
    if (!parseResponse(response, buffer))
    {
        if (examineRequest())
        {
            //!!!: 即刻重试。
            CCLOG("getDownloadList RETRY! request=%s", _targetCsvUrl.c_str());
            HttpClientTool::sharedHttpClient()->HttpGet(this, _targetCsvUrl.c_str(), httpresponse_selector(SGUpdateManager::getDownloadList), "getDownloadList");
            return;
        }
        else
        {
            CCLOG("SGUpdateClientBox=SGUCB_NETWORK_BUSY open!");
            SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_NETWORK_BUSY);
            SGMainManager::shareMain()->showBox(update);
            _isRucShowed = true;
            return;
        }
    }
    
    //vec = 2.zip (现在改成每次只有一个zip，由服务器保证。）
    std::vector<std::string>* vec = splitString(buffer.c_str(), "\r\n");
    _targetZipName = vec->at(0);
    _targetZipUrl = _targetCsvUrl.substr(0, _targetCsvUrl.find_last_of("/") + 1).append(_targetZipName); //http:/resdir/2.zip
    //准备下载ZIP文件。
    CCLOG("downloadOneZip request=%s", _targetZipUrl.c_str());
    HttpClientTool::sharedHttpClient()->HttpGet(this, _targetZipUrl.c_str(), httpresponse_selector(SGUpdateManager::downloadOneZip), "downloadOneZip");
    CC_SAFE_DELETE(vec);
}

//正式下载并解压文件。
void SGUpdateManager::downloadOneZip(CCHttpClient* client, CCHttpResponse* response)
{
    _zipBuffer = new std::string();
    if (!parseResponse(response, *_zipBuffer))
    {
        CC_SAFE_DELETE(_zipBuffer);
        if (examineRequest())
        {
            //!!!: 即刻重试。
            CCLOG("downloadOneZip RETRY! request=%s", _targetZipUrl.c_str());
            HttpClientTool::sharedHttpClient()->HttpGet(this, _targetZipUrl.c_str(), httpresponse_selector(SGUpdateManager::downloadOneZip), "downloadOneZip");
            return;
        }
        else
        {
            CCLOG("SGUpdateClientBox=SGUCB_NETWORK_BUSY open!");
            SGUpdateClientBox *update = SGUpdateClientBox::create(SGUCB_NETWORK_BUSY);
            SGMainManager::shareMain()->showBox(update);
            _isRucShowed = true;
            return;
        }
    }
    
    //设置写解状态。
    _updateState = SGUM_UPDATE_WRITE_AND_UNZIP;
}

//写入与解压。
void* SGUpdateManager::writeAndUnzip(void* arg)
{
    SGUpdateManager* um = (SGUpdateManager*) arg;
    while (um->_updateState != SGUM_UPDATE_SUCCESS) //《直到更新的尽头》
    {
        //MMDEBUG: TEMP
        unsigned int time = 500000; //500ms
        if (um->_updateState == SGUM_UPDATE_RUNNING) //空转等待。
        {
            usleep(time);
        }
        else if (um->_updateState == SGUM_UPDATE_FIRST_DEPLOY) //首次部署。
        {
            usleep(time);
        }
        else //资源更新。SGUM_UPDATE_WRITE_AND_UNZIP
        {
            std::string path = um->_writablePath;
            path.append(um->_targetZipName);
            char* data = (char*) um->_zipBuffer->c_str();
            unsigned long length = um->_zipBuffer->size();
            
            bool wuDone = false;
            CCLOG("%s() begin, file=%s", __FUNCTION__, path.c_str());
            //写入
            if (um->writeFile(path.c_str(), data, length))
            {
                //解压
                if (um->unzipFile(path.c_str()))
                {
                    wuDone = true;
                    CCLOG("%s() done, file=%s", __FUNCTION__, path.c_str());
                }
                else
                {
                    CCLOG("%s() error! unzipFile failed. file=%s", __FUNCTION__, path.c_str());
                }
            }
            else
            {
                CCLOG("%s() error! writeFile failed. file=%s", __FUNCTION__, path.c_str());
            }
            CC_SAFE_DELETE(um->_zipBuffer);
            um->_updateState = SGUM_UPDATE_RUNNING;
            //检测本ZIP完成状况。
            if (wuDone)
            {
                //XXX: post单个ZIP更新完成。
                SGNotificationCenter::sharedNotificationCenter()->postNotification(OBS_TAG_WRITE_LOCAL_RES_VER);
            }
            else
            {
                um->trigger(true);
            }
        }
    }
    CCLOG("BANGKAI! UPDATE COMPLETED!");
    pthread_exit((void*)"update finished");
    return NULL;
}

//写入本地资源版本号，由游戏线程调用。
void SGUpdateManager::writeLocalResVer(CCObject* obj)
{
    //写入版本文件。
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    ccud->setStringForKey(SGUM_LOCAL_RES_VER_KEY, this->_targetResVer);
    ccud->flush();
    this->_doneResNum++;
    this->_localResVer = this->_targetResVer;
    CCLOG("%s() done, _localResVer=%s, _targetResVer=%s, _doneResNum=%d, _remainingResNum=%d", __FUNCTION__, this->_localResVer.c_str(), this->_targetResVer.c_str(), this->_doneResNum, this->_remainingResNum);
    //完成一次流水线请求，则错误数归0。
    _errorReqNum = 0;
    //request and check
    this->trigger(true);
}

//写入大包组装完成标识，由游戏线程调用。
void SGUpdateManager::writeLocalIsBigReadyTrue(CCObject* obj)
{
    //更新完成，同时写入大包组装完成的标识。
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    ccud->setBoolForKey(SGUM_LOCAL_IS_BIG_READY_KEY, true);
    ccud->flush();
    _updateState = SGUM_UPDATE_SUCCESS;
    //XXX: check whether this is correct.
    _localIsBigReady = true;
    CCLOG("%s() done, _localClientVer=%s, _localResVer=%s, _targetClientVer=%s, _targetResVer=%s", __FUNCTION__, _localClientVer.c_str(), _localResVer.c_str(), _targetClientVer.c_str(), _targetResVer.c_str());
    
    //some clean work
    SGNotificationCenter* sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->removeObserver(this, OBS_TAG_WRITE_LOCAL_RES_VER);
    sgnc->removeObserver(this, OBS_TAG_WRITE_LOCAL_IS_BIG_READY_TRUE);
}

bool SGUpdateManager::writeFile(const char* pszFileName, char* pData, unsigned long len)
{
    FILE *fpWrite = fopen(pszFileName, "wb");
	if (!fpWrite)
	{
        CCLOG("error in %s, can't open file '%s'", __FUNCTION__, pszFileName);
        return false;
    }
	
    fwrite(pData, 1, len, fpWrite);
    fclose(fpWrite);
    return true;
}

//解压文件。
bool SGUpdateManager::unzipFile(const char* pszFileName)
{
    // Open the zip file
    unzFile zipfile = unzOpen(pszFileName);
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", pszFileName);
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", pszFileName);
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        std::string fullPath = _writablePath + fileName;
        
        // 6 if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            createDirectory(fullPath.substr(0,fullPath.find_last_of('/')+1).c_str());
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    if (remove(pszFileName) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", pszFileName);
    }
    return true;
}

//创建文件夹。
bool SGUpdateManager::createDirectory(const char* path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

//开启或关闭更新UI，统一由本管理器控制。
void SGUpdateManager::switchUpdateLayer(bool isOpen)
{
    UpdateLayer* updLa = dynamic_cast<UpdateLayer*>(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_updateLayer));
    if (updLa)
    {
        updLa->removeFromParent();
    }
    if (isOpen)
    {
        UpdateLayer* updLa = UpdateLayer::create(true);
        SGMainManager::shareMain()->getMainScene()->addChild(updLa, 80);
    }
}

//自动登陆
void SGUpdateManager::autoLogin()
{
    CCLOG("SGUpdateManager::autoLogin()");
    SGStaticDataManager::shareStatic()->loadPlist();
    ResourceManager::sharedInstance()->bindCommonImageTexture();
    
    CCCallFunc *call = CCCallFunc::create(SGMainManager::shareMain()->getMainScene(), callfunc_selector(SGMainScene::welcome));
    CCDelayTime *delay = CCDelayTime::create(1.0f);
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    SGMainManager::shareMain()->getMainScene()->runAction(sequence);
}

//设置是否已经触发资源更新，这样除非更新完成，否则不会停止。
void SGUpdateManager::setIsAlreadyTriggeredTrue()
{
    this->_isAlreadyTriggerd = true;
}

//获得触发资源更新开始的状态。
bool SGUpdateManager::getIsAlreadyTriggeredTrue()
{
    return this->_isAlreadyTriggerd;
}

//获得更新进度的状态。
SGUM_UPDATE_STATE SGUpdateManager::getUpdateState()
{
    return this->_updateState;
}

//获得本地是否已完成重启。
bool SGUpdateManager::getLocalIsRelaunchDone()
{
    return this->_localIsRelaunchDone;
}

//获得本地是否已完成小包->大包组装。
bool SGUpdateManager::getLocalIsBigReady()
{
    return this->_localIsBigReady;
}

//需要更新的文件总数量。
int SGUpdateManager::getRemainingResNum()
{
    return this->_remainingResNum;
}

//已完成更新的文件数量。
int SGUpdateManager::getDoneResNum()
{
    return this->_doneResNum;
}
