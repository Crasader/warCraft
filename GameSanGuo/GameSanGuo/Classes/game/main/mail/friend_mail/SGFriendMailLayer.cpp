//
//  SGFriendMailLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#include "SGFriendMailLayer.h"
#include "SGMainManager.h"
#include "SGMailCell.h"
#include "SGMail.h"
#include "SGFriendMailCell.h"
#include "SGFriendMailContent.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
int SGFriendMailLayer::_waitBeDeletedMailId = 0;

SGFriendMailLayer::SGFriendMailLayer()
{
    _mailArray = NULL;
    buttonModel = NULL;
    _isDeleteModel = false;
    m_pNoMailLabel = NULL;
}

SGFriendMailLayer::~SGFriendMailLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_friendMail);
    CC_SAFE_RELEASE(_mailArray);
}

SGFriendMailLayer* SGFriendMailLayer::create(CCArray *mailArray)
{
    SGFriendMailLayer *mail = new SGFriendMailLayer();
    
    if(mail && mail->init(NULL, sg_friendMail))
    {
        mail->_mailArray = CCArray::create();
        mail->_mailArray->retain();
        for (int ii = 0; ii < mailArray->count(); ii++) {
            CCObject *obj = mailArray->objectAtIndex(ii);
            mail->_mailArray->addObject(obj);
        }
        
        mail->viewDidLoad();
        mail->autorelease();
        return mail;
    }
    
    CC_SAFE_DELETE(mail);
    return NULL;
}

SNSTableViewCellItem* SGFriendMailLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGFriendMailCell *cell = (SGFriendMailCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGMailDataModel *mail = (SGMailDataModel*)_mailArray->objectAtIndex(indexPath->getRow());
    
    if(NULL == cell)
    {
        cell = SGFriendMailCell::create(_isDeleteModel,mail);
    }else
    {
        cell->update(_isDeleteModel,mail);
    }
    
    return cell;
}

void SGFriendMailLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMailDataModel *dataModel = (SGMailDataModel*)_mailArray->objectAtIndex(indexPath->getRow());
    SGFriendMailContent *content = SGFriendMailContent::create(dataModel);
    SGMainManager::shareMain()->showLayer(content);
}

void SGFriendMailLayer::viewDidLoad()
{
    this->datas = _mailArray;

    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMail);
    
    /************************页面布局********************************/
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    //星形背景
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_friendMail);
    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30+150+72, spriteBackground->getContentSize().width, SGLayout::getSkewingY(480)-47-150-72-61-101);
    spriteBackground->setTextureRect(r);
    spriteBackground->setScaleX(s.width/spriteBackground->getContentSize().width * 1.01);
    spriteBackground->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72)));
    spriteBackground->setAnchorPoint(ccp(0.5, 1));
    
    this->addChild(spriteBackground);
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    //标题
    SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(str_FriendMailLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//spriteTitle->setColor(ccc3(0xff, 0x95, 0x0c));
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72/2)));
    //spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -s.height*0.23)));

    this->addChild(spriteTitle);
    
    //五张图片的尺寸   -79-150-72-56-101-30
//    SGButton *buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png", this, menu_selector(SGFriendMailLayer::buttonClickGoBack),CCPointZero,false,true);
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGFriendMailLayer::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
	
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    buttonBack->setPosition(ccpAdd(upLeft, ccp(0,  -h+65/2)));
  
//      buttonBack->setPosition(ccpAdd(upLeft, ccp(0,  -s.height*0.23)));
    addBtn(buttonBack);
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    
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
    
    /*
     模式按钮
     friend/friend_deleteModel.png 删除
     friend/friend_sendModel.png   发送
     */
    buttonModel = SGButton::create("chatbtn.png", "friend_deleteModel.png", this, menu_selector(SGFriendMailLayer::buttonClickModelChange),CCPointZero,false,true);
    buttonModel->setAnchorPoint(ccp(1, 0.5));
    buttonModel->setPosition(ccp(SGLayout::getSkewing(320) ,buttonBack->getPositionY()));
    addBtn(buttonModel);
    
    //tableview
    this->tableView->setFrame(CCRectMake(0,101+61, SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-47-150-72-63-101));
    this->tableView->setShowSlider(false);
    this->tableView->setZOrder(999);
    tableViewHeight = 110;
    tableView->setDown(-25);
    //没有收到任何消息
    m_pNoMailLabel = SGCCLabelTTF::create(str_FriendMailLayer_str2, FONT_BOXINFO, 28);
    m_pNoMailLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -44)));
    addChild(m_pNoMailLabel);
    m_pNoMailLabel->setVisible(this->datas->count() == 0);
    
}

void SGFriendMailLayer::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMail *mail = SGMail::create();
    SGMainManager::shareMain()->showLayer(mail);
}

void SGFriendMailLayer::buttonClickModelChange()
{
	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMail);
    EFFECT_PLAY(MUSIC_BTN);
    //点击就切换图片
    if(_isDeleteModel)
    {
        buttonModel->setFontImage(CCSprite::createWithSpriteFrameName("friend_deleteModel.png")->displayFrame());
    }else
    {
        buttonModel->setFontImage(CCSprite::createWithSpriteFrameName("friend_sendModel.png")->displayFrame());
    }
    
    _isDeleteModel = !_isDeleteModel;
    tableView->reloadData();
}

void SGFriendMailLayer::deleteMailListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        printf("error deleteMailListener !!!  \n");
        return;
    }
    
    main::DeleteMailResponse *response = (main::DeleteMailResponse*)request->m_msg;
    if(1 == response->state())
    {
        SG_SHOW_WINDOW(str_FriendMailLayer_str3);
        if(0 != _waitBeDeletedMailId)
        {
            /* 从本地数组中删除响应的元素，只能通过静态数据记录发送删除的邮件 */
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getFriendMails();
            CCObject *object = NULL;
            CCARRAY_FOREACH(array, object)
            {
                SGMailDataModel *mailmodel = (SGMailDataModel*)object;
                if(_waitBeDeletedMailId == mailmodel->getMailSsid())
                {
                    array->removeObject(mailmodel);
                }
            }
        }
        
        tableView->reloadData();
    }else
    {
        SG_SHOW_WINDOW(str_FriendMailLayer_str4);
    }
}

void SGFriendMailLayer::recieveReloadUpdate(SGMailDataModel* model)
{
    if (datas->containsObject(model)) {
        datas->removeObject(model);
        tableView->reloadData();
    }
    m_pNoMailLabel->setVisible(this->datas->count() == 0);
}

