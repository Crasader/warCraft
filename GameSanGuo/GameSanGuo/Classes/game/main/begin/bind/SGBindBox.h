//
//  SGBindBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#ifndef __GameSanGuo__SGBindBox__
#define __GameSanGuo__SGBindBox__

#include "SGBaseBox.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class SGBindBox : public SGBaseBox
{
private:
    
    CCLabelTTF *label_account_error;
    CCLabelTTF *label_password_error;
    CCLabelTTF *label_password_again_error;
    
    CCEditBox *edit_account;
    CCEditBox *edit_password;
    CCEditBox *edit_password_again;
    
    
    void viewDidLoad();
    
    
public:
    
    SGBindBox();
    
    ~SGBindBox();
    
    static SGBindBox *create(SGBoxDelegate *dg);
};
#endif /* defined(__GameSanGuo__SGBindBox__) */
