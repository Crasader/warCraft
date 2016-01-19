//
//  SGMonthlySignLayer.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 12/16/14.
//
//

#include "SGMonthlySignLayer.h"
#include "SGMonthlySignItem.h"
#include "ResourceManager.h"
#include "SGShowString.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGCantAdvanceBox.h"
#include "SGShowRewardBox.h"

//本table列数。
const static int MSL_COLUMN_SIZE = 4;

//ctor
SGMonthlySignLayer::SGMonthlySignLayer() : sdrData_(NULL), selectedIdx_(-1), statLabel_(NULL)
{
    ;
}

//dtor
SGMonthlySignLayer::~SGMonthlySignLayer()
{
    CC_SAFE_DELETE(sdrData_);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MONTHLY_SIGN_GET_REWARD);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_monthlySignLayer);
}

//static create method
SGMonthlySignLayer* SGMonthlySignLayer::create(main::SigninDetailResponse* sdrData)
{
    SGMonthlySignLayer *ret = new SGMonthlySignLayer();
    if (ret && ret->init(NULL, sg_monthlySignLayer))
    {
        ret->sdrData_ = sdrData;
        ret->initView();
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//view
void SGMonthlySignLayer::initView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MONTHLY_SIGN_GET_REWARD, this, callfuncO_selector(SGMonthlySignLayer::getRewardResponse));
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("sgmonthlysignlayer/sgmonthlysignlayer.plist", RES_TYPE_LAYER_UI, sg_monthlySignLayer);
    rm->bindTexture("animationFile/ms_niubility.plist", RES_TYPE_LAYER_UI, sg_monthlySignLayer);
    
    
    //通用元素。
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSize winSz = CCDirector::sharedDirector()->getWinSize();
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGMonthlySignLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);

    //标题文本
    char buffer[80];
    sprintf(buffer, str_monthly_sign_str2, sdrData_->month());
    SGCCLabelTTF *title = SGCCLabelTTF::create(buffer, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    this->addChild(title);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65*.48)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));
    
    //依据960x640适配，所有的宽高全部按当前设备宽高进行加减。
    CCSize oriSize = CCSizeMake(640, 960);
    
    //淺色第一層背板
    CCScale9Sprite* bkg1 = CCScale9Sprite::createWithSpriteFrameName("ms_bkg.png", CCRectMake(25, 25, 256, 192)); //306x242
    bkg1->setContentSize(CCSizeMake(winSz.width - (oriSize.width - 614), winSz.height - (oriSize.height - 494))); //614x494
    bkg1->setPosition(ccp(winSz.width * 0.5, winSz.height * 0.5 - 66));
    this->addChild(bkg1);
    
    //深色第二層背板
    CCScale9Sprite* bkg2 = CCScale9Sprite::createWithSpriteFrameName("ms_frame.png", CCRectMake(5, 5, 286, 204)); //296x214
    bkg2->setContentSize(CCSizeMake(winSz.width - (oriSize.width -592), winSz.height - (oriSize.height - 430))); //592x430
    bkg2->setPosition(ccpAdd(bkg1->getPosition(), ccp(0, 10)));
    this->addChild(bkg2, 1);
    
    //统计文本
    sprintf(buffer, str_monthly_sign_str3, sdrData_->day());
    statLabel_ = SGCCLabelTTF::create(buffer, FONT_XINGKAI, 26 , CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, ccWHITE);
    statLabel_->setAnchorPoint(ccp(0.5, 0.5));
    statLabel_->setPosition(ccpAdd(bkg2->getPosition(), ccp(0, -bkg2->getContentSize().height * 0.5 - statLabel_->getContentSize().height * 0.5)));
    this->addChild(statLabel_, 3);
    
    //滑动列表 586x426
    const int tableWidth = bkg2->getContentSize().width - 6;
    const int tableHeight = bkg2->getContentSize().height - 4;
    this->tableViewHeight = 146;
    tableView->setZOrder(3);
    tableView->setFrame(CCRect(0, 0, tableWidth, tableHeight));
    tableView->setPosition(ccp((winSz.width - tableWidth) * 0.5, bkg2->getPosition().y - tableHeight * 0.5));
//    tableView->setPosition(ccp(0.5f * (winSz.width-580) , 0.5 * winSz.height - 245.0f * winSz.height / 960));
    tableView->setHorizontal(false);
    tableView->setDown(5);
}

//返回按钮
void SGMonthlySignLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}

//根据indexPath返回tableView的cellItem
SNSTableViewCellItem* SGMonthlySignLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    
    if (num < 0 || num >= sdrData_->cells_size())
        return NULL;
    
    CCLOG("SGMonthlySignLayer::tableViewItemForCell index = %d", num);
    
    main::SigninCell sc = sdrData_->cells(num);
    SGMonthlySignItem *item = (SGMonthlySignItem*) tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
	if (NULL == item)
    {
		item = SGMonthlySignItem::create(&sc);
    }
    else
    {
        item->updateItem(&sc);
    }
    return item;
}

