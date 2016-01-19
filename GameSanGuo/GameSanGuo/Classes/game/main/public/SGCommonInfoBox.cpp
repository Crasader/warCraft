//
//  SGCommonInfoBox.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 10/23/14.
//
//

#include "SGCommonInfoBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ArmyModule.pb.h"
#include "SGSocketClient.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGExchangeEquipBox.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGStringConfig.h"
#include "SGDrawCards.h"
#include "SGGuideManager.h"
#include "SGTools.h"
#include "SGMAllLayer.h"

//ctor
SGCommonInfoBox::SGCommonInfoBox() : _mainItem(NULL), _subItem(NULL), _opentype(SGCIBOT_PROTO_ITEM), _myEnterType(-1), _otherEnterType(-1), _extArg(""), _cibDelegate(NULL), _otherIsBeforeDel(false), _scData(NULL), _specmode(SGCSM_NULL)
{
    officer = NULL;
    equip = NULL;
    props = NULL;
    pieces = NULL;
    consumable = NULL;
    material = NULL;
}

//dtor
SGCommonInfoBox::~SGCommonInfoBox()
{
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->removeObserver(this, MSG_EQUIP_OFF);
    sgnc->removeObserver(this, MSG_EQUIP_WEAR);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_commonInfoBox);
    CC_SAFE_RELEASE(_mainItem);
    CC_SAFE_RELEASE(_subItem);
}

//create 完整版。boxDeg=窗口代理供关闭回调，main=主物品；sub=副物品；otype=本次BOX类型；myEnterType=本BOX的进入类型；otherEnterType=详情页面的进入类型，为了与以前直接进入全屏版详情页面的代码兼容；extArg=预留特殊参数，deg=预留本BOX代理。
SGCommonInfoBox* SGCommonInfoBox::create(SGBoxDelegate* boxDeg, SGBaseMilitaryCard* main, SGBaseMilitaryCard* sub, SGCibOpenType otype, int myEnterType, int otherEnterType, bool otherIsBeforeDel, std::string extArg, SGCommonInfoBoxDelegate* deg, main::SigninCell* scData, SGCibSpecialMode sm)
{
    if (!main)
        return NULL;
    SGCommonInfoBox *cib = new SGCommonInfoBox();
    
    if ( cib && cib->SGBaseBox::init( boxDeg, box_commonInfoBox, CCSizeMake(535, 634), true ) )
    {
        if (cib->deletage)
        {
            cib->deletage->setIsCanTouch(false);
        }
        cib->_specmode = sm;
        cib->_scData = scData;
        cib->_mainItem = main;
        cib->_subItem = sub;
        cib->_opentype = otype;
        cib->_myEnterType = myEnterType;
        cib->_otherEnterType = otherEnterType;
        cib->_otherIsBeforeDel = otherIsBeforeDel;
        cib->_extArg = extArg;
        cib->_cibDelegate = deg;
        CC_SAFE_RETAIN(cib->_mainItem);
        CC_SAFE_RETAIN(cib->_subItem);
        
        cib->initView();
        cib->autorelease();
        return cib;
    }
    CC_SAFE_RELEASE(cib);
    return NULL;
}

//@重要 原型级物品快捷调用窗口，参数请参考上方完整版。
SGCommonInfoBox* SGCommonInfoBox::create(SGBaseMilitaryCard* main, SGBoxDelegate* boxDeg, int otherEnterType, bool otherIsBeforeDel, int myEnterType, SGCibSpecialMode sm)
{
    return SGCommonInfoBox::create(boxDeg, main, NULL, SGCIBOT_PROTO_ITEM, myEnterType, otherEnterType, otherIsBeforeDel, "", NULL, NULL, sm);
}

//@特殊 月签奖励物品快捷调用窗口，参数请参考上方。
SGCommonInfoBox* SGCommonInfoBox::create(SGBoxDelegate* boxDeg, SGBaseMilitaryCard* main, SGCibOpenType otype, main::SigninCell* scData)
{
    return SGCommonInfoBox::create(boxDeg, main, NULL, otype, -1, -1, true, "", NULL, scData, SGCSM_NULL);
}

