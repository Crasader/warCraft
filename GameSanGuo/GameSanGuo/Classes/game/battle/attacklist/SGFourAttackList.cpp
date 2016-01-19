//
//  SGGameSPAttack.cpp
//  GameSanGuo
//
//  Created by wenlong w on 12-11-28.
//
//

#include "SGFourAttackList.h"
#include "SGStringConfig.h"

SGFourAttackList::SGFourAttackList()
:theNextGridSward(NULL)

{
    
}

SGFourAttackList::~SGFourAttackList()
{

}

//四格武将的攻击阵列
SGFourAttackList *SGFourAttackList::createWithSoldier(ShiBing *sb, bool ismy)
{
    SGFourAttackList *al = new SGFourAttackList();
    if (al && al->initWithSoldier(sb, ismy, kattackfour))   //调用SGAttackList.cpp
    {
        al->showFormEffect();
        al->setAttackType(kattackfour);
        al->autorelease();
        return al;
    }
    CC_SAFE_DELETE(al);
    return NULL;
}

//形成攻击阵列的特效
void SGFourAttackList::showFormEffect()
{
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap())
    {
        return;
    }
    
    wjhbEffect = CCSkeletonAnimation::createWithFile("effSpine/wjhb4.json", "effSpine/wjhb4.atlas", 1);
    wjhbEffect->setAnimation("animation", false);
    m_effectsLayer->addChild(wjhbEffect, FERM_EFFECT_TAG, FERM_EFFECT_TAG);
    
    bool me = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (!me)
    {
        wjhbEffect->setScaleY(-1);
        wjhbEffect->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
    }
    
}
//-----------------------------------------------------------------------------------------------------|
//庞统的被动技 ， 火凤凰来了
void SGFourAttackList::specialHeroSkillOfPangTong()
{
    std::string skillFire = "animationFile/jian_14_phoenix.scml";    //火球    
    CCSpriterX *spriterx_fire = CCSpriterX::create(skillFire.c_str(),true,true);
    spriterx_fire->setisloop(false);
    spriterx_fire->setanimaID(ACT_wait);
    spriterx_fire->play();
    spriterx_fire->setPosition(CCPointZero);
    m_effectsLayer->addChild(spriterx_fire,SKILL_EFFECT_TAG+10000,SKILL_EFFECT_TAG+110);
    if(!isMy)
    {
        spriterx_fire->setScaleY(-1);
        spriterx_fire->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
    }
    
    float time = spriterx_fire->getAnimaTimeLength(0, 0)*ANIMA_TIME_NUM+1;
    CCDelayTime *delayTime = CCDelayTime::create(time);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGFourAttackList::specialHeroSkillOfPangTong_2));
    CCFiniteTimeAction *sequence = CCSequence::create(delayTime,call,NULL);
    m_effectsLayer->runAction(sequence);
}

void SGFourAttackList::specialHeroSkillOfPangTong_2()
{
    myHero->removeChildByTag(SKILL_EFFECT_TAG+110, true);
    
    std::string skillPhoenix = "animationFile/jian_14_phoenix2.scml";   //火凤凰
    CCSpriterX *spriterx_phoenix = CCSpriterX::create(skillPhoenix.c_str(),true,true);
    spriterx_phoenix ->setisloop(true);
    spriterx_phoenix->setanimaID(ACT_wait);
    spriterx_phoenix->play();
    m_effectsLayer->addChild(spriterx_phoenix,SKILL_EFFECT_TAG+10000,SKILL_EFFECT_TAG+112);
    
    int distance = 0;
    if(isMy)
    {
        distance = 800;
    }
    else
    {
        distance = -800;
        spriterx_phoenix->setScaleY(-1);
        spriterx_phoenix->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
    }
    
    CCMoveBy *move_1 = CCMoveBy::create(2.0, ccp(0, distance));
    spriterx_phoenix->runAction(move_1);
    
    CCDelayTime *delay = CCDelayTime::create(2.0f);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGFourAttackList::specialHeroSkillOfPangTong_3));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    m_effectsLayer->runAction(sequence);
    
}

