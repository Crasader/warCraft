//
//  BattleArrayBase.cpp
//  GameSanGuo
// 攻击阵列类
//
//  Created by gengshengcai on 12-11-19.
//
//

#include "SGAttackList.h"
#include "SG_SkillsManager.h"
#include "SG_BuffBase.h"
#include "SGAIManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"



SGAttackList::SGAttackList()
:attackSbs(NULL)
,m_SkillsManager_Delegate(NULL)
,m_Skills_Delegate(NULL)
,damageNum(0)
,m_isDeath(false)
,m_isRevive(false)
,m_reviveNum(0)
,fuseNum(0)
,sg_lrType(lr_normal)
,fermEffect(NULL)
,IsRoundBuff(false)
,attOver(false)
,jianEffect(NULL)
,shengYuTime(0.0)
,m_sb_xueTiao(NULL)
,sb_xuetiao(0)
,pushData(NULL)
,roundNum(NULL)
,sb_jianEffect(NULL)
,isAdd(false)
,apLabel(NULL),
defendBloodV(0),
m_effectsLayer(NULL),
m_attackBuffObj(NULL),
attackI(0),
thisJ(0),
attackJ(0),
actionCount(0),
isMy(false),
currentAp(0),
finialAp(0),
addAp(0),
effBottomHit(NULL),
effWallHit(NULL),
mAttIsSpine(true)
{
    attackSbs = CCArray::create();
    attackSbs->retain();
    
    m_effectsLayer = CCLayer::create();
    m_effectsLayer->retain();
    
    m_attackBuffObj = CCArray::create();
    m_attackBuffObj->retain();
    
    
}
SGAttackList::~SGAttackList()
{    
    CCLOG("~SGAttackList()");

    attackSbs->removeAllObjects();
    CC_SAFE_RELEASE(attackSbs);
    
    m_attackBuffObj->removeAllObjects();
    CC_SAFE_RELEASE(m_attackBuffObj);
    
    m_effectsLayer->removeAllChildren();
    CC_SAFE_RELEASE(m_effectsLayer);
    
    CC_SAFE_DELETE(m_SkillsManager_Delegate);
    //楼下的这个实质上是存在m_SkillsManager_Delegate里的，会由它进行删除，这里设计坑爹了，画蛇添足了。
//    CC_SAFE_DELETE(m_Skills_Delegate);

}

void SGAttackList::callRemoveHitBottom()
{
    if (effBottomHit)
    {
        effBottomHit->removeFromParentAndCleanup(true);
        effBottomHit = NULL;
    }
}

void SGAttackList::callRemoveHitWall()
{
    if (effWallHit)
    {
        effWallHit->removeFromParentAndCleanup(true);
        effWallHit = NULL;
    }
}




    //打底线特效
void SGAttackList::cgpEffectTest(CCNode*  psender,  int indexJ)
{
    if(m_isDead)
    {
        return;
    }
    
    if (m_sb_xueTiao)
    {
        m_sb_xueTiao->setPercentage(1);
    }

    int gridRow = mapRow;
    int gridList = mapList - 1 - indexJ;
    
    
    bool isme = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    
    int iFileId = getFileId();
    int bottomID =  SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId)->getBottomEffId();
    
    effBottomHit = CCSkeletonAnimation::createWithFile
    (
     CCString::createWithFormat("effSpine/effect%03d.json", bottomID)->getCString(),
     CCString::createWithFormat("effSpine/effect%03d.atlas", bottomID)->getCString()
     );
    effBottomHit->setAnimation("animation", false);
   
    
    if(isme)
    {
        effBottomHit->setAnchorPoint(ccp(0.5, 1));
    }
    else
    {
        effBottomHit->setScaleY(-1);
        effBottomHit->setAnchorPoint(ccp(0.5, 0));
        gridRow -= 1;
    }

    CCPoint  effPos = GameConfig::getGridPoint(gridRow , gridList ,!isme);  //there isme 分上下，ture 为下
    if (!isme)
    {
        effPos.y += gridHeight;
    }
    
    if(getAttackType() == kattackfour)
    {
        effBottomHit->setPosition(ccp(isme ? effPos.x + 0.5 * gridWidth : effPos.x - 0.5 * gridWidth  , effPos.y));
    }
    else
    {
        effBottomHit->setPosition(ccp(effPos.x , effPos.y));
    }
  

    if(isme)
    {
        myHero->addChild(effBottomHit, 10000);
    }
    else
    {
        attackHero->addChild(effBottomHit, 10000);
    }
    
    CCDelayTime*  delay = CCDelayTime::create(1.0f);
    CCCallFunc*   call = CCCallFunc::create(this, callfunc_selector(SGAttackList::callRemoveHitBottom));
    this->runAction(CCSequence::create(delay, call, NULL));


}

SGAttackList *SGAttackList::createWithSoldier(ShiBing *sb, bool isMy, sgAttackSoldierType type)
{
    SGAttackList *al = new SGAttackList();
    if (al && al->initWithSoldier(sb, isMy, type))
    {
        al->autorelease();
        return al;
    }
    CC_SAFE_DELETE(al);
    return NULL;
}
//通过兵来初始化攻击阵列  //武将
bool SGAttackList::initWithSoldier(ShiBing *sb, bool isMy, sgAttackSoldierType type)
{
    bool bRet = false;

    this->isMy = isMy;
    sb->showXuLiEffect(sb->getSbType());
    attackSbs->addObject(sb);
    sb->hideDef();
    sb->removeColorFlame();
    sb->setLoopAction(NULL ,ACT_xuli);
    thisJ = sb->getSbindex().j;
    attackJ = abs(mapList-1-thisJ);
    sb->changSBState(kattack);
    
    this->addShowMsg(type);
    
    int iFileId = getFileId();
    m_attackTime =  SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId)->getAllTime()/SpineSpeed * ANIMA_TIME_NUM;
    m_effTime = SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId)->getEffTime()/SpineSpeed * ANIMA_TIME_NUM;
    //GPCCLOG("attackTime = %f",m_attackTime);
   // GPCCLOG("effTime == %f", m_effTime);
   // CCLOG("afftribute2==%d",this->getAfftributeNum2());
    
    
    if (this->isMy)
    {
        CCString *str = CCString::createWithFormat(str_Formation_attack_array, this->getAp());
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }

    //初始化兵的时候,检测武将技能,并根据技能id获取技能管理器
    if (this->getSkillsId() >= SKILLS_ID1)
    {
        setSkillsManager_Delegate(SG_SkillsManager::initskManager(this->getSkillsId()));
        setSkillsDelegate(getSkillsManager_Delegate()->getskBase());
    }
    
    showAttackName();
    SGHeroLayer* myHeroLayer = NULL;
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getHeroLayer(true);
    }
    else
    {
        myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getHeroLayer(false);
    }
    
    if (this->getSkillsDelegate())
    
    {
        this->detectionReviveSkills(myHeroLayer->getBattleMap(), this, 0);
        this->activateSkills7(NULL, this, this->getAfftributeNum());
        switch (this->getSkillsId())
        {
            case SKILLS_ID13:
            case SKILLS_ID31:
            case SKILLS_ID44:
            {
                this->showSkillName();
            }
                break;
                
            default:
                break;
        }
    }
    
    
    SGBattleMap *bm=(SGBattleMap *)sb->getParent();
    if (sb->getSbType()==ksbtwo)
    {
       bm->twoList->removeObject(sb);

    }
    if (sb->getSbType()==ksbfour)
       bm->fourList->removeObject(sb);
  
    
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(myHeroLayer->getBattleMap()->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        if (attack->getSkillsDelegate())
        {
            attack->getSkillsManager_Delegate()->chackReviveSkills(myHeroLayer->getBattleMap(), this, attack);
        }
    }
    
    activateBuff();
    
    EFFECT_PLAY(MUSIC_2);
    playEffectWithType(kStoreEffect);
    
    bRet = true;
    
    return bRet;
}
//激活buff
void SGAttackList::activateBuff()
{
    bool isround = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer* myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getHeroLayer(isround);
    
    CCObject *attackobj = NULL;
    CCARRAY_FOREACH(myHeroLayer->getBattleMap()->m_heroBuff, attackobj)
    {
        CCDictionary *data = (CCDictionary*)attackobj;
        int buffId = ((CCString *)data->objectForKey("buffId"))->intValue();
        
        switch (buffId)
        {
            case 1004:
            {
                float value2 = ((CCString*)data->objectForKey("value2"))->floatValue();
                this->changeAP(this->getAp() + value2);
                this->drawSkillsEffect(1);
                
            }
                break;
                
            default:
                break;
        }
    }
}

CCArray *SGAttackList::getAttackSbs()
{
    return attackSbs;
}
int SGAttackList::getAttackJ()
{
    return attackJ;
}
//攻击阵列激活之后显示的相关信息,(技能名称,攻击)    //只有武将走这边，普通兵在NormalAttackList.cpp同名函数中
void SGAttackList::addShowMsg(sgAttackSoldierType type)
{
    ShiBing *sb = (ShiBing *)attackSbs->objectAtIndex(0);

    this->setRoundV(sb->getRoundV());
    this->setAp(HALF_FRICE(sb->getAp()));
    this->setDef(sb->getDef());
    this->setSbColor(sb->getSbColor());
    this->setID(sb->getSbId());
    this->setAddAp(sb->getAddAp());
    if (sb->getSkillsId() == SKILLS_ID33 || sb->getSkillsId() == SKILLS_ID34)
    {
        //特殊处理
        this->setAddRoundV(-1);
    }
    else
    {
        this->setAddRoundV(sb->getRoundV());
    }
    
    this->setName(sb->getName());
    this->setMax_Ap(sb->getAp());
    this->setSpAddAp(0);
    this->setAddChain(0);
    this->setSkillsId(sb->getSkillsId());
    this->setAfftributeNum(sb->getAfftributeNum());
    this->setAfftributeNum1(sb->getAfftributeNum1());
    this->setAfftributeNum2(sb->getAfftributeNum2());
    
    this->setBuffId(sb->getBuffId());
    this->setBuffRound(sb->getBuffRound());
    this->setBuffAfftribute(sb->getBuffAfftribute());
    this->setBuffType(sb->getBuffType());
    this->setEffectType(sb->getEffectType());
    
    this->setAddDef(sb->getAddDef());
    this->setChainNUm(sb->getChainNUm());
    this->setRoundVAddDef(sb->getRoundVAddDef());
    this->setAtkmode(sb->getAtkmode());
    this->setAtkMode2(sb->getAtkMode2());
    this->setAtkModeSkill(sb->getAtkModeSkill());
    this->setHeadId(sb->getHeadId());
    this->setFileId(sb->getFileId());
    this->setPair(sb->getPair());
    this->setHitEff(sb->getHitEff());
    this->setAttackEffect(sb->getAttackEffect());
    this->setDeathEffect(sb->getDeathEffect());
    this->setResurrectionScale(0.0);
    this->setSkill_Head(sb->getSkill_Head());
    
    //added by cgp
    this->setFlyId(sb->getFlyId());
    this->setFlyPosX(sb->getFlyPosX());
    this->setFlyPosY(sb->getFlyPosY());
    
    
    if (sb->getSkillsId() > 0)
    {
        SGSkillDataModel *skdata = SGStaticDataManager::shareStatic()->getGeneralSkillById(sb->getSkillsId());
        if(skdata)
        {
            this->setsbSkName(skdata->getSkillName()->m_sString);
        }
        else
        {
            this->setsbSkName(str_Wuzhuangji_name);
        }
    }
    else
    {
        this->setsbSkName(str_Wuzhuangji_name);
    }
    CCLOG("sb->getsbSkLevel()==%d",sb->getsbSkLevel());
    this->setsbSkLevel(sb->getsbSkLevel());
    
    CCLOG("AddAp===%d",this->getAddAp());
    CCLOG("Max_Ap==%d",this->getMax_Ap());
    CCLOG("BuffId  id==%d",this->getBuffId());
    CCLOG("BuffRound  id==%d",this->getBuffRound());
    CCLOG("BuffAfftribute  id==%f",this->getBuffAfftribute());
    CCLOG("BuffType  id==%d",this->getBuffType());
    CCLOG("name = %s",this->getName().c_str());
    CCLOG("atkmode2 = %d",this->getAtkMode2());
    CCLOG("HitEff = %d",this->getHitEff());
    CCLOG("AfftributeNum2===%d",sb->getAfftributeNum2());
    CCLOG("sb->getSkill_Head()==%d",sb->getSkill_Head());
    
    sb->addChild(m_effectsLayer);
    m_effectsLayer->setPosition(ccp(sb->getContentSize().width * .5, sb->getContentSize().height * 0.5));
    

    CCSprite *xuetiaoBj = NULL;
    if (type == kattacktwo)  //两格武将
    {
        xuetiaoBj= CCSprite::createWithSpriteFrameName("sb_xueTiao_bj.png");
        m_sb_xueTiao = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("sb_xueTiao.png"));
    }
    else    //四格武将
    {
        xuetiaoBj= CCSprite::createWithSpriteFrameName("sb_xuetTiao2_bj.png");
        m_sb_xueTiao = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("sb_xuetTiao2.png"));
        ;
    }

    m_sb_xueTiao->setType(kCCProgressTimerTypeBar);
    m_sb_xueTiao->setMidpoint(ccp(0,0));
    m_sb_xueTiao->setBarChangeRate(ccp(1, 0));

    sb_xuetiao = m_sb_xueTiao->getContentSize().width;
   
     
    sb->addChild(xuetiaoBj, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG);
    sb->addChild(m_sb_xueTiao, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 2);
    
    char roundNumStr[256] = {0};
    sprintf(roundNumStr, "%d",this->getRoundV());
    
    roundNumLabel = CCLabelAtlas::create(roundNumStr, "battle/sb_round_label.png", SB_ROUND_LABEL_W, SB_ROUND_LABEL_H, '0');
    sb ->addChild(roundNumLabel, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 3);
    
    
    //跳动动画
    if (this->getRoundV() < 3 && this -> getRoundV() > 0)
    {
        CCBlink *blink = CCBlink::create(3, 10/this->getRoundV());
        CCRepeatForever *repeat = CCRepeatForever::create(blink);
        roundNumLabel->runAction(repeat);
    }
    
    char apStr[256] = {0};
    sprintf(apStr, "%d",this->getAp());
    
    apLabel = CCLabelAtlas::create(apStr, "battle/sb_xue_label.png", SB_XUE_LABEL_W, SB_XUE_LABEL_H, '0');
    sb->addChild(apLabel, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 4);

    apLabel->setAnchorPoint(ccp(1, 0));

    //两格或四格武将  cgpUseful
    xuetiaoBj->setPosition(ccp(0, apLabel->getContentSize().height - 10));
    apLabel->setPosition(ccp(xuetiaoBj->getContentSize().width * 0.5, 9));

    m_sb_xueTiao->setPosition(ccp( 0, xuetiaoBj->getPosition().y));

    roundNumLabel->setPosition(ccp(-30, apLabel->getContentSize().height + 5 + xuetiaoBj->getContentSize().height * 0.5+ 80));
    roundNumLabel->setAnchorPoint(ccp(0.5, 0));

    drawXueTiaoProportion();
    posxx = ccp(0, apLabel->getContentSize().height + 5 + xuetiaoBj->getContentSize().height * 0.5);
    roundNum = CCSprite::create();
    sb->addChild(roundNum,EFFECT_LAYER_TAG, ATTACK_ROUND_EFF_TAG);
   // roundNum->setPosition(ccpAdd(roundNumLabel->getPosition(), ccp(0, roundNumLabel->getContentSize().height)));
    roundNum->setPosition(ccpAdd(roundNumLabel->getPosition(), ccp(0, 15)));
    changeRound(0, true);
}

