#include "CCNative.h"
#include "cocoa/CCString.h"

using namespace std;
USING_NS_CC;


CCString *chartoCCc (const char* fntName);



 void cocos2d::extension::CCNative::createAlert(const char* title,
	const char* message,
	const char* cancelButtonTitle)
{
//	HelloWorld *hel= new HelloWorld();
//	hel->inithello();
//	hel->createAlertView(title,message,cancelButtonTitle);
//
//	if(title)
//		CCString *title_ = chartoCCc(title);
//	else
//		CCString *title_ = chartoCCc("");
//
//	if(message)
//		CCString *message_ = chartoCCc(message);
//	else
//		CCString *message_ = chartoCCc("");
//
//
//	if(cancelButtonTitle)
//		CCString *cancelButtonTitle_ = chartoCCc(cancelButtonTitle);
//	else
//		CCString *cancelButtonTitle_ = NULL;
   
}

 int cocos2d::extension::CCNative::addAlertButton(const char* buttonTitle)
{
//	if(buttonTitle)
//		CCString *buttonTitle_ = chartoCCc(buttonTitle);
//	else
//		CCString *buttonTitle_ = chartoCCc("Button");
//	HelloWorld *hel= new HelloWorld();
//	hel->inithello();
//	return hel->addButton(buttonTitle);
    return 0;
}

 void cocos2d::extension::CCNative::showAlert(CCAlertViewDelegate* delegate )
{
//	HelloWorld *hel= new HelloWorld();
//	hel->inithello();
//	hel->showAlertViewWithDelegate();
}

  
 CCString *chartoCCc (const char* fntName)
{
	string st(fntName);
	string sout;
	CCString *strin=CCString::createWithFormat("%s",sout.c_str());
	return strin;
}
