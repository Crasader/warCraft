//
//  SGSystemMailCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#include "SGSystemMailCell.h"
#include "SGLayout.h"
#include "SGButton.h"
#include "SGMail.h"
#include "SGTools.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGPlayerInfo.h"
#include "SGSystemMailLayer.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGSystemMailCell::SGSystemMailCell() : buttonDelete(NULL)
{
    labelContent = NULL;
    labelNickName = NULL;
    labelSendTime = NULL;
    spriteUnread = NULL;
    spriteAccessory = NULL;
    _mailData = NULL;
    _isDeleteModel = false;
}

SGSystemMailCell::~SGSystemMailCell()
{
    //ResourceManager::sharedInstance()->unBindLayerTexture(sg_systemMail);
}

SGSystemMailCell* SGSystemMailCell::create(bool isDeleteModel,SGMailSystemDataModel *mailData)
{
    SGSystemMailCell *cell = new SGSystemMailCell();
    if(cell && cell->init())
    {
        cell->_isDeleteModel = isDeleteModel;
        cell->_mailData = mailData;
		
        cell->viewDidLoad();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;
}

void SGSystemMailCell::viewDidLoad()
{
	ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_systemMail);
    //背景
    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    this->addChild(spriteBackground);
    bgbgbg = spriteBackground;
	
    //好友昵称
    labelNickName = SGCCLabelTTF::create(str_FriendMailLayer_str11, FONT_BOXINFO, 24);
    labelNickName->setAnchorPoint(ccp(0, 0.5));
    labelNickName->setPosition(ccp(-SGLayout::getSkewing(50), 20));
    addChild(labelNickName);
    
    //发送时间
    labelSendTime = SGCCLabelTTF::create(str_FriendMailLayer_str12, FONT_BOXINFO, 24);
    labelSendTime->setAnchorPoint(ccp(1, 0.5));
    labelSendTime->setPosition(ccp(SGLayout::getSkewing(80), 20));
    addChild(labelSendTime);
	
    //未读
    spriteUnread = CCSprite::createWithSpriteFrameName("mail_unread_server.png");
    spriteUnread->setAnchorPoint(ccp(0, 0.5));
    spriteUnread->setPosition(ccp(-SGLayout::getSkewing(100), 15));
    addChild(spriteUnread);
    
    //内容
    labelContent = SGCCLabelTTF::create(str_SearchFriend_str13, FONT_BOXINFO, 24);
    labelContent->setAnchorPoint(ccp(0, 0.5));
    labelContent->setPosition(ccp(-SGLayout::getSkewing(70), -20));
    addChild(labelContent);
    
    //删除按钮
    CCMenu *menu = CCMenu::create();
    buttonDelete = SGButton::create("friend_redButton.png","friend_delete.png", this, menu_selector(SGSystemMailCell::buttonClickDeleteMail),CCPointZero,false,true);
    buttonDelete->setAnchorPoint(ccp(1,0.5));
	//    buttonDelete->setPosition(ccp(SGLayout::getSkewing(130),0));
    buttonDelete->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(spriteBackground->getContentSize().width/2-buttonDelete->getContentSize().width/2, 0)));
    menu->setPosition(CCPointZero);
    menu->addChild(buttonDelete);
    addChild(menu);
    
    //附件
    spriteAccessory = CCSprite::createWithSpriteFrameName("mail_accessory.png");
    spriteAccessory->setAnchorPoint(ccp(1,0.5));
    spriteAccessory->setPosition(ccp(-SGLayout::getSkewing(90), -20));
    addChild(spriteAccessory);
    
    update(_isDeleteModel,_mailData);
}

void SGSystemMailCell::buttonClickDeleteMail()
{
    buttonDelete->setEnabled(false);
    EFFECT_PLAY(MUSIC_BTN);
    main::DeleteSystemMailRequest *deleteRequest = new main::DeleteSystemMailRequest();
    deleteRequest->set_mailid(_mailData->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_MAIL_DELETESYSTEMMAIL, deleteRequest);
    
    SGMainManager::shareMain()->getMainLayer()->setMailSelectId(_mailData->getSsid());
    
}

