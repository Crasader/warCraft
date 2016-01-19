/*
 **SGPvpFightRewardLayer.cpp
 **
 **
 **
 **
 **
 */

#include "SGPvpFightRewardLayer.h"
#include "SGCCLabelTTF.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "SGShowString.h"
#include "SGShareBox.h"
#include "SGGuideManager.h"
#include "SGStaticDataManager.h"
#include "PvpFight.pb.h"
#include "SGPlayerInfo.h"

SGPvpFightRewardLayer::SGPvpFightRewardLayer()
: m_shengWangNum(NULL)
, m_scoreNum(NULL)
, m_rankNum(NULL)
, m_objectRunningAction(NULL)
, m_rewardData(NULL)
, m_clickToContinue(NULL)
, m_progressInside(NULL)
, m_progressIndex(NULL)
, m_isActionAllDone(false)
, m_updateProgress(NULL)
, m_percent(0)
, m_maxPercent(0)
, m_finalPoint(0,0)
, m_isOverFifteen(false)
, m_winOrLost(NULL)
{
    memset(m_giftBox, 0, sizeof(SGCCLabelTTF *) * 6);
    
    m_objectRunningAction = CCArray::create();
    m_objectRunningAction->retain();
}

SGPvpFightRewardLayer::~SGPvpFightRewardLayer()
{
    m_objectRunningAction->release();
    
    /*m_rewardData 作为一个new的对象*/
    CC_SAFE_DELETE(m_rewardData);
    m_rewardData = NULL;
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_pvpFightRewardLayer);
    
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this) ;
}

SGPvpFightRewardLayer * SGPvpFightRewardLayer::create(SGPvpFightRewardData * data)
{
    SGPvpFightRewardLayer * ret = new SGPvpFightRewardLayer();
    if (ret && ret->init(NULL, sg_pvpFightRewardLayer) )
    {
        ret->initView(data);
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
        ret = NULL;
    }
    return ret;
}