//改变攻击阵列的续力回合
void SGAttackList::changeRound(int round, bool isAction)
{

    if (round < 0)
    {
        EFFECT_PLAY(MUSIC_19);
    }
    int nextRound = this->getRoundV() + round;
    nextRound = nextRound>0?nextRound:0;
    this->setRoundV(nextRound);
    if (roundNum->getChildByTag(ATTACK_ROUND_EFF_TAG))
    {
        CCSpriterX *s = (CCSpriterX*)roundNum->getChildByTag(ATTACK_ROUND_EFF_TAG);
        s->removeFromParentAndCleanup(true);
        s = NULL;
    }
    roundNum->removeChildByTag(ATTACK_ROUND_EFF_TAG);
    if (nextRound < 3)
    {
        if (nextRound == 0)
        {
            return;
        }
        roundNumLabel->stopAllActions();
        roundNumLabel->setVisible(false);
        CCString *str = CCString::createWithFormat("animationFile/round_%d.scml",nextRound);
        CCSpriterX *roundn = CCSpriterX::create(str->getCString(), true, true);
        roundn->setanimaID(0);
        roundn->setisloop(true);
        roundn->play();
        roundn->setAnchorPoint(ccp(0.5, 0.5));
        roundn->setPosition(CCPointZero);
        roundNum->addChild(roundn, EFFECT_LAYER_TAG, ATTACK_ROUND_EFF_TAG);
        return;
    }
    else
    {
        roundNumLabel->setVisible(true);
    }

    if (!isAction)
    {
        char roundNumStr[256] = {0};
        sprintf(roundNumStr, "%d",this->getRoundV());
        roundNumLabel->setString(roundNumStr);
        
        roundNumLabel->stopAllActions();
        roundNumLabel->setVisible(true);
        //跳动动画
        if(this->getRoundV() == 0 || this->getRoundV() > 2)
        {
            return;
        }
        CCBlink *blink = CCBlink::create(3, 10/this->getRoundV());
        CCRepeatForever *repeat = CCRepeatForever::create(blink);
        roundNumLabel->runAction(repeat);
        
    }
    else
    {
        labelJump(roundNumLabel, this->getRoundV());
    }
    
    
}

void SGAttackList::changeAP(int ap, bool isAction, bool isPlayEff, bool isDrawXuetiao)
{
    currentAp = this->getAp();
    addAp = ap-currentAp;
    finialAp = ap;
    if (addAp > 0)
    {
        isAdd = true;
    }
    else
    {
        isAdd = false;
        addAp = -addAp;
    }
    
    CCCallFunc *call=CCCallFunc::create(this, callfunc_selector(SGAttackList::scaleTOSec));
    CCCallFunc *call1=CCCallFunc::create(this, callfunc_selector(SGAttackList::updatebloodNum));
    CCSequence *seq=CCSequence::create(call1,call,NULL);
    
    this->runAction(seq);
    if (apLabel)
    {
        apLabel->runAction(CCScaleTo::create(0.3, 2.0f));  //removed by cgp
        updatebloodNum();
    }

    int nextAP = ap;
    nextAP = nextAP>0?nextAP:0;
    this->setAp(nextAP);
    
    
    if (isPlayEff && this->getAttackType() != kattacknormal)
    {
        this->playEffectWithType(kBlockEffect);
    }
    
    if (!isAction)
    {
        char apStr[256] = {0};
        sprintf(apStr, "%d",this->getAp());
        if (addAp == 0)
        {
            this->apLabel->setString(apStr);
        }
    }
    else
    {
        
        this->labelJump(apLabel, this->getAp());
    }
    if (isDrawXuetiao)
    {
        this->drawXueTiaoProportion();
    }

}

//更新血条和数字   //added by cgp
void SGAttackList::updateBlood(int leftBlood)
{
    char apStr[64] = {0};
    sprintf(apStr, "%d",leftBlood);
    this->apLabel->setString(apStr);
    
    int b = this->getMax_Ap();
    
    int d = leftBlood * 100 / b;
    if (d > 100)
    {
        d = 100;
    }
    
    if (d <= 0)
    {
        d = 0;//为了防止报错 = 0.5；
    }
    
    //GPCCLOG("==================MAX AP = %d, leftAP = %d, percentage = %d", b, leftBlood, d);
    
    this->m_sb_xueTiao->setPercentage(d);
}

//攻击阵列在这个网格里
bool SGAttackList::isGridin(SGGridBase *grid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackSbs, obj)
    {
        ShiBing *insb = (ShiBing*)obj;
        if(insb->isGridIn(grid))
        {
            return true;
        }
    }
    return false;
}
//向服务器推送攻击数据
void SGAttackList::pushAttackData(CCString *str, bool push)
{
    if (SGAIManager::shareManager()->isPVE)
    {
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    else
    {
        bool isRound = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        if (isRound)
        {
            SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
            CCLOG(" 攻击 攻击者输出===> %s", str->getCString());
        }
        else
        {
            CCLOG(" 攻击 被攻击者输出===> %s", str->getCString());
        }
    }
    
}

void SGAttackList::delayAttackOver(CCNode*  psender, void *mydate)
{
    myHero->attackOver(this);
}

void SGAttackList::myDispear(CCNode* psender, GameIndex * sbindex)
{
    if ( this->getAttackType() == kattacknormal)
    {
        int sbIndexI = sbindex->i;
        int sbIndexJ = sbindex->j;
        GameIndex sbIndex;
        GameIndex sbIndex2;
        GameIndex sbIndex3;
        
        sbIndex.i = sbIndexI -1;
        sbIndex.j = sbIndexJ;
        sbIndex2.i = sbIndexI - 2;
        sbIndex2.j = sbIndexJ;
        sbIndex3.i = sbIndexI - 3;
        sbIndex3.j = sbIndexJ;
        attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex);
        attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex2);
        attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex3);
    }
    else if(this->getAttackType() == kattacktwo)
    {
        int sbIndexI = sbindex->i;
        int sbIndexJ = sbindex->j;
        GameIndex sbIndex;
        sbIndex.j = sbIndexJ;
        CCPoint difPos = ccp(0, 0);
        if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            sbIndex.i = sbIndexI -1;
            difPos.y = 0.5 * gridHeight;
            attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex, difPos);
        }
        else
        {
            sbIndex.i = sbIndexI -1;
            attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex, difPos);
        }

    }
    else  //四格
    {
        
    }
   
}

void SGAttackList::myDispearRemote(CCNode* psender, GameIndex * sbindex)
{
    int sbIndexI = -m_dispearRemote.i;
    int sbIndexJ = mapList -1 - m_dispearRemote.j;
    GameIndex sbIndex;
    GameIndex sbIndex2;
    GameIndex sbIndex3;
    sbIndex.i = sbIndexI -4;
    sbIndex.j = sbIndexJ;
    sbIndex2.i = sbIndexI -3;
    sbIndex2.j = sbIndexJ;
    sbIndex3.i = sbIndexI - 2;
    sbIndex3.j = sbIndexJ;
    attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex);
    attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex2);
    attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex3);
}

void SGAttackList::attackEff(CCNode*  psender, GameSbIndex *sbindex)    //打墙和阵列特效
{
    int gridRow = sbindex->i;
    int gridList = sbindex->j;
    bool isme = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    
    int iFileId = getFileId();
    int wallID =  SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId)->getWallEffId();
    effWallHit = CCSkeletonAnimation::createWithFile
    (
     CCString::createWithFormat("effSpine/effect%03d.json", wallID)->getCString(),
     CCString::createWithFormat("effSpine/effect%03d.atlas", wallID)->getCString()
     );
    effWallHit->setAnimation("animation", false);
    
    if(isme)
    {
        effWallHit->setAnchorPoint(ccp(0.5, 0.5));
    }
    else
    {
        effWallHit->setScaleY(-1);
        effWallHit->setAnchorPoint(ccp(0.5, 0.5));
        gridRow -= 1;
    }
    
    CCPoint  effPos = GameConfig::getGridPoint(gridRow , gridList ,!isme);  //there isme 分上下，ture 为下
//    if (!isme)
//    {
//        effPos.y += gridHeight;
//    }
    
//    if(getAttackType() == kattackfour)
//    {
//        effWallHit->setPosition(ccp(isme ? effPos.x + 0.5 * gridWidth : effPos.x - 0.5 * gridWidth  , effPos.y));
//    }
//    else
    {
        effWallHit->setPosition(ccp(effPos.x , effPos.y));
    }
    
    
    if(isme)
    {
        myHero->addChild(effWallHit, 10000);
    }
    else
    {
        attackHero->addChild(effWallHit, 10000);
    }
    
    CCDelayTime*  delay = CCDelayTime::create(0.5f);
    CCCallFunc*   call = CCCallFunc::create(this, callfunc_selector(SGAttackList::callRemoveHitWall));
    this->runAction(CCSequence::create(delay, call, NULL));
}

