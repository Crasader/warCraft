//
//  ShiBing.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-22.
//
//

#include "ShiBing.h"
#include "SGBattleMap.h"
#include "SGPlayerInfo.h"
#include "SGBattleManager.h"
#include "GameMusicmanage.h"
#include "SGGuideManager.h"
#include "SGAIManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "GlobalConfig.h"
#include "SGStaticDataManager.h"

#define DEF_100     round(getInitDef() * getDefRend())
//#define DEF_100     round(getInitDef())
#define DEF_025     round(DEF_100 * 0.25)
#define DEF_020     round(DEF_100 * 0.20)
#define DEF_05      round(DEF_100 * 0.5) 
#define DEF_075     round(DEF_100 * 0.75)

#define DEF_125     round(DEF_100 + DEF_025)   


#define XULI_EFFECT_TAG             10
#define ADD_DEF_EFFECT_TAG          11
#define XUANFENG_EFFECT_TAG         15
#define SB_BUFF_EFFECT_TAG          16
#define  WALL_TAG    1
#define SB_COLOR_EFFECT_TAG   33

typedef enum
{
    E_RED = 0,
    E_YELLOW,
    E_BLUE
} E_COLOR;


ShiBing::ShiBing()
:m_princeGridArray(NULL),
battleMap(NULL)
,shadow_sp(NULL)
,sbInitData(NULL)
,sequnceNum(ACT_wait)
,sbId(0)
,ismy(false)
,defLabel(NULL)
{
    m_princeGridArray = CCArray::create();
    m_princeGridArray->retain();
    
}

ShiBing::~ShiBing()
{
    if(m_princeGridArray){
        m_princeGridArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_princeGridArray);
    if(sbInitData)
    sbInitData->release();
}

void ShiBing::cleanSbFile(){
    
    SGHeroLayer *myhero = SGBattleManager::sharedBattle()->getHeroLayer(true);
    SGHeroLayer *enemyhero = SGBattleManager::sharedBattle()->getHeroLayer(false);

    int fileid = this->getFileId();
    int colorid = this->getSbColor();
    
    CCLOG("fileid===%d",fileid);
    CCLOG("colorid===%d",colorid);

    bool isRemove = true;
    bool isRemove1 = true;
    bool isRemove2 = true;
    bool isRemove3 = true;
    if (myhero &&
        enemyhero) {
        
        for (int i = 0; i < myhero->battleMap->getSoldiers()->count(); i++) {
            ShiBing *sb = (ShiBing*)myhero->battleMap->getSoldiers()->objectAtIndex(i);
            if (sb != this) {
                if (this->getSbType() == ksbone) {
                    if (fileid == sb->getFileId() &&
                        colorid == sb->getSbColor()) {
                        isRemove = false;
                        break;
                    }
                }else{
                    if (fileid == sb->getFileId())
                    isRemove2 = false;
                    if (fileid == sb->getFileId() &&
                        colorid == sb->getSbColor()) {
                        isRemove = false;
                        break;
                    }
                }
                

            }
        }
        
        for (int i = 0; i < enemyhero->battleMap->getSoldiers()->count(); i++) {
            ShiBing *sb = (ShiBing*)enemyhero->battleMap->getSoldiers()->objectAtIndex(i);
            if (sb != this) {
                if (this->getSbType() == ksbone) {
                    if (fileid == sb->getFileId() &&
                        colorid == sb->getSbColor()) {
                        isRemove1 = false;
                        break;
                    }
                }else{
                    
                    if (fileid == sb->getFileId())
                    isRemove3 = false;
                    if (fileid == sb->getFileId() &&
                        colorid == sb->getSbColor()) {
                        isRemove1 = false;
                        break;
                    }
                }
                
            }
        }
    }
}

ShiBing *ShiBing::createWithData(SGSBObj *data,SGBattleMap *bm, bool isme, bool isSpine)  //cgpSpine
{
    ShiBing *sb = new ShiBing();
    if (sb && sb->initWithData(data, bm, isme, isSpine))
    {
        sb->autorelease();
        return sb;
    }
    CC_SAFE_DELETE(sb);
    return NULL;
}