//initView
void SGCommonInfoBox::initView()
{
    //触控屏蔽
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    //menu增强
    menu->setTouchMode(kCCTouchesOneByOne);
    menu->setTouchEnabled(true);
    
    //判断监听器
    if (SGCIBOT_MY_EQUIP_WEAR == this->_opentype || SGCIBOT_MY_EQUIP_UNWEAR == this->_opentype)
    {
        SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
        sgnc->addObserver(MSG_EQUIP_OFF, this, callfuncO_selector(SGCommonInfoBox::unwearEquipListener));
        sgnc->addObserver(MSG_EQUIP_WEAR, this, callfuncO_selector(SGCommonInfoBox::wearEquipListener));

    }
    
    //预加载资源
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_commonInfoBox);
    rm->bindTexture("sgskillcardslayer/sgskillcardslayer.plist", RES_TYPE_LAYER_UI, sg_commonInfoBox);
    rm->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_commonInfoBox);
    rm->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_commonInfoBox);
    rm->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_commonInfoBox, LIM_PNG_AS_PNG);
    rm->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_commonInfoBox);
    
    
    fontbg->setPreferredSize(CCSize(480,465));
    fontbg->setPosition(ccpAdd(biao->getPosition(), ccp(0,- fontbg->getContentSize().height*.5f-40)));
    
    //前置变量众
    SGStaticDataManager* sdm = SGStaticDataManager::shareStatic();

    CCPoint zwAnchor = ccp(0, 0.5); //锚点置于左中
    int cttZorder = 10; //内容的ZD
    int frmZorder = 1; //9切框的ZD
    
    //三个九切框背景区>>>>
    const char* frmPath = "ofc_100x100.png";
    float frameWidth = 460;
    float topHeight = 114;
    float midHeight = 160;
    float btmHeight = 114;
    
    CCScale9Sprite* top9p = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    CCPoint top9pPos = SGLayout::getPoint(kMiddleCenter) + ccp(0, -150);
    top9p->setPosition(top9pPos);
    top9p->setPreferredSize(CCSize(frameWidth, 114));
    this->addChild(top9p, frmZorder);
    
    CCScale9Sprite* mid9p = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    CCPoint mid9pPos = top9pPos + ccp(0, topHeight * 0.5 + midHeight * 0.5 + 15);
    mid9p->setPosition(mid9pPos);
    mid9p->setPreferredSize(CCSize(frameWidth, 160));
    this->addChild(mid9p, frmZorder);
    
    CCScale9Sprite* btm9p = CCScale9Sprite::createWithSpriteFrameName(frmPath);
    CCPoint btm9pPos = mid9pPos + ccp(0, midHeight * 0.5 + btmHeight * 0.5 + 15);
    btm9p->setPosition(btm9pPos);
    btm9p->setPreferredSize(CCSize(frameWidth, 114));
    this->addChild(btm9p, frmZorder);
    
    //通用区>>>> 获得物品的基础信息
    const char* adModel = "+%d";
    const char* lvModel = "LV %d";
    
    std::string nameStr = "";
    std::string adStr = "";
    std::string levelStr = "";
    int quality = _mainItem->getCurrStar(); //默认绿色
    CCSprite* iconSprite;
    
    //XXX:判断是什么物品，这个必须进行归一区分。
    //如果是碎片/道具/材料/消耗品，levelStr不同处理，将会不显示level，而是把名称再显示一次，主白描黑。
    if (SGCIBOT_MY_EQUIP_WEAR == this->_opentype || SGCIBOT_MY_EQUIP_UNWEAR == this->_opentype)
    {
        equip = dynamic_cast<SGEquipCard*>(_mainItem);
        officer = dynamic_cast<SGOfficerCard*>(_subItem);
        
        nameStr = equip->getOfficerName()->m_sString;
        adStr = CCString::createWithFormat(adModel, equip->getAdNum())->m_sString;
        levelStr = CCString::createWithFormat(lvModel, equip->getCurrLevel())->m_sString;
//        iconSprite = SGDrawCards::drawallcards(equip->getItemId(), 7, sg_commonInfoBox);
        iconSprite = SGDrawCards::createNormalSprite(equip->getItemId(), 7, sg_commonInfoBox);
    }
    else if (SGCIBOT_OTHER_EQUIP == this->_opentype)
    {
        equip = dynamic_cast<SGEquipCard*>(_mainItem);
        
        nameStr = equip->getOfficerName()->m_sString;
        adStr = CCString::createWithFormat(adModel, equip->getAdNum())->m_sString;
        levelStr = CCString::createWithFormat(lvModel, equip->getCurrLevel())->m_sString;
//        iconSprite = SGDrawCards::drawallcards(equip->getItemId(), 7, sg_commonInfoBox);
        iconSprite = SGDrawCards::createNormalSprite(equip->getItemId(), 7, sg_commonInfoBox);
    }
    else //包括原型/月签普通/月签VIP这三种类型
    {
        equip = dynamic_cast<SGEquipCard*>(_mainItem);
        officer = dynamic_cast<SGOfficerCard*>(_mainItem);
        props = dynamic_cast<SGPropsCard*>(_mainItem);
        pieces = dynamic_cast<SGPiecesCard*>(_mainItem);
        consumable = dynamic_cast<SGConsumableCard*>(_mainItem);
        material = dynamic_cast<SGMaterialCard*>(_mainItem);
        
        int itemId = _mainItem->getItemId(); //可以信任
        int itemType = -1;
        int itemNum = _mainItem->getItemNum(); //给SGBaseMilitaryCard新增的特殊参数，可以信任
        
        if (equip)
        {
            nameStr = equip->getOfficerName()->m_sString;
            adStr = CCString::createWithFormat(adModel, equip->getAdNum())->m_sString;
            levelStr = CCString::createWithFormat(lvModel, equip->getCurrLevel())->m_sString;
//            iconSprite = SGDrawCards::drawallcards(equip->getItemId(), 7, sg_commonInfoBox);
            itemType = 7;
        }
        else if (officer)
        {
            nameStr = officer->getOfficerName()->m_sString;
            adStr = CCString::createWithFormat(adModel, officer->getAdNum())->m_sString;
            levelStr = CCString::createWithFormat(lvModel, officer->getCurrLevel())->m_sString;
//            iconSprite = SGDrawCards::drawallcards(officer->getItemId(), 6, sg_commonInfoBox);
            itemType = 6;
        }
        else if (props)
        {
            nameStr = props->getOfficerName()->m_sString;
            adStr = CCString::createWithFormat(adModel, props->getAdNum())->m_sString;
            levelStr = nameStr;
//            iconSprite = SGDrawCards::drawallcards(props->getItemId(), 9, sg_commonInfoBox);
            itemType = 9;
        }
        else if (pieces)
        {
            nameStr = pieces->getPiecesName()->m_sString;
            adStr = CCString::createWithFormat(adModel, pieces->getAdNum())->m_sString;
            levelStr = nameStr;
//            iconSprite = SGDrawCards::drawallcards(pieces->getItemId(), 10, sg_commonInfoBox);
            itemType = 10;
        }
        else if (consumable)
        {
            nameStr = consumable->getOfficerName()->m_sString;
            adStr = CCString::createWithFormat(adModel, consumable->getAdNum())->m_sString;
            levelStr = nameStr;
//            iconSprite = SGDrawCards::drawallcards(consumable->getItemId(), 15, sg_commonInfoBox);
            itemType = 15;
        }
        else if (material)
        {
            nameStr = material->getOfficerName()->m_sString;
            adStr = CCString::createWithFormat(adModel, material->getAdNum())->m_sString;
            levelStr = nameStr;
//            iconSprite = SGDrawCards::drawallcards(material->getItemId(), 17, sg_commonInfoBox);
            itemType = 17;
            quality = material->getMaterialBorderColor();
        }
        else
        {
            // shall not come here.
            assert(1 == 0);
        }
        
        iconSprite = SGDrawCards::createComplexSprite(itemId, itemType, itemNum, sg_commonInfoBox);
    }