//返回tableView的列数
unsigned int SGMonthlySignLayer::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
    return MSL_COLUMN_SIZE;
}

// 返回tableView的cell数
unsigned int SGMonthlySignLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return this->sdrData_->cells_size();
}

//点击某个item。
void SGMonthlySignLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    selectedIdx_ = tableView->getItemPointerWithIndexPath(indexPath);
    
    if (selectedIdx_ < 0 || selectedIdx_ >= sdrData_->cells_size())
    {
        return;
    }
    
//    CCLOG("MonthlySign try select index = %d", selectedIdx_);
    
    main::SigninCell* scData_ = const_cast<main::SigninCell*>(&(sdrData_->cells(selectedIdx_)));
    
    //已领取的点击无反应。
    if (main::NORMAL_HAS == scData_->state())
    {
        return;
    }
    
    //弹框系列>>
    
    //未领取且不可领取的有如下两个规则。
    
    //1.普通的弹框，“累计X天获得”。
    
    //2.VIP限制的弹框，追加“升级到VIP X可领取”，注意苹果审核的屏蔽。
    
    if (main::NORMAL_NO == scData_->state())
    {
        SGBaseMilitaryCard* bmc = SGStaticDataManager::shareStatic()->getOneBmcByTypeAndId(scData_->itemdata().itemtype(), scData_->itemdata().itemid(), scData_->itemdata().itemnum());
        if (bmc)
        {
            SGCommonInfoBox* cib = NULL;
            if (scData_->vipdesc().size() > 0)
            {
                cib = SGCommonInfoBox::create(this, bmc, SGCIBOT_MS_VIP_ITEM, scData_);
            }
            else
            {
                cib = SGCommonInfoBox::create(this, bmc, SGCIBOT_MS_ORDINARY_ITEM, scData_);
            }
            if (cib)
            {
                SGMainManager::shareMain()->showBox(cib);
            }
        }
        return;
    }
    
    
    //领取系列>>
    
    //可领取的直接发消息，不区分普通和VIP。
    if (main::NORMAL_CAN == scData_->state())
    {
        main::SigninRewardRequest* req = new main::SigninRewardRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_MONTHLY_SIGN_GET_REWARD, req);
        return;
    }
    
    //领取一半系列>>
    
    //转其去充值。
    if (main::VIP_HALF == scData_->state() && SGPlayerInfo::sharePlayerInfo()->getShowvip())
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 2014, -1, -1, NULL, -1, scData_->vipdesc().c_str());
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
}

//领取奖励resp
void SGMonthlySignLayer::getRewardResponse(CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if (sr)
    {
        main::SigninRewardResponse *resp = dynamic_cast<main::SigninRewardResponse*>(sr->m_msg);
        if (resp)
        {
            if (resp->rewardstate()) //领取成功。
            {
                if (selectedIdx_ >= 0 || selectedIdx_ < sdrData_->cells_size())
                {
                    //                CCLOG("MonthlySign gr response index = %d", selectedIdx_);
                    int row = selectedIdx_ / MSL_COLUMN_SIZE;
                    int col = selectedIdx_ % MSL_COLUMN_SIZE;
                    
                    SNSIndexPath* ip = SNSIndexPath::create(row, col, 0);
                    SGMonthlySignItem* item = dynamic_cast<SGMonthlySignItem*>(this->tableView->getItemByIndexPath(ip));
                    
                    if (item)
                    {
                        //改变累计签到标签。
                        char buffer[100];
                        sprintf(buffer, str_monthly_sign_str3, resp->day());
                        statLabel_->setString(buffer);
                        //刷新图形。
                        main::SigninCell* sc = const_cast<main::SigninCell*>(&(sdrData_->cells(selectedIdx_)));
                        sc->set_state(resp->cellstate());
                        item->updateItem(sc);
                    }
                    
                    //TODO: 添加奖励，记得修改为通用ICON版。
                    CCArray *consumables = CCArray::create();
                    
                    SGMailSystemAccessoryDataModel *access = new SGMailSystemAccessoryDataModel();
                    access->setItemId(resp->itemdata().itemid());
                    access->setType(resp->itemdata().itemtype());
                    access->setCount(resp->itemdata().itemnum());
                    access->autorelease();
                    
                    consumables->addObject(access);
                    
                    SGShowRewardBox *rewardBox = SGShowRewardBox::create(this, consumables);
                    SGMainManager::shareMain()->showBox(rewardBox);
                }
                
                SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str20);
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str21);
            }
        }
    }
}
