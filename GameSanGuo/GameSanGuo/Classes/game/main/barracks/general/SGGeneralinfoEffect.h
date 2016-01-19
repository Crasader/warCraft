//
//  SGGeneralinfoEffect.h
//  GameSanGuo
//
//  Created by kanata on 13-11-15.
//
//

#ifndef __GameSanGuo__SGGeneralinfoEffect__
#define __GameSanGuo__SGGeneralinfoEffect__

#include <iostream>
#include "SGBaseBox.h"
#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#endif /* defined(__GameSanGuo__SGGeneralinfoEffect__) */


class SGGeneralinfoEffect: public SGBaseBox
{
public:
      CCScrollView *m_scrollview;
    SGGeneralinfoEffect();
   
    ~SGGeneralinfoEffect();
    void initView();
    void  backhandle(CCObject*obj);

private:
    
    SGBoxDelegate *_dele;
    SGOfficerCard *_card;
    int enterType;

     
private:
  
    void strengHandlerzjj();
    void strengHandlerwjj();

protected:
  
public:
   
    static SGGeneralinfoEffect *create(SGBoxDelegate *bdg, SGOfficerCard *card,int type);
    bool init(SGBoxDelegate *bdg, SGOfficerCard *card,int type);
};
    
    
    
    
    



