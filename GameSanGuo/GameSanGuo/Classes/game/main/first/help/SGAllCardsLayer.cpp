//
//  SGAllCardsLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-11.
//
//

#include "SGAllCardsLayer.h"
#include "SGMainManager.h"
#include "SGBarracksLayer.h"
#include "SGPlayerInfo.h"
#include "SGHaveCards.h"
#include "SGStaticDataManager.h"
#include "SGCCLabelTTF.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGAllCardsLayer::SGAllCardsLayer()
:diction(NULL)
,weiOfficerCount(0)
,shuOfficerCount(0)
,wuOfficerCount(0)
,qunOfficerCount(0)
, equipCount(0)
{
    diction = NULL;
    weiOfficerCount = 0;
    shuOfficerCount = 0;
    wuOfficerCount = 0;
    qunOfficerCount = 0;
    equipCount  = 0;
    
    weidatas = NULL;
    shudatas = NULL;
    wudatas = NULL;
    qundatas = NULL;
    equipdatas = NULL;
}
SGAllCardsLayer::~SGAllCardsLayer()
{
    CC_SAFE_RELEASE(diction);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_allCardsLayer);
    CC_SAFE_RELEASE(weidatas);
    CC_SAFE_RELEASE(shudatas);
    CC_SAFE_RELEASE(wudatas);
    CC_SAFE_RELEASE(qundatas);
    CC_SAFE_RELEASE(equipdatas);
}
SGAllCardsLayer *SGAllCardsLayer::create(CCArray *dict)
{
    SGAllCardsLayer *cardsLayer = new SGAllCardsLayer();
    if (cardsLayer && cardsLayer->init(NULL, sg_allCardsLayer))
    {
        cardsLayer->initView(dict);
        cardsLayer->autorelease();
        return cardsLayer;
    }
    CC_SAFE_DELETE(cardsLayer);
    return NULL;
}
void SGAllCardsLayer::initView(CCArray *dict)
{
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_allCardsLayer);
    
    weidatas = CCArray::create();
    weidatas->retain();
    
    shudatas = CCArray::create();
    shudatas->retain();
    
    wudatas = CCArray::create();
    wudatas->retain();
    
    qundatas = CCArray::create();
    qundatas->retain();
    
    equipdatas = CCArray::create();
    equipdatas->retain();
    
    diction = dict;
    diction->retain();
    
    weiOfficerCount = 0;
    shuOfficerCount = 0;
    wuOfficerCount = 0;
    qunOfficerCount = 0;
    for (int i = 0; i<diction->count(); i++)
    {
        SGHaveCards *card = (SGHaveCards*)diction->objectAtIndex(i);
        
        int item = card->getcardItem();
        if (item >= 1001 &&item<=2999) {
            SGOfficerDataModel *oCard = SGStaticDataManager::shareStatic()->getOfficerById(item);
            switch (oCard->getOfficerRace()) {
                case 1:
                {
                    //蜀国
                    shuOfficerCount++;
                    shudatas->addObject(card);
                }
                    break;
                case 2:
                {
                    //魏国
                    weiOfficerCount++;
                    weidatas->addObject(card);
                }
                    break;
                case 3:
                {
                    //吴国
                    wuOfficerCount++;
                    wudatas->addObject(card);
                }
                    break;
                case 4:
                {
                    //群英
                    qunOfficerCount++;
                    qundatas->addObject(card);
                }
                    break;
                default:
                    break;
            }
        }
        if (item>= 4000 &&item<=5999) {
            equipCount++;
            equipdatas->addObject(card);
        }
    }
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    tableViewHeight = skewing(55);
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
    
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
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGAllCardsLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);

    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_AllCardsLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);
    
}

