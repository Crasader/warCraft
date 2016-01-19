//
//  SGDrawCards.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#include "SGDrawCards.h"
#include "SGShowString.h"
#include "SGTools.h"

SGDrawCards::SGDrawCards()
{
    ;
}


SGDrawCards::~SGDrawCards()
{
    ;
}


CCSprite *SGDrawCards::drawofficercard(int itemid, LayerTag tag, int num /*= 0*/)

{
    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(itemid);
    SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),tag);
    
//    CCSprite* bj = CCSprite::createWithSpriteFrameName("card_bj.png");
//    card->addChild(bj);
    
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
    card->addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));

    int starlvl = temp->getOfficerCurrStarLevel();
    if (starlvl>1) {
        starlvl-=1;
    }
    CCString* str = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
    
    CCSprite* kuang = CCSprite::createWithSpriteFrameName(str->getCString());
    card->addChild(kuang);
    kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
    
    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
    countryName->setAnchorPoint(ccp(1,1));
    countryName->ignoreAnchorPointForPosition(false);
    countryName->setPosition(ccp(card->getContentSize().width*0.3,card->getContentSize().height));
    card->addChild(countryName,30,30);
    
    //转生+7
    if (temp->getAdvNum()>0)
    {
        SGCCLabelTTF* advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d", temp->getAdvNum())->getCString(), FONT_PANGWA, 24, ccGREEN);
        advancenumber->setAnchorPoint(ccp(1,1));
        advancenumber->setPosition(ccpAdd(kuang->getContentSize(),ccp(-advancenumber->getContentSize().width + 15, -advancenumber->getContentSize().height + 15)  ));
        kuang->addChild(advancenumber,50,250);
    }

    return card;
}

//由于历史原因，代币类型的东西也是使用itemId来绘制图标，这里做的统一处理。
CCSprite *SGDrawCards::drawcoinetccard(int itemid, LayerTag tag, int num /*= 0*/)
{
    SGMainManager::shareMain()->addHeadIconPlistByNum(itemid,tag);
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    CCSprite * spritePlayerHead = NULL;
    CCSprite* kuang = NULL;
    if (itemid==10000) { //元宝
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("big_yb.png")->getCString());
    }
    else if (9995 == itemid)//PVP积分（天梯积分）
    {
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName("store_tianti_big.png");
    }
    else if (9994 == itemid)//军魂
    {
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName("soul_icon.png");
    }
    else if (9993 == itemid)//体力
    {
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName("mail_streng.png");
    }
    else if (9992 == itemid)//军粮
    {
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName("mail_food.png");
    }
    else if (9991 == itemid) //勋章
    {
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName("tongqueXunBig.png");

    }
    else //现用：9999=金币(铜钱)，9996=秘方 ; 实质上废弃：9997=军功，9998=喇叭。
    {
        kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
        spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",itemid)->getCString());
    }
    card->addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
    
    card->addChild(kuang);
    kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));

    return card;
}

CCSprite *SGDrawCards::drawpropcard(int itemid, LayerTag tag, int num /*= 0*/)
{
    SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(itemid);
    SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),tag);
    
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    
    CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
    card->addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
    CCSprite* kuang = NULL;
    if (temp->getPropsCountry()) {
        int starlvl = temp->getPropsCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        CCString* str = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
        
        kuang = CCSprite::createWithSpriteFrameName(str->getCString());
        card->addChild(kuang);
        
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(card->getContentSize().width*0.3,card->getContentSize().height));
        card->addChild(countryName,30,30);
    }
    else
    {
        int starlvl = temp->getPropsCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }

        kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
        card->addChild(kuang);
    }
    kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));

    return card;
}

CCSprite *SGDrawCards::drawequipcard(int itemid, LayerTag tag, int num /*= 0*/)

{
    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(itemid);
    SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),tag);
    
//    CCSprite* bj = CCSprite::createWithSpriteFrameName("card_bj.png");
//    card->addChild(bj);
   CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
    card->addChild(spritePlayerHead);
    spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));

    int starlvl = temp->getEquipCurrStarLevel();
    if (starlvl>1) {
        starlvl-=1;
    }
    CCString* str = CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl);
    
    CCSprite* kuang = CCSprite::createWithSpriteFrameName(str->getCString());
    card->addChild(kuang);
    kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
    
    //转生+7
    if (temp->getAdvNum()>0)
    {
        SGCCLabelTTF* advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d", temp->getAdvNum())->getCString(), FONT_PANGWA, 24, ccGREEN);
        advancenumber->setAnchorPoint(ccp(1,1));
        advancenumber->setPosition(ccpAdd(kuang->getContentSize(),ccp(-advancenumber->getContentSize().width + 15, -advancenumber->getContentSize().height + 15)  ));
        kuang->addChild(advancenumber,50,250);
    }
 

    return card;
}

