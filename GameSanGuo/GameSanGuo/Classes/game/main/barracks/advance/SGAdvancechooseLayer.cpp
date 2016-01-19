//
//  SGAdvancechooseLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-8.
//
//

#include "SGAdvancechooseLayer.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGCardItem.h"
#include "cocos-ext.h"
#include "SGStrengBox.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ItemModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGMenu.h"
#include "SGGeneralInfoLayer.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "SGMainLayer.h"
#include "SGStrengLvlUpBox.h"
#include "SGStrengSoldierLayer.h"
#include "SimpleAudioEngine.h"
#include "SGTeamgroup.h"
#include <algorithm>
#include "AppMacros.h"

#include "ResourceManager.h"
#include "SGStringConfig.h"
static int SGLess(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getselectposition() < ((SGBaseMilitaryCard*)p2)->getselectposition();
}
SGAdvancechooseLayer::SGAdvancechooseLayer()
:cardCount(0)
,_card(NULL)
,addprogress(NULL)
,add(true)
,selectposition(0)
,selectcardcount(0),
fermEffect(NULL),
isfromfirst(false),
onlyOfficer(false),
Icon(NULL),
lab_lvl(NULL),
lab_orilvl(NULL),
need(NULL),
afterExp(NULL),
skilluprate(NULL),
progress(NULL),
needCoins(0),
officer(NULL),
equip(NULL),
soldier(NULL),
_arraySelectCard(NULL),
_remain(NULL),
_beforecard(NULL),
range(NULL),
cardType(0),
isSelect(0),
okBtn(NULL),
tabelItem(NULL),
lab(NULL),
lab2(NULL)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("strengofficersort", CCString::createWithFormat("%d",3)->getCString());
    CCUserDefault::sharedUserDefault()->setStringForKey("strengequipsort", CCString::createWithFormat("%d",3)->getCString());
    CCUserDefault::sharedUserDefault()->flush();
    
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(false);
	
	for (int i = 0; i < 300; i++)
	{
		selectItem[i] = 0;
	}
    
    
}
SGAdvancechooseLayer::~SGAdvancechooseLayer()
{
    CCLOG("~SGAdvancechooseLayer");
    //    _arraySelectCard->release();
    datas->release();
    _card->release();
    _remain->release();
    
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_advancechooselayer);

}

static int SGAdvSortMore(const CCObject* p1, const CCObject* p2)
{
	return (((SGOfficerCard *)p1)->getAdvSortNum() < ((SGOfficerCard *)p2)->getAdvSortNum());
}
SGAdvancechooseLayer *SGAdvancechooseLayer::create(SGBaseMilitaryCard *card,int type,bool enter,bool isOnlyOfficer,int currentPage,CCArray *array)
{
    SGAdvancechooseLayer *strengLayer = new SGAdvancechooseLayer();
//    for (int i = 0;i<3;i++) {
//        strengLayer->beforeselect[i] = a[i];
//    }
    if (strengLayer && strengLayer->init(card, type,isOnlyOfficer))
    {
        
        
//        if (strengLayer->_beforecard) {
//            strengLayer->_beforecard->retain();
//        }
        strengLayer->_beforecard = array;
        strengLayer->isfromfirst = enter;
        strengLayer->onlyOfficer = isOnlyOfficer;
        strengLayer->initView();
        strengLayer->_currentPage=currentPage ;
        strengLayer->autorelease();
        return strengLayer;
    }
    CC_SAFE_DELETE(strengLayer);
    return NULL;
}
bool SGAdvancechooseLayer::init(SGBaseMilitaryCard *card, int type,bool isOnlyOfficer)
{
    
     
    
    
    
    if (!SGBaseTableLayer::init(NULL, sg_advancechooselayer))
    {
        return false;
    }
    _card = card;
    
    _card->retain();
    
    cardType = type;
    
    datas = CCArray::create();
    datas->retain();
    
    _arraySelectCard = CCArray::create();
    _arraySelectCard->retain();
    
    _beforecard = CCArray::create();
    _beforecard->retain();
//    if (_beforecard && _beforecard->count()!=0) {
//        for (int i = 0;i<_beforecard->count(); i++) {
//            _arraySelectCard->addObject(_beforecard->objectAtIndex(i));
//        }
//    }
    
    _remain = CCArray::create();
    _remain->retain();
    
    if (cardType ==1) {
        CCObject *obj_ = NULL;
        
      
        officer = (SGOfficerCard *)_card;
        CCArray *offices = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
        
//        if (isOnlyOfficer) {
        
        if (_card->getCurrStar()>=1) {
            CCARRAY_FOREACH(offices, obj_)
            {
                SGOfficerCard *card = (SGOfficerCard *)obj_;
                
                
                int po1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,card);
                int po2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,card);
                int po3 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,card);
                if ( card->getSsid()!=_card->getSsid() && po1 == 0 && po2 ==0 && po3 == 0 && card->getProtoId()==((SGOfficerCard*)_card)->getProtoId() && card->getIsProt()==0 && card->getCurrStar() == _card->getCurrStar())
                {
                   
                    card->setAdvSortNum(card->getUpgradestar()*100+card->getAdNum()*10+card->getCurrLevel());
                     datas->addObject(card);
                    //                    officerNum++;
                }
                //                else if((po1!=0 ||po2!=0 ||po3!= 0) && card->getCurrStar()==_card->getCurrStar())
                //                {
                //                    _remain->addObject(card);
                ////                    datas->addObject(card);
                //                }
            }

        }
           else
           {
               CCARRAY_FOREACH(offices, obj_)
               {
                   SGOfficerCard *card = (SGOfficerCard *)obj_;
                   
                   
                   int po1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,card);
                   int po2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,card);
                   int po3 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,card);
                   if ( card->getSsid()!=_card->getSsid() && po1 == 0 && po2 ==0 && po3 == 0 && card->getCurrStar()==_card->getCurrStar() && card->getIsProt()==0)
                   {
                       
                       card->setAdvSortNum(card->getUpgradestar()*100+card->getAdNum()*10+card->getCurrLevel());
                       datas->addObject(card);
                       //                    officerNum++;
                   }
                   //                else if((po1!=0 ||po2!=0 ||po3!= 0) && card->getCurrStar()==_card->getCurrStar())
                   //                {
                   //                    _remain->addObject(card);
                   ////                    datas->addObject(card);
                   //                }
               }

           
           }

        std::sort(datas->data->arr, datas->data->arr + datas->data->num, SGAdvSortMore);
        
    }
    else if(cardType == 2)
    {
        equip = (SGEquipCard *)_card;
        CCArray *equips = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
        CCObject *obj_ = NULL;
//        if (_card->getCurrStar()>=4 && _card->getAdNum()==3)
//        {
//            CCARRAY_FOREACH(equips, obj_)
//            {
//                SGEquipCard *card = (SGEquipCard *)obj_;
//                if (card->getSsid() != _card->getSsid() &&card->getOfficerCardId() == 0 &&card->getIsProt() == 0 && card->getCurrStar() == _card->getCurrStar())
//                {
//                    datas->addObject(card);
//                }
//                //            else if (card->getOfficerCardId()!= 0 )
//                //            {
//                //                _remain->addObject(card);
//                ////                datas->addObject(card);
//                //            }
//            }
//
//        }
//        else
//        {
            CCARRAY_FOREACH(equips, obj_)
            {
                SGEquipCard *card = (SGEquipCard *)obj_;
                if (card->getSsid() != _card->getSsid() &&card->getOfficerCardId() == 0 &&card->getIsProt() == 0 && card->getCurrStar() == _card->getCurrStar())
                {
                    datas->addObject(card);
                }
                //            else if (card->getOfficerCardId()!= 0 )
                //            {
                //                _remain->addObject(card);
                ////                datas->addObject(card);
                //            }
            }
//    SGSortBox::sortCards(datas, 4,0);
        }
