//
//  SGAnnouncementLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-8.
//
//

#include "SGAnnouncementLayer.h"
#include "SGTools.h"
#include "SGMainManager.h"
#include "SGAnnouncementDataModel.h"
#include "SGFirstLayer.h"
#include "SGCCLabelTTF.h"
#include "SGAnnounceMsg.h"
#include "ResourceManager.h"
#include "SGRichLabel.h"
#include "AppMacros.h"
#include "SGMenu.h"
#include "SGStringConfig.h"
#include "SGBattleManager.h"
#include <unistd.h>
enum {TAG_Flower1 = 101, TAG_Flower2};//花瓣的tag值
#define UP_SHIFT 15//内容窗口拼装纵向偏移
#define FLOWER_NUM 5//花瓣的产生组数
///////////////////////////////////////////////////////////////////


static float widthFactor =0;
static float heightFactor =0;


SGAnnouncementLayer::SGAnnouncementLayer():
m_scrollview(NULL),
buttonOK(NULL),
_delegate(NULL),
menu(NULL),
m_arrayAnn(NULL),
scaley(0.0),
iscanmove(false)
{
    m_scrollview = NULL;
    m_arrayAnn = NULL;
    _delegate = NULL;
    m_clickPos = CCPointZero;
    scaley = 1.0;
}

SGAnnouncementLayer::~SGAnnouncementLayer()
{
	//10.25
    m_arrayAnn->release();
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_announcementLayer);

}

SGAnnouncementLayer* SGAnnouncementLayer::create(main::AnnouncementResponse &response, SGBoxDelegate *delegate)
{
    SGAnnouncementLayer *announcement = new SGAnnouncementLayer();
    if(announcement)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSize winPix = CCDirector::sharedDirector()->getWinSizeInPixels();
        CCSize visSize = CCDirector::sharedDirector()->getVisibleSize();
        CCSize visOrg = CCDirector::sharedDirector()->getVisibleOrigin();
        widthFactor = winSize.width / 640.0;
        heightFactor = winSize.height / 960.0;
        CCLOG("announce || winSizeWidth=%f, winSizeHeight=%f, winPixWidth=%f, winPixHeight=%f, visSizeWidth=%f, visSizeHeight=%f, visOrgWidth=%f, visOrgHeight=%f, widthFactor=%f, heightFactor = %f" , winSize.width, winSize.height, winPix.width, winPix.height
              , visSize.width, visSize.height, visOrg.width, visOrg.height, widthFactor, heightFactor);
        
        announcement->_delegate = delegate;
        announcement->deletage = delegate;
        announcement->viewDidLoad(response);
        announcement->autorelease();
        
       
        
        return announcement;
    }
    CC_SAFE_DELETE(announcement);
    return NULL;
}
//重写对应的onEnter方法，设置menu的优先级
void SGAnnouncementLayer::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(-256);
}

void SGAnnouncementLayer::viewDidLoad(main::AnnouncementResponse &response)//cgp come
{
    this->setTouchEnabled(true);
    
    this->setTag(box_announcementLayer);

    ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_announcementLayer);
	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer2.plist", RES_TYPE_LAYER_UI, sg_announcementLayer);
	ResourceManager::sharedInstance()->bindTexture("sglandingreward/sglandingreward.plist", RES_TYPE_LAYER_UI, sg_announcementLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_announcementLayer);
	
    m_arrayAnn = CCArray::create();
    m_arrayAnn->retain();
    
    //CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    //加个半透明黑背景
    CCLayerColor *blackLayer = CCLayerColor::create(ccc4(0, 0, 0, 150), SGLayout::getSkewing(380), SGLayout::getSkewingY(520));
	 blackLayer->setPosition(ccpAdd(ccp(0, 0), ccp(-385, -570)));
    addChild(blackLayer);
    
    //大个背景