void SGFourAttackList::specialHeroSkillOfPangTong_3()
{
    m_effectsLayer->removeChildByTag(SKILL_EFFECT_TAG+112, true);
    CCLOG("火凤凰结束");
}
//火凤凰结束
//-----------------------------------------------------------------------------------------------------|

//攻击
void SGFourAttackList::startAttack(SGHeroLayer *myHero_, SGHeroLayer *opponentHero)
{
#if PUSH_ATTACK_DATA
    pushAttackData(CCString::createWithFormat(str_Format_attackerId_attack_column, this->getID(), this->getAp(), this->getAttackIndex().j), true);
//    SGBattleManager::sharedBattle()->fightLogRequest(data->m_sString);
    
#endif
    
    //打印当前形成的攻击阵列的数据
    CCLog("四格武将");
    CCLog("atkmode = %d",this->getAtkmode());
    CCLog("atkmode2 = %d",this->getAtkMode2());
    CCLog("hiteff = %d",this->getHitEff());
    CCLog("name = %s",this->getName().c_str());
    
    this->playAttackEf(this->getAttackEffect());
    
    myHero = myHero_;
    attackHero = opponentHero;
    
    GameIndex index = this->getAttackIndex();
    m_dispearRemote.i = index.i;
    m_dispearRemote.j = index.j;
    
    int indexJ = index.j;
    GPCCLOG("4g选择的列1 %d",indexJ);
    GPCCLOG("4g打列%d",abs(mapList-1-indexJ));
    int indexJ1 = index.j + 1;
    GPCCLOG("4g选择的列2  %d",indexJ1);
    GPCCLOG("4g打列2  %d",abs(mapList-1-indexJ1));
    
    
    float moveTime ;
    if (this->getAtkmode() == 1)//远程
    {
        sbTime = 0.1;
        moveTime = (index.i + 1 + mapRow) * sbTime;
    }
    else   //近程
    {
        sbTime = 0.2;
        moveTime = (index.i + 1 + mapRow) * sbTime;
    }
    

    float delay_time1 = 0.0f;    //每格额外的延时时间（攻击动作时间等，每次循环进行累加
    float att_time = 0.;
    
    bool isme = true;
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        isme = false;
    }
    
    attOver = true;


    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        sb->hideXuLiEffect();
        int act = ACT_atkUp;
        int moveAct = ACT_moveUp;
        if (!isme)
        {
            //确定攻击方向,根据是敌是我,确定是向下或者向上攻击
            act = ACT_atkDown;
            moveAct = ACT_moveDown;
        }
        att_time = m_attackTime ;
        if (this->getAtkmode() == 0)   //近战
        {
            int a = sb->getSbindex().i - index.i  - (isme ? 1 : -1);
            att_time = spineAtkTimeTest * ANIMA_TIME_NUM;
            CCDelayTime *delay = CCDelayTime::create(0);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)moveAct);
            CCPoint pos  = GameConfig::getGridPoint(isme ? -(mapRow - a) : (mapRow - a), abs(isme ? -indexJ : (mapList - 1 - indexJ)),true);
            pos  = ccpAdd(pos, ccp(isme ? gridWidth / 2 : -gridWidth/2, 0));
            CCMoveTo *move = CCMoveTo::create(moveTime, pos);
            CCAction* action = (CCAction*) CCSequence::create(delay, call,move, NULL);
            action->setTag(110);
            sb->runAction(action);
            this->playEffectWithType(kChargeEffect);
  
        }
        else
        {
            sb->setanimaID(act, false);
        }
    }
    
    if (this->getAtkmode() == 1)
    {

        //庞统特殊被动技 火凤凰
//        if(this->getAtkMode2() == 14)
//        {
//            //先设定火凤凰动画的时间是6秒
//            delay_time += 3;
//            this->specialHeroSkillOfPangTong();
//        }
        GPCCLOG("4格武将：FlyId :  %d\n\n", this->getFlyId());
        jianEffect = CCSkeletonAnimation::createWithFile
        (
            CCString::createWithFormat("effSpine/fly%03d.json", this->getFlyId())->getCString(),
            CCString::createWithFormat("effSpine/fly%03d.atlas", this->getFlyId())->getCString()
         );
        jianEffect->setAnimation("animation", true);
        
        
        this->playEffectWithType(kAttackEffect);
        //如果是敌方发动攻击，让箭翻转180度
        if(!isme)
        {
            jianEffect->setScaleY(-1);
        }
        
        CCPoint jianPos = GameConfig::getGridPoint(isme ? (index.i + 1) : -index.i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -indexJ  :  (mapList - 1 - indexJ)), true);

        GPCCLOG("四格箭的位置:%f, %f", jianPos.x, jianPos.y);
        
        if (isme)
        {
            jianPos.y += (1.75 * gridHeight);
        }
        else
        {
            jianPos.y +=  0.25 * gridHeight;
        }
        jianEffect->setPosition(ccp(isme ? jianPos.x  + 0.5 * gridWidth : jianPos.x  - 0.5 * gridWidth, jianPos.y));
        //jianEffect->setPosition(GameConfig::getGridPoint(isme ? (index.i + 1): -index.i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -indexJ - 1 :  (mapList - 1 - indexJ - 1)),true));

        CCHide *hide = CCHide::create();
        CCDelayTime *delay = CCDelayTime::create(m_effTime);
        CCShow *show = CCShow::create();
        CCPoint movePos = GameConfig::getGridPoint(isme ? -(mapRow + 1) : mapRow, abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true);
        CCMoveTo *move = CCMoveTo::create(moveTime, ccp(isme ? movePos.x + 0.5 * gridWidth : movePos.x - 0.5 * gridWidth , movePos.y));
        GPCCLOG("4格 MoveToPos : %f  %f,  m_effTime: %f",  movePos.x, movePos.y, m_effTime);
    
        myHero->addChild(jianEffect, JIAN_EFFECT_TAG, JIAN_EFFECT_TAG);
        jianEffect->runAction(CCSequence::create(hide, delay, show, move, NULL));
        //是否画两列箭
        if(1 == this->getPair())
        {
            //四个武将，需要显示两列箭
            theNextGridSward = CCSkeletonAnimation::createWithFile
            (
             CCString::createWithFormat("effSpine/fly%03d.json", this->getFlyId())->getCString(),
             CCString::createWithFormat("effSpine/fly%03d.atlas", this->getFlyId())->getCString()
             );
            theNextGridSward->setAnimation("animation", true);
            //如果是敌方发动攻击，让箭翻转180度
            
            theNextGridSward->setPosition(GameConfig::getGridPoint(isme ? index.i : -index.i,   abs(isme ? -indexJ :  (mapList - 1 - indexJ)),true));
            
            CCHide *hide2 = CCHide::create();
            CCDelayTime *delay2 = CCDelayTime::create(m_effTime);
            CCShow *show2 = CCShow::create();
            CCMoveTo *move2 = CCMoveTo::create(moveTime, GameConfig::getGridPoint(isme ? -(mapRow) : (mapRow), abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true));
            myHero->addChild(theNextGridSward, JIAN_EFFECT_TAG, JIAN_EFFECT_TAG2);

            theNextGridSward->runAction(CCSequence::create(hide2, delay2, show2, move2, NULL));
        }
    }

    for (int i = 0; i <= mapRow; i++)
    {
        if (this->getAp() > 0)
        {
            float j = 0.0;
            if (i+index.i == 0)
            {
                j = 1.0;
            }
            else
            {
                j = (i + index.i + 1);
            }
            float b = sbTime * j;
            shengYuTime = moveTime - b;
            b = b + delay_time1;
            if (i == mapRow)
            {
                GPCCLOG("四格攻击底线:::::: " );
                
                CCObject *obj = NULL;
                CCARRAY_FOREACH(attackSbs, obj)
                {
                    ShiBing *sb = (ShiBing*)obj;
                    CCDelayTime* delay1 = CCDelayTime::create(b);
                    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGFourAttackList::vanishAttackList));
                    CCAction *action = (CCAction*) CCSequence::create(delay1, call, NULL);
                    action->setTag(111);
                    sb->runAction(action);
                    
                }
                

                float c = b + m_effTime;
                CCDelayTime *delaytime = CCDelayTime::create(c);
                CCCallFuncND*  callEffBz = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::cgpEffectTest), (void*)indexJ);
                
                CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::changBlood));
                myHero->runAction(CCSequence::create(delaytime, callEffBz, call, NULL));
            }
            else
            {
                //没有打到底线
                //GPCCLOG("四格武将没有打到底线::::::");
                SGGridBase *grid = attackHero->battleMap->myGrids[i][abs(mapList-1-indexJ)];
                SGGridBase *grid1 = attackHero->battleMap->myGrids[i][abs(mapList-1-indexJ1)];
                if (grid || grid1)
                {
                    
                    SGGridBase *randGrid = NULL;
                    if (grid)
                    {
                        randGrid = grid;
                    }
                    else
                    {
                        randGrid = grid1;
                    }
                    GameSbIndex *sbindex = new GameSbIndex();
                    sbindex->i = i;
                    sbindex->j = abs(mapList-1-indexJ);
                    sbindex->_time = shengYuTime;
                    CCCallFunc *callVibration = CCCallFunc::create(this, callfunc_selector(SGAttackList::attackSbVibration));
                    CCDelayTime* delay = CCDelayTime::create(b + m_effTime);
                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGFourAttackList::removesb), (void*)sbindex);
                    myHero_->runAction(CCSequence::create(delay, call, callVibration, NULL));
                    
                }
                else
                {
//                    CCDelayTime* delay = CCDelayTime::create(b);
//                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::addSkillsBuff), (void*)&i);
//                    myHero->runAction(CCSequence::create(delay, call, NULL));
                }
            }
        }
        else
        {
            break;
        }
    }
    
    
}

