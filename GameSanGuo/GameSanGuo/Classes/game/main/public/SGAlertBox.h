//
//  SGAlertBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-2-4.
//
//

#ifndef __GameSanGuo__SGAlertBox__
#define __GameSanGuo__SGAlertBox__

#include "SGBaseBox.h"
class SGAlertBox : public SGBaseBox
{
public:
    static SGAlertBox *create(SGBoxDelegate *delegate, const char *msg);
    virtual bool init(SGBoxDelegate *delegate, const char *msg);
};
#endif /* defined(__GameSanGuo__SGAlertBox__) */
