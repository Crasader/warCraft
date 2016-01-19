//
//  SGMail.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-26.
//
//

#include "SGMail.h"
#include "SGMailCell.h"
#include "SGMainManager.h"
#include "SGSystemMailLayer.h"
#include "SGFriendMailLayer.h"
#include "SGFriendMailContent.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGMail::SGMail()
{
    CCLOG("SGMail()");
}

SGMail::~SGMail()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_mailLayer);
    CCLOG("~SGMail()");
}

SGMail* SGMail::create()
{
    SGMail *mail = new SGMail();
    if(mail && mail->init(NULL, sg_mailLayer))
    {
        mail->viewDidLoad();
        mail->autorelease();
        return mail;
    }
    
    CC_SAFE_DELETE(mail);
    return NULL;
}



SNSTableViewCellItem* SGMail::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGMailCell *cell = (SGMailCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if(NULL == cell)
    {
        cell = SGMailCell::create(indexPath->getRow(),this,tableView);
    }
    
    return cell;
}

void SGMail:: itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    
    EFFECT_PLAY(MUSIC_BTN);
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    
    if(MAIL_FRIEND == indexpath->getRow())
    {
        SGFriendMailLayer *friendMail = SGFriendMailLayer::create(playerInfo->getFriendMails());
        SGMainManager::shareMain()->showLayer(friendMail);
        SNSTableView::showScrollItemMultiMoveFromRight(friendMail->getSNSTavleView());//1126效果优化
        
        
    }else if(MAIL_SYSTEM == indexpath->getRow())
    {
        SGSystemMailLayer *system = SGSystemMailLayer::create(playerInfo->getSystemMails());
        SGMainManager::shareMain()->showLayer(system);
        SNSTableView::showScrollItemMultiMoveFromRight(system->getSNSTavleView());//1126效果优化
        
    }else if(MAIL_GM == indexpath->getRow())
    {
        SG_SHOW_WINDOW(str_SystemMailLayer_str3);
        
        /*
         SGFriendMailContent *content = SGFriendMailContent::create((SGMailDataModel*)playerInfo->getGMMails()->objectAtIndex(indexPath->getRow()));
         SGMainManager::shareMain()->showLayer(content);
         */
    }

}

void SGMail::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//    
//    if(MAIL_FRIEND == indexPath->getRow())
//    {
//        SGFriendMailLayer *friendMail = SGFriendMailLayer::create(playerInfo->getFriendMails());
//        SGMainManager::shareMain()->showLayer(friendMail);
//        SNSTableView::showScrollItemMultiMoveFromRight(friendMail->getSNSTavleView());//1126效果优化
//
//        
//    }else if(MAIL_SYSTEM == indexPath->getRow())
//    {
//        SGSystemMailLayer *system = SGSystemMailLayer::create(playerInfo->getSystemMails());
//        SGMainManager::shareMain()->showLayer(system);
//        SNSTableView::showScrollItemMultiMoveFromRight(system->getSNSTavleView());//1126效果优化
//        
//    }else if(MAIL_GM == indexPath->getRow())
//    {
//        SG_SHOW_WINDOW("尚未开启");
//        
//        /*
//         SGFriendMailContent *content = SGFriendMailContent::create((SGMailDataModel*)playerInfo->getGMMails()->objectAtIndex(indexPath->getRow()));
//         SGMainManager::shareMain()->showLayer(content);
//         */
//    }
}

//就有三行
unsigned int SGMail::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return 2;
}

void SGMail::viewDidLoad()
{
    //界面布局
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    //星形背景
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_mailLayer);

    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30+150+72, spriteBackground->getContentSize().width, SGLayout::getSkewingY(480)-47-150-72-61-101);
    //spriteBackground->setScaleY(r.size.height / spriteBackground->getContentSize().height);
    spriteBackground->setTextureRect(r);
    spriteBackground->setScaleX(s.width/spriteBackground->getContentSize().width * 1.01);
    spriteBackground->setAnchorPoint(ccp(0.5, 1));
    spriteBackground->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72)));
    this->addChild(spriteBackground);
    spriteBackground->setVisible(false);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);

    
    //标题
    SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(str_SystemMailLayer_str4, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//spriteTitle->setColor(ccc3(0xff, 0x95, 0x0c));
//    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -s.height*0.23)));
    
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(spriteTitle);
    //五张图片的尺寸   -47-150-72-56-101
//    SGButton *buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png", this, menu_selector(SGMail::buttonClickGoBack),CCPointZero,false,true);
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGMail::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    buttonBack->setPosition(ccpAdd(upLeft, ccp(0, -h+65/2)));
    addBtn(buttonBack);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    //tableview
//    this->tableView->setFrame(CCRectMake(0,101+61, SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-47-150-72-63-101));
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));

    this->tableView->setZOrder(999);
    this->tableViewHeight = 110;
    tableView->setDown(-25);
    
    tableView -> setPageNEnable(true);
    