CCSprite *SGDrawCards::drawpiececard(int itemid, LayerTag tag, int num /*= 0*/)
{
   ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, tag);
    SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(itemid);
    int starLv = pieces->getStarLevel();
    if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
    {
        starLv -= 1;
    }
    
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    CCString *str_kuang=NULL;
    if (pieces->getCallingCardId() == -1)//如果是通用碎片，已废弃通用碎片，不可能出现。
    {
        str_kuang = CCString::createWithFormat("common_pieces_border.png");
    }
    else if (pieces->getPiecesType() == 1)//装备碎片
    {
//        str_kuang  = CCString::createWithFormat("equipPinzhi_%d_0.png",SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId())->getEquipStarPinzhi());
        str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",starLv);
        
        SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId());
        SGMainManager::shareMain()->addHeadIconPlistByNum(eq->getIconId(),tag);
        
        CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",eq->getIconId())->getCString());
        card->addChild(spritePlayerHead);
        spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
        
    }
    else if (pieces->getPiecesType() == 0)//武将碎片
    {
//        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
//        int starlvl = temp->getOfficerCurrStarLevel();
        str_kuang = CCString::createWithFormat("officerPinzhi_%d_0.png",starLv);
        
        SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
        SGMainManager::shareMain()->addHeadIconPlistByNum(of->getIconId(),tag);
        
        CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",of->getIconId())->getCString());
        card->addChild(spritePlayerHead);
        spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
        
        

    }
    CCSprite* kuang = CCSprite::createWithSpriteFrameName(str_kuang->getCString());
    card->addChild(kuang);
    kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
    
    if (pieces->getCallingCardId() != -1)
    {
        CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
        mask->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
        card->addChild(mask);
    }
    
    return card;

}

CCSprite *SGDrawCards::drawconsumecard(int itemid, LayerTag tag, int num /*= 0*/)
{
    SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(itemid);
    
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    if (consume==NULL ||consume->getIconId()<0) {
        return card;
    }
    else
    {
       SGMainManager::shareMain()->addHeadIconPlistByNum(consume->getIconId(), tag);
        int starLvl = consume->getConsumeStarLvl();
        if (starLvl > 1)
            starLvl -= 1;
        
        CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",consume->getIconId())->getCString());
        card->addChild(spritePlayerHead);
        spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
        
        CCString* str = CCString::createWithFormat("equipPinzhi_%d_0.png",starLvl);
        
        CCSprite* kuang = CCSprite::createWithSpriteFrameName(str->getCString());
        card->addChild(kuang);
        kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
        
    }
    return card;
}

//画装备转生材料
CCSprite *SGDrawCards::drawmaterialcard(int itemid,LayerTag tag, int num /*= 0*/)
{
    SGMaterialDataModel* mdm = SGStaticDataManager::shareStatic()->getMaterialById(itemid);
    
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    int iconId = mdm->getMaterialIconId();
    
    if (mdm==NULL || iconId <= 0)
    {
        ; //空ICON
    }
    else
    {
        SGMainManager::shareMain()->addHeadIconPlistByNum(iconId, tag);
        
        int starLvl = mdm->getMaterialBorderColor();
        if (starLvl > 1) //因为绿色品质填表为3，但是图片名字里是2。
            starLvl -= 1;
        //本体
        CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
        card->addChild(spritePlayerHead);
        spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
        //边框
        CCString* str = CCString::createWithFormat("equipPinzhi_%d_0.png",starLvl);
        CCSprite* kuang = CCSprite::createWithSpriteFrameName(str->getCString());
        card->addChild(kuang);
        kuang->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
    }
    return card;
    //----
}