bool ShiBing::initWithData(SGSBObj *data, SGBattleMap *bm, bool isme, bool isSpine)   //cgpSpine
{
    setIsSpine(isSpine);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer, LIM_PNG_AS_PNG);
    int iFileId = data->getFileId();   //动画文件id
    int colorid = data->getColorId();
    
    //added by cgp for far attack
    SGSbLocalDataModel*  sbLocal = SGStaticDataManager::shareStatic()->getSbLocalByFileId(iFileId);
    this->setFlyId(sbLocal->getFlyId());
    this->setFlyPosX(sbLocal->getFlyPosX());
    this->setFlyPosY(sbLocal->getFlyPosY());
    

    setIsSpine(true);
    char colorSufix[3] = {'r', 'y', 'b'};

    CCString*  jsonName  ;
    CCString*  atlasName ;
    if (iFileId < 100)
    {
        jsonName  = CCString::createWithFormat("spine/soldier_%03d.json",iFileId );
        atlasName = CCString::createWithFormat("spine/soldier_%03d.atlas",iFileId );
        setSbSpine(CCSkeletonAnimation::createWithFile(jsonName->getCString(), atlasName->getCString(), 0.15f));
    }
    else if(iFileId >= 100)
    {
        jsonName  = CCString::createWithFormat("spine/wujiang_%03d_%c.json",iFileId,  colorSufix[colorid]);
        atlasName = CCString::createWithFormat("spine/wujiang_%03d.atlas",iFileId);
        setSbSpine(CCSkeletonAnimation::createWithFile(jsonName->getCString(), atlasName->getCString(), 0.15f));
    }

    
    this->addChild(getSbSpine());
    getSbSpine()->timeScale = SpineSpeed;


    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap())
    {
        setanimaID(ACT_wait,  true);
    }
    else
    {
        if (isme)
        {
            setanimaID(ACT_moveUp,  true);
        }
        else
        {
            setanimaID(ACT_moveDown, true);
        }
    }

    
    if ((sgShiBingType)data->getType() != ksbone)  //武将
    {
        char colorName[3][20] = {"red", "yellow", "blue"};
        CCSkeletonAnimation*  heroFrame = CCSkeletonAnimation::createWithFile("effSpine/huoyan.json", "effSpine/huoyan.atlas", 1);

        if (heroFrame)
        {
            switch ((sgShiBingType)data->getType())
            {
                case ksbfour:
                {
                    heroFrame->setScaleX(2.0);
                    break;
                }
                default:
                {
                    heroFrame->setScaleX(1.0);
                    break;
                }
                    
            }
            heroFrame->setPosition(ccp(0, gridHeight));
            this->addChild(heroFrame, -SB_COLOR_EFFECT_TAG, -SB_COLOR_EFFECT_TAG);
            heroFrame->setAnimation(colorName[colorid], true);
        }
    }

    
    int rand = 1;
    
	//根据占格不同的武将,显示大小不同的的影子
    if (data->getType() == ksbfour)
    {
        shadow_sp = CCSprite::createWithSpriteFrameName("role_shadow_2.png");
        rand = 2;
    }
    else
    {
        shadow_sp = CCSprite::createWithSpriteFrameName("role_shadow_1.png");
        if (data->getType() == ksbtwo)
        {
            rand = 2;
        }
    }
    
    shadow_sp->setPosition(this->getPosition());
    addChild(shadow_sp,-1,-1);
    
    this->ismy = isme;
    battleMap = bm;
    this->setAp(data->getAp());
    this->setAddAp(data->getAddAp());
    this->setDef(data->getDef());
    this->setInitDef(data->getsbBaseDefend());
    this->setResetInitDef(data->getResetInitDef());
    this->setDefRend(data->getDefRend());
    this->setRoundV(data->getRound());
    this->setSbId(data->getSid());
    this->setSbType((sgShiBingType)data->getType());
    this->setSbColor((sgColorType)data->getColorId());
    this->setSbindex(data->getIndex());
    this->setSkillsId(data->getSkillsId());
    this->setAfftributeNum(data->getAfftributeNum());
    this->setAfftributeNum1(data->getAfftributeNum1());
    this->setAfftributeNum2(data->getAfftributeNum2());
    this->setBuffId(data->getBuffId());
    this->setBuffRound(data->getBuffRound());
    this->setBuffAfftribute(data->getBuffAfftribute());
    this->setBuffType(data->getBuffType());
    this->setEffectType(data->getEffectType());
    this->setAddDef(data->getAddDef());
    this->setChainNUm(data->getChainNUm());
    this->setRoundVAddDef(data->getRoundVAddDef());
    this->setName(data->getName());
    this->setAtkmode(data->getAtkmode());
    this->setAtkMode2(data->getAtkMode2());
    this->setAtkModeSkill(data->getAtkModeSkill());
    this->setHeadId(data->getHeadId());
    this->setFileId(data->getFileId());
    this->setPair(data->getPair());
    this->setHitEff(data->getHitEff());
    this->setFriendOff(data->getFriendOff());
    this->setDefType(data->getDefType());
    this->setAttackEffect(data->getAttackEffect());
    this->setDeathEffect(data->getDeathEffect());
    this->setSkill_Head(data->getSkill_Head());
    this->setsbSkLevel(data->getsbSkLevel());
    
    this->setsbDefeatPlus(data->getsbBaseDefendPlus());
    this->setsbBaseAttack(data->getsbBaseAttack());//未加成的攻击
    this->setsbBaseDefend(data->getsbBaseDefend());//未加成的防御
    
    if (this->getSbType() != ksbone) {
        battleMap->princeNum ++;
    }

    sbInitData = CCDictionary::create();
    sbInitData->setObject(data, "sbData");

    sbInitData->retain();

    initGrids(data->getIndex(), (sgShiBingType)data->getType());

    initSoldierPsn();
    
