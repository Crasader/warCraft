//
//  SGCardItem.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGCardItem.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGCCLabelTTF.h"
#include "SGLayout.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStrengSoldierLayer.h"
#include "SGStringConfig.h"
#include "Utils.h"
#define LIGUANGEFFECTAG 1000

//MMDEBUG: SUSPECTED update方法里居然也用到了tag，还是静态的，如果两个页面都有这个会出问题吗？
static LayerTag enterLayerTag = sg_LayerNull;

SGCardItem::SGCardItem()
: _card(NULL)
, timer(0)
, countryName(NULL)
, m_itemid(0)
, isSel(false)
, notice_bak(NULL)
, m_isLabelWithAction(false)
{
    btn=NULL;
    notice=NULL;
    timer =0;
    emtype=0;
    spriteItem=0;
    _card=0;
    labelIsUse=0;
    spriteIsUseBG=0;
    lock=0;
    kuang=0;
    name=NULL;
    arrayStar=NULL;
    bluestar=NULL;
    label_lvl=NULL;
    advance=NULL;
    countryName=NULL;
    advancenumber=NULL;
    
    tableview=NULL;
    menu=NULL;
    m_itemid=0;
    _entertype=0;
    jnbg=NULL;
    coins=NULL;
    cost=NULL;
    isSelectSpr=NULL;
    isSel=NULL;
    lock_sprite=NULL;
    lessGov=NULL;
    LabelCurrEmbattle=NULL;
    LabelAlternateEmbattle=NULL;
    _islocked=false;
    
} 
SGCardItem::~SGCardItem()
{
    CC_SAFE_RELEASE(_card);
    arrayStar->release();
    bluestar->release();
    
    notice->stopAllActions();
    notice_bak->stopAllActions();
}

void SGCardItem::addCardItemRes(LayerTag cardInLayer)
{
    enterLayerTag = cardInLayer;

    

	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, cardInLayer);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, cardInLayer, LIM_PNG_AS_PNG);
    
    if (cardInLayer != sg_generalsLayer && cardInLayer != sg_generalsBagLayer)
    {
        ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-3303.plist", RES_TYPE_LAYER_UI, cardInLayer);
        ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, cardInLayer);
    }
   
    
}

SGCardItem * SGCardItem::create(bool isLabelWithAction , bool issoldier,int em,SGCardItemDelegate*del,SNSTableView*table,int entertype)
{
    SGCardItem *item = new SGCardItem();
    item->emtype = em;
    item->m_isLabelWithAction = isLabelWithAction;
    if (item && item->init())
    {
        item->delegate=del;
        item->tableview=table;
        item->_entertype = entertype;
        item->initView(issoldier);
        item->autorelease();
        return item;
    }
    CC_SAFE_RELEASE(item);
    return NULL;
}

SGCardItem *SGCardItem::create(bool issoldier,int em,SGCardItemDelegate*del,SNSTableView*table,int entertype)
{
    SGCardItem *item = new SGCardItem();
    item->emtype = em;
    LogSystiemTiem("create");
    if (item && item->init())
    {
        item->delegate=del;
        item->tableview=table;
        item->_entertype = entertype;
        item->initView(issoldier);
        item->autorelease();
        return item;
    }
    CC_SAFE_RELEASE(item);
    return NULL;
}

//设置Item的颜色
void SGCardItem::setItemColor(ccColor3B color)
{
	if (btn)
	{
		btn->setColor(color);
	}
}