//    }
    

 

    
    tableViewHeight = 108;
    
    return true;
}
void SGAdvancechooseLayer::initMsg()
{
    
}
void SGAdvancechooseLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/strengjiantoutexiao.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/strengqhqr.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-3303.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_advancechooselayer);
    
    if (_beforecard && _beforecard->count()!=0)
    {
        for (int i = 0; i<datas->count(); i++)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)datas->objectAtIndex(i);
            for (int j = 0;j<_beforecard->count(); j++)
            {
                SGBaseMilitaryCard *card2 = (SGBaseMilitaryCard *)_beforecard->objectAtIndex(j);
                if (card->getSsid() == card2->getSsid())
                {
                    selectItem[i]=1;
                    _arraySelectCard->addObject(card);
                    cardCount+=1;
                }
            }
        }

    }
    

    
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSprite* title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-5);
    
    CCSprite *titlecenter =  CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    
    CCSprite *titlebg =  CCSprite::createWithSpriteFrameName("barrack_title_bg.png");
    this->addChild(titlebg);
    titlebg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,titlebg->getContentSize().height/2)));
    
    CCSprite *gonggao_bg_3_l =  CCSprite::createWithSpriteFrameName("streng_huawen.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0.5, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(0,titlebg->getContentSize().height)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r =  CCSprite::createWithSpriteFrameName("streng_huawen.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(0.5, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(s.width,titlebg->getContentSize().height)));
    this->addChild(gonggao_bg_3_r,3);
    
    SGCCLabelTTF *needCoin = SGCCLabelTTF::create(str_cost_tong_money, FONT_BOXINFO, 24);
    needCoin->setAnchorPoint(ccp(0, 0.5));
    this->addChild(needCoin);
    needCoin->setPosition(ccpAdd(titlebg->getPosition(), ccp(-290, 0)));
    
    SGCCLabelTTF *haveCoin = SGCCLabelTTF::create(str_you_have_tong_money, FONT_BOXINFO, 24);
    haveCoin->setAnchorPoint(ccp(0, 0.5));
    this->addChild(haveCoin);
    haveCoin->setPosition(ccpAdd(titlebg->getPosition(), ccp(10, 0)));
    
    CCSprite *coinIcon =  CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(coinIcon);
    coinIcon->setPosition(ccpAdd(needCoin->getPosition(), ccp(needCoin->getContentSize().width*1.15, 0)));
    
    CCSprite *coinIcon1 =  CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(coinIcon1);
    coinIcon1->setPosition(ccpAdd(haveCoin->getPosition(), ccp(haveCoin->getContentSize().width*1.15, 0)));
    
    need = SGCCLabelTTF::create("0", FONT_BOXINFO, 24);
    need->setAnchorPoint(ccp(0, 0.5));
    this->addChild(need);
    need->setPosition(ccpAdd(coinIcon->getPosition(), ccp(coinIcon->getContentSize().width*.85, 0)));
    
    CCString* str_coins = CCString::createWithFormat("%d",playerInfo->getPlayerCoins());
    SGCCLabelTTF *have = SGCCLabelTTF::create(str_coins->getCString(),  FONT_BOXINFO, 24);
    have->setAnchorPoint(ccp(0, 0.5));
    this->addChild(have);
    have->setPosition(ccpAdd(coinIcon1->getPosition(), ccp(coinIcon1->getContentSize().width*.85, 0)));
    
    
    CCSprite *rightbian =  CCSprite::createWithSpriteFrameName("public_bian.png");
    rightbian->setAnchorPoint(ccp(1, 1));
    rightbian->setFlipX(true);
    this->addChild(rightbian,-1);
    rightbian->setScaleY(1.5);
    rightbian->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0, -title_bg->getContentSize().height)));
    
    CCSprite *leftbian =  CCSprite::createWithSpriteFrameName("public_bian.png");
    leftbian->setAnchorPoint(ccp(0, 1));
    this->addChild(leftbian,-1);
    leftbian->setScaleY(1.5);
    leftbian->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -title_bg->getContentSize().height)));
    
    int modelTag = 0;
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png",
                                         str_back,
                                         this,
                                         menu_selector(SGAdvancechooseLayer::backHandler),
                                         CCPointZero,
                                         FONT_PANGWA,
                                         ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGAdvancechooseLayer::sortHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 0.5));
    range->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp( -range->getContentSize().width*0.05, -range->getContentSize().height*.55)));

    //这里新手引导修改，这里不再点击返回按钮，直接点击home按钮
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_18);
    backBtn->setTag(modelTag);
    
    
    
    CCSprite *bg =  CCSprite::createWithSpriteFrameName("barrack_bg.png");
    //modify by:zyc.
    CCRect re = CCRectMake(0, 30, bg->getContentSize().width, s.height - titlebg->getContentSize().height - title_bg->getContentSize().height);
    bg->setTextureRect(re);
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        bg->setScaleY(1.08);
    }
    else
    {
        bg->setScaleY(1.09);
    }
    bg->cocos2d::CCNode::setScaleX(s.width/bg->getContentSize().width * 1.02);
    
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-5, -30));
    
    
    int shift = -30;
   
    lab = SGCCLabelTTF::create(str_advance_tips,FONT_PANGWA,20);
    lab->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,-lab->getContentSize().height*3 + shift)));
    this->addChild(lab);
    lab2 = SGCCLabelTTF::create(str_use_after_zhuangbei_auto_,FONT_PANGWA,20);
    lab2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,-lab2->getContentSize().height*4 + shift)));
    this->addChild(lab2);
    
    if (cardType == 2) {
        
        lab->setString(str_max_3_zhuangbei);

        
    }
    
    //向需要用到引导的页面提前加入 对应的
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_15);
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height -title_bg->getContentSize().height-239));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, 130)));
    
    
    CCSprite *title =  CCSprite::createWithSpriteFrameName("barrack_font_xzsck.png");
    this->addChild(title);
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    CCString * xx= CCString::createWithFormat("%d",_card->getCost());
    need->setString(xx->getCString());
    
    
    
    
    if (datas->count() == 0)
    {
        if (!onlyOfficer) {
            
            SGButton *Btn = SGButton::create("box_btnbg.png",
                                             "public_font_fanhui.png",
                                             this,
                                             menu_selector(SGAdvancechooseLayer::visitHandle),
                                             ccp(0, 0),
                                             false,
                                             true);
            this->addBtn(Btn);
            Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, b)));
            
            
            SGCCLabelTTF *notice = SGCCLabelTTF::create(str_nohave_this_star_role, FONT_BOXINFO, 32);
            this->addChild(notice);
            notice->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 50)));
            //notice->setPosition(SGLayout::getPoint(kMiddleCenter));
            
            SGCCLabelTTF *notice2 = SGCCLabelTTF::create(str_tips_youmaybeto_vist, FONT_BOXINFO, 28);
            this->addChild(notice2);
            notice2->setPosition(ccpAdd(Btn->getPosition(), ccp(0, 200)));
            lab->setVisible(false);
            lab2->setVisible(false);
        }
        else{
            SGButton *Btn = SGButton::create("box_btnbg.png",
                                             "public_font_fanhui.png",
                                             this,
                                             menu_selector(SGAdvancechooseLayer::visitHandle),
                                             ccp(0, 0),
                                             false,
                                             true);
            this->addBtn(Btn);
            Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, b)));
            
            
            SGCCLabelTTF *notice = SGCCLabelTTF::create(str_nohave_this_star_zhuangbei, FONT_BOXINFO, 32);
            this->addChild(notice);
            notice->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 50)));
            //notice->setPosition(SGLayout::getPoint(kMiddleCenter));
            
            SGCCLabelTTF *notice2 = SGCCLabelTTF::create(str_tips_youmaybeto_vist1, FONT_BOXINFO, 28);
            this->addChild(notice2);
            notice2->setPosition(ccpAdd(Btn->getPosition(), ccp(0, 200)));
            lab->setVisible(false);
            lab2->setVisible(false);

            
        }
    }
    else
    {
        SGButton *Btn = SGButton::create("box_btnbg.png",
                                         "public_font_queding.png",
                                         this,
                                         menu_selector(SGAdvancechooseLayer::clickOK),
                                         ccp(0, 0),
                                         false,
                                         true);
        Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,90)));
        this->addBtn(Btn);
    
    }
}

void SGAdvancechooseLayer::clickOK()
{
    if (_arraySelectCard &&_arraySelectCard->count()!=0) {
        SGMainManager::shareMain()->showAdvanceLayer(_card,cardType,_currentPage,_arraySelectCard);
    }
    else
    {
        SGMainManager::shareMain()->showAdvanceLayer(_card,cardType,_currentPage,NULL);
    }

}

void SGAdvancechooseLayer::changeNum(int num)
{
    cardCount+= num;
//    CCString *cardnum = CCString::createWithFormat("(已选%d/5)",cardCount);
//    choose->setString(cardnum->getCString());
}