CCSprite *SGDrawCards::drawallcards(int itemid, int type, LayerTag tag, int num /*= 0*/)
{
    //转到新API。
    return createNormalSprite(itemid, type, tag);
    
    /*
    CCSprite *card = NULL;
    SGBaseItemType bit = (SGBaseItemType) type;
    
    switch (bit)
    {
        case BIT_OFFICER://武将
            card = SGDrawCards::drawofficercard(itemid,tag,num);
            break;
        case BIT_EQUIP://装备
            card = SGDrawCards::drawequipcard(itemid,tag,num);
            break;
        case BIT_PROP://道具
            card = SGDrawCards::drawpropcard(itemid,tag,num);
            break;
        case BIT_PIECE://碎片
            card = SGDrawCards::drawpiececard(itemid,tag,num);
            break;
        case BIT_CONSUMABLE://消耗品
            card = SGDrawCards::drawconsumecard(itemid,tag,num);
            break;
        case BIT_MATERIAL: //装备转生材料
            card = SGDrawCards::drawmaterialcard(itemid,tag,num);
            break;
        //原有的:
        case BIT_INGOT://元宝
        case BIT_GOLD://铜钱
        case BIT_RECIPE://炼丹秘方
        //新增的:
        case BIT_SOUL: //军魂
        case BIT_VIGOUR: //体力
        case BIT_GRAIN: //军粮
        case BIT_GRADE: //PVP积分（天梯）
        case BIT_MEDAL: //勋章
            card = SGDrawCards::drawcoinetccard(itemid,tag,num);
            break;
        //实质废弃的，这些不会再推送，如果还有的话，则代表必须处理的错误。
        case BIT_FEAT://军功
        case BIT_HORN://小喇叭
        default:
            break;
    }

    return card;
     */
}

//奥义图标
CCSprite* SGDrawCards::createSprite(const main::ItemDataProto& idp, LayerTag laTag, SGIconType iconType, bool haveNameBg)
{
    CCSprite* ret = NULL;
    switch (iconType)
    {
        case SGIT_PURE:
            ret = createPureSprite(idp.itemid(), idp.itemtype(), laTag);
            break;
        case SGIT_SIMPLE:
            ret = createSimpleSprite(idp.itemid(), idp.itemtype(), laTag);
            break;
        case SGIT_NORMAL:
            ret = createNormalSprite(idp.itemid(), idp.itemtype(), laTag);
            break;
        case SGIT_COMPLEX:
            ret = createComplexSprite(idp.itemid(), idp.itemtype(), idp.itemnum(), laTag);
            break;
        case SGIT_DETAILED:
            ret = createDetailedSprite(idp.itemid(), idp.itemtype(), idp.itemnum(), laTag, haveNameBg);
            break;
        default:
            break;
    }
    return ret;
}

//奥义按钮
SGButton* SGDrawCards::createButton(const main::ItemDataProto& idp, LayerTag laTag, SGIconType iconType, CCObject *target, SEL_MenuHandler selector, bool haveNameBg)
{
    CCSprite* spt1 = NULL;
    CCSprite* spt2 = NULL;
    
    switch (iconType)
    {
        case SGIT_PURE:
            spt1 = createPureSprite(idp.itemid(), idp.itemtype(), laTag);
            spt2 = createPureSprite(idp.itemid(), idp.itemtype(), laTag);
            break;
        case SGIT_SIMPLE:
            spt1 = createSimpleSprite(idp.itemid(), idp.itemtype(), laTag);
            spt2 = createSimpleSprite(idp.itemid(), idp.itemtype(), laTag);
            break;
        case SGIT_NORMAL:
            spt1 = createNormalSprite(idp.itemid(), idp.itemtype(), laTag);
            spt2 = createNormalSprite(idp.itemid(), idp.itemtype(), laTag);
            break;
        case SGIT_COMPLEX:
            spt1 = createComplexSprite(idp.itemid(), idp.itemtype(), idp.itemnum(), laTag);
            spt2 = createComplexSprite(idp.itemid(), idp.itemtype(), idp.itemnum(), laTag);
            break;
        case SGIT_DETAILED:
            spt1 = createDetailedSprite(idp.itemid(), idp.itemtype(), idp.itemnum(), laTag, haveNameBg);
            spt2 = createDetailedSprite(idp.itemid(), idp.itemtype(), idp.itemnum(), laTag, haveNameBg);
            break;
        default:
            break;
    }

    return SGButton::createFromSprite(spt1, spt2, target, selector);
}

//商品按钮
SGButton* SGDrawCards::createButtonInGoods(const main::GoodsDataProto& gdp, LayerTag laTag, SGIconType iconType, CCObject *target, SEL_MenuHandler selector, bool havePriceBg)
{
    SGIconInfo sgii;
    figureAndBind(gdp.item().itemid(), gdp.item().itemtype(), gdp.item().itemnum(), laTag, SGIT_COMPLEX, sgii, false);
    figureAndBindInGoods(gdp.pricetype(), gdp.pricevalue(), laTag, sgii, true);
    CCSprite* spt1 = generateSprite(sgii);
    CCSprite* spt2 = generateSprite(sgii);
    
    return SGButton::createFromSprite(spt1, spt2, target, selector);
}

