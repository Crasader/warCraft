//
//  SGSortBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-24.
//
//

#include "SGSortBox.h"
#include "SGOfficerCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGMenu.h"
#include "SGPlayerInfo.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
#include <algorithm>
#include "SGStringConfig.h"

#define btnwidth 203
#define btnheight 82
#define wid 20
#define hgt 30
SGSortBox::SGSortBox()
: down(NULL)
, up(NULL)
, _arrayCard(NULL)
, isfromstreng(0)
, boxscroll(NULL)
, cardType(0)
, boxscrollViewContainer(NULL)
, type(0)
{
}
SGSortBox::~SGSortBox()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_sortBox);

}
SGSortBox *SGSortBox::create(SGBoxDelegate *bdg,SGSortLayerDelegate *callBack , cocos2d::CCArray *arrayCard,int isstreng,int entertype )
{
    SGSortBox *Box = new SGSortBox();
    if (Box && Box->init(bdg,arrayCard,isstreng))
    {
        Box->type = entertype;
        Box->isfromstreng = isstreng;
        Box->_arrayCard = arrayCard;
        Box->_delegate = bdg;
        Box->_delegateCallBack = callBack;
        Box->autorelease();
        return Box;
    }
    CC_SAFE_DELETE(Box);
    return NULL;
}
bool SGSortBox::init(SGBoxDelegate *bdg,CCArray *arrayCard,int isstreng)
{
    if (!SGBaseBox::init(bdg,box_sortTag,CCSizeMake(535, 560)))
    {
        return false;
    }
    
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_box_sortBox, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_box_sortBox);
	ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_box_sortBox);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    fontbg->setPreferredSize(CCSizeMake(480, 384));
    fontbg->setPosition(ccpAdd(center, ccp(0, 18)));
    
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_SortBox_str1, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    this->addChild(title);
    title->setPosition(biao->getPosition());
    
    boxscrollViewContainer = CCLayer::create();
    boxscrollViewContainer->setContentSize(CCSize(btnwidth*2 + wid, btnheight*6 +hgt*5));
    boxscrollViewContainer->setPosition(CCPointZero);
    
    CCMenu *tempMenu = CCMenu::create();
	tempMenu->setTouchPriority(128);
    boxscrollViewContainer->addChild(tempMenu);
    tempMenu->setPosition(CCPointZero);
    CCArray *arrayButts = CCArray::create();
    for (int i = 0; i<11; i++)
    {
        if(i!= 1&&i!= 3)
        {
        SGButton *btn = SGButton::createFromLocal("jitian_btn.png", str_back, this, menu_selector(SGSortBox::buttonClickSort),ccp(0,5),FONT_PANGWA,ccWHITE,32,false,true);
        switch (i)
        {
            case 0:
                btn->setFont(str_SortBox_str2);
                break;
            case 1:
                btn->setFont(str_SortBox_str3);
                break;
                
            case 2:
                btn->setFont(str_SortBox_str4);
                break;
                
            case 3:
                btn->setFont(str_SortBox_str5);
                break;
                
            case 4:
                btn->setFont(str_SortBox_str6);
                break;
                
            case 5:
                btn->setFont(str_SortBox_str7);
                break;
                
            case 6:
                btn->setFont(str_SortBox_str8);
                break;
                
            case 7:
                btn->setFont(str_SortBox_str9);
                break;
                
            case 8:
                btn->setFont(str_SortBox_str10);
                break;
                
            case 9:
                btn->setFont(str_SortBox_str11);
                break;
                
            case 10:
                btn->setFont(str_SortBox_str12);
                break;
                
            case 11:
                btn->setFont(str_SortBox_str13);
                             break;
                             
            default:
                break;
        }
//

        tempMenu->addChild(btn);        
        btn->setTag(i);
        arrayButts->addObject(btn);
        
        }
    }
    
    for(int i = 0 ;i < arrayButts->count(); i++)
    {
        SGButton *btn = (SGButton *)arrayButts->objectAtIndex(i);
        btn->setPosition(ccp(btn->getContentSize().width/2+i%2 * (wid + btn->getContentSize().width),btn->getContentSize().height*6 + hgt*5 - btn->getContentSize().height/2- i/2 * (btn->getContentSize().height + hgt)+hgt));
        
    }
    boxscroll = CCScrollView::create(CCSizeMake(btnwidth*2 + wid, fontbg->getContentSize().height - 10));
    boxscroll->setDirection(kCCScrollViewDirectionVertical);
    boxscroll->setContainer(boxscrollViewContainer);
    boxscroll->setContentSize(CCSizeMake(btnwidth*2 + wid, hgt+ btnheight/2+ btnheight*6 +hgt*5));
    boxscroll->setPosition(ccpAdd(center, ccp( -(wid+btnwidth*2)/2, -btnheight*2 - 5)));
    boxscroll->setContentOffset(ccp(0,-339));
    this->addChild(boxscroll);
    
    down = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    this->addChild(down);
    down->setRotation(90);
    down->setPosition(ccpAdd(fontbg->getPosition(), ccp(-1.5, -fontbg->getContentSize().height*.57)));
    
    up = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    this->addChild(up);
    up->setPosition(ccpAdd(fontbg->getPosition(), ccp(-1.5, fontbg->getContentSize().height*.57)));
    up->setRotation(-90);
    up->setVisible(false);
    this->schedule(schedule_selector(SGSortBox::updateTime), 0.5);
    
    if (isstreng == 2)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("firstenterskill", CCString::createWithFormat("%d",1)->getCString());
    }
    
    if (arrayCard &&arrayCard->count()>0)
    {
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)arrayCard->objectAtIndex(0);
        if(!strcmp(card->getClassName(),"SGEquipCard"))
        {
            SGButton *temp = (SGButton *)tempMenu->getChildByTag(10);
            temp->setFont(str_SortBox_str13);
            CCUserDefault::sharedUserDefault()->setStringForKey("firstenterequip", CCString::createWithFormat("%d",1)->getCString());
        }
        if(!strcmp(card->getClassName(),"SGOfficerCard"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("firstenterofficer", CCString::createWithFormat("%d",1)->getCString());
        }
        if(!strcmp(card->getClassName(),"SGPropsCard"))
        {
            SGPropsCard *card_ = (SGPropsCard *)card;
            if (card_->getType() == 2)
            {
                SGButton *temp = (SGButton *)tempMenu->getChildByTag(10);
                CCSprite *a = CCSprite::createWithSpriteFrameName("barrack_sort_buttonbg.png");//barrack_sort_11.png
                temp->setFontImage(a->displayFrame());
                CCUserDefault::sharedUserDefault()->setStringForKey("firstenterequip", CCString::createWithFormat("%d",1)->getCString());
            }
        }
		if(!strcmp(card->getClassName(), "SGConsumableCard"))
		{
			CCUserDefault::sharedUserDefault()->setStringForKey("consumablesort", CCString::createWithFormat("%d",1)->getCString());
		}
    }
    CCUserDefault::sharedUserDefault()->flush();
    
    SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, menu_selector(SGSortBox::boxClose),CCPointZero,false,true);
    this->addBtn(cancelButton);
    cancelButton->setPosition(ccpAdd(center, ccp(0, -560/2+40)));
    
    return true;
}
SGSortBox *SGSortBox::updateBtn(SGButton *btn,int entertype)
{
    EFFECT_PLAY(MUSIC_BTN);
    std::string str;
    if (entertype == 1)
    {
        str = CCUserDefault::sharedUserDefault()->getStringForKey("officersort");
    
    }
    else if (entertype == 2)
    {
        str = CCUserDefault::sharedUserDefault()->getStringForKey("equipsort");
    }
    else if (entertype == 3)
    {
        str = CCUserDefault::sharedUserDefault()->getStringForKey("strengofficersort");
    }
    else if (entertype == 4)
    {
        str = CCUserDefault::sharedUserDefault()->getStringForKey("strengequipsort");
    }
    else if (entertype == 5)
    {
        str = CCUserDefault::sharedUserDefault()->getStringForKey("strengskill");
    }
    else if (entertype == 6)//消耗品
	{
		str = CCUserDefault::sharedUserDefault()->getStringForKey("consumablesort");
	}
    int type = CCString::create(str)->intValue();
    switch (type)
    {
        case 0:
            btn->setFont(str_SortBox_str2);
            break;
        case 1:
            btn->setFont(str_SortBox_str3);
            break;
            
        case 2:
            btn->setFont(str_SortBox_str4);
            break;
            
        case 3:
            btn->setFont(str_SortBox_str5);
            break;
            
        case 4:
            btn->setFont(str_SortBox_str6);
            break;
            
        case 5:
            btn->setFont(str_SortBox_str7);
            break;
            
        case 6:
            btn->setFont(str_SortBox_str8);
            break;
            
        case 7:
            btn->setFont(str_SortBox_str9);
            break;
            
        case 8:
            btn->setFont(str_SortBox_str10);
            break;
            
        case 9:
            btn->setFont(str_SortBox_str11);
            break;
            
        case 10:
            btn->setFont(str_SortBox_str12);
            break;
            
        case 11:
            btn->setFont(str_SortBox_str13);
            break;
            
        default:
            break;
    }
    
    return 0;
}
static int SGMaxStarLess(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getUpgradestar() > ((SGBaseMilitaryCard *)p2)->getUpgradestar());
}
static int SGMaxStarMore(const CCObject* p1, const CCObject* p2)
{
	return (((SGBaseMilitaryCard *)p1)->getUpgradestar() < ((SGBaseMilitaryCard *)p2)->getUpgradestar());
}