SNSTableViewCellItem* SGAdvancechooseLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	Tabel_Item1 *item = (Tabel_Item1*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(num);
   // SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
    CCLOG("indexPath->getRow()  is %d",num);
    if (item == NULL) {
        item = Tabel_Item1::create(card,num);
        item->updateOfficerCard(card,indexPath->getRow(),selectItem[indexPath->getRow()]);
    }
    else
    {
        item->updateOfficerCard(card,indexPath->getRow(),selectItem[indexPath->getRow()]);
    }
    if (cardType == 1) {

        if (_arraySelectCard->count() == 3)
        {
            item->setIsCanSelect(_arraySelectCard->containsObject(card));
        }
//        else if (p1!=0 || p2!=0 || p3!= 0)
        else if (_remain->containsObject(card))
        {
            item->setIsCanSelect(false);
        }
       
        else
        {
            item->setIsCanSelect(true);
        }

    }
    else if(cardType == 2)
    {
        if (_arraySelectCard->count() == 3)
        {
            item->setIsCanSelect(_arraySelectCard->containsObject(card));
        }
        else if (_remain->containsObject(card))
        {
          item->setIsCanSelect(false);
        }
        else
        {
            item->setIsCanSelect(true);
        }

    }
    item->setTouchEnabled(true);
	return item;
}

void SGAdvancechooseLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    CCLOG("btntag:%d",btnTag);
    
    if (btnTag) {
        return;
    }
    Tabel_Item1 *item = (Tabel_Item1*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBaseMilitaryCard *basecard = item->_card;
    
    isSelect = indexPath->getRow();

    if(cardCount >= 3 && 0==selectItem[indexPath->getRow()])
    {
        if (cardType==1) {
            SG_SHOW_WINDOW(str_max_3_role);
        }
        else if(cardType == 2)
        {
         SG_SHOW_WINDOW(str_max_3_zhuangbei);
        }
        
        return;
    }
    if (_remain->containsObject(basecard)) {
        if (cardType == 1) {
            SG_SHOW_WINDOW(str_no_as_material_role_);
        }
        else if (cardType == 2)
        {
        SG_SHOW_WINDOW(str_no_as_material_zhuangbei);
        }
        
        return;
    }
    if(selectItem[indexPath->getRow()])
    {
        this->changeNum(-1);
        selectItem[indexPath->getRow()] = 0;
        CCLOG("_arraySelectCard->indexOfObject(basecard) %d",_arraySelectCard->indexOfObject(basecard));
        _arraySelectCard->removeObject(datas->objectAtIndex(indexPath->getRow()));
        
    }else{
        
        CCString *str_ = CCString::create(basecard->getClassName());
        if (str_->isEqual(CCString::create("SGOfficerCard")))
        {
            this->changeNum(1);
            selectItem[indexPath->getRow()] = 1;
            _arraySelectCard->addObject(datas->objectAtIndex(indexPath->getRow()));
        }
        else{
            this->changeNum(1);
            selectItem[indexPath->getRow()] = 1;
            _arraySelectCard->addObject(datas->objectAtIndex(indexPath->getRow()));
        }

        
    }
//    updateCoind();
    item->setItemSelect(0 != selectItem[indexPath->getRow()]);
    tabelItem = item;
    
    tableView->reloadData(false);
    
    std::sort(_arraySelectCard->data->arr, _arraySelectCard->data->arr + _arraySelectCard->data->num, SGLess);
}

void SGAdvancechooseLayer::selectItemByIndex(int index)
{
    this->changeNum(1);
    selectItem[index] = 1;
    _arraySelectCard->addObject(datas->objectAtIndex(index));
//    updateCoind();
}

void SGAdvancechooseLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *type = (CCString *)dict->valueForKey("type");
        if(type->intValue() == 5)
        {
            this->changeNum(1);
            selectItem[isSelect] = 1;
            _arraySelectCard->addObject(datas->objectAtIndex(isSelect));
            tabelItem->setItemSelect(0 != selectItem[isSelect]);
//            updateCoind();
            SGBaseMilitaryCard *basecard = tabelItem->_card;
            basecard->setselectposition(selectposition);
            
            CCSprite *position = (CCSprite *)this->getChildByTag(basecard->getselectposition());
            while (position->getChildrenCount()) {
                selectposition++;
                basecard->setselectposition(selectposition);
                position = (CCSprite *)this->getChildByTag(basecard->getselectposition());
            }
            CCSprite *cardposition = (CCSprite *)this->getChildByTag(basecard->getselectposition());
            
            CCSprite *ico = (CCSprite*)this->getChildByTag(100+basecard->getselectposition());
            ico->setVisible(false);
            selectposition ++;
//            CCSprite *icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",basecard->getItemId())->getCString());
			 CCSprite *icon =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",basecard->getHeadSuffixNum())->getCString());//12.05
            icon->setPosition(ccp(cardposition->getContentSize().width/2, cardposition->getContentSize().height/2));
            cardposition->addChild(icon);
            CCSprite *kuang =  CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
            cardposition->addChild(kuang);
            kuang->setPosition(icon->getPosition());
            CCString *string = CCString::create(basecard->getClassName());
            if (string->isEqual(CCString::create("SGOfficerCard")))
            {
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
//                kuang->setDisplayFrame( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString())->displayFrame());
                if (kuang)
                {
                    kuang->removeChildByTag(30);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(kuang->getContentSize().width*0.3,kuang->getContentSize().height - countryName->getContentSize().height/4));
                    kuang->addChild(countryName,30,30);
                }
            }
            tableView->reloadData(false);
        }
    }
}
void SGAdvancechooseLayer::updateCoind()