void SGPvpFightRewardLayer::initView(SGPvpFightRewardData * data)
{
    m_rewardData = data;
    //是否当日超15场
    m_isOverFifteen = m_rewardData->getJoinCount() > 15;
    
    int joinCountBefore = m_rewardData->getJoinCount() - 1 ;
    CCAssert(data, "data can not be null");
    //绑定资源
    ResourceManager * resManager = ResourceManager::sharedInstance();
    resManager->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_pvpFightRewardLayer);
    resManager->bindTexture("sgfightrewardlayer/sgfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_pvpFightRewardLayer);
    resManager->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_pvpFightRewardLayer);
    resManager->bindTexture("sgpvpfightrewardlayer/sgpvpfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_pvpFightRewardLayer);
    resManager->bindTexture("updatelayer/updatelayer.plist", RES_TYPE_LAYER_UI, sg_pvpFightRewardLayer);
    resManager->bindTexture("animationFile/ybjs.plist", RES_TYPE_LAYER_UI, sg_pvpFightRewardLayer);
    
    float headH = H_TOP;
    
    //标题背景
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("fight_over_titlebg.png");
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height*.81f)));//,6
    this->addChild(title_bg, -1);
    
    //标题背景左右 花边 图片在sgcountrylayer.plist中
    CCSprite *title_Lbg = CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Lbg->setAnchorPoint(ccp(0, 0.5f));
    title_Lbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(-title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Lbg, -1);
    CCSprite *title_Rbg =CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Rbg->setAnchorPoint(ccp(1, 0.5f));
    title_Rbg->setFlipX(true);
    title_Rbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Rbg, -1);
    
    //标题
    CCSprite *title = CCSprite::createWithSpriteFrameName("fight_over_title_n.png");
    title->setPosition(title_bg->getPosition());
    this->addChild(title);

    //标题下方  一横杆
    CCSprite * stick = CCSprite::createWithSpriteFrameName("boss_title_bar.png");
    stick->setPosition(ccp( title_bg->getPositionX() , title_bg->getPositionY()  - title_bg->getContentSize().height*0.5 - stick->getContentSize().height * 0.6));
    this->addChild(stick , -1);
    
    //奖励内容背景图片kuang 	原始尺寸180*180
    CCScale9Sprite * bg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    //bg->setPreferredSize(CCSize( winSize.width - 70 , winSize.height -  headH - bottomH - title_bg->getContentSize().height - stick->getContentSize().height - 40 ));
    bg->setPreferredSize(CCSize(590, 460));
    //bg->setPosition(ccp(stick->getPositionX() , stick->getPositionY() - stick->getContentSize().height - bg->getContentSize().height * 0.5) );
    bg->setPosition(SGLayout::getPoint(kMiddleCenter));
    this->addChild(bg , -1);
    
    //框的背景图片
    CCSprite * kuang_bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    kuang_bg->setScaleX(bg->getContentSize().width / kuang_bg->getContentSize().width);
    kuang_bg->setScaleY(bg->getContentSize().height / kuang_bg->getContentSize().height);
    kuang_bg->setPosition(bg->getPosition() );
    kuang_bg->setOpacity(150);
    this->addChild(kuang_bg , -10);
    
    //点击屏幕继续的图片字 图片在 sggeneralslayer
    m_clickToContinue = CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
    m_clickToContinue->setVisible(true);
    m_clickToContinue->setPosition(ccp(stick->getPositionX() , bg->getPositionY() - bg->getContentSize().height * 0.5 - m_clickToContinue->getContentSize().height - 40) );
    this->addChild(m_clickToContinue);
    
    //天梯声望
    SGCCLabelTTF * shengWang = SGCCLabelTTF::create(SGPvpFightRewardLayer_1, FONT_PANGWA, 26);
    shengWang->setAnchorPoint(ccp(0 , 0) );
    shengWang->setPosition(ccp(bg->getPositionX() - bg->getContentSize().width*0.5 + 25 , bg->getPositionY() + bg->getContentSize().height * 0.5 - 80 - shengWang->getContentSize().height * 0.5));
    this->addChild(shengWang);
    
    m_shengWangNum = SGCCLabelTTF::create( CCString::createWithFormat("%d" , m_rewardData->getShengWang())->getCString() , FONT_PANGWA  , 28);
    m_shengWangNum->getMainLabel()->setVisible(false);
    m_shengWangNum->setAnchorPoint(ccp(0,0));
    m_shengWangNum->setPosition(ccp(shengWang->getPosition().x + shengWang->getContentSize().width  , shengWang->getPosition().y));
    this->addChild(m_shengWangNum);
    //加入array中，当用户点击屏幕时 停止所有的动画效果 显示最终结果
    m_objectRunningAction->addObject(m_shengWangNum);
    
    
    //天梯积分
    SGCCLabelTTF * score = SGCCLabelTTF::create(SGPvpFightRewardLayer_2, FONT_PANGWA, 26);
    score->setAnchorPoint(ccp(0 , 0) );
    score->setPosition(ccp(shengWang->getPosition().x , shengWang->getPosition().y - 20 - shengWang->getContentSize().height));
    this->addChild(score);
    
    m_scoreNum = SGCCLabelTTF::create( CCString::createWithFormat("%d" , m_rewardData->getScore())->getCString() , FONT_PANGWA  , 28);
    m_scoreNum->getMainLabel()->setVisible(false);
    m_scoreNum->setAnchorPoint(ccp(0,0));
    m_scoreNum->setPosition(ccp(score->getPosition().x + score->getContentSize().width  , score->getPosition().y));
    this->addChild(m_scoreNum);
    //加入array中，当用户点击屏幕时 停止所有的动画效果 显示最终结果
    m_objectRunningAction->addObject(m_scoreNum);
    
    
    //预计天梯排名
    SGCCLabelTTF * rank = SGCCLabelTTF::create(SGPvpFightRewardLayer_3, FONT_PANGWA, 26);
    rank->setAnchorPoint(ccp(0 , 0) );
    rank->setPosition(ccp(shengWang->getPosition().x , score->getPosition().y - 20 - score->getContentSize().height));
    this->addChild(rank);
    
    m_rankNum = SGCCLabelTTF::create( CCString::createWithFormat("%d" , m_rewardData->getRank())->getCString() , FONT_PANGWA  , 28);
    m_rankNum->getMainLabel()->setVisible(false);
    m_rankNum->setAnchorPoint(ccp(0,0));
    m_rankNum->setPosition(ccp(rank->getPosition().x+rank->getContentSize().width  , rank->getPosition().y));
    this->addChild(m_rankNum);
    //加入array中，当用户点击屏幕时 停止所有的动画效果 显示最终结果
    m_objectRunningAction->addObject(m_rankNum);
    
    
    //胜利或者失败的图片
    m_winOrLost = NULL;
    if( m_rewardData->getIsWin())
        m_winOrLost = CCSprite::createWithSpriteFrameName("pvpFightRewardWin.png");
    else
        m_winOrLost = CCSprite::createWithSpriteFrameName("pvpFightRewardLost.png");
    m_winOrLost->setPosition(ccp(m_scoreNum->getPosition().x + 120 + m_winOrLost->getContentSize().width*0.5 , m_scoreNum->getPosition().y ));
    this->addChild(m_winOrLost);
    m_objectRunningAction->addObject(m_winOrLost);
    
    //每日奖励的文本图片
    CCSprite * dayReward = CCSprite::createWithSpriteFrameName("pvpFightRewardText.png");
    dayReward->setPosition(ccp(dayReward->getContentSize().width *0.5 + bg->getPositionX() - bg->getContentSize().width*0.5 + 10 , m_winOrLost->getPositionY() - m_winOrLost->getContentSize().height * 0.77 ));
    this->addChild(dayReward);
    
    //3个宝箱 , 实际创建6个 然后再判断显示哪一个。
    for(int i = 1 ; i <= 6 ; ++i)
    {
        m_giftBox[i-1] = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("pvpFightRewardGiftBox%d.png",i)->getCString() );
        //亮的先隐藏 前三个 暗  后三个亮。
        if(i>3)
            m_giftBox[i-1]->setVisible(false);
        if(i%3 == 1)
            m_giftBox[i-1]->setPosition(ccp(15 + dayReward->getPositionX() + dayReward->getContentSize().width * 0.5 + m_giftBox[i-1]->getContentSize().width*0.5 , dayReward->getPositionY()));
        else if (i%3 == 2)
            m_giftBox[i-1]->setPosition(ccp(m_giftBox[i-2]->getPositionX() + 155 , m_giftBox[0]->getPositionY() ));
        else
            m_giftBox[i-1]->setPosition(ccp(m_giftBox[i-2]->getPositionX() + 155 , m_giftBox[0]->getPositionY() ));
        this->addChild(m_giftBox[i-1]) ;
    }
    if(5 <= joinCountBefore )
    {
        m_giftBox[0]->setVisible(false);
        m_giftBox[3]->setVisible(true);
        if ( 10 <= joinCountBefore )
        {
            m_giftBox[1]->setVisible(false);
            m_giftBox[4]->setVisible(true);
            if(15 <= joinCountBefore )
            {
                m_giftBox[2]->setVisible(false);
                m_giftBox[5]->setVisible(true);
            }
        }
    }
    
    
    //下方进度条背景
    CCSprite * progressBg = CCSprite::createWithSpriteFrameName("loadingbg.png");
    progressBg->setPosition(ccp(stick->getPositionX() , m_giftBox[0]->getPositionY() - m_giftBox[0]->getContentSize().height*0.5 - 32 - progressBg->getContentSize().height*0.5));
    this->addChild(progressBg);
    
    //2个填充色  变弧形为刻度
    CCSprite * filling1 = CCSprite::createWithSpriteFrameName("pvpFightRewardBeside.png");
    CCSprite * filling2 = CCSprite::createWithSpriteFrameName("pvpFightRewardBeside.png");
    this->addChild(filling1);
    this->addChild(filling2);
    filling1->setPosition(ccp(progressBg->getPositionX() - progressBg->getContentSize().width*0.5 + 50 , progressBg->getPositionY()));
    filling2->setPosition(ccp(progressBg->getPositionX() + progressBg->getContentSize().width*0.5 - 50 , progressBg->getPositionY()));
    
    
    float gap = (filling2->getPositionX() - filling1->getPositionX() )/3;
    
    //四个刻度数字 和 2个刻度图片
    CCSprite * index = NULL;
    CCSprite * indexSprite = NULL;
    for(int i = 1 ; i<=4 ; ++i)
    {
        index = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("pvpFightReward%d.png" , 5*(i-1)) ->getCString() );
        index->setPosition(ccp( filling1->getPositionX() + gap*(i-1) , progressBg->getPositionY() - progressBg->getContentSize().height*0.5 - index->getContentSize().height*0.5 -5));
        if(i==2 || i==3 )
        {
            indexSprite = CCSprite::createWithSpriteFrameName("pvpFightRewardScale.png");
            indexSprite->setPosition(ccp( index->getPositionX() , progressBg->getPositionY()));
            this->addChild(indexSprite , 1000);
        }
        this->addChild(index);
    }
    
    //进度指示条
    m_progressIndex = CCSprite::createWithSpriteFrameName("pvpFightRewardProgress.png");
    m_progressIndex->setPosition(ccp( filling1->getPositionX() + gap/5 * (joinCountBefore>15 ? 15:joinCountBefore ), progressBg->getPositionY() + progressBg->getContentSize().height * 0.5 + m_progressIndex->getContentSize().height * 0.5 ));
    this->addChild(m_progressIndex);
    m_objectRunningAction->addObject(m_progressIndex);
    if(!m_isOverFifteen)
        m_finalPoint = CCPoint( m_progressIndex->getPositionX() + gap/5 , m_progressIndex->getPositionY());
    else
        m_finalPoint = m_progressIndex->getPosition();

    
    //进度填充色
    m_progressInside = CCSprite::createWithSpriteFrameName("loading_progress.png");
    m_progressInside->setAnchorPoint(ccp(0,0.5));
    m_progressInside->setPosition(ccp(filling1->getPositionX() , progressBg->getPositionY() ));
    //this->addChild(m_progressInside , 50);
    
    if(m_isOverFifteen)
        m_percent = m_maxPercent = 100;
    else
    {
        m_percent = 100.0 * (m_rewardData->getJoinCount()-1)/15;
        m_maxPercent = 100.0*(m_rewardData->getJoinCount())/15.0 ;
    }
    m_updateProgress = CCProgressTimer::create(m_progressInside);
    m_updateProgress->setAnchorPoint(ccp(0,0.5));
    m_updateProgress->setPosition(filling1->getPositionX()+3, progressBg->getPositionY() + 2);
    m_updateProgress->setType(kCCProgressTimerTypeBar);
    m_updateProgress->setMidpoint(ccp(0, 0));
    m_updateProgress->setBarChangeRate(ccp(1, 0));
    m_updateProgress->setPercentage(m_percent);
    this->addChild(m_updateProgress , 10);
    m_objectRunningAction->addObject(m_updateProgress);
    
    
    //显示分享
    if(SGPlayerInfo::sharePlayerInfo()->getShowvip() == SHOW_SHAREBOX_ID)
        this->showFBShare(false);   //modified by cgp
    
    //播放胜利特效
    m_winOrLost->setScale(3);
    m_winOrLost->setOpacity(0);
    CCDelayTime * delayTime = CCDelayTime::create(0.5);
    CCFadeTo * fadeTo = CCFadeTo::create(0.2, 255);
    CCScaleTo * scaleTo = CCScaleTo::create(0.2, 1);
    CCSpawn * spawn = CCSpawn::create(fadeTo , scaleTo , NULL);
    /*
    //shake bgn
    CCPoint pos = winOrLost->getPosition();
    
    float shake1 = -8;
    float shake2 = 8;
    float shake3 = -5;
    float shake4 = 5;
    float shake5 = -2;
    float shake6 = 2;
    float shake7 = 0;
    
    float time1 = 0.05;
    float time2 = 0.06;
    float time3 = 0.05;
    float time4 = 0.07;
    float time5 = 0.05;
    float time6 = 0.06;
    float time7 = 0.04;
    
    bool dir = true;
    
    if (!dir)
    {
        shake1 = -shake1; shake2 = -shake2; shake3 = -shake3; shake4 = -shake4; shake5 = -shake5; shake6 = -shake6; shake7 = -shake7;
    }
    
    CCMoveTo *move1 = CCMoveTo::create(time1, ccpAdd(pos,ccp( shake1, 0)));
    CCMoveTo *move2 = CCMoveTo::create(time2, ccpAdd(pos,ccp( shake2, 0)));
    CCMoveTo *move3 = CCMoveTo::create(time3, ccpAdd(pos,ccp( shake3, 0)));
    CCMoveTo *move4 = CCMoveTo::create(time4, ccpAdd(pos,ccp( shake4, 0)));
    CCMoveTo *move5 = CCMoveTo::create(time5, ccpAdd(pos,ccp( shake5, 0)));
    CCMoveTo *move6 = CCMoveTo::create(time6, ccpAdd(pos,ccp( shake6, 0)));
    CCMoveTo *move7 = CCMoveTo::create(time7, ccpAdd(pos,ccp( shake7, 0)));
    //铭牌摇晃
    CCAction *act = (CCAction*)CCSequence::create( move1, move2, move3, move4, move5, move6, move7, NULL);
    
    //shake end
    */
    CCCallFunc * callFunc = CCCallFunc::create(this, callfunc_selector(SGPvpFightRewardLayer::labelAction ) );
    CCDelayTime * delayTime2 = CCDelayTime::create(0.5);
    CCPlaySound::sharePlaySound()->preloadeffectByType(MUSIC_63);
    CCSequence * sequence = NULL;
    //胜利特效
    if(m_rewardData->getIsWin())
        sequence = CCSequence::create(delayTime,spawn ,
                                               CCCallFuncO::create(this,callfuncO_selector(SGPvpFightRewardLayer::playSoundCallback) ,CCString::create(MUSIC_63)  ),
                                                 CCDelayTime::create(0.05) , CCScaleTo::create(0.05, 1.3),CCScaleTo::create(0.05, 1) , delayTime2 , callFunc, NULL );
    else
        sequence = CCSequence::create(delayTime,spawn ,
                                      CCCallFuncO::create(this,callfuncO_selector(SGPvpFightRewardLayer::playSoundCallback) ,CCString::create(MUSIC_67)  ),delayTime2 , callFunc, NULL );
    m_winOrLost->runAction(sequence);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    this->setTouchEnabled(true);
    
    //重新连接服务器  从pvp服撤退
    std::string ip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
    short port = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
    SGSocketClient::sharedSocketClient()->startConn(ip.c_str(), port);
}


