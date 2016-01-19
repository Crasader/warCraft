//SGLootWiKiLayer.cpp

#include "SGLootWiKiLayer.h"
#include "ResourceManager.h"
#include "SGShowString.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGLootWiKiCell.h"
#include "SGLootWiKiMainLayer.h"
#include "FightBaseModule.pb.h"
#include "RoleShopModule.pb.h" //新商城消息
#include "PlotModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGLootWiKiDataManager.h"
#include "SGCantAdvanceBox.h"
#include "SGStoryBean.h"
#include "SGGuideManager.h"



//ctor
SGLootWiKiLayer::SGLootWiKiLayer() :
m_pieceLootInfo(NULL)
, m_selectIndex(-1)
, m_plotId(-1)
, m_lootWikiMainLayer(NULL)
{
    datas = CCArray::create();
    datas->retain();
}

SGLootWiKiLayer::~SGLootWiKiLayer()
{
    datas->release();
    
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CREAM_STORY);
}

//创建对象
//参数：
SGLootWiKiLayer * SGLootWiKiLayer::create(CCArray * pieceLootInfo , SGLootWiKiMainLayer * wikimainlayer ,CCSize size)
{
    SGLootWiKiLayer * ret = new SGLootWiKiLayer();
    
    if (ret && ret->init(NULL,sg_lootWiKiLayer) )
    {
        ret->setContentSize(size);
        ret->initView(pieceLootInfo , wikimainlayer );
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void SGLootWiKiLayer::initView(CCArray * pieceLootInfo , SGLootWiKiMainLayer * lootWiKiMainLayer )
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CREAM_STORY,this,callfuncO_selector(SGLootWiKiLayer::showCream));
    
    m_pieceLootInfo = pieceLootInfo;
    m_lootWikiMainLayer = lootWiKiMainLayer;
    
    datas->addObjectsFromArray(m_pieceLootInfo);
    
    tableView->setFrame(CCRect(0,0, this->getContentSize().width ,this->getContentSize().height + 20));
    tableView->setPosition(ccp(0 , 0));
//    tableView->setPagegate(this);
//    tableView->setPageNEnable(true);
//    tableView->setShowSlider(false);
    tableView->setHorizontal(false);
    //引导中使用，不可改
    tableView->setTag(666);
    tableView->setDown(-25);
    this->tableViewHeight = 160;
    
    //计算开启与否变量。
    figureSourceOpenState();
}


SNSTableViewCellItem* SGLootWiKiLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    
    if (num >= datas->count())
        return NULL;
    SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
    HaveSource * source = ((HaveSource *)datas->objectAtIndex(num));
    SGLootWiKiCell *item = (SGLootWiKiCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
	if (NULL == item)
		item = SGLootWiKiCell::create( source  , this, _sourceStateVec.at(num).first );
    else
        item->updateItem(source, _sourceStateVec.at(num).first);
    if(0 == num  && playerInfo->getPlayerLevel() < playerInfo->getShowPlayerNoticeLimitLev() && source->getSourceType()!=0 /*&& !playerInfo->isGuid()*/)
    {
        int lev = playerInfo->getPlayerLevel();
        int type = source->getSourceType();
        if(
           (type==2 && lev>=playerInfo->getLimitLevelById(limitJinyinFuben )) || (type==3 && lev>=playerInfo->getLimitLevelById(limitPvpBattle) )
           || (type==4 && lev>=playerInfo->getLimitLevelById(limitShouJinZhou)) || (type==5 && lev>=playerInfo->getLimitLevelById(limitBossBattle))
           || (type==6 && lev>=playerInfo->getLimitLevelById(limitXianshiFuben) ) || (type==1 && (playerInfo->getMaxPlotId() + 1) >= source->getSourcePara())
           )
            item->showHandAnimate(true);
        else
            item->showHandAnimate(false);
        
    }
    else
        item->showHandAnimate(false);
    return item;
}



void SGLootWiKiLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    m_selectIndex = tableView->getItemPointerWithIndexPath(indexPath);//选择哪一item
    itemSelect();
}

