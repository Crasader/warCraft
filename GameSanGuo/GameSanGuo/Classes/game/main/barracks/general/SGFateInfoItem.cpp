//
//  SGFateInfoItem.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/17/13.
//
//

#include "SGFateInfoItem.h"
#include "SGFateManager.h"
#include "SGFateBase.h"
#include "SGFateEffect.h"
#include "SGOfficerCard.h"
#include "SGLayout.h"
#include "CCScale9Sprite.h"
#include "SGStaticDataManager.h"
#include "SGFateInfoData.h"
#include "SGMainManager.h"
#include "SGStringConfig.h"

//各类图片名称
//#define SGFII_IMG_BKG "ofc_bkg.png" //背景大图
//#define SGFII_IMG_FRAME "ofc_frame.png" //母框架图
//#define SGFII_IMG_BOX "ofc_100x100.png" //子框架图
//#define SGFII_IMG_NAME "ofc_bar_fate_name.png" //将缘名字图
#define SGFII_IMG_FRAME "ofc_frame_new" //母框架图
#define SGFII_IMG_BOX "tran.png" //子框架图
#define SGFII_IMG_NAME "tran.png" //将缘名字图

#define SGFII_IMG_ATK "ofc_icon_big_atk.png" //攻击，剑
#define SGFII_IMG_DEF "ofc_icon_big_def.png" //防御，盾
#define SGFII_IMG_MOR "ofc_icon_big_mor.png" //血量，心
#define SGFII_IMG_SPD "ofc_icon_big_spd.png" //速度，马



//蜀魏吴群
#define RACE_SHU 1
#define RACE_WEI 2
#define RACE_WU 3
#define RACE_QUN 4

//男性女性
#define GENDER_MALE 1
#define GENDER_FEMALE 2

//1-6星星
#define STAR_1 1
#define STAR_2 2
#define STAR_3 3
#define STAR_4 4
#define STAR_5 5
#define STAR_6 6

//6大类型。传奇，盖世无双，勇冠三军，神鬼莫测，能攻善守，运筹帷幄
#define TYPE_CQ 1
#define TYPE_GSWS 2
#define TYPE_YGSJ 3
#define TYPE_SGMC 4
#define TYPE_NGSS 5
#define TYPE_YCWW 6

USING_NS_CC_EXT;

//构造
SGFateInfoItem::SGFateInfoItem():
enterType(0),
m_officerCard(NULL),
m_fateBase(NULL),
state(0)
{
    ;
}

//析构
SGFateInfoItem::~SGFateInfoItem()
{
    ;
}