CCSprite* SGDrawCards::createPureSprite(int itemid, int itemType, LayerTag laTag) //单图，不包括边框
{
    SGIconInfo sgii;
    figureAndBind(itemid, itemType, 0, laTag, SGIT_PURE, sgii);
    return generateSprite(sgii);
}

CCSprite* SGDrawCards::createSimpleSprite(int itemid, int itemType, LayerTag laTag)
{
    SGIconInfo sgii;
    figureAndBind(itemid, itemType, 0, laTag, SGIT_SIMPLE, sgii);
    return generateSprite(sgii);
}

CCSprite* SGDrawCards::createNormalSprite(int itemid, int itemType, LayerTag laTag)
{
    SGIconInfo sgii;
    figureAndBind(itemid, itemType, 0, laTag, SGIT_NORMAL, sgii);
    return generateSprite(sgii);
}

CCSprite* SGDrawCards::createComplexSprite(int itemid, int itemType, int itemNum, LayerTag laTag)
{
    SGIconInfo sgii;
    figureAndBind(itemid, itemType, itemNum, laTag, SGIT_COMPLEX, sgii);
    return generateSprite(sgii);
}

CCSprite* SGDrawCards::createDetailedSprite(int itemid, int itemType, int itemNum, LayerTag laTag, bool haveNameBg)
{
    SGIconInfo sgii;
    figureAndBind(itemid, itemType, itemNum, laTag, SGIT_DETAILED, sgii, haveNameBg);
    return generateSprite(sgii);
}

SGButton* SGDrawCards::createSimpleButton(int itemid, int itemType, LayerTag laTag, CCObject *target, SEL_MenuHandler selector)
{
    CCSprite* spt1 = createSimpleSprite(itemid, itemType, laTag);
    CCSprite* spt2 = createSimpleSprite(itemid, itemType, laTag);
    return SGButton::createFromSprite(spt1, spt2, target, selector);
}

SGButton* SGDrawCards::createNormalButton(int itemid, int itemType, LayerTag laTag, CCObject *target, SEL_MenuHandler selector)
{
    CCSprite* spt1 = createNormalSprite(itemid, itemType, laTag);
    CCSprite* spt2 = createNormalSprite(itemid, itemType, laTag);
    return SGButton::createFromSprite(spt1, spt2, target, selector);
}

SGButton* SGDrawCards::createComplexButton(int itemid, int itemType, int itemNum, LayerTag laTag, CCObject *target, SEL_MenuHandler selector)
{
    CCSprite* spt1 = createComplexSprite(itemid, itemType, itemNum, laTag);
    CCSprite* spt2 = createComplexSprite(itemid, itemType, itemNum, laTag);
    return SGButton::createFromSprite(spt1, spt2, target, selector);
}

SGButton* SGDrawCards::createDetailedButton(int itemid, int itemType, int itemNum, LayerTag laTag, CCObject *target, SEL_MenuHandler selector, bool haveNameBg)
{
    CCSprite* spt1 = createDetailedSprite(itemid, itemType, itemNum, laTag, haveNameBg);
    CCSprite* spt2 = createDetailedSprite(itemid, itemType, itemNum, laTag, haveNameBg);
    return SGButton::createFromSprite(spt1, spt2, target, selector);
}

//绘制图标的一些预制变量。
const static int NUM_TO_SHOW_NUM_LABEL = 1; //大于等于此值才显示数量label。
const static char* ADV_LABEL_FORMAT = "+%d"; //转生label的样式。
const static char* NUM_LABEL_FORMAT = "X%d"; //数量label的样式。
const static char* PRICE_LABEL_FORMAT = "%d"; //价格label的样式。
static char buffer[100] = {0}; //字符串缓冲区