//判断来源开启与否。
void SGLootWiKiLayer::figureSourceOpenState()
{
    char buffer[128] = "\0";
    for (int k = 0; k < datas->count(); k++)
    {
        bool isOpened = false;
        std::string noticeText = "";
        
        SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
        HaveSource * source = ((HaveSource *)datas->objectAtIndex(k));
        switch(source->getSourceType())
        {
            case 1: //普通闯关
                if ( player->getMaxPlotId() + 1 >= source->getSourcePara() ) //判断是否能直达该关卡
                {
                    isOpened = true;
                }
                else
                {
                    isOpened = false;
                    noticeText = str_lootWiKi_11;
                }
                break;
            case 2: //精英闯关
                if( player->getMaxPlotId() < player->getLimitLevelById(limitJinyinFuben) ) //先判断等级是否够
                {
                    isOpened = false;
                    SGSpotDataModel* spotData = SGStaticDataManager::shareStatic()->getSpotById(player->getLimitLevelById(limitJinyinFuben), true);
                    sprintf(buffer, str_MainLayer_str152,spotData->getSpotSectionIndex());
                    noticeText = buffer;
                }
                else //再判断是否能直达该关卡
                {
                    if ( player->getMaxBossPlotId() + 1 >= source->getSourcePara() )
                    {
                        isOpened = true;
                    }
                    else
                    {
                        isOpened = false;
                        noticeText = str_lootWiKi_11;
                    }
                }
                break;
            case 3: //天梯
                isOpened = player->canBreakLimitById(limitPvpBattle);
                if (!isOpened)
                {
                    sprintf(buffer, str_MainLayer_str14, player->getLimitLevelById(limitPvpBattle));
                    noticeText = buffer;
                }
                break;
            case 4: //守荆州
                isOpened = player->canBreakLimitById(limitShouJinZhou);
                if (!isOpened)
                {
                    sprintf(buffer, str_FirstLayer_str11,player->getLimitLevelById(limitShouJinZhou));
                    noticeText = buffer;
                }
                break;
            case 5: //讨魔
                isOpened = player->canBreakLimitById(limitBossBattle);
                if (!isOpened)
                {
                    sprintf(buffer, str_FirstLayer_str12,player->getLimitLevelById(limitBossBattle));
                    noticeText = buffer;
                }
                break;
            case 6: //副本
                isOpened = player->canBreakLimitById(limitXianshiFuben);
                if (!isOpened)
                {
                    sprintf(buffer, str_MainLayer_str17, player->getLimitLevelById(limitXianshiFuben));
                    noticeText = buffer;
                }
                break;
            case 0:
            default:
                noticeText = str_lootWiKi_3;
                break;
        }
        
        _sourceStateVec.push_back(std::make_pair(isOpened, noticeText));
    }
}


void SGLootWiKiLayer::itemSelect()
{
    //前置判断isOpened，若通过再继续前往逻辑。
    if (!_sourceStateVec.at(m_selectIndex).first)
    {
        SGMainManager::shareMain()->showMessage(_sourceStateVec.at(m_selectIndex).second.c_str());
        return;
    }
    
    //进行到这里则理论上必然能通过检定，如果出现不通过，代表业务有问题。
    HaveSource * source = (HaveSource * )datas->objectAtIndex(m_selectIndex);
    /*0= 未开启 1=普通闯关 2=精英闯关 3=天梯 4=守荆州 5=讨魔 6副本*/
    switch(source->getSourceType())
    {
        case 0:
        {
            SGMainManager::shareMain()->showMessage(str_lootWiKi_3);
            if (SGGuideManager::shareManager()->isGuide)
            {
                SGMainManager::shareMain()->closeBox();
            }
        }
            break;
        case 1:
            onSourceChuangGuanClicked(source->getSourcePara());
            break;
        case 2:
            onJingYingChuangGuan(source->getSourcePara());
            break;
        case 3:
            onSourceTianTiClicked();
            break;
        case 4:
            onSourceShouJingZhouClicked();
            break;
        case 5:
            onSourceTaoMoClicked();
            break;
        case 6:
            onInstance();
            break;
        default:
            if (SGGuideManager::shareManager()->isGuide)
            {
                SGMainManager::shareMain()->closeBox();
            }
            break;
    }
}

void SGLootWiKiLayer::onSourceShouJingZhouClicked()
{
    m_lootWikiMainLayer->boxClose();
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_JING_ZHOU,SHOP_LAYER);
}

void SGLootWiKiLayer::onSourceTaoMoClicked()
{
    //一下代码和sgFirstLayer.cpp中的 bossHandler保持一致。发送 进入讨魔的消息
    SGPlayerInfo *player=SGPlayerInfo::sharePlayerInfo();
    if (!player->canBreakLimitById(limitBossBattle)) {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str12,player->getLimitLevelById(limitBossBattle))->getCString() );
        return;
    }
    if (SGPlayerInfo::sharePlayerInfo()->getIspvp()==true) {
        SG_SHOW_WINDOW(str_FirstLayer_str13);
        return;
    }
    
    m_lootWikiMainLayer->boxClose();
//    main::MainBossRequest *request=new main::MainBossRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->entryTongQueShow();

}

//普通闯关
void SGLootWiKiLayer::onSourceChuangGuanClicked(int plotId)
{
    SGSpotDataModel * spot = SGStaticDataManager::shareStatic()->getSpotById(plotId);
    int lev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if (spot)
    {
        int section = spot->getSpotSectionIndex();
        int index = spot->getSpotSpotIndex();
        if(lev < spot->getMinLevel())
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_lootWiKi_9 , spot->getMinLevel() )->getCString() );
        else
        {
            SGMainManager::static_plotId = plotId;
            SGMainManager::sourceToStory = true;
            
//            main::PlotListRequest *request = new main::PlotListRequest();
//            request->set_storyid(section );
//            request->set_poltaskid( section );
//            SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
            
            main::StoryListRequest *request = new main::StoryListRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_STORY_MAIN_ENTER, request);

            
            m_lootWikiMainLayer->boxClose();
        }
    }
}