//攻击阵列, 开始攻击
void SGAttackList::startAttack(SGHeroLayer *myHero_, SGHeroLayer *opponentHero)
{
#if PUSH_ATTACK_DATA
    pushAttackData(CCString::createWithFormat(str_Format_attackerId_attack_column, this->getID(), this->getAp(), this->getAttackIndex().j), false);
#endif
   
    m_isDead = false;
    this->playAttackEf(this->getAttackEffect());
    myHero = myHero_;
    attackHero = opponentHero;
    
    
    int myZorder = myHero->getZOrder();
    int enemyZorder = attackHero->getZOrder();
    GPCCLOG("++++++++++++++++++++++++++++++++++ myZorder:%d,  enemyZorder:%d", myZorder, enemyZorder);
    /******************************游戏攻击方式*************************************/
    /*      上边格子排列
        7  6  5  4  3  2  1  0
        |--|--|--|--|--|--|--|--| 5       -7      上边线
        |--|--|--|--|--|--|--|--| 4       -6
        |--|--|--|--|--|--|--|--| 3       -5
        |--|--|--|--|--|--|--|--| 2       -4
        |--|--|--|--|--|--|--|--| 1       -3
        |--|--|--|--|--|--|--|--| 0       -2
                 分界占一行                -1
        0 |--|--|--|--|--|--|--|--|       0
        1 |--|--|--|--|--|--|--|--|       1
        2 |--|--|--|--|--|--|--|--|       2
        3 |--|--|--|--|--|--|--|--|       3
        4 |--|--|--|--|--|--|--|--|       4
        5 |--|--|--|--|--|--|--|--|       5
          0   1  2  3  4  5  6  7         6
           下边格子排列
     */
    
    //cgpTest for git
    GameIndex index = this->getAttackIndex();  //最前排的一个兵的index
    m_dispearRemote.i = index.i;
    m_dispearRemote.j = index.j;
    int indexJ = index.j;
    //GPCCLOG("前排一个兵的index : i, j  : %d, %d", index.i, index.j);
//    CCLOG("选择的列 %d",indexJ);
//    CCLOG("打列%d",abs(mapList-1-indexJ));
    
    

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
    
    float delay_time1 = 0.0f;     //每格额外的延时时间（攻击动作时间等，每次循环进行累加
    float att_time = 0.0f;
    bool isme = true;
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        isme = false;
    }

    GPCCLOG("\n========================startAttack begin========================");
    
    CCObject *obj = NULL;
    
    //3个组成士兵相关,武将的话是一个
    CCARRAY_FOREACH(attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        sb->hideXuLiEffect();
        int act = ACT_atkUp;
        
        int moveAct = ACT_moveUp;
        
        if (!isme)
        {
            act = ACT_atkDown;
            moveAct = ACT_moveDown;
        }

        att_time = m_attackTime ;

        if (this->getAtkmode() == 0)  //近战
        {
            int a = sb->getSbindex().i - index.i - (isme ? 1 : -1);
            CCDelayTime *delay = CCDelayTime::create(0);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)moveAct);
            CCPoint movepos2 =  GameConfig::getGridPoint(isme ? -(mapRow - a) : (mapRow - a), abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true);
            CCMoveTo *move = CCMoveTo::create(moveTime, movepos2);
            CCAction* action = (CCAction*) CCSequence::create(delay , call, move, NULL);
            action->setTag(110);
            sb->runAction(action);
            
        }
        else   //远程
        {
            sb->setanimaID(act, false);
        }
    }
    
    if (this->getAtkmode() == 1)  //远程
    {

        GPCCLOG("FlyId :  %d\n\n", this->getFlyId());
        jianEffect = CCSkeletonAnimation::createWithFile
        (
             CCString::createWithFormat("effSpine/fly%03d.json", this->getFlyId())->getCString(),
             CCString::createWithFormat("effSpine/fly%03d.atlas", this->getFlyId())->getCString()
         );
        jianEffect->setAnimation("animation", true);

        if (this->getAttackType() == kattacktwo)
        {
            this->playEffectWithType(kAttackEffect);
        }
        //如果是敌方进行攻击，那么就把箭翻转过来
        if(!isme &&
           this->getAtkMode2() < 100)
        {
            jianEffect->setScaleY(-1);
        }
        
        CCPoint jianPos = GameConfig::getGridPoint(isme ? (index.i + 1) : -index.i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -indexJ :  (mapList - 1 - indexJ)), true);
//                movePos.y = isme ? movePos.y + 0.5 * jianEffect->getContentSize().height : movePos.y - 0.5 * jianEffect->getContentSize().height;    //减去剑的高度的一半，箭的锚点在（0.5, 0.5)
        if (isme)
        {
            jianPos.y += (1.75 * gridHeight);
        }
        else
        {
            jianPos.y +=  0.25 * gridHeight;
        }
        jianEffect->setPosition(ccp(jianPos.x, jianPos.y));
        CCHide *hide = CCHide::create();
        CCDelayTime *delay = CCDelayTime::create(m_effTime);
        CCShow *show = CCShow::create();
        CCPoint movePos = GameConfig::getGridPoint(isme ? -(mapRow + 1) : mapRow, abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true);
        CCMoveTo *move = CCMoveTo::create(moveTime, ccp(movePos.x , movePos.y));
        jianEffect->runAction(CCSequence::create(hide, delay, show,  move, NULL));
        myHero->addChild(jianEffect, JIAN_EFFECT_TAG, JIAN_EFFECT_TAG);

    }
    attOver = true;
    bool isAttack = false;
    int _i = 0;
    
    for (int i = 0; i <= mapRow; i++)
    {
        if ( i > _i)
        {
            isAttack = false;
            _i = 0;
        }
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
            float b = sbTime * j ;
            shengYuTime = moveTime - b;

            b = b + delay_time1;

            
            if (i == mapRow)
            {
                GPCCLOG("攻击底线:::::: " );
                CCObject *obj = NULL;
                CCARRAY_FOREACH(attackSbs, obj)
                {
                    ShiBing *sb = (ShiBing*)obj;
                    CCDelayTime* delay1 = CCDelayTime::create(b);
                    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::vanishAttackList));
                    CCFiniteTimeAction *action = CCSequence::create(delay1, call, NULL);
                    sb->runAction(action);
                }

                float c = b + m_effTime ;
                CCDelayTime *delaytime = CCDelayTime::create(c);  //刀尖时间
                CCCallFuncND*  callEffBz = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::cgpEffectTest), (void*)indexJ);
                CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::changBlood));
                myHero->runAction(CCSequence::create(delaytime, callEffBz,   call, NULL));
                
            }
            else
            {
                //没有打到底线
                //GPCCLOG("没有打到底线::::::");
                SGGridBase *grid = attackHero->battleMap->myGrids[i][abs(mapList-1-indexJ)];

                //被打格子的位置。
                if (grid)   //格子上有东西
                {
                    GameSbIndex *sbindex = new GameSbIndex();
                    sbindex->i = grid->getIndex().i;
                    sbindex->j = grid->getIndex().j;
                    sbindex->_time = shengYuTime;
                    
                    if (grid->getStype() == knormal)   //散兵
                    {
                        if (grid->getSbType() != ksbone)
                        {
                            i++;
                            /*补丁*/
                            ShiBing *sbs = attackHero->battleMap->getShiBingWithGrid(grid);
                            if(sbs)
                            {
                                SGGridBase *gridds = (SGGridBase*)sbs->getPrinceGridArray()->objectAtIndex(0);
                                sbindex->i = gridds->getIndex().i;
                            }
                        }
                        
                         sbindex->is_sanbing = true;
                        CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::removesb), (void*)sbindex);
                        if (this->getAtkmode() == 0)  //近战
                        {
                            CCCallFuncND* callPause = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::movePause), (void*)sbindex);
                            CCDelayTime* delay = CCDelayTime::create(b);
                            myHero->runAction(CCSequence::create(delay, callPause, call, NULL));
                        }
                        else    //远程
                        {
                            CCDelayTime* delay = CCDelayTime::create(b + m_effTime);
                            myHero->runAction(CCSequence::create( delay,  call, NULL));
                        }
                    }
                    else if(grid->getStype() == kdefend)   //防御格子
                    {
                        if (this->getAtkmode() == 0)  //近程
                        {
                            delay_time1 += att_time;

                            CCDelayTime *delaytime = CCDelayTime::create(b);

                            CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                   callfuncND_selector(SGAttackList::setAct),
                                                                   (void*)sbindex);
                            myHero->runAction(CCSequence::create(delaytime, call1, NULL));
                            
                        }
                        else  //远程
                        {
                            CCDelayTime* delay = CCDelayTime::create(b + m_effTime);
                            CCCallFuncND*  callEffect = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::attackEff), (void*)sbindex);    //攻击特效
                            
                            CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::removesb), (void*)sbindex);
 
                            myHero->runAction(CCSequence::create(delay, callEffect, call, NULL));
 
                        }
                        
                    }
                    else    //攻击格子
                    {
                        SGAttackList* attack = attackHero->battleMap->getAttackListByGrid(grid);
                        if(attack)
                        {
                    
                            float time = 0;
                            if (!attack->getIsDeath())
                            {
                                if (!isAttack) {
                                    isAttack = true;
                                    if (attack->getAttackType() == kattacknormal)
                                    {
                                        _i = i + 2;
                                    }
                                    else
                                    {
                                        _i = i + 1;
                                        /*补丁*/
                                        ShiBing *sbs = attackHero->battleMap->getShiBingWithGrid(grid);
                                        if(sbs)
                                        {
                                            SGGridBase *gridds = (SGGridBase*)sbs->getPrinceGridArray()->objectAtIndex(0);
                                            sbindex->i = gridds->getIndex().i;
                                        }
                                    }
                                    
                                    if (this->getAtkmode() == 0)  //近战
                                    {
                                        
                                        delay_time1 += att_time;

                                        CCDelayTime *delaytime = CCDelayTime::create(b);
                                        CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                                   callfuncND_selector(SGAttackList::setAct),
                                                                                   (void*)sbindex);
                                        myHero->runAction(CCSequence::create(delaytime, call1, NULL));
                                        
                                     }
                                    else  //远程
                                    {
                                        CCDelayTime* delay = CCDelayTime::create(b + m_effTime);
                                        CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::removesb), (void*)sbindex);
                                        CCCallFuncND*  callEffect = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::attackEff), (void*)sbindex);    //攻击特效
                                        myHero->runAction(CCSequence::create(delay, callEffect, call, NULL));

                                    }
                                    
                                }

                            }
                        }

                    }
                        
                }
                else    //格子为空地
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

void SGAttackList::attackSbVibration()
{
    //GPCCLOG("attackSbVibration");
    if (this->getAp()>0)
        SGBattleManager::sharedBattle()->getBattleLayer()->fuckLayer(0,true );
}
//技能buff触发
void SGAttackList::addSkillsBuff(CCObject *obj, int* rand)
{
    
    GameIndex index_ = gameIndex(*rand, abs(mapList-1-this->getAttackIndex().j));
    
    GameIndex index__ = index_;
    
    if (this->getAttackType() == kattackfour) {
        index__ = gameIndex(index_.i, index_.j - 1);
    }
    
    if (this->getSkillsDelegate()) {
        this->activateBuffSkills(NULL, this, gameIndex(index_.i, index_.j), gameIndex(index__.i, index__.j));
    }
}

//消失动画  每个兵都执行
void SGAttackList::vanishAttackList(CCObject *obj)
{
    float time = 0.0;
    ShiBing *sb = (ShiBing*)obj;
    sb->stopActionByTag(110);
    if (this->getAtkmode() == 1)
    {

        if (jianEffect)
        {
            CCDelayTime*  jianEffDelay = CCDelayTime::create(m_effTime);
            CCCallFunc*   jianEffDelayCall = CCCallFunc::create(this, callfunc_selector(SGAttackList::cgpCallJianEffDelay));
            jianEffect->runAction(CCSequence::create(jianEffDelay, jianEffDelayCall, NULL));
        }
    }
    else
    {
        //近战
        EFFECT_PLAY(MUSIC_16);

    }
    
    CCDelayTime *delay = CCDelayTime::create(0);  //before is time ,modified by cgp
    CCCallFuncO *callo = CCCallFuncO::create(this, callfuncO_selector(SGAttackList::removeAttackSb), sb);
    CCFiniteTimeAction *sequence = CCSequence::create(delay,callo,NULL);
    sb->runAction(sequence);
}