//产生图标相关信息，并且绑定资源。
void SGDrawCards::figureAndBind(int itemid, int itemType, int itemNum, LayerTag laTag, SGIconType iconType, SGIconInfo& info, bool haveNameBg)
{
    SGMainManager* sgmm = SGMainManager::shareMain();
    SGStaticDataManager* sgsdm = SGStaticDataManager::shareStatic();
    
    SGBaseItemType bit = (SGBaseItemType) itemType;
    info.itemType = bit;
    info.itemId = itemid;
    info.haveNameBg = haveNameBg;
    
    switch (bit)
    {
        case BIT_OFFICER://武将
        {
            SGOfficerDataModel *temp = sgsdm->getOfficerById(itemid);
            sgmm->addHeadIconPlistByNum(temp->getIconId(),laTag);
            
            //本尊
            sprintf(buffer, "head%d.png", temp->getIconId());
            info.iconImage = buffer;
            
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                //边框
                int starlvl = temp->getOfficerCurrStarLevel();
                info.quality = starlvl;
                if (starlvl>1)
                {
                    starlvl-=1;
                }
                sprintf(buffer, "officerPinzhi_%d_0.png",starlvl);
                info.frameImage = buffer;
                
                //国家从属
                sprintf(buffer, "countryName%d.png", temp->getOfficerRace());
                info.nationImage = buffer;
                
                //转生等级
                if (iconType > SGIT_SIMPLE && temp->getAdvNum()>0) //非简略模式即显示转生。
                {
                    sprintf(buffer, ADV_LABEL_FORMAT, temp->getAdvNum());
                    info.advNumLabel = buffer;
                }
                
                //名称
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    sprintf(buffer, "%s", temp->getOfficerName()->getCString());
                    info.nameLabel = buffer;
                }
            }
        }
            break;
        case BIT_EQUIP://装备
        {
            SGEquipmentDataModel *temp = sgsdm->getEquipById(itemid);
            sgmm->addHeadIconPlistByNum(temp->getIconId(),laTag);
            
            //本尊
            sprintf(buffer, "head%d.png", temp->getIconId());
            info.iconImage = buffer;
            
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                //边框
                int starLvl = temp->getEquipStarPinzhi();
                info.quality = starLvl + 1; //因为装备本来就-1了，反而需要加回来。
                sprintf(buffer, "equipPinzhi_%d_0.png",starLvl);
                info.frameImage = buffer;
                
                //转生等级
                if (iconType > SGIT_SIMPLE && temp->getAdvNum()>0) //非简略模式即显示转生。
                {
                    sprintf(buffer, ADV_LABEL_FORMAT, temp->getAdvNum());
                    info.advNumLabel = buffer;
                }

                //名称
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    sprintf(buffer, "%s", temp->getEquipName()->getCString());
                    info.nameLabel = buffer;
                }
            }
        }
            break;
        case BIT_PROP://道具
        {
            SGPropsDataModel *temp = sgsdm->getPropsById(itemid);
            sgmm->addHeadIconPlistByNum(temp->getIconId(),laTag);
            
            //本尊
            sprintf(buffer, "head%d.png", temp->getIconId());
            info.iconImage = buffer;
            
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                //边框
                if (temp->getPropsPropsType() == 3 || temp->getPropsPropsType() == 4 || temp->getPropsPropsType() == 5) //如果是兵魂卡或者是技能卷轴？以前有特殊处理？
                {
                    int starLvl = temp->getPropsCurrStarLevel();
                    info.quality = starLvl;
                    if (starLvl > 1)
                    {
                        starLvl -= 1;
                    }
                    sprintf(buffer, "equipPinzhi_%d_0.png", starLvl);
                }
                else //与if是一样的处理，暂保留。
                {
                    int starLvl = temp->getPropsCurrStarLevel();
                    info.quality = starLvl;
                    if (starLvl > 1)
                    {
                        starLvl -= 1;
                    }
                    sprintf(buffer, "equipPinzhi_%d_0.png", starLvl);
                }
                
                info.frameImage = buffer;
                
                //名称
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    sprintf(buffer, "%s", temp->getPropsName()->getCString());
                    info.nameLabel = buffer;
                }
            }
        }
            break;
        case BIT_PIECE://碎片
        {
            //碎片遮罩的资源，现场绑定，略坑。
            ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, laTag);
            
            SGPiecesDataModel *temp = sgsdm->getPiecesDictById(itemid);
            if (temp->getCallingCardId() == -1) //通用碎片（废弃）
            {
                /*
                 str_kuang = CCString::createWithFormat("common_pieces_border.png");
                 */
            }
            else if (temp->getPiecesType() == 1) //装备碎片（废弃）
            {
                /*
                str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",starLv);
                
                SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId());
                SGMainManager::shareMain()->addHeadIconPlistByNum(eq->getIconId(),laTag);
                
                CCSprite * spritePlayerHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",eq->getIconId())->getCString());
                card->addChild(spritePlayerHead);
                spritePlayerHead->setPosition(ccp(card->getContentSize().width/2,card->getContentSize().height/2));
                 */
            }
            else if (temp->getPiecesType() == 0) //武将碎片
            {
                SGOfficerDataModel *odm = sgsdm->getOfficerById(temp->getCallingCardId());
                sgmm->addHeadIconPlistByNum(odm->getIconId(),laTag);
                
                //本尊
                sprintf(buffer, "head%d.png", odm->getIconId());
                info.iconImage = buffer;
                
                if (iconType > SGIT_PURE) //非纯粹模式才有其他。
                {
                    //边框。改为采用碎片表中的星级，原为采用召唤的武将星级。
                    int starlvl = temp->getStarLevel();
//                    int starlvl = odm->getOfficerCurrStarLevel();
                    info.quality = starlvl;
                    if (starlvl>1)
                    {
                        starlvl-=1;
                    }
                    sprintf(buffer, "officerPinzhi_%d_0.png",starlvl);
                    info.frameImage = buffer;
                    
                    //国家从属
                    sprintf(buffer, "countryName%d.png", odm->getOfficerRace());
                    info.nationImage = buffer;
                    
                    //转生等级
                    if (iconType > SGIT_SIMPLE && odm->getAdvNum()>0) //非简略模式即显示转生。
                    {
                        sprintf(buffer, ADV_LABEL_FORMAT, odm->getAdvNum());
                        info.advNumLabel = buffer;
                    }
                }
            }
            
            //碎片遮罩
            info.maskImage = "pieces_mask.png";
            
            //名称
            if (iconType >= SGIT_DETAILED) //详情显示名称。
            {
                sprintf(buffer, "%s", temp->getPiecesName()->getCString());
                info.nameLabel = buffer;
            }
        }
            break;
        case BIT_CONSUMABLE://消耗品
        {
            SGConsumableDataModel *temp = sgsdm->getConsumableById(itemid);
            sgmm->addHeadIconPlistByNum(temp->getIconId(), laTag);
            
            //本尊
            sprintf(buffer, "head%d.png", temp->getIconId());
            info.iconImage = buffer;
            
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                //边框
                int starLvl = temp->getConsumeStarLvl();
                info.quality = starLvl;
                if (starLvl > 1)
                {
                    starLvl -= 1;
                }
                sprintf(buffer, "equipPinzhi_%d_0.png",starLvl);
                info.frameImage = buffer;
                
                //名称
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    sprintf(buffer, "%s", temp->getConsumeName()->getCString());
                    info.nameLabel = buffer;
                }
            }
        }
            break;
        case BIT_MATERIAL: //装备转生材料
        {
            SGMaterialDataModel* temp = sgsdm->getMaterialById(itemid);
            sgmm->addHeadIconPlistByNum(temp->getMaterialIconId(), laTag);
            
            //本尊
            sprintf(buffer, "head%d.png", temp->getMaterialIconId());
            info.iconImage = buffer;
            
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                //边框
                int starLvl = temp->getMaterialBorderColor();
                info.quality = starLvl;
                if (starLvl > 1)
                {
                    starLvl -= 1;
                }
                sprintf(buffer, "equipPinzhi_%d_0.png",starLvl);
                info.frameImage = buffer;

                //名称
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    sprintf(buffer, "%s", temp->getMaterialName()->getCString());
                    info.nameLabel = buffer;
                }
            }
        }
            break;
            //原有的货币:
        case BIT_INGOT://元宝
        {
            info.iconImage = "big_yb.png";
            info.quality = 5;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_4_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_yuanbao;
                }
            }
        }
            break;
        case BIT_GOLD://铜钱
        {
            sgmm->addHeadIconPlistByNum(9999,laTag);
            info.iconImage = "head9999.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_Tong_money;
                }
            }

        }
            break;
        case BIT_RECIPE://炼丹秘方
        {
            sgmm->addHeadIconPlistByNum(9996,laTag);
            info.iconImage = "head9996.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_liandianmifang;
                }
            }
        }
            break;
            //新增的货币:
        case BIT_SOUL: //军魂
        {
            info.iconImage = "soul_icon.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_FriendMailLayer_str8;
                }
            }
        }
            break;
        case BIT_VIGOUR: //体力
        {
            info.iconImage = "mail_streng.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_CantAdvance_tips64;
                }
            }
        }
            break;
        case BIT_GRAIN: //军粮
        {
            info.iconImage = "mail_food.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_CantAdvance_tips65;
                }
            }
        }
            break;
        case BIT_GRADE: //PVP积分（天梯）
        {
            info.iconImage = "store_tianti_big.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_FriendMailLayer_str9;
                }
            }
        }
            break;
        case BIT_MEDAL: //勋章
        {
            info.iconImage = "tongqueXunBig.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_Format_xunzhang__;
                }
            }
        }
            break;
        case BIT_JADE: //勾玉
        {
            info.iconImage = "big_gouyu.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_chinese_jade;
                }
            }
        }
            break;
        case BIT_ARENA_SCORE: //征服点
        {
            info.iconImage = "arean_coin_big.png";
            info.quality = 4;
            if (iconType > SGIT_PURE) //非纯粹模式才有其他。
            {
                info.frameImage = "equipPinzhi_3_0.png";
                if (iconType >= SGIT_DETAILED) //详情显示名称。
                {
                    info.nameLabel = str_arena_score;
                }
            }
        }
            break;
            //已废弃的物品类型，如果还有推送的话，则代表必须处理的错误。
        case BIT_FEAT://军功
        case BIT_HORN://小喇叭
        default:
            CCAssert(1 == 0, "BAD ITEM TYPE!");
            break;
    }
    
    //数量统一处理
    if (iconType >= SGIT_COMPLEX && itemNum >= NUM_TO_SHOW_NUM_LABEL) //复杂和详情显示数量。
    {
        sprintf(buffer, NUM_LABEL_FORMAT, itemNum);
        info.amountLabel = buffer;
    }
}