{

    
//    CCString *cardnum = CCString::createWithFormat("(已选%d/5)",cardCount);
//    choose->setString(cardnum->getCString());
    
//    if (cardType != 4)
//    {
//        CCString *str_ = CCString::create(_card->getClassName());
//        if (addprogress)
//        {
//            this->removeChild(addprogress, true);
//        }
//        int i = 100;
//        if (cardType == 1) {
//            switch (_card->getCurrStar()) {
//                case 1:
//                    i = 100;
//                    break;
//                case 2:
//                    i = 500;
//                    break;
//                case 3:
//                    i = 1000;
//                    break;
//                case 4:
//                    i = 1500;
//                    break;
//                case 5:
//                    i = 3000;
//                    break;
//                case 6:
//                    i = 5000;
//                    break;
//                default:
//                    break;
//            }
//            
//        }else if (cardType == 2)
//        {
//            switch (_card->getCurrStar()) {
//                case 1:
//                    i = 30;
//                    break;
//                case 2:
//                    i = 150;
//                    break;
//                case 3:
//                    i = 300;
//                    break;
//                case 4:
//                    i = 450;
//                    break;
//                case 5:
//                    i = 900;
//                    break;
//                case 6:
//                    i = 1500;
//                    break;
//                default:
//                    break;
//            }
//            
//        }
//        
//        needCoins = cardCount * i *  ((_card->getCurrLevel() - 1 )*.1  + 1 );
//        if (str_->isEqual(CCString::create("SGSoldierCard")))
//        {
//            needCoins = cardCount *150*  ((_card->getCurrLevel() - 1 )*.1  + 1 );
//        }
//        CCString *aa = CCString::createWithFormat("%d", needCoins);
//        
//        need->setString(aa->getCString());
//        
////        if (cardType == 1)
////        {
////            int rate = 0;
////            SGOfficerCard *temp = (SGOfficerCard *)_card;
////            CCObject *obj = NULL;
////            if (temp->getLordSkill())
////            {
////                SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(temp->getLordSkill());
////                CCARRAY_FOREACH(_arraySelectCard, obj)
////                {
////                    SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)obj;
////                    CCString *str_ = CCString::create(card->getClassName());
////                    if (str_->isEqual(CCString::create("SGOfficerCard")))
////                    {
////                        SGSkillDataModel *lordSkill1 = SGStaticDataManager::shareStatic()->getLordSkillById(((SGOfficerCard *)card)->getLordSkill());
////                        if (lordSkill  &&lordSkill1 &&temp->getLordSkill() &&lordSkill->getSkillUpId() == lordSkill1->getSkillUpId()  )
////                        {
////                            rate += (card->getCurrStar() *3 + 15);
////                        }
////                    }
////                }
////                temp->getLordTimes();
////                rate -= temp->getLordTimes()*2*temp->getCurrStar();
////                if (rate>100)
////                {
////                    rate = 100;
////                }
////                skilluprate->setString(CCString::createWithFormat("%d%%",rate)->getCString());
////                if (rate>0) {
////                    skilluprate->setColor(ccc3(0x0c, 0xff, 0x00));
////                }else
////                {
////                    skilluprate->setString("0%");
////                    skilluprate->setColor(ccWHITE);
////                }
////                
////            }
////        }
////        
////        int addEXP = 0;
////        int proExp = 0;
////        for(int i=0;i<_arraySelectCard->count();i++)
////        {
////            SGBaseMilitaryCard *card_ = (SGBaseMilitaryCard*)_arraySelectCard->objectAtIndex(i);
////            int cardCurrStar = card_->getCurrStar();
////            CCString *str_1 = CCString::create(card_->getClassName());
////            if (str_1->isEqual(CCString::create("SGPropsCard")))
////            {
////                proExp += (((SGPropsCard *)card_)->getValue());
////                if (str_->isEqual(CCString::create("SGOfficerCard")) &&((SGPropsCard *)card_)->getRace() == ((SGOfficerCard *)_card)->getRace()) {
////                    proExp = proExp + (((SGPropsCard *)card_)->getValue())*0.3;
////                }
////            }
////            else if(str_1->isEqual(CCString::create("SGOfficerCard")))
////            {
////                float expRate = card_->getExpRate();
////                int gov = card_->getGoverning();
////                int lvl = card_->getCurrLevel();
////                addEXP += (((gov>=0?gov:-gov) + cardCurrStar) * expRate * lvl);
////                if (((SGOfficerCard *)card_)->getRace() == ((SGOfficerCard *)_card)->getRace()) {
////                    addEXP = addEXP + ((gov + cardCurrStar) * expRate * lvl)*0.3;
////                }
////            }
////            else if(str_1->isEqual(CCString::create("SGEquipCard")))
////            {
////                float expRate = card_->getExpRate();
////                int gov = card_->getGoverning();
////                int lvl = card_->getCurrLevel();
////                addEXP += ((cardCurrStar + (gov>=0?gov:-gov)) * expRate * lvl);
////                if (((SGEquipCard *)card_)->getItemType() == ((SGEquipCard *)_card)->getItemType())
////                {
////                    addEXP = addEXP + ((cardCurrStar*2) * expRate * lvl)*0.3;
////                }
////            }
////        }
////        int totleExp = addEXP +proExp;
////        
////        if (_card->getMaxExp() == _card->getCurrExp()) {
////            progress->setPercentage(100);
////            return;
////        }
////        if (totleExp< 0) {
////            totleExp = 0;
////        }
////        afterExp->setString(CCString::createWithFormat("%d",totleExp)->getCString());
////        if (totleExp == 0)
////        {
////            afterExp->setColor(ccWHITE);
////        }else
////        {
////            afterExp->setColor(ccc3(0x26, 0xd9, 0xff));
////        }
////        int type = 0;
////        if(str_->isEqual(CCString::create("SGOfficerCard")))
////        {
////            SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
////            if (officer->getOfficerExpType()->isEqual(CCString::create("A"))) {
////                type = 0;
////            }
////            else if (officer->getOfficerExpType()->isEqual(CCString::create("B"))) {
////                type = 1;
////            }
////            else if (officer->getOfficerExpType()->isEqual(CCString::create("C"))) {
////                type = 2;
////            }
////            else if (officer->getOfficerExpType()->isEqual(CCString::create("D"))) {
////                type = 3;
////            }
////        }else if(str_->isEqual(CCString::create("SGSoldierCard")))
////        {
////            SGSoldierDataModel *officer = SGStaticDataManager::shareStatic()->getSoliderById(_card->getItemId());
////            if (officer->getSoldierExpType()->isEqual(CCString::create("A"))) {
////                type = 0;
////            }
////            else if (officer->getSoldierExpType()->isEqual(CCString::create("B"))) {
////                type = 1;
////            }
////            else if (officer->getSoldierExpType()->isEqual(CCString::create("C"))) {
////                type = 2;
////            }
////            else if (officer->getSoldierExpType()->isEqual(CCString::create("D"))) {
////                type = 3;
////            }
////        }
////        else if(str_->isEqual(CCString::create("SGEquipCard")))
////        {
////            SGEquipmentDataModel *officer = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
////            if (officer->getEquipExpType()->isEqual(CCString::create("A"))) {
////                type = 0;
////            }
////            else if (officer->getEquipExpType()->isEqual(CCString::create("B"))) {
////                type = 1;
////            }
////            else if (officer->getEquipExpType()->isEqual(CCString::create("C"))) {
////                type = 2;
////            }
////            else if (officer->getEquipExpType()->isEqual(CCString::create("D"))) {
////                type = 3;
////            }
////        }
////        
////        CCLOG("type is %d",type);
////        int currLvl = _card->getCurrLevel();
////        SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
////        totleExp += _card->getCurrExp();
////        while (totleExp >= num->getExpValue()) {
////            
////            totleExp -= num->getExpValue();
////            currLvl += 1;
////            num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
////        }
////        SGExpDataModel *maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
////        
////        float rate = 0;
////        float per = (float)_card->getCurrExp()/_card->getMaxExp();
////        
////        if (currLvl >_card->getCurrLevel())
////        {
////            rate = (float)totleExp/maxExp->getExpValue();
////            progress->setPercentage(rate*100);
////            progress->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(1, 0),CCFadeTo::create(1, 255),NULL)));
////        }else
////        {
////            rate = (float)totleExp/_card->getMaxExp();
////        }
////        
////        if (currLvl >_card->getMaxLevel()) {
////            currLvl = _card->getMaxLevel();
////            SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl+1);
////            SGExpDataModel *orinum = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, _card->getCurrLevel());
////            afterExp->setString(CCString::createWithFormat("%d",num->getexpAll() - orinum->getexpAll() - _card->getCurrExp())->getCString());
////            progress->setPercentage(100);
////        }else if (currLvl == _card->getCurrLevel() && cardCount != 0) {
////            progress->stopAllActions();
////            progress->setVisible(true);
////            progress->setColor(ccWHITE);
////            progress->setPercentage(per*100);
////            addprogress =  CCSprite::createWithSpriteFrameName("barrack_addexp.png");
////            addprogress -> setTextureRect(CCRectMake(progress->getContentSize().width*per, 0,progress->getContentSize().width*(rate-per), 10));
////            //        addprogress =  CCSprite::create("barrack/barrack_addexp.png",CCRectMake(progress->getContentSize().width*per, 0,progress->getContentSize().width*(rate-per), 10));
////            this->addChild(addprogress,10);
////            addprogress->setAnchorPoint(ccp(0, 0.5));
////            addprogress->setPosition(ccpAdd(progress->getPosition(), ccp(-progress->getContentSize().width/2 + progress->getContentSize().width*per, 0)));
////            addprogress->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(1, 0),CCFadeTo::create(1, 255),NULL)));
////        }
////        if (cardCount == 0)
////        {
////            progress->stopAllActions();
////            progress->setOpacity(255);
////            progress->setColor(ccWHITE);
////            currLvl = _card->getCurrLevel();
////            progress->setPercentage(per*100);
////        }
////        lab_orilvl->setop(0);
////        lab_lvl->labelstopact();
////        lab_lvl->setop(255);
////        lab_lvl->stopAllActions();
////        lab_lvl->setString(CCString::createWithFormat("Lv%d",currLvl)->getCString());
////        
////        if (currLvl> _card->getCurrLevel())
////        {
////            lab_lvl->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create((CCActionInstant*)lab_lvl->fadout(1),CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(SGAdvancechooseLayer::labelcallback)),CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(SGAdvancechooseLayer::labelcallback1)),CCDelayTime::create(1),(CCActionInstant*)lab_lvl->fadin(1),CCDelayTime::create(1),NULL)));
////        }
////        
////    }else
////    {
////        SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getGeneralSkillById(officer->getOfficerSkil());
////        
////        int rate = 0;
////        
////        int addexp = 0;
////        int len = _arraySelectCard->count();
////        for (int i = 0; i<len; i++)
////        {
////            SGPropsCard *card = (SGPropsCard *)_arraySelectCard->objectAtIndex(i);
////            addexp += card->getCurrStar()*card->getCurrStar()/8*20 + card->getCurrStar()*5 +10;
////            rate += (card->getCurrStar()+1)*0.5;
////            if (card->getItemId() == skill->getSkillItemId()) {
////                addexp += card->getCurrStar()*card->getCurrStar()/8*20 + card->getCurrStar()*5 +10;
////            }
////        }
////        
////        
////        int needexp = skill->getSkillStarLvl()*skill->getSkillMaxLevel()*((int)pow(skill->getSkillMaxLevel(), 2)/9 +1)*50;
////        
////        if (needexp <= (addexp + officer->getSkillExp())) {
////            rate += 62 + 2*_arraySelectCard->count();
////        }else
////        {
////            rate += 100*addexp/(needexp - officer->getSkillExp())*2/3;
////        }
////        if (rate>0) {
////            
////            if (rate>=80) {
////                rate = 80;
////            }
////            if (rate<=5) {
////                rate = 5;
////            }
////            skilluprate->setString(CCString::createWithFormat("%d%%",rate)->getCString());
////            skilluprate->setColor(ccc3(0x0c, 0xff, 0x00));
////        }else
////        {
////            skilluprate->setString("0%");
////            skilluprate->setColor(ccWHITE);
////            
////        }
////        needCoins = (skill->getSkillStarLvl() + skill->getSkillMaxLevel())*50*_arraySelectCard->count();
////        need->setString(CCString::createWithFormat("%d",needCoins)->getCString());
////    }
//  }
}