void SGAttackList::testlog()
{
    CCLOG("调用了这个方法");
}
//设置动作和动作的时间
float SGAttackList::setActAndTime(ShiBing *sb, bool onlyTime)
{
    int act = ACT_atkUp;
    float atkTime = 0;
    if (!onlyTime)
    {
        
        if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            act = ACT_atkDown;
        }
        sb->setanimaID(act, false);
       // cgpEffectTest();
    }

//    if (mAttIsSpine)
//    {
//        atkTime = spineAtkTimeTest * ANIMA_TIME_NUM;
//    }
    atkTime = m_attackTime;
    return atkTime;
}


//攻击的移动动作 //cgpUseful   //打到城墙和攻击阵列
void SGAttackList::setAct(cocos2d::CCObject *obj, GameSbIndex *sbindex)
{
    
    if (m_isDead)
    {
        return;
    }
    
    CCCallFunc *callVibration = CCCallFunc::create(this, callfunc_selector(SGAttackList::attackSbVibration));

    GameIndex index = this->getAttackIndex();
    
    float time1 = sbindex->_time;
    SGGridBase *grid = attackHero->battleMap->myGrids[sbindex->i][sbindex->j];
    
    float attackTime = 0.0;
    float b = 0.0;
    CCObject *obj1 = NULL;
    CCARRAY_FOREACH(attackSbs, obj1)
    {
        ShiBing *sb = (ShiBing*)obj1;
        sb->stopActionByTag(110);
        int act = ACT_atkUp;
        int moveAct = ACT_moveUp;
        bool isme = true;
        //不是玩家的就向下
        if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {
            //act = aAction_5;
             act = ACT_atkDown;
            moveAct = ACT_moveDown;
            isme = false;
        }
        
//        if (mAttIsSpine)
//        {
//            attackTime = spineAtkTimeTest * ANIMA_TIME_NUM;
//        }
        attackTime = m_attackTime;


        CCDelayTime *delay = CCDelayTime::create(0);
        CCCallFuncND *call0 = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setAttAction), (void*)act);

        CCDelayTime *delay1 = CCDelayTime::create(attackTime);
        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)moveAct);
        int a = sb->getSbindex().i - index.i - (isme ? 1 : -1);  //not me , 越大越靠上
        CCPoint pos = GameConfig::getGridPoint(isme ? -(mapRow - a) : (mapRow - a), abs(isme ? -sb->getSbindex().j : (mapList - 1 - sb->getSbindex().j)),true);
       
        CCMoveTo *move = CCMoveTo::create(time1, pos);

        CCAction *action = (CCAction*) CCSequence::create(delay, call0, delay1, call, move, NULL);
        action->setTag(110);
        sb->runAction(action);
        
    }

//    CCDelayTime *effectDelay = CCDelayTime::create(Effect_DelayTime);
     CCDelayTime *effectDelay = CCDelayTime::create(m_effTime);
    CCCallFuncND*  callEffect = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::attackEff), (void*)sbindex);
    
    myHero->runAction(CCSequence::create(effectDelay, callEffect, NULL));


    
//    CCDelayTime* delay = CCDelayTime::create(Effect_DelayTime);
    CCDelayTime* delay = CCDelayTime::create(m_effTime);

    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::removesb), (void*)sbindex);
    myHero->runAction(CCSequence::create( delay, call, callVibration, NULL));
}
//移动中止
void SGAttackList::movePause(CCObject *obj, GameSbIndex *sbindex)
{
    if (this->getAtkmode() != 0)
    {
        return;
    }
    GameIndex index = this->getAttackIndex();
    float time1 = sbindex->_time;
    bool isme = true;
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {
        isme = false;
    }
    
    CCObject *obj1 = NULL;
    CCARRAY_FOREACH(attackSbs, obj1)
    {
        ShiBing *sb = (ShiBing*)obj1;
        sb->stopActionByTag(110);
        
//        CCDelayTime *delay = CCDelayTime::create(ATTACK_DEALY_TIME);
        
        //近战非打底线时的位置
        int a = sb->getSbindex().i - index.i - (isme ? 1 : -1);
        CCPoint pos = GameConfig::getGridPoint(isme ? -(mapRow - a) : (mapRow - a ), abs(isme ? -sb->getSbindex().j : (mapList - 1 - sb->getSbindex().j)),true);
        CCMoveTo *move = CCMoveTo::create(time1, pos);
        
       
        //GPCCLOG("time slice %f", time1);
        //GPCCLOG("Current Pos[%d]: %f, %f\n", sb->getTag(),sb->GetPosition().x, sb->GetPosition().y);
        //GPCCLOG("Target Pos[%d]: %f, %f\n", sb->getTag(),pos.x, pos.y);
        //GPCCLOG("---------------------------\n");
        CCAction *action = (CCAction*) CCSequence::create( move, NULL);
        action->setTag(110);
        sb->runAction(action);
        
        
    }
}

//去掉攻击阵列中的兵   每个兵都执行
void SGAttackList::removeAttackSb(CCObject *obj)
{
    ShiBing *sb = (ShiBing*) obj;
    float time = 0;
    float nearTime = 0;     //近战增加延时
    if (sb->getAtkmode() == 0)//近战
    {
//        attackHero->showJianBlastEffect(sb->getHitEff(), true, gameIndex(mapRow, mapList - 1 - sb->getSbindex().j), true);
        
        if (this->getAttackType() == kattacktwo)
        {
            //removed by cgp
           // attackHero->showJianBlastEffect(sb->getHitEff(), true, gameIndex(mapRow, mapList - 1 - sb->getSbindex().j), true);
        }
        else
        {
            attackHero->showJianBlastEffect(sb->getHitEff(), true, gameIndex(mapRow, mapList - 1 - sb->getSbindex().j), true);
        }
        time = setActAndTime(sb);
       // nearTime = 0.5;

    }
    else  //远程
    {
        nearTime = m_effTime + 0.5;  //跑到点的时间开始算
    }
    
    CCDelayTime* delay = CCDelayTime::create(time + nearTime);
    CCCallFuncN* call = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::removeAttack));
    CCAction *act = CCSequence::create(delay, call, NULL);
    sb->runAction(act);

}
//攻打底线后移去攻击阵列  每个兵都执行
void SGAttackList::removeAttack(cocos2d::CCObject *obj)
{
//    bool isme = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();

    ShiBing *sb = (ShiBing*) obj;
   
    //GPCCLOG("+++++++++++++++++++++++ atkSize %d", atkSize);
    if (this->getAtkmode() == 0)
    {
        if ( this->getAttackType() == kattacknormal)
        {
            int sbIndexI = 6;
            int sbIndexJ = mapList - 1 - sb->getGridIndex().j;
            GameIndex sbIndex;
            GameIndex sbIndex2;
            GameIndex sbIndex3;
            sbIndex.i = sbIndexI -1;
            sbIndex.j = sbIndexJ;
            sbIndex2.i = sbIndexI - 2;
            sbIndex2.j = sbIndexJ;
            sbIndex3.i = sbIndexI - 3;
            sbIndex3.j = sbIndexJ;
            attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex);
            attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex2);
            attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex3);
        }
        else  if(this->getAttackType() == kattacktwo)
        {
            int sbIndexI = 6;
            int sbIndexJ = mapList - 1 - sb->getGridIndex().j;
            GameIndex sbIndex;
            
            
            sbIndex.j = sbIndexJ;
            CCPoint difPos = ccp(0, 0);
            if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
            {
                sbIndex.i = sbIndexI -1;
                difPos.y = 0.5 * gridHeight;
                attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex, difPos);
            }
            else
            {
                sbIndex.i = sbIndexI - 2;
                //difPos.y = 0.5 * gridHeight;
                attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex, difPos);
            }
        }
        else  //四格武将
        {
            
        }

    }
    else
    {
        int sbIndexI = - (sb->getSbindex().i);
        int sbIndexJ = (mapList - 1 - sb->getGridIndex().j);
        GameIndex sbIndex;
        GameIndex sbIndex2;
        GameIndex sbIndex3;
        sbIndex.i = sbIndexI -0;
        sbIndex.j = sbIndexJ;
        sbIndex2.i = sbIndexI - 1;
        sbIndex2.j = sbIndexJ;
        sbIndex3.i = sbIndexI - 2;
        sbIndex3.j = sbIndexJ;
        attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex);
        attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex2);
        attackHero->showEffectLayerCgp(Eff_xiaoshi, true, sbIndex3);
        //GPCCLOG("##### sbIndexI = %d, sbIndexJ = %d, sb.i  = %d, sb.j = %d", sbIndexI,  sbIndexJ, sb->getSbindex().i, sb->getSbindex().j);
    }
    sb->removeFromParentAndCleanup(true);
    sb = NULL;
    myHero->attackOver(this);
}


void SGAttackList::cgpCallRmSbDelay(cocos2d::CCNode * pSender)
{
    ShiBing*  sb = (ShiBing*)pSender;
    attackHero->battleMap->removeAttackedSB(sb, 0, myHero);  //把对方打死了 //cgpUseful
}

void SGAttackList::cgpCallRmAttackList()
{
    SGAttackList * soldierattack = attackHero->battleMap->getAttackListByGrid(mCgpGrid);
    if(soldierattack)
    {
        //removed by cgp
        //attackHero->showEffectLayer(2, false, gameIndex(grid->getIndex().i, grid->getIndex().j));

        
        attackHero->battleMap->removeAttackList(soldierattack, attackHero, mCgpGrid);
    }

}