//创建
SGFateInfoItem* SGFateInfoItem::create(SGFateInfoData* data, int enterType)
{
    SGFateInfoItem* pRet = new SGFateInfoItem();
    
    if ( pRet && pRet->init(data) ) {
		pRet->enterType = enterType;
        pRet->initCellItem();
		
//        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    CCLOG("%s", "初始化SGFateInfoItem数据错误");
    return NULL;
}

//设置两个数据引用
bool SGFateInfoItem::init(SGFateInfoData* data)
{
    if (SNSTableViewCellItem::init()) //么有办法，initCellItem本身不接受参数。
    {
        if (!data)
            return false;
        m_officerCard = data->getCard();
        SGFateManager* sgfm = SGFateManager::sharedManager();
        m_fateBase = sgfm->getFateBaseById(data->getId());
        state = data->getState();
        return true;
    }
    return false;
}

//初始化界面
bool SGFateInfoItem::initCellItem()
{
    //检测本将缘是否条件满足，不检测是否开启了，留给外围判断
    SGFateManager* sgfm = SGFateManager::sharedManager();
  
    bool light = false;
    if (enterType != CHATSHOWENTRYTAG&&enterType != 20001) {
        light = sgfm->checkIsFateGoal(m_officerCard, m_fateBase->getId());
    }
    else
    {
        light =  state == 1 ?false:true;
    }
    
	bool isFromMarket = ((enterType != 20) ? true : false);
    
    ccColor3B onColor = ccc3(0xf4, 0xac, 0x00);
    ccColor3B offColor = ccc3(0xa4, 0xa4, 0xa4);
    
    ccColor3B textOnColor = onColor;
    ccColor3B textOffColor = ccc3(0xa4, 0xa4, 0xa4);
    
#define ScaleList0   1.0//cgpUI for fate 3->2
    //大背景框架
//    CCScale9Sprite* frame = CCScale9Sprite::create("ofc_frame_new.png");
//    frame->setPreferredSize(CCSizeMake(467, 228 * ScaleList0));
//    frame->setPosition(screenwidth * 0.5, 228 * 0.5 * ScaleList0);
    CCSprite* frame = CCSprite::createWithSpriteFrameName("ofc_frame_new.png");
    frame->setPosition(ccp(screenwidth * 0.5, 228 * 0.5 ));
    frame->setScaleY(0.72);
    frame->setScaleX(0.85);
    this->addChild(frame, 2);
    //frame->setVisible(false);  //cgpUI
    
    //名字底图
    CCSprite* nameImg = CCSprite::createWithSpriteFrameName("tran.png");
    CCPoint niPos = ccp(139, 210);
    nameImg->setPosition(niPos);
    nameImg->setScaleX(1.2);
    nameImg->setScaleY(1.3);
    frame->addChild(nameImg);

    //名字
    SGCCLabelTTF* nameText = SGCCLabelTTF::create(m_fateBase->getName()->getCString(), FONT_PANGWA, 36, (light && isFromMarket) ? onColor : offColor );
    //modify by:zyc. merge into create.
    //nameText->setColor((light && isFromMarket) ? onColor : offColor);
    CCPoint ntPos = ccp(nameImg->getContentSize().width * 0.5, nameImg->getContentSize().height * 0.5 + 10);
    nameText->setPosition(ntPos);
    nameImg->addChild(nameText);
    
    //获得图标和文字
    CCArray* iconAndText = getIconAndTextString();
    
    //加成图标
    CCSprite* iconImg = CCSprite::createWithSpriteFrameName( ((CCString*) iconAndText->objectAtIndex(0))->getCString() );
    CCPoint iiPos = ccpAdd(niPos, ccp(172, 0));
    iconImg->setPosition(iiPos);
    frame->addChild(iconImg);
    
    //加成文字
    SGCCLabelTTF* iconText = SGCCLabelTTF::create( ((CCString*) iconAndText->objectAtIndex(1))->getCString(), FONT_BOXINFO, 32 ,(light && isFromMarket) ? textOnColor : textOffColor);
    //modify by:zyc. merge into create.
    //iconText->setColor((light && isFromMarket) ? textOnColor : textOffColor);
    CCPoint itPos = ccpAdd(iiPos, ccp(77, 0));
    iconText->setPosition(itPos);
    frame->addChild(iconText);
    
    //细节文本，略复杂。
    CCString* detailText = getDetailString( (CCString*) iconAndText->objectAtIndex(2) );
    SGCCLabelTTF* detailLabel = SGCCLabelTTF::create(detailText->getCString(), FONT_BOXINFO, 32, CCSizeMake(300, 140), kCCTextAlignmentLeft , kCCVerticalTextAlignmentTop , (light && isFromMarket) ? textOnColor : textOffColor);
    CCPoint detailPos = ccp(410 * 0.5f, 130 * 0.5f);
    //detailLabel->setFontFillColor((light && isFromMarket) ? textOnColor : textOffColor);
    detailLabel->setPosition(detailPos);
    

    
    //文本框架，动态调整
    CCScale9Sprite* tnine = CCScale9Sprite::createWithSpriteFrameName(SGFII_IMG_BOX);
    CCPoint tninePos = ccp(467 * 0.5f, 90);
    tnine->setPreferredSize(CCSizeMake(410, 130));
    tnine->setPosition(tninePos);
    frame->addChild(tnine, 2);
    
    //添加细节文本到框体
    tnine->addChild(detailLabel);
    
    //底部获取方式细节文本
    //如果有对应的获取方式文本就显示
    if (m_fateBase->getSourceFateInfo())
    {
        SGCCLabelTTF *sourceDetailTxt = SGCCLabelTTF::create(m_fateBase->getSourceFateInfo()->getCString(), FONT_BOXINFO, 28,
                                                             CCSizeMake(390, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, textOffColor);
        sourceDetailTxt->setPosition(ccpAdd(detailPos, ccp(0, -40)));
        tnine->addChild(sourceDetailTxt);
    }

    return true;
}

//返回拼接好的字符串
CCString* SGFateInfoItem::getDetailString(CCString* ccstr)
{    
    //最终字符串
    std::string text;
    //临时变量
    SGFateManager* sgfm = SGFateManager::sharedManager();
    SGStaticDataManager* man = SGStaticDataManager::shareStatic();
    std::vector<int>* vec = NULL;
    
//    return CCString::create("这是描述噢~这是描述噢~这是描述噢~这是描述噢~这是描述噢~这是描述噢~这是描述噢~");
    
    switch (m_fateBase->getConditionType())
    {
        case FCT_OFFICER: //武将
        {
            //与XXX、XXX、XXX
            text = str_FateInfoItem_str1;
            SGOfficerDataModel* data = NULL;
            vec = sgfm->parseString(m_fateBase->getConditionString());
            int cur = 1;
            int size = vec->size();
            std::vector<int>::iterator iter = vec->begin();
            for (; iter != vec->end(); iter++)
            {
                //排除自己
                if (m_officerCard->getProtoId() != *iter)
                {
                    data = man->getOffcierByProtoId(*iter);
                    text += data->getOfficerName()->m_sString;
                    if (cur < size) //不是最后一个，则添加顿号
                        text += "、";
                    cur++;
                }
            }
            //同时上阵
            text += str_FateInfoItem_str2;
            //攻击+10% / 防御+100
            text += ccstr->m_sString;
        }
            break;
        case FCT_EQUIP: //装备
        {
            //装备XXX、XXX
            text = str_FateInfoItem_str3;
            SGEquipmentDataModel* data = NULL;
            vec = sgfm->parseString(m_fateBase->getConditionString());
            int cur = 1;
            int size = vec->size();
            std::vector<int>::iterator iter = vec->begin();
            for (; iter != vec->end(); iter++)
            {
                data = man->getEquipByProtoId(*iter);
                text += data->getEquipName()->m_sString;
                if (cur < size) //记得添加顿号
                    text += "、";
                cur++;
            }
            //可使本武将
            text += str_FateInfoItem_str4;
            //攻击+10% / 防御+100
            text += ccstr->m_sString;
        }
            break;
        case FCT_NATION: //国家
        {
            //同时上阵 （蜀魏吴群）
            text = str_FateInfoItem_str5;
            vec = sgfm->parseString(m_fateBase->getConditionString());
            int type = (*vec)[0];
            int amount = (*vec)[1];
            //数量
            char hey[4];
            sprintf(hey, "%d", amount);
            text += hey;
            text += str_FateInfoItem_str6;
            if (RACE_SHU == type)
                text += str_FateInfoItem_str7;
            else if (RACE_WEI == type)
                text += str_FateInfoItem_str8;
            else if (RACE_WU == type)
                text += str_FateInfoItem_str9;
            else if (RACE_QUN == type)
                text += str_FateInfoItem_str10;
            else
                text += str_FateInfoItem_str11;
            //武将同时上阵
            text += str_FateInfoItem_str12;
            //攻击+10% / 防御+100
            text += ccstr->m_sString;
        }
            break;
        case FCT_GENDER: //性别
        {
            //与X个XX
            text = str_FateInfoItem_str5;
            vec = sgfm->parseString(m_fateBase->getConditionString());
            int type = (*vec)[0];
            int amount = (*vec)[1];
            //数量
            char hey[4];
            sprintf(hey, "%d", amount);
            text += hey;
            text += str_FateInfoItem_str6;
            if (GENDER_MALE == type)
                text += str_FateInfoItem_str13;
            else if (GENDER_FEMALE == type)
                text += str_FateInfoItem_str14;
            else
                text += str_FateInfoItem_str15;
            //武将同时上阵
            text += str_FateInfoItem_str12;
            //攻击+10% / 防御+100
            text += ccstr->m_sString;
        }
            break;
        case FCT_TYPE: //类型
        {
            //与X个XX
            text = str_FateInfoItem_str5;
            vec = sgfm->parseString(m_fateBase->getConditionString());
            int type = (*vec)[0];
            int amount = (*vec)[1];
            //数量
            char hey[4];
            sprintf(hey, "%d", amount);
            text += hey;
            text += str_FateInfoItem_str6;
            //添加武将类型文本
            if (TYPE_CQ == type)
                text += str_FateInfoItem_str16;
            else if (TYPE_GSWS == type)
                text += str_FateInfoItem_str17;
            else if (TYPE_YGSJ == type)
                text += str_FateInfoItem_str18;
            else if (TYPE_SGMC == type)
                text += str_FateInfoItem_str19;
            else if (TYPE_NGSS == type)
                text += str_FateInfoItem_str20;
            else if (TYPE_YCWW == type)
                text += str_FateInfoItem_str21;
            else
                text += str_FateInfoItem_str22;
            //武将同时上阵
            text += str_FateInfoItem_str12;
            //攻击+10% / 防御+100
            text += ccstr->m_sString;
        }
            break;
        case FCT_STAR: //星级
        {
            //与X个XX
            text = str_FateInfoItem_str5;
            vec = sgfm->parseString(m_fateBase->getConditionString());
            int type = (*vec)[0];
            int amount = (*vec)[1];
            //数量
            char hey[4];
            sprintf(hey, "%d", amount);
            text += hey;
            text += str_FateInfoItem_str6;
            if (STAR_1 == type)
                text += str_FateInfoItem_star1;
            else if (STAR_2 == type)
                text += str_FateInfoItem_star2;
            else if (STAR_3 == type)
                text += str_FateInfoItem_star3;
            else if (STAR_4 == type)
                text += str_FateInfoItem_star4;
            else if (STAR_5 == type)
                text += str_FateInfoItem_star5;
            else if (STAR_6 == type)
                text += str_FateInfoItem_star6;
            else
                text += str_FateInfoItem_str23;
            //武将同时上阵
            text += str_FateInfoItem_str12;
            //攻击+10% / 防御+100
            text += ccstr->m_sString;
        }
            break;
        default:
        {
            text = str_FateInfoItem_str24;
        }
            break;
    }
    CC_SAFE_DELETE(vec);
    return CCString::create(text);
}

//获得加成类型图标，以及加成文字
CCArray* SGFateInfoItem::getIconAndTextString()
{
    //取第一个缘分效果数据的类型
    SGFateEffect* sgfe = SGFateManager::sharedManager()->getFateEffectById( *m_fateBase->getEffectList().begin() );
    CCArray* pArr = CCArray::createWithCapacity(2);
    //图标与文本
    std::string str;
    std::string attr;
    switch (sgfe->getAttrType())
    {
        case FAT_MOR:
        {
            str = SGFII_IMG_MOR;
            attr = SGFII_ATTR_MOR;
        }
            break;
        case FAT_ATK:
        {
            str = SGFII_IMG_ATK;
            attr = SGFII_ATTR_ATK;
        }
            break;
        case FAT_DEF:
        {
            str = SGFII_IMG_DEF;
            attr = SGFII_ATTR_DEF;
        }
            break;
        case FAT_SPD:
        {
            str = SGFII_IMG_SPD;
            attr = SGFII_ATTR_SPD;
        }
            break;
        default:
        {
            str = SGFII_IMG_ATK;
            attr = SGFII_ATTR_ATK;
        }
            break;
    }
    pArr->addObject(CCString::create(str));
    //文字
    CCString* text = NULL;
    FateValueType fvt = sgfe->getValueType();
    int amnt = sgfe->getValue();
    if (FVT_PERCENT == fvt) //百分比
    {
        text = CCString::createWithFormat("+%d%%", amnt);
    }
    else //数字
    {
        text = CCString::createWithFormat("+%d", amnt);
    }
    pArr->addObject(text);
    //攻击+25% / 防御+100
    pArr->addObject(CCString::create((attr + text->m_sString)));
    return pArr;
}