//    ccColor3B titleColor;
//    if(quality <3 || quality>6)
//        titleColor = ccWHITE;
//    else
//        titleColor = color[quality-1];
    
    //nameStr (title)
    SGCCLabelTTF* title = SGCCLabelTTF::create(nameStr.c_str(), FONT_PANGWA, 40 , SGTools::getColorByQuality(quality) , ccBLACK, 3.0);
    title->setPosition( biao->getPosition() + ccp(0, -5) );
    this->addChild(title, cttZorder);

    //adStr
    //MMDBG: temp
    if (_mainItem->getAdNum() > 0)
//    if (true)
    {
//        adStr = "+3";
        SGCCLabelTTF* adLabel = SGCCLabelTTF::create(adStr.c_str(), FONT_PANGWA, 40, ccGREEN , ccc3(143, 87, 0));
        adLabel->setAnchorPoint(zwAnchor);
        adLabel->setPosition(title->getPosition() + ccp(title->getContentSize().width * 0.5, 0));
        this->addChild(adLabel, cttZorder);
    }
    
    //iconSprite
    iconSprite->setPosition(ccpAdd(biao->getPosition(),ccp(-iconSprite->getContentSize().width - iconSprite->getContentSize().width/2 - 12, -118)));
    this->addChild(iconSprite, cttZorder);
    
    //特殊情况处理。
    if (SGCIBOT_MS_VIP_ITEM != this->_opentype && SGCIBOT_MS_ORDINARY_ITEM != this->_opentype) //非月签界面，照旧。
    {
        //levelStr，如果是碎片/道具/材料/消耗品，则不显示level，而是把名称再显示一次，主白描黑。
        SGCCLabelTTF* lvLabel = SGCCLabelTTF::create(levelStr.c_str(), FONT_PANGWA,26 ,ccc3(0xff, 0xff, 0xff) );
        lvLabel->setAnchorPoint(zwAnchor);
        lvLabel->setPosition(ccp(iconSprite->getPosition().x + iconSprite->getContentSize().width*0.7, iconSprite->getPosition().y + 17));
        this->addChild(lvLabel, cttZorder);
        
        //三个星星，目前所有都默认3个星星。
        for(int i = 0; i < 3; i++)
        {
            CCSprite* xingji = CCSprite::createWithSpriteFrameName("xx.png");
            xingji->setAnchorPoint(zwAnchor);
            xingji->setPosition(ccp(iconSprite->getPosition().x + iconSprite->getContentSize().width*0.6 + xingji->getContentSize().width * i, iconSprite->getPosition().y - 25));
            this->addChild(xingji, cttZorder, i);
        }
    }
    else //当从月签奖励页面过来，则不显示等级和星级，改为显示提示性文本。
    {
        if (_scData)
        {
            CCSprite* ordLabel = NULL;
            CCSprite* vipLabel = NULL;
            
            CCPoint ordPos = ccp(iconSprite->getPosition().x + iconSprite->getContentSize().width*0.7, iconSprite->getPosition().y + 17);
            CCPoint vipPos = ccp(iconSprite->getPosition().x + iconSprite->getContentSize().width*0.7, iconSprite->getPosition().y - 25);
            
            //普通提示
            if (_scData->itemdesc().size() > 0)
            {
                ordLabel = SGRichLabel::getFomLabel(_scData->itemdesc().c_str(), CCSizeMake(304, 40), FONT_PANGWA, 22);
                ordLabel->setAnchorPoint(ccp(0, 1));
            }
            
            //VIP提示
            if (_scData->vipdesc().size() > 0 && SGPlayerInfo::sharePlayerInfo()->getShowvip())
            {
                vipLabel = SGRichLabel::getFomLabel(_scData->vipdesc().c_str(), CCSizeMake(304, 40), FONT_PANGWA, 22);
                vipLabel->setAnchorPoint(ccp(0, 1));
            }
            
            if (ordLabel)
            {
                ordLabel->setPosition(ordPos);
                this->addChild(ordLabel, cttZorder);
            }
            
            if (vipLabel)
            {
                vipLabel->setPosition(vipPos);
                this->addChild(vipLabel, cttZorder);
            }
        }
    }
    
    //按钮区>>>>
    
    if ( SGCIBOT_MY_EQUIP_UNWEAR== this->_opentype )
    {
        //取消
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGCommonInfoBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        cancel->setZOrder(cttZorder);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
        
        
        //"穿戴“和”卸下“需要分别处理。
        SGButton* onoffBtn = NULL;
        if (SGCIBOT_MY_EQUIP_WEAR == this->_opentype)
        {
            //卸下
            onoffBtn = SGButton::createFromLocal("generalInfoBtnBg.png", str_xiexia, this, menu_selector(SGCommonInfoBox::unwearEquipHandler), CCPointZero, FONT_PANGWA,ccWHITE,32, false, true);
            
        }
        else if (SGCIBOT_MY_EQUIP_UNWEAR == this->_opentype)
        {
            //穿戴
            onoffBtn = SGButton::createFromLocal("generalInfoBtnBg.png", str_zhuangbei, this, menu_selector(SGCommonInfoBox::wearEquipHandler), CCPointZero, FONT_PANGWA,ccWHITE,32, false, true);
        }
        onoffBtn->setZOrder(cttZorder);
        onoffBtn->setPosition(iconSprite->getPosition() + ccp(303, 0));
        this->addBtn(onoffBtn);
    }
    else if ( SGCIBOT_MY_EQUIP_WEAR == this->_opentype)
    {
        //强化
        SGButton *streng = SGButton::create("box_btn_newbg.png",
                                            "font_skill_streng.png",
                                            this,
                                            menu_selector(SGCommonInfoBox::strengthenEquipHandler),
                                            ccp(0, 0),
                                            false,
                                            true);
        streng->setZOrder(cttZorder);
        this->addBtn(streng);
        
        int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitEquipGuide,7);
        streng->setTag(limittag);
        streng->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-180, -270)));
        
        //转生
        SGButton* advance = SGButton::createFromLocal("box_btn_newbg.png", str_zhuansheng, this, menu_selector(SGCommonInfoBox::advanceEquipHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
        advance->setZOrder(cttZorder);
        
        //武将详情 已穿戴装备可转生 添加可转生红点提醒
        if(SGCIBOT_MY_EQUIP_WEAR == this->_opentype  && equip->selfCanAdv())
        {
            CCSprite * sprite = CCSprite::createWithSpriteFrameName("publc_notice.png");
            sprite->setPosition(ccpAdd(advance->getContentSize(), ccp(-5, -5)));
            advance->addChild(sprite ,2);
        }
        //武将详情 已穿戴装备可强化 添加红点提醒
        SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
        if(SGCIBOT_MY_EQUIP_WEAR == this->_opentype  && equip->selfShowStrengNotice() )
        {
            CCSprite * sprite = CCSprite::createWithSpriteFrameName("publc_notice.png");
            sprite->setPosition(ccpAdd(streng->getContentSize(), ccp(-5, -5)));
            streng->addChild(sprite ,2);
        }

        
        //装备转生第五步，高亮弹出框转生按钮
        int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_EUIQP_ADVANCED, 5);
        advance->setTag(tag);
        
        this->addBtn(advance);
        advance->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
        
        //取消
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGCommonInfoBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        cancel->setZOrder(cttZorder);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(180, -270)));

        
        //"穿戴“和”卸下“需要分别处理。
        SGButton* onoffBtn = NULL;
        if (SGCIBOT_MY_EQUIP_WEAR == this->_opentype)
        {
            //卸下
            onoffBtn = SGButton::createFromLocal("generalInfoBtnBg.png", str_xiexia, this, menu_selector(SGCommonInfoBox::unwearEquipHandler), CCPointZero, FONT_PANGWA,ccWHITE,32, false, true);

        }
        else if (SGCIBOT_MY_EQUIP_UNWEAR == this->_opentype)
        {
            //穿戴
            onoffBtn = SGButton::createFromLocal("generalInfoBtnBg.png", str_zhuangbei, this, menu_selector(SGCommonInfoBox::wearEquipHandler), CCPointZero, FONT_PANGWA,ccWHITE,32, false, true);
        }
        onoffBtn->setZOrder(cttZorder);
        onoffBtn->setPosition(iconSprite->getPosition() + ccp(303, 0));
        this->addBtn(onoffBtn);
    }
    else if (SGCIBOT_OTHER_EQUIP == this->_opentype)
    {
        //取消
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGCommonInfoBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        cancel->setZOrder(cttZorder);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
    }
    else if (SGCIBOT_PROTO_ITEM == this->_opentype)
    {
                //取消
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGCommonInfoBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        cancel->setZOrder(cttZorder);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
        
        
        if (_specmode != SGCSM_NULL)
        {
            //TODO: 添加判断去往哪儿的按钮。
            SGButton * goWhere = SGButton::createFromLocal("box_btn_newbg.png", str_source, this,
                                                           menu_selector(SGCommonInfoBox::gotoCallback),
                                                           ccp(0, 0),
                                                           FONT_PANGWA,
                                                           ccWHITE,
                                                           32);
            goWhere->setZOrder(cttZorder);
            this->addBtn(goWhere);
            
            goWhere->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-100, -270) ));
            cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(100, -270) ));
        }
        

        
        //详情，仅仅针对装备和武将，其他物品暂定以后只有小弹窗。
        if ( (equip || officer) && _myEnterType != CIBET_FIND_HELP && _myEnterType != CIBET_ALL_TASK_LAYER) //针对findHelpLayer和alltasklayer，不显示“详情”按钮。
        {
            SGButton* detailBtn = SGButton::createFromLocal("btn_fenbg.png", str_soldier_detail, this, menu_selector(SGCommonInfoBox::viewDetailHandler), CCPointZero, FONT_PANGWA,ccWHITE,32, false, true);
            detailBtn->setZOrder(cttZorder);
            this->addBtn(detailBtn);
            detailBtn->setPosition(iconSprite->getPosition() + ccp(303, 0));
        }
    }
    else //月签的两种情况，只有取消按钮。
    {
        //取消
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGCommonInfoBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        cancel->setZOrder(cttZorder);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
    }
    
    //介绍文字区>>>>
    
    if (equip || officer) //装备和武将，带有六类属性。
    {
        SGBaseMilitaryCard* eoCard = _mainItem;
        
        SGCCLabelTTF *atk_ = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 32 , COLOR_YELLOW);
        atk_->setPosition(ccpAdd(iconSprite->getPosition(), ccp(-iconSprite->getContentSize().width*.5f + atk_->getContentSize().width*.5f,-103)));
        this->addChild(atk_, cttZorder);
        
        SGCCLabelTTF *def_ = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 32 , COLOR_YELLOW);
        def_->setPosition(ccpAdd(atk_->getPosition(), ccp(0,-45)));
        this->addChild(def_, cttZorder);
        
        SGCCLabelTTF *mor_ = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 32 , COLOR_YELLOW);
        mor_->setPosition(ccpAdd(def_->getPosition(), ccp(0,-45)));
        this->addChild(mor_, cttZorder);
        
        SGCCLabelTTF *xuli_ = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 32 , COLOR_YELLOW);
        xuli_->setPosition(ccpAdd(atk_->getPosition(), ccp(230, 0)));
        this->addChild(xuli_, cttZorder);
        
        SGCCLabelTTF *sudu_ = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 32 , COLOR_YELLOW);
        sudu_->setPosition(ccpAdd(xuli_->getPosition(), ccp(0,-45)));
        this->addChild(sudu_, cttZorder);
        
        SGCCLabelTTF *gov_ = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 32 , COLOR_YELLOW);
        gov_->setPosition(ccpAdd(sudu_->getPosition(), ccp(16,-45)));
        this->addChild(gov_, cttZorder);
        
        CCString* str_atk = CCString::createWithFormat("%d",eoCard->getAtk());
        SGCCLabelTTF* atk = SGCCLabelTTF::create(str_atk->getCString(), FONT_BOXINFO, 30);
        atk->setAnchorPoint(ccp(1, 0.5));
        atk->setPosition(ccpAdd(atk_->getPosition(), ccp(120, 0)));
        this->addChild(atk, cttZorder);
        
        CCString* str_def = CCString::createWithFormat("%d",eoCard->getDef());
        SGCCLabelTTF* def = SGCCLabelTTF::create(str_def->getCString(), FONT_BOXINFO, 30);
        def->setAnchorPoint(ccp(1, 0.5));
        def->setPosition(ccpAdd(def_->getPosition(), ccp(120, 0)));
        this->addChild(def, cttZorder);
        
        CCString* str_shiqi = CCString::createWithFormat("%d",eoCard->getMorale());
        SGCCLabelTTF* shiqi = SGCCLabelTTF::create(str_shiqi->getCString(), FONT_BOXINFO, 30);
        shiqi->setAnchorPoint(ccp(1, 0.5));
        shiqi->setPosition(ccpAdd(mor_->getPosition(), ccp(120,0)));
        this->addChild(shiqi, cttZorder);
        
        CCString* strxuli = CCString::createWithFormat("%d",eoCard->getRound());
        SGCCLabelTTF* xuli = SGCCLabelTTF::create(strxuli->getCString(), FONT_BOXINFO, 30);
        xuli->setAnchorPoint(ccp(1, 0.5));
        xuli->setPosition(ccp(xuli_->getPosition().x + 120, atk->getPosition().y));
        this->addChild(xuli, cttZorder);
        
        CCString* str_spd = CCString::createWithFormat("%d",(int)eoCard->getSpeed());
        SGCCLabelTTF* speed = SGCCLabelTTF::create(str_spd->getCString(), FONT_BOXINFO, 30);
        speed->setAnchorPoint(ccp(1, 0.5));
        speed->setPosition(ccp(xuli_->getPosition().x + 120, def->getPosition().y));
        this->addChild(speed, cttZorder);
        
        CCString* str_gov = CCString::createWithFormat("%d",eoCard->getGoverning());
        SGCCLabelTTF* govern = SGCCLabelTTF::create(str_gov->getCString(), FONT_BOXINFO, 30);
        govern->setAnchorPoint(ccp(1, 0.5));
        govern->setPosition(ccp(gov_->getPosition().x+ 120-16, shiqi->getPosition().y));
        this->addChild(govern, cttZorder);
        

        if (equip) //装备显示缘分
        {
            SGCCLabelTTF* foLabel = SGCCLabelTTF::create(str_cib_fate_officer, FONT_BOXINFO, 32, COLOR_YELLOW);
            foLabel->setAnchorPoint(zwAnchor);
            foLabel->setPosition( iconSprite->getPosition() + ccp(-iconSprite->getContentSize().width * 0.5, -265) );
            this->addChild(foLabel, cttZorder);
            
            //緣分武將羅列（取DataModel）
            SGCCLabelTTF* foListLabel = SGCCLabelTTF::create( sdm->getEquipById(equip->getItemId())->getFateDesc()->getCString(), FONT_BOXINFO, 32, CCSize(400, 78), ccWHITE);
            foListLabel->setAnchorPoint(zwAnchor);
            foListLabel->setPosition(foLabel->getPosition() + ccp(0, -58));
            this->addChild(foListLabel, cttZorder);
            
        }
        else if (officer) //武将显示双技能
        {
            //主将计文本
            SGCCLabelTTF *font_zjji = SGCCLabelTTF::create(str_zhujiangji, FONT_BOXINFO, 28 , COLOR_YELLOW);
            CCPoint zjjFontPos = iconSprite->getPosition() + ccp(-iconSprite->getContentSize().width * 0.5, -275);
            font_zjji->setAnchorPoint(zwAnchor);
            font_zjji->setPosition(zjjFontPos);
            this->addChild(font_zjji, cttZorder);
            
            //被动技文本
            CCPoint wjjFontPos = ccpAdd(zjjFontPos, ccp(0, -58));
            SGCCLabelTTF *font_wjji = SGCCLabelTTF::create(str_beidongji, FONT_BOXINFO, 28 , COLOR_YELLOW);
            font_wjji->setAnchorPoint(zwAnchor);
            font_wjji->setPosition(wjjFontPos);
            this->addChild(font_wjji, cttZorder);
            
            //主将计文本介绍
            SGSkillDataModel *lordSkill = NULL;
//            if (enterType == TUJIAN_ENTER || enterType == PVP_SHOP_ENTER || enterType == LOGIN_ENTER||enterType==SHOW_OFFICER_ENTER) {
            if (true)
            {
                lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(officer->getLordSkill());
                //lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
            }
            else
            {
                int lordSkillId = officer->getLordId();
                lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(officer->getLordId());
            }
            
            float offX = 110;

            CCString* zjjDesc;
            if (lordSkill)
                zjjDesc = CCString::createWithFormat(str_GeneralInfoLayer_str1,lordSkill->getSkillName()->getCString(),lordSkill->getLevel());
            else
                zjjDesc = CCString::create(str_No);
            SGCCLabelTTF* labelZjj = SGCCLabelTTF::create( zjjDesc->getCString(), FONT_BOXINFO, 28 );
            this->addChild(labelZjj, cttZorder);
            labelZjj->setAnchorPoint(zwAnchor);
            labelZjj->setPosition( ccpAdd(zjjFontPos, ccp(offX, 0)));
            
            //被动技文本介绍
            CCString* wjjDesc;
            int idd = officer->getOfficerSkil();
            SGSkillDataModel *officerSkill=SGStaticDataManager::shareStatic()->getGeneralSkillById(officer->getOfficerSkil());
            if (officerSkill)//某种情况下直接使用_card,数据不正确
                wjjDesc = CCString::createWithFormat("%s Lv %s",officerSkill->getSkillName()->getCString(),officerSkill->getSkillClientLevel()->getCString());
            else
                wjjDesc = CCString::create(str_No);
            SGCCLabelTTF* labelWjj = SGCCLabelTTF::create( wjjDesc->getCString(), FONT_BOXINFO, 28 );
            this->addChild(labelWjj, cttZorder);
            labelWjj->setAnchorPoint(zwAnchor);
            labelWjj->setPosition( ccpAdd(wjjFontPos, ccp(offX, 0)) );
        }
    }
    else if (props || pieces || consumable || material) //道具/碎片/消耗品/装备转生材料
    {
        std::string desc;
        //TODO: 卖萌信息待添加。
        std::string meng = "这里是卖萌信息嗷，一步两步，是魔鬼的步伐。";//逗比逗比
        if (props)
        {
            SGPropsDataModel* pdm = sdm->getPropsById(props->getItemId());
            desc = pdm->getPropsInfo()->m_sString;
            meng = pdm->getPropJokeInfo()->m_sString;
        }
        else if (pieces)
        {
            SGPiecesDataModel* pdm = sdm->getPiecesDictById(pieces->getItemId());
            desc = pdm->getPiecesDesc()->m_sString;
            meng = pdm->getPiecesJokeInfo()->m_sString;
        }
        else if (consumable)
        {
            SGConsumableDataModel* cdm = sdm->getConsumableById(consumable->getItemId());
            desc = cdm->getConsumeDesc()->m_sString;
            meng = cdm->getConsumeJokeInfo()->m_sString;
        }
        else if (material)
        {
            SGMaterialDataModel* mdm = sdm->getMaterialById(material->getItemId());
            desc = mdm->getMaterialDesc()->m_sString;
            meng = mdm->getMaterialJokeInfo()->m_sString;
        }
        else
        {
            assert(1 == 0);
        }
        
        //基础信息(400x138)
        SGCCLabelTTF* descLabel = SGCCLabelTTF::create(desc.c_str(), FONT_BOXINFO, 28, CCSize(405, 135), ccWHITE);
        descLabel->setAnchorPoint(ccp(0, 1));
        descLabel->setPosition( mid9p->getPosition() + ccp(-mid9p->getContentSize().width * 0.5 + 25, mid9p->getContentSize().height * 0.5 - 5));
        this->addChild(descLabel, cttZorder);
        
        //介绍：
        SGCCLabelTTF* jieshaoLabel = SGCCLabelTTF::create(str_jieshao_colon, FONT_BOXINFO, 28, CCSize(405, 135), COLOR_YELLOW);
        jieshaoLabel->setAnchorPoint(ccp(0, 1));
        jieshaoLabel->setPosition( descLabel->getPosition() + ccp(0, -midHeight - 15));
        this->addChild(jieshaoLabel, cttZorder);
        
        //卖萌信息(400x100)
        SGCCLabelTTF* mengLabel = SGCCLabelTTF::create(meng.c_str(), FONT_BOXINFO, 28, CCSize(400, 100), ccWHITE);
        mengLabel->setAnchorPoint(ccp(0, 1));
        mengLabel->setPosition(jieshaoLabel->getPosition() + ccp(0, -30));
        this->addChild(mengLabel, cttZorder);
    }

}

