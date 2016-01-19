#include "SGLootWiKiMainLayer.h"
#include "SGLootWiKiDataManager.h"
#include "SGShowString.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "LootPiece.h"
#include "SGLootWiKiLayer.h"
#include "cocos-ext.h"
#include "SGButton.h"
#include "SGDrawCards.h"
#include "SGGuideManager.h"
#include <algorithm>

using namespace std;
USING_NS_CC_EXT;


SGLootWiKiMainLayer::SGLootWiKiMainLayer()
: m_pieceLootInfo(NULL)
, m_pieceId(-1)
, m_isMaterial(-1)
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }

}


SGLootWiKiMainLayer::~SGLootWiKiMainLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_lootWikiMainLayer);
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }

}


SGLootWiKiMainLayer * SGLootWiKiMainLayer::create(int pieceId , int isMaterial)
{
    SGLootWiKiMainLayer * ret = new SGLootWiKiMainLayer();
    
    if (ret && ret->init(NULL,box_lootWikiMain,CCSize(460,660),false,false ) )
    {
        ret->m_isMaterial = isMaterial;
        ret->initView(pieceId );
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

void SGLootWiKiMainLayer::initView(int pieceId)
{
    
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT_MAX, true);
    
    //保存创建对象时传入的参数
    m_pieceId = pieceId;
    
    SGLootWiKiDataManager * lootData = SGLootWiKiDataManager::shareSGLootWiKiDataManager();
    m_pieceLootInfo = lootData->getLootPieceArray(m_pieceId);
    sortData();
    
    //绑定资源
    ResourceManager::sharedInstance()->bindTexture("sglootwikilayer/sglootwikilayer.plist", RES_TYPE_LAYER_UI, sg_lootWikiMainLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_lootWikiMainLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_lootWikiMainLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_lootWikiMainLayer);
    //-------------开始添加图片资源
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint centerPoint = ccp(0.5 *winSize.width , 0.5 * winSize.height);
    
    //灰色的覆盖色
    CCLayerColor * layerColor = CCLayerColor::create(ccc4(0,0,0 , 180));
    this->addChild(layerColor , -10);
    
    //最外边的板
    CCScale9Sprite * outBg = CCScale9Sprite::createWithSpriteFrameName("Tips.png");
    outBg->setPreferredSize(CCSize(508,900 * winSize.height/1136.0f));
    outBg->setPosition(centerPoint);
    this->addChild(outBg , -10);
    
    //标题背景
    CCSprite * titleBg = CCSprite::createWithSpriteFrameName("source_titleBg.png");
    titleBg->setPosition( ccp(0.5 * winSize.width , -12 +  outBg->getPositionY() + 0.5 * outBg->getContentSize().height -0.5*titleBg->getContentSize().height  ) );
    this->addChild(titleBg);
    //标题背景下边的杆
    CCSprite * stick = CCSprite::createWithSpriteFrameName("source_stick.png");
    stick->setPosition(ccpAdd(titleBg->getPosition(), ccp(0, -titleBg->getContentSize().height*0.5 + 1 )));
    this->addChild(stick);
    //标题
    SGCCLabelTTF * title = SGCCLabelTTF::create(str_lookWiKi_1, FONT_PANGWA, 32);
    title->setPosition(titleBg->getPosition());
    this->addChild(title);
    
    //内容背景
    CCScale9Sprite * contentBg = CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
//    contentBg->setColor(ccc3(150, 150, 150));
    contentBg->setPreferredSize(CCSize( 432,668 * winSize.height/1136.0f ));
    contentBg->setPosition(centerPoint);
    this->addChild(contentBg);
    
    CCScale9Sprite * iconBg = CCScale9Sprite::createWithSpriteFrameName("source_upBg.png");
    iconBg->setPreferredSize(CCSize(430 , 170));
    iconBg->setAnchorPoint(ccp(0.5 , 1));
    iconBg->setPosition(ccp(winSize.width * 0.5, title->getPosition().y - title->getContentSize().height));
    this->addChild(iconBg);
    
    //碎片
    CCSprite * icon = NULL;
    //名字
    SGCCLabelTTF * name = NULL;
    //如果是材料 需要加上边框
    CCSprite * materialKuang = NULL;
    if(m_isMaterial == -1)
    {
//        icon = SGDrawCards::drawallcards(m_pieceId , 10, sg_lootWikiMainLayer);
        icon = SGDrawCards::createNormalSprite(m_pieceId , 10, sg_lootWikiMainLayer);
        name= SGCCLabelTTF::create(SGStaticDataManager::shareStatic()->getPiecesDictById(m_pieceId)->getPiecesName()->getCString(), FONT_XINGKAI, 36);
    }
    //材料类型
    else
    {
        SGMaterialDataModel * model = SGStaticDataManager::shareStatic()->getMaterialById(m_pieceId);
        icon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png" , model->getMaterialIconId() )->getCString());
        name= SGCCLabelTTF::create(model->getMaterialName()->getCString(), FONT_XINGKAI, 36);
        //表数据时自然数索引  但是资源从0开始索引
        materialKuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png" , model->getMaterialBorderColor() - 1)->getCString());
    }
    icon->setAnchorPoint(ccp(0 , 0.5) );
    icon->setPosition(ccp(50 , iconBg->getContentSize().height*0.5));
    iconBg->addChild(icon);
    
    
    name->setAnchorPoint(icon->getAnchorPoint());
    name->setPosition(ccpAdd(ccp(icon->getContentSize().width + icon->getPositionX() + 10, icon->getPositionY()), ccp(20 , 0) ));
    iconBg->addChild(name);
    
    if(materialKuang)
    {
        materialKuang->setAnchorPoint(icon->getAnchorPoint());
        materialKuang->setPosition( ccpAdd(icon->getPosition() , ccp(-28 , 0) ));
        iconBg->addChild(materialKuang , 2);
    }
    
    //关闭按钮
    SGButton * closeBtn = SGButton::createFromLocal("an.png",str_lookWiKi_2, this, menu_selector(SGLootWiKiMainLayer::onCloseBtnClicked) ,CCPointZero ,FONT_PANGWA , ccWHITE ,36 );
    closeBtn->setPosition( ccp(0.5 * winSize.width, outBg->getPositionY() - 0.5 * outBg->getContentSize().height + 60) );
    addBtn(closeBtn);

    CCSize size = CCSize(contentBg->getContentSize().width , contentBg->getContentSize().height - iconBg->getContentSize().height - 10);
    SGLootWiKiLayer * lootWiKiLayer = SGLootWiKiLayer::create(m_pieceLootInfo , this , size);
    lootWiKiLayer->setPosition(ccp(winSize.width*0.5 - contentBg->getContentSize().width*0.5 , contentBg->getPositionY() - contentBg->getContentSize().height*0.5 + 10));
    this->addChild(lootWiKiLayer);
    //引导中用，不可更改
    lootWiKiLayer->setTag(666);
}


void SGLootWiKiMainLayer::onCloseBtnClicked()
{
    SGGuideManager::shareManager()->setMainTouchEnable(true);

    this->boxClose();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }
}

bool compare(CCObject * a , CCObject * b)
{
    HaveSource * c = (HaveSource*)a;
    HaveSource * d = (HaveSource*)b;
    if(c && d)
        return c->getPriority() < d->getPriority();
    else
        return true;
}

void SGLootWiKiMainLayer::sortData()
{
    sort(m_pieceLootInfo->data->arr, m_pieceLootInfo->data->arr + m_pieceLootInfo->count() ,compare );
}