//攻击之后的数据改变,攻击之后兵移除等等
void SGAttackList::removesb(cocos2d::CCObject *obj, GameSbIndex *sbindex)
{
    if (this->getAp() <= 0) {
        return;
    }
    //attackSbVibration();
    
    int defNum = 0;
    int skillsNum = 0;
    int resistForce = 0;
    int leftBlood = 0;
    int row = 0;
    
    SGGridBase *grid = attackHero->battleMap->myGrids[sbindex->i][sbindex->j];
    if (grid)
    {
        ShiBing* sb = attackHero->battleMap->getShiBingWithGrid(grid);   //被攻击格子的士兵
        if(sb)
        {
            if (this->getAtkmode() == 0)
            {
                //attackHero->showEffectLayer(10, false, sb->getSbindex());
            }
            
            
            //active skills
            //被攻击的是散兵
            if (grid->getStype() == knormal)
            {
                
                resistForce = sb->getDef();
                defNum = sb->getDef();
                skillsNum = sb->getDef();
                if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j]) //cgpGrid
                {
                    resistForce = this->activateSkills5_1(NULL, this, resistForce);
                    resistForce = this->activateSkills5(NULL, this, resistForce, sb);
                    if (resistForce == 0) {
                        attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                    }
                }
                if (sb->getSbType() != ksbone) {
                    row = 1;
                }
                
                
            }
            //防御阵列
            else if(grid->getStype() == kdefend)
            {
                
                if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j]) //cgpGrid
                {
                    if (this->activateSkills11(NULL, this, sb) )
                    {
                        attackHero->showBattleLabel(this->getAfftributeNum() * sb->getDef(), "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex(), 1.6);
                    }
                }
                resistForce = sb->getDef();
               
            }
            //攻击阵列
            else
            {
                
                SGAttackList *soldierAttack = attackHero->battleMap->getAttackListByGrid(grid);
                if(soldierAttack)
                {
                    row = 0;
                    if (soldierAttack->getAttackType() != kattacknormal) {
                        row = 1;
                    }
                    else if (soldierAttack->getAttackType() == kattacknormal)
                    {
                        row = 2;
                    }
                        
                    
                    if (soldierAttack->getSkillsDelegate())
                    {
                        
                        soldierAttack->activatePassivitySkills(NULL, soldierAttack, soldierAttack->getAfftributeNum(),this);
                        soldierAttack->activatePassivitySkills1(NULL, soldierAttack, this->getAp());
                    }
                    
                    resistForce = soldierAttack->getAp();
                    skillsNum = soldierAttack->getAp();
                    //遇强则强
                    if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j])  //cgpGrid
                    {
                        if (//(soldierAttack->getAttackType() == kattackfour ||  soldierAttack->getAttackType() == kattacktwo)
                            this->getAp() < soldierAttack->getAp())
                        {
                            this->activateSkills6(NULL, this, this->getAfftributeNum());
                        }
                        resistForce = this->activateSkills5_1(NULL, this, resistForce);
                    }
                }
                
            }
            
            bool w = false;
            if (this->getSkillsDelegate())
            {
                if (grid->getStype() != kdefend)
                {
                    w = this->activateSkills1(NULL, this, skillsNum);
                    
                    if (w)
                    {
                        if (grid->getStype() == kattack)
                        {
        //                    CCObject *sbAtt = NULL;
        //                    CCARRAY_FOREACH(sbArray, sbAtt)
        //                    {
        //                        ShiBing *attSb = (ShiBing*)sbAtt;
        //                        attackHero->showEffectLayer(3, true, attSb->getSbindex());
        //                    }
                        }
                        else
                        {
                            attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
                        }
                    }
                }
            }
            
            
            leftBlood = this->getAp() - resistForce;
            
            
           
            
    #if PUSH_ATTACK_DATA
            data = CCString::createWithFormat(str_Format_attack_behitId_defense, this->getAp(), sb->getSbId(), resistForce);
            pushAttackData( data, false);
    #endif
            
            CCArray *sbArray = CCArray::create();
            CCArray *listvanishArray = CCArray::create();
            
            if (leftBlood >=0 )
            {
                
                updateBlood(leftBlood);
                //removed by cgp
//                if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j])  //cgpGrid
//                {
//                    this->activateSkills12(NULL, this, this->getAfftributeNum(), gameIndex(grid->getIndex().i, grid->getIndex().j), gameIndex(grid->getIndex().i, grid->getIndex().j));
//                }
                

                
                
                //modified by cgp
                if (this->getAtkmode() == 0)   //近战
                {
                    //attackHero->showBattleLabel(resistForce, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                    if(grid->getStype() == kattack)//攻击阵列
                    {
                        SGAttackList * soldierattack = attackHero->battleMap->getAttackListByGrid(grid);
                        if(soldierattack)
                        {
                            attackHero->showEffectLayer(2, false, gameIndex(grid->getIndex().i, grid->getIndex().j));
                            sbArray->addObjectsFromArray(soldierattack->attackSbs);
                            listvanishArray->addObjectsFromArray(soldierattack->attackSbs);
                            attackHero->battleMap->removeAttackList(soldierattack, attackHero, grid);
                        }
                        
                    }
                    else if(grid->getStype() == knormal)  //散兵
                    {
                        GameIndex effectIndex = gameIndex(grid->getIndex().i, grid->getIndex().j);
                        attackHero->showEffectLayer(2, false, effectIndex);
                        attackHero->battleMap->removeAttackedSB(sb, 0, myHero);
                       // attackHero->showEffectLayer(22, false, effectIndex);  //散兵消失
                        attackHero->showEffectLayerCgp(Eff_sanbing, true, effectIndex);  //散兵消失

                    }
                    else  //城墙
                    {
                        attackHero->battleMap->removeAttackedSB(sb, 0, myHero);
                    }
                   
                }
                else    //远程
                {
                    if(grid->getStype() == kattack) //攻击阵列
                    {
                        SGAttackList * soldierattack = attackHero->battleMap->getAttackListByGrid(grid);
                        if(soldierattack)
                        {
                            mCgpGrid = grid;
                            sbArray->addObjectsFromArray(soldierattack->attackSbs);
                            listvanishArray->addObjectsFromArray(soldierattack->attackSbs);

                            
                            CCDelayTime*  rmAttacklistDelay = CCDelayTime::create(0);
                            CCCallFunc*  callRmAttList = CCCallFunc::create(this, callfunc_selector(SGAttackList::cgpCallRmAttackList));
                            sb->runAction(CCSequence::create(rmAttacklistDelay, callRmAttList, nullptr));
                        }
                    }
                    else if (grid->getStype() == knormal) //散兵
                    {
                        GameIndex effectIndex = gameIndex(grid->getIndex().i, grid->getIndex().j);
                        CCDelayTime*  rmSbDelay = CCDelayTime::create(0);
                        CCCallFuncN*  callRmSb = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::cgpCallRmSbDelay));
                        sb->runAction(CCSequence::create(rmSbDelay, callRmSb, nullptr));
                        //attackHero->showEffectLayer(22, false, effectIndex);  //散兵消失
                        attackHero->showEffectLayerCgp(Eff_sanbing, true, effectIndex);  //散兵消失
                    }
                    else  //城墙
                    {

                        CCDelayTime*  rmSbDelay = CCDelayTime::create(0);
                        CCCallFuncN*  callRmSb = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::cgpCallRmSbDelay));
                        sb->runAction(CCSequence::create(rmSbDelay, callRmSb, nullptr));
                        
                    }
                }
            }
            else   //自己被打死了
            {
                m_sb_xueTiao->setPercentage(1);
                apLabel->setString("0");
                
                m_isDead = true;
                //int leftattack = this->getAp();
                if (grid->getStype() == knormal) //散兵
                {
                    //attackHero->showEffectLayer(22, false, gameIndex(grid->getIndex().i, grid->getIndex().j));
                    attackHero->showEffectLayerCgp(Eff_sanbing, true, gameIndex(grid->getIndex().i, grid->getIndex().j));  //散兵消失
                    attackHero->battleMap->removeAttackedSB(sb, 0, attackHero);
                }
                else if(grid->getStype() == kattack)
                {

                    SGAttackList *soldierAttack = attackHero->battleMap->getAttackListByGrid(grid);
                    if(soldierAttack)
                    {
                        sbArray->addObjectsFromArray(soldierAttack->attackSbs);
                        soldierAttack->setAp(-leftBlood);
                        soldierAttack->changeAP(soldierAttack->getAp(), false, true);
                        
                        if ((grid->getStype() == kattackfour || grid->getStype() == kattacktwo) && leftBlood)
                        {
                            soldierAttack->playEffectWithType(kAttackEffect);
                        }
                        
                        //获取SG_SkillsBase
                        if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j]) //cgpGrid
                        {
                            this->activateSkills9(NULL, this, this->getAfftributeNum(), soldierAttack);
                            if (soldierAttack->getAp() > 0) {
                                this->activateBuffSkills1(NULL, this, 0,soldierAttack);
                            }
                        }
                        CCObject *obj1 = NULL;
                        CCARRAY_FOREACH(soldierAttack->attackSbs, obj1)
                        {
                            ShiBing* sb1 = (ShiBing*)obj1;
                            int addheight = isMy==true?2:0;
                            SGGridBase* grid1 = (SGGridBase*)sb1->getPrinceGridArray()->objectAtIndex(0);
                            //removed by cgp
                            //attackHero->showEffectLayer(2, false, gameIndex(grid1->getIndex().i-addheight, grid1->getIndex().j));
                        }
                    }
                }
                else    //城墙
                {
                    sb->setDef(-leftBlood);
                    sb->setDefImage();
#if DEBUG
                    sb->showDef(sb->getDef());
                    
#endif
                    //removed by cgp
                    //attackHero->showBattleLabel(leftattack, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                }

            }


            
            //removed by cgp
//            if(leftBlood > 0)
//            {
//                
//
//                //攻击时溅射类型的技能触发，包含武将与士兵，变金币效果
//                if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j]) //cgpGrid
//                {
//                    //grid = attackHero->battleMap->myGrids[sbindex->i][sbindex->j];
//                   //this->activateSkills8(NULL, this, this->getAfftributeNum(), gameIndex(grid->getIndex().i, grid->getIndex().j),gameIndex(grid->getIndex().i, grid->getIndex().j),attackap -leftBlood);
//                }
//            }
//            else
//            {
//                if ( grid != NULL &&  this->getSkillsDelegate())
//                {
//                    //this->activateSkills8(NULL, this, this->getAfftributeNum(), gameIndex(grid->getIndex().i, grid->getIndex().j),gameIndex(grid->getIndex().i, grid->getIndex().j),attackap > resistForce?resistForce:attackap); //removed by cgp
//                }
//
//            }
            
            
            this->changeAP(leftBlood, false, false, false);
            //drawXueTiaoProportion();   //added by cgp for xuetiao
            
            
            //把对方阵列打死时， 播放对方阵列消失动画
            if (this->getAp() > 0)
            {
                //added by cgp
                CCObject *sbAtt1 = NULL;
                CCARRAY_FOREACH(listvanishArray, sbAtt1)
                {
                    ShiBing *attSb = (ShiBing*)sbAtt1;
                    attackHero->showEffectLayerCgp(Eff_xiaoshi, true, attSb->getSbindex());
                }
            }
            
            
            //removed by cgp
           // if (leftBlood > 0)
//            {
//                if (this->getSkillsDelegate() && attackHero->battleMap->myGrids[sbindex->i][sbindex->j])  //cgpGrid
//                {
//                    if (w)
//                    {
//                        if (grid->getStype() == kattack)
//                        {
//                            CCObject *sbAtt = NULL;
//                            CCARRAY_FOREACH(sbArray, sbAtt)
//                            {
//                                ShiBing *attSb = (ShiBing*)sbAtt;
//                                attackHero->showEffectLayer(3, true, attSb->getSbindex());
//                            }
//                        }
//                    }
//                    
//                    bool wuShun = this->activateSkills1(NULL, this, skillsNum, leftBlood);
//                    if (wuShun)
//                    {
//                        if (grid->getStype() == kattack)
//                        {
//                            CCObject *sbAtt = NULL;
//                            CCARRAY_FOREACH(sbArray, sbAtt)
//                            {
//                                ShiBing *attSb = (ShiBing*)sbAtt;
//                                attackHero->showEffectLayer(3, true, attSb->getSbindex());
//                                
//                            }
//                        }
//                        else
//                        {
//                            attackHero->showEffectLayer(3, true, gameIndex(grid->getIndex().i, grid->getIndex().j));
//                        }
//                    }
//                    if (this->getAttackType() != kattacknormal)
//                    this->activateSkills1_1(NULL, this, defNum, grid->getIndex());
//                }
//                if (grid !=NULL && this->getAttackType() == kattacknormal)
//                {
//                    //cgpGrid
//                    if (attackHero->battleMap->myGrids[sbindex->i][sbindex->j] && grid->getStype() != kdefend  )
//                    {
//                        this->activateSkills1_1(NULL, this, resistForce, grid->getIndex());
//                    }
//
//                }
//            }
            
        }

    }
    
    if (this->getAp() > 0)   //当前血量
    {
        if (this->getSkillsDelegate())
        {
            this->activateBuffSkills(NULL, this, gameIndex(sbindex->i, sbindex->j), gameIndex(sbindex->i, sbindex->j), row);
        }
#if PUSH_ATTACK_DATA
//        pushAttackData(CCString::createWithFormat(str_Format_Residual_damage, this->getAp()), false);
        
        data = CCString::createWithFormat(str_Format_Residual_damage, this->getAp());
        pushAttackData(data, false);
#endif
        
    }
    else   //当前血量小于0
    {
#if PUSH_ATTACK_DATA
//        pushAttackData(CCString::createWithFormat(" 剩余攻击力:%d ", 0), true);
        data = CCString::createWithFormat(str_Format_Residual_damage, 0);
        pushAttackData(data, false);
#endif
        
        
        
        this->apLabel->setString("0");  //血的数字
        
        if (jianEffect)
        {
            playAttackEf(this->getDeathEffect());
            jianEffect->removeFromParentAndCleanup(true);
            jianEffect = NULL;
        }
        
        if (this->getAttackType() == kattacktwo)
        {
            //打中敌人的特效，removed by cgp
           // attackHero->showJianBlastEffect(this->getHitEff(), true, gameIndex(sbindex->i + 1, sbindex->j), false);
            if (this->getAtkmode() == 0)
            {
                if (sb_jianEffect)
                {
//                  sb_jianEffect->removeAllChildrenWithCleanup(true);
                    sb_jianEffect->removeFromParentAndCleanup(true);
                    sb_jianEffect = NULL;
                }
            }
        }
        else
        {
            attackHero->showJianBlastEffect(this->getHitEff(), true, gameIndex(sbindex->i, sbindex->j));
        }
        
        
        if (this->getAtkmode() == 0)  //近战
        {
            float deadDelayTime = 0;
            if (!sbindex->is_sanbing)
            {
//                deadDelayTime = m_attackTime - Effect_DelayTime;
                deadDelayTime = m_attackTime - m_effTime;

            }

            m_dispearIndex.i = sbindex->i;
            m_dispearIndex.j = sbindex->j;
            CCDelayTime *deadDelay = CCDelayTime::create(deadDelayTime);
            CCCallFuncND*  callEffect = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::delayAttackOver), (void*)1);
            CCCallFuncND*  callDispear = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::myDispear),(void*)(&m_dispearIndex));
            myHero->runAction(CCSequence::create(deadDelay, callEffect, callDispear, NULL));
           // myHero->runAction(CCSequence::create(deadDelay, callDispear, NULL));

            //myHero->attackOver(this);
        }
        else   //远程
        {
            myHero->attackOver(this, true);  //死翘翘 ,just this
            CCDelayTime *deadDelay = CCDelayTime::create(0.4);
            
            CCCallFuncND*  callDispear = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::myDispearRemote),(void*)(&m_dispearRemote));
            myHero->runAction(CCSequence::create(deadDelay, callDispear, NULL));
            
            
        }
        
        
        
        

    }
    
    CC_SAFE_RELEASE(sbindex);
    
}



