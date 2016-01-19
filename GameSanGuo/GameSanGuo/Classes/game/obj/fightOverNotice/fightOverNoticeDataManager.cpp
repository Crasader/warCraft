#include "fightOverNoticeDataManager.h"
#include "SGStaticDataManager.h"



SGFightOverNoticeDataManager * SGFightOverNoticeDataManager::m_self = NULL;

SGFightOverNoticeDataManager::SGFightOverNoticeDataManager()
: m_noticeDict(NULL)
, m_btnDict(NULL)
{
    
}

SGFightOverNoticeDataManager::~SGFightOverNoticeDataManager()
{
    for(int i=0; i<m_fightOverNoticeDataVec.size() ; ++i)
    {
        delete m_fightOverNoticeDataVec[i];
    }
    
    for(int i=0; i<m_fightOverNoticeBtnDataVec.size() ; ++i)
    {
        delete m_fightOverNoticeBtnDataVec[i];
    }
    
}

SGFightOverNoticeDataManager * SGFightOverNoticeDataManager::getInstance()
{
    if(m_self==NULL)
    {
        m_self = new SGFightOverNoticeDataManager();
        m_self->m_noticeDict = SGStaticDataManager::shareStatic()->getFightOverNoticeDict();
        m_self->m_btnDict = SGStaticDataManager::shareStatic()->getFightOverNoticeBtnDict();
        m_self->formatData();
    }

    return m_self;
}

void SGFightOverNoticeDataManager::formatData()
{
    CCDictElement * ele;
    CCDictionary  * row;
    FightOverNoticeData * fightOverNoticeData;
    FightOverNoticeBtnData * fightOverNoticeBtnData;
    CCDICT_FOREACH(m_noticeDict , ele)
    {
        row = (CCDictionary *)ele->getObject() ;
        
        fightOverNoticeData = new FightOverNoticeData;
        fightOverNoticeData->vipLev = ((CCString*)row->objectForKey("vipLevel"))->intValue();
        fightOverNoticeData->noticeType = ((CCString*)row->objectForKey("noticeType"))->intValue();
        fightOverNoticeData->buttonA = ((CCString*)row->objectForKey("buttonA"))->intValue();
        fightOverNoticeData->buttonB = ((CCString*)row->objectForKey("buttonB"))->intValue();
        fightOverNoticeData->weight = ((CCString*)row->objectForKey("weight"))->intValue();
        fightOverNoticeData->bgFileName = ((CCString*)row->objectForKey("showPng"))->m_sString;
        fightOverNoticeData->noticeText = ((CCString*)row->objectForKey("noticeType"))->m_sString;
        
        CCString * str = (CCString *)row->objectForKey("bgColor");
        if(str->compare("-1") == 0)
        {
            fightOverNoticeData->bgColor[0] = -1;
        }
        else
        {
            sscanf(str->getCString() ,"%d-%d-%d", fightOverNoticeData->bgColor , fightOverNoticeData->bgColor + 1 , fightOverNoticeData->bgColor + 2);
        }
        
        m_fightOverNoticeDataVec.push_back(fightOverNoticeData);
    }
    
    //按钮数据
    CCDICT_FOREACH(m_btnDict , ele)
    {
        row = (CCDictionary *)ele->getObject() ;
        
        fightOverNoticeBtnData = new FightOverNoticeBtnData;
        fightOverNoticeBtnData->btnId = ((CCString*)row->objectForKey("id"))->intValue();
        fightOverNoticeBtnData->btnText = ((CCString*)row->objectForKey("btnText"))->m_sString;
        fightOverNoticeBtnData->gotoId = ((CCString*)row->objectForKey("gotoWhere"))->intValue();
        
        m_fightOverNoticeBtnDataVec.push_back(fightOverNoticeBtnData);
    }

}

vector<FightOverNoticeData *> SGFightOverNoticeDataManager::getFightOverNoticeData(int noticeType)
{
    vector<FightOverNoticeData *> ret;
    int vipLev = SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel();
    if(!SGPlayerInfo::sharePlayerInfo()->getShowvip())
        vipLev = 12;
    for(int i =0 ; i<m_fightOverNoticeDataVec.size() ; i++)
    {
        if(noticeType == m_fightOverNoticeDataVec[i]->noticeType &&  vipLev == m_fightOverNoticeDataVec[i]->vipLev)
//        if(0==m_fightOverNoticeDataVec[i]->vipLev)
            ret.push_back(m_fightOverNoticeDataVec[i]);
    }
    return ret;
}

FightOverNoticeBtnData * SGFightOverNoticeDataManager::getFightOverNoticeBtnData(int btnId)
{
    if(0==btnId)
        return NULL;
    
    FightOverNoticeBtnData * ret=NULL;
    for(int i=0 ; i<m_fightOverNoticeBtnDataVec.size() ; i++)
    {
        if(btnId == m_fightOverNoticeBtnDataVec[i]->btnId)
            ret = m_fightOverNoticeBtnDataVec[i];
    }
    return ret;
}








//--------------------------file end.