//    CCSprite *sprite_background = CCSprite::create("gonggao/gonggao_bj.png");
    CCSprite *sprite_background = CCSprite::createWithSpriteFrameName("bg.png");
    addChild(sprite_background);
    
	
    //cgp useful
    //按钮
   	buttonOK = SGButton::create("gonggao_an.png", "public_font_queding.png", this, menu_selector(SGAnnouncementLayer::buttonClickOK),CCPointZero,false,true);
    buttonOK->setPosition(ccp(sprite_background->getContentSize().width * 0.5 - 5, sprite_background->getContentSize().height * 0.05));
//    addBtn(buttonOK);
	
	//遮挡住按钮穿透，临时做法
	SGButton *mask = SGButton::create("gonggao_an.png", NULL, this, menu_selector(SGAnnouncementLayer::pressMask), CCPointZero, false, true);
	mask->setScaleY(0.3);
	mask->setOpacity(0);
	mask->setPosition(ccp(buttonOK->getPositionX(), buttonOK->getPositionY() + 40));
	
    
    menu = CCMenu::create(buttonOK, mask, NULL);
	menu->setTag(254);
    menu->setPosition(CCPointZero);
    sprite_background->addChild(menu);
	
	

	//CCScrollView
    m_scrollview = CCScrollView::create(CCSizeMake(574, 761));
    CCLayer *container = CCLayer::create();
	
    container->setContentSize(CCSizeMake(574, 770));
	
    m_scrollview->setContainer(container);
	
    CCSize backgroundSize = sprite_background->getContentSize();
	m_scrollview->setPosition(ccpAdd(ccp(0, 0), ccp(-287, -382)));//设置scrollview位置
    m_scrollview->setDirection(kCCScrollViewDirectionVertical);
    addChild(m_scrollview);

    //解析接收到的数据
    int modelSize = response.announcementmodel_size();
    for(int i = 0;i < modelSize;i++)
    {
        main::AnnouncementModel model = response.announcementmodel(i);
        
        SGAnnouncementDataModel *ann = new SGAnnouncementDataModel();
        //ann->setTimeStamp(model.timestamp());
        ann->setTitle(CCString::create(model.title()));
        ann->setContent(CCString::create(model.content()));
		ann->setViewId(model.viewid());
		if (model.url() != "")//如果url不为空才设置对应的url
		{
			ann->setUrl(CCString::create(model.url()));
		}
        m_arrayAnn->addObject(ann);
        ann->autorelease();
    }
    
    //constructScrollView();
    initScrollView();
    //removed by cgp
	//this->flowerEffect();//花瓣效果
   // buttonClickOK();

}
//防止穿透产生，暂时做法
void SGAnnouncementLayer::pressMask()
{
	CCLOG("");
}

void SGAnnouncementLayer::flowerEffect()
{
	//  花瓣飘落效果========================================================================================================================================
	for (int i = 0; i < FLOWER_NUM; i++)
	{
		srand(time(NULL));
		CCSprite *flower1 = CCSprite::createWithSpriteFrameName("yh1.png");
		flower1->setRotation(-50);
		flower1->setAnchorPoint(ccp(0.0,3));
		flower1->setPosition(ccp(-( arc4random() % 550) + 250, arc4random()% 300 + 590));
		this->addChild(flower1,100,1);
		this->playFlowerAnim(flower1);
		CCSprite *flower2 = CCSprite::createWithSpriteFrameName("yh2.png");
		flower2->setRotation(-30);
		flower2->setAnchorPoint(ccp(0.0,2.0));
		flower2->setPosition(ccp(-( arc4random() % 550) + 250, arc4random()% 320 + 590));
		//this->playFlowerAnim(Flower2);
		CCSprite *flower3 = CCSprite::createWithSpriteFrameName("yh3.png");
		flower3->setRotation(-10);
		flower3->setAnchorPoint(ccp(0.0,1.0));
		flower3->setPosition(ccp(-( arc4random() % 550) + 250, arc4random()% 300 + 590));
		//this->playFlowerAnim(Flower3);
		
		this->addChild(flower2,101,2);
		CCCallFuncN *cc = CCCallFuncN::create(this,callfuncN_selector(SGAnnouncementLayer::playFlowerAnim));
		CCDelayTime *dt =CCDelayTime::create(rand()%3+1);
		CCFiniteTimeAction *ta = CCSequence::create(dt, cc, NULL);
		flower2->runAction(ta);
		
		this->addChild(flower3,101,3);
		CCCallFuncN *cc1 = CCCallFuncN::create(this,callfuncN_selector(SGAnnouncementLayer::playFlowerAnim));
		CCDelayTime *dt1 =CCDelayTime::create(rand()%2);
		CCFiniteTimeAction *ta1 = CCSequence::create(dt1, cc1, NULL);
		flower3->runAction(ta1);
		usleep(200);//每次生成对应的花瓣后间歇200ms
		
	}
	//  花瓣飘落效果========================================================================================================================================
	
}

void SGAnnouncementLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SGAnnouncementLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_clickPos = convertTouchToNodeSpace(pTouch);
 
	//CCLOG("%f, %f", m_clickPos.x, m_clickPos.y);
    CCLOG("posy====%f",m_clickPos.y);
    
//    CCPoint center = SGLayout::getPoint(kMiddleCenter);
//    CCLOG("center.y - 330==%f",center.y - 320);
//    CCLOG("center.y + 250==%f",center.y + 240);
	
    return true;
}




void SGAnnouncementLayer::buttonClickOK()
{
    
    EFFECT_PLAY(MUSIC_BTN);
    
    SGFirstLayer *firstLayer = (SGFirstLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
    
    if (firstLayer)
    {
        firstLayer->setIsCanTouch(true);
    }
    
    this->boxClose();
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    if(BT_PEACE != btp  &&  BT_LADDER!= btp  && BT_ARENA!=btp )
    {
        SGMainManager::shareMain()->showPveFinish();
    }
}

CCSize SGAnnouncementLayer::stringInLabelRect(const char *content)
{
    //计算所有字符的长度
    float allWidth = 0;
    //一个英文字符宽度
    float englishWidth = SGTools::getWidthOfOneEnglishWord('e', FONT_PENGYOU, 24);
    //一个汉语文字宽度
    float chineseWidth = SGTools::getWidthOfOneChineseWord(FONT_PENGYOU, 24);
    
    CCLOG("textWidth: eng=%f, chs=%f", englishWidth, chineseWidth);
    
    //580是以640为标准屏幕宽度时候，这些文本在设计上占据的最大宽度。
    float maybeLineWidth = 560 * widthFactor;
    
	/*
    //连续换行时 + allWidht加上正行的长度
    bool lastIsNewline = false;
    int index = 0;
    while (strlen(content) > index) {
        char ch = content[index];
        if(SGTools::charIsChinese(ch))
        {
            index += 3;
            allWidth += chineseWidth;
			lastIsNewline = false;
        }
        else if (ch == '\n')
        {
			if ( lastIsNewline )
			{
				index ++;
				allWidth += maybeLineWidth;
			}
			else
			{
				index ++;
				allWidth += maybeLineWidth * 0.55;
			}
			lastIsNewline = true;
        }
        else
        {
            index ++;
            allWidth += englishWidth;
			lastIsNewline = false;
        }
    }
    */
    
    //尝试真实计算。
    int index = 0;
    while (strlen(content) > index) {
        char ch = content[index];
        if(SGTools::charIsChinese(ch))
        {
            index += 3;
            allWidth += chineseWidth;
        }
        else if (ch == '\n')
        {
            index ++;
            allWidth += maybeLineWidth - (int)allWidth % (int)maybeLineWidth;
        }
        else
        {
            index ++;
            allWidth += englishWidth;
        }
    }

    
    float adjust = 1.0f;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    adjust = 1.0f;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    adjust = 0.64f;
#endif
    CCSize contentSize = CCSizeZero;
    contentSize.width = maybeLineWidth;
    contentSize.height = (allWidth / contentSize.width * adjust + 2.1)*24*heightFactor;

    return contentSize;
}


//创建单个的内容窗口
CCLayerColor *SGAnnouncementLayer::initContentWindow(CCString *content, bool isHaveBtn)
{
	//根据内容确定窗口大小
	CCSize s = stringInLabelRect(content->getCString());
	
    CCLOG("calFont: width=%f, height=%f" , s.width, s.height);
    //these are for IPAD!
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float subFix = 60;
    float subHeight = 0;
    if (winSize.width > 650)
    {
        subFix = 180;
        subHeight = 1.4 * 24;
    }
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(255,255,255,0), CCDirector::sharedDirector()->getWinSize().width - subFix, s.height + subHeight + (isHaveBtn ? 90 : 40) * heightFactor);
    return layer;
}