#if SHOW_SB_DEF
    
    char label[255] = {0};
    sprintf(label, "%d",this->getDef());
    defLabel = CCLabelTTF::create(label, "", FONTSIZE(12));
    addChild(defLabel,5);
    defLabel->setColor(ccRED);
    defLabel->setPosition(ccp(this->getContentSize().width * 0.5, this->getContentSize().height * 0.3));
    CCLabelTTF *labelName = CCLabelTTF::create(this->getName().c_str(), "", FONTSIZE(12));
    addChild(labelName, 6);
    labelName->setColor(ccc3(0, 0, 0));
    labelName->setPosition(ccp(this->getContentSize().width * 0.5, this->getContentSize().height * 0.3));
    if (labelName->getContentSize().width > (this->getContentSize().width * 0.8)) {
        
        labelName->setScale((this->getContentSize().width * 0.8) / labelName->getContentSize().width);
        
    }
#endif
    
    //给武将添加名字
    //removed by cgp
    /*********
    if(this->getSbType() == ksbfour || this->getSbType() == ksbtwo)
    {
        std::string officerNameRect;
        int colorId = data->getColorId();
        if(colorId == 0)        //红色
        {
            officerNameRect = "battle_officer_red.png";
        }else if(colorId == 1){ //绿色
            
            officerNameRect = "battle_officer_green.png";
        }else if(colorId == 2)  //蓝色
        {
            officerNameRect = "battle_officer_blue.png";
        }
        CCSprite *spriteBG = CCSprite::createWithSpriteFrameName(officerNameRect.c_str());
        SGCCLabelTTF *labelOfficerName = SGCCLabelTTF::create(data->getName().c_str(), FONT_PENGYOU, 14, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, ccWHITE, ccBLACK, 0.5f);
        spriteBG->addChild(labelOfficerName);
        labelOfficerName->setPosition(ccp(spriteBG->getContentSize().width/2, spriteBG->getContentSize().height/2));
        
        if(this->getSbType() == ksbtwo)
        {
            spriteBG->setPosition(ccp(-spriteBG->getContentSize().width *0/3, spriteBG->getContentSize().height*1.5+25));
        }else{
            spriteBG->setPosition(ccp(-20 - spriteBG->getContentSize().width* 0/3, spriteBG->getContentSize().height*1.5+25));
        }
        
        this->addChild(spriteBG,10000);
    }
     ******/
    
    
    
    if (this->getDefType())
    {
        this->setDef(data->getCurrentdef());
        this->showDef(this->getDef());
        this->setDefImage();
    }
    
    return true;
}

//cgpSpine
void ShiBing::setanimaID(int id , bool isLoop)
{
   // if (getIsSpine())
    {
        getSbSpine()->setAnimation(acActionNames[id], isLoop);
    }
}

void ShiBing::resetSbData()
{
    if (!this->sbInitData) {
        CCLOG("重置数据失败！！");
        return;
    }
    if (this->sbInitData->objectForKey("sbData"))
    {
        SGSBObj *data = (SGSBObj*)this->sbInitData->objectForKey("sbData");
        
        this->setAp(data->getAp());
        this->setAddAp(data->getAddAp());
        this->setDef(data->getDef());
        this->setInitDef(data->getResetInitDef());
        this->setResetInitDef(data->getResetInitDef());
        this->setDefRend(data->getDefRend());
        this->setRoundV(data->getRound());
        this->setSbId(data->getSid());
        this->setSbType((sgShiBingType)data->getType());
        this->setSbColor((sgColorType)data->getColorId());
        this->setAfftributeNum(data->getAfftributeNum());
        this->setAfftributeNum1(data->getAfftributeNum1());
        this->setAfftributeNum2(data->getAfftributeNum2());
        this->setBuffId(data->getBuffId());
        this->setBuffRound(data->getBuffRound());
        this->setBuffAfftribute(data->getBuffAfftribute());
        this->setBuffType(data->getBuffType());
        this->setEffectType(data->getEffectType());
        this->setAddDef(data->getAddDef());
        this->setChainNUm(data->getChainNUm());
        this->setRoundVAddDef(data->getRoundVAddDef());
        this->setName(data->getName());
        this->setAtkmode(data->getAtkmode());
        this->setAtkMode2(data->getAtkMode2());
        this->setAtkModeSkill(data->getAtkModeSkill());
        this->setHeadId(data->getHeadId());
        this->setFileId(data->getFileId());
        this->setPair(data->getPair());
        this->setHitEff(data->getHitEff());
        this->setFriendOff(data->getFriendOff());
        this->setAttackEffect(data->getAttackEffect());
        this->setDeathEffect(data->getDeathEffect());
        this->setSkill_Head(data->getSkill_Head());
        
//        defLabel->setString(CCString::createWithFormat("%d",this->getDef())->getCString());
        resetSbGrid();
        
        this->setanimaID(0, true);
        
    }
    else
    {
        CCLOG("数据为空 重置数据失败！！");
    }
    
    hideXuLiEffect();
    setShadowDraw(true);
    
    showDef(this->getDef());
    
}

