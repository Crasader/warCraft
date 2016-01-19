//
//  SGComplainCell.h
//  GameSanGuo
//
//  Created by kanata on 14-1-6.
//
//

#ifndef __GameSanGuo__SGComplainCell__
#define __GameSanGuo__SGComplainCell__

#include <iostream>
#include "SNSTableViewCellItem.h"
#include "cocos-ext.h"
#include "SGQAConcent.h"


USING_NS_CC_EXT;

class QADetail:public CCObject
{
public:
    int typetype;
    int idid;
    
    
    
public:
    QADetail();
    ~QADetail();
    
public:
    CC_SYNTHESIZE(int , m_gmid, gmid);
    CC_SYNTHESIZE_RETAIN(CCString* , m_time, time);
    CC_SYNTHESIZE_RETAIN(CCString*, m_content, content);
    
    
} ;













class  SGComplainCell:public SNSTableViewCellItem
{

    public:
    SGComplainCell();
    virtual ~SGComplainCell();

    static SGComplainCell* create(SGQAConcent*data);
    void  deletefun(CCNode*node);
    void  lookfun(CCNode*node);
   // void checkqaresponse(CCObject*obj);

    private:
    void initView(SGQAConcent*data);
    void initBackground();
    CCSize bgSize;
    CCSprite *spriteBg;
    SGCCLabelTTF *qtitle;  // 问题标题
//    SGCCLabelTTF *time;      //时间
    SGQAConcent  *_data;
};
#endif /* defined(__GameSanGuo__SGComplainCell__) */