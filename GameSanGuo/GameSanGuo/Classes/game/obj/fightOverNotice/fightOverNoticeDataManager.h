/*
 *战斗结束弹出提示数据表解析
 */


#ifndef __GameSanGuo__SGFightOverNoticeDataManager__
#define __GameSanGuo__SGFightOverNoticeDataManager__

#include "cocos2d.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

using namespace cocos2d;

struct FightOverNoticeData
{
    //字段意义请参见 fightOverNotice.xls
    int vipLev;
    int noticeType;
    int buttonA;
    int buttonB;
    int weight;
    int bgColor[3];
    string bgFileName;
    string noticeText;
    FightOverNoticeData()
    {
        memset(bgColor, 0, sizeof(bgColor));
    }
};

struct FightOverNoticeBtnData
{
    int btnId;
    int gotoId;
    string btnText;
};


class SGFightOverNoticeDataManager
{
public:
    ~SGFightOverNoticeDataManager();
    static SGFightOverNoticeDataManager * getInstance();
    vector<FightOverNoticeData *> getFightOverNoticeData(int noticeType);
    FightOverNoticeBtnData * getFightOverNoticeBtnData(int btnId);
private:
    SGFightOverNoticeDataManager();
    
    void formatData();
    
    CCDictionary * m_noticeDict;
    CCDictionary * m_btnDict;
    
    vector<FightOverNoticeData *> m_fightOverNoticeDataVec;
    vector<FightOverNoticeBtnData *> m_fightOverNoticeBtnDataVec;
    
    static SGFightOverNoticeDataManager * m_self;
};




#endif   //----------file end.