void ShiBing::resetSbGrid()
{
    SGGridBase *grid = SGGridBase::createWithType(knormal);
    grid->setIndex(this->getGridIndex());
    grid->setSbType(this->getSbType());
    grid->setSaveID(this->getSbId());
    grid->setSaveColorID(this->getSbColor());
    this->getPrinceGridArray()->removeAllObjects();
    this->getPrinceGridArray()->addObject(grid);
    battleMap->initGrid(grid);
}


void ShiBing::showDef( int def)
{
#if SHOW_SB_DEF
//    char label[255] = {0};
//    sprintf(label, "%d",def);
    defLabel->setString(CCString::createWithFormat("%d",def)->getCString());
#endif
}

void ShiBing::hideDef()
{
#if SHOW_SB_DEF
    defLabel->setVisible(false);
#endif
}

void ShiBing::initGrids(GameIndex index, sgShiBingType sbType)
{
    SGGridBase *grid = SGGridBase::createWithType(this->getDefType()? kdefend:knormal);
    grid->setIndex(index);
    grid->setSbType(sbType);
    grid->setSaveID(this->getSbId());
    grid->setSaveColorID(this->getSbColor());
    this->getPrinceGridArray()->addObject(grid);
    
    battleMap->initGrid(grid);
    
//    if ((grid->getIndex().i == 1 ||
//         grid->getIndex().i == 2) &&
//        (grid->getIndex().j == 0)) {
//        int a = 0;
//        a = 0;
//    }
    
}
void ShiBing::runFade()
{
    CCFadeOut *fadeOut = CCFadeOut::create(0.5);
    CCFadeIn *fadeIn = CCFadeIn::create(0.5);
    this->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(fadeOut,fadeIn,NULL)));
}

void ShiBing::changSBState(sgGridType gridtype)
{
    switch (gridtype) {
        case kdefend:
        {
            bool isme = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
            SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(isme);
            switch (hero->getCountry()) {
                case 1:
                {
                    EFFECT_PLAY(MUSIC_14);
                }
                    break;
                case 2:
                {
                    EFFECT_PLAY(MUSIC_12);
                }
                    break;
                default:
                {
                    EFFECT_PLAY(MUSIC_15);
                }
                    break;
            }
            //this->setDef(DEF_05 + round(DEF_100 * getAddDef()));
            this->setDef(round(DEF_100 * this->getsbDefeatPlus()) );
            
            if (this->battleMap->getIsMe() ||
                SGAIManager::shareManager()->isPVE) {
                CCString *def = CCString::createWithFormat(str_Format_Formation_defense, this->getDef());
                SGBattleManager::sharedBattle()->fightLogRequest(def->m_sString);
            }
            

            this->showDef(this->getDef());
            this->setDefImage();
        }
            break;
            
        default:
            break;
    }
    
    
    
    if (gridtype!=knormal) {
        //删除形成防御阵列的小兵
        this->battleMap->oneList->removeObject(this);

    }
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_princeGridArray, obj)
    {
        CCLOG("setGridStype:%d",gridtype);
        SGGridBase *tgrid = (SGGridBase *)obj;
        tgrid->setStype(gridtype);
    }
}
bool ShiBing::isGridIn(SGGridBase *grid)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_princeGridArray, obj)
    {
        SGGridBase *tgrid = (SGGridBase *)obj;
        
        if (tgrid == grid)
        {
            return true;
        }
    }
    return false;
}
SGGridBase *ShiBing::getOnlyGrid()
{
	if (this->getPrinceGridArray()->data->arr && this->getPrinceGridArray()->count())
		return (SGGridBase*)this->getPrinceGridArray()->objectAtIndex(0);
	else
		return NULL;
}
GameIndex ShiBing::getGridIndex()
{
    SGGridBase *gb = getOnlyGrid();
    if (gb)
    {
        GameIndex index = gb->getIndex();
        return index;
    }
    else
    {
        return gameIndex(0, 0);
    }
}
void ShiBing::initSoldierPsn()
{
    GameIndex index = getGridIndex();
    CCPoint sgsoldierPos = GameConfig::getGridPoint(index.i, index.j, this->ismy);
    bool repair = SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap();
    if (repair) {
        if (this->getSbType() == ksbtwo) {
            sgsoldierPos = GameConfig::getGridPoint(index.i , index.j, this->ismy);
            sgsoldierPos = ccpAdd(sgsoldierPos, ccp(0,this->ismy ? -gridHeight : 0));
        }else{
            
        }
        
        this->setPosition(sgsoldierPos);
        
    }else{
        float needy = this->ismy?-(riverWide + gridHeight * 0.5):(screenheight + riverWide + gridHeight * 0.5);
          this->setPosition(ccp(sgsoldierPos.x, needy));  //cgpSpine
//        if (getIsSpine())
//        {
//            this->SbSpine->setPosition(ccp(sgsoldierPos.x, needy));
//        }
//        else
//        {
//            this->setPosition(ccp(sgsoldierPos.x, needy));
//        }
        
    }
    
}
void ShiBing::moveToIndex(GameIndex index)
{
    this->stopAllActions();
    this->setSbindex(index);
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j,this->ismy);
    float s = fabs(this->getPosition().y - topoint.y);
    float t = s/GET_ALONE_NUM(moveSpeed);
    CCDelayTime* delay = CCDelayTime::create(index.i * 0.2+index.j * 0.005);
    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
    CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
    
    this->runAction(CCSequence::create(delay, moveTo, callND, NULL));
}

