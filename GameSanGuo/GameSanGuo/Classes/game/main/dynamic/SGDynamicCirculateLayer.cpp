//
//  SGDynamicCirculateLayer.cpp
//  Test
//
//  Created by 江南岸 on 13-4-8.
//
//

#include "SGDynamicCirculateLayer.h"
#include "SGDynamicModel.h"
#include "SGLayout.h"
#include "SGCCLabelTTF.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"


SGDynamicCirculateLayer::SGDynamicCirculateLayer()
{
    stringArray = NULL;
    labelArray = NULL;
    moveArray = NULL;
    localArray = NULL;
    _isMoving = false;
    _isInMiddle = false;
    _inMiddleCounter = 0;
    localCount = 0;
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_DYNAMIC_MOVINGMESSAGE, this, callfuncO_selector(SGDynamicCirculateLayer::dynamicListener));
    
}

SGDynamicCirculateLayer::~SGDynamicCirculateLayer()
{
    CC_SAFE_RELEASE(stringArray);
    CC_SAFE_RELEASE(labelArray);
    CC_SAFE_RELEASE(moveArray);
    CC_SAFE_RELEASE(localArray);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_DYNAMIC_MOVINGMESSAGE);
}

SGDynamicCirculateLayer* SGDynamicCirculateLayer::create()
{
    SGDynamicCirculateLayer *dynamic = new SGDynamicCirculateLayer();
    if(dynamic && dynamic->initWithColor(ccc4(255, 255, 255, 0), SGLayout::getSkewing(320), 50))
    {
        dynamic->viewDidLoad();
        dynamic->autorelease();
        return dynamic;
    }
    
    CC_SAFE_DELETE(dynamic);
    return NULL;
    
}

void SGDynamicCirculateLayer::viewDidLoad()
{
    //存放所有CCLabelTTF的数组
    labelArray = CCArray::create();
    labelArray->retain();
    
    //存放所有动态的数组
    stringArray = CCArray::create();
    stringArray->retain();
    
    //存放当前移动CCLabelTTF的数组
    moveArray = CCArray::create();
    moveArray->retain();
    
    localArray = CCArray::create();
    localArray->retain();
    CCArray *arr = NULL;
    arr =SGStaticDataManager::shareStatic()->gettips();
    
    for (int i = 0 ; i<arr->count(); i++) {
        SGDynamicModel *dynamic = new SGDynamicModel();
        dynamic->setDyId(1);
//        dynamic->getDyArray()->addObject(CCString::createWithFormat("%d.马上要关服了  都快下吧",i));
         dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
         dynamic->getDyArray()->addObject(arr->objectAtIndex(i));
        localArray->addObject(dynamic);
        dynamic->release();
    }
    
    //初始化一个Label池
    for(int i=0;i<8;i++)
    {
        SGCCLabelTTF *label = SGCCLabelTTF::create("", FONT_BOXINFO, 28 , ccc3(245, 187, 81));
        label->setAnchorPoint(CCPointMake(0, 0.5));
        //modify by:zyc. merge into create.
        //label->setColor(ccc3(245, 187, 81));
        addChild(label);
        labelArray->addObject(label);
    }
    schedule(schedule_selector(SGDynamicCirculateLayer::updateTimer), 0.01);
}

void SGDynamicCirculateLayer::updateTimer()
{
    if(_isMoving)
    {
        if(0 == moveArray->count())
        {
            //no moving label
            return;
        }
        
        int allWidth = 0;
        CCObject *object = NULL;
        CCARRAY_FOREACH(moveArray, object)
        {
            SGCCLabelTTF *label = (SGCCLabelTTF*)object;
            CCPoint pt = label->getPosition();
            if(!_isInMiddle)
            {
                pt.x--;
                label->setPosition(ccp(--pt.x, pt.y));
            }
            
            allWidth += label->getContentSize().width;
            
        }
        
        //检测最后一个label移除屏幕，这时结束这个动态
        SGCCLabelTTF *labelLast = (SGCCLabelTTF*)moveArray->lastObject();
        
        if(labelLast->getPosition().x == /*SGLayout::getSkewing(320)/2+*/allWidth/2-labelLast->getContentSize().width)
        {
            _isInMiddle = true;
            //此时已经走到了屏幕中间
            _inMiddleCounter ++;
            if(_inMiddleCounter >= 100)
            {
                _isInMiddle = false;
                _inMiddleCounter = 0;
            }
        }else{
            _isInMiddle = false;
        }
        
        if(labelLast->getPosition().x  + labelLast->getContentSize().width/2 < -skewing(320))
        {
            _isMoving = false;
            moveArray->removeAllObjects();
            for(int i=0;i<labelArray->count();i++)
            {
                SGCCLabelTTF *label = (SGCCLabelTTF*)labelArray->objectAtIndex(i);
                label->setInsideColor(ccc3(245, 187, 81));
            }
        }
    
    }else{
        if(0 == stringArray->count())
        {
            //全部动态已发出
            stringArray->addObject(localArray->objectAtIndex(localCount));
            localCount++;
            if (localCount == localArray->count()) {
                localCount = 0;
            }
            constructDynamic();
            
            _isMoving = true;
            return;
        }
        
        constructDynamic();
        
        _isMoving = true;
        
    }
}

