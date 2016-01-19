//
//  SGAIHero.cpp
//  GameSanGuo
//
//  Created by geng on 13-3-29.
//
//

#include "SGAIHero.h"
#include "SGHeroLayer.h"
#include "SGSkillManager.h"
#include "SGAttackList.h"
#include "PlatformAdp.h"

SGAIHero *  SGAIHero::m_SsgAiHero = NULL;
void SGAIHero::readSkillAIConfig()
{
	//解析问题现在交给数据包。
//    std::string filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename("skillfuzzy.xml");
//#if (PLATFORM == IOS)
//    //xmlDoc
//    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
//    
//    int i = pDoc->LoadFile(filePath.c_str());
//	
//#else
//	//android 下使用tinyxml解析会出现问题
//    unsigned long size;
//    char *pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str() , "r", &size);
//    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
//    int i = pDoc->Parse(pFileContent);
//	
//#endif
    
    tinyxml2::XMLDocument* pDoc = SGStaticDataManager::shareStatic()->getSkillFuzzyXml();

    //得到根节点
    tinyxml2::XMLElement *rootEle = pDoc->RootElement();
    
    //打印节点的
    //CCLog("%s",rootEle->GetText());
    
    tinyxml2::XMLElement *childNode = rootEle->FirstChildElement();
    while(childNode)
    {
        childNode->GetText();
        //CCLog("%s",childNode->GetText());
        
        if(!strcmp(childNode->Name(),"SkillClass"))
        {
            //CCLog("%s",childNode->Name());
            
            int id  = 0;
            // childNode->QueryFloatAttribute("id", &value);
            childNode->QueryIntAttribute("id",&id);
            const char *  str = childNode->Attribute("name");
            
            //CCLog("%s",str);
           // CCLog("%d",id);
            
            
            FuzzyModule *fuzzy = new FuzzyModule() ;
            std::map<std::string, FzSet> mapset;
            tinyxml2::XMLElement *childElement= childNode->FirstChildElement();
            //读取模糊变量
            while(childElement)
            {
                if(!strcmp(childElement->Name(),"FuzzyVar"))
                {
                    FuzzyVariable &var = fuzzy->CreateFLV(std::string(childElement->Attribute("name")));
                    tinyxml2::XMLElement * setNode = childElement->FirstChildElement();
                    while(setNode)
                    {
                        const char* setType = setNode->Attribute("setType");
                        double minBound,peak,maxBound;
                        setNode->QueryDoubleAttribute("minBound", &minBound);
                        setNode->QueryDoubleAttribute("peak", &peak);
                        setNode->QueryDoubleAttribute("maxBound", &maxBound);
                        const char * name = setNode->Attribute("name");
                        
                       // CCLog("%s",name);
                        if(!strcmp(setType,"left"))
                        {
                            
                            mapset.insert(std::map<std::string, FzSet>::value_type(name,var.AddLeftShoulderSet(std::string(name), minBound,peak,maxBound)));
                        }
                        else if(!strcmp(setType,"middle"))
                        {
                            mapset.insert(std::map<std::string, FzSet>::value_type(name,var.AddTriangularSet(std::string(name), minBound,peak,maxBound)));
                        }
                        else if(!strcmp(setType,"right"))
                        {
                            mapset.insert(std::map<std::string, FzSet>::value_type(name,var.AddRightShoulderSet(std::string(name), minBound,peak,maxBound)));
                        }
                        setNode = setNode->NextSiblingElement();
                    }
                    
                }
                childElement = childElement->NextSiblingElement();
                
            }
            childElement= childNode->FirstChildElement();
            //读取模糊规则
            
            while(childElement)
            {
                if(!strcmp(childElement->Name(),"FuzzyRule"))
                {
                    
                    const char * strant1,*strant2;
                    tinyxml2::XMLElement * AntecedentEle = childElement->FirstChildElement();
                    int typeAnt= 0;
                    AntecedentEle->QueryIntAttribute("type", &typeAnt);
                    if (typeAnt==0) {
                        
                        tinyxml2::XMLElement * ant1 = AntecedentEle->FirstChildElement();
                        strant1 =  ant1->Attribute("name");
                        
                    }
                    
                    else{
                        tinyxml2::XMLElement * ant1 = AntecedentEle->FirstChildElement();
                        strant1 =  ant1->Attribute("name");
                        tinyxml2::XMLElement * ant2 = AntecedentEle->LastChildElement();
                        strant2=  ant2->Attribute("name");
                        
                    }
                    
                    tinyxml2::XMLElement * Consequence = childElement->LastChildElement();
                    int typeConse=0;
                    Consequence->QueryIntAttribute("type", &typeConse);
                    
                    const char * strConse = Consequence->Attribute("value");
                    if(typeAnt==0&&typeConse==0)
                    {
                        fuzzy->AddRule(mapset.find(strant1)->second, mapset.find(strConse)->second);
                    }else if(typeAnt==0&&typeConse==1)
                    {
                        FzSet fz1 =  mapset.find(strConse)->second;
                        FzFairly ff= FzFairly(fz1) ;
                        fuzzy->AddRule(mapset.find(strant1)->second, ff);
                    }
                    else if(typeAnt==0&&typeConse==2)
                    {
                        FzSet fz1 =  mapset.find(strConse)->second;
                        FzVery fv = FzVery(fz1) ;
                        fuzzy->AddRule(mapset.find(strant1)->second, fv);
                    }
                    else if(typeAnt==1&&typeConse==0)
                    {
                        FzAND fand= FzAND(mapset.find(strant1)->second,mapset.find(strant2)->second);
                        //FzSet fz1 =  mapset.find(strConse)->second;
                        //FzVery fv = FzVery(fz1) ;
                        fuzzy->AddRule(fand, mapset.find(strConse)->second);
                    }
                    else if(typeAnt==1&&typeConse==1)
                    {
                        FzAND fand= FzAND(mapset.find(strant1)->second,mapset.find(strant2)->second);
                        FzSet fz1 =  mapset.find(strConse)->second;
                        FzFairly ff = FzFairly(fz1) ;
                        fuzzy->AddRule(fand, ff);
                    }
                    else if(typeAnt==1&&typeConse==2)
                    {
                        FzAND fand= FzAND(mapset.find(strant1)->second,mapset.find(strant2)->second);
                        FzSet fz1 =  mapset.find(strConse)->second;
                        FzVery fv = FzVery(fz1) ;
                        fuzzy->AddRule(fand, fv);
                    }
                    else if(typeAnt==2&&typeConse==0)
                    {
                        FzOR fo= FzOR(mapset.find(strant1)->second,mapset.find(strant2)->second);
                        //FzSet fz1 =  mapset.find(strConse)->second;
                        //FzVery fv = FzVery(fz1) ;
                        fuzzy->AddRule(fo, mapset.find(strConse)->second);
                    }
                    else if(typeAnt==2&&typeConse==1)
                    {
                        FzOR fo= FzOR(mapset.find(strant1)->second,mapset.find(strant2)->second);
                        FzSet fz1 =  mapset.find(strConse)->second;
                        FzFairly ff = FzFairly(fz1) ;
                        fuzzy->AddRule(fo, ff);
                    }
                    else if(typeAnt==2&&typeConse==2)
                    {
                        FzOR fo= FzOR(mapset.find(strant1)->second,mapset.find(strant2)->second);
                        FzSet fz1 =  mapset.find(strConse)->second;
                        FzVery fv = FzVery(fz1) ;
                        fuzzy->AddRule(fo, fv);
                    }
                    
                    
                    
                    
                }
                
                childElement = childElement->NextSiblingElement();
                
            }
            
            m_mapFuzzy.insert(std::map<int, FuzzyModule*>::value_type(id,fuzzy));
            //mapfuzzy[id] = fuzzy;
            
        }
        childNode = childNode->NextSiblingElement();
    }
    delete pDoc;
    
    

}
bool SGAIHero::startSkill(SGBattleMap*bm, int skillId)
{
    CCLOG("skillId:%d",skillId);
    switch (skillId)
    {
        case skill_id_20000:
        {//貂蝉
            int num4 = 0;
            int num2 = 0;
            int num3 = 0;
            CCArray *al = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(al, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                switch (al->getAttackType())
                {
                    case kattacknormal:
                    {
                        num3++;
                    }
                        break;
                    case kattacktwo:
                    {
                        num2++;
                    }
                        break;
                    case kattackfour:
                    {
                        num4++;
                    }
                    default:
                        break;
                }
            }
            CCLOG("num3:%d",num3);
            CCLOG("num2:%d",num2);
            CCLOG("num4:%d",num4);

            if ((num3+num2*2+num4*4)>4)
            {
                return true;
            }
        }
            break;
        case skill_id_20002:
        case skill_id_20073:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            int num3 = 0;
            CCArray *al = sbm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(al, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                switch (al->getAttackType())
                {
                    case kattacknormal:
                    {
                        num3++;
                    }
                        break;
                        
                    default:
                        break;
                }
            }
            if (num3>2)
            {
                return true;
            }
        }
            break;
        case skill_id_20004:
        case skill_id_20075:
        {
            return true;
        }
            break;
        case skill_id_20005:
        case skill_id_20076:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            for (int i = 0; i<mapRow; i++)
            {
                for (int j = 0; j<mapList; j++)
                {
                    SGGridBase *grid = sbm->myGrids[i][j];
                    if (grid && grid->getStype() == knormal)
                    {
                        if (grid->getSbType() == ksbtwo)
                        {
                            return true;
                        }
                        if (skillId == skill_id_20076 && grid->getSbType() == ksbfour)
                        {
                            return true;
                        }
                    }
                }
            }
        }
            break;
        case skill_id_20007://空城计
        case skill_id_20078:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            int maxnum = heroLayer->max_soldierNum;//最大兵数量
            int soldier =  heroLayer->getSoldier();//现在地图上的兵的数量,武将与士兵有一个换算
            int left = maxnum-soldier;//没有兵的数量
            if (left<12)
            {
                return true;
            }
            if (skillId ==skill_id_20007 && left<16)
            {
                return true;
            }
        }
            break;
        case skill_id_20008:
        case skill_id_20009:
        case skill_id_20079:
        case skill_id_20094:
        {//回血技能   青囊  妙手回春 治疗术
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            float leftrate = (heroLayer->getBlood())/(heroLayer->heroBloodMax*1.0f);
            CCLOG("leftrate:%f",leftrate);
            
            int Id = 3;
            m_mapFuzzy[Id]->Fuzzify("Life", (double)clampf(leftrate*100,0,100));
            
            
            int score = m_mapFuzzy[Id]->DeFuzzify("Desirability",FuzzyModule::max_av);
            CCLog("desirabel_score************%d",score);
            if(score>=desirable_score)
                return  true;
            /*if (leftrate>=0.05 && skillId == skill_id_20079)
            {
                return true;
            }
            if (leftrate>=0.1 && skillId == skill_id_20009)
            {
                return true;
            }
            if (leftrate>=0.2 && skillId == skill_id_20008)
            {
                return true;
            }
            return true;*/
            
        }
            break;
        case skill_id_20010:
        case skill_id_20011:
        {
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getLRType() == lr_l || al->getLRType() == lr_r || al->getLRType() == lr_lr)
                {
                    return true;
                }
            }
        }
            break;
        case skill_id_20012:
        {
            bool my4 = false;
            bool his4 = false;
            for (int i = 0; i<mapRow; i++)
            {
                for (int j = 0; j<mapList; j++)
                {
                    SGGridBase *grid = bm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbfour)
                    {
                        my4 = true;
                        break;
                    }
                }
                if (my4)
                {
                    break;
                }
            }
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            for (int i = 0; i<mapRow; i++)
            {
                for (int j = 0; j<mapList; j++)
                {
                    SGGridBase *grid = sbm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbfour)
                    {
                        his4 = true;
                        break;
                    }
                }
                if (his4)
                {
                    break;
                }
            }
            if (his4 && !my4)
            {
                return true;
            }
        }
            break;
        case skill_id_20013:
        {
            bool my2 = false;
            bool his2 = false;
            for (int i = 0; i<mapRow; i++)
            {
                for (int j = 0; j<mapList; j++)
                {
                    SGGridBase *grid = bm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbtwo)
                    {
                        my2 = true;
                        break;
                    }
                }
                if (my2)
                {
                    break;
                }
            }
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            for (int i = 0; i<mapRow; i++)
            {
                for (int j = 0; j<mapList; j++)
                {
                    SGGridBase *grid = sbm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbtwo)
                    {
                        his2 = true;
                        break;
                    }
                }
                if (his2)
                {
                    break;
                }
            }
            if (my2 && !his2)
            {
                return true;
            }
        }
            break;
        case skill_id_20014:
        case skill_id_20015:
        case skill_id_20080:
        {//苦肉计,破釜沉舟,搏命
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            float rate = heroLayer->getBlood()/(heroLayer->heroBloodMax*1.0f);
            int Id = 1;
            //CCDirector::sharedDirector()->end();
            //mapfuzzy[Id]->Fuzzify("Life", (double)2);
            //mapfuzzy[Id]->Fuzzify("Damage_opp", (double)4);
            CCArray *als = bm->getAttackList();
            int sum = als->count();
            
            m_mapFuzzy[Id]->Fuzzify("Life", (double)clampf(rate*100,0,100));
            m_mapFuzzy[Id]->Fuzzify("Queue_Self", (double)clampf((float)sum,0,12));
            
            int score = m_mapFuzzy[Id]->DeFuzzify("Desirability",FuzzyModule::max_av);
            CCLog("desirabel_score************%d",score);
            if(score>=desirable_score)
                return  true;
            /*
            if (rate>0.8 && skillId == skill_id_20080)
            {
                return true;
            }
            if (rate>0.75 && skillId == skill_id_20015)
            {
                return true;
            }
            if (rate>0.7 && skillId  == skill_id_20014)
            {
                return true;
            }*/
        }
            break;
        case skill_id_20016:
        case skill_id_20017:
        case skill_id_20081:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            int num3 = 0;
            for (int i = 0; i < mapRow; i++)
            {
                for (int j = 0; j < mapList; j++)
                {
                    SGGridBase *grid = sbm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbone && grid->getStype() == knormal)
                    {
                        num3++;
                    }
                }
            }
            if (num3>8 && skillId == skill_id_20016)
            {
                return true;
            }
            if (num3>5 && skillId == skill_id_20017)
            {
                return true;
            }
            if (num3>3 && skillId == skill_id_20081)
            {
                return true;
            }
            
        }
            break;
        case skill_id_20018:
        case skill_id_20019:
        case skill_id_20082:
        case skill_id_20097:
        {//火弹,炎爆术,火烧连营
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            int numd = 0;
            for (int i = 0; i < mapRow; i++)
            {
                for (int j = 0; j < mapList; j++)
                {
                    SGGridBase *grid = sbm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbone && grid->getStype() == kdefend)
                    {
                        numd++;
                    }
                }
            }
            int Id = 7;
           // m_mapFuzzy[Id]->Fuzzify("Life", (double)clampf(rate*100,0,100));
            m_mapFuzzy[Id]->Fuzzify("Wall_opp", (double)clampf((float)numd,0,12));
            
            int score = m_mapFuzzy[Id]->DeFuzzify("Desirability",FuzzyModule::max_av);
            CCLog("desirabel_score************%d   %d",score,numd);
            if(score>=desirable_score)
                return  true;

        }
            break;
        case skill_id_20020:
        case skill_id_20021:
        case skill_id_20083:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            if (sbm->getAttackList()->count()>=2)
            {
                return true;
            }
        }
            break;
        case skill_id_20024:
        {
            
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            if (heroLayer->getRound()<3)
            {
                return false;
            }

            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattackfour)
                {
                    return true;
                }
            }
        }
            break;
        case skill_id_20025:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            if (heroLayer->getRound()<2)
            {
                return false;
            }
            
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattackfour)
                {
                    return true;
                }
            }
        }
            break;
        case skill_id_20085:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            if (heroLayer->getRound()<1)
            {
                return false;
            }
            
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattackfour)
                {
                    return true;
                }
            }
        }
            break;
        case skill_id_20028:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            float rate = heroLayer->getBlood()/(heroLayer->heroBloodMax*1.0f);
            if (rate<0.75)
            {
                return false;
            }
    
            int num4= 0;
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattackfour)
                {
                    num4++;
                }
            }
            if (num4>1 ||(bm->getAttackList()->count()>=2))
            {
                return true;
            }
        }
            break;
        case skill_id_20029:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            float rate = heroLayer->getBlood()/(heroLayer->heroBloodMax*1.0f);
            if (rate<0.70)
            {
                return false;
            }
            
            int num4= 0;
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattackfour)
                {
                    num4++;
                }
            }
            if (num4>1 ||(bm->getAttackList()->count()>=2))
            {
                return true;
            }
        }
            break;
        case skill_id_20087:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            float rate = heroLayer->getBlood()/(heroLayer->heroBloodMax*1.0f);
            if (rate<0.65)
            {
                return false;
            }
            int num4= 0;
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattackfour)
                {
                    num4++;
                }
            }
            if (num4>1 ||(bm->getAttackList()->count()>=2))
            {
                return true;
            }
        }
            break;
        case skill_id_20030:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            CCArray *als = sbm->getAttackList();
            int l = 0;
            int r = 0;
            int lr = 0;
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getLRType() == lr_l)
                {
                    l++;
                }
                if (al->getLRType() == lr_r)
                {
                    r++;
                }
                if (al->getLRType() == lr_lr)
                {
                    lr++;
                }
            }
            if ((l>0&& r>0) || lr>0)
            {
                return true;
            }
        }
            break;
        case skill_id_20031:
        case skill_id_20088:    
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            CCArray *als = sbm->getAttackList();

            int r = 0;
            int lr = 0;
            CCObject *obj = NULL;
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                
                if (al->getLRType() == lr_r)
                {
                    r++;
                }
                if (al->getLRType() == lr_lr)
                {
                    lr++;
                }
            }
            if (r>0 || lr>0)
            {
                return true;
            }
        }
            break;
        case skill_id_20032:
        case skill_id_20071:
        case skill_id_20093:
        {
            int no4 = 0;
            int num4 = 0;
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getAttackType() == kattacknormal || al->getAttackType() == kattacktwo)
                {
                    no4++;
                }
                if (al->getAttackType() == kattackfour)
                {
                    num4++;
                }
            }
            int sum = no4+num4*4;
            int Id = 2;
            m_mapFuzzy[Id]->Fuzzify("Queue_Self", (double)clampf((float)sum,0,6));
            
            int score = m_mapFuzzy[Id]->DeFuzzify("Desirability",FuzzyModule::max_av);
            CCLog("desirabel_score************%d",score);
            if(score>=desirable_score)
                return  true;
        }
            break;
        case skill_id_20033:
        case skill_id_20089:
        case skill_id_20054:
        case skill_id_20055:
        {

            int num2 = 0;
            CCArray *als = bm->getAttackList();
            CCObject *obj = NULL;
            
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
               
                if (al->getAttackType() == kattacktwo)
                {
                    num2++;
                }
            }
            if (num2>0)
            {
                return true;
            }
        }
            break;
        case skill_id_20036:
        case skill_id_20037:
        case skill_id_20091:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGHeroLayer *hisHero = heroLayer->getOpponentHeroLayer();
            int maxnum = hisHero->max_soldierNum;
            int soldier =  hisHero->getSoldier();
            int left = maxnum-soldier;
            if (left>16)
            {
                return true;
            }
        }
            break;
        case skill_id_20038:
        case skill_id_20039:
        case skill_id_20092:
        case skill_id_20099:
        case skill_id_20043:
        case skill_id_20044:
        case skill_id_20045:
        {
            int numd = 0;
            for (int i = 0; i < mapRow; i++)
            {
                for (int j = 0; j < mapList; j++)
                {
                    SGGridBase *grid = bm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbone && grid->getStype() == kdefend)
                    {
                        numd++;
                    }
                }
            }
            if (numd>5)
            {
                return true;
            }
        }
            break;
        case skill_id_20058:
        case skill_id_20059:
        case skill_id_20069:
        {
            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            int maxnum = heroLayer->max_soldierNum;
            int soldier =  heroLayer->getSoldier();
            if (soldier == 0)
            {
                return false;
            }

            int num = 0;
            int num2 = 0;
            int num4 = 0;
            for (int i = 0; i<mapRow; i++)
            {
                for (int j = 0; j<mapList; j++)
                {
                    SGGridBase *grid = bm->myGrids[i][j];
                    if (grid &&
                        grid->getStype() == knormal)
                    {
                        num++;
                        if (grid->getSbType() == ksbtwo)
                        {
                            num2++;
                        }
                        if (grid->getSbType() == ksbfour)
                        {
                            num4++;
                        }
                    }
                }
            }
            num = num-num2*.5 - num4*0.25;
            CCLOG("maxnum:%d",maxnum);
            CCLOG("num:%d",num);
            CCLOG("soldier:%d",soldier);
            
            if (num <maxnum/2 && num>0)
            {
                return true;
            }
        }
            break;
        case skill_id_20060:
        case skill_id_20061:
        {
            int num2 = 0;
            for (int i = 0; i < mapRow; i++)
            {
                for (int j = 0; j < mapList; j++)
                {
                    SGGridBase *grid = bm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbtwo && grid->getStype() == knormal)
                    {
                        num2++;
                    }
                }
            }
            if (num2>0)
            {
                return true;
            }
        }
            
            break;
        case skill_id_20062:
        case skill_id_20063:
        case skill_id_20100://虚空盾
        {

            SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
            SGBattleMap *sbm = heroLayer->getOpponentHeroLayer()->getBattleMap();
            CCArray *als = sbm->getAttackList();
            CCObject *obj = NULL;
            int ap = 0;
            /*
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getRoundV()== 1)
                {
                    CCLOG("getap:%d",al->getAp());
                    CCLOG("max:%d",heroLayer->heroBloodMax);
                    CCLOG("blood:%d",heroLayer->getBlood());
                    if ((al->getAp() >.2*heroLayer->heroBloodMax) || al->getAp()>= heroLayer->getBlood())
                    {
                        return true;
                    } 
                }
            }*/
            CCARRAY_FOREACH(als, obj)
            {
                SGAttackList *al = (SGAttackList *)obj;
                if (al->getRoundV()== 1)
                {
                    
                    ap +=al->getAp();
                }
            }
            int Id = 4;
            float rate = heroLayer->getBlood()/(heroLayer->heroBloodMax*1.0f);
            m_mapFuzzy[Id]->Fuzzify("Life", (double)clampf(rate*100,0,100));
            m_mapFuzzy[Id]->Fuzzify("Damage_opp", (double)clampf((ap/heroLayer->getBlood())*100,0,100));
            
            
            int score = m_mapFuzzy[Id]->DeFuzzify("Desirability",FuzzyModule::max_av);
            CCLog("desirabel_score************%d",score);
            if(score>=desirable_score)
                return  true;
            
        }
            break;
      
        case skill_id_20001:
        case skill_id_20072:
        case skill_id_20095:
        case skill_id_20003:
        case skill_id_20074:
        case skill_id_20006:
        case skill_id_20077:
        case skill_id_20022:
        case skill_id_20023:
        case skill_id_20084:
        case skill_id_20098:
        case skill_id_20026:
        case skill_id_20027:
        case skill_id_20086:
        case skill_id_20096:
        case skill_id_20034:
        case skill_id_20035:
        case skill_id_20090:
        case skill_id_20040:
        case skill_id_20041:
        case skill_id_20042:
        case skill_id_20046:
        case skill_id_20047:
        case skill_id_20048:
        case skill_id_20049:
        case skill_id_20050:
        case skill_id_20051:
        case skill_id_20052:
        case skill_id_20053:
        case skill_id_20056:
        case skill_id_20057:
        case skill_id_20064:
        
        case skill_id_20066:
        case skill_id_20067:
        case skill_id_20068:
        {
            return true;
        }
            break;
        case skill_id_20065://亡魂杀
        {
            int num = 0;
            for (int i = 0; i < mapRow; i++)
            {
                for (int j = 0; j < mapList; j++)
                {
                    SGGridBase *grid = bm->myGrids[i][j];
                    if (grid && grid->getSbType() == ksbone && grid->getStype() == knormal)
                    {
                        num++;
                    }
                }
            }
            
            int Id = 11;
            m_mapFuzzy[Id]->Fuzzify("Soldier_self", (double)clampf((float)num,0,20));
            
            
            int score = m_mapFuzzy[Id]->DeFuzzify("Desirability",FuzzyModule::max_av);
            CCLog("desirabel_score************%d",score);
            if(score>=desirable_score)
                return  true;
        }
            break;
            
        default:
            break;
    }
    return false;
}