//穿戴装备
void SGCommonInfoBox::wearEquipHandler()
{
    SGEquipCard* equipCard = (SGEquipCard*) _mainItem;
    SGOfficerCard* officerCard = (SGOfficerCard*) _subItem;
    int enterTypeOther = _otherEnterType;
    SGBoxDelegate* dlg = this->deletage;
    EFFECT_PLAY(MUSIC_BTN);
    if (equipCard->getOfficerCardId() != 0)
    {
        this->boxCloseWithOutCallBack();
        SGExchangeEquipBox *exchangeBox = SGExchangeEquipBox::create(dlg, officerCard, equipCard,enterTypeOther);
        SGMainManager::shareMain()->showBox(exchangeBox);
        
    }else{
        main::WearEquipmentRequest *request = new main::WearEquipmentRequest();
        request->set_officerid(officerCard->getSsid());
        request->set_equipmentid(equipCard->getSsid());
        SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_WEAR, request);
    }

}

//卸下装备
void SGCommonInfoBox::unwearEquipHandler()
{
    SGEquipCard* equipCard = (SGEquipCard*) _mainItem;
    SGOfficerCard* officerCard = (SGOfficerCard*) _subItem;
    EFFECT_PLAY(MUSIC_BTN);
    main::OffEquipmentRequest *request = new main::OffEquipmentRequest();
    request->set_officerid(officerCard->getSsid());
    request->set_equipmentid(equipCard->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_OFF, request);
}

