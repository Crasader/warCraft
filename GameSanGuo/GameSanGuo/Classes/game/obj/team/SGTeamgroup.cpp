//
//  SGPlayerInfo.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#include "SGTeamgroup.h"
//#include "SGStoryBean.h"
//#include "SGStorySceneBean.h"
#include "SGStaticDataManager.h"

static SGTeamgroup *s_TeamInfo = NULL;

SGTeamgroup::SGTeamgroup()
{
}


SGTeamgroup::~SGTeamgroup()
{
}


SGTeamgroup* SGTeamgroup::shareTeamgroupInfo()
{
    if (!s_TeamInfo)
    {
        s_TeamInfo = new SGTeamgroup();
    }
    return s_TeamInfo;
}

void SGTeamgroup::setEmbattleOfficers(int index, int position,int value)
{
    
   array[index].setEmbattleOfficers(position, value);
   
}

CCArray* SGTeamgroup::getEmbattleOfficers(int id)
{
    int index = getteamindex(id);
    CCArray *ary = array[index].getEmbattleOfficers();
    if (ary!=NULL) {
        return ary;
    }
    else
    {
        return NULL;
    }
}

void SGTeamgroup::setteamId(int a,int value)
{
    array[a].setTeamId(value);
}

int SGTeamgroup::getteamindex(int a)
{
    int i=0;
    int b = a-1;
    for (; i<TEAMID;i++) {
        if (array[i].getTeamId()==b)
        {
            break;
        }
    }
    return i;
}


void SGTeamgroup::setlordId(int a, int value)
{
    array[a].setLordId(value);
}

int SGTeamgroup::getlordId(int a)
{
    int i = getteamindex(a);
    return array[i].getLordId();
}

void SGTeamgroup::setiscurrent(int a, int value)
{
    array[a].setIscurrent(value);
}

int SGTeamgroup::getiscurrent()
{
    int i=0;
    for (;i<TEAMID;i++)
    {
        if(array[i].getIscurrent()== 1)
            break;
    }
    if (i>=TEAMID) {
        i=0;
    }
    int x = (array[i].getTeamId())+1;
    return x;
}

void SGTeamgroup::setsoldierblue(int a, int value)
{
    array[a].setSoldierblue(value);
}

int SGTeamgroup::getsoldierblue(int a)
{
   int i = getteamindex(a);
    return array[i].getSoldierblue();
}

void SGTeamgroup::setsoldierred(int a, int value)
{
    array[a].setSoldierred(value);
}

int SGTeamgroup::getsoldierred(int a)
{
   int i = getteamindex(a);
    return array[i].getSoldierred();
}

void SGTeamgroup::setsoldiergreen(int a, int value)
{
    array[a].setSoldiergreen(value);
}

int SGTeamgroup::getsoldiergreen(int a)
{
   int i = getteamindex(a);
    return array[i].getSoldiergreen();
}

int SGTeamgroup::isOneOfficer(int id)
{
    
    int index=getteamindex(id);
    if (array[index].isOneOfficer()==1) {
        return 1;
    }
    else if(array[index].isOneOfficer()==0)
    {
        return 0;
    }
    else
    {
        return 2;
    }
}

int SGTeamgroup::embattleNumber()
{
    int is=TEAMID;
    for (int i=0;i<TEAMID;i++) {
       if( array[i].isOneOfficer()==0)
       {
           is--;
       }
        else
            continue;
    }
    return is;
}

SGOfficerCard *SGTeamgroup::getlordofficer(int teamid)
{
   int i = getteamindex(teamid);
    return array[i].getlordofficer();
}

int SGTeamgroup::isOnEmbattle(const SGOfficerCard *card)
{
   int currTeam = this->getiscurrent();
   int nIdTeam = 0;
   for (int i=0;i<TEAMID;i++) {
        if (array[i].isOnEmbattle(card)) {
            nIdTeam = array[i].getTeamId()+1;
            if(nIdTeam == currTeam)
               return nIdTeam;
        }
        else
            continue;
    }
    return nIdTeam;
}

CCArray *SGTeamgroup::getAllOfficercards()
{
    
    CCArray *ary = CCArray::create();
    for (int i=0;i<TEAMID;i++)
    {
        CCArray *ary1 = array[i].getEmbattleOfficers();
        if (ary1!=NULL)
        {
            for (int j=0;j<(ary1->count()); j++)
            {
                ary->addObject(ary1->objectAtIndex(j));

            }
            
        }
        else
            continue;
    }
    
    return ary;
}


int SGTeamgroup::getPositionId(int a,SGOfficerCard *card)
{
    int index = getteamindex(a);
   return  array[index].getPositonId(card);

}

int SGTeamgroup::islord(int a,SGOfficerCard *card)
{
    int index = getteamindex(a);
    int id = card->getSsid();
    if (array[index].getLordId()==id) {
        return 1;
    }
    return 0;
}

CCArray* SGTeamgroup::showoffofficercards(int id)
{
    int index = getteamindex(id);
    
    CCArray* a1 = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    CCArray *a2 = CCArray::create();
    for (int i = 0;i<a1->count();i++)
    {
        int sid = ((SGOfficerCard*)a1->objectAtIndex(i))->getSsid();
        int d =0;
        for (int j = 0; j<POSITION;j++)
        {
            if (array[index].arry[j]==sid)
            {
                d=1;
            }
            
        }
        if (d==0) {
            a2->addObject(a1->objectAtIndex(i));
        }
    }
    return a2;
}

void SGTeamgroup::changelord(int teamid,SGOfficerCard *card)
{
    int id = card->getSsid();
    int index = getteamindex(teamid);
    array[index].setLordId(id);
}