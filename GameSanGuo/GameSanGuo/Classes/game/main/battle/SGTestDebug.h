//
//  SGTestDebug.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-11.
//
//

#ifndef __GameSanGuo__SGTestDebug__
#define __GameSanGuo__SGTestDebug__

#define LC_ESS      "\033"      // start fix
#define LC_ESC      "#FFFFFF"   // reset fix

#define LC_RED      "#EE0000"
#define LC_YELLOW   "#EEEE00"
#define LC_GREED    "#00FF00"
#define LC_BLUE     "#0000CD"
#define LC_BLACK    "#000000"
#define LC_GRAY     "#666666"


void LogInfo(const char * color, const char * pszFormat, ...);

void show(); // test color log

#endif /* defined(__GameSanGuo__SGTestDebug__) */