//箭发射完，该消失了
void SGFourAttackList::vanishAttackList(CCObject *obj)
{
    float time = 0.;
    ShiBing *sb = (ShiBing*)obj;
    sb->stopAllActions();
    
    if (this->getAtkmode() == 1)
    {
        //远程攻击
        
        //一个箭，一个爆炸
        if(0 == this->getPair())
        {
            if(jianEffect)
            {
//                jianEffect->removeFromParentAndCleanup(true);
//                jianEffect = NULL;
//                myHero->removeChildByTag(JIAN_EFFECT_TAG, true);
//                myHero->showJianBlastEffect(sb->getHitEff(), true, gameIndex(-mapRow, sb->getSbindex().j+1), true);
                
                CCDelayTime*  jianEffDelay = CCDelayTime::create(m_effTime);
                CCCallFunc*   jianEffDelayCall = CCCallFunc::create(this, callfunc_selector(SGAttackList::cgpCallJianEffDelay));
                jianEffect->runAction(CCSequence::create(jianEffDelay, jianEffDelayCall, NULL));
            }
        }
        else if(1 == this->getPair())  //两个箭，两个爆炸
        {
            if(jianEffect)
            {
                jianEffect->removeFromParentAndCleanup(true);
                jianEffect = NULL;
                myHero->removeChildByTag(JIAN_EFFECT_TAG, true);
            }
            if(theNextGridSward)
            {
                theNextGridSward->removeFromParentAndCleanup(true);
                theNextGridSward = NULL;
                myHero->removeChildByTag(JIAN_EFFECT_TAG2, true);
            }
            
            //两列箭，展示两个爆炸效果
            myHero->showJianBlastEffect(sb->getHitEff(), true, gameIndex(-mapRow, sb->getSbindex().j), true);
            myHero->showJianBlastEffect(sb->getHitEff(), true,gameIndex(-mapRow, sb->getSbindex().j+1), true);
        }
    }
    else
    {
        time = setActAndTime(sb);
        this->playEffectWithType(kAttackEffect);
    }
    
    CCDelayTime *delaytime = CCDelayTime::create(0);
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::removeAttackSb));
    sb->runAction(CCSequence::create(delaytime, call, NULL));
}


