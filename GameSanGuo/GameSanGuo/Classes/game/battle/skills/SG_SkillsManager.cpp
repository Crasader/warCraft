//
//  SG_SkillsManager.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-11.
//
//

#include "SG_SkillsManager.h"
#include "SG_SkillsID1.h"
#include "SG_SkillsID2.h"
#include "SG_SkillsID3.h"
#include "SG_SkillsID4.h"
#include "SG_SkillsID5.h"
#include "SG_SkillsID6.h"
#include "SG_SkillsID7.h"
#include "SG_SkillsID8.h"
#include "SG_SkillsID9.h"
#include "SG_SkillsID10.h"
#include "SG_SkillsID11.h"
#include "SG_SkillsID12.h"
#include "SG_SkillsID13.h"
#include "SG_SkillsID14.h"
#include "SG_SkillsID15.h"
#include "SG_SkillsID16.h"
#include "SG_SkillsID17.h"
#include "SG_SkillsID18.h"
#include "SG_SkillsID19.h"
#include "SG_SkillsID20.h"
#include "SG_SkillsID21.h"
#include "SG_SkillsID22.h"
#include "SG_SkillsID23.h"
#include "SG_SkillsID24.h"
#include "SG_SkillsID25.h"
#include "SG_SkillsID26.h"
#include "SG_SkillsID27.h"
#include "SG_SkillsID28.h"
#include "SG_SkillsID29.h"
#include "SG_SkillsID30.h"
#include "SG_SkillsID31.h"
#include "SG_SkillsID32.h"
#include "SG_SkillsID33.h"
#include "SG_SkillsID34.h"
#include "SG_SkillsID35.h"
#include "SG_SkillsID36.h"
#include "SG_SkillsID37.h"
#include "SG_SkillsID38.h"
#include "SG_SkillsID42.h"
#include "SG_SkillsID43.h"
#include "SG_SBSkillsID1.h"
#include "SG_SBSkillsID2.h"
#include "SGAIManager.h"

SG_SkillsManager::SG_SkillsManager()
:m_skBase(NULL)
{
    
}


SG_SkillsManager::~SG_SkillsManager()
{
    CC_SAFE_DELETE(m_skBase);
}
//根据技能id初始化技能管理器
SG_SkillsManager* SG_SkillsManager::initskManager(int skid)
{
    SG_SkillsManager* skillsManager = new SG_SkillsManager();
    if (skillsManager && skillsManager->init()) {
    
        skillsManager->setSkillsType(skid);
        return skillsManager;
    }
    CC_SAFE_DELETE(skillsManager);
    return NULL;
    
}