void SGCardItem::initView(bool issoldier)
{
	arrayStar = CCArray::create();
    arrayStar->retain();
    bluestar = CCArray::create();
    bluestar->retain();
    this->setTouchEnabled(true);
    
    menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    btn=SGButton::create("item_smallbg.png", NULL, this, menu_selector(SGCardItem::buttonClick),CCPointZero,false,true);
  
    btn->setTag(-1010);
    this->addChild(btn);
    addBtn(btn);

    /////////////
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background,-1,-101);
    
    spriteItem = CCSprite::createWithSpriteFrameName("jinkuang.png");
    this->addChild(spriteItem);
    
    CCPoint itemBgPos =  ccp(47, 0);
    btn->setPosition(itemBgPos);
    spriteItem->setPosition(ccpAdd(itemBgPos, ccp(-btn->getContentSize().width*.5f - 47, 2)));
    background->setPosition(spriteItem->getPosition());
    
    kuang = CCSprite::createWithSpriteFrameName("jinkuang.png");

    this->addChild(kuang,2);
    kuang->setPosition(spriteItem->getPosition());
    
    spriteIsUseBG = CCSprite::createWithSpriteFrameName("carditem_infobg.png");
    spriteIsUseBG->setPosition(ccpAdd(itemBgPos, ccp(btn->getContentSize().width*.3,-15)));
    spriteIsUseBG->setScaleX(.8f);
    spriteIsUseBG->setVisible(false);
    this->addChild(spriteIsUseBG,1);
    
    if (_entertype ==4 ||_entertype ==5)
    {
        jnbg = CCSprite::createWithSpriteFrameName("streng_jn_bg.png");
        jnbg->setPosition(ccpAdd(itemBgPos, ccp(btn->getContentSize().width*.3,-15)));
        jnbg->setScaleX(.8);
        this->addChild(jnbg,1);
    }
    
    notice = SGCCLabelTTF::create("",FONT_BOXINFO, 20);
    this->addChild(notice,2);
    notice->setAnchorPoint(ccp(0, 0.5f));
    notice->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(-55, 0)));
    notice->setCascadeColorEnabled(true);
    notice->setCascadeOpacityEnabled(true);
    
    notice_bak = SGCCLabelTTF::create("",FONT_BOXINFO, 20);
    this->addChild(notice_bak,2);
    notice_bak->setAnchorPoint(ccp(0, 0.5f));
    notice_bak->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(-55, 0)));
    notice_bak->setCascadeColorEnabled(true);
    notice_bak->setCascadeOpacityEnabled(true);
    notice_bak->setOpacity(0);
    
    float t = 0.5;
    if(this->m_isLabelWithAction)
    {
        notice->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1) , CCFadeTo::create(t, 0) , CCDelayTime::create(1.5) , CCFadeTo::create(t, 255) , NULL)));
        notice_bak->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1), CCFadeTo::create(t, 255) , CCDelayTime::create(1.5) , CCFadeTo::create(t, 0) , NULL)));
    }

    if(_entertype!=6)
    {
        //&
        labelIsUse = SGCCLabelTTF::create("", FONT_BOXINFO, 26 , COLOR_RED);
        labelIsUse->setAnchorPoint(ccp(1, 0.5f));
        labelIsUse->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(spriteIsUseBG->getContentSize().width*.5f - 18, 40)));
        this->addChild(labelIsUse,1);
        labelIsUse->setVisible(false);
        //&
        lock = CCSprite::createWithSpriteFrameName("card_suo.png");
        this->addChild(lock,10);
        lock->setVisible(false);
        lock->setAnchorPoint(ccp(1, 1));
        lock->setPosition(ccpAdd(background->getPosition(), ccp(54, 53)));
        //&
        //换人和下场，没有做具体的区分
        lock_sprite = CCSprite::createWithSpriteFrameName("ofc_lock_on.png");
        this->addChild(lock_sprite,11);
        lock_sprite->setVisible(false);
        lock_sprite->setAnchorPoint(ccp(1, 1));
        lock_sprite->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(35, 55)));
        lock_sprite->setScale(0.5);
        //&
        //"统御力不足"
        lessGov = SGCCLabelTTF::create(str_CardItem_str1,FONT_BOXINFO, 23 , ccRED);
        this->addChild(lessGov,2);
        lessGov->setVisible(false);
        lessGov->setAnchorPoint(ccp(1, 0.5f));
        lessGov->setPosition(ccpAdd(notice->getPosition(), ccp(15, 35)));
        //&
        advance = SGCCLabelTTF::create(str_material_not_enough,FONT_BOXINFO, 23 , ccRED);
        this->addChild(advance,2);
        advance->setVisible(false);
        advance->setAnchorPoint(ccp(0, 0.5f));
        advance->setPosition(ccpAdd(notice->getPosition(), ccp(-120, 40)));
    }
   
    namebg = CCSprite::createWithSpriteFrameName("name_bg.png");
    this->addChild(namebg);
    namebg->setPosition(ccpAdd(spriteItem->getPosition(), ccp(spriteItem->getContentSize().width*.5f +namebg->getContentSize().width*.5f, 22)));
	
	//上阵武将页面要显示复选框
	isSelectSpr =  CCSprite::createWithSpriteFrameName("baikuang.png");
    this->addChild(isSelectSpr);
    isSelectSpr->setPosition(ccpAdd(itemBgPos, ccp(btn->getContentSize().width*.359,0)));
	if (_entertype == 111)
	{
        isSelectSpr->setPosition(ccpAdd(itemBgPos, ccp(btn->getContentSize().width*.359,30)));
		coins = CCSprite::createWithSpriteFrameName("coinicon.png");
		coins->setPosition(ccpAdd(isSelectSpr->getPosition(), ccp(-130, 0)));
		this->addChild(coins);
		cost = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
		cost->setAnchorPoint(ccp(0, 0.5));
		cost->setPosition(ccpAdd(coins->getPosition(), ccp(coins->getContentSize().width / 2 + 3, 0)));
		this->addChild(cost);
		isSelectSpr->setVisible(true);
	}
	else
	{
		isSelectSpr->setVisible(false);
	}
		
    if (issoldier)
    {
        spriteIsUseBG->setScaleX(1);
        spriteIsUseBG->setPosition(ccpAdd(itemBgPos, ccp(btn->getContentSize().width*.3 - 15,-15)));
        
        labelIsUse->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(spriteIsUseBG->getContentSize().width*.5f - 30, 36)));
        notice->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(-73, 0)));
    }
}
//获取当前Item对应的卡牌
SGBaseMilitaryCard *SGCardItem::getItemCard()
{
	return _card;
}
//设置每个card的铜钱数
void SGCardItem::setCardCost(int co)
{
	CCLOG("SetCardCost");
	cost->setString(CCString::createWithFormat("%d", co)->getCString());
}