bool SGPvpFightRewardLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void SGPvpFightRewardLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int joinCountTotal = m_rewardData->getJoinCount() ;
    //跳转页面
    if(m_isActionAllDone)
    {
        //宝箱没有打开
        if( 0 !=  joinCountTotal && joinCountTotal%5 != 0 )
        {
            SGMainManager::shareMain()->showPvpMatchLayer();
        }
        else
        {
            main::PvPDayAwardGiftEntryRequest *request = new main::PvPDayAwardGiftEntryRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_PVP_EVERYDAY, request);
        }
    }
    //停止所有动画 ， 显示最终的ui , 置m_isActionAllDone 为true
    else
    {
        for(int i = 0; i < m_objectRunningAction->count() ; ++i)
        {
            CCNode * nodeRunningAction = (CCNode * )m_objectRunningAction->objectAtIndex(i);
            nodeRunningAction->stopAllActions();
        }
        
        this->showFinalUI();
        m_isActionAllDone = true;
    }
    
}

void SGPvpFightRewardLayer::showFinalUI()
{
    //胜利或失败的图片一开始是透明的  故
    m_winOrLost->setScale(1);
    m_winOrLost->setOpacity(255);
    
    CCLabelTTF * m_shengWangNum = this->m_shengWangNum->getMainLabel();
    CCLabelTTF * m_scoreNum = this->m_scoreNum->getMainLabel();
    CCLabelTTF * m_rankNum = this->m_rankNum->getMainLabel();
    
    m_shengWangNum->setVisible(true);
    m_shengWangNum->setScale(1);
    m_shengWangNum->runAction(CCScaleTo::create(0.00000001, 1) );
    m_shengWangNum->setOpacity(255);
    
    m_scoreNum->setVisible(true);
    m_scoreNum->setScale(1);
    m_scoreNum->runAction(CCScaleTo::create(0.00000001, 1) );
    m_scoreNum->setOpacity(255);

    m_rankNum->setVisible(true);
    m_rankNum->setScale(1);
    m_rankNum->runAction(CCScaleTo::create(0.00000001, 1) );
    m_rankNum->setOpacity(255);

    m_progressIndex->setPosition(m_finalPoint);
    
    m_updateProgress->setPercentage(m_maxPercent);
    
    this->updateGiftBox();
}