//穿戴装备回调
void SGCommonInfoBox::wearEquipListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::WearEquipmentResponse *response = (main::WearEquipmentResponse *)sr->m_msg;
    if (sr)
    {
        SGEquipCard* equipCard = (SGEquipCard*) _mainItem;
        SGOfficerCard* officerCard = (SGOfficerCard*) _subItem;
        int enterType = this->_myEnterType;
        if (response->state() == 1)
        {
            equipCard->setOfficerCardId(officerCard->getSsid());
            officerCard->addEquip(equipCard);
            
            //首页布阵提示
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            
            CCString *str = CCString::createWithFormat(str_SGExchangeEquipBox_str5,officerCard->getOfficerName()->getCString(),equipCard->getOfficerName()->getCString());
            SGMainManager::shareMain()->showMessage(str->getCString());
            SGMainManager::shareMain()->showEffect(equipCard, 1);
            int type = _otherEnterType;
            this->boxCloseWithOutCallBack();
            if (type==11 || type==12)
            {
                SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,enterType,NULL,false,NULL,type);
            }
            else
            {
                SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,enterType,NULL,false);
            }
            
        }
    }
}

//卸下装备回调
void SGCommonInfoBox::unwearEquipListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::OffEquipmentResponse *response = (main::OffEquipmentResponse *)sr->m_msg;
    if (sr)
    {
        SGEquipCard* equipCard = (SGEquipCard*) _mainItem;
        SGOfficerCard* officerCard = (SGOfficerCard*) _subItem;
        if (response->state() == 1)
        {
            equipCard->setOfficerCardId(0);
            
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::createWithFormat("%d",2), "type");
            data = dict;
            CCString *str = CCString::createWithFormat(str_GeneralEquipBox_str4,officerCard->getOfficerName()->getCString(),equipCard->getOfficerName()->getCString());
            SGMainManager::shareMain()->showMessage(str->getCString());
            SGMainManager::shareMain()->showEffect(equipCard, 2);
            officerCard->getOfficerEquips()->removeObject(equipCard);
            
            
            //首页布阵提示
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            
            //刷新武将页面文本
            SGMainManager::shareMain()->refreshGilOfficerInfo();
            this->boxCloseWithOutCallBack();
            
        }
    }
}

