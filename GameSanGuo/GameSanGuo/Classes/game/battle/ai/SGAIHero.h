//
//  SGAIHero.h
//  GameSanGuo
//
//  Created by geng on 13-3-29.
//
//

#ifndef __GameSanGuo__SGAIHero__
#define __GameSanGuo__SGAIHero__

#include "SGBattleMap.h"
#include "cocos2d.h"
#include "FuzzyModule.h"
#include <map>
const int desirable_score = 45;
USING_NS_CC;
class SGAIHero 
{
private:
    SGAIHero(){}
    static SGAIHero * m_SsgAiHero;

public:
    ~SGAIHero(){
        std::map<int, FuzzyModule*>::iterator it = m_mapFuzzy.begin();
        for(;it!=m_mapFuzzy.end();it++)
        {if(it->second!=NULL) delete it->second;}
    }
    static SGAIHero * createSig(){//创建单例对象
        if(m_SsgAiHero ==NULL)
        {
            m_SsgAiHero = new SGAIHero();
            m_SsgAiHero->readSkillAIConfig();
            return  m_SsgAiHero;
    
        }else
        {
            return  m_SsgAiHero;
        }
    }
    bool startSkill(SGBattleMap*bm, int skillId);
    std::map<int ,FuzzyModule*> m_mapFuzzy;///模糊逻辑组
    void readSkillAIConfig();///读取模糊逻辑组规则
    //static SGAIHero * m_SgAiHero;
};

#endif /* defined(__GameSanGuo__SGAIHero__) */
