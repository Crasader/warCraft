#include "SGLootWiKiCell.h"
#include "SGDrawCards.h"
#include "SGLootWiKiLayer.h"
#include "SGLootWiKiDataManager.h"
#include "SGShowString.h"
#include "SGTouchLayer.h"

SGLootWiKiCell::SGLootWiKiCell():
m_source(NULL),
m_lootWiKiLayer(NULL)
, m_rate(NULL)
, m_sourceDes1(NULL)
, m_sourceDes2(NULL)
, m_sourceTitle(NULL)
, m_notOpenLab(NULL)
, m_spriteOpen(NULL)
, _isOpened(false)
, _redLabel(NULL)
{
    
}

SGLootWiKiCell::~SGLootWiKiCell()
{
    
}

SGLootWiKiCell * SGLootWiKiCell::create(HaveSource * source ,SGLootWiKiLayer * wikilayer, bool isOpened )
{
    SGLootWiKiCell * ret = new SGLootWiKiCell();
    
    if (ret && ret->init() )
    {
        ret->_isOpened = isOpened;
        ret->initView(source ,wikilayer );
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return  ret ;
}

void SGLootWiKiCell::initView(HaveSource * source , SGLootWiKiLayer * wikilayer )
{
    m_source = source;
    m_lootWiKiLayer = wikilayer;
    
    m_spriteOpen = createSprite(source->getSourceType() );
    
    //是否开启的提示文本。
    _redLabel = SGCCLabelTTF::create(str_lootWiKi_12, FONT_BOXINFO, 24, ccRED);
    m_spriteOpen->addChild(_redLabel);
    _redLabel->setAnchorPoint(ccp(1, 0));
    _redLabel->setPosition(ccp(m_spriteOpen->getContentSize().width - 10, 25));
    
    CCSprite * bg = CCSprite::createWithSpriteFrameName("source_cellBg.png");
    bg->setPosition(ccp(0 , 0));
    this->addChild(bg);
    
    m_spriteOpen->setPosition(ccpAdd(bg->getPosition(), ccp(-2, 60) ) );
    this->addChild(m_spriteOpen);
    
    //1行描述
    m_sourceDes1 = SGCCLabelTTF::create(source->getDes1().c_str(), FONT_BOXINFO, 24);
    m_sourceDes1->setAnchorPoint(ccp(0 ,1));
    m_sourceDes1->setPosition(ccp(70 , bg->getContentSize().height - 25 ));
    bg->addChild(m_sourceDes1);
    
    //di2行描述
    if(source->getDes2().compare("-1") == 0)
        m_sourceDes2 = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
    else
        m_sourceDes2 = SGCCLabelTTF::create(source->getDes2().c_str(), FONT_BOXINFO, 24);
    m_sourceDes2->setAnchorPoint(ccp(0 ,1));
    m_sourceDes2->setPosition(ccp(70 , m_sourceDes1->getPosition().y - m_sourceDes1->getContentSize().height - 10 ));
    bg->addChild(m_sourceDes2);
    
    m_notOpenLab = SGCCLabelTTF::create(str_lootWiKi_3, FONT_BOXINFO, 28);
    m_notOpenLab->setPosition(bg->getContentSize()/2);
    bg->addChild(m_notOpenLab,5);
    
    //如果是精英副本 许显示  参加 、 参加上限  从playerinfo中读取。
    if(source->getSourceType() == 2)
    {
        int count = SGPlayerInfo::sharePlayerInfo()->getCreamCanJoinCount(source->getSourcePara());
        int total = SGStaticDataManager::shareStatic()->getCreamByPlotId(source->getSourcePara())->getCreamDayTimes();
        if (_isOpened)
        {
            m_rate = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",count , total)->getCString(), FONT_BOXINFO, 24);
        }
        else
        {
            m_rate = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
        }
    }
    else
    {
        m_rate = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
    }
    m_rate->setAnchorPoint(ccp(1,1));
    m_rate->setPosition(ccp(bg->getContentSize().width - 15, m_sourceDes2->getPosition().y));
    bg->addChild(m_rate);
    
    showHideEle(source);
    
    CCSpriterX *m_spTouch = CCSpriterX::create("animationFile/jiantou.scml", true , true);
    m_spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
    m_spTouch->setPosition(ccp( 100 , 0) );
    m_spTouch->setisloop(true);
    m_spTouch->play();
    m_spTouch->setVisible(false);
    addChild(m_spTouch,1000 , 1000);
}

CCSprite * SGLootWiKiCell::createSprite(int lootType)
{
    CCSprite * ret = NULL;
    /*0= 未开启 1=普通闯关 2=精英闯关 3=天梯 4=守荆州 5=讨魔 6副本*/
    switch(lootType)
    {
        case 0:
            ret = CCSprite::createWithSpriteFrameName("source_ChuangGuan.png");
            m_sourceTitle = SGCCLabelTTF::create(CCString::createWithFormat("")->getCString(), FONT_BOXINFO, 32);
            m_sourceTitle->setPosition(ccp(85, ret->getContentSize().height*0.5));
            ret->addChild(m_sourceTitle);
            break;
        case 1:
        case 2:
            ret = CCSprite::createWithSpriteFrameName("source_ChuangGuan.png");
            m_sourceTitle = SGCCLabelTTF::create(str_lootWiKi_6, FONT_BOXINFO, 28 , COLOR_YELLOW);
            m_sourceTitle->setAnchorPoint(ccp(0,0.5));
            m_sourceTitle->setPosition(ccp(85, ret->getContentSize().height*0.5));
            ret->addChild(m_sourceTitle);
            
            break;
        case 3:
            ret = CCSprite::createWithSpriteFrameName("source_TianTi.png");
            m_sourceTitle = SGCCLabelTTF::create(str_lootWiKi_7, FONT_BOXINFO, 28 , COLOR_YELLOW);
            m_sourceTitle->setAnchorPoint(ccp(0,0.5));
            m_sourceTitle->setPosition(ccp(85, ret->getContentSize().height*0.5));
            ret->addChild(m_sourceTitle);

            break;
        case 4:
            ret = CCSprite::createWithSpriteFrameName("source_ShouJingZhou.png");
            m_sourceTitle = SGCCLabelTTF::create(str_lootWiKi_4, FONT_BOXINFO, 28 , COLOR_YELLOW);
            m_sourceTitle->setAnchorPoint(ccp(0,0.5));
            m_sourceTitle->setPosition(ccp(85, ret->getContentSize().height*0.5));
            ret->addChild(m_sourceTitle);
            
            break;
        case 5:
            ret = CCSprite::createWithSpriteFrameName("sourceTaomo.png");
            m_sourceTitle = SGCCLabelTTF::create(str_lootWiKi_8, FONT_BOXINFO, 28 , COLOR_YELLOW);
            m_sourceTitle->setAnchorPoint(ccp(0,0.5));
            m_sourceTitle->setPosition(ccp(85, ret->getContentSize().height*0.5));
            ret->addChild(m_sourceTitle);
            
            break;
        case 6:
            ret = CCSprite::createWithSpriteFrameName("sourceFuBen.png");
            m_sourceTitle = SGCCLabelTTF::create(str_lootWiKi_5, FONT_BOXINFO, 28 , COLOR_YELLOW);
            m_sourceTitle->setAnchorPoint(ccp(0,0.5));
            m_sourceTitle->setPosition(ccp(85, ret->getContentSize().height*0.5));
            ret->addChild(m_sourceTitle);
            
            break;
    }
    return ret;
}

void SGLootWiKiCell::onClicked()
{
    m_lootWiKiLayer->itemSelect();
}


void SGLootWiKiCell::updateItem(HaveSource *source, bool isOpened)
{
    _isOpened = isOpened;
    showHideEle(source);
    //参加精英闯关的次数 从playerinfo中读取
    int count = 1;
    int total = 1;
    /*0= 未开启 1=普通闯关 2=精英闯关 3=天梯 4=守荆州 5=讨魔 6副本*/
    switch(source->getSourceType())
    {
        case 0:
            break;
        case 1:
            m_spriteOpen->setDisplayFrame(CCSprite::createWithSpriteFrameName("source_ChuangGuan.png")->displayFrame());
            m_sourceTitle->setString(str_lootWiKi_6);
            setDesc(source->getDes1() , source->getDes2());
            m_rate->setString("");
            break;
        case 2:
            m_spriteOpen->setDisplayFrame(CCSprite::createWithSpriteFrameName("source_ChuangGuan.png")->displayFrame());
            
            //如果是精英副本 许显示  参加 、 参加上限 从playerinfo中读取。
            count = SGPlayerInfo::sharePlayerInfo()->getCreamCanJoinCount(source->getSourcePara());
            total = SGStaticDataManager::shareStatic()->getCreamByPlotId(source->getSourcePara())->getCreamDayTimes();
            if (_isOpened)
            {
                //显示为 剩余次数/总的可参加次数。
                m_rate->setString(CCString::createWithFormat("%d/%d",/*total - */count , total)->getCString() );
            }
            else
            {
                m_rate->setString("");
            }
            
            m_sourceTitle->setString(str_lootWiKi_6);
//            m_rate->setString("1/2");
            setDesc(source->getDes1() , source->getDes2());
            break;
        case 3:
            m_spriteOpen->setDisplayFrame(CCSprite::createWithSpriteFrameName("source_TianTi.png")->displayFrame());
            m_sourceTitle->setString(str_lootWiKi_7);
            m_rate->setString("");
            setDesc(source->getDes1() , source->getDes2());
            break;
        case 4:
            m_spriteOpen->setDisplayFrame(CCSprite::createWithSpriteFrameName("source_ShouJingZhou.png")->displayFrame());
            m_sourceTitle->setString(str_lootWiKi_4);
            m_rate->setString("");
            setDesc(source->getDes1() , source->getDes2());
            break;
        case 5:
            m_spriteOpen->setDisplayFrame(CCSprite::createWithSpriteFrameName("sourceTaomo.png")->displayFrame());
            m_sourceTitle->setString(str_lootWiKi_8);
            m_rate->setString("");
            setDesc(source->getDes1() , source->getDes2());
            break;
        case 6:
            m_spriteOpen->setDisplayFrame(CCSprite::createWithSpriteFrameName("sourceFuBen.png")->displayFrame());
            m_sourceTitle->setString(str_lootWiKi_5);
            m_rate->setString("");
            setDesc(source->getDes1() , source->getDes2());
            break;
    }
}

//false = 开 ， true=没开。
void SGLootWiKiCell::showHideEle(HaveSource * hs)
{
    //判断元素显隐
    bool isSourceOpen = hs->getSourceType() == 0;
    
    if(isSourceOpen)
    {
        m_spriteOpen->setVisible(false);
        m_sourceDes1->setVisible(false);
        m_sourceDes2->setVisible(false);
        
        m_notOpenLab->setVisible(true);
    }
    else
    {
        m_spriteOpen->setVisible(true);
        m_sourceDes1->setVisible(true);
        m_sourceDes2->setVisible(true);
        
        m_notOpenLab->setVisible(false);
    }
    //判断“(未开启)"的红色提示文本。
    _redLabel->setVisible(!_isOpened);
}

void SGLootWiKiCell::setDesc(std::string desc1 , std::string desc2)
{
    m_sourceDes1->setString(desc1.c_str());
    //约定  当第二个描述文本空 时 填入 0
    if(desc2.compare("-1") == 0)
        m_sourceDes2->setString("");
    else
        m_sourceDes2->setString(desc2.c_str());
}

void SGLootWiKiCell::showHandAnimate(bool isVisible)
{
    getChildByTag(1000)->setVisible(isVisible);
}

