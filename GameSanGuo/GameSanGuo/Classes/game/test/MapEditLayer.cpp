//
//  MapEditLayer.cpp
//  TestSG
//
//  Created by gengshengcai on 12-11-28.
//
//

#include "MapEditLayer.h"

#include "TestData.h"
#include "SGStringConfig.h"

MapEditLayer* MapEditLayer::create()
{
    MapEditLayer *mel = new MapEditLayer();
    if (mel && mel->initWithColor(ccc4(200, 120, 120, 230)))
    {
        mel->init();
        mel->autorelease();
        return mel;
    }
    CC_SAFE_DELETE(mel);
    return NULL;
}
bool MapEditLayer::init()
{
    
    this->setTouchEnabled(true);
    
    CCLabelTTF* label = CCLabelTTF::create(str_back, "", 20);
    CCMenuItemLabel* labelItem = CCMenuItemLabel::create(label, this, menu_selector(MapEditLayer::back));
    labelItem->setPosition(ccp(135,-200));
    TMenuItem *item1 = TMenuItem::create(1, 1, 1,this, menu_selector(MapEditLayer::menuSelect));
    item1->setPosition(ccp(-100, 180));
    
    TMenuItem *item2 = TMenuItem::create(2, 2, 1,this, menu_selector(MapEditLayer::menuSelect));
    item2->setPosition(ccp(-50, 180));
    
    TMenuItem *item3 = TMenuItem::create(3, 3, 1,this, menu_selector(MapEditLayer::menuSelect));
    item3->setPosition(ccp(0, 180));
    
    TMenuItem *item4 = TMenuItem::create(4, 2, 2,this, menu_selector(MapEditLayer::menuSelect));
    item4->setPosition(ccp(50, 180));
    
    TMenuItem *item5 = TMenuItem::create(5, 1, 3,this, menu_selector(MapEditLayer::menuSelect));
    item5->setPosition(ccp(100, 180));
    
    CCMenu *menu = CCMenu::create(item1, item2, item3, item4, item5,labelItem, NULL);
    this->addChild(menu);
    
    return true;
}
void MapEditLayer::menuSelect(CCNode *sender)
{
    if (selectItem)
    {
        selectItem ->stopAllActions();
        selectItem->setOpacity(255);
        selectItem = NULL;
    }
    selectItem = (TMenuItem *)sender;
    
    CCFadeOut *fadeOut = CCFadeOut::create(0.2);
    CCFadeIn *fadeIn = CCFadeIn::create(0.2);
    selectItem->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(fadeOut,fadeIn,NULL)));

}
void MapEditLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch* touch = (CCTouch*)( pTouches->anyObject() );
	CCPoint location = touch->getLocation();
    
    if (location.x >30 && location.x<(30 + 8*32) &&
        location.y>100 && location.y <(100 + 6*32))
    {
        
        int indexj = (location.x - 30)/32;
        int  indexi = (100 + 6* 32 - location.y)/32;
        CCLOG("indexi:%d",indexi);
        CCLOG("indexj:%d",indexj);
        if (grid[indexi][indexj])
        {
//            GridData *gd = grid[indexi][indexj];
//          
//            this->removeChild(gd->sp, true);
//            CCArray* del = TestData::shareData()->getIndexs(indexi,indexj);
//            
//            for (int i = 0; i<del->count(); i++)
//            {
//                Index *index = (Index *)del->objectAtIndex(i);
//                CCLOG("删除index:%d   %d",index->i,index->j);
//                grid[index->i][index->j] = NULL;
//            }
//            TestData::shareData()->removeData(indexi,indexj);
        }
        else
        if (selectItem)
        {
            putOnScene(indexi, indexj);
        }
    }
}
void MapEditLayer::putOnScene(int i, int j)
{
    CCPoint pst = ccp(30 + j*32 + 16,100 + 6* 32 - i * 32 - 16);
    GridData *gd = NULL;
    switch (selectItem->getType()) {
        case 1:
        {
            gd = new GridData();
        }
            break;
        case 2:
        {
            if (i>4) {
                return;
            }
            pst = ccpAdd(pst, ccp(0,-16));
            gd = new GridData();
            grid[i+1][j] = gd;
            
        }
            break;
        case 3:
        {
            if (i>4 || j>6)
            {
                return;
            }
            pst = ccpAdd(pst, ccp(16,-16));
            
            gd = new GridData();
            grid[i][j+1] = gd;
            grid[i+1][j] = gd;
            grid[i+1][j+1] = gd;
        }
            break;
        default:
            gd = new GridData();
            break;
    }

    char spriteName[256] = {0};
    sprintf(spriteName, "ani_%d.png",selectItem->getSid());
    CCSprite *sp = CCSprite::createWithSpriteFrameName(spriteName);
    this->addChild(sp);
    sp->setPosition(pst);
    
    gd->sid = selectItem->getSid();
    gd->sp = sp;
    grid[i][j] = gd;
    
    switch (selectItem->getColorId())
    {
        case 1:
            sp->setColor(ccRED);
            break;
        case 2:
            sp->setColor(ccBLUE);
            break;
        case 3:
        {
            sp->setColor(ccYELLOW);
        }
            break;
        default:
            break;
    }

    TestData::shareData()->addData(selectItem->getSid(), selectItem->getColorId(), selectItem->getType(), i, j);
}
void MapEditLayer::draw()
{
    CCLayerColor::draw();
    ccDrawColor4B(0,255,255,255);
    CHECK_GL_ERROR_DEBUG();
    for (int i = 0; i<9; i++)
    {
        ccDrawLine(ccp(30 +i*32, 100), ccp(30 +i*32,100 + 6* 32));
    }
    for (int j = 0; j< 7; j++)
    {
        ccDrawLine(ccp(30, 100 + j*32), ccp(30 + 8*32,100 + j*32));
    }
}
void MapEditLayer::back()
{
//    CCScene *pScene = SGGameMenu::create();
//    
//    CCDirector::sharedDirector()->replaceScene(pScene);
}