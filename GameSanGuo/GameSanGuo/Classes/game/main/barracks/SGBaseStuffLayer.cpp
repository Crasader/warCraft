//
//  SGBaseStuffLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#include "SGBaseStuffLayer.h"
#include "CCSpriterX.h"
#include "ResourceManager.h"
#include "SGTouchLayer.h"
#include "SGStringConfig.h"

SGBaseStuffLayer::SGBaseStuffLayer()
:frameSize(CCRectMake(0, 0, 768, 200))
,atk_add(NULL)
,def_add(NULL)
,round_add(NULL)
,speed_add(NULL)
,mor_add(NULL)
,m_break(false)
,starFrame(NULL)
, title_bg(NULL)
, labelExp(NULL)
, labelTitle(NULL)
, m_fightStrengthNum(NULL)
, m_fightStrengthLab(NULL)
, titleBgStick(NULL)
{
    memset(starList,0,HELLO_MAX_STAR*sizeof(CCSprite *));
    memset(starList2,0,HELLO_MAX_STAR*sizeof(CCSprite *));
}
SGBaseStuffLayer::~SGBaseStuffLayer()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_stuffLayer);

    CCLOG("~SGBaseStuffLayer");
}
bool SGBaseStuffLayer::init(const char *fileName_, LayerTag tag_, bool isSpriteFrame)
{
	ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);

    
    if (!SGBaseLayer::init(fileName_, tag_, isSpriteFrame))
    {
        return false;
    }
    return true;
}
void SGBaseStuffLayer::addLabel(stuffType type, cocos2d::CCPoint point)
{
    switch (type) {
        case stuff_lvl:
        case stuff_equipLevel:
        {
            SGCCLabelTTF *lvl = SGCCLabelTTF::create("Lv: ", FONT_BOXINFO, 28 , ccWHITE);
            this->addChild(lvl,9);
            lvl->setPosition(point);
            lvl->setTag((int)type);
        }
            break;
        case stuff_exp:
        {
            SGCCLabelTTF *exp = SGCCLabelTTF::create(str_jingyan, FONT_BOXINFO, 28 ,COLOR_YELLOW);
            this->addChild(exp,9);
            exp->setPosition(point);
            exp->setTag((int)type);
        }
            break;
        case stuff_atk:
        {
            SGCCLabelTTF *atk = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(atk,9);
            atk->setPosition(point);
            atk->setTag((int)type);
            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            this->addChild(bg,8);
            bg->setPosition(ccpAdd(atk->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_def:
        {
            SGCCLabelTTF *def = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(def,9);
            def->setPosition(point);
            def->setTag((int)type);
            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            this->addChild(bg,8);
            bg->setPosition(ccpAdd(def->getPosition(), ccp(130, 0)));

        }
            break;
        case stuff_round:
        {
            SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(round,9);
            round->setPosition(point);
            round->setTag((int)type);
            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            this->addChild(bg,8);
            bg->setPosition(ccpAdd(round->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_speed:
        {
            SGCCLabelTTF *speed = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(speed,9);
            speed->setPosition(point);
            speed->setTag((int)type);
            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            this->addChild(bg,8);
            bg->setPosition(ccpAdd(speed->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_mor:
        {
            SGCCLabelTTF *mor = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(mor,9);
            mor->setPosition(point);
            mor->setTag((int)type);
            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            this->addChild(bg,8);
            bg->setPosition(ccpAdd(mor->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_gov:
        {
            SGCCLabelTTF *gov = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 28 , COLOR_YELLOW);
            this->addChild(gov,9);
            gov->setPosition(point);
            gov->setTag((int)type);
            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            this->addChild(bg,8);
            bg->setPosition(ccpAdd(gov->getPosition(), ccp(144, 0)));

        }
            break;
        default:
            break;
    }
}
void SGBaseStuffLayer::addValueLabel(stuffType type, int value1,float move, int value2, int anchorType,int qianvalue,int strengtype,bool ismax)
{
    CCSprite *sp = (CCSprite *)this->getChildByTag(type);
    switch (type) {
        case stuff_lvl: //等级
        {
            CCString *lvel = NULL;// CCString::create("");
            if (value1 == value2) {
                lvel = CCString::create("MAX");
//                lvel->setString("MAX");
            }else
            {
                if(strengtype==1)
                {
                    lvel = CCString::createWithFormat(str_Format_ji,qianvalue);
                }
                
                else
                {
                    lvel = CCString::createWithFormat(str_Format_ji,value1);
                }
//                lvel->setString(CCString::createWithFormat("%d级",value1)->getCString());
            }
            
            SGCCLabelTTF *abc = SGCCLabelTTF::create(lvel->getCString(),FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            abc->setTag(999);
            abc->setAnchorPoint(ccp(0, 0.5));
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move+4, 0)));
            SGCCLabelTTF *max = SGCCLabelTTF::create(CCString::createWithFormat("（%d/%d）",value1, value2)->getCString(), FONT_BOXINFO, 28);
            max->setAnchorPoint(ccp(0, 0.5));
            this->addChild(max,9);
            max->setPosition(ccpAdd(abc->getPosition(), ccp( max->getContentSize().width/2, 0)));
        }
            break;
        case stuff_equipLevel://装备等级
        {
            CCString *lvel = NULL;// CCString::create("");
            if (value1 == value2) {
                lvel = CCString::create("MAX");
                //                lvel->setString("MAX");
            }else
            {
                if(strengtype==1)
                {
                    lvel = CCString::createWithFormat(str_Format_lvl,qianvalue, value2);
                }
                
                else
                {
                    lvel = CCString::createWithFormat(str_Format_lvl,value1, value2);
                }
            }
            
            SGCCLabelTTF *abc = SGCCLabelTTF::create(lvel->getCString(),FONT_BOXINFO, 28 , ccWHITE);
            abc->setTag(999);
            abc->setAnchorPoint(ccp(0, 0.5));
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move+4, 2)));
        }
            break;
        case stuff_exp: //经验
        {
            CCString *def = CCString::createWithFormat("%d/%d",value1,value2);
            if(ismax)
              {
              def = CCString::createWithFormat(" ");
              
              }
//            if (value2 == -1) {
//                def = CCString::create("MAX");
////                def->setString("MAX");
//            }
            
            SGCCLabelTTF *abc = SGCCLabelTTF::create(def->getCString(),FONT_BOXINFO,28);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(150, 0)));
            
        }
            break;
        case stuff_atk: //攻击
        {
            CCString *atk = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(atk->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1000);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                atk_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                atk_add->setAnchorPoint(ccp(0, 0.5));
                //modify by:zyc. merge into create.
                //atk_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(atk_add,9);
                atk_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_def: //防御
        {
            CCString *def = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(def->getCString(), FONT_BOXINFO, 28 ,ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1001);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                def_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //def_add->setColor(ccc3(0x0c, 0xff, 0x00));
                def_add->setAnchorPoint(ccp(0, 0.5));
                this->addChild(def_add,9);
                def_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_mor: //血量
        {
            CCString *shiqi = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(shiqi->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1004);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                mor_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //mor_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(mor_add,9);
                mor_add->setAnchorPoint(ccp(0, 0.5));
                mor_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_speed: //速度
        {
            CCString *speed = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(speed->getCString(), FONT_BOXINFO, 28 ,ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1003);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                speed_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //speed_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(speed_add,9);
                speed_add->setAnchorPoint(ccp(0, 0.5));
                speed_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));                
            }
        }
            break;
        case stuff_gov: //统御
        {
            CCString *shiqi = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(shiqi->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1005);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                
            }
        }
            break;
        case stuff_round: //蓄力
        {
            CCString *xuli = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(xuli->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1002);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                round_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO,28 , ccc3(0x0c, 0xff , 0x00));
                //modify by:zyc. merge into create.
                //round_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(round_add,9);
                round_add->setAnchorPoint(ccp(0, 0.5));
                round_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_area: //占格
        {
            CCString *zhange = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(zhange->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1002);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                round_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO,28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //round_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(round_add,9);
                round_add->setAnchorPoint(ccp(0, 0.5));
                round_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
     
            
        default:
            break;
    }
}

void SGBaseStuffLayer::initMsg()
{
    title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);

    titleBgStick = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titleBgStick->setAnchorPoint(ccp(0.5, 0));
    titleBgStick->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titleBgStick,2);
    titleBgStick->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,10);
}

//星级图片
void SGBaseStuffLayer::setstar(int star,int max)
{
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    //星级母体框架
    if (!starFrame)
    {
        float sfWidth = 270;
        float sfHeight = 44;
        starFrame = SGTouchLayer::create(sfWidth, sfHeight, -100 /*, ccc4(0,255,0,255)*/);
        starFrame->ignoreAnchorPointForPosition(false);
        starFrame->setAnchorPoint(ccp(0, 1));
        this->addChild(starFrame, 1000);
        starFrame->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(12, -title_bg->getContentSize().height -20)));
        //
        //放空的星星
        CCSprite *maxstar = NULL;
        for(int i=0; i < HELLO_MAX_STAR; i++)
        {
            maxstar = CCSprite::createWithSpriteFrameName("blackstar.png");
            maxstar->setAnchorPoint(ccp(0.5, 0));
            maxstar->setPosition(ccp(maxstar->getContentSize().width/2 + 14 + maxstar->getContentSize().width * i, +10));
            //            currstar->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(currstar->getContentSize().width/2 + 10 + currstar->getContentSize().width * i, -currstar->getContentSize().height/2 - title_bg->getContentSize().height - 5)));
            starFrame->addChild(maxstar,1000);
                        starList2[i] = maxstar;
        }

        
        
        //放满留6个星星，控制显隐
        ResourceManager::sharedInstance()->bindTexture("animationFile/staranimation.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
		ResourceManager::sharedInstance()->bindTexture("animationFile/bsxx.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
		ResourceManager::sharedInstance()->bindTexture("animationFile/btnhuabiananim.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
		ResourceManager::sharedInstance()->bindTexture("animationFile/wjzs.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
        CCSprite *currstar = NULL;
        for(int i=0; i < HELLO_MAX_STAR; i++)
        {
            currstar = CCSprite::createWithSpriteFrameName("star.png");
            currstar->setAnchorPoint(ccp(0.5, 0));
//            currstar->setPosition(ccp(currstar->getContentSize().width/2 + 5 + currstar->getContentSize().width * i, +5));
            currstar->setPosition(ccpAdd(starList2[i]->getPosition(), ccp(-1,-5)));
            starFrame->addChild(currstar,1000);
   

            CCSpriterX* fermEffect = CCSpriterX::create("animationFile/staranimation.scml", true, true);
            fermEffect->setanimaID(0);
            fermEffect->setisloop(true);
            currstar->addChild(fermEffect);
            fermEffect->setPosition(ccp(currstar->getContentSize().width*.5, currstar->getContentSize().height*.5 +1));
            fermEffect->play();
            starList[i] = currstar;
        }
        
       
    }
    
    
    //根据传进来的数字控制显隐
    bool visa = false;
    for (int k = 0; k < HELLO_MAX_STAR; k++)
    {
        if (k < star)
            visa = true;
        else
            visa = false;
        starList[k]->setVisible(visa);
    }
    //根据传进来的数字控制空星星显隐
    bool visa2 = false;

    for (int k = 0; k < HELLO_MAX_STAR; k++)
    {
        if (k < max && k>=star)
            visa2 = true;
        else
            visa2 = false;
        starList2[k]->setVisible(visa2);
    }
  
    
    //获得最后星星位置
//    int index = star - 1;
    int index = star;
    if (starList[index]) {
//        pos = ccpAdd(starList[index]->getPosition(), ccp(starList[index]->getContentSize().width, 0));
        pos = starList[index]->getPosition();
    }
}
//    武将名字
void SGBaseStuffLayer::setTitle(const char *title_ , bool isGenerInfo, int PinZhi)
{
    //非武将详情页
    if(!isGenerInfo)
        labelTitle = SGCCLabelTTF::create(title_, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);//42
    else
    {
        //绿 蓝 紫 橙 对应转生次数 3 4 5 6
        ccColor3B color[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
        ccColor3B titleColor = ccc3(0,0,0);
        if(PinZhi <3 || PinZhi>6)
            titleColor = ccGREEN;
        else
            titleColor = color[PinZhi - 3];
        labelTitle = SGCCLabelTTF::create(title_, FONT_XINGKAI, 36 , titleColor);//42
    }
    this->addChild(labelTitle);
    labelTitle->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -title_bg->getContentSize().height*.5)));
}

//武将战斗力
void SGBaseStuffLayer::setOfficerFightStrength(int fightStrength)
{
    if(!m_fightStrengthLab)
    {
        m_fightStrengthLab = SGCCLabelTTF::create(str_GeneralInfoLayer_str10, FONT_BOXINFO, 28 ,COLOR_YELLOW);
        m_fightStrengthLab->setPosition(ccp(70 , starFrame->getPositionY() - starFrame->getContentSize().height - m_fightStrengthLab->getContentSize().height * 0.5) );
        this->addChild(m_fightStrengthLab , 1000);
        
        m_fightStrengthNum = SGCCLabelTTF::create(CCString::createWithFormat("%d" , fightStrength)->getCString() , FONT_BOXINFO , 24);
        m_fightStrengthNum->setPosition(ccp( m_fightStrengthLab->getPosition().x + m_fightStrengthLab->getContentSize().width * 0.5 + m_fightStrengthNum->getContentSize().width *0.5 + 8 , m_fightStrengthLab->getPosition().y ));
        this->addChild(m_fightStrengthNum ,1000);
    }
    else
        m_fightStrengthNum->setString(CCString::createWithFormat("%d",fightStrength)->getCString() );
}

//    数据库ID
void SGBaseStuffLayer::setSsid(int iSsid)
{
    
}
//    背景ID
void SGBaseStuffLayer::setItemID(const char *cItemId,bool isFrame)
{
    CCSize s= CCDirector::sharedDirector()->getWinSize();
  //  CCSprite *itemID = CCSprite::create(cItemId,frameSize);
    CCSprite *itemID;
    if (isFrame) {
          itemID=CCSprite::createWithSpriteFrameName(cItemId);
    }
    else
    {
        itemID=CCSprite::create(cItemId);
    }
   
    this->addChild(itemID,-1);

    if (s.height == 960)
    {
        float rate = (float)768/s.width;
        itemID->setScale(s.width/768);
        itemID->setScaleY(0.75);
        itemID->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - frameSize.size.height/2/rate + 20)));

    }
    else if (s.height == 1136)
    {
        itemID->setScaleX(s.width/768);
        itemID->setScaleY(0.85);
        itemID->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                                   ccp(0, -title_bg->getContentSize().height - itemID->getContentSize().height * 0.8 /2 - 16)));

    }else
    {
        itemID->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height -10- frameSize.size.height*.45)));
    }
}
void SGBaseStuffLayer::setCardItem(const char *ItemId)
{
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    CCSprite *itemID = CCSprite::create(ItemId);
    itemID->setAnchorPoint(ccp(0.5, 0));
    this->addChild(itemID,-1000);
    
    float rate = (float)768/s.width;
    if (s.height == 1136) {
        itemID->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - frameSize.size.height*.85)));
        
    }else
    {
        itemID->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - frameSize.size.height/rate*.85)));
    }

}
void SGBaseStuffLayer::updateLabel(stuffType type, int value)
{
   
    switch (type) {
        case stuff_atk:
        {
            if (atk_add) {               
        
            if (value == 0)
            {
                this->removeChild(atk_add, true);
            }else
            {
            CCString *add = CCString::createWithFormat(" +%d",value);
            atk_add->setString(add->getCString());
            }
            }
            
        }
        break;
        case stuff_def:
        {
            if (def_add) {                
            
            if (value == 0 )
            {
                this->removeChild(def_add, true);
            }
            else
            {
            CCString *add = CCString::createWithFormat(" +%d",value);
            def_add->setString(add->getCString());
            }
        }
    
        }
        break;
        case stuff_round:
        {
            if (round_add) {                
        
            if (value == 0 && round_add)
            {
                this->removeChild(round_add, true);
            }else
            {
            CCString *add = CCString::createWithFormat(" +%d",value);
            round_add->setString(add->getCString());
            }
            }
        }
            break;
        case stuff_speed:
        {
            if (speed_add) {                
    
            if (value == 0 && speed_add)
            {
                this->removeChild(speed_add, true);
            }
            else
            {
            CCString *add = CCString::createWithFormat(" +%d",value);
            speed_add->setString(add->getCString());
            }
            }
        }
            break;
        case stuff_mor:
        {
            if (mor_add) {                
        
            if (value == 0 && mor_add)
            {
                this->removeChild(mor_add, true);
            }
            else
            {
            CCString *add = CCString::createWithFormat(" +%d",value);
            mor_add->setString(add->getCString());
            }
                
            }
        }
            break;
            
        default:
            break;
    }
}
void SGBaseStuffLayer::setCardType(int type)
{
    SGCCLabelTTF *title = SGCCLabelTTF::create("", FONT_PANGWA, 30);
    title->setAnchorPoint(ccp(1, 0.5));
    switch (type) {
        case 1:
            title->setString(str_FateInfoItem_str16);
            break;
        case 2:
            title->setString(str_FateInfoItem_str17);
            break;
        case 3:
            title->setString(str_FateInfoItem_str18);
            break;
        case 4:
            title->setString(str_FateInfoItem_str19);
            break;
        case 5:
            title->setString(str_FateInfoItem_str20);
            break;
        case 6:
            title->setString(str_FateInfoItem_str21);
            break;
        case 7:
            title->setString(str_BassStuffLayer_str1);
            break;
        case 8:
            title->setString(str_BassStuffLayer_str2);
            break;
        case 9:
            title->setString(str_wuqi);
            break;
        case 10:
            title->setString(str_kaijia);
            break;
        case 11:
            title->setString(str_jiezhi);
            break;
        case 12:
            title->setString(str_zuoji);
            break;
        case 99:
            title->setString(str_soldiers);
            break;
        case 100:
            title->setString(str_suipian);
            break;
        default:
            break;
    }
    this->addChild(title,99);
    title->setAnchorPoint(ccp(1, 0.5));
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(-skewing(10), -125)));
}
//void SGBaseStuffLayer::labAnimation(stuffType type, cocos2d::CCDictionary *data)
//{
//    SGCCLabelTTF *label= NULL;
//    
//    switch (type) {
//        case stuff_atk:
//        {
//            label = (SGCCLabelTTF *)this->getChildByTag(1000);
//            
//        }
//            break;
//        case stuff_def:
//        {
//            label = (SGCCLabelTTF *)this->getChildByTag(1001);
//        }
//            break;
//        case stuff_round:
//        {
//            label = (SGCCLabelTTF *)this->getChildByTag(1002);
//        }
//            break;
//        case stuff_speed:
//        {
//            label = (SGCCLabelTTF *)this->getChildByTag(1003);
//        }
//            break;
//        case stuff_mor:
//        {
//            label = (SGCCLabelTTF *)this->getChildByTag(1004);
//        }
//            break;
//            
//        default:
//            break;
//    }
//    if (label)
//    {
//        this->lableAction(label,data);
//    }
//}
void SGBaseStuffLayer::labAnimation(stuffType type, int change)
{
    SGCCLabelTTF *label= NULL;
    int tag = 0;
    switch (type) {
        case stuff_lvl:
        case stuff_equipLevel:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(999);
        }
            break;
        case stuff_atk:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(1000);
        }
            break;
        case stuff_def:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(1001);
        }
            break;
        case stuff_round:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(1002);
        }
            break;
        case stuff_speed:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(1003);
        }
            break;
        case stuff_mor:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(1004);
        }
            break;
            
        case stuff_gov:
        {
            label = (SGCCLabelTTF *)this->getChildByTag(1005);
        }
            break;
            
        default:
            break;
    }
    if (label)
    {

//        CCScaleTo *scale1 = CCScaleTo::create(0.05, 1.35);
//        CCScaleTo *scale2 = CCScaleTo::create(0.05, 1.5);
//        CCScaleTo *scale3 = CCScaleTo::create(0.59, 1);
//        CCAction *actionFunc1 =  CCSequence::create(scale1,CCCallFuncND::create(this, callfuncND_selector(SGBaseStuffLayer::midCallBack),(void *)change),scale2,CCDelayTime::create(0.3),scale3,NULL);
//        label->runAction(actionFunc1);
        label->runactionWithScale(1.3, 1.35);
		label->runactionWithScale(1, 1);
		label->setString(CCString::createWithFormat("%d",change)->getCString());
		label->runactionWithScale(1, 1.35);
    }
    
    
}

