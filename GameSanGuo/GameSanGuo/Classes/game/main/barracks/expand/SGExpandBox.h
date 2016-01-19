//
//  SGExpandBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-22.
//
//

#ifndef __GameSanGuo__SGExpandBox__
#define __GameSanGuo__SGExpandBox__
#include "SGBaseBox.h"
class SGExpandBox : public SGBaseBox
{
private:
    void confirmHandler();
public:
    SGExpandBox(void);
    ~SGExpandBox(void);
    
    void storeEnterListener(CCObject *obj);
    static SGExpandBox *create(SGBoxDelegate *bdg);
    bool init(SGBoxDelegate *bdg);
    //关半当前的窗口
    void closeExpandBox();
};

#endif /* defined(__GameSanGuo__SGExpandBox__) */