//强化装备
void SGCommonInfoBox::strengthenEquipHandler()
{
    SGEquipCard* equipCard = (SGEquipCard*) _mainItem;
    if (equipCard->getCurrExp() >= equipCard->getMaxExp())
    {
        SGMainManager::shareMain()->showMessage(str_qianghua_tips1);
    }
    else
    {
        int enterType = this->_myEnterType;
        EFFECT_PLAY(MUSIC_BTN);
        equipCard->retain();
        this->boxCloseWithOutCallBack();
        SGMainManager::shareMain()->showStrengLayer(equipCard, 2, enterType,false); //hehe
        equipCard->release();
    }
}

//转生装备
void SGCommonInfoBox::advanceEquipHandler()
{
    if (equip)
    {
        SGEquipCard* equipCard = equip;
        SGEquipmentDataModel * edm = SGStaticDataManager::shareStatic()->getEquipById(equipCard->getItemId());
        if (edm->getEquipNextId() <= 0)
        {
            SGMainManager::shareMain()->showMessage(str_zhuansheng_no_need);
        }
        else
        {
            EFFECT_PLAY(MUSIC_BTN);
            equipCard->retain();
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showEquipAdvancedLayer(equipCard, true, 1); //1和2目前是一样的效果。
        }
        equipCard->release();
    }
}