void ShiBing::moveToIndexCall(cocos2d::CCObject *obj, int speed, GameIndex index, SEL_CallFuncN selector)
{
    this->stopAllActions();
    this->setSbindex(index);
    int nSpeed = 0;
    CCDelayTime* delay  = NULL;
    if(battleMap->m_isStartFillUpSoldier)
    {
        nSpeed = chubing_moveSpeed;
        delay = CCDelayTime::create((index.i-battleMap->m_nTheFormerPosI )* 0.2+index.j * 0.005);
    }
    else
    {
        nSpeed = moveSpeed;
        delay = CCDelayTime::create(index.i * 0.002+index.j * 0.005);
    }
    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j,this->ismy);
    float s = fabs(this->getPosition().y - topoint.y);
    float t = s/GET_ALONE_NUM(nSpeed);
    t = t < 0.25 ? 0.25 : t;
    int rand = ismy ? -1 : 1;
    float toy = (screenheight  * (ismy ? 0 : 1)+ this->getContentSize().height)  * rand;
    this->setPosition(ccp(topoint.x, toy ));
    
    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
    CCCallFuncN *call = CCCallFuncN::create(obj, selector);
    CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
    CCCallFunc *call1 = CCCallFunc::create(this, callfunc_selector(ShiBing::setSbReceive));
    this->runAction(CCSequence::create( delay,moveTo, callND, call, call1, NULL));
}

void ShiBing::setSbReceive()
{
    SGBattleManager::sharedBattle()->openReceive();
}

void ShiBing::moveToBottom(cocos2d::CCObject *obj, SEL_CallFunc selector)
{
    CCPoint point = CCPointZero;
    point.x = this->getPositionX();
    point.y = this->getPositionY();
    
    int rand = ismy ? -1 : 1;
    float toy = this->getPosition().y + ((screenheight * 0.5 + this->getContentSize().height)  * rand);
    if (selector) {
        float s = fabs(this->getPosition().y - toy);
        float t = s/GET_ALONE_NUM(moveSpeed);
        CCMoveTo *move = CCMoveTo::create(t, ccp(this->getPosition().x, toy));
        CCCallFuncND *callND = CCCallFuncND::create(this, callfuncND_selector(ShiBing::setLoopAction), (void*)ACT_wait);
        CCCallFunc *call = CCCallFunc::create(obj, selector);
        
        this->runAction(CCSequence::create(move, callND, call, NULL));
    }
    else
    {
        this->setPosition(ccp(this->getPosition().x, toy));
    }
    
}

void ShiBing::moveToSceneCall(CCObject *obj, SEL_CallFuncN selector)
{
	//GPCCLOG("Soldier Moving");
    if (battleMap)
    {
		//GPCCLOG("Enter MoveToIndexCall");
        moveToIndexCall(obj, moveSpeed, getGridIndex(), selector);
    }
    else
    {
		GPCCLOG("Enter moveToIndex [%s-%d]", __FUNCTION__, __LINE__);
        moveToIndex(getGridIndex());
    }
    setBingZOrder();
}
void ShiBing::swapGridMove(SGBattleMap *battleMap,GameIndex index, SEL_CallFunc selector)
{

//    CCPoint topoint = GameConfig::getGridPoint(index.i, index.j, ismy);
//    float s = fabs(this->getPosition().y - topoint.y);
//    float t = s/GET_ALONE_NUM(moveSpeed);
//    CCMoveTo *moveTo = CCMoveTo::create(t, topoint);
//    CCDelayTime *time = CCDelayTime::create(0.25);

    this->setSbindex(index);
    this->stopActionByTag(999);
    CCCallFunc *call = CCCallFunc::create(battleMap, selector);
    CCMoveTo *moveTo = CCMoveTo::create(0.25f + sbMoveSpeed, GameConfig::getGridPoint(index.i, index.j, ismy));
    CCAction *act = CCSequence::create(moveTo, call, NULL);
    act->setTag(999);
    this->runAction(act);
}