//此处根据技能id确定对应的技能,应该调用哪个技能
SG_SkillsBase* SG_SkillsManager::setSkillsType(int skid)
{
    switch (skid) {
        case SKILLS_ID1://煽动术
        case SKILLS_ID79:
        {
            SG_SkillsID1 *sk = new SG_SkillsID1();
            sk->skillsId = skid;
            setskBase((SG_SkillsBase*)sk);
            
        }
            break;
        case SKILLS_ID2://刺杀//同过关斩将..不知道为什么  SKILLS_ID53
        case SKILLS_ID80:
        {
            SG_SkillsID2 *sk = new SG_SkillsID2();
            sk->skillsId = skid;
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID55://技能表中没有(2014年4月10日)
        case SKILLS_ID56:
        case SKILLS_ID57:
        case SKILLS_ID58:
        {
            SG_SkillsID3 *sk = new SG_SkillsID3();
            sk->skillsId = skid;
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID4://威慑 
        case SKILLS_ID82:
        {
            SG_SkillsID4 *sk = new SG_SkillsID4();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID5://灼烧
        case SKILLS_ID83:
        case SKILLS_ID3://电击
        case SKILLS_ID81:
        {
//            SG_SkillsID5 *sk = new SG_SkillsID5();
//            setskBase((SG_SkillsBase*)sk);
            
            SG_SkillsID42 *sk = new SG_SkillsID42();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID6://瞬杀
        case SKILLS_ID84:
        {
            SG_SkillsID6 *sk = new SG_SkillsID6();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID7://藐视 碾压 凌弱术
        case SKILLS_ID77:
        case SKILLS_ID78:
        case SKILLS_ID85:
        {
            SG_SkillsID7 *sk = new SG_SkillsID7();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID8://火计
        case SKILLS_ID86:
        case SKILLS_ID25:
        case SKILLS_ID26:
        case SKILLS_ID93:
        case SKILLS_ID70:
        case SKILLS_ID71:
        case SKILLS_ID72:
        {
//            SG_SkillsID8 *sk = new SG_SkillsID8();
            SG_SkillsID43 *sk = new SG_SkillsID43();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID9://吸血
        case SKILLS_ID10:
        case SKILLS_ID87:
        {
            SG_SkillsID9 *sk = new SG_SkillsID9();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
//        case SKILLS_ID11:
//        case SKILLS_ID12:
//        {
//            SG_SkillsID10 *sk = new SG_SkillsID10();
//            setskBase((SG_SkillsBase*)sk);
//        }
//            break;
        case SKILLS_ID13://隐忍 汲魂术 反戈一击
        case SKILLS_ID14:
        {
            SG_SkillsID11 *sk = new SG_SkillsID11();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
//        case SKILLS_ID15:
//        case SKILLS_ID16:
//        case SKILLS_ID88:
//        {
//            SG_SkillsID12 *sk = new SG_SkillsID12();
//            setskBase((SG_SkillsBase*)sk);
//        }
//            break;
        case SKILLS_ID17://挑战 自强术
        case SKILLS_ID18:
        case SKILLS_ID89:
        {
            SG_SkillsID13 *sk = new SG_SkillsID13();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID19://毒龙斩 
        case SKILLS_ID20:
        case SKILLS_ID90:
        case SKILLS_ID37:
        case SKILLS_ID38:
        case SKILLS_ID99:
        {
            SG_SkillsID14 *sk = new SG_SkillsID14();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID15://筑城
        case SKILLS_ID16:
        case SKILLS_ID88:
        case SKILLS_ID21:
        case SKILLS_ID22:
        case SKILLS_ID91:
        {
            SG_SkillsID15 *sk = new SG_SkillsID15();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID23://战意
        case SKILLS_ID24:
        case SKILLS_ID92:
        {
            SG_SkillsID16 *sk = new SG_SkillsID16();
//            SG_SkillsID5 *sk = new SG_SkillsID5();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
//        case SKILLS_ID25:
//        case SKILLS_ID26:
//        case SKILLS_ID93:
//        {
//            SG_SkillsID17 *sk = new SG_SkillsID17();
//            setskBase((SG_SkillsBase*)sk);
//        }
//            break;
        case SKILLS_ID75:
        case SKILLS_ID76:
        {
            SG_SkillsID19 *sk = new SG_SkillsID19();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
//        case SKILLS_ID27:
//        case SKILLS_ID28:
//        case SKILLS_ID94:
//        {
//            SG_SkillsID18 *sk = new SG_SkillsID18();
//            setskBase((SG_SkillsBase*)sk);
//            
//        }
//            break;
        case SKILLS_ID29://破袭
        case SKILLS_ID30:
        case SKILLS_ID95:
        {
            SG_SkillsID20 *sk = new SG_SkillsID20();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID31://复活攻击队列
        case SKILLS_ID32:
        case SKILLS_ID96:
        {
            SG_SkillsID21 *sk = new SG_SkillsID21();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID33://技能表中没有(2014年4月10日)
        {
            SG_SkillsID23 *sk = new SG_SkillsID23();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID34://技能表中没有(2014年4月10日)
        case SKILLS_ID97:
        {
            SG_SkillsID22 *sk = new SG_SkillsID22();
            setskBase((SG_SkillsBase*)sk);
            
        }
            break;
          
        case SKILLS_ID35://左右溅射
        case SKILLS_ID36:
        case SKILLS_ID98:
        {
            SG_SkillsID24 *sk = new SG_SkillsID24();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
            
//        case SKILLS_ID37:
//        case SKILLS_ID38:
//        case SKILLS_ID99:
            
//        {
//            SG_SkillsID25 *sk = new SG_SkillsID25();
//            setskBase((SG_SkillsBase*)sk);
//        }
//            break;
//        case SKILLS_ID39:
//        case SKILLS_ID40:
//        case SKILLS_ID100:
//        {
//            SG_SkillsID27 *sk = new SG_SkillsID27();
//            setskBase((SG_SkillsBase*)sk);
//        }
//            break;
        case SKILLS_ID41://破城
        case SKILLS_ID42:
        case SKILLS_ID43:
        {
        
            SG_SkillsID28 *sk = new SG_SkillsID28();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
            
        case SKILLS_ID44://固守
        case SKILLS_ID45:
        case SKILLS_ID46:
        case SKILLS_ID64:
        case SKILLS_ID65:
        case SKILLS_ID66:
        {
            SG_SkillsID29 *sk = new SG_SkillsID29();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
            
        case SKILLS_ID47://向后溅射
        case SKILLS_ID48:
        {
            SG_SkillsID30 *sk = new SG_SkillsID30();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID49:
        case SKILLS_ID50:
        {
            SG_SkillsID31 *sk = new SG_SkillsID31();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
            
        case SKILLS_ID51://攻杀
        case SKILLS_ID52:
        {
            SG_SkillsID32 *sk = new SG_SkillsID32();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
            
        case SKILLS_ID53:
        case SKILLS_ID54:
        {
            SG_SkillsID33 *sk = new SG_SkillsID33();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
            
        case SKILLS_ID59:
        case SKILLS_ID60:
        case SKILLS_ID11://疗伤
        case SKILLS_ID12:
        {
            SG_SkillsID35 *sk = new SG_SkillsID35();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID27:
        case SKILLS_ID28://乾坤阵
        case SKILLS_ID94:
        case SKILLS_ID61:
        case SKILLS_ID62:
        case SKILLS_ID63:
        {
//            SG_SkillsID36 *sk = new SG_SkillsID36();
            SG_SkillsID8 *sk = new SG_SkillsID8();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID67:
        case SKILLS_ID68:
        case SKILLS_ID69:
        case SKILLS_ID73:
        case SKILLS_ID74:
        case SKILLS_ID39://淬毒
        case SKILLS_ID40:
        case SKILLS_ID100:
        {
            SG_SkillsID37 *sk = new SG_SkillsID37();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
//        case SKILLS_ID70:
//        case SKILLS_ID71:
//        case SKILLS_ID72:
//        {
//            SG_SkillsID38 *sk = new SG_SkillsID38();
//            setskBase((SG_SkillsBase*)sk);
//        }
//            break;
        case SKILLS_ID30000:
        {
            SG_SBSkillsID1 *sk = new SG_SBSkillsID1();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        case SKILLS_ID30001:
        {
            SG_SBSkillsID2 *sk = new SG_SBSkillsID2();
            setskBase((SG_SkillsBase*)sk);
        }
            break;
        default:
        {
            CCLOG("skid==%d",skid);
            setskBase(NULL);
        }
            break;
    }
	
    return getskBase();
}

void SG_SkillsManager::activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills(battleMap, attackList, num);
    }
    
}

void SG_SkillsManager::activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills(isHero, battleMap, attackList, num);
    }
}

bool SG_SkillsManager::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->activateSkills1(battleMap, attackList, num, damage);
    }
    return false;
}

bool SG_SkillsManager::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->activateSkills1(battleMap, attackList, num);
    }
    return false;
}

void SG_SkillsManager::activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills1_1(battleMap, attackList, num, index);
    }
}

bool SG_SkillsManager::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->activateSkills2(battleMap, attackList, num);
    }
    return false;
}

void SG_SkillsManager::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills3(battleMap, attackList, num);
    }
}

void SG_SkillsManager::activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills4(battleMap, attackList, num);
    }
}

int SG_SkillsManager::activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->activateSkills5(battleMap, attackList, num, sb);
    }
    return num;
}

int SG_SkillsManager::activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->activateSkills5_1(battleMap, attackList, num);
    }
    return num;
}

void SG_SkillsManager::activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills6(battleMap, attackList, num);
    }
}

void SG_SkillsManager::activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills7(battleMap, attackList, num);
    }
}

void SG_SkillsManager::activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage)
{
    GPCCLOG("SG_SkillsManager::activateSkills8\n");
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills8(battleMap, attackList, num, index, index1, damage);
    }
}

void SG_SkillsManager::activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills9(battleMap, attackList, num, attackList1);
    }
}

