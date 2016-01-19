//
//  SGCardsLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-10.
//
//

#include "SGCardsLayer.h"
#include "SGMainManager.h"
#include "SGAllCardItem.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGHaveCards.h"
#include "SimpleAudioEngine.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGCardsLayer::SGCardsLayer()
:enterType(0),
_array(NULL)
{
}
SGCardsLayer::~SGCardsLayer()
{
    CC_SAFE_RELEASE(datas);
    CC_SAFE_RELEASE(_array);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_cardsLayer);
    
}
SGCardsLayer *SGCardsLayer::create(CCArray *array,int type)
{
    SGCardsLayer *cardsLayer = new SGCardsLayer();
    if (cardsLayer && cardsLayer->init(NULL, sg_cardsLayer))
    {
        cardsLayer->enterType = type;
        cardsLayer->initView(array);
        cardsLayer->autorelease();
        return cardsLayer;
    }
    CC_SAFE_DELETE(cardsLayer);
    return NULL;
}
void SGCardsLayer::initView(CCArray *array)
{
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_cardsLayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_cardsLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_cardsLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_cardsLayer, LIM_PNG_AS_PNG);

    tableViewHeight = 145;
    tableViewColumns = 5;
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGCardsLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);

    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));

    
    
    SGCCLabelTTF *title = NULL;
    
    datas = CCArray::create();
    datas->retain();
    _array = CCArray::create();
    _array->retain();
    if (enterType == 0 || enterType == 1 || enterType == 2 || enterType == 3) {
        
        CCDictionary * dic = NULL;
        switch (enterType) {
            case 0:
            {
                title = SGCCLabelTTF::create(str_CardsLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
                dic = SGStaticDataManager::shareStatic()->getWeiOfficerDict();
            }
                break;
            case 1:
            {
                title = SGCCLabelTTF::create(str_CardsLayer_str2, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
                dic = SGStaticDataManager::shareStatic()->getShuOfficerDict();
            }
                break;
            case 2:
            {
                title = SGCCLabelTTF::create(str_CardsLayer_str3, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
                dic = SGStaticDataManager::shareStatic()->getWuOfficerDict();
            }
                break;
            case 3:
            {
                title = SGCCLabelTTF::create(str_CardsLayer_str4, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
                dic = SGStaticDataManager::shareStatic()->getQunOfficerDict();
            }
                break;
            case 4:
            {
                dic = SGStaticDataManager::shareStatic()->getEquipViewDict();
            }
                break;

        }

        
        for (int i = 0; i<dic->allKeys()->count(); i++) {
            CCString *string = (CCString*)dic->allKeys()->objectAtIndex(i);
            SGOfficerDataModel *oCard = SGStaticDataManager::shareStatic()->getOfficerById(string->intValue());
            
            if (oCard)
            {
                int areId = ((CCString*)((CCDictionary *)dic->objectForKey(string->getCString()))->objectForKey("areaId"))->intValue();
                
                SGOfficerCard *officerCard = new SGOfficerCard;
                officerCard->setSsid(areId);//排序用
                
                officerCard->setOfficerName(oCard->getOfficerName()); 
                officerCard->setMaxLevel(oCard->getOfficerMaxLevel());
                officerCard->setCurrExp(0);
                officerCard->setCurrStar(oCard->getOfficerCurrStarLevel());
                officerCard->setCurrLevel(1);
                officerCard->setRace(oCard->getOfficerRace());
                officerCard->setItemId(oCard->getOfficerId());
                //转生最大星级
                officerCard->setUpgradestar(oCard->getUpgradestar());
                //转生等级
                officerCard->setUpgradelevel(oCard->getUpgradelevel());
                //武将计最大等级
                officerCard->setSkillMaxLevel(oCard->getSkillMaxLevel());
                //主将计最大等级
                officerCard->setLordMaxLevel(oCard->getLordMaxLevel());
                //性别和缘分
                officerCard->setGender((OfficerGender) oCard->getOfficerGender());
                officerCard->setFateList(oCard->getOfficerFateList());
                //武将原型ID
                officerCard->setProtoId(oCard->getOfficerProtoId());
                //速度修正系数
                officerCard->setSpeedFactor(oCard->getOfficerSpeedFactor());
                //转生次数
                officerCard->setAdNum(oCard->getAdvNum());
                officerCard->setHeadSuffixNum(oCard->getIconId());//12.05
                
                if (oCard->getOfficerGeneralsSkill()&& !oCard->getOfficerGeneralsSkill()->isEqual(CCString::create(""))) {
                   // CCLOG("%s",oCard->getOfficerGeneralsSkill()->getCString());
                    
                    officerCard->setOfficerSkil(SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(oCard->getOfficerGeneralsSkill()->getCString()));
                }
                
                if (oCard->getOfficerLordSkill() &&!oCard->getOfficerLordSkill()->isEqual(CCString::create(""))) {
                    
                    int lordskill = SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(oCard->getOfficerLordSkill()->getCString());
                    
                    
                    officerCard->setLordSkill(lordskill);
                    
                }
                officerCard->setAtk(oCard->getOfficerBaseAtk());
                officerCard->setDef(oCard->getOfficerBaseDef());
                officerCard->setRound(oCard->getOfficerRound());
                officerCard->setSpeed(oCard->getOfficerBaseSpeed());
                officerCard->setMorale(oCard->getOfficerMorale());
                officerCard->setGoverning(oCard->getOfficerGoverning());
                officerCard->setHeadSuffixNum(oCard->getIconId());//12.05
                
                datas->addObject(officerCard);
                officerCard->autorelease();
            }
        }
        sortArray();
        CCObject *obj = NULL;
        for (int i = 0; i<datas->count(); i++) {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
            CCARRAY_FOREACH(array, obj)
            {
                SGHaveCards *temp = (SGHaveCards*)obj;
                if (temp->getcardItem() == card->getItemId()) {
                    temp->setcardPosition(i);
                    _array->addObject(temp);
                }
            }
        }

    }
   else if(enterType == 4){
        title = SGCCLabelTTF::create(str_CardsLayer_str5, FONT_XINGKAI, 36);
        title->setInsideColor(COLOR_UNKNOW_TAN);
        CCDictionary * dic = SGStaticDataManager::shareStatic()->getEquipViewDict();
        for (int i = 0; i<dic->allKeys()->count(); i++)
        {
        CCString *string = (CCString*)dic->allKeys()->objectAtIndex(i);
        SGEquipmentDataModel *eCard = SGStaticDataManager::shareStatic()->getEquipById(string->intValue());
        
        if (eCard)
        {
            int areId = ((CCString*)((CCDictionary *)dic->objectForKey(string->getCString()))->objectForKey("areaId"))->intValue();
                
            SGEquipCard *equipCard = new SGEquipCard;
            equipCard->setSsid(areId);
            equipCard->setCurrStar(eCard->getEquipCurrStarLevel());
            equipCard->setCurrExp(0);
            equipCard->setOfficerCardId(0);
            equipCard->setItemType(eCard->getEquipType());
            equipCard->setIsProt(0);
            equipCard->setCurrLevel(1);
            equipCard->setOfficerName(CCString::create(eCard->getEquipName()->getCString()));
            equipCard->setItemId(eCard->getEquipmentId());
            equipCard->setMorale(eCard->getEquipHP());
            equipCard->setAtk(eCard->getEquipmentBaseAtk());
            equipCard->setDef(eCard->getEquipBaseDef());
            equipCard->setGoverning(eCard->getEquipGoverning());
            equipCard->setRound(eCard->getEquipRound());
            equipCard->setSpeed(eCard->getEquipBaseSpeed());
            equipCard->setMaxLevel(eCard->getEquipMaxLevel());
           
            //转生最大星级
            equipCard->setUpgradestar(eCard->getUpgradestar());
            //转生等级
                equipCard->setUpgradelevel(eCard->getUpgradelevel());
                //转生次数
                equipCard->setAdNum(eCard->getAdvNum());
            //原型ID
            equipCard->setProtoId(eCard->getEquipProtoId());
                
            equipCard->setHeadSuffixNum(eCard->getIconId());//12.05
            datas->addObject(equipCard);
            equipCard->autorelease();
        }
        }
        sortArray();
        CCObject *obj = NULL;
        for (int i = 0; i<datas->count(); i++) {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
            CCARRAY_FOREACH(array, obj)
            {
                SGHaveCards *temp = (SGHaveCards*)obj;
                if (temp->getcardItem() == card->getItemId()) {
                    temp->setcardPosition(i);
                    _array->addObject(temp);
                }
            }
        }


    }

    if (title) {
        title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
        this->addChild(title);
    }
    tableView->setFrame(CCRectMake(0, 0, s.width,  s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, btmhgt)));    
        
}

SNSTableViewCellItem *SGCardsLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGAllCardItem *item = (SGAllCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBaseMilitaryCard *card = NULL;
    CCObject *obj = NULL;
    int state = 0;
    if (num < datas->count()) {
        card = (SGBaseMilitaryCard *)datas->objectAtIndex(num);
        CCARRAY_FOREACH(_array, obj)
        {
            SGHaveCards  *pcard = (SGHaveCards *)obj;
            if ((pcard->getcardPosition()) == num)
            {
                state = pcard->getcardState();
                break;
            }
        }
    }

    if (card == NULL) {
        if (NULL == item) {
            item = SGAllCardItem::create(0);
        }
        else
        {
            item->updataCard(NULL,num);
        }
    }
    else
    {
        if (NULL == item) {
            item = SGAllCardItem::create(card,state);
        }
        else
        {
            item->updataCard(card,state);
        }
    }


    return item;
}
void SGCardsLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGBaseMilitaryCard *card = NULL;

    if (index < datas->count()) {
         card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
    }
//   CCObject* obj = NULL;
//    CCARRAY_FOREACH(_array, obj)
//    {
//        SGHaveCards *temp = (SGHaveCards*)obj;
//        if (temp->getcardPosition() == index) {
//            card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
//            break;
//        }
//    }
    
    if (card) {
        switch (enterType) {
            case 0:
            case 1:
            case 2:
            case 3:
            {
                //如果找不到图，就别往下show了，show也是崩溃
                if (card->getHeadSuffixNum() == -1)
                {
                    return ;
                }
               SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)card, 16,0,false);//估计其他的写错了
            }
                break;
            case 4:
            {
                SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)card, 4,false);
            }
                break;
            default:
                break;
        }
    }
}
void SGCardsLayer::sortArray()
{
    int arrayCount = datas->count();
    if(0 == arrayCount)
    {
        CCLOG("数组为空");
        return;
    }
    
    for(int i=0;i<arrayCount-1;i++)
    {
        for(int m=i+1;m<arrayCount;m++)
        {
            
            SGBaseMilitaryCard *cardSoldier = (SGBaseMilitaryCard*)datas->objectAtIndex(i);
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(m);
            if(cardSoldier->getSsid() > card->getSsid())
                {
                    datas->exchangeObjectAtIndex(i, m);
                }
        }
    }   

}

void SGCardsLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseLayer *layer = (SGBaseLayer *) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_allCardsLayer);
    SGMainManager::shareMain()->showLayer(layer);
}