void ShiBing::attackAnimation(CCObject *obj, SEL_CallFunc selector)
{
    this->stopAllActions();
    CCDelayTime *time = CCDelayTime::create(0.5);
    CCCallFunc *call = CCCallFunc::create(obj, selector);
    this->runAction(CCSequence::create( time, call, NULL));
}

void ShiBing::setBingZOrder()
{
    SGGridBase *grid = (SGGridBase*)m_princeGridArray->objectAtIndex(0);
    int ismysb = this->ismy?1:-1;
    if (ismysb) {
        if (this->getSbType() == ksbtwo ) {
            grid = (SGGridBase*)m_princeGridArray->objectAtIndex(1);
        }else if (this->getSbType() == ksbfour ){
            grid = (SGGridBase*)m_princeGridArray->objectAtIndex(2);
        }
    }
    
    this->setZOrder(ismysb * (grid->getIndex().i + 2));
}

void ShiBing::drawSynthesisDefend(int defendLimit)
{
    if (this->battleMap->getIsMe() || SGAIManager::shareManager()->isPVE) {
        CCString *def = CCString::createWithFormat(str_Format_before_Fusion_defense, this->getDef());
        SGBattleManager::sharedBattle()->fightLogRequest(def->m_sString);
    }
    EFFECT_PLAY(MUSIC_11);
    
    if ( (getDef() + DEF_025) >= defendLimit )
    {
        setDef(defendLimit);
    }
    else
    {
        setDef(getDef() + DEF_025);
    }
    
    //setDef(getDef() + DEF_025);
    setDefImage();
}

void ShiBing::setDefImage()
{
     //cgpSpine
    if(getIsSpine())
    {
        getSbSpine()->setVisible(false);
    }
    else
    {
        setanimaID(ACT_null, true);

    }
    
    setShadowDraw(false);
    
    int contry = 1;
    
    SGHeroLayer *hero = NULL;
    hero = SGBattleManager::sharedBattle()->getHeroLayer(this->battleMap->getIsMe());

    if (hero)
    {
        contry = hero->getCountry();
    }
    
    if (contry <= 0 || contry > 3)
    {
        contry = 1;//出界默认为1
    }
    
    contry = 1;    //暂时设为相同的， cgp
    
    CCString *name = NULL;
    CCSprite *sp = NULL;

   // if (!SGGuideManager::shareManager()->isGuide)
    {

        if (getDef() >= DEF_075)
        {
            name = CCString::createWithFormat("wall_%d_3.png",contry);
        }
        else if (getDef() <= DEF_075 && getDef() > DEF_020)
        {
            name = CCString::createWithFormat("wall_%d_2.png",contry);
        }
        else
        {
            name = CCString::createWithFormat("wall_%d_1.png",contry);
        }
    }

    
    
    if (NULL != name)
    {
        sp = CCSprite::createWithSpriteFrameName(name->getCString());
    }
    if (sp)
    {
        if (this->getChildByTag(WALL_TAG))
        {
            this->removeChildByTag(WALL_TAG, true);
        }
        sp = sp->createWithSpriteFrame(sp->displayFrame());
        this->addChild(sp ,WALL_TAG, WALL_TAG);
        sp->setAnchorPoint(ccp(0.5, 0));
        sp->setPosition(ccp(0, -5));
    }
    showDef(getDef());
    if (this->battleMap->getIsMe() ||  SGAIManager::shareManager()->isPVE)
    {
        CCString *def = CCString::createWithFormat(str_Format_finaly_defense, this->getDef());
        SGBattleManager::sharedBattle()->fightLogRequest(def->m_sString);
    }
    else
    {
        CCString *def = CCString::createWithFormat(str_Format_finaly_defense, this->getDef());
        CCLOG("被攻击者输出====%s", def->getCString());
    }
}