//计算商品相关信息，并且绑定资源。
void SGDrawCards::figureAndBindInGoods(int priceType, int priceValue, LayerTag laTag, SGIconInfo& info, bool havePriceBg)
{
    SGBaseItemType pTp = (SGBaseItemType) priceType;
    switch (pTp)
    {
        case BIT_INGOT://元宝
        {
            info.priceImage = "store_yb.png";
            info.priceQuality = 5;
        }
            break;
        case BIT_GOLD://铜钱
        {
            info.priceImage = "coinicon.png";
            info.priceQuality = 4;
        }
            break;
            //新增的货币:
        case BIT_SOUL: //军魂
        {
            info.priceImage = "store_junhun.png";
            info.priceQuality = 4;
        }
            break;
        case BIT_GRADE: //PVP积分（天梯）
        {
            info.priceImage = "store_tianti.png";
            info.priceQuality = 4;
        }
            break;
        case BIT_JADE: //勾玉
        {
            info.priceImage = "gouyuIcon.png";
            info.priceQuality = 4;
        }
            break;
        case BIT_ARENA_SCORE: //征服点
        {
            info.priceImage = "arean_coin_small.png";
            info.priceQuality = 4;
        }
            break;
        //这些不是货币！
        case BIT_MEDAL: //勋章
        case BIT_GRAIN: //军粮
        case BIT_VIGOUR: //体力
        case BIT_RECIPE://炼丹秘方
        default:
            break;
    }
    
    //价格统一处理
    sprintf(buffer, PRICE_LABEL_FORMAT, priceValue);
    info.priceLabel = buffer;
    
    info.havePriceBg = havePriceBg;
}

