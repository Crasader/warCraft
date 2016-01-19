//
//  SGStrengConfirm.h
//  GameSanGuo
//
//  Created by kanata on 13-11-15.
//
//

#ifndef __GameSanGuo__SGStrengConfirm__
#define __GameSanGuo__SGStrengConfirm__

#include <iostream>
#include "SGBaseBox.h"
#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#endif /* defined(__GameSanGuo__SGStrengConfirm__) */


class SGStrengConfirm: public SGBaseBox
{
public:
//      CCScrollView *m_scrollview;
    SGStrengConfirm();
   
    ~SGStrengConfirm();
    void initView();
    void  backhandle(CCObject*obj);
    void strenghandel(CCObject*obj);

private:
    
    SGBoxDelegate *_dele;
    SGOfficerCard *_card;
    int enterType;
     
private:
  
    void strengHandlerzjj();
    void strengHandlerwjj();
    void setstar(int star,int max);

protected:
  
public:
   
    static SGStrengConfirm *create(SGBoxDelegate *bdg, SGOfficerCard *card,int type);
    bool init(SGBoxDelegate *bdg, SGOfficerCard *card,int type);
};
    
    
    
    
    