void SGAdvancechooseLayer::sortHandler()
{
    SGSortBox *Box = SGSortBox::create(this, this, datas,0,2);
    SGMainManager::shareMain()->showBox(Box);
}
//void SGAdvancechooseLayer::strengOfficerListener(CCObject *obj)
//{
//    SGOfficerCard* officerCard = NULL;
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    
//    if (sr)
//    {
//        main::OfficerCompositeResponse *response = (main::OfficerCompositeResponse *)sr->m_msg;
//        main::OfficerCardItemProto proto = response->officercarditemproto();
//        if (response->state() == 1)
//        {
//            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//            officerCard = new SGOfficerCard;
//            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
//            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
//            int type = 0;
//            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
//            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
//            
//            officerCard->setSsid(proto.ssid());
//            officerCard->setItemId(proto.itemid());
//            officerCard->setPositionId(proto.position());
//            officerCard->setCurrExp(proto.currexp());
//            officerCard->setLordTimes(proto.lordskilllevel());
//            officerCard->setIsProt(proto.isprotection());
//            //            officerCard->setIsExped(proto.isexpedition());
//            officerCard->setCurrLevel(proto.currlevel());
//            
//            if (temp->getOfficerNextId()) {
//                officerCard->setCost(temp->getOfficerCost());
//                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
//            }
//            officerCard->setExpRate(temp->getOfficerExpRate());
//            officerCard->setMaxStar(temp->getOfficerNextId());
//            officerCard->setMaxExp(expdata->getExpValue());
//            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
//            officerCard->setOfficerName(temp->getOfficerName());
//            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
//            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
//            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
//            officerCard->setRound(temp->getOfficerRound());
//            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
//            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
//            officerCard->setGoverning(temp->getOfficerGoverning());
//            officerCard->setRace(temp->getOfficerRace());
//            officerCard->setItemType(temp->getOfficerType());
//            //性别和缘分
//            officerCard->setGender((OfficerGender) temp->getOfficerGender());
//            officerCard->setFateList(temp->getOfficerFateList());
//            //武将原型ID
//            officerCard->setProtoId(temp->getOfficerProtoId());
//            //速度修正系数
//            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
//            
//            if (temp->getOfficerGeneralsSkill() &&temp->getOfficerGeneralsSkill()->length()>3)
//            {
//                std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
//                officerCard->setOfficerSkil(atoi(generalskill.c_str()));
//            }
//            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
//            {
//                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
//                officerCard->setLordSkill(atoi(lordskill.c_str()));
//            }
//            
//            int len = _arraySelectCard->count();
//            for (int i = 0; i < len; i++) {
//                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
//                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
//                if (str_->isEqual(CCString::create("SGPropsCard")))
//                {
//                    playerInfo->getPropCards()->removeObject(baseMilitaryCard);
//                }
//                
//                else if(str_->isEqual(CCString::create("SGOfficerCard")))
//                {
//                    SGOfficerCard *card = (SGOfficerCard *)baseMilitaryCard;
//                    if (card->getOfficerEquips())
//                    {
//                        for (int i = 0 ; i<card->getOfficerEquips()->count(); i++)
//                        {
//                            SGEquipCard *equip =(SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
//                            playerInfo->getEquipCard(equip->getSsid())->setOfficerCardId(0);
//                        }
//                    }
//                    playerInfo->getOfficerCards()->removeObject(baseMilitaryCard);
//                }
//            }
//            SGOfficerCard *before = (SGOfficerCard *)_card;
//            CCObject *object = NULL;
//            CCARRAY_FOREACH(before->getOfficerEquips(), object)
//            {
//                SGEquipCard *card = (SGEquipCard *)object;
//                officerCard->addEquip(card);
//            }
//            playerInfo->getOfficerCards()->removeObject(_card);
//            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - needCoins);
//            SGMainManager::shareMain()->updataUserMsg();
//            playerInfo->addOfficerCard(officerCard);
//            
//            CCDictionary *dict = CCDictionary::create();
//            CCString *str = NULL;
//            if (response->skillup() == 0) {
//                str= CCString::createWithFormat("增加经验%d",response->allexp());
//                
//            }else
//            {
//                SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(officerCard->getLordSkill());
//                CCString *lvl = CCString::create("");
//                int times = 0;
//                if (lordSkill->getSkillType() == 1) {
//                    lvl->setString("初级");
//                    times = 3;
//                }else if (lordSkill->getSkillType() == 2) {
//                    lvl->setString("中级");
//                    times = 4;
//                }else if (lordSkill->getSkillType() == 3) {
//                    lvl->setString("高级");
//                    times = 5;
//                }
//                
//                str= CCString::createWithFormat("增加经验%d ,%s%s升级",response->allexp(),lvl->getCString(),lordSkill->getSkillName()->getCString());
//            }
//            
//            
//            dict->setObject(CCString::createWithFormat("%d",response->upgradestate()), "upstate");
//            dict->setObject(str, "skillup");
//            
//            SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,10000,true,before,isfromfirst,dict);
//            officerCard->autorelease();
//            
//        }
//        else
//        {
//            SGMainManager::shareMain()->showGeneralInfoLayer(officer, 10001);
//           //SGMainManager::shareMain()->showMessage("强化武将失败");
//            
//        }
//    }
//}
//void SGAdvancechooseLayer::strengSkillListener(cocos2d::CCObject *obj)
//{
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    
//    if (sr)
//    {
//        int ori = ((SGOfficerCard*)_card)->getOfficerSkil();
//        main::SkillCompositeResponse *response = (main::SkillCompositeResponse *)sr->m_msg;
//        if (response->state() == 1)
//        {
//            ((SGOfficerCard*)_card)->setSkillExp(response->skillexp());
//            ((SGOfficerCard*)_card)->setOfficerSkil(response->skillid());
//            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//            int len = _arraySelectCard->count();
//            for (int i = 0; i < len; i++)
//            {
//                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
//                playerInfo->getPropCards()->removeObject(baseMilitaryCard);
//            }
//            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - needCoins);
//            SGMainManager::shareMain()->updataUserMsg();
//            if (isfromfirst) {
//                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 3,true,ori==response->skillid());
//            }else
//            {
//                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 4,true,ori==response->skillid());
//            }
//            
//        }
//        else
//        {
//            SGMainManager::shareMain()->showSkillCardInfoLayer(_card, 10);
////            SGMainManager::shareMain()->showMessage("失败");
//        }
//    }
//    
//}
//void SGAdvancechooseLayer::strengEquipListener(cocos2d::CCObject *obj)
//{
//    SGEquipCard* equipCard = NULL;
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    if (sr)
//    {
//        main::EquipmentCompositeResponse *response = (main::EquipmentCompositeResponse *)sr->m_msg;
//        main::EquipmentCardItemProto cardInfo = response->equipmentcarditemproto();
//        if (response->state() == 1)
//        {
//            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//            equipCard = new SGEquipCard;
//            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
//            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
//            int type = 0;
//            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
//            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
//            
//            SGEquipCard *equipCard = new SGEquipCard();
//            equipCard->setSsid(cardInfo.ssid());
//            //装备原型ID
//            equipCard->setProtoId(temp->getEquipProtoId());
//            equipCard->setItemId(cardInfo.itemid());
//            //            equipCard->setSetNum(cardInfo.setnum());
//            equipCard->setCurrLevel(cardInfo.currlevel());
//            equipCard->setCurrExp(cardInfo.currexp());
//            equipCard->setOfficerCardId(cardInfo.officercarditemid());
//            equipCard->setIsProt(cardInfo.isprotection());
//            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
//            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
//            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
//            equipCard->setMaxExp(expdata->getExpValue());
//            equipCard->setRound(temp->getEquipRound());
//            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
//            equipCard->setExpRate(temp->getEquipExpRate());
//            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
//            equipCard->setGoverning(temp->getEquipGoverning());
//            equipCard->setItemType(temp->getEquipType());
//            equipCard->setMaxStar(temp->getEquipNextId());
//            equipCard->setOfficerName(temp->getEquipName());
//            equipCard->setMaxLevel(temp->getEquipMaxLevel());
//            if (temp->getEquipNextId()) {
//                equipCard->setCost(temp->getEquipCost());
//                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
//            }
//            int len = _arraySelectCard->count();
//            for (int i = 0; i < len; i++) {
//                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
//                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
//                if (str_->isEqual(CCString::create("SGPropsCard")))
//                {
//                    playerInfo->getPropCards()->removeObject(baseMilitaryCard);
//                }
//                
//                else if(str_->isEqual(CCString::create("SGEquipCard")))
//                {
//                    playerInfo->getEquipCards()->removeObject(baseMilitaryCard);
//                }
//            }
//            if (equipCard->getOfficerCardId()) {
//                SGOfficerCard *card = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(equipCard->getOfficerCardId());
//                card->removeEquip((SGEquipCard*)_card);
//                card->addEquip(equipCard);
//            }
//            playerInfo->getEquipCards()->removeObject(_card);
//            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - needCoins);
//            SGMainManager::shareMain()->updataUserMsg();
//            playerInfo->addEquipCard(equipCard);
//            
//            
//            CCDictionary *dict = CCDictionary::create();
//            
//            CCString *str = CCString::createWithFormat("增加经验%d",response->allexp());
//            dict->setObject(CCString::createWithFormat("%d",response->upgradestate()), "upstate");
//            dict->setObject(str, "skillup");
//            
//            SGMainManager::shareMain()->showEquipInfoLayer(equipCard,10000,true,(SGEquipCard*)_card,isfromfirst,dict);
//            equipCard->autorelease();
//        }
//        else
//        {
//             SGMainManager::shareMain()->showEquipInfoLayer(equip,10001);
//           // SGMainManager::shareMain()->showMessage("失败");
//            
//        }
//    }
//    
//}
//void SGAdvancechooseLayer::strengSoldierListener(cocos2d::CCObject *obj)
//{
//    SGSoldierCard* soldierCard = NULL;
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    if (sr)
//    {
//        main::SoldierCompositeResponse *response = (main::SoldierCompositeResponse *)sr->m_msg;
//        main::SoldierCardItemProto proto = response->soldiercarditemproto();
//        if (response->state() == 1)
//        {
//            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//            soldierCard = new SGSoldierCard;
//            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
//            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
//            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
//            
//            soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
//            soldierCard->setSsid(proto.ssid());
//            soldierCard->setItemId(proto.itemid());
//            soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
//            soldierCard->setMaxExp(expdata->getExpValue());
//            soldierCard->setOfficerName(temp->getSoldierName());
//            soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
//            soldierCard->setRound(temp->getSoldierRound());
//            soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
//            soldierCard->setCurrLevel(proto.currlevel());
//            soldierCard->setCurrStar(temp->getSoldierStarLevel());
//            soldierCard->setCurrExp(proto.currexp());
//            
//            int len = _arraySelectCard->count();
//            for (int i = 0; i < len; i++)
//            {
//                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
//                
//                playerInfo->getPropCards()->removeObject(baseMilitaryCard);
//            }
//            
//            playerInfo->getSoldierCards()->removeObject(_card);
//            playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() - needCoins);
//            SGMainManager::shareMain()->updataUserMsg();
//            playerInfo->addSoldierCard(soldierCard);
//            
//            CCDictionary *dict = CCDictionary::create();
//            
//            CCString *str = CCString::createWithFormat("增加经验%d",response->allexp());
//            dict->setObject(CCString::createWithFormat("%d",response->upgradestate()), "upstate");
//            dict->setObject(str, "skillup");
//            
//            SGMainManager::shareMain()->showSoldierInfoLayer(soldierCard,10000,true,(SGSoldierCard*)_card,isfromfirst,dict);
//            soldierCard->autorelease();
//        }
//        else
//        {
//            SGMainManager::shareMain()->showSoldierInfoLayer(soldier,10001);
////            SGMainManager::shareMain()->showMessage("失败");
//        }
//    }
//}