void SGDynamicCirculateLayer::constructDynamic()
{
    if(0 == stringArray->count())
    {
        //数组已空
        return;
    }
    
    SGDynamicModel *dynamic = (SGDynamicModel*)stringArray->objectAtIndex(0);

    moveArray->removeAllObjects();
    
    int positonX = SGLayout::getSkewing(320);
    
    int count = dynamic->getDyArray()->count();
    int j= 0;
    for(int i=0;i<count;i++)
    {
        CCString *strColor = (CCString*)dynamic->getDyArray()->objectAtIndex(i);
        i++;
        SGCCLabelTTF *label = (SGCCLabelTTF*)labelArray->objectAtIndex(j++);
        label->setString(((CCString*)dynamic->getDyArray()->objectAtIndex(i))->getCString());
        label->setPosition(ccp(positonX,25));
        label->setInsideColor(FontColor(strColor));
        moveArray->addObject(label);

//        SGCCLabelTTF *label = (SGCCLabelTTF*)labelArray->objectAtIndex(i);
//        label->setString(((CCString*)dynamic->getDyArray()->objectAtIndex(i))->getCString());
//        label->setPosition(ccp(positonX,25));
//        moveArray->addObject(label);
//        
//        if(dynamic->getDyId() == 1 || dynamic->getDyId() == 2)
//        {
//            if(i%2 == 1)
//            {
//                label->setInsideColor(ccRED);
//                if(i!=1)
//                {
//                    label->setInsideColor(FontColor(((CCString*)dynamic->getDyArray()->objectAtIndex(i))));
//                }
//            }
//        }else
//        {
//            if(i%2 == 0)
//            {
//                label->setInsideColor(ccRED);
//                if(i!=0)
//                {
//                     label->setInsideColor(FontColor(((CCString*)dynamic->getDyArray()->objectAtIndex(i))));
//                }
//            }
//        }
        
        positonX += label->getContentSize().width;
    }
    
    stringArray->removeObjectAtIndex(0);
}
ccColor3B SGDynamicCirculateLayer::FontColor(CCString * str)
{
    //绿 蓝 紫 橙 对应转生次数 3 4 5 6
    
    if(str->isEqual(CCString::create(str_DynamicCirculateLayer_str15)))
    {
        return ccc3(0 , 239 , 22);
    }else if(str->isEqual(CCString::create(str_DynamicCirculateLayer_str16)))
    {
        return ccc3(75, 147, 255);
    }else if(str->isEqual(CCString::create(str_DynamicCirculateLayer_str17)))
    {
        return  ccc3(215,56,255);
    }else if(str->isEqual(CCString::create(str_DynamicCirculateLayer_str18)))
    {
        return ccc3(249, 158, 0);
    }
    else if(str->isEqual(CCString::create(str_DynamicCirculateLayer_str19)))
    {
        return ccRED;
    }
    return ccc3(245, 187, 81);
}
void SGDynamicCirculateLayer::dynamicListener(cocos2d::CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("dynamicListener error !!!");
        return;
    }
    main::ServerMovingResponse* response = (main::ServerMovingResponse*)request->m_msg;
    //存放一个动态数组
    int modelSize = response->model_size();
    for(int i=0;i<modelSize;i++)
    {
        //每条动态的模型
        main::ServerMovingModel model2 = response->model(i);
        
        SGDynamicModel *dynamic = new SGDynamicModel();
        
        int model2Id = model2.id();
        
        //每条动态里边会有一些物品，物品数组，用来拼接起来形成字符串
        int model2Size = model2.model_size();
        
        dynamic->setDyId(model2Id);
        switch(model2Id)//不同情景下解析顺序不同当心,下面已有注释
        {
            case 1://1.2是获得了卡牌探访之类的
            case 2:
            {
                //3个拼接文字，所以如果不是3个，会出错
                if(3 != model2Size)
                {
                    return;
                }
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20)); // 成对出现 前面是文本颜色后面是具体文本下同
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str1));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str19));
                dynamic->getDyArray()->addObject(CCString::create(model2.model(0).msg()));//玩家名字
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str2));
                const char * str = "";
                switch(atoi((model2.model(1).msg()).c_str()))//此时是品质
                {
                    case 1:
                    case 2:
                    case 3:
                         {
                             str = str_DynamicCirculateLayer_str15;
                         }
                        break;
                    case 4:
                         {
                              str = str_DynamicCirculateLayer_str16;
                         }
                        break;
                    case 5:
                         {
                              str = str_DynamicCirculateLayer_str17;
                         }
                        break;
                    case 6:
                         {
                              str = str_DynamicCirculateLayer_str18;
                         }
                        break;
                    default:
                        break;
                        
                }
                dynamic->getDyArray()->addObject(CCString::create(str));
                dynamic->getDyArray()->addObject(CCString::create(str));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                if(model2Id == 1)
                {
                    dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str3));
                }else{
                    dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str4));
                }
                
                dynamic->getDyArray()->addObject(CCString::create(str));
                dynamic->getDyArray()->addObject(CCString::create(model2.model(2).msg()));//卡牌名字
                
                
                break;
            }
                
            case 3://3,4 是卡牌进阶
            case 4:
            {
                if(4 != model2Size)
                {
                    return;
                }
                
                const char * str = "";
                switch(atoi((model2.model(2).msg()).c_str()))//品质
                {
                    case 1:
                    case 2:
                    case 3:
                    {
                        str = str_DynamicCirculateLayer_str15;
                    }
                        break;
                    case 4:
                    {
                        str = str_DynamicCirculateLayer_str16;
                    }
                        break;
                    case 5:
                    {
                        str = str_DynamicCirculateLayer_str17;
                    }
                        break;
                    case 6:
                    {
                        str = str_DynamicCirculateLayer_str18;
                    }
                        break;
                    default:
                        break;
                        
                }
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str5));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str19));
                dynamic->getDyArray()->addObject(CCString::createWithFormat("%s",model2.model(0).msg().c_str()));//玩家名字
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                if(model2Id == 3)
                {
                    dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str6));

                }else{
                    dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str7));

                }
                dynamic->getDyArray()->addObject(CCString::create(str));
                dynamic->getDyArray()->addObject(CCString::create(model2.model(1).msg()));//卡牌名字
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str8));
                
                
                
                
                dynamic->getDyArray()->addObject(CCString::create(str));
                dynamic->getDyArray()->addObject(CCString::create(str));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str9));
