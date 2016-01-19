//
//  SGNormalAttackList.cpp
//  GameSanGuo
// 
//  Created by gengshengcai on 12-12-4.
//
//

#include "SGNormalAttackList.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGNormalAttackList::SGNormalAttackList()
:effect(NULL)
{
    setIsDeath(false);
    setIsRevive(false);
    setReviveNum(0);
}

SGNormalAttackList::~SGNormalAttackList()
{
    
    if (effect)
    {
        effect->removeFromParentAndCleanup(true);
        effect = NULL;
    }
    if (apLabel)
    {
        apLabel->release();
    }
    
}
SGNormalAttackList *SGNormalAttackList::createWithSbs(CCArray *sbs, bool ismy)
{
    SGNormalAttackList *al = new SGNormalAttackList();
    
    if (al && al->initWithSBs(sbs, ismy))
    {
        al->autorelease();
        return al;
    }
    CC_SAFE_DELETE(al);
    return NULL;
}
//兵形成的攻击阵列
bool SGNormalAttackList::initWithSBs(cocos2d::CCArray *sbs, bool ismy)
{

    bool bRet = false;
    apLabel = NULL;

    CCObject *obj = NULL;
    CCARRAY_FOREACH(sbs, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        attackSbs->addObject(sb);
        thisJ = sb->getGridIndex().j;
        attackJ = abs(mapList-1-thisJ);
        sb->setLoopAction(NULL ,ACT_xuli);
        sb->showXuLiEffect(sb->getSbType());
        
        sb->changSBState(kattack);
        
        
        SGBattleMap *bm= (SGBattleMap *)sb->getParent();
        bm->oneList->removeObject(sb);
        mAttIsSpine = sb->getIsSpine();
    }
    this->setAttackType(kattacknormal);
    this->isMy = ismy;
    
    this->addShowMsg();
    
    int iFileId = getFileId();
    m_attackTime =  SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId)->getAllTime()/SpineSpeed * ANIMA_TIME_NUM;
    m_effTime = SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId)->getEffTime()/SpineSpeed * ANIMA_TIME_NUM;
//        GPCCLOG("attackTime = %f",m_attackTime);
//        GPCCLOG("effTime == %f", m_effTime);
    
//        this->drawEffectSp(0);
//        this->drawEffectSp(1);
    
    if (this->getSkillsId() >= SKILLS_ID1)
    {
        setSkillsManager_Delegate(SG_SkillsManager::initskManager(this->getSkillsId()));
        setSkillsDelegate(getSkillsManager_Delegate()->getskBase());
    }
    
    SGHeroLayer* myHeroLayer = NULL;
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getHeroLayer(true);
    }
    else
    {
        myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getHeroLayer(false);
    }
    
    
    CCObject *_obj = NULL;
    CCARRAY_FOREACH(myHeroLayer->getBattleMap()->getAttackList(), _obj)
    {
        SGAttackList *attack = (SGAttackList *)_obj;
        if (attack->getSkillsDelegate())
        {
            attack->getSkillsManager_Delegate()->chackReviveSkills(myHeroLayer->getBattleMap(), this, attack);
        }
    }
    showAttackName();
    this->activateBuff();
    EFFECT_PLAY(MUSIC_5);
    
    bRet = true;
    showSkillName();
    
    return bRet;
}