//回调函数
void SGAdvancechooseLayer::sortLayerCallBack(CCArray *cardArray)
{
    this->datas = cardArray;
    _arraySelectCard->removeAllObjects();
    cardCount = 0;
    tableView->reloadData();
    selectposition = 0;
//    updateCoind();
//    for (int i = 0; i<5; i++) {
//        CCSprite *a = (CCSprite *)this->getChildByTag(i);
//        a->removeAllChildrenWithCleanup(true);
//        CCSprite *v = (CCSprite *)this->getChildByTag( 100+i);
//        v->setVisible(true);
//    }
    for (int i = 0; i<300; i++) {
        selectItem[i] = 0;
    }
    if (cardType == 1) {
        SGSortBox::updateBtn(range,3);
    }
    if (cardType == 2) {
        SGSortBox::updateBtn(range,4);
    }
}

void SGAdvancechooseLayer::confirmHandler()
{
    
    EFFECT_PLAY(MUSIC_BTN);
    if (cardType == 2 ||cardType == 3) {
        if (_card->getCurrExp() == _card->getMaxExp())
        {
            SGMainManager::shareMain()->showMessage(str_qianghua_tips1);
            return;
        }
    }
    if (cardCount>0 && needCoins < SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())
    {
        selectcardcount = _arraySelectCard->count()-1;
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(selectcardcount);
        CCSprite *a = (CCSprite*)this->getChildByTag(card->getselectposition());
        cardBlink(a);
        this->setIsCanTouch(false);
        fermEffect->stop();
        this->removeChild(fermEffect, true);
        this->removeChildByTag(77, true);
    }
    else if(cardCount == 0){
        SGMainManager::shareMain()->showMessage(str_one_card_need);
    }
    else if(needCoins > SGPlayerInfo::sharePlayerInfo()->getPlayerCoins()){
        SGCantAdvanceBox *noMoneyBox = SGCantAdvanceBox::create(this, _card,3,needCoins);
        SGMainManager::shareMain()->showBox(noMoneyBox);
    }
    
    
	//
	if (CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") <= guide_tag_17)
	{
		main::BeginnersGuideRequest *request = new main::BeginnersGuideRequest();
        request->set_id(guide_tag_17);
        SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_ASK, request, false);
		//CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_17);//点击强化确认后直接成功
		//CCUserDefault::sharedUserDefault()->flush();
    }
    //没有引导层，直接把引导相关的东西去掉
    if (SGGuideManager::shareManager()->getGuideLayer())
    {
        SGGuideManager::shareManager()->getGuideLayer()->setVisible(false);
        SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
    }
        

}
void SGAdvancechooseLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_beforecard &&_beforecard->count()!=0)
    {
        SGMainManager::shareMain()->showAdvanceLayer(_card,cardType,_currentPage,_beforecard);
    }
    else
    {
        SGMainManager::shareMain()->showAdvanceLayer(_card,cardType,_currentPage,NULL);
    }
    
    //每次经过这个页面都检测是否完成了19这个引导，不太科学
    //SGGuideManager::shareManager()->checkIsDone(guide_tag_19);
}
void SGAdvancechooseLayer::setIsCanTouch(bool isTouch)
{
    tableView->setIsTouchEnabled(isTouch);
    SGBaseLayer::setIsCanTouch(isTouch);
}
void SGAdvancechooseLayer::setIsLock(bool isLock)
{
    this->isLock = isLock;
}

