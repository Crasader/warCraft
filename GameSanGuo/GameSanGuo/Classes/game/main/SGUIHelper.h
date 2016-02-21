#ifndef _SG_UIHELPER_H_
#define _SG_UIHELPER_H_

#define DesignWidth 1136
#define DesignHeight 640
#define SubFormHeigh 575  


#define TagTeamHeroStart  1000


#define TagHeroEquipStart 1400


//csb name
#define   CSB_mainlayout   mainLayout
#define   CSB_topLayout    topLayout

#define CSB_layout(name) static_cast<Layout*>(CSLoader::createNode(name));
#define CSB_node(name) CSLoader::createNode(name);





enum layoutZorder
{
    E_zorderNormal = 0,
    E_zorderHigh,
    E_zorderMask,
    E_zorderModel
};

//#define UI_adaptation(father, me, layoutSize)  auto  Vsize = Director::getInstance()->getVisibleSize();\
//        Size mySize;\
//        mySize.width = Vsize.width  * (layoutSize.width) / DesignWidth;\
//        mySize.height = Vsize.height * (layoutSize.height) / DesignHeight ;\
//        me->setContentSize(mySize);\
//        ui::Helper::doLayout(me);\
//        father->addChild(me);



//#define IS_PAD (Director::getInstance()->getVisibleSize().width / Director::getInstance()->getVisibleSize().height < 1.5)
//#define ADAPTION(me)      if (IS_PAD)\
//{\
//    me->setScale(0.83);\
//}


//#define CSB_NAME(svString, nameNosufix)    if(IS_PAD)\
//{\
//    svString = StringUtils::format("%sHD.csb",nameNosufix);\
//}\
//else\
//{\
//    svString = StringUtils::format("%s.csb",nameNosufix);\
//}


#define UIGet_Layout(itemName, father, me)      me = static_cast<Layout*>(father->getChildByName(itemName));
#define UIGet_Label(itemName, father, me)       me = static_cast<Label*>(father->getChildByName(itemName));
#define UIGet_Button(itemName, father, me)      me = static_cast<Button*>(father->getChildByName(itemName));
#define UIGet_ImageView(itemName, father, me)   me = static_cast<ImageView*>(father->getChildByName(itemName));
#define UIGet_LoadingBar(itemName, father, me)  me = static_cast<LoadingBar*>(father->getChildByName(itemName));
#define UIGet_Slider(itemName, father, me)      me = static_cast<Slider*>(father->getChildByName(itemName));   
#define UIGet_ListView(itemName, father, me)    me = static_cast<ListView*>(father->getChildByName(itemName)); 
#define UIGet_PageView(itemName, father, me)    me = static_cast<PageView*>(father->getChildByName(itemName)); 
#define UIGet_ScrollView(itemName, father, me)  me = static_cast<ScrollView*>(father->getChildByName(itemName)); 
#define UIGet_Armature(itemName, father, me)    me = static_cast<Armature*>(father->getChildByName(itemName)); 
#define UIGet_TextField(itemName, father, me)    me = static_cast<TextField*>(father->getChildByName(itemName)); 
#define UIGet_Text(itemName, father, me)    me = static_cast<Text*>(father->getChildByName(itemName)); 
#define UIGet_Node(itemName, father, me)    me = static_cast<Node*>(father->getChildByName(itemName)); 
#define UIGet_ParticleSystemQuad(itemName, father, me)    me = static_cast<ParticleSystemQuad*>(father->getChildByName(itemName)); 
#define UIGet_TextBMFont(itemName, father, me)    me = static_cast<TextBMFont*>(father->getChildByName(itemName)); 

//#define UIClick(itemName, callFunc) itemName->addClickEventListener(CC_CALLBACK_1(callFunc, this));
#define UIClick(itemName, callFunc) itemName->addTouchEventListener(this, toucheventselector(callFunc));
#define UIbtnImgOff(btnName)   btnName->getChildByName("Image_on")->setVisible(false);\
                        btnName->getChildByName("Image_off")->setVisible(true);
#define UIbtnImgOn(btnName)   btnName->getChildByName("Image_on")->setVisible(true);\
                    btnName->getChildByName("Image_off")->setVisible(false);


#define RT_Label(content, fontsize, color, label, father) label = SGCCLabelTTF::create(content, FONT_PANGWA, fontsize, color);\
father->addNode(label);

#define RT_SPRITE(name, sprite)  sprite = CCSprite::createWithSpriteFrameName(name);

#define RT_spine(jsonName, altasName, spine) spine = CCSkeletonAnimation::createWithFile(jsonName, altasName);


#endif