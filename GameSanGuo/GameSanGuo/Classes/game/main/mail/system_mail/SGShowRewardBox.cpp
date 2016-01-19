//
//  SGShowRewardBox.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-1-2.
//
//

#include "SGShowRewardBox.h"
#include "SGMainManager.h"
#include "SGStringConfig.h"
#include "SGGuideManager.h"

SGShowRewardBox::SGShowRewardBox() : colomNum(0) , rowNUm(0) , showTaskGuide(false)
{
    _datas=NULL;
    entryTrpe = 0;
}
SGShowRewardBox::~SGShowRewardBox(){}

SGShowRewardBox *SGShowRewardBox::create(SGBoxDelegate *target,CCArray *data,int num,int entryTrpe){
    
    int colomNum=ceil(data->count()/(num+0.1));
    
    if(colomNum>=3)
        colomNum=3;
    int height=130+70*colomNum;
    CCSize boxSize=CCSizeMake(skewing(280), height*2);

    SGShowRewardBox *box=new SGShowRewardBox();
    if (box->init(target, box_showRewardBox, boxSize)) {
        box->_datas=CCArray::create();
        box->_datas->retain();
        box->_datas->initWithArray(data);
        box->colomNum=colomNum;
        box->rowNUm=num;
        box->entryTrpe = entryTrpe;
        box->initView();
//        box->boxSize=boxSize;
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
    

}

void SGShowRewardBox ::initView(){

    this->setTouchEnabled(true);

    SGButton*ok= SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGShowRewardBox::btncloseBox), CCPointZero, false, true);
    
    ok->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -fontbg->getContentSize().height*0.45-ok->getContentSize().height)));
    
    //新手引导最后一步，要领取奖励，这里的按钮高亮，guide_tag_48
    int tag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_48);
    ok->setTag(tag);
    
    addBtn(ok);
    
    
    SGCCLabelTTF*label = NULL;
    showTaskGuide = false;
    if (entryTrpe == STORYREWARDTAG ||entryTrpe == STORYREWARDAUTO)
    {
       label = SGCCLabelTTF::create(str_FriendMailLayer_str10, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
    }
    else if(entryTrpe == JINGYANDAN)
    {
        label = SGCCLabelTTF::create(str_SoulExchangeLayer_str10, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
    }
    else if (entryTrpe == SPBATTLE_PREVIEW)
    {
        //str_SpecialBattlePreview
        label = SGCCLabelTTF::create(str_SpecialBattlePreview, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));

    }
    else
    {
       label = SGCCLabelTTF::create(str_SoulExchangeLayer_str9, FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
       showTaskGuide = true;
    }
    label->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-fontbg->getContentSize().width*0, fontbg->getContentSize().height*0.65)));
    this->addChild(label);
    
    float propY=SGLayout::getProportion();
    CCSize showIconSize= CCSizeMake(240, (boxSize.height-152)/propY);
    if (this->_datas->count() > 0 )
    {
        SGShowHeadIconLayer *iconLayer=SGShowHeadIconLayer::create(this->_datas, rowNUm, showIconSize);
        this->addChild(iconLayer);
        iconLayer->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width/2+10, -fontbg->getContentSize().height/2-15)));
    }

}
void SGShowRewardBox::btncloseBox(){


    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGBaseLayer *layer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    SGBaseLayer *mainlayer = (SGBaseLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainlayer->setIsCanTouch(true);
    if (layer != NULL)
    {
        layer->setIsCanTouch(true);
    }
	
	if (entryTrpe == 112)
	{
		SGMainManager::shareMain()->closeBox();
	}
	else
	{
		this->boxClose();
        //SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(false);
		this->removeFromParentAndCleanup(true);
	}
    //点击之后，引发领取奖励之后的逻辑
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId >= guide_tag_48)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_49);
    }
    

}

void SGShowRewardBox::registerWithTouchDispatcher()
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId < UIG_MAX1)
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    }
    else
    {
        //本层层层级提高，对应放sgbutton的menu也需要提高触摸优先级，不然层会阻拦按钮点击
        menu->setTouchPriority(-200);
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -200, true); //貌似这个会跟任务大列表等界面上的button重叠，需要让其优先级更高。
    }
}

bool SGShowRewardBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