void SGAnnouncementLayer::initScrollView()
{
	//ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer2.plist", RES_TYPE_LAYER_UI, sg_announcementLayer);
    m_scrollview->getContainer()->removeAllChildrenWithCleanup(true);
    int positionY = 0;// Y轴纵向偏移，
	int border_shift = 20;//内容窗口偏移
	
    int text_wid_expand = 40;//文本区宽度扩展
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		text_wid_expand = 28;
	}
	
	//记录上一个内容窗口的底线位置
	float lastWindowBorderPos = 0.0f;
    for(int i = 0; i < m_arrayAnn->count(); i++)
	//for (int i = 0; i < 6; i++)
    {
		
		//=======================获取数据================================
		SGAnnouncementDataModel *ann = (SGAnnouncementDataModel *)m_arrayAnn->objectAtIndex(i);
		CCString *title = ann->getTitle();
		CCString *content = ann->getContent();

		int viewId = ann->getViewId();//跳转至哪个页面
		//=======================获取数据================================
		
        CCLayer *layer = (CCLayer*)m_scrollview->getContainer();//获取scrollview的容器

        CCLayerColor* contentWindow = initContentWindow(content, viewId > 0);
        contentWindow->setAnchorPoint(ccp(0.5, 0));
        contentWindow->ignoreAnchorPointForPosition(false);
        
        CCSize cwSize = contentWindow->getContentSize();
        
        CCSprite * sprite = CCSprite::createWithSpriteFrameName("Tips.png");
        sprite->setScaleX(cwSize.width / 584.0);
        sprite->setScaleY(cwSize.height / 522.0);
        sprite->setPosition(ccp(cwSize.width * 0.5, cwSize.height * 0.5));
        contentWindow->addChild(sprite, 2);
        
		//根据上一个内容窗口的位置计算下一个窗口的位置
		contentWindow->setPosition(ccp(layer->getContentSize().width * 0.5,
									   lastWindowBorderPos + border_shift));
		
        /* tips.png图片拉长的倍数 */
        float pngScale = cwSize.height / 522 ;
//        CCLOG("*******************>>>");
//        CCLOG("i = %d" , i);
//        CCLOG("title = %s" , title->getCString());
//        CCLOG("content = %s" , content->getCString());
//        CCLOG("pos: x=%f , y = %f" , contentWindow->getPositionX() , contentWindow->getPositionY());
//        CCLOG("cwSize: width=%f, height=%f", cwSize.width, cwSize.height);
//        CCLOG("lastWindowBorderPos = %f" , lastWindowBorderPos);
//        CCLOG("<<<********************");
        lastWindowBorderPos = contentWindow->getPositionY() + cwSize.height;

		//公告title
        CCLabelTTF *label = CCLabelTTF::create(title->getCString(), FONT_PANGWA, 35);
        label->setColor(COLOR_RED);
        label->setAnchorPoint(ccp(0.5 , 1));
		label->setPosition(ccp(cwSize.width *0.5f, cwSize.height - pngScale * 20));
		contentWindow->addChild(label, 100);
		//公告内容
//        CCSize maybeTextSize = stringInLabelRect(content->getCString());
//		CCLabelTTF *label_content=CCLabelTTF::create(content->getCString(), FONT_PENGYOU, 24,CCSizeMake(maybeTextSize.width + text_wid_expand * 4,maybeTextSize.height + text_hei_expand),kCCTextAlignmentLeft);
        CCLabelTTF *label_content=CCLabelTTF::create(content->getCString(), FONT_PENGYOU, 24,CCSizeMake(cwSize.width - 40 * widthFactor, cwSize.height - label->getContentSize().height),kCCTextAlignmentLeft);
		label_content->setAnchorPoint(ccp(0.5, 1));//内容顶线对齐
        label_content->setColor(ccBLACK);
		label_content->setPosition(ccp(label->getPositionX(), label->getPositionY() - label->getContentSize().height));
		label_content->setZOrder(102);
        contentWindow->addChild(label_content);
		
		//点击前往按钮
        if (viewId > 0) {
            SGButton *goButton = SGButton::create("box_btnbg.png", NULL, this, menu_selector(SGAnnouncementLayer::AnnounceButton),CCPointZero,false,true);
            
            CCString *go = CCString::createWithFormat(str_Hit_goto);
            SGCCLabelTTF *go_label = SGCCLabelTTF::create(go->getCString(), FONT_PANGWA, 30);
            
            goButton->addChild(go_label);
            goButton->setTag(viewId);//跳转到某个视图界面
            go_label->setPosition(ccp(goButton->getContentSize().width / 2, goButton->getContentSize().height / 2));
            
            //立即前往Button
            CCMenu *menu = CCMenu::create(goButton, NULL);
            menu->setTouchPriority(0);
            menu->setPosition(CCPointZero);
            goButton->setScale(0.7);
            goButton->setZOrder(200);
            //goButton->setPosition(ccp(label->getPositionX(), goButton->getContentSize().height / 1.5));
            goButton->setAnchorPoint(ccp(0.5, 0));
            goButton->setPosition(ccp(label->getPositionX(), pngScale * 18));
            menu->setZOrder(200);
            contentWindow->addChild(menu);
        }

		layer->addChild(contentWindow);
        //每次加对应的contentWindow的大小，为之后计算scrollview的内容体偏移
        positionY += (cwSize.height + border_shift);

    }
	//layer->setContentSize(CCSizeMake(504, m_scrollview->getContentSize().height));
	//控制是否可以滑动
	iscanmove = (positionY)>791?true:false;//@note：591是m_scrollview的可视区域高度
	

	m_scrollview->setContentOffset(ccp(0, -positionY + m_scrollview->getContentSize().height - 30));//20是顶部窗口的起始位置偏移，不减去会有一小部分遮挡
    m_scrollview->setContentSize(CCSizeMake(m_scrollview->getContentSize().width, positionY));//scrollview的真正大小
    m_scrollview->getContainer()->setContentSize(CCSizeMake(m_scrollview->getContentSize().width, positionY + 10));//滚动重置时的container位置
}