void SGAttackList::cgpCallJianEffDelay()
{
    if (jianEffect)
    {
        playAttackEf(this->getDeathEffect());
        //            jianEffect->removeAllChildrenWithCleanup(true);
        jianEffect->removeFromParentAndCleanup(true);
        jianEffect = NULL;
    }
}

//改变血量
void SGAttackList::changBlood()
{
    if (this->getAp() > 0) {
        if (this->getSkillsDelegate()) {
            if (this->activateSkills2(NULL, this, this->getAfftributeNum())) {
                attackHero->changBloodNum(-attackHero->getBlood());
            }
        }
        
        attackHero->changBloodNum(-this->getAp());
        myHero->showLJEff(false, this->getAp());
        
        
        if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            int h = myHero->uibg->getContentSize().height;
            int x = GameConfig::getGridPoint(this->getAttackIndex().i, this->getAttackIndex().j).x;
            
            if (this->getAttackType() == kattacknormal) {
                myHero->showBattleFieldDrop(false, ccp(x, screenheight - h - gridHeight * 0.5), (rand() % 2) + 2);
            }else{
                myHero->showBattleFieldDrop(false, ccp(x, screenheight - h - gridHeight * 0.5), (rand() % 4) + 4);
            }
            
        }
    }
}
//攻击完成
void SGAttackList::attackOver()
{
    actionCount++;
    if (actionCount == attackSbs->count())
    {
        actionCount = 0;
        myHero->attackOver(this);
        attackHero->changBloodNum(-this->getAp());//改变血量
    }
    
}
//攻击阵列融合检测,
bool SGAttackList::chackAttackFuse(SGAttackList *attalkList)
{
    if (attalkList->getAttackType() != kattacknormal) {
        return false;
    }
    
    if (this->getID() == attalkList->getID() &&
        this->getSbColor() == attalkList->getSbColor() &&
        this->thisJ == attalkList->thisJ) {
        
        this->fuseNum ++;
        this->drawEffectSp(0);
        this->changeLRType(lr_r);
        return true;
    }else{
        CCLOG("没有融合。。。");
    }
    return false;
}
//是不是连锁的攻击阵列
bool SGAttackList::chackAttackChain(SGAttackList *attalkList)
{
    if(this->getRoundV() == attalkList->getRoundV() &&
             this->getSbColor() == attalkList->getSbColor()){
        
        if (this->getAddChain() == 0) {
            
            bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
            SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
            for (int i = 0; i < hero->battleMap->m_heroBuff->count(); i++) {
                CCDictionary *buff = (CCDictionary*)hero->battleMap->m_heroBuff->objectAtIndex(i);
                int id = ((CCString*)buff->objectForKey("buffId"))->intValue();
                if (id == 1011) {
                    float value1 = ((CCString*)buff->objectForKey("value1"))->floatValue();
                    int ap = this->getMax_Ap() * value1;
                    
                    
                    
                    
                    this->setSpAddAp(this->getSpAddAp() + ap);
                    this->changeAP(this->getAp() + ap, true);
                    ShiBing *sb = (ShiBing*)this->attackSbs->objectAtIndex(0);
                    this->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                    break;
                }
            }
        }
        
        if (attalkList->getAddChain() < 2) {
            
            int num = attalkList->getAddChain();
            num += 1;
            attalkList->setAddChain(num);
            attalkList->changeLRType(lr_l);
        }
        
        if (this->getAddChain() < 2) {
            int num = this->getAddChain();
            num += 1;
            this->setAddChain(num);
            this->changeLRType(lr_l);
        }

        
        this->drawEffectSp(1);
        return true;
        
    }else{
        CCLOG("没有连锁。。。");
    }
    
    
    return false;
}
//画出武将的攻击特效
void SGAttackList::drawEffectSp(int effectId)
{
    CCString *str = NULL;
    CCSpriterX *effect = NULL;
    
    float scale = 1.0;
    switch (effectId) {
        case 0:
        {
            str = CCString::create("rh");    //融合
            scale = 3.0;
            CCSpriterX *s = CCSpriterX::create("animationFile/rh_icon.scml", true, true);
            s->setanimaID(0);
            s->setisloop(true);
            s->play();
            s->setPosition(ccp(0, gridHeight * 2));
            m_effectsLayer->addChild(s, 1, RONGHE_EFFECT_TAG);
        }
            break;
            
        case 1:
        {
            str = CCString::create("lx");     //链接
            CCSpriterX *s = CCSpriterX::create("animationFile/ls_icon.scml", true, true);
            s->setanimaID(0);
            s->setisloop(false);
            s->play();
            s->setPosition(ccp(0, gridHeight * 1));
            m_effectsLayer->addChild(s, 0, LIANSUO_EFFECT_TAG);
   
        }
            break;
            
        default:
            break;
    }
    if (NULL != str)
    {
        CCString *name1 = CCString::createWithFormat("animationFile/%s.scml",str->getCString());

        effect = CCSpriterX::create(name1->getCString(), true, true);
        effect->setScale(scale);
        effect->setanimaID(0);
        effect->setisloop(false);
        effect->play();
        m_effectsLayer->addChild(effect, ChainSp_Tag, ChainSp_Tag);
    }
}

void SGAttackList::hideEffectSp()
{

}

void SGAttackList::subRoundEffect()
{
    CCSpriterX *subRoundEffect = CCSpriterX::create("animationFile/szg.scml", true, true);
    if (subRoundEffect) {
        switch (this->getAttackType()) {
            case kattacktwo:
            case kattacknormal:
            {
                subRoundEffect->setScale(0.8);
                //subRoundEffect->setScale(5);
            }
                break;
                
            default:
                break;
        }
        
        subRoundEffect->setanimaID(0);
        subRoundEffect->setisloop(false);
        
        m_effectsLayer->addChild(subRoundEffect, SUB_ROUND_EFFECT_TAG, SUB_ROUND_EFFECT_TAG);
        subRoundEffect->play();
    }
}

//移除特效
void SGAttackList::removeEff(CCObject *obj)
{
    m_effectsLayer->removeAllChildren();
}

//计算连锁攻击
void SGAttackList::calculateChainAp(SGHeroLayer *hero)
{
    int rand = 0;
    int ap = this->getAp();
    pushAttackData(CCString::createWithFormat(str_Format_before_chain, this->getAp()), false);
    switch (this->getAddChain()) {
        case 2:
        {
            float b = this->getChainNUm();
            float c = 1.4f;
            float a = c + b;
            rand = ap * a;
            CCLOG("rand===rand==%d",rand);
            pushAttackData(CCString::createWithFormat(str_Format_after_chain2_attack_, rand, ap, b, a), false);
            
            CCString *bb = CCString::createWithFormat(str_Format_after_chain2_attack_, rand, ap, b, a);
            //CCLOG("%s",bb->getCString());
        }
            break;
        case 1:
        {
            float b = this->getChainNUm();
            float c = 1.2f;
            float a = c + b;
            rand = ap * a;
            CCLOG("rand===rand==%d",rand);
            
            pushAttackData(CCString::createWithFormat(str_Format_after_chain1_attack_, rand, ap, b, a), false);
            
            CCString *bb = CCString::createWithFormat(str_Format_after_chain1_attack_, rand, ap, b, a);
           // CCLOG("%s",bb->getCString());
        }
            break;
            
        default:
        {
            
        }
            break;
    }
//    this->showAttackTip("连锁攻击");
    if (rand != 0) {
        pushAttackData(CCString::createWithFormat(str_Format_chain_add_attack, rand - ap), false);
        
        this->showAttackTip(str_Chain_attack);
        this->changeAP(rand);
    }
    
    int stap = this->getAp();
    
    CCLOG("真正的最后的攻击力: %d", stap);

    pushAttackData(CCString::createWithFormat(str_Format_endchain_finaly_attack, rand), false);
}
//计算融合后的攻击数值
void SGAttackList::calculateFuseAp(SGHeroLayer *hero)
{
    if (fuseNum <= 0) {
        return;
    }
    this->showAttackTip(str_Fusion_attack, 0.25f);
    float b = this->getChainNUm();
    float a = fuseNum + b;
    int ap = this->getAp() * a;
    CCString *data = CCString::createWithFormat(str_Format_fusion_add_attack, ap, this->getChainNUm(), a);
    //if (hero->isme) {
        pushAttackData(data, false);
    //}
   // CCLOG("%s",data->getCString());
     
    if (ap != 0){
      
        this->changeAP(this->getAp() + ap);
    
    }
    
}

//激活技能
void SGAttackList::activateSkills(SGBattleMap* battleMap, SGAttackList *attackList, int skID)
{
    if (attackList->getSkillsManager_Delegate()) {
        
        attackList->getSkillsManager_Delegate()->activateSkills(battleMap, attackList, attackList->getAfftributeNum());
    }
}

void SGAttackList::activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        
        attackList->getSkillsManager_Delegate()->activateSkills( isHero, battleMap, attackList, attackList->getAfftributeNum());
    }
}

bool SGAttackList::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage)
{
    if (attackList->getSkillsManager_Delegate()) {
        return attackList->getSkillsManager_Delegate()->activateSkills1(battleMap, attackList, num, damage);
    }
    return false;
}

bool SGAttackList::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        return attackList->getSkillsManager_Delegate()->activateSkills1(battleMap, attackList, num);
    }
    return false;
}

void SGAttackList::activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index)
{
    if (attackList->getSkillsManager_Delegate())
    {
        //modified by cgp
       attackList->getSkillsManager_Delegate()->activateSkills1_1(battleMap, attackList, num, index);
    }
}

bool SGAttackList::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        return attackList->getSkillsManager_Delegate()->activateSkills2(battleMap, attackList, num);
    }
    return false;
}

void SGAttackList::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills3(battleMap, attackList, attackList->getAfftributeNum1());
    }
}

void SGAttackList::activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills4(battleMap, attackList, num);
    }
}

int SGAttackList::activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb)
{
    if (attackList->getSkillsManager_Delegate()) {
        return attackList->getSkillsManager_Delegate()->activateSkills5(battleMap, attackList, num, sb);
    }
    return num;
}

int SGAttackList::activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        return attackList->getSkillsManager_Delegate()->activateSkills5_1(battleMap, attackList, num);
    }
    return num;
}

void SGAttackList::activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills6(battleMap, attackList, num);
    }
}

void SGAttackList::activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills7(battleMap, attackList, num);
    }
}

void SGAttackList::activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage)
{
//    GPCCLOG("SGAttackList::activateSkills8\n");
//    if (attackList->getSkillsManager_Delegate()) {
//        attackList->getSkillsManager_Delegate()->activateSkills8(battleMap, attackList, num, index, index1, damage);
//    }
}

void SGAttackList::activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    //SG_skillsBase
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills9(battleMap, attackList, num, attackList1);
    }
}

void SGAttackList::activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills10(battleMap, attackList, num);
    }
}