//攻击完成,对应的攻击阵列应该消失
void SGFourAttackList::removesb(cocos2d::CCObject *obj, GameSbIndex *sbindex)
{
    if (this->getAp() <= 0)
    {
        return;
    }
    
    SGGridBase *attackGrid1 = attackHero->battleMap->myGrids[sbindex->i][sbindex->j];
    SGGridBase *attackGrid0 = attackHero->battleMap->myGrids[sbindex->i][sbindex->j - 1];//右
    
    if (attackGrid0 && attackGrid1)
    {

        SGSoldier *soldier = NULL;
        SGSoldier *rightSoldier = NULL;
        
        bool isSoldierSklls = false;
        bool isRightSoldierSkills = false;
        
        int defNum = 0;
        int defNum1 = 0;
        
        switch (attackGrid0->getStype())
        {
            case knormal:
            case kdefend:
            {
                soldier = attackHero->battleMap->getShiBingWithGrid(attackGrid0);
                if(soldier)
                {
                    defNum = soldier->getDef();
                    
                    if (attackGrid0->getStype() == knormal)
                    {
                        if (soldier->getSkillsId() >= SKILLS_ID1 )
                        {
                            isSoldierSklls = true;
                        }
                    }
                }
            }
            break;
            case kattack:
            {
                soldier = attackHero->battleMap->getAttackListByGrid(attackGrid0);
                if(soldier)
                {
                    SGAttackList * soldierattack = (SGAttackList *)soldier;
                    if (!soldierattack->getIsDeath())
                    {
                        
                        defNum = soldier->getAp();
                        if (soldierattack->getSkillsDelegate())
                        {
                            isSoldierSklls = true;
                        }
                    }
                }
                
                
            }
            break;
                
            default:
                break;
        }
        
        switch (attackGrid1->getStype())
        {
            case knormal:
            {
                rightSoldier = attackHero->battleMap->getShiBingWithGrid(attackGrid1);
                if(rightSoldier)
                {
                    if (soldier != rightSoldier)
                    {
                        defNum1 = rightSoldier->getDef();
                    }
                    
                    if (attackGrid1->getStype() == knormal)
                    {
                        if (rightSoldier->getSkillsId() >= SKILLS_ID1 )
                        {
                            isRightSoldierSkills = true;
                        }
                    }
                }
            }
                break;
            case kdefend:
            {

                rightSoldier = attackHero->battleMap->getShiBingWithGrid(attackGrid1);
                if(rightSoldier)
                    defNum1 = rightSoldier->getDef();
            
            }
                break;
            case kattack:
            {
                rightSoldier = attackHero->battleMap->getAttackListByGrid(attackGrid1);
                if(rightSoldier)
                {
                    SGAttackList * soldierattack = (SGAttackList *)rightSoldier;
                    if (!soldierattack->getIsDeath())
                    {
                        if (soldier != rightSoldier)
                        {
                            defNum1 = rightSoldier->getAp();
                            if (soldierattack->getSkillsDelegate())
                            {
                                
                                isRightSoldierSkills = true;
                                
                            }
                            
                        }
                    }
                }
            }
                break;
                
            default:
                break;
        }
        
        
        int attackAp = 0;
        int attackAp0 = 0;
        int attackAp1 = 0;
        attackAp = this->getAp();
        if(defNum >= defNum1 )
        {
            attackAp0 = floor(attackAp*(float)defNum/(float)(defNum+defNum1));
            attackAp1= attackAp- attackAp0;
        }
        else
        {
            attackAp1 =  floor(attackAp*(float)defNum1/(float)(defNum+defNum1));
            attackAp0 = attackAp - attackAp1;
        }
        
        int attack1 = 0;
        int attack0 = 0;
        attack1 = attackGrid(attackHero->battleMap, attackGrid1, attackAp1);
        
       
//        if (this->getSkillsDelegate())
//        {
//             this->activateSkills8(NULL, this, this->getAfftributeNum(),gameIndex(attackGrid1->getIndex().i, attackGrid1->getIndex().j), gameIndex(attackGrid0->getIndex().i, attackGrid0->getIndex().j - 1),attack1);
//          
//        }
       
        SGGridBase *attactempkGrid0 = attackHero->battleMap->myGrids[sbindex->i][sbindex->j - 1];//右
        if(attactempkGrid0)
        {
            attack0 = attackGrid(attackHero->battleMap, attactempkGrid0, attackAp0);
        }

        if (this->getAp() > 0)
        {
            if (this->getSkillsDelegate())
            {
                this->activateBuffSkills(NULL, this, gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1), 1);
                this->activateSkills12(NULL, this, this->getAfftributeNum(), gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1));
            }
        }
        this->changeAP(this->getAp(), true);
        
        
    }
    else if(!attackGrid0 && attackGrid1)
    {
        
        SGGridBase *nowGrid = NULL;
        if (attackGrid0)
        {
            nowGrid = attackGrid0;
        }
        if(attackGrid1)
        {
            nowGrid = attackGrid1;
        }
        
        if (nowGrid)
        {
           
            int demage = this->getAp();
            int attacktmep = 0;
            attacktmep = attackGrid(attackHero->battleMap, nowGrid,demage);
            if (this->getSkillsDelegate())
            {
               
                    this->activateSkills8(NULL, this, this->getAfftributeNum(),gameIndex(sbindex->i, sbindex->j - 1), gameIndex(sbindex->i, sbindex->j - 2),attacktmep);
                
            }
            
            if (this->getAp() > 0)
            {
                if (this->getSkillsDelegate())
                {
                    this->activateBuffSkills(NULL, this, gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1), 1);
                    this->activateSkills12(NULL, this, this->getAfftributeNum(), gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1));
                }
            }
        }
        this->changeAP(this->getAp(), true);
        
    }
    else if(attackGrid0 && !attackGrid1)
    {
        
        SGGridBase *nowGrid = NULL;
        if (attackGrid0)
        {
            nowGrid = attackGrid0;
        }
        if(attackGrid1)
        {
            nowGrid = attackGrid1;
        }
        
        if (nowGrid)
        {
            
            int demage = this->getAp();
            int attacktmep = 0;
            attacktmep = attackGrid(attackHero->battleMap, nowGrid,demage);
            if (this->getSkillsDelegate())
            {
                this->activateSkills8(NULL, this, this->getAfftributeNum(),gameIndex(sbindex->i, sbindex->j - 1), gameIndex(sbindex->i, sbindex->j - 2),attacktmep);
              
            }
            
            if (this->getAp() > 0)
            {
                if (this->getSkillsDelegate())
                {
                    this->activateBuffSkills(NULL, this, gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1), 1);
                    this->activateSkills12(NULL, this, this->getAfftributeNum(), gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1));
                }
            }
        }
        this->changeAP(this->getAp(), true);
        
    }
    else
    {
        if (this->getAp() > 0)
        {
            this->activateBuffSkills(NULL, this, gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j - 1), 1);
        }
    }
    
    if (this->getAp() <= 0)
    {
        if (jianEffect)
        {
            jianEffect->removeFromParentAndCleanup(true);
            jianEffect = NULL;
            myHero->removeChildByTag(JIAN_EFFECT_TAG, true);
            myHero->showJianBlastEffect(this->getHitEff(), true, gameIndex(-mapRow, (mapList - 1 - sbindex->j)));
        }

        
        if (this->getPair() == 1)
        {
            if(theNextGridSward)
            {
                theNextGridSward->removeFromParentAndCleanup(true);
                theNextGridSward = NULL;
                myHero->removeChildByTag(JIAN_EFFECT_TAG2, true);
                myHero->showJianBlastEffect(this->getHitEff(), true, gameIndex(-mapRow, (mapList - 1 - (sbindex->j + 1))));
            }
        }
        myHero->attackOver(this);
    }
    
    CC_SAFE_RELEASE(sbindex);
}
//攻击到的网格,再由网格确定是墙,还是阵列,还是普通等
int  SGFourAttackList::attackGrid(SGBattleMap *map, SGGridBase *grid,int damage)
{
   
    SGSoldier *soldier = NULL;

    int resistForce = 0;
    int leftBlood = 0;
    
    int skillsNum = 0;
    int skillsNum_1 = 0;
    ShiBing *sb = map->getShiBingWithGrid(grid);
    if(!sb)
        return 0;
    switch (grid->getStype())
    {
        case knormal:
        {
            soldier = map->getShiBingWithGrid(grid);
            if(soldier)
            {
                resistForce = soldier->getDef();
                skillsNum = resistForce;
                skillsNum_1 = resistForce;
                
                if (this->getSkillsDelegate())
                {
                    resistForce = this->activateSkills5(NULL, this, resistForce, sb);
                    if (resistForce == 0)
                    {
                        attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                    }
                }
            }
            
        }
            break;
        case kdefend:
        {
            soldier = map->getShiBingWithGrid(grid);
            if(soldier)
            {
                if (this->getSkillsDelegate())
                {
                    
                    if (this->activateSkills11(NULL, this, (ShiBing*)soldier))
                    {
                        attackHero->showBattleLabel(this->getAfftributeNum() * soldier->getDef(), "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                    }
                }
                resistForce = soldier->getDef();
            }
            
        }
            break;
        case kattack:
        {
            soldier = map->getAttackListByGrid(grid);
            if(soldier)
            {
                SGAttackList * soldierattack = (SGAttackList *)soldier;
                if (!soldierattack->getIsDeath())
                {
                    resistForce = soldier->getAp();
                    skillsNum = resistForce;
                    
                    if (this->getSkillsDelegate())
                    {
                        SGAttackList * attack = (SGAttackList *)soldier;
                        if (attack->getAttackType() == kattackfour &&
                            this->getAp() < attack->getAp())
                        {
                            this->activateSkills6(NULL, this, this->getAfftributeNum());
                        }
                    }
                    
                    if (soldierattack->getSkillsDelegate())
                    {
                        soldierattack->activatePassivitySkills(NULL, soldierattack, soldierattack->getAfftributeNum(), this);
                        soldierattack->activatePassivitySkills1(NULL, soldierattack, this->getAp());
                    }
                    
                }
            }
            
            
        }
            break;
            
        default:
            break;
    }
    
    bool w = false;
    if (this->getSkillsDelegate())
    {
        if (grid->getStype() != kdefend)
        {
            w = this->activateSkills1(NULL, this, skillsNum);
            if (w)
            {
                if (grid->getStype() != kattack)
                {
                    attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                }
            }
        }
    }
    
    
    leftBlood = damage - resistForce;
#if PUSH_ATTACK_DATA

    CCString *data = CCString::createWithFormat(str_Format_behitId_defense, sb->getSbId(), resistForce);
#endif
    
    CCLOG("this->getAp()==%d",this->getAp());
    CCLOG("resistForce===%d",resistForce);

    CCArray *sbArray = CCArray::create();

    if (leftBlood >= 0)
    {
        
        if(grid->getStype() == kattack)
        {
            SGAttackList * soldierattack = (SGAttackList *)soldier;
            sbArray->addObjectsFromArray(soldierattack->attackSbs);

            map->removeAttackList(soldierattack, attackHero, grid);
            attackHero->showBattleLabel(resistForce, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());

        }
        else
        {

            attackHero->showBattleLabel(resistForce, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
            map->removeAttackedSB((ShiBing*)soldier, soldier->getAp(), attackHero);


        }
        myHero->showLJEff(false , resistForce);
        
    }
    else
    {
        
        if (grid->getStype() == knormal)
        {
            if (this->getSkillsDelegate())
            {
                if (this->activateSkills2(NULL, this, 0))
                {
                    attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                }
            }
            map->removeAttackedSB((ShiBing*)soldier, 0, attackHero);
            myHero->showLJEff(false, damage);
//            attackHero->showBattleLabel(damage, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
        }
        else
        {
            int leftattack = damage;
            if (grid->getStype() == kdefend)
            {
                soldier->setDef(soldier->getDef() - leftattack);
                ShiBing *bing = (ShiBing *) soldier;
                bing->showDef(soldier->getDef());
                bing->setDefImage();
                
                if (this->getSkillsDelegate())
                {
                    if (this->activateSkills2(NULL, this, 0))
                    {
                        attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                        map->removeAttackedSB((ShiBing*)soldier, leftattack, attackHero);
                    }
                }
            }
            else
            {
                SGAttackList *attack = (SGAttackList *)soldier;
                sbArray->addObjectsFromArray(attack->attackSbs);
                CCLOG("ap===%d",soldier->getAp());
                soldier->setAp(soldier->getAp() - leftattack);
				
				if (leftattack > 0)
				{
					attack->playEffectWithType(kAttackEffect);
				}
				
                attack->changeAP(soldier->getAp(), false, true);
                
                if (this->getSkillsDelegate())
                {
                    if (this->activateSkills2(NULL, this, 0))
                    {
                        
                        SGAttackList * soldierattack = (SGAttackList *)soldier;
                        CCObject *solSb = NULL;
                        CCARRAY_FOREACH(soldierattack->attackSbs, solSb)
                        {
                            ShiBing *solsb = (ShiBing *)solSb;
                            attackHero->showEffectLayer(3, true, solsb->getSbindex());
                        }

                        map->removeAttackList(soldierattack, attackHero, grid);
                    }
                }
                
            }
            
            attackHero->showBattleLabel(leftattack, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
        }

    }
    
    this->setAp(this->getAp()-resistForce);
    if (leftBlood > 0)
    {
        if (this->getSkillsDelegate())
        {
            
            if (grid->getStype() != kdefend)
            {
                if (w)
                {
                    if (grid->getStype() == kattack)
                    {
                        CCObject *sbAtt = NULL;
                        CCARRAY_FOREACH(sbArray, sbAtt)
                        {
                            ShiBing *attSb = (ShiBing*)sbAtt;
                            attackHero->showEffectLayer(3, true, attSb->getSbindex());
                        }
                    }
                }
            }
            
            bool wuShun = this->activateSkills1(NULL, this, skillsNum, leftBlood);
            if (wuShun)
            {
                if (grid->getStype() == kattack)
                {
                    CCObject *sbAtt = NULL;
                    CCARRAY_FOREACH(sbArray, sbAtt)
                    {
                        ShiBing *attSb = (ShiBing*)sbAtt;
                        attackHero->showEffectLayer(3, true, attSb->getSbindex());
                    }
                    if (sbArray)
                    {
                        sbArray->removeAllObjects();
                    }
                    
                    
                }
                else
                {
                    attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                }
            }
            //this->activateSkills1_1(NULL, this, skillsNum_1, grid->getIndex());
        }
#if PUSH_ATTACK_DATA
        data = CCString::createWithFormat(str_Format_damage_residual, data->getCString(),leftBlood);
        pushAttackData(data, false);
#endif
    }
    else
    {
#if PUSH_ATTACK_DATA

        data = CCString::createWithFormat(str_Format_damage_residual, data->getCString(),0);
        pushAttackData(data, false);
#endif
    }

    return leftBlood>=0?resistForce:damage;
}