void SGAdvancechooseLayer::updateposition(cocos2d::CCNode *node)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    node->setPosition(ccp(s.width, node->getPosition().y));
    fermEffect->setVisible(true);
    fermEffect->play();
}
void SGAdvancechooseLayer::stopjiantouAction()
{
    fermEffect->stop();
    fermEffect->setVisible(false);
}
void SGAdvancechooseLayer::cardBlink(cocos2d::CCNode *node)
{
    //素材卡闪烁
    CCSpriterX *texiao = CCSpriterX::create("animationFile/qhqr.scml", true, true);
    texiao->setanimaID(0);
    addChild(texiao, 105);
    texiao->setisloop(false);
    texiao->setPosition(node->getPosition());
    texiao->play();
    float dela = texiao->getAnimaTimeLength(0, 0);
    node->runAction(CCSequence::create(
                                       CCDelayTime::create((float)dela/4),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::nextcardBlindcall)),
                                       CCDelayTime::create((float)dela/4),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::cardBlinkMidcall)),
                                       CCDelayTime::create((float)dela/4),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::cardBlinkcall)),NULL));
}
void SGAdvancechooseLayer::cleanAnotherball()
{
    for (int i = 0; i<_arraySelectCard->count()-1; i++) {
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
        this->removeChildByTag(5000+card->getselectposition(), true);
        CCLOG("e23234234 %d",5000 + card->getselectposition());
    }
    CCScaleTo *scale  = CCScaleTo::create(0.1, 0.2);
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(_arraySelectCard->count()-1);
    CCSpriterX *ball = (CCSpriterX *)this->getChildByTag(5000+card->getselectposition());
    ball->setScale(1.23);
    ball->runAction(CCSequence::create(CCDelayTime::create(0.3),CCMoveBy::create(0.1, ccp(0, -30)),CCMoveTo::create(0.2, Icon->getPosition()),scale,CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::showblinkframe)),CCDelayTime::create(0.05),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::cleanballandrotate)),NULL));
}
void SGAdvancechooseLayer::showblinkframe()
{
    CCSprite *spriteBig = CCSprite::createWithSpriteFrameName("qhqr_7.png");
    this->addChild(spriteBig,105,789);
    spriteBig->setPosition(Icon->getPosition());
    
}
void SGAdvancechooseLayer::sendrequest()
{
    if (cardType != 4) {
        CCString *str_nowcard = CCString::create(_card->getClassName());
        if (str_nowcard->isEqual(CCString::create("SGOfficerCard")))
        {
            main::OfficerCompositeRequest *request = new main::OfficerCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
                
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    request->add_propsid(baseMilitaryCard->getSsid());}
                
                else if(str_->isEqual(CCString::create("SGOfficerCard")))
                {
                    request->add_materialid(baseMilitaryCard->getSsid());
                }
            }
            SGSocketClient::sharedSocketClient()->send(MSG_BARRACKS_STRENG, request);
            
        }
        else if (str_nowcard->isEqual(CCString::create("SGEquipCard")))
        {
            main::EquipmentCompositeRequest *request = new main::EquipmentCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
                if (str_->isEqual(CCString::create("SGPropsCard")))
                {
                    request->add_propsid(baseMilitaryCard->getSsid());}
                
                else if(str_->isEqual(CCString::create("SGEquipCard")))
                {
                    request->add_materialid(baseMilitaryCard->getSsid());
                }
                
            }
            SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_STRENG, request);
        }else{
            
            main::SoldierCompositeRequest *request = new main::SoldierCompositeRequest();
            request->set_ssid(_card->getSsid());
            for (int i = 0; i < _arraySelectCard->count(); i++)
            {
                SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
                request->add_propsid(baseMilitaryCard->getSsid());
            }
            SGSocketClient::sharedSocketClient()->send(MSG_SOLDIER_STRENG, request);
        }
    }else
    {
        main::SkillCompositeRequest *request = new main::SkillCompositeRequest();
        request->set_ssid(_card->getSsid());
        for (int i = 0; i < _arraySelectCard->count(); i++)
        {
            SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(i);
            request->add_propsid(baseMilitaryCard->getSsid());
        }
        SGSocketClient::sharedSocketClient()->send(MSG_STRENG_SKILL, request);
    }
    
}
void SGAdvancechooseLayer::cleanballandrotate(cocos2d::CCNode *node)
{
    this->removeChild(node, true);
    this->removeChildByTag(789, true);
    CCSpriterX *texiao2 = CCSpriterX::create("animationFile/qhqr_001.scml", true, true);
    texiao2->setanimaID(0);
    texiao2->setisloop(false);
    addChild(texiao2, 10);
    texiao2->setPosition(Icon->getPosition());
    texiao2->play();
    this->runAction(CCSequence::create(CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::sendrequest)),NULL));
}
void SGAdvancechooseLayer::nextcardBlindcall(cocos2d::CCNode *node)
{
    selectcardcount -- ;
    CCLOG("wegdasdf");
    if (selectcardcount >= 0){
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arraySelectCard->objectAtIndex(selectcardcount);
        CCSprite *kuang = (CCSprite*)this->getChildByTag(card->getselectposition());
        cardBlink(kuang);
    }
}
void SGAdvancechooseLayer::cardBlinkcall(cocos2d::CCNode *node)
{
    //光球
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSpriterX *texiao1 = CCSpriterX::create("animationFile/qhqr_000.scml", true, true);
    texiao1->setanimaID(0);
    texiao1->setisloop(true);
    addChild(texiao1,106,5000+node->getTag());
    CCLOG("texiao tag is %d",5000+node->getTag());
    texiao1->setPosition(node->getPosition());
    texiao1->play();
    texiao1->runAction(CCMoveTo::create(0.5, ccp(s.width/2-s.width*.377-3, node->getPosition().y)));
    
    if (selectcardcount == 0) {
        runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::cleanAnotherball)),NULL));
    }
    if (selectcardcount == 1 || _arraySelectCard->count() == 2) {
        runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::cleanAnotherball)),NULL));
    }
    if (selectcardcount == -1 || _arraySelectCard->count() == 1) {
        runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFuncN::create(this, callfuncN_selector(SGAdvancechooseLayer::cleanAnotherball)),NULL));
    }
}
void SGAdvancechooseLayer::cardBlinkMidcall(CCNode *node)
{
    this->getChildByTag(node->getTag() +100)->setVisible(true);
    node->removeAllChildrenWithCleanup(true);
}
#pragma mark - Tabel_Item1 Classr

Tabel_Item1::Tabel_Item1()
{
    
}

Tabel_Item1::~Tabel_Item1()
{
    arrayStar->release();
    bluestar->release();
}

void Tabel_Item1::setView(int num)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    SGMenu *menu1 = SGMenu::create(CCRectMake(0, 50, s.width, s.height - 180 - 284 - 50));
    menu1->setTouchPriority(1);
    menu1->setTag(101);
    menu1->setPosition(CCPointZero);
    this->addChild(menu1,5);
    
    arrayStar = CCArray::create();
    arrayStar->retain();
    bluestar = CCArray::create();
    bluestar->retain();
    item =  CCSprite::createWithSpriteFrameName("item_smallbg.png");
    bgbgbg = item;
    this->addChild(item);
    
    SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_advancechooselayer);
	
	spritePlayerHead = SGButton::create(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString(), "jinkuang.png", this, menu_selector(Tabel_Item1::showgenerinfo),ccp(0, 0),false,true);//12.05
    item->setPosition(ccpAdd(item->getPosition(), ccp(spritePlayerHead->getContentSize().width/2, 0)));
    menu1->addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width/2 - spritePlayerHead->getContentSize().width/2, 2)));
    spritePlayerHead->setEnabled(false);
    CCSprite *background =  CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background,-1);
    background->setPosition(spritePlayerHead->getPosition());
    
    labelName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_BOXINFO, 24);
    
    labelName->setAnchorPoint(ccp(0, 0.5));
    labelName->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6, labelName->getContentSize().height/2+3)));
    this->addChild(labelName);
    
    spriteIsScelected =  CCSprite::createWithSpriteFrameName("baikuang.png");
    this->addChild(spriteIsScelected);
    spriteIsScelected->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.359,0)));
    this->setItemSelect(false);
    
    labelLevel = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
    this->addChild(labelLevel);
    labelLevel->setAnchorPoint(ccp(0, 0.5));
    labelLevel->setPosition(ccpAdd(labelName->getPosition(), ccp(labelName->getContentSize().width + 10, 0)));
    if (_card->getCurrLevel()) {
        labelLevel->setString(CCString::createWithFormat("%d级",_card->getCurrLevel())->getCString());
    }
    
    CCString *str_ = CCString::create(_card->getClassName());
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
        if (spritePlayerHead)
        {
            spritePlayerHead->removeChildByTag(30, true);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spritePlayerHead->getContentSize().width*0.3-2,spritePlayerHead->getContentSize().height - countryName->getContentSize().height/4 +6));
            spritePlayerHead->addChild(countryName,30,30);
            
            spritePlayerHead->removeChildByTag(250, true);
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(spritePlayerHead->getContentSize().width*1,spritePlayerHead->getContentSize().height - advancenumber->getContentSize().height*.1));
                spritePlayerHead->addChild(advancenumber,50,250);
            }

        }
    }else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
        
        if (temp->getAdvNum()>0) {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
            advancenumber->setInsideColor(ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->ignoreAnchorPointForPosition(false);
            advancenumber->setPosition(ccp(spritePlayerHead->getContentSize().width*1,spritePlayerHead->getContentSize().height - advancenumber->getContentSize().height*.1));
            spritePlayerHead->addChild(advancenumber,50,250);
        }

    }else
    {
        SGPropsCard *card = (SGPropsCard *)_card;
        if (card->getType() == 1) {
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",temp->getPropsCountry())->getCString())->displayFrame());
        }else
        {
            //spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName("yinkuang.png")->displayFrame());
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png")->displayFrame());
        }
    }
    this->addBtn(spritePlayerHead);
    spritePlayerHead->setTag(11);
    this->showPlayerStar();
}

