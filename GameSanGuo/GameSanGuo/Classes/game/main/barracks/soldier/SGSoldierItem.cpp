//
//  SGSoldierItem.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#include "SGSoldierItem.h"
#include "SGPlayerInfo.h"
#include "ResourceManager.h"
#include "SGShowString.h"
#include "SGSoldierSkillDataModel.h"
#include "SGStaticDataManager.h"
#include "SGMainManager.h"
#include "AppMacros.h"

#define ICON_ATK "ofc_icon_big_atk.png" //攻击，剑
#define ICON_DEF "ofc_icon_big_def.png" //防御，盾
#define ICON_MOR "ofc_icon_big_mor.png" //血量，心
#define ICON_SPD "ofc_icon_big_spd.png" //速度，马

#define ICON_SCL    0.8f


SGSoldierItem::SGSoldierItem()
:card(NULL),
_cardId(0),
_colorId(0),
soldieritem(NULL),
m_noticeFlag(NULL)
,soldierCanStreng(false)
{
    
}

SGSoldierItem::~SGSoldierItem()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_soldiersLayer);
}

SGSoldierItem *SGSoldierItem::create(int cardId,int colorId)
{
    SGSoldierItem *soldierItem = new SGSoldierItem();
    if (soldierItem && soldierItem->init(cardId,colorId))
    {
        soldierItem->autorelease();
        return soldierItem;
    }
    CC_SAFE_DELETE(soldierItem);
    return NULL;
}

//获取一个渐变层
CCLayerGradient *SGSoldierItem::getGeadientLayer(float hgt)
{
   // CCLayerGradient *color = CCLayerGradient::create(ccc4(0, 0, 0, 255), ccc4(0, 0, 0, 0), ccp(1, 0));
     CCLayerGradient *color = CCLayerGradient::create(ccc4(0, 0, 0, 0), ccc4(0, 0, 0, 0), ccp(1, 0));
    color->setContentSize(CCSizeMake(100, hgt / 2 * ICON_SCL));
    color->setAnchorPoint(ccp(0, 0.5));
    
    return color;
}


bool SGSoldierItem::init(int cardId,int colorId)
{
    
	ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer);
	ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-3303.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer);
	ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist" , RES_TYPE_LAYER_UI, sg_soldiersLayer, LIM_PNG_AS_PNG);

    if (!CCSprite::init())
    {
        return false;
    }
    
    
    
    _cardId = cardId;
    _colorId = colorId;
    char spriteName[256] = {0};
    sprintf(spriteName, "%d-%d.png",_cardId,colorId);
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    
    
    std::string soldierNameStr = str_SGSoldierItem_1;
    int soldierLvl = 1;
    int curExp = 0;
    int maxExp = 100;
    int growSkillLvl = 1;
    std::string growSkillStr = str_SGSoldierItem_2;
    std::string superSkillStr = str_SGSoldierItem_3;
    int atkVal = 0;
    int defVal = 0;
    int morVal = 0;
    int spdVal = 0;
    
    
    CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
    CCObject *obj1 = NULL;
    CCARRAY_FOREACH(allSoldiers, obj1)
    {
        SGSoldierCard *card = (SGSoldierCard *)obj1;
        if (card->getItemId() == _cardId)
        {
            this->card = card;
            soldierNameStr = card->getOfficerName()->getCString();
            soldierLvl = card->getCurrLevel();
            curExp = card->getCurrExp();
            
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(card->getClassName()), card->getItemId());
            SGExpDataModel *num2 = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soldierLvl+1);//比如currLvl为5级 此句话获得5级升6级所需经验值
            maxExp = num2->getExpValue();
            
            if (soldierLvl >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
            {
                curExp = maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soldierLvl)->getExpValue();
            }
            
            
            growSkillLvl = card->getsoldiergrowskill();
            atkVal = card->getAtk();
            defVal = card->getDef();
            morVal = card->getMorale();
            spdVal = card->getSpeed();
            
            soldierCanStreng = (soldierLvl < SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
            
            
            int growid = SGStaticDataManager::shareStatic()->getSoldierGrowSkillid(card->getCurrLevel(),card->getItemId());
            SGSoldierSkillDataModel *growskill = SGStaticDataManager::shareStatic()->getSoldierSkillById(growid);
            
            if (growskill)
            {
                growSkillStr = growskill->getSkillName()->getCString();
            }
            else
            {
                growSkillStr = str_SGSoldierItem_4;
            }
            
            SGSoldierSkillDataModel *talentSkill= SGStaticDataManager::shareStatic()->getSoldierSkillById(card->getsoldiertalentskill());
            if (talentSkill)
            {
                superSkillStr = talentSkill->getSkillName()->getCString();
            }
            else
            {
                superSkillStr = str_SGSoldierItem_4;
            }
            break;

        }
    }
    
    int shift = 0;
    if (CCDirector::sharedDirector()->getWinSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
    {
        shift = 50;
    }
    
    
    //拼底板UI
    CCSprite *soldierBg = CCSprite::createWithSpriteFrameName("soldier_bg.png");
    soldierBg->setAnchorPoint(ccp(0, 0.5));
    soldierBg->setScaleX((winSize.width - 40)/ soldierBg->getContentSize().width);
    soldierBg->setPosition(CCPointZero);
    
    this->addChild(soldierBg, 0);
    
    //用于放置三个button
    CCMenu *menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu,1,MENUTAG);
    
    //SGButton *head = SGButton::create(spriteName, NULL, this, menu_selector(SGSoldierItem::showSoldier));
    //士兵卡头像