//设置上阵界面武将是否选中
void SGCardItem::setIsSelected(bool isSelect)
{
	isSelectSpr->setVisible(true);
	if(isSelect)
    {
        CCSprite *spriteSelectOK =  CCSprite::createWithSpriteFrameName("streng_choose.png");
        isSelectSpr->setDisplayFrame(spriteSelectOK->displayFrame());
		isSel = true;
    }
    else
    {
        CCSprite *spriteSelectNO =  CCSprite::createWithSpriteFrameName("baikuang.png");
        isSelectSpr->setDisplayFrame(spriteSelectNO->displayFrame());
		isSel = false;
    }
}
bool SGCardItem::getIsSelected()
{
	return isSel;
}

//设置新数据
void SGCardItem::setCard(SGBaseMilitaryCard *newCard)
{
    if(_card)
    {
        _card->release();
    }
    _card = newCard;
    _card->retain();
    m_itemid = _card->getItemId();
}

void SGCardItem::updataCard(SGBaseMilitaryCard *card, CardType type,bool ishui,bool isAdvance,bool enough, int isFromNormal)
{
//    LogSystiemTiem("update");
    
    //huise->setVisible(false);
    spriteIsUseBG->setVisible(false);
    
    if(name)
    {
        name->setString(card->getOfficerName()->getCString());
    }
    else
    {
        name = SGCCLabelTTF::create(card->getOfficerName()->getCString(),FONT_BOXINFO, 26);
        this->addChild(name,10);
        name->setAnchorPoint(ccp(0, 0.5f));
        name->setPosition(ccpAdd(namebg->getPosition(), ccp(-73, 0)));

    }
    
     this->setCard(card);
    
    if (_card->getCurrLevel())
    {
        CCString * strLvl = CCString::createWithFormat("Lv%d",_card->getCurrLevel());
        if(label_lvl){
            label_lvl->setString(strLvl->getCString());
        }
        else{
        label_lvl = SGCCLabelTTF::create(strLvl->getCString(), FONT_PANGWA, 24);//cgp
        this->addChild(label_lvl,10);
        label_lvl->setPosition(ccpAdd(spriteItem->getPosition(), ccp(0,-35)));
        }
    }
    if(LabelCurrEmbattle)
        LabelCurrEmbattle->setVisible(false);
    if(LabelAlternateEmbattle)
        LabelAlternateEmbattle->setVisible(false);
    
    
    notice->setString("");
    
   
    if(_entertype!=6)
    lock->setVisible(false);
    this->stopAllActions();
    
    if (ishui)
    {
      //if (isAdvance)
        if (isAdvance && advance) //by:zyc  任务-前往转生时advance==null
        {
            if (( _card->getCurrLevel() < _card->getUpgradelevel()))
            {
                advance->setVisible(true);
                advance->setString(str_Class_not_enough);
                advance->setInsideColor(ccRED);
            }
            else if (!enough)
            {
                advance->setVisible(true);
                advance->setString(str_material_not_enough);
                advance->setInsideColor(ccRED);
            }
            else
            {
                advance->setVisible(true);
                advance->setString(str_CardItem_str2);
                advance->setInsideColor(ccGREEN);
            }

        }
      else if(strcmp(card->getClassName(),"SGPropsCard"))
      {
          if (!checkCanStrength(card,type) && isFromNormal) {
              //huise->setVisible(false);
              //btn->setColor(ccc3(80, 80, 80));
          }
          else
          {
              //huise->setVisible(false);
              btn->setColor(ccWHITE);
          }
      }
    }
    else
    {
        btn->setColor(ccWHITE);
    }
    
    removeChildByTag(LIGUANGEFFECTAG, true);
    
    if (ishui && isAdvance && checkCanUpdate(card,type))
    {
        CCSpriterX *spritex = CCSpriterX::create("animationFile/liuguangLong.scml",false,true);
        spritex->setanimaID(0);
        spritex->setisloop(true);
        spritex->play();
        spritex->setPosition(CCPointZero);
        this->addChild(spritex,-20, LIGUANGEFFECTAG);
    }

    CCSprite *item = NULL;
	SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),enterLayerTag);//bymm 14.08.11
    switch (type)
    {
        case cardType_officer:
        {
            
            if(_entertype!=6)
            labelIsUse->setVisible(false);
            char spriteName[256] = {0};
            
			sprintf(spriteName, "head%d.png",_card->getHeadSuffixNum());//12.05
			
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(spriteName);
            spriteItem->setDisplayFrame(sprite->displayFrame());
            std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("officersort");
            int type = CCString::create(str)->intValue();
            
            char spriteName1[256] = {0};
            
            bool isSgofficerCard = strcmp(_card->getClassName(),"SGOfficerCard")?false:true;
            if (isSgofficerCard)
            {
                SGOfficerCard *officerCard = (SGOfficerCard *)_card;
                if (label_lvl)
                {
                    label_lvl->setVisible(true);
                }
                
                //非阵容入口
                if (emtype == 0)
                {
                    int po = SGTeamgroup::shareTeamgroupInfo()->isOnEmbattle(officerCard);
                    int curr = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();

                    if (po == 0)
                    {
                        if (_entertype==4 || _entertype==5 || _entertype==6)
                        {
                            if(_entertype!=6)
                            {
                                labelIsUse->setString(str_CardItem_str3);
                                labelIsUse->setVisible(false);
                            }
                            if(LabelCurrEmbattle)
                                LabelCurrEmbattle->setVisible(false);
                            if(LabelAlternateEmbattle)
                                LabelAlternateEmbattle->setVisible(false);
                        }
                        else
                        {
                            lessGov->setString(str_CardItem_str3);
                            lessGov->setVisible(false);
                            if(LabelCurrEmbattle)
                                LabelCurrEmbattle->setVisible(false);
                            if(LabelAlternateEmbattle)
                                LabelAlternateEmbattle->setVisible(false);
                        }
                    }
                    else
                    {
                        if (_entertype==4 || _entertype==5 || _entertype==6)
                        {
                            if(po ==curr)
                            {
                                if(!LabelCurrEmbattle)
                                {
                                    LabelCurrEmbattle = SGCCLabelTTF::create(str_CardItem_str101,FONT_PANGWA, 24 , ccc3(100, 255, 70));
                                    this->addChild(LabelCurrEmbattle,2);
                                    LabelCurrEmbattle->setVisible(true);
                                    LabelCurrEmbattle->setAnchorPoint(ccp(1, 0.5f));
                                    LabelCurrEmbattle->setPosition(ccpAdd(notice->getPosition(), ccp(85, 35)));
                                }
                                if(LabelCurrEmbattle)
                                    LabelCurrEmbattle->setVisible(true);
                                if(LabelAlternateEmbattle)
                                    LabelAlternateEmbattle->setVisible(false);
                            }
                            else
                            {
                                if(!LabelAlternateEmbattle)
                                {
                                    LabelAlternateEmbattle = SGCCLabelTTF::create(str_CardItem_str102,FONT_PANGWA, 24 , ccc3(255, 67, 67));
                                    this->addChild(LabelAlternateEmbattle,2);
                                
                                    LabelAlternateEmbattle->setVisible(true);
                                    LabelAlternateEmbattle->setAnchorPoint(ccp(1, 0.5f));
                                    LabelAlternateEmbattle->setPosition(ccpAdd(notice->getPosition(), ccp(85, 35)));
                                }
                                if(LabelCurrEmbattle)
                                    LabelCurrEmbattle->setVisible(false);
                                if(LabelAlternateEmbattle)
                                    LabelAlternateEmbattle->setVisible(true);
                                
                            }

                        }
                        else
                        {
                            
                            if(po ==curr)
                            {
                                if(!LabelCurrEmbattle)
                                {
                                    LabelCurrEmbattle = SGCCLabelTTF::create(str_CardItem_str101,FONT_PANGWA, 24 , ccc3(100, 255, 70));
                                    this->addChild(LabelCurrEmbattle,2);
                                   
                                    LabelCurrEmbattle->setVisible(true);
                                    LabelCurrEmbattle->setAnchorPoint(ccp(1, 0.5f));
                                    LabelCurrEmbattle->setPosition(ccpAdd(notice->getPosition(), ccp(85, 35)));
                                }
                                if(LabelCurrEmbattle)
                                    LabelCurrEmbattle->setVisible(true);
                                if(LabelAlternateEmbattle)
                                    LabelAlternateEmbattle->setVisible(false);
                            }
                            else
                            {
                                if(!LabelAlternateEmbattle)
                                {
                                    LabelAlternateEmbattle = SGCCLabelTTF::create(str_CardItem_str102,FONT_PANGWA, 24 , ccc3(255, 67, 67));
                                    this->addChild(LabelAlternateEmbattle,2);
                                    
                                    LabelAlternateEmbattle->setVisible(true);
                                    LabelAlternateEmbattle->setAnchorPoint(ccp(1, 0.5f));
                                    LabelAlternateEmbattle->setPosition(ccpAdd(notice->getPosition(), ccp(85, 35)));
                                }
                                if(LabelCurrEmbattle)
                                    LabelCurrEmbattle->setVisible(false);
                                if(LabelAlternateEmbattle)
                                    LabelAlternateEmbattle->setVisible(true);
                            }
                        }
                    }
    
                }
                else
                {
                    int po = SGTeamgroup::shareTeamgroupInfo()->isOnEmbattle(officerCard)? 10:(-10);
                    if (po > 0)
                    {

                        if (_entertype==4 || _entertype==5 || _entertype==6)
                        {
                            if(_entertype!= 6)
                            {
                            labelIsUse->setString(str_CardItem_str3);
                            labelIsUse->setVisible(true);
                            }
                        }
                        else
                        {
                            if(_entertype!=6)
                            {
                                lessGov->setString(str_CardItem_str3);
                                lessGov->setVisible(true);
                            }
                            
                        }
                    }
                }
               
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                sprintf(spriteName1, "officerPinzhi_%d_0.png",starlvl);

                
                int m_atk = officerCard->getAtk();
                int m_def = officerCard->getDef();
                float m_speed= officerCard->getSpeed();
                int m_mor = officerCard->getMorale();
                int m_gov = officerCard->getGoverning();
               
                CCObject *obj = NULL;
                CCARRAY_FOREACH(officerCard->getOfficerEquips(), obj)
                {
                    SGEquipCard *card = (SGEquipCard *)obj;
//                    m_atk += card->getAtk();
//                    m_def += card->getDef();
//                    m_speed += card->getSpeed();
//                    m_mor += card->getMorale();
                    m_gov += card->getGoverning();
                }
                if (m_gov<0)
                    m_gov = 0;
                
                //计算武将战力
                SGStaticDataManager * dataManage = SGStaticDataManager::shareStatic();
                int combat=0;
                int grid = dataManage->getOfficerById(officerCard->getItemId())->getArea();
                int officerCombat = dataManage->getBaseCombatValue(6, officerCard->getCurrLevel(), officerCard->getCurrStar(), officerCard->getAdNum());
                int skillCombat = dataManage->getSkillCombatValue(grid, officerCard->getLordId(), officerCard->getOfficerSkil());
                combat = officerCombat + skillCombat;
                //计算装备战力加成
                if(officerCard->getOfficerEquips())
                {
                    for(int i=0; i<officerCard->getOfficerEquips()->count() ; ++i)
                    {
                        SGEquipCard * temp = (SGEquipCard*)officerCard->getOfficerEquips()->objectAtIndex(i);
                        if(temp)
                            combat += dataManage->getBaseCombatValue(7, temp->getCurrLevel(), temp->getCurrStar(), temp->getAdNum());
                    }
                    
                }
                
                if (_entertype==5)
                {
                    SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(officerCard->getOfficerSkil());
                    if(officerSkill)
                        notice->setString(CCString::createWithFormat("%s  Lv%d",(officerSkill->getSkillName())->getCString(),officerSkill->getSkillMaxLevel())->getCString());
                    else
                        notice->setString(CCString::createWithFormat(str_GeneralInfoEffect_str2)->getCString());
                }
                else if(_entertype == 4)
                {
                    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(officerCard->getLordId());
                    if(lordSkill)
                    notice->setString(CCString::createWithFormat("%s  Lv%d",(lordSkill->getSkillName())->getCString(),lordSkill->getLevel())->getCString());
                    else
                    notice->setString(CCString::createWithFormat(str_EmbattleLayer_str15)->getCString());
                }
                //强化武将卡 武将背包  type值根据排序按钮的顺序 从0开始索引
                else if(_entertype == 6)
                {
                    if(type<=4 || type==6 || type==10)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str5,m_atk)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    if(type == 5)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str4,m_mor)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 7)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str6,(int)m_speed)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 8)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str7,m_def)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 9)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
