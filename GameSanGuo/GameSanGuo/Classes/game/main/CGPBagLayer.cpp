//
//  CGPBagLayer.cpp
//  GameSanGuo
//
//  Created by caoguoping on 15/12/10.
//
//

#include "CGPBagLayer.h"
#include "SGMainManager.h"
#include "SGStrengSoldierLayer.h"
#include "SGExpDataModel.h"
#include "SGStaticDataManager.h"


#define JUNYING_INFO_ENTER 10
#define HOME_INFO_ENTER 11
#define EMBATTLE_INFO_ENTER 12




CGPBagLayer::CGPBagLayer():
mBagEquips(NULL),
mBagMaterials(NULL)
{
//    panelUpdate = NULL;
//    mBarSoul = NULL;
//    mBarFragments = NULL;
//    for (int i = 0 ; i < E_updateBtnMax; i ++)
//    {
//        mBtnUpdate[i] = NULL;
//    }
//    for (int i = 0; i < E_updateBtnMax; i ++)
//    {
//        mPanel_update[i] = NULL;
//    }
//    expCom = false;
//    needUpdate = false;
//    pieceNeedNum = 1;
//    pieceHasNum = 0;
    
}

CGPBagLayer::~CGPBagLayer()
{
   // CC_SAFE_DELETE(mBagEquips);
   // CC_SAFE_DELETE(mBagMaterials);
}

CGPBagLayer*  CGPBagLayer::create(SGBaseMilitaryCard *card, int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage)
{
    CGPBagLayer *Layer = new CGPBagLayer();
    if (Layer && Layer->init(card  , type,  isOnlyOfficer))
    {
        Layer->initView();
        Layer->autorelease();
//        Layer->EmbattleInfo = info;//是否从布阵界面进入
//        Layer->isfromfirst = enter;
//        Layer->onlyOfficer = isOnlyOfficer;
//        Layer->_currentPage = currentPage;
//        Layer->cardType = type;
        
        
        
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

bool CGPBagLayer::init(SGBaseMilitaryCard *card, int type,bool isOnlyOfficer)
{
    if (!SGBaseLayer::init(NULL, cgp_bagLayer))
    {
        return false;
    }
// //   SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    //notification->addObserver(MSG_BARRACKS_STRENG,   this, callfuncO_selector(SGStrengLayer::strengOfficerListener));
//
//    _card = card;
//    
//    //SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
//    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
//    
//    _card->retain();
    return true;
    
}

void CGPBagLayer::btnCallItemSelect(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        int tag = static_cast<ImageView*>(pSender)->getTag();
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat("你选择了第 %d 个物品", tag + 1)->getCString());
    }
}

void CGPBagLayer::btnCallTabSwitch(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        Button*  btnNow = static_cast<Button*>(pSender);
        E_bagTag  btnTag = E_bagTag(btnNow->getTag());
        if (btnTag == mNowTabTag)
        {
            return;
        }
        
        for (int i = 0; i < E_bagTagMax; i ++)
        {
            mTabBtns[i]->setBrightStyle(BRIGHT_HIGHLIGHT);    //按下的状态
        }
        
        mNowTabTag = btnTag;
        mTabBtns[mNowTabTag]->setBrightStyle(BRIGHT_NORMAL);
        
        switch (btnTag)
        {
            case E_equip:
                showBagEquips();
                break;
                
            case E_prop:
                showBagProps();
            break;
            
            case E_material:
                showBagMaterial();
            break;
                
            case E_fragments:
                showBagFragments();
                break;
                
            default:
                break;
        }

    }
}

void CGPBagLayer::showBagEquips()
{
    mBagEquips = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
    int whichItem = 0;
    int nlists = ceil(mBagEquips->count()/4.0);
    mListBag->removeAllItems();
    for (int i = 0; i < nlists; i++)
    {
        Layout* oneList = Layout::create();
        oneList->setSize(CCSize(530, 133));
        
        for(int j = 0; j < 4; j++)
        {
            whichItem = i * 4 + j;
            ImageView* oneBag = ImageView::create();
            oneBag->loadTexture("bagGrid.png", UI_TEX_TYPE_PLIST);
            oneBag->setAnchorPoint(ccp(0, 0));
            oneBag->setPositionX( j * 133);
            oneList->addChild(oneBag);
            
            if (whichItem < mBagEquips->count())
            {
                ImageView* oneItem = ImageView::create();
                SGEquipCard* oneCard = (SGEquipCard*)mBagEquips->objectAtIndex(whichItem);
                int iconId = SGStaticDataManager::shareStatic()->getEquipById(oneCard->getItemId())->getIconId();
                oneItem->loadTexture(CCString::createWithFormat("head%d.png", iconId)->getCString() , UI_TEX_TYPE_PLIST);
                oneItem->setPosition(ccp(67, 67));
                oneItem->setTag(whichItem);
                oneItem->setTouchEnabled(true);
                oneItem->setScale(120.0/94.0);
                oneItem->addTouchEventListener(this, toucheventselector(CGPBagLayer::btnCallItemSelect));
                oneBag->addChild(oneItem);
            }
        }
        mListBag->pushBackCustomItem(oneList);
    }
    

}