//    soldieritem = CCSprite::createWithSpriteFrameName(spriteName);
    soldieritem = SGButton::create(spriteName, NULL, this, menu_selector(SGSoldierItem::showSoldier), CCPointZero,false,true);
    menu->addChild(soldieritem);
    soldieritem->setPosition(ccpAdd(soldierBg->getPosition(), ccp(soldieritem->getContentSize().width / 1.4 + shift,
                                                                  soldieritem->getContentSize().height / 6)));
    
    //士兵的名称
    SGCCLabelTTF *soldierName = SGCCLabelTTF::create(CCString::create(soldierNameStr)->getCString(), FONT_PANGWA, 19);
    this->addChild(soldierName, 2);
    soldierName->setPosition(ccpAdd(soldieritem->getPosition(), ccp(0, -soldieritem->getContentSize().height / 2
                                                                    + soldierName->getContentSize().height / 2)));
    
    
    //白色边框
    CCSprite *outBorder = CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
    this->addChild(outBorder, 1);
    outBorder->setPosition(ccpAdd(soldieritem->getPosition(), ccp(0, 0)));
    
    //士兵卡半透明背影
    CCSprite *bj = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(bj,0);
    bj->setPosition(soldieritem->getPosition());
    
    // 士兵等级
    SGCCLabelTTF *soldierRate = SGCCLabelTTF::create(CCString::createWithFormat("LV %d", soldierLvl)->getCString(), FONT_PANGWA, 22);
    this->addChild(soldierRate, 1);
    soldierRate->setPosition(ccpAdd(outBorder->getPosition(), ccp(0, -soldierRate->getContentSize().height * 2.6)));
    
    //经经验条背影
    
    CCSprite *expbg = CCSprite::createWithSpriteFrameName("mainlayer_exp_bg.png");
    this->addChild(expbg);
    expbg->setPosition(ccpAdd(soldierRate->getPosition(), ccp(-3 + soldierRate->getContentSize().width / 1.2 +
                                                             expbg->getContentSize().width , 0)));
    //经验条
    CCProgressTimer *expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("mainlayer_exp.png"));
    expprog->setType(kCCProgressTimerTypeBar);
    expprog->setMidpoint(ccp(0,0));
    expprog->setBarChangeRate(ccp(1, 0));
    expprog->setPosition(expbg->getPosition());
    expprog->setPercentage((curExp * 1.0) / (maxExp * 1.0) * 100);
    this->addChild(expprog);
    
    expbg->setScaleX(2);
    expprog->setScaleX(2);
    
    //经验条上面的具体经验数字
    SGCCLabelTTF *expNum = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", curExp, maxExp)->getCString(), FONT_PANGWA, 16);
    this->addChild(expNum, 3);
    expNum->setPosition(ccpAdd(expprog->getPosition(), ccp(0, 3)));
    
    
    //属性相关图标
    //攻击图标
    CCSprite *atk = CCSprite::createWithSpriteFrameName(ICON_ATK);
    atk->setScale(ICON_SCL);
    this->addChild(atk, 1);
    atk->setPosition(ccpAdd(soldieritem->getPosition(), ccp(soldieritem->getContentSize().width / 1.1,
                                                            soldieritem->getContentSize().height / 3.5)));
    //渐变黑色层
    CCLayerGradient *colorAtk = this->getGeadientLayer(atk->getContentSize().height);
    this->addChild(colorAtk, 0);
    colorAtk->setPosition(ccpAdd(atk->getPosition(), ccp(0, -colorAtk->getContentSize().height / 2)));
    
    //攻击力数值
    CCLabelTTF *atkNum = CCLabelTTF::create(CCString::createWithFormat("%d", atkVal)->getCString(), FONT_PENGYOU, 24);
    this->addChild(atkNum, 1);
    atkNum->setAnchorPoint(ccp(0, 0));
    atkNum->setPosition(ccpAdd(colorAtk->getPosition(), ccp(atk->getContentSize().width / 3, 0)));
    
    
    //防御图标
    CCSprite *def = CCSprite::createWithSpriteFrameName(ICON_DEF);
    def->setScale(ICON_SCL);
    this->addChild(def, 1);
    def->setPosition(ccpAdd(atk->getPosition(), ccp(atk->getContentSize().width + colorAtk->getContentSize().width / 1.5, 0)));
    
    //黑色渐变
    CCLayerGradient *colorDef = getGeadientLayer(atk->getContentSize().height);
    this->addChild(colorDef, 0);
    colorDef->setPosition(ccpAdd(def->getPosition(), ccp(0, -colorDef->getContentSize().height / 2)));
    
    //防御数值
    CCLabelTTF *defNum = CCLabelTTF::create(CCString::createWithFormat("%d", defVal)->getCString(), FONT_PENGYOU, 24);
    this->addChild(defNum, 1);
    defNum->setAnchorPoint(ccp(0, 0));
    defNum->setPosition(ccpAdd(colorDef->getPosition(), ccp(def->getContentSize().width / 3, 0)));
    
    
    //血量
    CCSprite *mor = CCSprite::createWithSpriteFrameName(ICON_MOR);
    mor->setScale(ICON_SCL);
    this->addChild(mor, 1);
    mor->setPosition(ccpAdd(atk->getPosition(), ccp(0, -atk->getContentSize().height)));
    
    //黑色渐变
    CCLayerGradient *colorMor = getGeadientLayer(atk->getContentSize().height);
    this->addChild(colorMor, 0);
    colorMor->setPosition(ccpAdd(mor->getPosition(), ccp(0, -colorMor->getContentSize().height / 2)));
    
    //血量数值
    CCLabelTTF *morNum = CCLabelTTF::create(CCString::createWithFormat("%d", morVal)->getCString(), FONT_PENGYOU, 24);
    this->addChild(morNum, 1);
    morNum->setAnchorPoint(ccp(0, 0));
    morNum->setPosition(ccpAdd(colorMor->getPosition(), ccp(mor->getContentSize().width / 3, 0)));
    
    
    //速度
    CCSprite *speed = CCSprite::createWithSpriteFrameName(ICON_SPD);
    speed->setScale(ICON_SCL);
    this->addChild(speed, 1);
    speed->setPosition(ccpAdd(def->getPosition(), ccp(0, -atk->getContentSize().height)));
    
    //渐变层
    CCLayerGradient *colorSpd = getGeadientLayer(atk->getContentSize().height);
    this->addChild(colorSpd, 0);
    colorSpd->setPosition(ccpAdd(speed->getPosition(), ccp(0, -colorSpd->getContentSize().height / 2)));
    
    //速度数值
    CCLabelTTF *spdNum = CCLabelTTF::create(CCString::createWithFormat("%d", spdVal)->getCString(), FONT_PENGYOU, 24);
    this->addChild(spdNum, 1);
    spdNum->setAnchorPoint(ccp(0, 0));
    spdNum->setPosition(ccpAdd(colorSpd->getPosition(), ccp(speed->getContentSize().width / 3, 0)));
    

    
    //强化按钮
    SGButton *strengBtn = SGButton::createFromLocal("btn_fenbg.png", str_qianghua, this,
                                                    menu_selector(SGSoldierItem::strengSoldier),CCPointZero,FONT_PANGWA,ccWHITE);
    
    strengBtn->setPosition(ccpAdd(soldierBg->getPosition(), ccp(soldierBg->getContentSize().width - strengBtn->getContentSize().width / 1.95 +shift,
                                                                strengBtn->getContentSize().height / 1.3)));
    m_noticeFlag = CCSprite::createWithSpriteFrameName("publc_notice.png");
    this->addChild(m_noticeFlag,1111);
    m_noticeFlag->setPosition(ccpAdd(strengBtn->getPosition(),ccp(strengBtn->getContentSize().width*.37, strengBtn->getContentSize().height*.38)));
    m_noticeFlag->setVisible(false);
    
    //详情按钮
    SGButton *showBtn = SGButton::createFromLocal("btn_fenbg.png", str_soldier_detail, this,
                                                    menu_selector(SGSoldierItem::showSoldier),CCPointZero,FONT_PANGWA,ccWHITE);
    showBtn->setPosition(ccpAdd(strengBtn->getPosition(), ccp(0, -strengBtn->getContentSize().height * 1.6)));
    
    
    menu->addChild(strengBtn);
    menu->addChild(showBtn);
    
    int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitStrenghSoilder,4);
    strengBtn->setTag(limittag);
    
    //技能名称与等级显示
    SGCCLabelTTF *growSkill = SGCCLabelTTF::create(str_chengzhangji, FONT_PANGWA, 19 , ccc3(254, 195, 8));
    //color value: 254	195	8
    //modify by:zyc. merge into create.
    //growSkill->setColor(ccc3(254, 195, 8));
    this->addChild(growSkill, 1);
    growSkill->setPosition(ccpAdd(colorDef->getPosition(), ccp(colorDef->getContentSize().width + growSkill->getContentSize().width / 2,
                                                               growSkill->getContentSize().height)));
    
    std::string skillNameDesc = "";
    //技能名称
    if (growSkillLvl == 0)
    {
        skillNameDesc = growSkillStr;
    }
    else
    {
        CCString *desc = CCString::createWithFormat("LV %d %s", growSkillLvl, growSkillStr.c_str());
        skillNameDesc = desc->getCString();
    }
    SGCCLabelTTF *growSkillName = SGCCLabelTTF::create(CCString::create(skillNameDesc)->getCString(), FONT_PANGWA, 16);
    this->addChild(growSkillName, 1);
    growSkillName->setPosition(ccpAdd(growSkill->getPosition(), ccp(growSkill->getContentSize().width / 3,
                                                                    -growSkill->getContentSize().height)));
    
    //天赋技能
    SGCCLabelTTF *superSkill = SGCCLabelTTF::create(str_tianfuji, FONT_PANGWA, 19 , ccc3(254, 195, 8));
    //modify by:zyc. merge into create.
    //superSkill->setColor(ccc3(254, 195, 8));
    this->addChild(superSkill, 1);
    superSkill->setPosition(ccpAdd(growSkill->getPosition(), ccp(0, -growSkillName->getContentSize().height * 3.5)));
    
    //技能名称
    SGCCLabelTTF *superSkillName = SGCCLabelTTF::create(CCString::createWithFormat("%s", superSkillStr.c_str())->getCString(), FONT_PANGWA, 16);
    this->addChild(superSkillName, 1);
    superSkillName->setPosition(ccpAdd(superSkill->getPosition(), ccp(superSkill->getContentSize().width / 3,
                                                                      -superSkill->getContentSize().height)));
    
    
    //this->setContentSize(CCSizeMake((winSize.width - 60), soldierBg->getContentSize().height));
    return true;
}

void SGSoldierItem::strengSoldier(CCNode *node)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    CCObject *obj = NULL;
    CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();

    CCARRAY_FOREACH(allSoldiers, obj)
    {
        SGSoldierCard *_card = (SGSoldierCard *)obj;
        if (_card->getItemId() == _cardId)
        {
            card = _card;
        }
    }
    
    if (!soldierCanStreng)
    {
        SGMainManager::shareMain()->showMessage(str_qianghua_tips1);
    }
    else
    {
        SGMainManager::shareMain()->showStrengLayer(card,3,false);
    }
}
void SGSoldierItem::showSoldier(CCNode *node)
{
    SGMainManager::shareMain()->showSoldierInfoLayer(card, 0);
}

int SGSoldierItem::getCardId()
{
    return _cardId;
}
void SGSoldierItem::setNoticeFlag(bool b)
{
    if(m_noticeFlag)
    {
       m_noticeFlag->setVisible(b && soldierCanStreng);
    }
    
}
int SGSoldierItem::getColorId()
{
    return _colorId;
}
void SGSoldierItem::setSoldierItem(CCSprite *temp)
{
    if (temp) {
        soldieritem->setBackFrame(temp->displayFrame());

    }
}