//查看详情
void SGCommonInfoBox::viewDetailHandler()
{
    //因为close之后会将物品释放，先保存。
    SGBaseMilitaryCard* temp = _mainItem;
    temp->retain();
    
    this->boxCloseWithOutCallBack();
    if (equip)
    {
        SGMainManager::shareMain()->showEquipInfoLayer(equip,_otherEnterType, _otherIsBeforeDel);
    }
    else if (officer)
    {
        //for sgrewardsublayer.
        SGMainManager::shareMain()->showGeneralInfoLayer(officer, _otherEnterType, NULL, _otherIsBeforeDel);
        //old codes, could work well.
//        SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create(officer,_otherEnterType);
//        SGMainManager::shareMain()->showLayer(Layer,_otherIsBeforeDel);
    }
    
    //然后在release。
    temp->release();
}

void SGCommonInfoBox::boxClose()
{
    this->boxCloseWithOutCallBack();
    EFFECT_PLAY(MUSIC_BTN);
    //???
//    SGMainLayer *arranglayer = (SGMainLayer *)mainScene->getChildByTag(sg_equipsLayer);
//    if (arranglayer) {
//        mainScene->removeChildByTag(sg_equipsLayer);
//    }
}

void SGCommonInfoBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_myEnterType != CIBET_TOTAL_LOGIN_REWARD) //这个代表是从“登入抽奖”里过来的，因为这个是全屏界面，所以不能将mainlayer设置可触控，否则会造成穿透，把重置的任务交给“登入抽奖"本身。
    {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        if (mainLayer) {
            mainLayer->setIsCanTouch(true, false);
            mainLayer->setPortraitMenuCanTouch(false);
        }
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}

//阻挡其他点击。
bool SGCommonInfoBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
}

void SGCommonInfoBox::gotoCallback()
{
    this->boxCloseWithOutCallBack();
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    switch(_specmode)
    {
        case SGCSM_GOTO_ARENA:
        {
            SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_ARENA , SHOP_LAYER);
            break;
        }
        default:
            break;
    }
}





