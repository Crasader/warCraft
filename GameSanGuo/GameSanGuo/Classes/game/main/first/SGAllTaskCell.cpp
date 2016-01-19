//
//  SGAllTaskCell.cpp
//  GameSanGuo
//
//  Created by kanata on 14-3-27.
//
//

#include "SGAllTaskCell.h"
#include "SimpleAudioEngine.h"
#include "GameMusicmanage.h"
#include "SGMailSystemDataModel.h"
#include "DayTaskModule.pb.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"
#include "SGAllTaskLayer.h"



SGAllTaskCell::SGAllTaskCell()
:labelstring(NULL),
ling(NULL),
entertype(0),
statestate(0)
{



}
SGAllTaskCell::~SGAllTaskCell()
{

}

SGAllTaskCell* SGAllTaskCell::create(SGTaskDelegate*del,CCString*string,int type)
{
    SGAllTaskCell *cell = new SGAllTaskCell();
    if (cell && cell->init()) {
        cell->delegate=del;
        cell->entertype=type;
        cell->labelstring=string;
        cell->initView();
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;


}

void SGAllTaskCell::initView()
{
    ling=SGButton::create("reward_lingq_hui.png", NULL, this,menu_selector(SGAllTaskCell::lingling),CCPointZero,false,true );
    ling->setPosition(ccp(200/*btn->getContentSize().width*0.5-zhan->getContentSize().width*0.78*/, 0));
    SNSTableViewCellItem::addBtn(ling);
    SGButton* btn=SGButton::create("item_bigbg.png", NULL, this,menu_selector(SGAllTaskCell::showSectionInfoHandler),CCPointZero,false,true );
    
    this->addChild(btn);
    SNSTableViewCellItem::addBtn(btn);
    this->addChild(ling);
 
    bgSize = btn->getContentSize();
    
    SGCCLabelTTF *name = SGCCLabelTTF::create(labelstring->getCString(), FONT_PANGWA, 36);
    name->setAnchorPoint(ccp(0, 0.5));
    name->setPosition(ccpAdd(btn->getPosition(), ccp(-btn->getContentSize().width*.4, btn->getContentSize().height*0.0)));
    this->addChild(name);
    
    

}


void SGAllTaskCell::updateview(int state,int tagtag,int entertype)
{
     //ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_alltasklayer);
    
    statestate=state;
   if(entertype==0)
   {
        if(state==0) //0不满足  1已领取 2满足未领取
        {
            ling->setImage("reward_lingq_hui.png");
        }
            
        else if(state==1)
        {
            ling->setImage("reward_font_ylq.png");
        }
        else
        {
            ling->setImage("reward_lingq.png");
        }
   }
    
    else
    {
        if(state==2)
            ling->setImage("reward_lingq.png");
        else
            ling->setImage("reward_lingq_hui.png");
            
    
    
    
    
    }
    ling->setTag(tagtag);
    
}



void SGAllTaskCell::showSectionInfoHandler(CCNode*node)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int tag=node->getTag();
    if (delegate) {
        delegate->showSectionInfoHandler(this,tag);
    }
    
}

void SGAllTaskCell::entrySectionHandler(CCNode*node)
{
    EFFECT_PLAY(MUSIC_BTN);
    int tag=node->getTag();
    if (delegate) {
        delegate->entrySectionHandler(this,tag);
    }

}

void SGAllTaskCell::lingling(CCNode*node)
{
    EFFECT_PLAY(MUSIC_BTN);
    int tag=node->getTag();
    tag=node->getTag();
    
    if(!SGAllTaskLayer::m_buttonCanTouch)
        return;
    
    
    if(entertype==0)
    {
        
        
        if(statestate==0)
        {
             SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str1);
            return;
            
        }
        if(statestate==1)
        {
            SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str2);
            return;
        }
        if(statestate==2)
        {
            SGAllTaskLayer::m_buttonCanTouch = false;
            
            main::DayTaskGetRewardRequest *request = new main::DayTaskGetRewardRequest();
            request->set_taskid(tag);
            SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_STATUS, request);
            SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(tag);
            if (task) {
                SGMainManager::shareMain()->trackTaskCompleted(task->gettaskname());
            }
        }
        
        
        
        


    }
    
    
    else
    {
        
        
        
        
        if(statestate==2)
        {
            SGAllTaskLayer::m_buttonCanTouch = false;
            
          main::ReceiveQuestRewardRequest *request = new main::ReceiveQuestRewardRequest();
          request->set_questid(tag);
          SGSocketClient::sharedSocketClient()->send(MSG_GETMAINTASK_REWARD, request);
            
            if (delegate)
            {
                delegate->m_questId = tag;
            }
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str1);
        }
            
    }
    
}

    
    
    
    
    
    
    
    
