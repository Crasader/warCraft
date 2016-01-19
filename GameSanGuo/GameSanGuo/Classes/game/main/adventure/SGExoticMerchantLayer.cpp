//
//  SGExoticMerchantLayer.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 12/26/14.
//
//

#include "SGExoticMerchantLayer.h"
#include "ResourceManager.h"
#include "SGTools.h"
#include "cocos-ext.h"
#include "SGShowString.h"
#include "SGDrawCards.h"
#include "SGRichLabel.h"
#include "SGAdventureLayer.h"
#include "SGMainManager.h"

//倒计时缓冲区
static char clockBuffer[20];

//ctor
SGExoticMerchantLayer::SGExoticMerchantLayer() : data_(NULL), timeLeft_(0), clockLabel_(NULL), lastBuyIndex_(0)
{
    for (int k = 0; k < MAX_GOODS_NUM; k++)
    {
        buyBtnArray_[k] = NULL;
        maskLayerArray_[k] = NULL;
    }
    
}

//dtor
SGExoticMerchantLayer::~SGExoticMerchantLayer()
{
    CC_SAFE_DELETE(data_);
    SGNotificationCenter* nc = SGNotificationCenter::sharedNotificationCenter();
    nc->removeObserver(this, MSG_EXO_MECH_BUY_GOODS);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_exoticMerchantLayer);
}

