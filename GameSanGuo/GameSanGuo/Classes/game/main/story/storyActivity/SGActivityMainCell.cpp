//
//  SGActivityMainCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGActivityMainCell.h"
#include "GameConfig.h"
#include "SGCCLabelTTF.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
using namespace std;
/*
const char* getTimeString(time_t tt_time)
{
    time_t now_time = time(NULL);
    time_t dis_time = (tt_time - now_time);
    
    tm* t = localtime(&dis_time);
    
    CCString *sTime = CCString::createWithFormat("%d天%d小时%d分%d秒", t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return sTime->getCString();
}
*/
SGActivityMainCell::SGActivityMainCell() : spriteIsPass(NULL), labelName(NULL), labelDesc(NULL), limit_time(0), isStartActivity(false), isCountClose(false),
activityBean(NULL), labelTime(NULL), m_labelChallengeTimes(NULL), activityId(NULL), delegate(NULL), tableview(NULL)
{
    ;
}

SGActivityMainCell::~SGActivityMainCell()
{
	//ResourceManager::sharedInstance()->unBindTexture("story/story2.plist", RES_TYPE_LAYER_UI, sg_activityDetail);
	//ResourceManager::sharedInstance()->unBindLayerTexture(sg_activityDetail);
}

SGActivityMainCell* SGActivityMainCell::create(SGActivityMainCellDelegate *del,SNSTableView*table)
{
    SGActivityMainCell *actionCell = new SGActivityMainCell();
    if(actionCell && actionCell->init())
    {
        actionCell->delegate=del;
        actionCell->tableview=table;
        actionCell->viewDidLoad();
        actionCell->autorelease();
        return actionCell;
    }
    
    CC_SAFE_DELETE(actionCell);
    return actionCell;
}

void SGActivityMainCell::viewDidLoad()
{
	//ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_activityDetail);
	
    
    ///////////
    btn=SGButton::create("upwar_cell_bg.png", NULL, this, menu_selector(SGActivityMainCell::buttonClick),CCPointZero,false,true);
    this->addChild(btn);
    addBtn(btn);
    //////////
    spriteIsPass = CCSprite::createWithSpriteFrameName("Label_win6.png");
    spriteIsPass->setPosition(ccp(-bgSize.width/2+spriteIsPass->getContentSize().width*.5f, bgSize.height*.5f-spriteIsPass->getContentSize().height*.5f));
    labelName = CCLabelTTF::create(str_ActivityMainCell_str1, FONT_PANGWA, FONTSIZE(17));
    labelDesc = SGCCLabelTTF::create(str_ActivityMainCell_str2, FONT_BOXINFO, FONTSIZE(12));
    labelTime = SGCCLabelTTF::create(str_ActivityMainCell_str3, FONT_BOXINFO, FONTSIZE(12));
    m_labelChallengeTimes = SGCCLabelTTF::create(CCString::createWithFormat(str_ActivityMainCell_str101,0,0)->getCString(), FONT_BOXINFO, FONTSIZE(12));
    m_labelChallengeTimes->setAnchorPoint(ccp(0, 0.5f));
    
    labelName->setAnchorPoint(ccp(0, 0.5f));
    labelName->setPosition(ccp(-bgSize.width/2+skewing(30), labelName->getPositionY()));
    labelDesc->setAnchorPoint(ccp(1.0f, 0.5f));
    labelDesc->setPosition(ccp(bgSize.width/2-skewing(25) - 75, labelDesc->getPositionY()+10));
    
    labelTime->setAnchorPoint(ccp(1.0f, 0));
    labelTime->ignoreAnchorPointForPosition(false);
    labelTime->setPosition(ccp(labelDesc->getPosition().x + labelDesc->getContentSize().width + labelTime->getContentSize().width / 2, labelDesc->getPositionY()+10));
    m_labelChallengeTimes->setPosition(ccp(labelDesc->getPosition().x -m_labelChallengeTimes->getContentSize().width / 4-10 , labelDesc->getPositionY()-20));
    this->addChild(spriteIsPass);
    this->addChild(labelName);
    this->addChild(labelTime);
    this->addChild(labelDesc);
    this->addChild(m_labelChallengeTimes);
}

void SGActivityMainCell::flashTimer()
{
	/*
	 
	 即将开启的副本，显示 开启剩余:x小时，低于1小时显示分钟，低于1分钟都显示1分钟，红色字体。
	 
	 */
	
    limit_time -= 1;
    int tempMin = limit_time;
	std::string times = str_ActivityMainCell_str4;
	
	if (limit_time <= 1) {
        // 刷新数据，将过期的数据隐藏掉
        this->unschedule(schedule_selector(SGActivityMainCell::flashTimer));
        labelTime->setString(" ");
        labelDesc->setString(" ");
    }
	
	if (limit_time >= 3600)
	{
        CCString *timeStr = CCString::createWithFormat(str_ActivityMainCell_str5, (limit_time / 3600));
        labelTime->setString(timeStr->getCString());
        tempMin = limit_time / 3600;
		
		if (isStartActivity)
		{
            labelDesc->setString(str_ActivityMainCell_str6);
            isStartActivity = false;
        }
    }
	else if (limit_time > 60 && limit_time < 3600)//一个小时
	{
		CCString *timeStr = CCString::createWithFormat(str_ActivityMainCell_str7, (limit_time / 60));
        labelTime->setString(timeStr->getCString());
        tempMin = limit_time / 60;
		times = str_ActivityMainCell_str8;
		if (isStartActivity)
		{
            labelDesc->setString(str_ActivityMainCell_str6);
            isStartActivity = false;
        }
	}
	else//小于等于一分钟的情况
	{
		CCString *timeStr = CCString::createWithFormat(str_ActivityMainCell_str7, 1);
        labelTime->setString(timeStr->getCString());
        tempMin = 1;
		times = str_ActivityMainCell_str8;
		if (isStartActivity)
		{
            labelDesc->setString(str_ActivityMainCell_str6);
            isStartActivity = false;
        }
	}
	
	
	
    if (isCountClose) {
        CCString *timeStr = CCString::createWithFormat("%d %s", tempMin, times.c_str());
        labelTime->setString(timeStr->getCString());
    }

}

