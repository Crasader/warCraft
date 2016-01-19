//
//  SGPlayerInfo.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#include "SGTeamInfo.h"
//#include "SGStoryBean.h"
//#include "SGStorySceneBean.h"
#include "SGStaticDataManager.h"

static SGTeamInfo *s_TeamInfo = NULL;

SGTeamInfo::SGTeamInfo()
: teamId(0)
, lordId(0)
, iscurrent(0)
, soldierred(0)
, soldierblue(0)
, soldiergreen(0)
{
    for (int j= 0;j<POSITION;j++)
    {
        arry[j]=0;
    }
    
}


SGTeamInfo::~SGTeamInfo()
{
//    CC_SAFE_RELEASE(_soldierCards);
       
    
}


SGTeamInfo* SGTeamInfo::shareTeamInfo(void)
{
    if (!s_TeamInfo)
    {
        s_TeamInfo = new SGTeamInfo();
        for (int j= 0;j<POSITION;j++)
        {
            s_TeamInfo->arry[j]=0;
        }
        s_TeamInfo->setTeamId(-1);
        s_TeamInfo->setSoldierred(0);
        s_TeamInfo->setSoldiergreen(0);
        s_TeamInfo->setSoldierblue(0);
        s_TeamInfo->setLordId(0);
        s_TeamInfo->setIscurrent(-1);
    }
    return s_TeamInfo;
}

void SGTeamInfo::setEmbattleOfficers(int position,int value)
{
    if (arry[position] == value) {
        return;
    }

    arry[position]=value;
}

CCArray* SGTeamInfo::getEmbattleOfficers()
{
    CCArray *ary = CCArray::create();
    for (int i = 0; i<POSITION;i++) {
        if(arry[i]!=0)
        {
			SGOfficerCard *card = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(arry[i]);
            if (card !=NULL)
			{
                ary->addObject(card);
            }
        
        }
        else
            continue;
    }
    return ary;
}

int SGTeamInfo::getPositonId(SGOfficerCard *card)
{
    int id = card->getSsid();
    for (int i=0;i<POSITION;i++) {
        if (arry[i]==id) {
            return i+1;
        }
    }
    return 0;

}
//void SGTeamInfo::setteamId(int a,int value)
//{
//    teamId[a]=value;
//}
//
//int SGTeamInfo::getteamindex(int a)
//{
//    int i=0;
//    for (; i<TEAMID;i++) {
//        if (teamId[i]==a) {
//            break;
//        }
//    }
//    return i;
//}
//
//
//void SGTeamInfo::setlordId(int a, int value)
//{
//    majorId[a]=value;
//}
//
//int SGTeamInfo::getlordId(int a)
//{
//    int i = getteamindex(a);
//    return majorId[i];
//}
//
//void SGTeamInfo::setiscurrent(int a, int value)
//{
//    iscurrent[a]=value;
//}
//
//int SGTeamInfo::getiscurrent()
//{
//    int i=0;
//    for (;i<TEAMID;i++) {
//        if(iscurrent[i]== 1)
//            break;
//    }
//    return teamId[i];
//}

//void SGTeamInfo::setsoldierblue(int a, int value)
//{
//    soldierblue[a]=value;
//}
//
//int SGTeamInfo::getsoldierblue(int a)
//{
//   int i = getteamindex(a);
//    return soldierblue[i];
//}
//
//void SGTeamInfo::setsoldierred(int a, int value)
//{
//    soldierred[a]=value;
//}
//
//int SGTeamInfo::getsoldierred(int a)
//{
//   int i = getteamindex(a);
//    return soldierred[i];
//}
//
//void SGTeamInfo::setsoldiergreen(int a, int value)
//{
//    soldiergreen[a]=value;
//}
//
//int SGTeamInfo::getsoldiergreen(int a)
//{
//   int i = getteamindex(a);
//    return soldiergreen[i];
//}

int SGTeamInfo::isOneOfficer()
{
   int number = 10;
   for (int i=0;i<POSITION;i++) {
        if(arry[i]==0)
        {
            number--;
        }
        else
        {
            continue;
        }
    }
    if (number==1) {
        return 1;
    }
    else if (number==0)
    {
        return 0;
    }
    else
    {
        return 2;
    }
}
//
//int SGTeamInfo::embattleNumber()
//{
//    int is=TEAMID;
//    for (int i=0;i<TEAMID;i++) {
//       if( soldierblue[i]==0)
//       {
//           is--;
//           break;
//       }
//    }
//    return is;
//}
//
SGOfficerCard *SGTeamInfo::getlordofficer()
{
    SGOfficerCard *card=SGPlayerInfo::sharePlayerInfo()->getOfficerCard(lordId);
    if (card) {
        return card;
    }
    else{
        return NULL;
    }
    
}
//
bool SGTeamInfo::isOnEmbattle(const SGOfficerCard *card)
{
    int id = card->getSsid();
    for (int j= 0;j<POSITION; j++)
    {
            if (arry[j]==id)
            {
                return true;
            }
            else
                continue;
        
    }
    return false;
}
//
//CCArray *SGTeamInfo::getAllOfficercards()
//{
//    
//    CCArray *ary = CCArray::create();
//    for (int i = 0; i<TEAMID;i++) {
//        for (int j=0;j<POSITION; j++) {
//            if (arry[i][j]!=0) {
//                SGOfficerCard *card = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(arry[i][j]);
//                ary->addObject(card);
//            }
//            else
//                continue;
//        }
//        
//    }
//    return ary;
//}
