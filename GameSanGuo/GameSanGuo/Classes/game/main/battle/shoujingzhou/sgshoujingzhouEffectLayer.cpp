//
//  sgshoujingzhouEffectLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 13-12-19.
//
//

#include "sgshoujingzhouEffectLayer.h"
#include "SGMainScene.h"
#include "SGMainManager.h"
#include "SGSoulExchangeLayer.h"
#include "SGStringConfig.h"

SGshoujingzhouEffectLayer::SGshoujingzhouEffectLayer()

{
    
}

SGshoujingzhouEffectLayer::~SGshoujingzhouEffectLayer()
{

    
}


SGshoujingzhouEffectLayer *SGshoujingzhouEffectLayer::create(std::vector<SeRewardTypeId*>vector)
{
    SGshoujingzhouEffectLayer *layer = new SGshoujingzhouEffectLayer();
    
 CCSize s=CCDirector::sharedDirector()->getWinSize();
    
    if (layer && layer->init(NULL, box_null,CCSizeMake(s.width*0.8, s.height*0.4)))
    {
        layer->vec=vector;
       // layer->initDatas(vector);
        layer->initView();
       
        layer->autorelease();
        return layer;
    }
    CC_SAFE_RELEASE(layer);
    return NULL;
}


void SGshoujingzhouEffectLayer::initView()
{
    initDatas(vec);
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    /////
//    CCLayerColor *black = CCLayerColor::create(ccc4(255, 255, 255,255), 920, 1200);
//    this->addChild(black,11);
//    black->setPosition(ccp(0, 0));
//    CCFadeIn*fadein=CCFadeIn::create(2.0f);
//    CCFadeOut*fadeout=CCFadeOut::create(2.0f);
//    CCDelayTime*delay=CCDelayTime::create(0.5f);
//    CCAction*action10= CCSequence::create(delay,delay,fadein,NULL);
//    black->runAction(action10);
//    /////
   CCMenu*  menu1=CCMenu::create();
    this->addChild(menu1,9);
    menu1->setPosition(CCPointZero);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_shoujingzhou);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_shoujingzhou);

    cocos2d::extension::CCScale9Sprite *frame2 = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("Tips.png");
    frame2->setPreferredSize(CCSizeMake(s.width*0.8, s.height*0.4));
    this->addChild(frame2);
    //this->addChild(frame);
    frame2->setPosition(center);
    
    CCScale9Sprite* fontbg = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    fontbg->setPreferredSize(CCSizeMake(s.width*0.65, s.height*0.22));
    fontbg->setPosition(center);
    this->addChild(fontbg);
    SGButton*ok= SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGshoujingzhouEffectLayer::boxClose), CCPointZero, false, true);
    ok->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -fontbg->getContentSize().height*0.65)));
    menu1->addChild(ok);
    
    SGCCLabelTTF*label=SGCCLabelTTF::create(str_SoulExchangeLayer_str9, FONT_BOXINFO, 40 , ccWHITE , ccc3(143,87,0));
    //modify by:zyc . merge into create.
    //label->setOutSideColor(ccc3(143,87,0));
    label->setPosition(ccpAdd(center, ccp(0, fontbg->getContentSize().height*0.7)));
    this->addChild(label);

    ///////滑动
    //tableView = SNSTableView::create(CCRectMake(450,600, fontbg->getContentSize().width, fontbg->getContentSize().height), TableViewTypeVertical);
    tableView->setFrame(CCRectMake(450,600, fontbg->getContentSize().width, fontbg->getContentSize().height));
    tableView->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width*0.5, -fontbg->getContentSize().height*0.5)));
    ///设置tableview每行有几个item
  //  tableViewColumns = 3;
    tableView->setDown(-10);
    tableViewHeight=150;
    
   // this->addChild(tableView);
 
//    scrollview = CCScrollView::create(fontbg->getContentSize());
//    CCLayer *container = CCLayer::create();
//	
//    container->setContentSize(fontbg->getContentSize()*0.9);
//	
//    scrollview->setContainer(container);
//    scrollview->setDirection(kCCScrollViewDirectionVertical);
//    scrollview->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width*0.5, -fontbg->getContentSize().height*0.5)));
//    this->addChild(scrollview);
    //tableViewHeight=300;
   // tableView->setDown(0);
   

}

 unsigned int SGshoujingzhouEffectLayer::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
    return 3;
}

SNSTableViewCellItem *SGshoujingzhouEffectLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{  ////////

    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGshoujingzhouEffectLayerCell  *item = (SGshoujingzhouEffectLayerCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (NULL == item)
    {
        item = SGshoujingzhouEffectLayerCell::create(datas,num);

    }
    
    
    if (num<datas->count()) {
        SeRewardTypeId *model =  (SeRewardTypeId *)datas->objectAtIndex(num);
        item->setData(model);
    }else {
        item->setVisible(false);
    }
    
    return item;
    
    
//    int num = tableView->getItemPointerWithIndexPath(indexPath);
//    SGshoujingzhouEffectLayerCell  *item = (SGshoujingzhouEffectLayerCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
//    if (NULL == item)
//    {
//        item = SGshoujingzhouEffectLayerCell::create(datas,num);
//        
//    }
//   
//    return item;
}
unsigned int  SGshoujingzhouEffectLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
   
  //  return  vec.size();
    return datas->count();
}
void SGshoujingzhouEffectLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    //    EFFECT_PLAY(MUSIC_BTN);
    //
    //    int index = tableView->getItemPointerWithIndexPath(indexPath);
    //    SGHelpTitleDataModel *tit = (SGHelpTitleDataModel*)datas->objectAtIndex(index);
    //
    //    SGHelpBookInfo *info = SGHelpBookInfo::create(tit);
    //    SGMainManager::shareMain()->showLayer(info);
}



void SGshoujingzhouEffectLayer::boxClose(CCObject*obj)
{
   SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
   SGBaseLayer *layer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
     if (layer != NULL)
    {
    layer->setIsCanTouch(true);
    }
    SGSoulExchangeLayer *seLayer = (SGSoulExchangeLayer *)mainScene->getChildByTag(sg_soulExchangeLayer);
    if (seLayer)
        seLayer->setCanOpenBox(true);
    
   this->removeFromParentAndCleanup(true);
}




void SGshoujingzhouEffectLayer::initDatas(std::vector<SeRewardTypeId*>vector)
{
    this->datas = CCArray::create();
    this->datas->retain();
    
//    if (array != NULL) {
//        for (int ii = 0; ii < array->count(); ii++) {
//            CCObject *obj = array->objectAtIndex(ii);
//            this->datas->addObject(obj);
//        }
//    }
//
    std::vector<SeRewardTypeId*>::iterator iter = vec.begin();
        for (;iter != vec.end();iter++)
       {
           
         //  datas->addObject(*iter);
           SeRewardTypeId *data = new SeRewardTypeId();
           data->setidid((*iter)->idid);
           data->settypetype((*iter)->typetype);
           this->datas->addObject(data);
           data->autorelease();
           
       }
    
    

    
    
    
    
    
    
    
    
    
    
    
    
	
}