void SGActivityMainCell::setData(SGStoryBean *bean)
{
    //定时器时长控制
    int scheduleType = 0;
    
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_activityDetail);
    this->unschedule(schedule_selector(SGActivityMainCell::flashTimer));
    activityId = bean->getStoryId();
    isStartActivity = false;
    isCountClose = false;
    
    if (activityBean) {
        if (activityBean->getActivityStartTime() > 0) {
            activityBean->setActivityStartTime(limit_time);
        }
        if (activityBean->getActivityEndTime() > 0) {
            activityBean->setActivityEndTime(limit_time);
        }
    }
    activityBean = bean;
    long activityStartTime = bean->getActivityStartTime();
    long activityEndTime = bean->getActivityEndTime(); 
    //long currentTime = getCurrentTime();
    CCString *timeStr = NULL;
    /*
    struct tm * ptm = localtime(&activityStartTime);
    char stime[21] = {0};
    strftime(stime, 20, "%H:%M:%S", ptm);
    std::string pRet = stime;
    CCLOG("ServerTime: %s",pRet.c_str());
    */
	labelTime->setVisible(true);
    labelDesc->setVisible(true);
    if (activityStartTime > 0)
	{
        //开始活动
        limit_time = (int)(activityStartTime);
        if (limit_time >= 3600)//只有一个小时
        {
            //timeStr = CCString::createWithFormat("%s", pRet.c_str());
			timeStr = CCString::createWithFormat(str_ActivityMainCell_str5, limit_time / 3600);
            isStartActivity = true;
            scheduleType = 1;//全小时显示，定时器间隔60s
        }
        else if (limit_time > 60 && limit_time < 3600)//小于一小时的显示分钟
        {
            timeStr = CCString::createWithFormat(str_ActivityMainCell_str7, limit_time / 60);
			isStartActivity = true;
            scheduleType = 2;//分钟显示，定时器间隔30s
        }
		else
		{
			timeStr = CCString::createWithFormat(str_ActivityMainCell_str7, 1);
			isStartActivity = true;
            scheduleType = 2;//分钟显示，定时器间隔30s
		}
		labelDesc->setString(str_ActivityMainCell_str6);
        labelTime->setVisible(true);
        labelDesc->setVisible(true);
        labelTime->setInsideColor(ccRED);
        labelDesc->setInsideColor(ccRED);
    }
    else if (activityStartTime < 0 && activityEndTime > 0)
    {
		/*
		 开启中的副本，显示 关闭剩余:x小时，低于1小时显示分钟，低于1分钟都显示1分钟，绿色字体。
		 */
		
        //结束活动
        limit_time = (int)(activityEndTime);
		
		//如果一个小时还多
		if (limit_time >= 3600)
		{
			//显示几个小时
			timeStr = CCString::createWithFormat(str_ActivityMainCell_str5, limit_time / 3600);
            scheduleType = 1;//全小时显示，定时器间隔60s
		}
		else if (limit_time < 3600 && limit_time > 60)
		{
			//显示剩余多少分钟
			timeStr = CCString::createWithFormat(str_ActivityMainCell_str7, limit_time / 60);
            scheduleType = 2;//分钟显示，定时器间隔30s
		}
		else
		{
			//只显示一分钟
			timeStr = CCString::createWithFormat(str_ActivityMainCell_str7, 1);
            scheduleType = 2;//分钟显示，定时器间隔30s
		}
		
		
        labelDesc->setString(str_ActivityMainCell_str9);
        labelTime->setVisible(true);
        labelDesc->setVisible(true);
        isCountClose = true;
        labelTime->setInsideColor(ccGREEN);
        labelDesc->setInsideColor(ccGREEN);
    }
    else
    {
        //活动已结束
        labelTime->setVisible(false);
        labelDesc->setVisible(false);
        timeStr =  CCString::createWithFormat(" ");
    }
 
    labelTime->setString(timeStr->getCString());
    
    labelName->setString(bean->getStoryName()->getCString());
    if (scheduleType == 1)//全小时显示，间隔60s
    {
        this->schedule(schedule_selector(SGActivityMainCell::flashTimer), 60.0f);
    }
    else if (scheduleType == 2)//全分钟显示，间隔30s
    {
        this->schedule(schedule_selector(SGActivityMainCell::flashTimer), 30.0f);
    }
    else//出现错误，不属于任何类型，间隔1s
    {
        this->schedule(schedule_selector(SGActivityMainCell::flashTimer), 1.0f);
    }
    
    if(bean->getIsWin())
	{
        spriteIsPass->setDisplayFrame(CCSprite::createWithSpriteFrameName("Label_win5.png")->displayFrame());
    }else
	{
        spriteIsPass->setDisplayFrame(CCSprite::createWithSpriteFrameName("Label_win6.png")->displayFrame());
    }
    m_labelChallengeTimes->setString(CCString::createWithFormat(str_ActivityMainCell_str101,bean->getChallengeCount(),bean->getMaxChallengeCount())->getCString());
}

long SGActivityMainCell::getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec;
}
///////
void SGActivityMainCell::buttonClick()
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);
}