bool SGAttackList::activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList, ShiBing *sb)
{
    if (attackList->getSkillsManager_Delegate()) {
        return attackList->getSkillsManager_Delegate()->activateSkills11(battleMap, attackList, sb);
    }
    
    return false;
}

void SGAttackList::activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateSkills12(battleMap, attackList, num, index, index1);
    }
}


bool SGAttackList::assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (NULL != attackList) {
        if (attackList->getSkillsManager_Delegate()) {
            return attackList->getSkillsManager_Delegate()->assistSkills7(battleMap, attackList, num);
        }
    }
    return false;
}


void SGAttackList::detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->detectionReviveSkills(battleMap, attackList, num);
    }
}

void SGAttackList::chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->chackReviveSkills(battleMap, attackList, attackList1);
    }
}


void SGAttackList::activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activatePassivitySkills(battleMap, attackList, num, attackList1);
    }
}

void SGAttackList::activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activatePassivitySkills1(battleMap, attackList, num);
    }
}


void SGAttackList::activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateBuffSkills(battleMap, attackList, index, index1, row);
    }
}

void SGAttackList::activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    if (attackList->getSkillsManager_Delegate()) {
        attackList->getSkillsManager_Delegate()->activateBuffSkills1(battleMap, attackList, num, attackList1);
    }
}

void SGAttackList::changeLRType(sgLRtype lrtype)
{
    if (lrtype == lr_normal)
    {
        this->setLRType(lr_normal);
        return;
    }
    switch (this->getLRType())
    {
        case lr_normal:
        {
            this->setLRType(lrtype);
        }
            break;
        case lr_l:
        {
            if (lrtype == lr_r) {
                this->setLRType(lr_lr);
            }
        }
            break;
         case lr_r:
        {
            if (lrtype == lr_l) {
                this->setLRType(lr_lr);
            }
        }
            break;
        case lr_lr:
        {
            this->setLRType(lrtype);
        }
            break;
        default:
            break;
    }
}
//设置buff对象
void SGAttackList::setBuffListObj(SG_BuffBase* buff)
{
    if(!buff)return;
    bool isAdd = true;
    bool isInit = false;
    
    if (this->m_attackBuffObj->count()) {
        for (int i = 0; i < this->m_attackBuffObj->count(); i++) {
            SG_BuffBase *sgbuff = (SG_BuffBase*)this->m_attackBuffObj->objectAtIndex(i);
            if (buff->m_buffType == sgbuff->m_buffType) {
                isAdd = false;
                
                sgbuff->setRoundNum(sgbuff->getRoundNum() + buff->getRoundNum());
                sgbuff->setAfftributeNum(sgbuff->getAfftributeNum() + buff->getAfftributeNum());
            }
        }
    }else{
        isInit = true;
        this->m_attackBuffObj->addObject(buff);
    }
    
    if (!isInit &&
        isAdd) {
        this->m_attackBuffObj->addObject(buff);
    }
}
//攻击阵列中最后一个兵的index
GameIndex SGAttackList::getAttackIndex()
{
    GameIndex index = gameIndex(mapRow, mapList);
    switch (this->getAttackType()) {
        case kattacknormal:
        {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(attackSbs, obj)
            {
                ShiBing* sb = (ShiBing*)obj;
                if (index.i >= sb->getGridIndex().i) {
                    index = sb->getGridIndex();
                }
            }
        }
            break;
        case kattacktwo:
        case kattackfour:
        {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(attackSbs, obj)
            {
                ShiBing* sb = (ShiBing*)obj;
                index = sb->getSbindex();
            }
            
        }
            break;
            
        default:
            break;
    }
    return index;
}


//技能攻击加成
void SGAttackList::attackAddAp(float num)
{
    this->drawSkillsEffect(2);
    int ap = this->getAp() * num;
    CCLOG("ap====%d",ap);
    this->setSpAddAp(this->getSpAddAp() + ap);
    
    
    
    this->changeAP(this->getAp() + ap);
}
//武将被伤害的数值
void SGAttackList::heroSkillSubAp(int num)
{
    //this->damageNum += num;
}

int SGAttackList::attackSubAp(int ap)
{
    int rand = 0;
    //this->damageNum += ap;
    
    if (this->getSkillsDelegate()) {
        //放被动技能
        ap = this->getSkillsManager_Delegate()->assistPassivitySkills(ap, this, this->getAfftributeNum());
        this->activatePassivitySkills1(NULL, this, ap);
        rand = this->getAp() - ap;
        
    }else{
        rand = this->getAp() - ap;
    }
    
    return rand;
}
//设置融合攻击数值
void SGAttackList::setFuseNum(int num)
{
    fuseNum = num;
}

//画技能特效
void SGAttackList::drawSkillsEffect(int num)
{
    bool isRelease = true;
    float scaleNum = 1.;
    CCString *str = NULL;
    switch (num) {
        case 0://兵器破碎
        {
            str = CCString::create("bqsl");
        }
            break;
        case 1://提升防御力
        {
            str = CCString::create("fylts");
            scaleNum = 0.6;
        }
            break;
        case 2://增加攻击
        {
            str = CCString::create("zjgj");
            scaleNum = 0.6;
        }
            break;
        case 3://旗帜燃烧
        {
            str = CCString::create("qzrs");
        }
            break;
        case 4://绿色符文
        {
        
        }
            break;
        case 5://解除联捷
        {
//            str = CCString::create("lx_000");
//            isRelease = false;
        }
            break;
        case 6://解除融合
        {
//            str = CCString::create("rh_000");
//            isRelease = false;
        }
            break;
        case 7:
        {
            str = CCString::create("youPan");
        }
            break;
        case 8:
        {
            str = CCString::create("qingGuo");
        }
            break;
        case 9:
        {
            str = CCString::create("yanChi");
        }
            break;
        case 10:
        {
            str = CCString::create("shiYao");
        }
            break;
        case 11://反噬
        {
            str = CCString::create("fanShi");
        }
            break;
        case 12:
        {
            str = CCString::create("lssf");
        }
            break;
        case 13:
        {
            str = CCString::create("fw");
        }
            break;
        case 14:
        {
            str = CCString::create("sp");
        }
            break;
            
        default:
            break;
    }
    
    
    if ( NULL != str) {
        m_effectsLayer->stopActionByTag(300);
        
        CCString *name = CCString::createWithFormat("animationFile/%s.plist", str->getCString());
        if (isRelease) {
            ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        }
        CCString *name1 = CCString::createWithFormat("animationFile/%s.scml", str->getCString());
        
        
        CCSpriterX * effectE = CCSpriterX::create(name1->getCString(), true, true);
        effectE->setanimaID(0);
        effectE->setisloop(false);
        effectE->setPosition(CCPointZero);
        
        m_effectsLayer->addChild(effectE, SKILL_EFFECT_TAG, SKILL_EFFECT_TAG);
        effectE->play();
        if (this->getAttackType() != kattackfour) {
            effectE->setScale(scaleNum);
        }
        
        if (isRelease) {
            CC_SAFE_RETAIN(name);
            float a = (effectE->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM) + 1;
            CCDelayTime *time = CCDelayTime::create(a);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::removePlist), (void*)name);
            CCAction *act = (CCAction*)CCSequence::create(time, call, NULL);
            act->setTag(300);
            m_effectsLayer->runAction(act);
        }
    }
}


void SGAttackList::removePlist(CCObject *obj, CCString *str)
{
    CC_SAFE_RELEASE(str);
    //不要删除这里的代码，留档，大坑。
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(str->getCString());
    m_effectsLayer->removeChildByTag(SKILL_EFFECT_TAG, true);
}

//相关标签的动作
void SGAttackList::showBattleLabel(int num, const char *name, int w, int h, GameIndex index)
{
    ShiBing *sb = (ShiBing *)attackSbs->objectAtIndex(0);
    CCString *strN = CCString::createWithFormat("%d", num);
    
    bool meRound = true;
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        //meRound = false;
    }
    
    
    CCLabelAtlas *label = CCLabelAtlas::create(strN->getCString(), name, w, h, '0');
    label->setPosition(CCPointZero);
    CCJumpTo *jump = CCJumpTo::create(0.4, ccp(label->getPosition().x + (gridWidth / 10 * 2), label->getPosition().y), gridHeight * 0.5, 1);
    CCJumpTo *jump1 = CCJumpTo::create(0.2, ccp(label->getPosition().x + (gridWidth / 10 * 3), label->getPosition().y), gridHeight * 0.3, 1);
    CCDelayTime *delay = CCDelayTime::create(1.);
    CCCallFuncN* call = CCCallFuncN::create(this, callfuncN_selector(SGAttackList::removeBalleLabel));
    CCAction *act = (CCAction*)CCSequence::create( jump, jump1, delay, call, NULL);
    
    label->runAction(act);
    sb->addChild(label, ATTACK_ADD_AP_NUM_TAG, ATTACK_ADD_AP_NUM_TAG);
    
}

void SGAttackList::removeBalleLabel(CCObject *obj)
{
    CCLabelAtlas *label = (CCLabelAtlas*)obj;
    label->removeFromParentAndCleanup(true);
}

//改变血量显示
void SGAttackList::changAPLabel()
{
	CCString *apNum = CCString::createWithFormat("%d", this->getAp() - this->getSpAddAp());
	apLabel->setString(apNum->getCString());
}


//画血条长度
void SGAttackList::drawXueTiaoProportion()
{

    float a = this->getAp() - this->getSpAddAp();
    float b = this->getMax_Ap();
    float c = this->damageNum;
    
//    float d = a / (b - c);
    float d = a / b;
    if (d > 1.) {
        d = 1;
    }
    
    if (d <= 0) {
        d = 0.5;//为了防止报错 = 0.5；
    }
    
    
    this->m_sb_xueTiao->setPercentage(d * 100);
    
}
//标签跳动效果
void SGAttackList::labelJump(CCLabelAtlas *label, int num)
{
    CCPoint pos = ccp(label->getPosition().x, label->getPosition().y);
    label->stopAllActions();
    label->setScale(1.0);
    label->setPosition(pos);
    
    CCScaleTo *scale = CCScaleTo::create(0.1, 3);
    CCMoveTo *move = CCMoveTo::create(0.1, ccp(label->getPosition().x, label->getPosition().y + 20));
    CCSpawn *spawn = CCSpawn::createWithTwoActions(scale, move);
    CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGAttackList::setLabelNum), (void*)num);
    CCScaleTo *scale1 = CCScaleTo::create(0.1, 1);
    CCMoveTo *move1 = CCMoveTo::create(0.2, ccp(label->getPosition().x, label->getPosition().y));
    CCSpawn *spawn1 = CCSpawn::createWithTwoActions(scale1, move1);
    label->runAction(CCSequence::create(spawn, call, spawn1, NULL));
}
//设置标签的数值
void SGAttackList::setLabelNum(cocos2d::CCObject *obj, int num)
{
    CCString *str = CCString::createWithFormat("%d", num);
    CCLabelAtlas *label = (CCLabelAtlas *) obj;
    
    label->setString(str->getCString());

}
//播放攻击声效
void SGAttackList::playAttackEf(int num)
{
    return;
    switch (num)
    {
        case -1:
        {
        }
        break;
        default:
        {
            CCString *str = CCString::createWithFormat("music/%d.mp3", num);
            EFFECT_PLAY(str->getCString());
        }
            break;
    }
}