SGSortBox *SGSortBox::sortCards(cocos2d::CCArray *_arrayCard, int type, int isstreng,bool needSave)
{
    int arrayCount = _arrayCard->count();
    if (needSave)
    {
        if (_arrayCard &&arrayCount>0)
        {
            if (isstreng == 1)
            {
                SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arrayCard->objectAtIndex(0);
                if(!strcmp(card->getClassName(),"SGEquipCard"))
                {
                    std::string str1;
                    str1 = CCUserDefault::sharedUserDefault()->getStringForKey("firstenterequip");
                    int firstequip = CCString::create(str1)->intValue();
                    if (firstequip == 0)
                    {
                        type = 1;
                    }
                    
                    if (type == 10)
                    {
                        type = 11;
                    }
                    CCUserDefault::sharedUserDefault()->setStringForKey("equipsort", CCString::createWithFormat("%d",type)->getCString());
                }
                if(!strcmp(card->getClassName(),"SGOfficerCard"))
                {
                    std::string str;
                    str = CCUserDefault::sharedUserDefault()->getStringForKey("firstenterofficer");
                    int firsttype = CCString::create(str)->intValue();
                    if (firsttype == 0)
                    {
                        type = 1;
                    }
                    
                    CCUserDefault::sharedUserDefault()->setStringForKey("officersort", CCString::createWithFormat("%d",type)->getCString());
                }
                if(!strcmp(card->getClassName(),"SGPropsCard"))
                {
                    SGPropsCard *card_ = (SGPropsCard *)card;
                    if (card_->getType() == 2)
                    {
                        std::string str1;
                        str1 = CCUserDefault::sharedUserDefault()->getStringForKey("firstenterequip");
                        int firstequip = CCString::create(str1)->intValue();
                        if (firstequip == 0)
                        {
                            type = 3;
                        }
                        
                        if (type == 10)
                        {
                            type = 11;
                        }
                        CCUserDefault::sharedUserDefault()->setStringForKey("equipsort", CCString::createWithFormat("%d",type)->getCString());
                    }
                }
                
            }
            else if( isstreng == 0)
            {
                SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arrayCard->objectAtIndex(0);
                if(!strcmp(card->getClassName(),"SGEquipCard"))
                {
                    if (type == 10)
                    {
                        type = 11;
                    }
                    CCUserDefault::sharedUserDefault()->setStringForKey("strengequipsort", CCString::createWithFormat("%d",type)->getCString());
                }
                if(!strcmp(card->getClassName(),"SGOfficerCard"))
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("strengofficersort", CCString::createWithFormat("%d",type)->getCString());
                }
                if(!strcmp(card->getClassName(),"SGPropsCard"))
                {
                    SGPropsCard *card_ = (SGPropsCard *)card;
                    if (card_->getType() == 2)
                    {
                        if (type == 10)
                        {
                            type = 11;
                        }
                        CCUserDefault::sharedUserDefault()->setStringForKey("strengequipsort", CCString::createWithFormat("%d",type)->getCString());
                    }
                }
            }
            else if( isstreng == 2)
            {
                
                std::string str;
                str = CCUserDefault::sharedUserDefault()->getStringForKey("firstenterskill");
                int firsttype = CCString::create(str)->intValue();
                if (firsttype == 0)
                {
                    type = 1;
                }
                CCUserDefault::sharedUserDefault()->setStringForKey("strengskill", CCString::createWithFormat("%d",type)->getCString());
            }
            else if (isstreng == 3)//为消耗品加的
            {
                std::string str;
                str = CCUserDefault::sharedUserDefault()->getStringForKey("consumablesort");
                int firstType = CCString::create(str)->intValue();
                if (firstType == 0)
                    type = 0;
                CCUserDefault::sharedUserDefault()->setStringForKey("consumablesort", CCString::createWithFormat("%d", type)->getCString());
            }
        }
        
        CCUserDefault::sharedUserDefault()->flush();
    }

    int num1_atk = 0;
    int num2_atk = 0;
    int num1_def = 0;
    int num2_def = 0;
    int num1_speed = 0;
    int num2_speed = 0;
    int num1_morale = 0;
    int num2_morale = 0;
    int num1_gov = 0;
    int num2_gov = 0;
    int star[7] = {0,0,0,0,0,0,0};//同星级计数
    
    for(int i=0;i<arrayCount-1;i++)
    {
        for(int m=i+1;m<arrayCount;m++)
        {
            
            SGBaseMilitaryCard *cardSoldier = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(i);
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(m);
            


            if(!strcmp(cardSoldier->getClassName(),"SGOfficerCard"))
            {
                SGOfficerCard *officer = (SGOfficerCard*)cardSoldier;
                num1_atk = officer->getBaseAtk();
                num1_def = officer->getBaseDef();
                num1_speed = officer->getBaseSpeed();
                num1_morale = officer->getBaseMorale();
                num1_gov = officer->getGoverning();
                CCArray *array = officer->getOfficerEquips();
                if(array)
                {
                    for(int i=0;i<array->count();i++)
                    {
                        SGEquipCard *equip = (SGEquipCard*)array->objectAtIndex(i);
                        num1_atk += equip->getAtk();
                        num1_def += equip->getDef();
                        num1_speed += equip->getSpeed();
                        num1_morale += equip->getMorale();
                        num1_gov += equip->getGoverning();
                    }
                }
                
            }
            else
            {
                num1_atk = cardSoldier->getAtk();
                num1_def = cardSoldier->getDef();
                num1_speed = cardSoldier->getSpeed();
                num1_morale = cardSoldier->getMorale();
                num1_gov = cardSoldier->getGoverning();
            }
        
            if(!strcmp(card->getClassName(), "SGOfficerCard"))
            {
                SGOfficerCard *officer = (SGOfficerCard*)card;
                num2_atk = officer->getBaseAtk();
                num2_def = officer->getBaseDef();
                num2_speed = officer->getBaseSpeed();
                num2_morale = officer->getBaseMorale();
                num2_gov = officer->getGoverning();
                
                CCArray *array = officer->getOfficerEquips();
                if(array)
                {
                    for(int i=0;i<array->count();i++)
                    {
                        SGEquipCard *equip = (SGEquipCard*)array->objectAtIndex(i);
                        num2_atk += equip->getAtk();
                        num2_def += equip->getDef();
                        num2_speed += equip->getSpeed();
                        num2_morale += equip->getMorale();
                        num2_gov += equip->getGoverning();
                    }
                    
                }
            }
            else
            {
                num2_atk = card->getAtk();
                num2_def = card->getDef();
                num2_speed = card->getSpeed();
                num2_morale = card->getMorale();
                num2_gov = card->getGoverning();
            }
            
            switch (type)
            {
                case 0:    //新获得
                {
                    if(cardSoldier->getSsid() < card->getSsid())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                        
                    }
                }
                    break;
                case 1:    //星级高
                {
                    
                    if(cardSoldier->getCurrStar() < card->getCurrStar())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                }
                    break;
                case 2: //等级高
                {
                    if(cardSoldier->getCurrLevel() < card->getCurrLevel())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                case 3:    //星级低
                {
                    
                    if(cardSoldier->getCurrStar() > card->getCurrStar())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                }
                    break;
                case 4: //等级低
                {
                    if(cardSoldier->getCurrLevel() > card->getCurrLevel())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;

                case 5: //血量高
                {
                    if(num1_morale < num2_morale)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                }
                    break;
                    
                case 6: //攻击高
                {
                    
                    if(num1_atk < num2_atk)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                case 7: //速度高
                {
                    if(num1_speed < num2_speed)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                case 8: //防御高
                {
                    if(num1_def < num2_def)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }                    
                    
                }
                    break;
                case 9: //蓄力快
                {
                    if(num1_gov < num2_gov)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                case 10: //同阵营
                {
                     if(cardSoldier->getRace() < card->getRace())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                case 11: //装备类型
                {
                    if(cardSoldier->getItemType() > card->getItemType())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                default:
                {
                    CCLOG("Default...");
                    break;
                }
            }
        }
 

    }
    
    if (type == 10)
    {
        //同阵营时提前和player同国家
        int playerCountry = SGPlayerInfo::sharePlayerInfo()->getPlayerCountry();
        int index = 0;
        for (int ii = 0; ii < _arrayCard->count(); ii++)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(ii);
            if(!strcmp(card->getClassName(),"SGOfficerCard"))
            {
                if (card->getRace() == playerCountry)// 上阵武将
                {
                    card->retain();
                    _arrayCard->removeObjectAtIndex(ii);
                    _arrayCard->insertObject(card, index);
                    card->release();
                    index++;
                }
            }
        }
    }
    
    else if (type==3)//星级低
    {
        
        for (int ii=0; ii<arrayCount; ii++)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(ii);
            int stars = card->getCurrStar();
            //        for (int x=stars;x<7;x++) {
            star[stars]+=1;//index0作为开始写成0,1-6分别是1-6星的个数
            
        }
        for(int x=1 ;x<6;x++)//计算区间,为6星-5星,5星-4星等,数量要每个星级相加
        {
            star[x+1]+=star[x];
        }
        
        for (int j=0;j<6;j++)
        {
            std::sort(_arrayCard->data->arr +star[j], _arrayCard->data->arr + star[j+1], SGMaxStarMore);
        }
        
        
    }
    
    else if (type==1)//星级高
    {
        for (int ii=0; ii<arrayCount; ii++)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(ii);
            int stars = card->getCurrStar();
            star[stars-1]+=1;//index6作为开始写成0,0-5分别是1-6星的个数
            
        }
        for(int x=5 ;x>0;x--)//计算区间,为6星-5星,5星-4星等,数量要每个星级相加
        {
            star[x-1]+=star[x];
        }
        
        for (int j=5;j>=0;j--)
        {
            std::sort(_arrayCard->data->arr +star[j+1], _arrayCard->data->arr + star[j], SGMaxStarLess);
        }
    }
    
    
    int index = 0;
    for (int ii = 0; ii < _arrayCard->count(); ii++)
    {
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(ii);
        if(!strcmp(card->getClassName(),"SGEquipCard"))
        {
            if (((SGEquipCard*)card)->getOfficerCardId())// 装备道具
            {
                card->retain();
                _arrayCard->removeObjectAtIndex(ii);
                _arrayCard->insertObject(card, index);
                card->release();
                index++;
            }
        }
        if(!strcmp(card->getClassName(),"SGOfficerCard"))
        {
            int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
            int m = SGTeamgroup::shareTeamgroupInfo()->getPositionId(teamid, (SGOfficerCard*)card);
            if (m > 0)// 上阵武将
            {
                card->retain();
                _arrayCard->removeObjectAtIndex(ii);
                _arrayCard->insertObject(card, index);
                card->release();
                index++;
            }
        }
    }
    
    return 0;
}
void SGSortBox::buttonClickSort(CCNode *sender)
{
    this->sortCards(_arrayCard,sender->getTag(),isfromstreng);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    this->boxClose();
  
}


void SGSortBox::updateTime()
{   
    //CCLOG("%f",    boxscroll->getContentOffset().y);  //cgpLogDel
    float position = boxscroll->getContentOffset().y;
    if (position >= 0)
    {
        CCBlink *blink = CCBlink::create(0.5, 1);
        up->runAction(blink);
        down->setVisible(false);
    }
    if (position <= -339)
    {
        up->setVisible(false);
        CCBlink *blink1 = CCBlink::create(0.5, 1);
        down->runAction(blink1);
    }
    if (position>-339 && position<0)
    {
        up->setVisible(true);
        down->setVisible(true);
        CCBlink *blink1 = CCBlink::create(0.5, 1);
        down->runAction(blink1);
        CCBlink *blink = CCBlink::create(0.5, 1);
        up->runAction(blink);
    }
}

int SGSortBox::getIndexByType(int ssid, int sType, int iType)
{
    CCArray *array = NULL;
    if (iType == 1)
    {
        int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(teamid);
    }
    else if (iType == 2)
    {
        array = CCArray::create();
        CCArray *eArr = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
        for (int ii = 0; ii < eArr->count(); ii++)
        {
            SGEquipCard *card = (SGEquipCard*)eArr->objectAtIndex(ii);
            if (card->getOfficerCardId())
            {
                array->addObject(card);
            }
        }
    }
    
    sortCards(array, sType, 1);
    if (array)
    {
        for (int ii = 0; ii < array->count(); ii++)
        {
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)array->objectAtIndex(ii);
            if (card->getSsid() == ssid)
            {
                return ii;
            }
        }
    }
    return -1;
}

void SGSortBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        if (type == 2 )
        {
            mainLayer->setIsCanTouch(false);
        }
        else
        {
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
        }
    }
    
    SGBaseBox::boxClose();
}