//create
SGExoticMerchantLayer* SGExoticMerchantLayer::create(main::XiyuInfoResponse* data)
{
    SGExoticMerchantLayer* ret = new SGExoticMerchantLayer();
    if (ret && ret->init(NULL, sg_exoticMerchantLayer))
    {
        ret->data_ = new main::XiyuInfoResponse();
        ret->data_->CopyFrom(*data);
        ret->initView();
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

//iv
bool SGExoticMerchantLayer::initView()
{
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("sgexoticmerchantlayer/sgexoticmerchantlayer.plist", RES_TYPE_LAYER_UI, sg_exoticMerchantLayer);
    rm->bindTexture("sgmonthlysignlayer/sgmonthlysignlayer.plist", RES_TYPE_LAYER_UI, sg_exoticMerchantLayer);
    rm->bindTexture("sgmalllayer/sgmalllayer.plist", RES_TYPE_LAYER_UI, sg_exoticMerchantLayer);
    
    SGNotificationCenter* nc = SGNotificationCenter::sharedNotificationCenter();
    nc->addObserver(MSG_EXO_MECH_BUY_GOODS, this, callfuncO_selector(SGExoticMerchantLayer::buyGoodsResponse));
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    //主题原画，顶距190，底距170。
    CCSprite* paint = CCSprite::createWithSpriteFrameName("sgem_painting.png");
//    paint->setAnchorPoint(ccp(0,0));
//    paint->setPosition(ccp(s.width * 0.5 - paint->getContentSize().width * 0.5, 170 + (s.height - paint->getContentSize().height) * 0.5));
    paint->setPosition(ccp(s.width * 0.5, s.height * 0.5 + 14));
    this->addChild(paint);
    
    //保持menu位置一致
    menu->setAnchorPoint(ccp(0.5,0.5));
    menu->ignoreAnchorPointForPosition(false);
    menu->setContentSize(paint->getContentSize());
    menu->setPosition(paint->getPosition());
    
    //为添加角标考虑。
    CCLayer* markLayer = CCLayer::create();
    markLayer->ignoreAnchorPointForPosition(false);
    markLayer->setAnchorPoint(ccp(0.5,0.5));
    markLayer->setContentSize(paint->getContentSize());
    markLayer->setPosition(paint->getPosition());
    this->addChild(markLayer, 4);
    
    //位置
//    this->ignoreAnchorPointForPosition(false);
//    this->setContentSize(paint->getContentSize());
//    this->setAnchorPoint(ccp(0.5,0.5));
//    this->setPosition(ccp(s.width * 0.5, 170 + paint->getContentSize().height * 0.5 + (s.height - 960) * 0.5 ));
    
    //标题和其他元素
    CCSprite* title = CCSprite::createWithSpriteFrameName("sgem_title.png");
    SGCCLabelTTF* info = NULL;
    
    const char* camel = "sgem_camel.png";
    const char* cloud = "sgem_cloud.png";
    
    //内容zd
    const int ctZd = 4;
    //底框zd
    const int diZd = 2;
    //按钮zd
    const int btnZd = 6;
    
    if (data_->state()) //开启
    {
        title->setPosition(ccp(177, 499));
        
        //提示文本
        const char* str = NULL;
        
        if (SGPlayerInfo::sharePlayerInfo()->getShowvip())
        {
            if (SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() > 0)
            {
                str = str_exomech_1;
            }
            else
            {
                str = str_exomech_2;
            }
        }
        else //VIP未开启，统一提示。
        {
            str = str_exomech_3;
        }
        
        info = SGCCLabelTTF::create(str, FONT_PANGWA, 17, CCSizeMake(485, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, ccGREEN);
        info->setPosition(ccp(293, 236));
        
        //骆驼左
        CCSprite* cmSpt = CCSprite::createWithSpriteFrameName(camel);
        cmSpt->setPosition(ccp(92, 503));
        paint->addChild(cmSpt, ctZd);
        
        //倒计时文本
        SGCCLabelTTF* timeLb = SGCCLabelTTF::create(str_exomech_5, FONT_PANGWA, 22);
        timeLb->setPosition(ccp(340, 500));
        paint->addChild(timeLb, ctZd);
        
        //倒计时计时
        clockLabel_ = SGCCLabelTTF::create("", FONT_PANGWA, 22, CCSizeMake(136, 24), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter, ccGREEN);
        clockLabel_->setPosition(ccp(440, 500));
        paint->addChild(clockLabel_, ctZd);
        
        timeLeft_ = data_->time();
        this->schedule(schedule_selector(SGExoticMerchantLayer::updateClock), 1.0);
    }
    else //关闭
    {
        title->setPosition(ccp(303, 499));
        
        //提示文本
        info = SGCCLabelTTF::create(str_exomech_3, FONT_PANGWA, 17, CCSizeMake(485, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, ccGREEN);
        info->setPosition(ccp(293, 236));
        
        //骆驼左
        CCSprite* cm1 = CCSprite::createWithSpriteFrameName(camel);
        cm1->setPosition(ccp(92, 503));
        paint->addChild(cm1, ctZd);
        
        //怒风左
        CCSprite* cd1 = CCSprite::createWithSpriteFrameName(cloud);
        cd1->setFlipX(true);
        cd1->setPosition(ccp(cm1->getPosition().x + 105, cm1->getPosition().y));
        paint->addChild(cd1, ctZd);
        
        //怒风右
        CCSprite* cd2 = CCSprite::createWithSpriteFrameName(cloud);
        cd2->setPosition(ccp(cd1->getPosition().x + 208, cd1->getPosition().y));
        paint->addChild(cd2, ctZd);
        
        //骆驼右
        CCSprite* cm2 = CCSprite::createWithSpriteFrameName(camel);
        cm2->setFlipX(true);
        cm2->setPosition(ccp(cd2->getPosition().x + 105, cd2->getPosition().y));
        paint->addChild(cm2, ctZd);
    }
    
    paint->addChild(title, ctZd);
    paint->addChild(info, diZd);
    
    //对话框
    CCSprite* dlg = CCSprite::createWithSpriteFrameName("sgavn_dialog.png");
    dlg->setPosition(ccp(442, 383));
    paint->addChild(dlg, ctZd);
    
    //对话框文本
    SGCCLabelTTF* dlgLb = SGCCLabelTTF::create(str_exomech_4, FONT_FZXK, 22, CCSizeMake(204, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, ccBLACK, ccBLACK, 0, false);
    dlgLb->setPosition(ccp(150, 64));
    dlg->addChild(dlgLb, ctZd);
    
    //底框。
    CCScale9Sprite* frame = CCScale9Sprite::createWithSpriteFrameName("sgavn_frame.png", CCRectMake(20, 20, 160, 160));
    frame->setAnchorPoint(ccp(0.5,0.5));
    frame->setContentSize(CCSizeMake(590, 260));
    frame->setPosition(ccp(info->getPosition().x, info->getPosition().y - 130 - 32));
    paint->addChild(frame, 2);
    
    //底框内容。
    
    if (data_->state()) //商品列表
    {
        //單元之间的间隔
        const int dist = 140;
        //修复元素Y偏移
        const float yfix = -59;
        
        //是否有未购买的商品。
        bool canBuy = false;
        
        for (int k = 0; k < data_->goods_size(); k++)
        {
            if (k >= MAX_GOODS_NUM)
            {
                break;
            }
            
            //背板
            CCScale9Sprite* tray = CCScale9Sprite::createWithSpriteFrameName("ms_slot_gotten.png", CCRectMake(20, 20, 102, 102));
            tray->setContentSize(CCSizeMake(138, 220));
            tray->setPosition(ccp(83 + k * dist, 138 + yfix));
            paint->addChild(tray, btnZd);
            
            if (data_->goods(k).mark().size() > 0)
            {
                //角标
                CCSprite* mark = CCSprite::createWithSpriteFrameName("ms_corner_mask.png");
                mark->setPosition(ccp(52 + k * dist, 210 + yfix));
                markLayer->addChild(mark, btnZd + 2);
                
                //角标文本
                SGCCLabelTTF* markLabel = SGCCLabelTTF::create(data_->goods(k).mark().c_str(), FONT_PANGWA, 14, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, ccc3(255,236,2), ccWHITE, 0.0, false);
                markLabel->setAnchorPoint(ccp(0.5,0.5));
                markLabel->setPosition(ccpAdd(mark->getPosition(), ccp(-11, 11)));
                markLabel->setRotation(-45);
                markLayer->addChild(markLabel, btnZd + 4);
            }
            
            //商品
            SGButton* goods = SGDrawCards::createButtonInGoods(data_->goods(k), sg_exoticMerchantLayer, SGIT_GOODS, this, menu_selector(SGExoticMerchantLayer::viewGoods));
            goods->setTag(k);
            goods->setPosition(83 + k * dist, 172 + yfix);
            menu->addChild(goods);
            
            //购买
            SGButton* buy = SGButton::createFromLocal("advance_manzubg.png", str_GoldShopGoodsItem_str3, this, menu_selector(SGExoticMerchantLayer::buyGoods), CCPointZero, FONT_BOXINFO, ccWHITE);
            buy->setTag(k);
            buy->setPosition(ccp(83 + k * dist, 60 + yfix));
            menu->addChild(buy);
            
            buyBtnArray_[k] = buy;
            
            //售罄遮罩。
            CCLayerColor* maskLa = CCLayerColor::create(ccc4(44, 20, 0, 180), tray->getContentSize().width, tray->getContentSize().height);
            maskLa->ignoreAnchorPointForPosition(false);
            maskLa->setPosition(tray->getPosition());
            markLayer->addChild(maskLa, btnZd + 6);
            
            //售罄。
            CCSprite* selot = CCSprite::createWithSpriteFrameName("sellOut.png");
            selot->setPosition(ccp(maskLa->getContentSize().width * 0.5, maskLa->getContentSize().height * 0.5));
            maskLa->addChild(selot, 2);
            
            maskLayerArray_[k] = maskLa;
            
            //判定显隐
            maskLa->setVisible(data_->goods(k).state() ? false : true);
            buy->setEnabled(data_->goods(k).state() ? true : false);
            
            //判定购买
            if (data_->goods(k).state())
            {
                canBuy = true;
            }
        }
        //红点相关。
        this->setNoticeTime(timeLeft_);
        this->setIsForcedOpen(canBuy);
    }
    else //提示文字
    {
        //大框 142x142
        CCScale9Sprite* smf = CCScale9Sprite::createWithSpriteFrameName("ms_slot_gotten.png", CCRectMake(20, 20, 102, 102));
        smf->setContentSize(CCSizeMake(540, 194));
        smf->setPosition(frame->getPosition());
        paint->addChild(smf, btnZd);
        
        //未开启的提示。
        CCSprite* text = SGRichLabel::getFomLabel(str_exomech_6, CCSizeMake(480, 160), FONT_PANGWA, 23);
        text->setAnchorPoint(ccp(0.5, 0.5));
        text->setPosition(ccp(frame->getPosition().x , frame->getPosition().y - 20));
        paint->addChild(text, btnZd + 2);
        
//        //文本
//        SGCCLabelTTF* text = SGCCLabelTTF::create("主公在闖關模式（包括精英闖關）勝利後，會隨機遇到這個商人。", FONT_PANGWA, 24, CCSizeMake(470, 160), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        text->setPosition(frame->getPosition());
//        paint->addChild(text, 2);
        
        //红点相关。
        this->setNoticeTime(0);
        this->setIsForcedOpen(false);
    }
    
    return true;
}

//upd
void SGExoticMerchantLayer::updateClock(float dt)
{
    SGTools::calClockString(timeLeft_--, clockBuffer);
    clockLabel_->setString(clockBuffer); //此时clockLabel不可能为空。
}

//查看详情。
void SGExoticMerchantLayer::viewGoods(CCObject* obj)
{
    SGButton* btn = dynamic_cast<SGButton*>(obj);
    if (btn)
    {
        const main::ItemDataProto& item = data_->goods(btn->getTag()).item();
        SGMainManager::shareMain()->showCommonInfoBoxInProto((SGBaseItemType)item.itemtype(), item.itemid(), this, -1, true, CIBET_ALL_TASK_LAYER);
    }
}

//购买物品。
void SGExoticMerchantLayer::buyGoods(CCObject* obj)
{
    SGButton* btn = dynamic_cast<SGButton*>(obj);
    if (btn)
    {
        lastBuyIndex_ = btn->getTag();
        main::XiyuBuyRequest* req = new main::XiyuBuyRequest();
        req->set_index(lastBuyIndex_);
        SGSocketClient::sharedSocketClient()->send(MSG_EXO_MECH_BUY_GOODS, req);
    }
}

//修改触控。
void SGExoticMerchantLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
    
    SGAdventureLayer* hehe = dynamic_cast<SGAdventureLayer*>(this->getParent());
    hehe->setIsCanTouch(isTouch);
}

//购买物品resp。
void SGExoticMerchantLayer::buyGoodsResponse(CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if (sr)
    {
        main::XiyuBuyResponse *resp = dynamic_cast<main::XiyuBuyResponse*>(sr->m_msg);
        if (resp)
        {
            if (resp->state() > 0) //领取成功。
            {
                if (lastBuyIndex_ >= 0 && lastBuyIndex_ < MAX_GOODS_NUM && buyBtnArray_[lastBuyIndex_])
                {
                    maskLayerArray_[lastBuyIndex_]->setVisible(true);
                    buyBtnArray_[lastBuyIndex_]->setEnabled(false);
                }
                SGMainManager::shareMain()->showMessage(str_MallLayer_str6);
            }
            else
            {
                SGMainManager::shareMain()->showMessage(resp->hint().c_str());
            }
            
            //红点相关。
            bool canBuy = false;
            for (int k = 0; k < MAX_GOODS_NUM; k++)
            {
                if (maskLayerArray_[k] && !maskLayerArray_[k]->isVisible()) //有任意一个可购买，即标记红点。
                {
                    canBuy = true;
                    break;
                }
            }
            this->setIsForcedOpen(canBuy);
            
            //MM: HACK触发一下红点逻辑。
            SGAdventureLayer* al = dynamic_cast<SGAdventureLayer*>(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_adventureLayer));
            if (al)
            {
                al->handleNotices();
            }
        }
    }
}

/* --红点相关-- */

static int noticeTime = 0;

static bool isForcedOpen = true;

void SGExoticMerchantLayer::setNoticeTime(int time)
{
    noticeTime = time;
}

int& SGExoticMerchantLayer::getNoticeTime()
{
    return noticeTime;
}

void SGExoticMerchantLayer::setIsForcedOpen(bool bl)
{
    isForcedOpen = bl;
}

bool SGExoticMerchantLayer::getIsForcedOpen()
{
    return isForcedOpen;
}

bool SGExoticMerchantLayer::getNoticeState()
{
    
}

/* --红点相关-- */