void SGSystemMailCell::update(bool isDeleteModel,SGMailSystemDataModel *mailData)
{
    
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_systemMail);
    _isDeleteModel = isDeleteModel;
    //更新内容
    _mailData = mailData;
    if (_mailData->getIsRead() && _mailData->getIsReceive() && _isDeleteModel) {
        buttonDelete->setVisible(_isDeleteModel);
    }
    else
        buttonDelete->setVisible(false);
    
    
    labelSendTime->setVisible(!_isDeleteModel);
    
    
    labelSendTime->setString(SGTools::getTimeDistanceNow(_mailData->getSendTime()).c_str());
    
    std::string content;
    content.append(getFirstTenWords(_mailData->getcontent()->getCString(), 10));
    labelContent->setString(content.c_str());
    
    //是否已读
    if(_mailData->getIsRead())
    {
        spriteUnread->setDisplayFrame(CCSprite::createWithSpriteFrameName("mail_read_server.png")->displayFrame());
        spriteAccessory->setVisible(!_mailData->getIsReceive());
        
    }else
    {
        spriteUnread->setDisplayFrame(CCSprite::createWithSpriteFrameName("mail_unread_server.png")->displayFrame());
        spriteAccessory->setVisible(!_mailData->getIsReceive());
    }
    
    //是否有附件
    if(_mailData->getIsReceive())
    {
        spriteAccessory->setVisible(false);
        spriteUnread->setPosition(ccp(-SGLayout::getSkewing(110), 0));
    }else{
        spriteAccessory->setVisible(true);
        spriteUnread->setPosition(ccp(-SGLayout::getSkewing(100), 15));
    }
}

/*
 void SGSystemMailCell::deleteMailListener(cocos2d::CCObject *object)
 {
 SGSocketRequest *request = (SGSocketRequest*)object;
 if(!request)
 {
 CCLOG("删除邮件失败");
 return;
 }
 
 main::DeleteSystemMailResponse *response = (main::DeleteSystemMailResponse*)request->m_msg;
 if(1 == response->state())
 {
 SGPlayerInfo::sharePlayerInfo()->getSystemMails()->removeObject(_mailData);
 //更新数据
 //        SGSystemMailLayer *layer = (SGSystemMailLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_systemMail);
 //        layer->recieveReloadUpdate(_mailData);
 
 CCArray *datas = SGPlayerInfo::sharePlayerInfo()->getSystemMails();
 SGSystemMailLayer *system = SGSystemMailLayer::create(datas);
 SGMainManager::shareMain()->showLayer(system);
 
 }
 }
 */
std::string SGSystemMailCell::getFirstTenWords(const char *charBuffer,int size)
{
    
    int index = 0;          //字符串的当前位置
    int counter = 0;        //汉字计数器
    int sizeCounter = 0;    //需要多少个字
    
    std::string returnString;
    
#if (PLATFORM == IOS)
    while (charBuffer[index] != '\0') {
        char ch = charBuffer[index];
        if(ch >= 0 && ch < 256)
        {
            returnString.append(&ch);
            
            sizeCounter ++;
            if(sizeCounter == size)
            {
                //已达到截取字符长度
                break;
            }
            
        }else{
            counter ++;
            if(counter == 3)
            {
                //用三个字符存储一个汉字，这里我们就截取三个字符
                char mm[4];
                mm[0] = charBuffer[index-2];
                mm[1] = charBuffer[index-1];
                mm[2] = charBuffer[index];
                mm[3] = '\0';
				
                returnString.append(mm);
                sizeCounter ++;
                if(sizeCounter == size)
                {
                    break;
                }
                
                counter = 0;
                
            }
            
        }
        
        index ++;
        
    }
#else
	char buff[8] = "\0";
	//ascii 字符:< 0x80 中文标点: <0xe0  中文: < 0xf0
	while (charBuffer[index] != '\0')
	{
		
		//如果是ascii字符
		if (charBuffer[index] < 0x80)
		{
			CCString *temp = CCString::createWithFormat("%c", charBuffer[index]);
			returnString.append(temp->getCString());
			index++;
			sizeCounter++;
		}
		else if (charBuffer[index] < 0xe0)
		{
			index += 2;
			sizeCounter++;
			
			buff[0] = charBuffer[index - 2];
			buff[1] = charBuffer[index - 1];
			buff[2] = '\0';
			
			returnString.append(buff);
		}
		else if (charBuffer[index] < 0xf0)
		{
			index += 3;
			sizeCounter++;
			
			buff[0] = charBuffer[index - 3];
			buff[1] = charBuffer[index - 2];
			buff[2] = charBuffer[index - 1];
			
			buff[3] = '\0';
			
			returnString.append(buff);
		}
		
		if (sizeCounter >= size)
		{
			break;
		}
    }
#endif
    if(sizeCounter == 10)
    {
        returnString.append("....");
    }
    
    return returnString;
	
}