//产生图标
CCSprite* SGDrawCards::generateSprite(SGIconInfo& info)
{
    //背景底框.
    CCSprite* card = CCSprite::createWithSpriteFrameName("card_bj.png");
    
    //中心位置.
    CCPoint cPos = ccp(card->getContentSize().width * 0.5, card->getContentSize().height * 0.5);
    
    //图标本尊
    CCSprite* mainSprite = NULL;
    if (info.iconImage.size() > 0)
    {
        mainSprite = CCSprite::createWithSpriteFrameName(info.iconImage.c_str());
        mainSprite->setPosition(cPos);
        card->addChild(mainSprite);
    }
    
    CCAssert(mainSprite != NULL, "GENERATE ICON SPRITE ERROR!");
    
    //边框。
    if (info.frameImage.size() > 0)
    {
        CCSprite* fiSpt = CCSprite::createWithSpriteFrameName(info.frameImage.c_str());
        fiSpt->setPosition(cPos);
        card->addChild(fiSpt);
    }
    
    //国家从属。
    if (info.nationImage.size() > 0)
    {
        CCSprite* niSpt = CCSprite::createWithSpriteFrameName(info.nationImage.c_str());
        niSpt->setPosition(ccpAdd(cPos, ccp(-mainSprite->getContentSize().height*0.5 + niSpt->getContentSize().width*0.5, mainSprite->getContentSize().height * 0.5 - niSpt->getContentSize().height * 0.5)));
        card->addChild(niSpt, 50);
    }
    
    //碎片遮罩。
    if (info.maskImage.size() > 0)
    {
        CCSprite* miSpt = CCSprite::createWithSpriteFrameName(info.maskImage.c_str());
        miSpt->setPosition(cPos);
        card->addChild(miSpt);
    }
    
    //右上角，转生等级。
    if (info.advNumLabel.size() > 0)
    {
        SGCCLabelTTF* anlLabel = SGCCLabelTTF::create(info.advNumLabel.c_str(), FONT_PANGWA, 20, ccGREEN);
        anlLabel->setAnchorPoint(ccp(1,1));
//        anlLabel->ignoreAnchorPointForPosition(false);
        anlLabel->setPosition(ccp(card->getContentSize().width, card->getContentSize().height + 2));
        card->addChild(anlLabel, 50, 250);
    }
    
    //正下方，数量。
    if (info.amountLabel.size() > 0)
    {
        SGCCLabelTTF* alLabel = SGCCLabelTTF::create(info.amountLabel.c_str(), FONT_PANGWA, 18, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, ccWHITE/*ccc3(29, 225, 34)*/);
        alLabel->setZOrder(20);
        alLabel->getMainLabel()->setCascadeColorEnabled(true);
        
        alLabel->setAnchorPoint(ccp(0.5, 0.5));
//        alLabel->setPosition(ccpAdd(cPos, ccp(mainSprite->getContentSize().width * 0.5 - alLabel->getContentSize().width * 0.5, -mainSprite->getContentSize().height * 0.5 + 12)));
        alLabel->setPosition(ccpAdd(cPos, ccp(0, -card->getContentSize().height * 0.5 + 12)));
        card->addChild(alLabel, 50);
    }
    
    //下框外，名称和名称底板。
    if (info.nameLabel.size() > 0)
    {
        SGCCLabelTTF* nlLabel = SGCCLabelTTF::create(info.nameLabel.c_str(), FONT_PANGWA, 20, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, SGTools::getColorByQuality(info.quality));
        nlLabel->setAnchorPoint(ccp(0.5, 0.5));
//        nlLabel->setPosition(ccpAdd(cPos, ccp(-nlLabel->getContentSize().width * 0.5 + 5, -card->getContentSize().height * 0.5 - 24)));
        nlLabel->setPosition(ccpAdd(cPos, ccp(0, -card->getContentSize().height * 0.5 - 20)));
        card->addChild(nlLabel);
        
        if (info.haveNameBg)
        {
            CCSprite *namebg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
            namebg->setPosition(nlLabel->getPosition());
            namebg->setScaleX(1.1);
            namebg->setScaleY(1.05);
            card->addChild(namebg,-1);
        }
    }
    //下框外，价格和价格底板。
    else if (info.priceImage.size() > 0 && info.priceLabel.size() > 0)
    {
        CCSprite* piSpt = CCSprite::createWithSpriteFrameName(info.priceImage.c_str());
        piSpt->setPosition(ccpAdd(cPos, ccp(-46, -card->getContentSize().height * 0.5 - 20)));
        card->addChild(piSpt);
        
        //价格都用白色。
        SGCCLabelTTF* plLabel = SGCCLabelTTF::create(info.priceLabel.c_str(), FONT_PANGWA, 20, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter, ccWHITE);
        plLabel->setAnchorPoint(ccp(0.5, 0.5));
//        nlLabel->setPosition(ccpAdd(cPos, ccp(-nlLabel->getContentSize().width * 0.5 + 5, -card->getContentSize().height * 0.5 - 24)));
//        plLabel->setPosition(ccpAdd(cPos, ccp(-24, -card->getContentSize().height * 0.5 - 18)));
//        plLabel->setPosition(ccp(piSpt->getPosition().x + piSpt->getContentSize().width * 0.5 + plLabel->getContentSize().width * 0.5 + 4, piSpt->getPosition().y));
        plLabel->setPosition(ccpAdd(cPos, ccp(13, -card->getContentSize().height * 0.5 - 20)));
        card->addChild(plLabel);
        
        if (info.havePriceBg)
        {
            CCSprite *priceBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
            priceBg->setPosition(ccpAdd(cPos, ccp(0, -card->getContentSize().height * 0.5 - 20)));
            priceBg->setScaleX(1.1);
            priceBg->setScaleY(1.05);
            card->addChild(priceBg,-1);
        }
    }
    
    return card;
}