void SGAnnouncementLayer::restartFlower( cocos2d::CCNode *s )
{
	CCSprite *sprite =(CCSprite*)s;
	int tag = sprite->getTag();
	int zoder = sprite->getZOrder();
	sprite->removeFromParentAndCleanup(true);
	//CCLayer *layer = (CCLayer*)m_scrollview->getContainer();
	
	srand((unsigned)time(NULL));
	
	int numb = arc4random()%3+1;
	char name[10];
	sprintf(name,"yh%d.png",numb);
	CCSprite *newFlower = CCSprite::createWithSpriteFrameName(name);
	newFlower->setRotation(-(arc4random() % 50) - 10);
	newFlower->setAnchorPoint(ccp(0.0,1));
	
	int xPos = -(arc4random() % 550) + 250;
//	CCLOG("xPos =======> %d", xPos);
	
	newFlower->setPosition(ccp(xPos, arc4random()% 300 + 590));
	this->addChild(newFlower,zoder + 100,tag);
	this->playFlowerAnim(newFlower);
}

//花瓣飘落动作
void SGAnnouncementLayer::playFlowerAnim(CCSprite *spriteFlower)
{
	int iTag = spriteFlower->getTag();
	srand((unsigned)time(NULL));
	float times, roTime;
	float fAngle1, fAngle2;
	if (iTag == TAG_Flower1)
	{
		times = arc4random() % 7 + 3;//花瓣下落的时间
		roTime = 0.8;//花瓣摆动一次时间
		fAngle1 = -80;//花瓣摆动角度
		fAngle2 = 80;//摆动角度
	}
	else
	{
		times = arc4random() % 7 + 3;
		roTime = 1.8;
		fAngle1 = -100;
		fAngle2 = 100;
	}
//	CCLog("Down Time =======> %f", times);
	//随机生成花瓣横向偏移值
	//srand((unsigned)time(NULL));
	int randPos = arc4random() % ((int)CCDirector::sharedDirector()->getWinSize().width) + 400;
//	CCLog("Shift_flower======>%d", randPos);
	//运动到的位置
	int movPos = CCDirector::sharedDirector()->getWinSize().width - randPos;
	int hori_shift = (arc4random() % 2) * 150;// 再次横向偏移, 或者有偏移,或者无随机
//	CCLOG("movPos ============>%d hori_shift ===========> %d", movPos, hori_shift);
	
	//	CCMoveTo *moveTo = CCMoveTo::create(times, ccp(CCDirector::sharedDirector()->getWinSize().width - iRandPos, 30));
	CCMoveTo *moveTo = CCMoveTo::create(times, ccp(movPos + hori_shift, -550));
	CCCallFuncN *actDone = CCCallFuncN::create(this, callfuncN_selector(SGAnnouncementLayer::restartFlower));
	CCFiniteTimeAction *putdown = CCSequence::create(moveTo, actDone, NULL);
	//花瓣旋转动作
	CCRotateBy *rotaBy1 = CCRotateBy::create(roTime, fAngle1);
	CCRotateBy *rotaBy2 = CCRotateBy::create(roTime, fAngle2);
	
	//花瓣翻转动作
	spriteFlower->setVertexZ(60);//防止出现遮挡
	//from internet
	CCOrbitCamera * orbit = CCOrbitCamera::create(8, 1, 0, 0, 360, 45, 0);
	//让花执行三维翻转的动作
	CCRepeat *fz3d = CCRepeat::create(orbit, -1);//不停进行翻转花瓣翻转的动作
	
	//类似淡入淡出
	CCEaseInOut *ease1 = CCEaseInOut::create(rotaBy1, 3);
	CCEaseInOut *ease2 = CCEaseInOut::create(rotaBy2, 3);
	//摆动动作合成
	CCFiniteTimeAction *seq2 = CCSequence::create(ease1, ease2, NULL);
	CCRepeat *blink = CCRepeat::create(seq2, -1);//合成对应摆动作
	
	//同时执行所有动作
	spriteFlower->runAction(CCSpawn::create(putdown, blink, fz3d, NULL));
	
}