void SG_SkillsManager::activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills10(battleMap, attackList, num);
    }
}

bool SG_SkillsManager::activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList, ShiBing *sb)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->activateSkills11(battleMap, attackList, sb);
    }
    return false;
}

void SG_SkillsManager::activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateSkills12(battleMap, attackList, num, index, index1);
    }
}

bool SG_SkillsManager::assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->assistSkills7(battleMap, attackList, num);
    }
    return false;
}


void SG_SkillsManager::activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList, true);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activatePassivitySkills(battleMap, attackList, num, attackList1);
    }
}

int SG_SkillsManager::assistPassivitySkills(int ap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList, true);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        return skbase->assistPassivitySkills(ap, attackList, num);
    }
    return ap;
}

void SG_SkillsManager::activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList, true);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activatePassivitySkills1(battleMap, attackList, num);
    }
}



void SG_SkillsManager::activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateBuffSkills(battleMap, attackList, index, index1, row);
    }
}

void SG_SkillsManager::activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->activateBuffSkills1(battleMap, attackList, num, attackList1);
    }
}

void SG_SkillsManager::detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsDelegate()) {
        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->detectionReviveSkills(battleMap, attackList, num);
    }
}

void SG_SkillsManager::chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1)
{
//    CCObject *obj = NULL;
//    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
//    {
//        SGAttackList *attack = (SGAttackList *)obj;
//        if (attack->getSkillsDelegate()) {
//            attack->getSkillsDelegate()->chackReviveSkills(battleMap, attackList, num);
//            
//        }
//    }
    if (attackList1->getSkillsDelegate()) {
//        pushSkillData(attackList);
        attackList1->getSkillsDelegate()->chackReviveSkills(battleMap, attackList, attackList1);
        
    }
    
}

void SG_SkillsManager::removeReviveAttack(SGBattleMap *battleMap, SGAttackList *attackList)
{
    if (attackList->getSkillsDelegate()) {
//        pushSkillData(attackList);
        SG_SkillsBase *skbase = attackList->getSkillsDelegate();
        skbase->removeReviveAttack(battleMap, attackList);
    }
}

void SG_SkillsManager::pushSkillData(SGAttackList *attackList, bool isbsk)
{
    bool isRound = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (isRound ||
        SGAIManager::shareManager()->isPVE) {
        attackList->getID();
        attackList->getSkillsId();
        CCString *str = CCString::createWithFormat(isbsk ? "bskill: %d , %d":"skill: %d , %d", attackList->getID(), attackList->getSkillsId());
		//CCLOG("%s", str->getCString());
        attackList->pushAttackData(str, true);
        //GPCCLOG("攻击技能数据===> %s", str->getCString());
    }
}