SNSTableViewCellItem *SGAllCardsLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    CCSize size = tableView->getItemSize();
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    LabelItem *item = (LabelItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if (NULL == item)
    {
        item = LabelItem::create(size,num,false,this,tableView);
    }
    SGCCLabelTTF *label = SGCCLabelTTF::create("", FONT_PANGWA, FONTSIZE(18));
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.3, 0)));
    item->addChild(label);
        switch (num) {
            case 0:
            {               
                label->setString(str_AllCardsLayer_str2);
                CCDictionary * dic = SGStaticDataManager::shareStatic()->getWeiOfficerDict();
                int count = dic->allKeys()->count();
                CCString *str = CCString::createWithFormat("%d/%d",weiOfficerCount,count);
                
                CCLabelTTF *label = CCLabelTTF::create(str->getCString(), FONT_XINGKAI, 34);
                label->setPosition(ccpAdd(item->getPosition(), ccp(640*.3, 0)));
                item->addChild(label);
            }
                break;
            case 1:
            {
                label->setString(str_AllCardsLayer_str3);
                CCDictionary * dic = SGStaticDataManager::shareStatic()->getShuOfficerDict();
                int count = dic->allKeys()->count();

                CCString *str = CCString::createWithFormat("%d/%d",shuOfficerCount,count);
                CCLabelTTF *label = CCLabelTTF::create(str->getCString(), FONT_XINGKAI, 34);
                label->setPosition(ccpAdd(item->getPosition(), ccp(640*.3, 0)));
                item->addChild(label);
            }
                break;
            case 2:
            {
                label->setString(str_AllCardsLayer_str4);
                CCDictionary * dic = SGStaticDataManager::shareStatic()->getWuOfficerDict();
                int count = dic->allKeys()->count();
                CCString *str = CCString::createWithFormat("%d/%d",wuOfficerCount,count);
                
                CCLabelTTF *label = CCLabelTTF::create(str->getCString(), FONT_XINGKAI, 34);
                label->setPosition(ccpAdd(item->getPosition(), ccp(640*.3, 0)));
                item->addChild(label);
            }
                break;
            case 3:
            {
                label->setString(str_AllCardsLayer_str5);
                CCDictionary * dic = SGStaticDataManager::shareStatic()->getQunOfficerDict();
                int count = dic->allKeys()->count();
                CCString *str = CCString::createWithFormat("%d/%d",qunOfficerCount,count);
                
                CCLabelTTF *label = CCLabelTTF::create(str->getCString(), FONT_XINGKAI, 34);
                label->setPosition(ccpAdd(item->getPosition(), ccp(640*.3, 0)));
                item->addChild(label);
            }
                break;
            case 4:
            {

                label->setString(str_AllCardsLayer_str6);
                CCDictionary * dic = SGStaticDataManager::shareStatic()->getEquipViewDict();
                int count = dic->allKeys()->count();
                CCString *str = CCString::createWithFormat("%d/%d",equipCount,count);
                
                CCLabelTTF *label = CCLabelTTF::create(str->getCString(), FONT_XINGKAI, 34);
                label->setPosition(ccpAdd(item->getPosition(), ccp(640*.3, 0)));
                item->addChild(label);

            }
                break;
            default:
                break;
        
    }
    return item;
}
unsigned int  SGAllCardsLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{    
    return 5;
}
void SGAllCardsLayer::itemselect(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_28);
    CCArray *showarray = NULL;

    switch (indexPath->getRow()) {
        case 0:
        {
            //魏国
            showarray = weidatas;
        }
            break;
        case 1:
        {
            //蜀国
            showarray = shudatas;
        }
            break;
        case 2:
        {
            //吴国
            showarray = wudatas;
        }
            break;
        case 3:
        {
            //群英
            showarray = qundatas;
        }
            break;
        case 4:
        {
            //群英
            showarray = equipdatas;
        }
            break;
        default:
            break;
    }
    if (showarray) {
        
        SGMainManager::shareMain()->showCardsInfo(showarray,indexPath->getRow(),false);
    }
}

void SGAllCardsLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_28);
//
//    SGMainManager::shareMain()->showCardsInfo(datas,indexPath->getRow(),false);
}

void SGAllCardsLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showHelpLayer();
}