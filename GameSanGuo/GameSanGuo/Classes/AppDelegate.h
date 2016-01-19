//
//  GameSanGuoAppDelegate.h
//  GameSanGuo
//
//  Created by wenlong w on 12-11-19.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"
#include <vector>
#include <string>


/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();
    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function is called when the application enters the background
    @param  the pointer of the application instance
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function is called when the application enters the foreground
    @param  the pointer of the application instance
    */
    virtual void applicationWillEnterForeground();
    
    /**
    @brief add other resource search paths. the default path is already set in the function.
    @param the vector of paths.
     */
    void handleResourceSearchPaths(const std::vector<std::string>& vec);
};

#endif // _APP_DELEGATE_H_