//去向 天梯商城
void SGLootWiKiLayer::onSourceTianTiClicked()
{
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_PVP , SHOP_LAYER);
    m_lootWikiMainLayer->boxClose();
}

void SGLootWiKiLayer::onJingYingChuangGuan(int plotId)
{
    m_plotId = plotId;
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    int count = player->getGoodsNumInBag();
    if (count >= player->getPlayerBagSize())
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
    else if(player->getMaxPlotId() < player->getLimitLevelById(limitJinyinFuben))
    {
        SGSpotDataModel* spotData = SGStaticDataManager::shareStatic()->getSpotById(player->getLimitLevelById(limitJinyinFuben), true);
        SGMainManager::shareMain()->showMessage( CCString::createWithFormat(str_MainLayer_str152,spotData->getSpotSectionIndex())->getCString());
    }
    else
    {
        SGMainManager::sourceToStory = true;
        SGMainManager::static_plotId = plotId;
        main::BossStoryListRequest *request = new main::BossStoryListRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_CREAM_BATTLE, request);
    }

}

void SGLootWiKiLayer::onInstance()
{
    SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
    if(playerInfo->getPlayerLevel() < playerInfo->getLimitLevelById(limitXianshiFuben))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str17 , playerInfo->getLimitLevelById(limitXianshiFuben))->getCString());
        return ;
    }
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->activityHandler();
    
    m_lootWikiMainLayer->boxClose();
}

void SGLootWiKiLayer::showCream(CCObject * sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::BossPlotListResponse *response = (main::BossPlotListResponse *)req->m_msg;
    CCArray * array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
    if (response)
    {
        int index = array->count() - (m_plotId - 20000);
        if (index >= 0 && index < array->count())
        {
            SGStoryBean *story = (SGStoryBean *)array->objectAtIndex( index ); //精英关卡的排序是反向的！最初的关卡索引反而大！
            int size = response->bossplotmodel_size();
            if (size > 0)
            {
                for (int ii = 0; ii < size; ii++)
                {
                    main::BossPlotModel model = response->bossplotmodel(ii);
                    CCLOG("scene ID=================%d", model.plotid());
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL)
                    {
                        continue;
                    }
                    else
                    {
                        scene->setVisible(true);
                        //scene->setRank(model.ra);
                        scene->setPower(model.power());
                        scene->setGold(model.gold());
                        scene->setExp(model.exp());
                        scene->setZhandou(model.zhandou());
                        scene->setRank(model.rank());
                        scene->setCreamCanJoin(model.canjoin());
                        
                        //数据存储在playerinfo中
                        SGPlayerInfo::sharePlayerInfo()->setCreamData(model.plotid(), model.canjoincount(), model.canjoin());
                        
                        
                        scene->setCreamDayTimes(model.canjoincount());
                        scene->setCreamBuyTimes(model.canbuycount());
                        scene->setBuyPrice(model.price());
                        scene->setBuyCompTimes(model.buycount());// 已经购买次数
                        scene->setDropInfo(model.exp(), model.gold());
                        
                        //精英闯关 剩余次数存储在playerinfo中
                        //SGPlayerInfo::sharePlayerInfo()->setLeftCreamTimes(model.canjoincount());
                        
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                        
                    }
                    SGPlayerInfo::sharePlayerInfo()->setMaxBossPlotId(model.plotid());
                }
            }
            //假如可能出现问题，也能跳关，只是关卡会显示不正确。
            SGMainManager::shareMain()->showStorySectionLayer(story);
        }
        
        //不判断该关卡是否能打，交给精英节列表的逻辑。
//        if(!SGPlayerInfo::sharePlayerInfo()->getCreamIsCanJoin(SGMainManager::static_plotId))
//        {
//            int leftJoinCount = SGStaticDataManager::shareStatic()->getCreamByPlotId(SGMainManager::static_plotId)->getCreamDayTimes() - SGPlayerInfo::sharePlayerInfo()->getCreamJoinedCount(SGMainManager::static_plotId) ;
//            SGMainManager::shareMain()->showMessage(str_lootWiKi_10);
//            SGMainManager::sourceToStory = false;
//        }
//        else
        {
            SGMainManager::shareMain()->m_trackIsPve = false;
            m_lootWikiMainLayer->boxClose();
        }
    }
    SGMainManager::sourceToStory = false;
}

void SGLootWiKiLayer::sendMsg()
{
    main::BossPlotListRequest *request = new main::BossPlotListRequest();
    request->set_storyid(SGMainManager::static_plotId);
    SGSocketClient::sharedSocketClient()->send(MSG_CREAM_STORY, request);
}