//                    else
//                    {
//                        notice->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
//                        spriteIsUseBG->setVisible(true);
//                    }

                }
                //阵容处、换人
                else
                {
                    if(type == 5)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str4,m_mor)->getCString());
                        notice_bak->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 6)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str5,m_atk)->getCString());
                        notice_bak->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 7)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str6,(int)m_speed)->getCString());
                        notice_bak->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 8)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str7,m_def)->getCString());
                        notice_bak->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else if(type == 9)
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        notice_bak->setString(CCString::createWithFormat(str_CardItem_str12,combat)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                    else
                    {
                        notice->setString(CCString::createWithFormat(str_CardItem_str8,m_gov)->getCString());
                        notice_bak->setString(CCString::createWithFormat(str_CardItem_str12,combat)->getCString());
                        spriteIsUseBG->setVisible(true);
                    }
                }
            }
            else
            {
                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
                int starlvl = temp->getPropsCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                sprintf(spriteName1, "officerPinzhi_%d_0.png",starlvl);
                if (advancenumber) {
                kuang->removeChildByTag(250);
                }
                if (label_lvl)
                {
                    label_lvl->setVisible(false);
                }
            }
           
            item = CCSprite::createWithSpriteFrameName(spriteName1);
            kuang->removeChildByTag(30);
            countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",_card->getRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(item->getContentSize().width*0.35,item->getContentSize().height - countryName->getContentSize().height));
            kuang->addChild(countryName,30,30);
            
            if (isSgofficerCard)//武将身上的+1,+2,+3
            {
                if (advancenumber)
                    kuang->removeChildByTag(250);
                if (_card->getAdNum()>0)
                {
                    advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d", _card->getAdNum())->getCString(), FONT_PANGWA, 24, ccGREEN); //cgp
                    advancenumber->setAnchorPoint(ccp(1,1));
                    advancenumber->setPosition(ccpAdd(item->getContentSize(),ccp(-advancenumber->getContentSize().width + 15, -advancenumber->getContentSize().height + 15)  ));
                    kuang->addChild(advancenumber,50,250);
                }
            }
        }
            break;
        case cardType_equip:
        {
            std::string string = CCUserDefault::sharedUserDefault()->getStringForKey("equipsort");
            int type = CCString::create(string)->intValue();
            
            CCString *str = NULL;// CCString::create("");
            labelIsUse->setVisible(false);
            CCString *str_ = CCString::create(_card->getClassName());
            if (str_->isEqual(CCString::create("SGEquipCard")))
            {
                SGEquipCard *card = (SGEquipCard *)_card;
                if (label_lvl)
                    label_lvl->setVisible(true);
                if(type == 5)
                {
                    notice->setString(CCString::createWithFormat(str_CardItem_str4,card->getMorale())->getCString());
                    spriteIsUseBG->setVisible(true);
                }
                else if(type == 6)
                {
                    notice->setString(CCString::createWithFormat(str_CardItem_str5,card->getAtk())->getCString());
                    spriteIsUseBG->setVisible(true);
                }
                else if(type == 7)
                {
                    notice->setString(CCString::createWithFormat(str_CardItem_str6,(int)card->getSpeed())->getCString());
                    spriteIsUseBG->setVisible(true);
                }
                else if(type == 8)
                {
                    notice->setString(CCString::createWithFormat(str_CardItem_str7,card->getDef())->getCString());
                    spriteIsUseBG->setVisible(true);
                }
                else if(type == 9)
                {
                    notice->setString(CCString::createWithFormat(str_CardItem_str8,card->getGoverning())->getCString());
                    spriteIsUseBG->setVisible(true);
                }
                
                if (card->getOfficerCardId() != 0)
                {
                    SGOfficerCard *ofcard =  SGPlayerInfo::sharePlayerInfo()->getOfficerCard(card->getOfficerCardId());
                    
                    labelIsUse->setString(CCString::createWithFormat(str_CardItem_str9,ofcard->getOfficerName()->getCString()/*,ofcard->getCurrLevel()*/)->getCString());
                    labelIsUse->setVisible(true);
                }
                
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
                str = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
            }
            else
            {
                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
                int starlvl = temp->getPropsCurrStarLevel();
                if (starlvl>1)
                    starlvl-=1;
                str = CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl);
                if (label_lvl)
                    label_lvl->setVisible(false);
                if (advancenumber)
                    kuang->removeChildByTag(250);
            }
                
            item = CCSprite::createWithSpriteFrameName(str->getCString());
            
            if (str_->isEqual(CCString::create("SGEquipCard")))//武将身上的+1,+2,+3
            {
                if (advancenumber) {
                    kuang->removeChildByTag(250);
                }
                if (_card->getAdNum()>0) {
                    advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d", _card->getAdNum())->getCString(), FONT_PANGWA, 24, ccGREEN);//cgp
                    advancenumber->setAnchorPoint(ccp(1,1));
                    advancenumber->ignoreAnchorPointForPosition(false);
                    advancenumber->setPosition(ccp(item->getContentSize().width*.97,item->getContentSize().height));
                    kuang->addChild(advancenumber,50,250);
                }
                
                
            }

            
            char spriteName[256] = {0};
			sprintf(spriteName, "head%d.png",_card->getHeadSuffixNum());//12.06
			
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(spriteName);
            spriteItem->setDisplayFrame(sprite->displayFrame());
        }
            break;
        case cardType_prop:
        {
            char spriteName[256] = {0};
			sprintf(spriteName, "head%d.png",_card->getHeadSuffixNum());//12.06

            CCSprite *sprite = CCSprite::createWithSpriteFrameName(spriteName);
            spriteItem->setDisplayFrame(sprite->displayFrame());
            item = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        }
            break;
        case cardType_soldier:
        {
            char spriteName[256] = {0};
            sprintf(spriteName, "head%d.png",_card->getItemId());
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(spriteName);
            spriteItem->setDisplayFrame(sprite->displayFrame());
			SGPropsDataModel *temp =  SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
			int starLvl = temp->getPropsCurrStarLevel();
			if (starLvl > 1)
				starLvl -= 1;
			
            item = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString());
            spriteIsUseBG->setVisible(true);
            notice->setString(CCString::createWithFormat(str_CardItem_str10,((SGPropsCard*)_card)->getValue())->getCString());
        }
            break;
        case cardType_skill:
        {
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
            int starlvl = temp->getPropsCurrStarLevel();
            if (starlvl>1)
                starlvl-=1;
            
            char spriteName[256] = {0};
            sprintf(spriteName, "head%d.png",_card->getHeadSuffixNum());
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(spriteName);
            spriteItem->setDisplayFrame(sprite->displayFrame());
            item = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
            
            if (kuang)
            {

                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
    
                if (kuang->getChildByTag(30)== NULL) {
                    countryName->setPosition(ccp(kuang->getContentSize().width/2-4,kuang->getContentSize().height+8));
                }
                else
                {
                    kuang->removeChildByTag(30, true);
                    countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height+2));
                }
                kuang->addChild(countryName,30,30);
            }
  
        }
            break;
        case cardType_streng:
        {
            char spriteName[256] = {0};
            switch (_card->getItemId()) {
                case 3301:
                case 3203:
                case 3103:
                    sprintf(spriteName, "%d-2.png",_card->getItemId());
                    break;
                case 3303:
                case 3201:
                case 3102:
                    sprintf(spriteName, "%d-3.png",_card->getItemId());
                    break;
                case 3302:
                case 3202:
                case 3101:
                    sprintf(spriteName, "%d-1.png",_card->getItemId());
                    break;
                default:
                    break;
            }
            
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(spriteName);
            spriteItem->setDisplayFrame(sprite->displayFrame());
            item = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",SGPlayerInfo::sharePlayerInfo()->getPlayerCountry())->getCString());
        }
            break;
        default:
            break;
    }
    kuang->setDisplayFrame(item->displayFrame());
    
    showPlayerStar();
   
    
}