//    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
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
    
}

void SGMail::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}

std::string SGMail::getTimeStringByTimeStamp(long long timeStamp)
{
    
    //        time_t rawtime;
    //
    //       time ( &rawtime );
    //       printf("%ld\n", &rawtime);
    //       printf ( "The current local time is: %s", ctime (&rawtime) );
    //
    //       char * time = ctime(&rawtime);
    //ctime(&rawtime) : time_t/timestampe -> "Www Mmm dd hh:mm:ss yyyy" format
    //        cout<< time << endl;
    //       printf("%s", asctime(gmtime(&rawtime)));
    // asctime(gmtime(&rawtime)) = ctime(&rawtime);
    //here gmtime(&rawtime) : time_t(timpstampe) -> struct tm
    //here asctime(gmtime) : struct tm -> "Www Mmm dd hh:mm:ss yyyy" format
    
    //  int	tm_sec;		/* seconds after the minute [0-60] */
    //	int	tm_min;		/* minutes after the hour [0-59] */
    //	int	tm_hour;	/* hours since midnight [0-23] */
    //	int	tm_mday;	/* day of the month [1-31] */
    //	int	tm_mon;		/* months since January [0-11] */
    //	int	tm_year;	/* years since 1900 */
    //	int	tm_wday;	/* days since Sunday [0-6] */
    //	int	tm_yday;	/* days since January 1 [0-365] */
    //	int	tm_isdst;	/* Daylight Savings Time flag */
    //	long	tm_gmtoff;	/* offset from CUT in seconds */
    //	char	*tm_zone;	/* timezone abbreviation */
    
    //    cout<<(ptm->tm_year + 1900)<<"year "<<(ptm->tm_mon + 1)<<"month "<<(ptm->tm_mday)<<"day "<<(ptm->tm_hour)<<":"<<(ptm->tm_min)<<":"<<(ptm->tm_sec)<<endl;
    
    
    time_t rawtime = (time_t )(timeStamp/1000);
    struct tm * ptm;
    ptm = localtime(&rawtime);
    
    int tm_min = ptm->tm_min;
    int tm_hour = ptm->tm_hour;
    int tm_mday = ptm->tm_mday;
    int tm_month = ptm->tm_mon+1;
    int tm_year = ptm->tm_year+1900;
    
    string timeStr;
    
    //拼接时间字符串
    char buffer[1024];
    sprintf(buffer, "%d-%d-%d %d:%d",tm_year,tm_month,tm_mday,tm_hour,tm_min);
    timeStr.append(buffer);
    
    return timeStr;
}


SGMailDataModel* SGMail::getMailDataByProto(main::MailProto proto)
{
    
    SGMailDataModel *mail = new SGMailDataModel();
    
    //邮件主体部分
    mail->setMailIsRead(proto.isread());
    mail->setMailOneSelfId(proto.oneselfid());
    mail->setMailOneselfNickname(CCString::create(proto.oneselfnickname()));
    mail->setMailOtherId(proto.otherid());
    mail->setMailOtherNickname(CCString::create(proto.othernickname()));
    mail->setMailSsid(proto.ssid());
    mail->setMailType(proto.type());
    mail->setMailItemId(proto.image());
    //辅体部分
    int detailSize = proto.maildetailsproto_size();
    for(int i=0;i<detailSize;i++)
    {
        SGMailDetailDataModel *detail = new SGMailDetailDataModel();
        main::MailDetailsProto detailProto = proto.maildetailsproto(i);
        
        detail->setMailDeContent(CCString::create(detailProto.content()));
        detail->setMailDeId(detailProto.mailid());
        detail->setMailDeReceiverId(detailProto.receiverid());
        detail->setMailDeReceiverNickName(CCString::create(detailProto.receivernickname()));
        detail->setMailDeSendDate(detailProto.senddate());
        detail->setMailDeSenderId(detailProto.senderid());
        detail->setMailDeSenderNickName(CCString::create(detailProto.sendernickname()));
        
        mail->getMailContent()->addObject(detail);
        
        detail->release();
        
    }
    
    mail->autorelease();
    
    return mail;
    
}

void SGMail::recieveMailUpdate()
{
    tableView->reloadData();
}











