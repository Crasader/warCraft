//
//  TestData.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-22.
//
//

#include "TestData.h"
#include "SGGridBase.h"
#include "SGBattleMap.h"
#include "ShiBing.h"
static TestData *s_ShareBattle = NULL;

TestData* TestData::shareData(void)
{
    if (!s_ShareBattle) {
        s_ShareBattle = new TestData();
        //        s_ShareBattle->init();
    }
    return s_ShareBattle;
}
BData *TestData::get(int i, int c,int t ,GameIndex index)
{
    BData *data = new BData();
    data->setSid(i);
    data->setColorId(c);
    data->setType(t);
    data->setIndex(index);
    
    data->autorelease();
    return data;
}
Index* TestData::getIndex(int i, int j)
{
    Index* index = new Index();
    index->i = i;
    index->j = j;
	//10.23//george
	index->autorelease();
    return index;
}
cocos2d::CCArray *TestData::getMapArray()
{
    CCArray *mapArray = CCArray::create();
    CCArray *indexs = getIndexArray();
    for (int i = 0; i<indexs->count(); i++)
    {
        BData *data = (BData *)indexs->objectAtIndex(i);
        switch (data->getType()) {
            case ksbone:
            {
                //                SGSoldierBase *soldier = SGSoldierBase::createWithId(data->getSid(),(sgColorType)data->getColorId(),(sgSoldierType)data->getType());
                //                SGSoldierBase *
                //                mapArray->addObject(soldier);
            }
                break;
            case ksbtwo:
            {
                
            }
                break;
            case ksbfour:
            {
                
            }
                break;
            default:
                break;
        }
        
        
    }
    return mapArray;
}
cocos2d::CCArray *TestData::getIndexArray()
{
    
    CCArray *test = CCArray::create(

                                     // id 颜色 
                                     get(1,1,1,gameIndex(0, 0)),
                                     get(3,3,1,gameIndex(0, 1)),
                                     get(1,1,1,gameIndex(0, 2)),
                                     get(1,1,1,gameIndex(0, 3)),
                                     get(3,3,1,gameIndex(0, 4)),
                                     get(2,2,1,gameIndex(0, 5)),
                                     get(1,1,1,gameIndex(0, 6)),
                                     get(1,1,1,gameIndex(0, 7)),
                                     
                                     
                                     get(5,2,2,gameIndex(1, 0)),
//                                     get(3,3,1,gameIndex(1, 1)),
                                    get(5,2,2,gameIndex(1, 1)),
                                     get(1,1,1,gameIndex(1, 2)),
                                     get(1,1,1,gameIndex(1, 3)),
                                     get(2,2,1,gameIndex(1, 4)),
                                     get(2,2,1,gameIndex(1, 5)),
                                     get(3,3,1,gameIndex(1, 6)),
                                     get(3,3,1,gameIndex(1, 7)),
                                     
                                     
                                     
                                     
                                     get(3,3,1,gameIndex(2, 2)),
//                                     get(4,2,3,gameIndex(2, 4)),

                                    get(4,3,3,gameIndex(2, 4)),
                                    get(2,2,1,gameIndex(2, 6)),
                                     get(1,1,1,gameIndex(2, 7)),
                                     
                                     
                                     get(4,3,3,gameIndex(3, 0)),
                                     get(3,3,1,gameIndex(3, 2)),
                                     get(3,3,1,gameIndex(3, 7)),
                                     
                                     
                                     
                                     
                                     get(2,2,1,gameIndex(4, 2)),
                                     get(3,3,1,gameIndex(4, 4)),
                                     get(2,2,1,gameIndex(4, 5)),
                                     get(1,1,1,gameIndex(4, 7)),
                                     
                                     
                                     get(1,1,1,gameIndex(5, 0)),
                                     get(2,2,1,gameIndex(5, 1)),
                                     get(1,1,1,gameIndex(5, 2)),
                                     get(3,3,1,gameIndex(5, 5)),
                                     get(1,1,1,gameIndex(5, 7)),
                                    
                                    
                                    
                                    NULL);
    return test;
}
CCArray *TestData::getMapData()
{
    return mapArray;
}
void TestData::addData(int i, int c, int t, int indexi, int indexj)
{
    if (!mapArray)
    {
        mapArray = CCArray::create();
        mapArray->retain();
    }
//    CCArray *indexs = CCArray::create();
//    switch (t)
//    {
//        case 1:
//        {
//            indexs->addObject(getIndex(indexi, indexj));
//        }
//            break;
//        case 2:
//        {
//            indexs -> addObject(getIndex(indexi, indexj));
//            indexs->addObject(getIndex(indexi+1, indexj));
//        }
//            break;
//        case 3:
//        {
//            indexs->addObject(getIndex(indexi, indexj));
//            indexs->addObject(getIndex(indexi, indexj + 1));
//            indexs->addObject(getIndex(indexi +1, indexj));
//            indexs->addObject(getIndex(indexi + 1, indexj +1));
//        }
//            break;
//        default:
//            break;
//    }
    BData *data = get(i, c, t, gameIndex(indexi, indexj));
    CCLOG("data:%i",data->getSid());
    mapArray->addObject(data);
}
void TestData::removeData(int indexi,int indexj)
{
    int removeData = -1;
    for (int i = 0; i<mapArray->count(); i++)
    {
        BData *data = (BData *)mapArray->objectAtIndex(i);
        
        if (data->getType() == 1)
        {
            if (data->getIndex().i == indexi && data->getIndex().j == indexj)
            {
                removeData = i;
            }
        }
        else if(data->getType() == 2)
        {
            GameIndex index = data->getIndex();
            if ((index.i == indexi && index.j == indexj)
                || ((index.i + 1) == indexi && index.j == indexj))
            {
                removeData = i;
            }
        }
        else
        {
            GameIndex index = data->getIndex();
            if ((index.i == indexi && index.j == indexj)
                || ((index.i + 1) == indexi && index.j == indexj)
                || (index.i == indexi && (index.j+1) == indexj)
                || ((index.i + 1) == indexi && (index.j+1) == indexj))
            {
                removeData = i;
            }
        }
    }
    if (removeData>=0)
    {
        mapArray->removeObjectAtIndex(removeData);
    }
  
}