void SGCardItem::showPlayerStar()
{
    for(int i=0;i<arrayStar->count();i++)
    {
        CCSprite *sprite = (CCSprite*)arrayStar->objectAtIndex(i);
        sprite->removeFromParentAndCleanup(true);
    }
    for(int i=0;i<bluestar->count();i++)
    {
        CCSprite *sprite = (CCSprite*)bluestar->objectAtIndex(i);
        sprite->removeFromParentAndCleanup(true);
    }
    
    //需求更改，有星星的地方 要=3颗星星，不需要黑色的。
    //int starCount = _card->getCurrStar();
    int starCount = 3;
    for(int i=0; i < 6;i++)
    {
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("xx.png");
        sprite->setAnchorPoint(ccp(0.5, 1));
        sprite->setPosition(ccpAdd(spriteItem->getPosition(), ccp(spriteItem->getContentSize().width+sprite->getContentSize().width*i - 17,4)));
        arrayStar->addObject(sprite);
        this->addChild(sprite);
        if (i >= starCount) {
            sprite->setVisible(false);
        }
    }
    /*
    if(strcmp(_card->getClassName(), "SGOfficerCard") || strcmp(_card->getClassName(), "SGEquipCard"))
    {
        for(int i=starCount; i < _card->getUpgradestar();i++)
        {
            //这是一个坑  bluestar 是一个黑色的星星图案。
            CCSprite *sprite = CCSprite::createWithSpriteFrameName("bluestar.png");
            sprite->setAnchorPoint(ccp(0.5, 1));
            sprite->setPosition(ccpAdd(((CCSprite *)arrayStar->objectAtIndex(i))->getPosition(), ccp(0,-3.5)));
            bluestar->addObject(sprite);
            this->addChild(sprite);
        }
    }
     */
}