void SGBaseStuffLayer::lableAction(cocos2d::CCNode *node, int change)
{
//    CCScaleTo *scale1 = CCScaleTo::create(0.05, 1.35);
//    CCScaleTo *scale2 = CCScaleTo::create(0.05, 1.5);
//    CCScaleTo *scale3 = CCScaleTo::create(0.59, 1);
//    CCAction *actionFunc1 =  CCSequence::create(scale1,CCCallFuncND::create(this, callfuncND_selector(SGBaseStuffLayer::midCallBack),(void *)change),scale2,CCDelayTime::create(0.3),scale3,NULL);
//    node->runAction(actionFunc1);
}
void SGBaseStuffLayer::midCallBack(cocos2d::CCNode *node, int change)
{
    SGCCLabelTTF *a = (SGCCLabelTTF*)node;
    a->setString(CCString::createWithFormat("%d",change)->getCString());
}

void SGBaseStuffLayer::backHandler()
{
}

void SGBaseStuffLayer::callBack(cocos2d::CCNode *node)
{
    this->removeChild(node, true);
}
void SGBaseStuffLayer::progressAction(cocos2d::CCProgressTimer *m_progress, int times, SGBaseMilitaryCard *card, CCCallFuncN *call)
{
    _call = call;
    leftTimes = times;
    float max = 100;
    float per = m_progress->getPercentage();
    float time = 1;
    if (per != 100) {
        time = 0.01*(100 - per);
    }
    if (leftTimes == 0) {
        max = 100*((float)card->getCurrExp()/card->getMaxExp());
        m_progress->runAction(CCProgressTo::create(time, max));
    }
    else
    {
    CCAction *action =  CCSequence::create(CCProgressTo::create(time, max),CCCallFuncND::create(this, callfuncND_selector(SGBaseStuffLayer::progressCallback),card),call,NULL);
    m_progress->runAction(action);
    }
}
void SGBaseStuffLayer::progressCallback(cocos2d::CCNode *node,SGBaseMilitaryCard *card)
{
//    ResourceManager::sharedInstance()->bindTexture("barrack/barrack3.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    if (m_break) {
        return;
    }
    leftTimes--;
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    CCSprite *waifaguang = CCSprite::createWithSpriteFrameName("waifaguang.png");
    this->addChild(waifaguang,9);
    CCFadeOut *fade = CCFadeOut::create(0.2);
    
    waifaguang->runAction(CCSequence::create(fade,CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGBaseStuffLayer::callBack)),NULL));
    waifaguang->setPosition(node->getPosition());
    progressAction((CCProgressTimer*)node, leftTimes, card, _call);
}
void SGBaseStuffLayer::setlabelValue(stuffType type, int value,int value2)
{
SGCCLabelTTF *label= NULL;

switch (type) {
    case stuff_lvl:
    case  stuff_equipLevel:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(999);
        label->setScale(1);
        label->stopAllActions();
        label->setString(CCString::createWithFormat(str_Format_ji,value)->getCString());
        return;
    }
        break;
    case stuff_atk:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(1000);
    }
        break;
    case stuff_def:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(1001);
    }
        break;
    case stuff_round:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(1002);
    }
        break;
    case stuff_speed:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(1003);
    }
        break;
    case stuff_mor:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(1004);
    }
        break;
        
    case stuff_gov:
    {
        label = (SGCCLabelTTF *)this->getChildByTag(1005);
    }
        break;
        
    default:
        break;
}
    if (label)
  {
    label->setScale(1);
    label->stopAllActions();
    if (value2) {
        label->setString(CCString::createWithFormat("%d/%d",value,value2)->getCString());
    }else
    {
    label->setString(CCString::createWithFormat("%d",value)->getCString());
    }
  }
}

void SGBaseStuffLayer::setadvancestar()
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/staranimation.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/bsxx.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/btnhuabiananim.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/wjzs.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);

    CCSprite* currstar = CCSprite::createWithSpriteFrameName("star.png");
    currstar->setAnchorPoint(ccp(0.5, 0));
    currstar->setPosition(pos);
        
    CCSpriterX* fermEffect = CCSpriterX::create("animationFile/staranimation.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    currstar->addChild(fermEffect);
    fermEffect->setPosition(ccp(currstar->getContentSize().width*.5, currstar->getContentSize().height*.5 +1));
    currstar->setScale(5);
    starFrame->addChild(currstar,1000);
    
    currstar->runAction(CCScaleTo::create(.5, 1));
    fermEffect->play();
//           fermEffect->runAction(CCSequence::create(CCDelayTime::create(4.5),CCCallFuncN::create(this,callfuncN_selector(SGBaseStuffLayer::playact)),NULL));
}
void SGBaseStuffLayer::playact(CCNode *node)
{
    ((CCSpriterX*)node)->play();
}