void CGPBagLayer::showBagProps()
{
    mBagProps = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    int whichItem = 0;
    int nlists = ceil(mBagProps->count()/4.0);
    mListBag->removeAllItems();
    for (int i = 0; i < nlists; i++)
    {
        Layout* oneList = Layout::create();
        oneList->setSize(CCSize(530, 133));
        
        for(int j = 0; j < 4; j++)
        {
            whichItem = i * 4 + j;
            ImageView* oneBag = ImageView::create();
            oneBag->loadTexture("bagGrid.png", UI_TEX_TYPE_PLIST);
            oneBag->setAnchorPoint(ccp(0, 0));
            oneBag->setPositionX( j * 133);
            oneList->addChild(oneBag);
            
            if (whichItem < mBagProps->count())
            {
                ImageView* oneItem = ImageView::create();
                SGPropsCard* oneCard = (SGPropsCard*)mBagProps->objectAtIndex(whichItem);
                int iconId = SGStaticDataManager::shareStatic()->getPropsById(oneCard->getItemId())->getIconId();
                oneItem->loadTexture(CCString::createWithFormat("head%d.png", iconId)->getCString() , UI_TEX_TYPE_PLIST);
                oneItem->setPosition(ccp(67, 67));
                oneItem->setTag(whichItem);
                oneItem->setTouchEnabled(true);
                oneItem->setScale(120.0/94.0);
                oneItem->addTouchEventListener(this, toucheventselector(CGPBagLayer::btnCallItemSelect));
                oneBag->addChild(oneItem);
            }
        }
        mListBag->pushBackCustomItem(oneList);
    }
    
    
}


void CGPBagLayer::showBagMaterial()
{
    mBagMaterials = SGPlayerInfo::sharePlayerInfo()->getMaterialCards();
    int whichItem = 0;
    int nlists = ceil(mBagMaterials->count()/4.0);
    mListBag->removeAllItems();
    for (int i = 0; i < nlists; i++)
    {
        Layout* oneList = Layout::create();
        oneList->setSize(CCSize(530, 133));
        
        for(int j = 0; j < 4; j++)
        {
            whichItem = i * 4 + j;
            ImageView* oneBag = ImageView::create();
            oneBag->loadTexture("bagGrid.png", UI_TEX_TYPE_PLIST);
            oneBag->setAnchorPoint(ccp(0, 0));
            oneBag->setPositionX( j * 133);
            oneList->addChild(oneBag);
            
            if (whichItem < mBagMaterials->count())
            {
                ImageView* oneItem = ImageView::create();
                SGMaterialCard* oneCard = (SGMaterialCard*)mBagMaterials->objectAtIndex(whichItem);
                int iconId = SGStaticDataManager::shareStatic()->getMaterialById(oneCard->getItemId())->getMaterialIconId();
                oneItem->loadTexture(CCString::createWithFormat("head%d.png", iconId)->getCString() , UI_TEX_TYPE_PLIST);
                oneItem->setPosition(ccp(67, 67));
                oneItem->setTag(whichItem);
                oneItem->setTouchEnabled(true);
                oneItem->setScale(120.0/94.0);
                oneItem->addTouchEventListener(this, toucheventselector(CGPBagLayer::btnCallItemSelect));
                oneBag->addChild(oneItem);
            }
        }
        mListBag->pushBackCustomItem(oneList);
    }
    

}

void CGPBagLayer::showBagFragments()
{
    mListBag->removeAllItems();
}



void CGPBagLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("headIcon/head4001-4100.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head4101-4200.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head1-100.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head101-200.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head201-300.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head301-400.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head401-500.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head501-600.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head601-700.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head701-800.plist", RES_TYPE_LAYER_UI, cgp_bagLayer, LIM_PNG_AS_PNG);
    

    
    setIsCocostudio(true);
    panelBag  = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("cocosUi/bag.ExportJson"));
    SGMainManager::shareMain()->getMainScene()->mCocosLayers[cgp_bagLayer - cocostudioLayerStart] = panelBag;

    
    this->setTag(cgp_bagLayer);
    SGMainManager::shareMain()->getTouchGroup()->addWidget(panelBag);
       
    panelBag->setPosition(ccp(0, 120));
    
    UIGet_ListView("ListView_item", panelBag, mListBag)
    
    UIGet_Button("Button_3", panelBag, mTabBtns[0])
    UIGet_Button("Button_3_0", panelBag, mTabBtns[1])
    UIGet_Button("Button_3_1", panelBag, mTabBtns[2])
    UIGet_Button("Button_3_2", panelBag, mTabBtns[3])
    
    for (int i = 0 ; i < E_bagTagMax; i ++)
    {
        mTabBtns[i]->setBrightStyle(BRIGHT_HIGHLIGHT);
        mTabBtns[i]->setTag(i);
        UIClick(mTabBtns[i], CGPBagLayer::btnCallTabSwitch)
    }
    mTabBtns[0]->setBrightStyle(BRIGHT_NORMAL);
    
    mTabNames[0] = SGCCLabelTTF::create("装  备", FONT_PANGWA, 28, ccShitYellow);
    mTabBtns[0]->addNode(mTabNames[0]);
    mTabNames[0]->setPosition(ccp(3, -3));
    
    mTabNames[1] = SGCCLabelTTF::create("道  具", FONT_PANGWA, 28, ccShitYellow);
    mTabBtns[1]->addNode(mTabNames[1]);
    mTabNames[1]->setPosition(ccp(3, -3));
    
    mTabNames[2] = SGCCLabelTTF::create("材  料", FONT_PANGWA, 28, ccShitYellow);
    mTabBtns[2]->addNode(mTabNames[2]);
    mTabNames[2]->setPosition(ccp(3, -3));
    
    mTabNames[3] = SGCCLabelTTF::create("碎  片", FONT_PANGWA, 28, ccShitYellow);
    mTabBtns[3]->addNode(mTabNames[3]);
    mTabNames[3]->setPosition(ccp(3, -3));
    showBagEquips();
}