bool SGCardItem::checkCanStrength(SGBaseMilitaryCard *checkCard,CardType cardType)
{
    int type = 0;
    if (cardType == cardType_officer)
    {
        type = SGStaticDataManager::shareStatic()->getStrengOfficerType(checkCard->getItemId());
    }
    else
    {
    }
    
    //由等级来限制，不再使用经验，（当前卡卡片的等级不能高于主公人物的等级）
    if (checkCard->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
    {
        return false;
    }
    return  true;
}

bool SGCardItem::checkCanUpdate(SGBaseMilitaryCard *checkCard,int type)
{
    if (checkCard->getCurrLevel() < checkCard->getMaxLevel()) {
        return false;
    }
    CCObject *obj = NULL;
    CCDictionary* dict = CCDictionary::create();
    if (type == cardType_officer)
    {
        SGOfficerCard * officer = (SGOfficerCard *)checkCard;
        CCArray *reqItemIDs = officer->getRequitemIds();
        
        CCARRAY_FOREACH(reqItemIDs, obj)
        {
            CCString *itemid = (CCString *)obj;
            CCString *str = (CCString *)dict->objectForKey(itemid->getCString());
            if (str)
            {
                int value = str->intValue();
                value ++;
                str = CCString::createWithFormat("%d",value);
            }
            else
            {
                str = CCString::createWithFormat("%d",1);
            }
            dict->setObject(str, itemid->getCString());
        }
    }
    else if(type == cardType_equip)
    {
        SGEquipCard *equip = (SGEquipCard *)checkCard;
        CCArray *reqItemIDs = equip->getRequitemIds();
        
        CCARRAY_FOREACH(reqItemIDs, obj)
        {
            CCString *itemid = (CCString *)obj;
            CCString *str = (CCString *)dict->objectForKey(itemid->getCString());
            if (str)
            {
                int value = str->intValue();
                value ++;
                str = CCString::createWithFormat("%d",value);
            }
            else
            {
                str = CCString::createWithFormat("%d",1);
            }
            dict->setObject(str, itemid->getCString());
        }
    }
    else
    {
        return  false;
    }
    
    CCArray *datas = dict->allKeys();
    if (datas == NULL) {
        return  false;
    }
    
    CCArray *playerProp = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject* obj_ =NULL;
    CCArray *haveCount = CCArray::create();
    CCARRAY_FOREACH(datas, obj)
    {
        int i_temp = 0;
        CCString *temp = (CCString *)obj;
        CCARRAY_FOREACH(playerProp, obj_)
        {
            SGPropsCard *card = (SGPropsCard *)obj_;
            if (temp->intValue() == card->getItemId()) {
                i_temp++;
            }
        }
        haveCount->addObject(CCString::createWithFormat("%d",i_temp));
    }
    int size = datas->count();
    for (int i = 0; i<size; i++)
    {
        CCString *obj = (CCString *)datas->objectAtIndex(i);
        CCString *count = (CCString *)dict->objectForKey(obj->getCString());
        int needCount = count->intValue();
        CCString *str_hav = (CCString *)haveCount->objectAtIndex(i);
        int haveCount = str_hav->intValue();
        if (haveCount < needCount)
        {
            return  false;
        }
    }
    return true;
}
///////////
void SGCardItem::buttonClick(CCObject*obj)
{
   
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    if (delegate) {
        delegate->itemselect(tableview,ind);
    }
}
void SGCardItem::setIsCanSelect(bool isCanSelecdt,bool isUpbattel,bool isEnoughPosition, bool haveSameName)
{
    setisLocked(isCanSelecdt);
    btn->setColor(ccWHITE);
    lessGov->setVisible(false);
    lock_sprite->setVisible(false);
//    comboxSprite->setVisible(true);
  
    //不可点击的
	//先判定是否有同名武将
	if (haveSameName)
	{
		lessGov->setString(str_CardItem_str11);
		//lessGov->setPosition(ccpAdd(notice->getPosition(), ccp(15, 35)));
		lessGov->setVisible(true);
		lock_sprite->setVisible(true);
		btn->setColor(ccc3(80, 80, 80));
	}
    else if (!isCanSelecdt && !isUpbattel && isEnoughPosition)
	{
        lessGov->setString(str_CardItem_str1);
		//lessGov->setPosition(ccpAdd(notice->getPosition(), ccp(15, 35)));
        lessGov->setVisible(true);
        lock_sprite->setVisible(true);
            
        btn->setColor(ccc3(80, 80, 80));
       
    }
	else if(!isCanSelecdt &&!isUpbattel)
    {
        lessGov->setVisible(false);
        lock_sprite->setVisible(true);
        
        btn->setColor(ccc3(80, 80, 80));
    }
	else if(isUpbattel)
    {
        lessGov->setVisible(true);
		//lessGov->setPosition(ccpAdd(notice->getPosition(), ccp(15, 35)));
        lock_sprite->setVisible(false);
    }
        
        
}
void SGCardItem:: changeSelected(bool isSelected){
//    chooseSprite->setVisible(isSelected);
    isSelectSpr->setPosition(ccpAdd(spriteIsUseBG->getPosition(), ccp(30, 45)));
    if(isSelected)
    {
        CCSprite *spriteSelectOK =  CCSprite::createWithSpriteFrameName("streng_choose.png");
        isSelectSpr->setDisplayFrame(spriteSelectOK->displayFrame());
        isSelectSpr->setVisible(true);
        
    }else{
        CCSprite *spriteSelectNO =  CCSprite::createWithSpriteFrameName("baikuang.png");
        isSelectSpr->setDisplayFrame(spriteSelectNO->displayFrame());
        isSelectSpr->setVisible(true);
    }
    
    if (isSelected) {
        lessGov->setString(str_CardItem_str3);
        lessGov->setVisible(isSelected);
        lock_sprite->setVisible(false);

    }


}

//void SGCardItem::callback()
//{
//    CCLog("\n\n\n\n..zyc debug: num of actions running : %d\n\n\n\n" , CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(notice));
//}