void SGPvpFightRewardLayer::showFBShare(bool isShow)
{
    if (isShow == false)
        return ;
    m_clickToContinue->setVisible(false);
    
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic2 =(CCDictionary *) dic->objectForKey("25");
    int nyuanbao =dic2->valueForKey("value")->intValue();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("26");
    int ntongqian =dic1->valueForKey("value")->intValue();
    
    SGCCLabelTTF*label3 = NULL;
    
    label3 = SGCCLabelTTF::create(str_SGShareBox_str3, FONT_PANGWA, 30 , ccWHITE , ccBLACK);
    label3->setPosition(m_clickToContinue->getPosition() );
    this->addChild(label3);
    CCSprite *yubao = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yubao);
    yubao->setPosition(ccpAdd(label3->getPosition(),ccp(-label3->getContentSize().width/2,-35)));
    yubao->setScale(1);
    
    SGCCLabelTTF*label4 = NULL;
    
    label4 = SGCCLabelTTF::create(CCString::createWithFormat("x%d",nyuanbao)->getCString(), FONT_PANGWA, 30 , ccGREEN , ccBLACK);
    label4->setPosition(ccpAdd(yubao->getPosition(), ccp(label4->getContentSize().width/2+yubao->getContentSize().width/2, 0)));
    this->addChild(label4);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int level = playerInfo->getPlayerLevel();
    
    CCSprite *tongqian = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(tongqian);
    tongqian->setPosition(ccpAdd(label4->getPosition(),ccp(label4->getContentSize().width/2+tongqian->getContentSize().width/2,0)));
    tongqian->setScale(1);
    
    SGCCLabelTTF*label5 = NULL;
    
    label5 = SGCCLabelTTF::create(CCString::createWithFormat("x%d",ntongqian)->getCString(), FONT_PANGWA, 30 , ccGREEN , ccBLACK);
    label5->setPosition(ccpAdd(tongqian->getPosition(), ccp(label5->getContentSize().width/2+tongqian->getContentSize().width/2, 0)));
    this->addChild(label5);

    SGButton * shareBtn = SGButton::create("sharefacebook-3.png",
                                        "sharefacebook-3.png",
                                        this,
                                        menu_selector(SGPvpFightRewardLayer::onFBBtnClicked),
                                        ccp(0, 0),
                                        false,
                                        true);
    shareBtn->setScale(0.7);
    this->addBtn(shareBtn);
    shareBtn->setPosition(yubao->getPositionX() - 80 , (yubao->getPositionY() + label3->getPosition().y)*0.5 );
}