void SGAnnouncementLayer::AnnounceButton(cocos2d::CCObject *obj)
{
    CCLayer *layer = (CCLayer*)m_scrollview->getContainer();

    SGButton *item = (SGButton*)obj;
    CCLOG("ItemTag ============================== %d", item->getTag());
	
    switch (item->getTag()) {
        case 1://领奖
        {
            EFFECT_PLAY(MUSIC_BTN);
            SGMainManager::shareMain()->requestFirstActivityLayer();
        }
            break;
        case 2://限时副本
        {
            EFFECT_PLAY(MUSIC_BTN);
            SGMainManager::shareMain()->requestFirstActivityLayer();
        }
            break;
        case 3://进商店
        case 4:
        {
            EFFECT_PLAY(MUSIC_BTN);
            SGMainManager::shareMain()->requestFirstActivityLayer();
        }
            break;
        case 5:
        {
            SGMainManager::shareMain()->requestFirstActivityLayer();
        }
            break;
        case 6:
        {
            SGMainManager::shareMain()->requestFirstActivityLayer();
        }
            break;
        case 7:
        {
            SGMainManager::shareMain()->requestFirstActivityLayer();
        }
            break;
        case 8:
        {
            
        }
            break;
        case 9:
        {
            
        }
            break;
            
        default:
            break;
    }
    
    this->boxClose();
    
}