void ShiBing::showXuLiEffect(sgShiBingType type)
{
     
    setShadowDraw(false);
    
    int contry = 1;
    
//    SGHeroLayer *hero = NULL;
//    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
//    {
//        hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
//    }else{
//        hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
//    }
//    if (hero) {
//        contry = hero->getCountry();
//    }
//    if (contry <= 0 ||
//        contry > 3) {
//        contry = 1;//出界默认为1
//    }
    
    CCSkeletonAnimation*  fermEffect = CCSkeletonAnimation::createWithFile("effSpine/xuli.json", "effSpine/xuli.atlas");
    fermEffect->setAnimation("animation", true);
    fermEffect->setAnchorPoint(ccp(0.5, 0.5));
    addChild(fermEffect, XULI_EFFECT_TAG, XULI_EFFECT_TAG);
    
    CCSkeletonAnimation*  fermEffect1 = CCSkeletonAnimation::createWithFile("effSpine/xuli.json", "effSpine/xuli.atlas");
    fermEffect1->setAnimation("animation2", true);
    fermEffect1->setAnchorPoint(ccp(0.5, 0.5));
    addChild(fermEffect1, -XULI_EFFECT_TAG, -XULI_EFFECT_TAG);
    
    fermEffect->setPositionY(fermEffect->getPositionY() + 0.5 * gridHeight);
    fermEffect1->setPositionY(fermEffect1->getPositionY() + 0.5 * gridHeight);

    
    int rand = 1;
    
    switch (type)
    {
        case ksbone:
        {

            rand = 1;
            
        }
            break;
        case ksbtwo:
        {

            rand = 2;
        }
            break;
        case ksbfour:
        {
            fermEffect->setScaleX(2);
            fermEffect1->setScaleX(2);
            rand = 2;
        }
            break;
            
        default:
            break;
    }
    
}

void ShiBing::hideXuLiEffect()
{
//    CCSpriterX *s = NULL;
    if (this->getChildByTag(-XULI_EFFECT_TAG))
    {
        this->getChildByTag(-XULI_EFFECT_TAG)->removeFromParentAndCleanup(true);
//        s = (CCSpriterX*)this->getChildByTag(-XULI_EFFECT_TAG);
//        s->removeFromParentAndCleanup(true);
//        s = NULL;
    }
    if (this->getChildByTag(XULI_EFFECT_TAG))
    {
        this->getChildByTag(XULI_EFFECT_TAG)->removeFromParentAndCleanup(true);
//        s = (CCSpriterX*)this->getChildByTag(XULI_EFFECT_TAG);
//        s->removeFromParentAndCleanup(true);
//        s = NULL;
    }
//    
//    removeChildByTag(-XULI_EFFECT_TAG, true);
//    removeChildByTag(XULI_EFFECT_TAG, true);
    
}

void ShiBing::showEffect(int num, bool isRelease)
{
    float scalNum = 1.;
    CCString *str = NULL;
    switch (num) {
        case 0://绿色旋风
        {
            str = CCString::create("lssf");
            scalNum = 0.8;
        }
            break;
        case 1://提升防御力
        {
            str = CCString::create("fylts");
            scalNum = 0.8;
        }
            break;
        case 2://绿色符文
        {
            str = CCString::create("fw");
        }
            break;
            
        default:
            break;
    }
    
    if ( NULL != str) {
        CCString *name = CCString::createWithFormat("animationFile/%s.plist", str->getCString());
        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        
        CCString *name1 = CCString::createWithFormat("animationFile/%s.scml", str->getCString());
        CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(false);
        fermEffect->setPosition(CCPointZero);
        
        if (this->getSbType() != ksbfour) {
            fermEffect->setScale(scalNum);
        }
        
        addChild(fermEffect, XUANFENG_EFFECT_TAG, XUANFENG_EFFECT_TAG);
        fermEffect->play();
        if (isRelease) {
            CC_SAFE_RETAIN(name);
            float a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM) + 1;
            CCDelayTime *time = CCDelayTime::create(a);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(ShiBing::removePlist), (void*)name);
            this->runAction(CCSequence::create(time, call, NULL));
        }
    }
}

void ShiBing::show_Buff_Effect()
{
    CCNode *node = this->getChildByTag(SB_BUFF_EFFECT_TAG);
    if (node) {
        return;
    }
	
	ResourceManager::sharedInstance()->bindTexture("animationFile/zd.plist", RES_TYPE_LAYER_UI, sg_battleLayer);
	
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/zd.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    fermEffect->setPosition(CCPointZero);
    
    addChild(fermEffect, SB_BUFF_EFFECT_TAG, SB_BUFF_EFFECT_TAG);
    fermEffect->play();
    
}

void ShiBing::removeBuffEffect()
{
    if (this->getChildByTag(SB_BUFF_EFFECT_TAG)) {
        CCSpriterX *sp = (CCSpriterX*)this->getChildByTag(SB_BUFF_EFFECT_TAG);
//        sp->removeAllChildrenWithCleanup(true);
        sp->removeFromParentAndCleanup(true);
        sp = NULL;
    }
    removeChildByTag(SB_BUFF_EFFECT_TAG, true);
}

void ShiBing::removePlist(CCObject *obj, CCString *str)
{
    CC_SAFE_RELEASE(str);
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(str->getCString());
}

void ShiBing::showAddDefEffect(sgShiBingType type)
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/fylts.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/fylts.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(false);
    fermEffect->setPosition(CCPointZero);
    
    addChild(fermEffect, ADD_DEF_EFFECT_TAG, ADD_DEF_EFFECT_TAG);
    fermEffect->play();
}