void SGPvpFightRewardLayer::onFBBtnClicked()
{
    if(!SGGuideManager::shareManager()->isGuide)
    {
        SGShareBox *box = SGShareBox::create(this,SGShareTypeGeneralFight);
        SGMainManager::shareMain()->showBox(box);
    }

}

void SGPvpFightRewardLayer::labelAction()
{
    CCLabelTTF * m_shengWangNum = this->m_shengWangNum->getMainLabel();
    CCLabelTTF * m_scoreNum = this->m_scoreNum->getMainLabel();
    CCLabelTTF * m_rankNum = this->m_rankNum->getMainLabel();
    
    float delay = 0.2;
    float fadeToTime = 0.2;
    int opacity = 0;
    CCPlaySound * playSound = CCPlaySound::sharePlaySound();
    m_shengWangNum->setScale(5);
    m_shengWangNum->setOpacity(opacity);
    
    CCFadeTo * fadeTo = CCFadeTo::create(fadeToTime, 255);
    CCScaleTo * scaleTo = CCScaleTo::create(fadeToTime, 1);
    CCSpawn * spawn = CCSpawn::create(fadeTo , scaleTo , NULL);
    
    playSound->preloadeffectByType(MUSIC_64);
    CCSequence * sequence = CCSequence::create(CCShow::create(), spawn,
                                               CCCallFuncO::create(this,callfuncO_selector(SGPvpFightRewardLayer::playSoundCallback) , CCString::create(MUSIC_64)  ) , NULL );
    m_shengWangNum->runAction(sequence);

    m_scoreNum->setScale(5);
    m_scoreNum->setOpacity(opacity);
    fadeTo = CCFadeTo::create(fadeToTime, 255);
    scaleTo = CCScaleTo::create(fadeToTime, 1);
    spawn = CCSpawn::create(fadeTo , scaleTo , NULL);
    CCDelayTime *  delayTime = CCDelayTime::create(delay);
    
    playSound->preloadeffectByType(MUSIC_64);
    sequence = CCSequence::create( delayTime ,CCShow::create() , spawn ,
                                  CCCallFuncO::create(this,callfuncO_selector(SGPvpFightRewardLayer::playSoundCallback) , CCString::create(MUSIC_64)  ), NULL );
    m_scoreNum->runAction(sequence);

    
    m_rankNum->setVisible(true);
    m_rankNum->setScale(5);
    m_rankNum->setOpacity(opacity);
    fadeTo = CCFadeTo::create(fadeToTime, 255);
    scaleTo = CCScaleTo::create(fadeToTime, 1);
    spawn = CCSpawn::create(fadeTo , scaleTo , NULL);
    delayTime = CCDelayTime::create(delay*2);
    
    playSound->preloadeffectByType(MUSIC_64);
    sequence = CCSequence::create(delayTime, CCShow::create() , spawn ,
                                  CCCallFuncO::create(this,callfuncO_selector(SGPvpFightRewardLayer::playSoundCallback) , CCString::create(MUSIC_64)  ) , NULL );
    m_rankNum->runAction(sequence);
    
    CCCallFunc * callFunc = CCCallFunc::create(this, callfunc_selector(SGPvpFightRewardLayer::finalAciotn ) );
    //15场之后满的  不显示进度。
    if (!m_isOverFifteen)
    {
        CCMoveTo * moveTo = CCMoveTo::create(1.2 , m_finalPoint);
        delayTime = CCDelayTime::create(delay*4.5);
        playSound->preloadeffectByType(MUSIC_65);
        playSound->preloadeffectByType(MUSIC_66);
        sequence = CCSequence::create(delayTime ,
                                      CCCallFuncO::create(this,callfuncO_selector(SGPvpFightRewardLayer::playSoundCallback) , CCString::create(MUSIC_65)  ), moveTo , NULL);
        m_progressIndex->runAction(sequence);
        
        CCProgressFromTo * progress = CCProgressFromTo::create(1.2, m_percent, m_maxPercent);
        delayTime = CCDelayTime::create(delay*4.5);
        CCDelayTime * delayTime2 = CCDelayTime::create(0.2);
        CCCallFunc * callFunc2 = CCCallFunc::create(this , callfunc_selector(SGPvpFightRewardLayer::updateAfterAllAction));
        CCSequence * sequence = CCSequence::create(delayTime , progress , callFunc , delayTime2 , callFunc2 ,  NULL);
        m_updateProgress->runAction(sequence);
    }
    else
    {
        CCDelayTime * delayTime2 = CCDelayTime::create(2);
        sequence = CCSequence::create(delayTime2  , callFunc , NULL);
        m_rankNum->runAction(sequence);
    }
}