//                CCString* ss = CCString::create(model2.model(3).msg());
                int adnum =CCString::create(model2.model(3).msg())->intValue();//转生公告加入+n的提示
                if (adnum>0) {
                    dynamic->getDyArray()->addObject(CCString::create(str));
                    dynamic->getDyArray()->addObject(CCString::createWithFormat("+%d",adnum));
                }
                

                break;
            }
                
            case 5://已废除
            {
                if(2 != model2Size)
                {
                    return;
                }
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str5));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str19));
                dynamic->getDyArray()->addObject(CCString::createWithFormat("%s",model2.model(0).msg().c_str()));
                
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str10));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str19));
                dynamic->getDyArray()->addObject(CCString::create(model2.model(1).msg()));
                
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str11));
                
                break;
            }
                
            case 6:
            {
                if(3 != model2Size)
                {
                    return;
                }
                
                
                
                break;
            }
            case 7://活动公告
            {
                if(1 != model2Size)
                {
                    return;
                }
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str20));
                dynamic->getDyArray()->addObject(CCString::create(model2.model(0).msg()));
                break;
            }
            case 8://系统公告
            {
                if(1 != model2Size)
                {
                    return;
                }
                dynamic->getDyArray()->addObject(CCString::create(str_DynamicCirculateLayer_str19));
                dynamic->getDyArray()->addObject(CCString::create(model2.model(0).msg()));
                break;
            }
        }
        
        stringArray->addObject(dynamic);
        dynamic->release();

    }
}