void ShiBing::hideAddDefEffect()
{
    if (getChildByTag(ADD_DEF_EFFECT_TAG)) {
        CCSpriterX *s = (CCSpriterX*)getChildByTag(ADD_DEF_EFFECT_TAG);
//        s->removeAllChildrenWithCleanup(true);
        s->removeFromParentAndCleanup(true);
        s = NULL;
    }
    removeChildByTag(ADD_DEF_EFFECT_TAG, true);
}

float ShiBing::removeSbEffect()
{
//    ResourceManager::sharedInstance()->bindTexture("effects/xiaoshi.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
//   // CCSpriterX* remove = CCSpriterX::create("animationFile/xcd.scml", true, true);
//    CCSpriterX* remove = CCSpriterX::create("effects/xiaoshi.scml", true, true);

    
//    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("cocosUi/xiaoshi.ExportJson");
//    CCArmature* remove = CCArmature::create("xiaoshi");
    CCSkeletonAnimation*  remove = CCSkeletonAnimation::createWithFile("effSpine/xiaoshi.json", "effSpine/xiaoshi.atlas", 1);
    remove->setAnimation("animation", true, 0);

    remove->setAnchorPoint(ccp(0.5, 0.5));
    addChild(remove, ADD_DEF_EFFECT_TAG, ADD_DEF_EFFECT_TAG);
    remove->setPosition(ccp(0, gridHeight * 0.5));
    return 0.5;

}

void ShiBing::setShadowDraw(bool isdraw)
{
//    shadow_sp->setVisible(isdraw);
}

void ShiBing::removeSBGrid()
{
    for (int i = 0; i < this->getPrinceGridArray()->count(); i++) {
        SGGridBase* grid = (SGGridBase*)this->getPrinceGridArray()->objectAtIndex(i);
        battleMap->myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
    }
}

void ShiBing::setAttAction(cocos2d::CCObject *obj, Action_Sequnce sequnce)
{
    if (obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        sb->setanimaID(sequnce, false);
        //GPCCLOG("setAttAction :%d", sequnce);
        
        //sb->setisloop(false);
        if (sb->getSbType() != ksbtwo)
        {
            EFFECT_PLAY(MUSIC_40);
        }
    }
}

void ShiBing::setLoopAction(CCObject *obj ,Action_Sequnce sequnce)
{
    
    if (obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        sb->setanimaID(sequnce, true);
    }
    else
    {
        setanimaID(sequnce, true);
    }
    
    if (this->getSbType() == ksbone) return;
	
	
	CCLOG("setLoopAction in [%s-%d]", __FUNCTION__, __LINE__);
    if (sequnce == ACT_wait)
    {
        if (this->getChildByTag(-SB_COLOR_EFFECT_TAG)) {
            CCSpriterX *colorFlame = (CCSpriterX*)this->getChildByTag(-SB_COLOR_EFFECT_TAG);
			CCLOG("colorFlame:[%p] in [%s-%d]", colorFlame, __FUNCTION__, __LINE__);
            if (colorFlame)
            {
                colorFlame->setVisible(true);
            }
        }
    }
    else
    {
        if (this->getChildByTag(-SB_COLOR_EFFECT_TAG))
        {
            CCSpriterX *colorFlame = (CCSpriterX*)this->getChildByTag(-SB_COLOR_EFFECT_TAG);
			CCLOG("colorFlame:[%p] in [%s-%d]", colorFlame, __FUNCTION__, __LINE__);
            if (colorFlame)
            {
                colorFlame->setVisible(true);
            }
        }
    }
}

void ShiBing::removeColorFlame()
{
    if (this->getSbType() == ksbone) return;
    if (this->getChildByTag(-SB_COLOR_EFFECT_TAG)) {
        CCSpriterX *colorFlame = (CCSpriterX*)this->getChildByTag(-SB_COLOR_EFFECT_TAG);
        if (colorFlame)
        {
            //            colorFlame->setVisible(true);
    //        colorFlame->removeAllChildrenWithCleanup(true);
            colorFlame->removeFromParentAndCleanup(true);
            colorFlame = NULL;
    //        this->removeChildByTag(-SB_COLOR_EFFECT_TAG, true);
        }
    }
}

void ShiBing::effPlay(cocos2d::CCObject *obj)
{
    CCSpriterX *s = (CCSpriterX*)obj;
    s->play();
}
bool ShiBing::isMaxDef()
{
//    SGHeroLayer *hero = NULL;
//   
//    hero = SGBattleManager::sharedBattle()->getHeroLayer(this->battleMap->getIsMe());
//    
//    int country=0;
//    if (hero) {
//     country = hero->getCountry();
//    }
    
    int a = round(this->getInitDef() * this->getDefRend()) + round(round(this->getInitDef() * this->getDefRend()) * this->getAddDef());
    
//    if (country ==1 &&  getDef() >= DEF_125) {
//        return true;
//    }else if (country!=1 && getDef() >= DEF_100)
//        return true;
    
    return ( getDef() >= a );
}
 