void SGPvpFightRewardLayer::finalAciotn()
{
    if(m_rewardData->getJoinCount()%5 == 0 )
    {
        CCSpriterX *newRewardEffect = CCSpriterX::create("animationFile/ybjs.scml", true, true);
        newRewardEffect->setanimaID(0);
        newRewardEffect->setPosition(ccpAdd(m_progressIndex->getPosition() , ccp(-5, 2) ));
        //newRewardEffect->setScale(0.9);
        newRewardEffect->setAnimaSpeed(2);
        this->addChild(newRewardEffect, 512);
        m_objectRunningAction->addObject(newRewardEffect);
        newRewardEffect->play();
    }
}

void SGPvpFightRewardLayer::updateAfterAllAction()
{
    this->updateGiftBox();
    m_isActionAllDone = true;
}

void SGPvpFightRewardLayer::playSoundCallback(CCObject * obj)
{
    CCString * str = (CCString *)obj;
    EFFECT_PLAY(str->getCString());
}

void SGPvpFightRewardLayer::updateGiftBox()
{
    int joinCountTotal = m_rewardData->getJoinCount();
    if(5 == joinCountTotal )
    {
        m_giftBox[0]->setVisible(false);
        m_giftBox[3]->setVisible(true);
        EFFECT_PLAY(MUSIC_66);
    }
    else if(10 == joinCountTotal)
    {
        m_giftBox[0]->setVisible(false);
        m_giftBox[3]->setVisible(true);
        
        m_giftBox[1]->setVisible(false);
        m_giftBox[4]->setVisible(true);
        EFFECT_PLAY(MUSIC_66);
    }
    else if(15 == joinCountTotal)
    {
        m_giftBox[0]->setVisible(false);
        m_giftBox[3]->setVisible(true);
        
        m_giftBox[1]->setVisible(false);
        m_giftBox[4]->setVisible(true);
        
        m_giftBox[2]->setVisible(false);
        m_giftBox[5]->setVisible(true);
        EFFECT_PLAY(MUSIC_66);
    }
}