//设置星星数量
void Tabel_Item1::showPlayerStar()
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

    int starCount = _card->getCurrStar();
    for(int i=0; i < 6;i++)
    {
        CCSprite *sprite =  CCSprite::createWithSpriteFrameName("xx.png");
        sprite->setAnchorPoint(ccp(0.5, 1));
        sprite->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width+sprite->getContentSize().width*i-17,4)));
        arrayStar->addObject(sprite);
        this->addChild(sprite);
        if (i >= starCount) {
            sprite->setVisible(false);
        }
    }
    
    for(int i=starCount; i < _card->getUpgradestar();i++)
    {
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("bluestar.png");
        sprite->setAnchorPoint(ccp(0.5, 1));
        sprite->setPosition(ccpAdd(((CCSprite *)arrayStar->objectAtIndex(i))->getPosition(), ccp(0,-3.5)));
        bluestar->addObject(sprite);
        this->addChild(sprite);
    }

}

Tabel_Item1* Tabel_Item1::create(SGBaseMilitaryCard *card,int num)
{
	Tabel_Item1 *instance = new Tabel_Item1();
    instance->_card = card;
	if (instance->init())
    {
        instance->setView(num);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}
void Tabel_Item1::setIsCanSelect(bool isCanSelecdt)
{
    item->setColor(ccWHITE);
    if (!isCanSelecdt)
    {
        item->setColor(ccc3(80, 80, 80));
    }
}
//设置选中或非选中
void Tabel_Item1::setItemSelect(bool isSelect)
{
    if(isSelect)
    {
        CCSprite *spriteSelectOK =  CCSprite::createWithSpriteFrameName("streng_choose.png");
        spriteIsScelected->setDisplayFrame(spriteSelectOK->displayFrame());
    }else{
        CCSprite *spriteSelectNO =  CCSprite::createWithSpriteFrameName("baikuang.png");
        spriteIsScelected->setDisplayFrame(spriteSelectNO->displayFrame());
    }
}

//刷新
void Tabel_Item1::updateOfficerCard(SGBaseMilitaryCard *card,int index, int isSelect)
{
    _card = card;
    labelName->setString(card->getOfficerName()->getCString());
    CCString *str_ = CCString::create(_card->getClassName());
    SGMainManager::shareMain()->addHeadIconPlistByNum(card->getHeadSuffixNum(), sg_advancechooselayer);
//    spritePlayerHead->setImage(CCString::createWithFormat("head%d.png",card->getItemId())->getCString());
	spritePlayerHead->setImage(CCString::createWithFormat("head%d.png",card->getHeadSuffixNum())->getCString());//12.05
    
    labelLevel->setString("");
    if (card->getCurrLevel()) {
        labelLevel->setString(CCString::createWithFormat("%d级",card->getCurrLevel())->getCString());
        labelLevel->setPosition(ccpAdd(labelName->getPosition(), ccp(labelName->getContentSize().width + 10, 0)));
    }
    
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
//        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString())->displayFrame());
        if (spritePlayerHead)
        {
            spritePlayerHead->removeChildByTag(30, true);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spritePlayerHead->getContentSize().width*0.3-2,spritePlayerHead->getContentSize().height - countryName->getContentSize().height/4+6));
            spritePlayerHead->addChild(countryName,30,30);
            
            spritePlayerHead->removeChildByTag(250, true);
            if (temp->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(spritePlayerHead->getContentSize().width*1,spritePlayerHead->getContentSize().height - advancenumber->getContentSize().height*.1));
                spritePlayerHead->addChild(advancenumber,50,250);
            }

            
        }
        if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,(SGOfficerCard*)_card)) {
            SGCCLabelTTF *labisteam = SGCCLabelTTF::create(str_is_zhengying1,FONT_BOXINFO,24 , ccRED);
            //modify by:zyc. merge into create.
            //labisteam->setColor(ccRED);
            labisteam->setPosition(ccpAdd(labelLevel->getPosition(), ccp(labisteam->getContentSize().width*1,0)));
            this->addChild(labisteam);
//            this->setIsCanSelect(false);
        }
        else if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,(SGOfficerCard*)_card)) {
            SGCCLabelTTF *labisteam = SGCCLabelTTF::create(str_is_zhengying2,FONT_BOXINFO,24 , ccRED);
            //modify by:zyc. merge into create.
            //labisteam->setColor(ccRED);
            labisteam->setPosition(ccpAdd(labelLevel->getPosition(), ccp(labisteam->getContentSize().width*1,0)));
            this->addChild(labisteam);
//            this->setIsCanSelect(false);
        }
        else if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,(SGOfficerCard*)_card)) {
            SGCCLabelTTF *labisteam = SGCCLabelTTF::create(str_is_zhengying3,FONT_BOXINFO,24 , ccRED);
            //modify by:zyc. merge into create.
            //labisteam->setColor(ccRED);
            labisteam->setPosition(ccpAdd(labelLevel->getPosition(), ccp(labisteam->getContentSize().width*1,0)));
            this->addChild(labisteam);
//            this->setIsCanSelect(false);
        }
        else if (((SGOfficerCard *)_card)->getOfficerEquips() && ((SGOfficerCard *)_card)->getOfficerEquips()->count()!=0)
        {
            SGCCLabelTTF *labisteam = SGCCLabelTTF::create(str_have_zhuangbei,FONT_BOXINFO,24  , ccYELLOW);
            //modify by:zyc. merge into create.
            //labisteam->setColor(ccYELLOW);
            labisteam->setPosition(ccpAdd(labelLevel->getPosition(), ccp(labisteam->getContentSize().width*1,0)));
            this->addChild(labisteam);
        }
        

    }else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString())->displayFrame());
        
        spritePlayerHead->removeChildByTag(250, true);
        if (temp->getAdvNum()>0) {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
            advancenumber->setInsideColor(ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->ignoreAnchorPointForPosition(false);
            advancenumber->setPosition(ccp(spritePlayerHead->getContentSize().width*1,spritePlayerHead->getContentSize().height - advancenumber->getContentSize().height*.1));
            spritePlayerHead->addChild(advancenumber,50,250);
        }

        if (((SGEquipCard *)_card)->getOfficerCardId()!=0) {
//            CCString *str = CCString::createWithFormat("被<%s武将>穿戴",(_card->getOfficerName()));
//            SGCCLabelTTF *labisteam = SGCCLabelTTF::create(str->getCString(),FONT_BOXINFO,24);
//            labisteam->setColor(ccYELLOW);
//            labisteam->setPosition(ccpAdd(labelLevel->getPosition(), ccp(labisteam->getContentSize().width*1,0)));
//            this->addChild(labisteam);
        }
    }else
    {
        SGPropsCard *card = (SGPropsCard *)_card;
        if (card->getType() == 1) {
            
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",temp->getPropsCountry())->getCString())->displayFrame());
        }
        else if (card->getType() == 4) {
            
            //spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName("jinkuang.png")->displayFrame());
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png")->displayFrame());
        }
        else
        {
            //spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName("yinkuang.png")->displayFrame());
            spritePlayerHead->setFontImage( CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png")->displayFrame());
        }
    }
    this->setItemSelect(isSelect != 0);
    showPlayerStar();
}
void Tabel_Item1::showgenerinfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCString *str_ = CCString::create(_card->getClassName());
    if (str_->isEqual(CCString::create("SGPropsCard")))
    {
        if (((SGPropsCard *)_card)->getType() ==4) {
            SGMainManager::shareMain()->showSkillCardInfoLayer((SGPropsCard *)_card, 1,false);
        }else
        {
            SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)_card, 5,false);
        }
    }
    else if(str_->isEqual(CCString::create("SGOfficerCard")) )
    {
        SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create((SGOfficerCard *)_card, 21);
        SGMainManager::shareMain()->showLayer(Layer,false);
        
    }
    else if(str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 6,false);
    }
}
void SGAdvancechooseLayer::labelcallback()
{
    lab_lvl->setop(0);
    runAction(lab_orilvl->fadin(1));
}
void SGAdvancechooseLayer::labelcallback1()
{
    runAction(lab_orilvl->fadout(1));
    
}
void SGAdvancechooseLayer::visitHandle()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_beforecard &&_beforecard->count()!=0) {
        SGMainManager::shareMain()->showAdvanceLayer(_card,cardType,_currentPage,_beforecard);
    }
    else
    {
        SGMainManager::shareMain()->showAdvanceLayer(_card,cardType,_currentPage,NULL);
    }


}