//地图上各种信息,血条状态,攻击阵列情况,回合数等
void SGNormalAttackList::addShowMsg()
{
    ShiBing *sb = NULL;
    
    if (this->isMy)
    {
        sb = getLastSB();
    }
    else
    {
        sb = getFirstSB();
    }
    if (sb)
    {
        this->setRoundV(sb->getRoundV());
        this->setAp(HALF_FRICE(sb->getAp()));
        this->setDef(sb->getDef());
        this->setSbColor(sb->getSbColor());
        this->setAddAp(sb->getAddAp());
        this->setAddRoundV(sb->getRoundV());
        this->setMax_Ap(sb->getAp());
        this->setSpAddAp(0);
        this->setAddChain(0);
        this->setSkillsId(sb->getSkillsId());
        
        
        this->setID(sb->getSbId());
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
        this->setSkill_Head(sb->getSkill_Head());
        
        //added by cgp
        this->setFlyId(sb->getFlyId());
        this->setFlyPosX(sb->getFlyPosX());
        this->setFlyPosY(sb->getFlyPosY());
        
    }

    this->setResurrectionScale(0.0);
    if (this->isMy)
    {
        CCString *str = CCString::createWithFormat(str_Formation_attack_array, this->getAp());
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    else
    {
        CCString *str = CCString::createWithFormat(str_Formation_attack_array, this->getAp());
        CCLOG("敌方===> %s", str->getCString());
    }
    

    CCSprite *xuetiaoBj = CCSprite::createWithSpriteFrameName("sb_xueTiao_bj.png");

    m_sb_xueTiao = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("sb_xueTiao.png"));
    m_sb_xueTiao->setType(kCCProgressTimerTypeBar);
    m_sb_xueTiao->setMidpoint(ccp(0,0));
    m_sb_xueTiao->setBarChangeRate(ccp(1, 0));
    sb_xuetiao = m_sb_xueTiao->getContentSize().width;

    char roundNumStr[256] = {0};
    sprintf(roundNumStr, "%d",this->getRoundV());
    
    roundNumLabel = CCLabelAtlas::create(roundNumStr, "battle/sb_round_label.png", SB_ROUND_LABEL_W, SB_ROUND_LABEL_H, '0');
    
    //跳动动画
    if (this->getRoundV() < 3 && this->getRoundV()>0) {
        CCBlink *blink = CCBlink::create(3, 10/this->getRoundV());
        CCRepeatForever *repeat = CCRepeatForever::create(blink);
        roundNumLabel->runAction(repeat);
    }
    
    char apStr[256] = {0};
    sprintf(apStr, "%d",this->getAp());
    apLabel = CCLabelAtlas::create(apStr, "battle/sb_xue_label.png", SB_XUE_LABEL_W, SB_XUE_LABEL_H, '0');
    
    if (sb)
    {
        sb->addChild(m_effectsLayer);
        m_effectsLayer->setPosition(ccp(sb->getContentSize().width * .5, sb->getContentSize().height));
        sb->addChild(xuetiaoBj, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG);
        sb->addChild(m_sb_xueTiao, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 2);
        sb ->addChild(roundNumLabel, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 3);
        sb->addChild(apLabel, EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 4);
        apLabel->retain();
    }

    xuetiaoBj->setPosition(ccp(0, apLabel->getContentSize().height - 10));
    m_sb_xueTiao->setPosition(ccp( 0, xuetiaoBj->getPosition().y+0));

    if (apLabel)
    {
        apLabel->setPosition(ccp(xuetiaoBj->getContentSize().width * 0.5, 9));
        apLabel->setAnchorPoint(ccp(1, 0));
    }
    roundNumLabel->setPosition(ccp(-30, apLabel->getContentSize().height + 20 + xuetiaoBj->getContentSize().height * 0.5 + 110));
    roundNumLabel->setAnchorPoint(ccp(0.5, -0.5));
    drawXueTiaoProportion();
    posxx = ccp(0, apLabel->getContentSize().height + 5 + xuetiaoBj->getContentSize().height * 0.5);
    
    roundNum = CCSprite::create();
    sb->addChild(roundNum,EFFECT_LAYER_TAG, EFFECT_LAYER_TAG * 3);
    roundNum->setPosition(ccpAdd(roundNumLabel->getPosition(), ccp(0, 40)));
    changeRound(0, true);
    if (this->getRoundV() >=3 )
    {
        roundNumLabel->setPosition(ccpAdd(roundNumLabel->getPosition(), ccp(0,10)));
    }
    
}
//攻击阵列的第一个兵所处的index
GameIndex SGNormalAttackList::getFirstIndex()
{
    ShiBing *sb = getFirstSB();
    return sb->getSbindex();
}
//攻击阵列的最后一个士兵所处的index
GameIndex SGNormalAttackList::getLastIndex(){
    ShiBing *sb=getLastSB();
    return sb->getSbindex();
}
//获取攻击阵列的第一个士兵
ShiBing *SGNormalAttackList::getFirstSB()
{
    int indexi = 100;
    int indexj = 0;
    ShiBing *wantsb = NULL;
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        if (sb->getSbindex().i<indexi)
        {
            wantsb = sb;
            indexj = sb->getSbindex().j;
            indexi = sb->getSbindex().i;
        }
    }
    return wantsb;
}
//获取攻击阵列的最后一个士兵
ShiBing *SGNormalAttackList::getLastSB()
{
    int indexi = -1;
    int indexj = 0;
    ShiBing *wantsb = NULL;
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        
        if (sb->getSbindex().i > indexi)
        {
            wantsb = sb;
            indexj = sb->getSbindex().j;
            indexi = sb->getSbindex().i;
        }
    }
    return wantsb;
    
}


void SGNormalAttackList::suicideAttack()
{
    
}

void SGNormalAttackList::changeAP(int ap, bool isAction , bool isPlayEff,  bool isDrawXuetiao)
{
    
    currentAp=this->getAp();
    addAp= ap-currentAp;
    
    if (addAp>0)
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
    
    apLabel->runAction(CCScaleTo::create(0.3, 2.0f));
    updatebloodNum();

    
    int nextAP = ap;
    nextAP = nextAP>0?nextAP:0;
    this->setAp(nextAP);
    
    if (!isAction)
    {
        char apStr[256] = {0};
        sprintf(apStr, "%d",this->getAp());
        this->apLabel->setString(apStr);
    }
    else
    {
        this->labelJump(apLabel, this->getAp());
    }
    
    if (isDrawXuetiao)
    {
        drawXueTiaoProportion();
    }
}

//画攻击阵列的特效
void SGNormalAttackList::drawEffectSp(int effectId)
{
    CCString *str = NULL;
    CCSpriterX *effect = NULL;
    float scale = 1.0;
    switch (effectId)
    {
        case 0://融合
        {
            str = CCString::create("rh");
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
            str = CCString::create("lx");
            CCSpriterX *s = CCSpriterX::create("animationFile/ls_icon.scml", true, true);
            s->setanimaID(0);
            s->setisloop(false);
            s->play();
            s->setPosition(ccp(0, gridHeight * 1.5));
            m_effectsLayer->addChild(s, 0, LIANSUO_EFFECT_TAG);
            
        }
            break;
            
        default:
            break;
    }
    if (str)
    {
        CCString *name1 = CCString::createWithFormat("animationFile/%s.scml",str->getCString());
        effect = CCSpriterX::create(name1->getCString(), true, true);
        effect->setanimaID(0);
        effect->setScale(scale);
        effect->setisloop(false);
        effect->setPosition(CCPointZero);
        effect->play();
        m_effectsLayer->addChild(effect, ChainSp_Tag, ChainSp_Tag);
    }
    
}