//每轮切换回合的时候,复活已被干掉的攻击队列
//激活复活的技能,复活/*   */
void SGAttackList::changeRevive()
{
    if((this->getIsDeath() &&
     this->getReviveNum() > 0)) {
        this->IsRoundBuff = false;
        
        if (this->getIsDeath()) {
            SGHeroLayer *hero = NULL;
            if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
            {
                hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
            }else{
                hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
            }
            if (hero) {
                hero->showEffectLayer(4, true, gameIndex(4, 4));
            }
            this->setIsDeath(false);
            this->setIsRevive(true);
            this->setReviveNum(0);
            CCLOG("macAp====%d",this->getMax_Ap());
            CCLOG("死亡可以复活时attackList->getResurrectionScale()==%f",this->getResurrectionScale());
			
			//加号之前的this->getAp()新加入,如果不加,在演说技能会先调用,这里复活时会把加上的攻击力重置,(临时解决办法)
			if (this->getAp() >= 0)
			{
				this->changeAP(this->getAp() + this->getMax_Ap() * this->getResurrectionScale());
			}
            
			
            this->setResurrectionScale(0);
            CCObject *sbobj = NULL;

            CCARRAY_FOREACH(this->attackSbs, sbobj)
            {
                ShiBing *sbo = (ShiBing *)sbobj;
                sbo->setVisible(true);
            }
        }
    }
}
//播放音效
void SGAttackList::playEffectWithType(sgEffectType effectType)
{
    return;
    int itemId=this->getID();
    SGOfficerDataModel *officerData=SGStaticDataManager::shareStatic()->getOfficerById(itemId);
    if (NULL == officerData) {
        CCLOG("SGAttackList::playEffectWithType==> officerData=NULL");
        return;
    }
    int num=officerData->getSoundId();
    CCString *name = NULL;
    switch (effectType) {
        case kStoreEffect:
        {
            name = CCString::createWithFormat("music/%d_Store.mp3",num);
        }
            break;
        case kAttackEffect:
        {
            name = CCString::createWithFormat("music/%d_Attack.mp3",num);
        }
            break;
        case kChargeEffect:
        {
            name = CCString::createWithFormat("music/%d_Charge.mp3",num);
        }
            break;
        case kDeadEffect:
        {
            name = CCString::createWithFormat("music/%d_Dead.mp3",num);
        }
            break;
        case kBlockEffect:
        {
            name = CCString::createWithFormat("music/%d_Block.mp3",num);
        }
            break;
            
        default:
            break;
    }
    
    EFFECT_PLAY(name->getCString());
}
//显示技能名称
void SGAttackList::showSkillName()
{
    
    if (this->getAttackType() == kattacknormal) {
        return;
    }
    if (this->getSkillsDelegate()) {
        ShiBing *sb = (ShiBing *)attackSbs->objectAtIndex(0);
        
        SGOfficerDataModel *datao = SGStaticDataManager::shareStatic()->getOfficerById(sb->getSbId());
        int skid = 0;
        if (datao) {
            if (strcmp(datao->getOfficerGeneralsSkill()->getCString(), "0") &&datao->getOfficerGeneralsSkill()->length()>3)
            {
                std::string generalskill(datao->getOfficerGeneralsSkill()->getCString(),3,datao->getOfficerGeneralsSkill()->length() - 3);
                skid = atoi(generalskill.c_str());
            }
        }
        
        
        SGSkillDataModel *data = SGStaticDataManager::shareStatic()->getGeneralSkillById(/*this->getSkillsId()*/skid);
        if (data) {
            CCLabelTTF *label1 = CCLabelTTF::create(data->getSkillName()->getCString(), FONT_BOXINFO, FONTSIZE(20));
            label1->setColor(ccBLACK);
            
            CCLabelTTF *label = CCLabelTTF::create(data->getSkillName()->getCString(), FONT_BOXINFO, FONTSIZE(20));
            label->addChild(label1);
            label->enableStroke(ccc3(52, 0, 0), 4);
            label1->setPosition(ccp(label1->getContentSize().width * 0.5, label->getContentSize().height * 0.52 + 2));
            label1->setColor(ccc3(255, 84, 0));
            label->setColor(ccc3(52, 0, 0));
            label1->setOpacity(0);
            label->setOpacity(0);
            label->setPosition(ccp(0, gridHeight));
            sb->addChild(label);
            
            float time = 0.5f;
            CCMoveTo *move = CCMoveTo::create(time, ccp(label->getPosition().x, label->getPosition().y + gridHeight));
            CCFadeIn *fadei = CCFadeIn::create(time);
            CCScaleTo *scalet = CCScaleTo::create(time, 1.0f);
            CCSpawn *spawn = CCSpawn::create( move, fadei, scalet, NULL);
            CCDelayTime *dtime = CCDelayTime::create(time);
            CCFadeOut *fadeo = CCFadeOut::create(time);
             
            
            CCFadeIn *fadei1 = CCFadeIn::create(time);
            CCScaleTo *scalet1 = CCScaleTo::create(time, 1.0f);
            CCSpawn *spawn1 = CCSpawn::create( fadei1, scalet1, NULL);
            CCDelayTime *dtime1 = CCDelayTime::create(time);
            CCFadeOut *fadeo1 = CCFadeOut::create(time);
            
            label->runAction(CCSequence::create( spawn, dtime, fadeo, NULL));
            label1->runAction(CCSequence::create( spawn1, dtime1, fadeo1, NULL));

        }
    }
    
//    CCRenderTexture
    
}
//显示攻击阵列形成字
void SGAttackList::showAttackName()
{
    if (SGBattleManager::sharedBattle()->isGuide) {
        ShiBing *sb = (ShiBing *)attackSbs->objectAtIndex(0);
        
        CCLabelTTF *label1 = CCLabelTTF::create(str_Formation_attack, FONT_BOXINFO, FONTSIZE(20));
        label1->setColor(ccBLACK);
        
        CCLabelTTF *label = CCLabelTTF::create(str_Formation_attack, FONT_BOXINFO, FONTSIZE(20));
        label->addChild(label1);
        label->enableStroke(ccc3(52, 0, 0), 4);
        label1->setPosition(ccp(label1->getContentSize().width * 0.5, label->getContentSize().height * 0.52 + 2));
        label1->setColor(ccYELLOW);
        label->setColor(ccc3(52, 0, 0));
        label1->setOpacity(0);
        label->setOpacity(0);
        label->setPosition(ccp(0, gridHeight));
        sb->addChild(label);
        
        float time = 0.5f;
        CCMoveTo *move = CCMoveTo::create(time, ccp(label->getPosition().x, label->getPosition().y + gridHeight));
        CCFadeIn *fadei = CCFadeIn::create(time);
        CCScaleTo *scalet = CCScaleTo::create(time, 1.0f);
        CCSpawn *spawn = CCSpawn::create( move, fadei, scalet, NULL);
        CCDelayTime *dtime = CCDelayTime::create(time);
        CCFadeOut *fadeo = CCFadeOut::create(time);
        
        CCFadeIn *fadei1 = CCFadeIn::create(time);
        CCScaleTo *scalet1 = CCScaleTo::create(time, 1.0f);
        CCSpawn *spawn1 = CCSpawn::create( fadei1, scalet1, NULL);
        CCDelayTime *dtime1 = CCDelayTime::create(time);
        CCFadeOut *fadeo1 = CCFadeOut::create(time);
        
        label->runAction(CCSequence::create( spawn, dtime, fadeo, NULL));
        label1->runAction(CCSequence::create( spawn1, dtime1, fadeo1, NULL));
        
    }

    
}
//显示攻击提示
void SGAttackList::showAttackTip(const char* name, float delayT)
{
    CCLabelTTF *label1 = CCLabelTTF::create(name, FONT_BOXINFO, FONTSIZE(20));
    label1->setColor(ccBLACK);
    
    CCLabelTTF *label = CCLabelTTF::create(name, FONT_BOXINFO, FONTSIZE(20));
    label->addChild(label1);
    label->enableStroke(ccc3(58, 13, 13), 4);
    label1->setPosition(ccp(label1->getContentSize().width * 0.5, label->getContentSize().height * 0.52 + 2));
    label1->setColor(ccc3(255, 46, 46));
    label->setColor(ccc3(58, 13, 13));
    label1->setOpacity(0);
    label->setOpacity(0);
    label->setPosition(ccp(0, gridHeight * 2));
    this->m_effectsLayer->addChild(label);
    label->setScale(0.5f);
    label1->setScale(0.5f);
    
    float time = 0.45f;
    int addy = ((delayT == 0) ? (gridHeight * 1.5) : (gridHeight * 1.5 + label->getContentSize().height + 0.5f));
    
    CCDelayTime *det = CCDelayTime::create(delayT);
    CCMoveTo *move = CCMoveTo::create(time, ccp(label->getPosition().x, label->getPosition().y + addy));
    CCFadeIn *fadei = CCFadeIn::create(time);
    CCScaleTo *scalet = CCScaleTo::create(time, 1.0f);
    CCSpawn *spawn = CCSpawn::create( move, fadei, scalet, NULL);
    CCScaleTo *scalet_ = CCScaleTo::create(0.05f, 1.2f);
    CCScaleTo *scalet__ = CCScaleTo::create(0.05f, 0.8f);
    CCScaleTo *scalet___ = CCScaleTo::create(0.05f, 1.0f);
    CCDelayTime *dtime = CCDelayTime::create(time * 1.0f);
    CCFadeOut *fadeo = CCFadeOut::create(time);
    
    CCDelayTime *det1 = CCDelayTime::create(delayT);
    CCFadeIn *fadei1 = CCFadeIn::create(time);
    CCScaleTo *scalet1 = CCScaleTo::create(time, 1.0f);
    CCSpawn *spawn1 = CCSpawn::create( fadei1, scalet1, NULL);
    CCScaleTo *_scalet_ = CCScaleTo::create(0.05f, 1.2f);
    CCScaleTo *_scalet__ = CCScaleTo::create(0.05f, 0.8f);
    CCScaleTo *_scalet___ = CCScaleTo::create(0.05f, 1.0f);
    CCDelayTime *_dtime = CCDelayTime::create(time * 1.0f);
    CCFadeOut *fadeo1 = CCFadeOut::create(time);
    
    label->runAction(CCSequence::create( det, spawn, scalet_, scalet__, scalet___, dtime, fadeo, NULL));
    label1->runAction(CCSequence::create( det1, spawn1, _scalet_, _scalet__, _scalet___, _dtime, fadeo1, NULL));
}
//更新血量数字
void SGAttackList::updatebloodNum(){
    int randNum = 1;
    float randN = CCRANDOM_0_1();
    int value =addAp;
    
    if (value >= 10000) {
        randNum = randN*600 + 600;
    }else if (value >= 7500 && value < 10000) {
        randNum = randN*300 + 175;
    }else if (value >= 5000 && value < 7500) {
        randNum = randN*200 + 70;
    }else if (value >= 2500 && value < 5000) {
        randNum = randN*150 + 55;
    }else if (value >= 300 && value < 2500) {
        randNum = randN*100 + 25;
    }else if (value >= 500 && value < 1000) {
        randNum = randN*40 + 3;
    }else if (value >= 250 && value < 500) {
        randNum = randN*30 + 30;
    }else if (value >= 80 && value < 250) {
        randNum = randN*20 + 20;
    }else if (value >= 15 && value < 80) {
        randNum = randN*5 + 10;
    }
    else if (value >= 5 && value < 80) {
        randNum = randN*3 + 2;
    }
    else
        randNum =  1;
    
    
    
    
    addAp-=randNum;
    
    if (addAp >=0  ) {
        
        if(isAdd)
           currentAp+=randNum;
        else
            currentAp-=randNum;
        if (currentAp<=0) {
            currentAp=0;
            currentAp=0;
        }
        
        CCString *booldStr = CCString::createWithFormat("%d",currentAp);
        apLabel->setString(booldStr->getCString());
//        CCLog("currentAp===============%d",currentAp);
        CCDelayTime * Delay= CCDelayTime::create(0.01);
        CCCallFunc *call=CCCallFunc::create(this, callfunc_selector(SGAttackList::updatebloodNum));
        apLabel->runAction(CCSequence::create(Delay,call,NULL));
//        updatebloodNum();
       
    }else {
        if (isAdd) {
             addAp+=randNum;
            currentAp=  currentAp+addAp;
        }else
        {
             addAp+=randNum;
            currentAp-=addAp;
        }
       
        if (currentAp<=0) {
            currentAp=0;
            currentAp=0;
        }
        
        //CCLog("正确的:%d,得到的：%d",this->getAp(),currentAp);
        CCString *booldStr = CCString::createWithFormat("%d",currentAp);
        apLabel->setString(booldStr->getCString());
        apLabel->runAction(CCScaleTo::create(0.3, 1));
    }
    
    
}

void SGAttackList::scaleTOSec(){
   
    
}

void SGAttackList::setAp(int appVal)
{
    if (appVal < 0) {
        damageNum += sb_ap;
    }
    else{
        if (sb_ap - appVal > 0) {
            //血量变少，受到伤害
            damageNum += sb_ap - appVal;
        }
    }
    
    sb_ap = appVal;
}


