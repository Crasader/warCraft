//
//  SGLuaDataProto.h
//  GameSanGuo
//
//  Created by 董博谦 on 14-1-8.
//
//

#ifndef __GameSanGuo__SGLuaDataProto__
#define __GameSanGuo__SGLuaDataProto__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#define HTTPLENGHT 200


class SGLuaDataProto : public CCObject
{

//    std::string string;
//    int toAdd;
    char msgId;
    unsigned char *requestData;
    char *responseData;
    int isget;
    
public:
    SGLuaDataProto();
    
    ~SGLuaDataProto();
    int toAdd;
     static SGLuaDataProto *shareStatic();
    char *getRequestData();
    int getRequestLength();
    void writeShort(int s);
    void writeString(const char *str);
    void writeInt(int a);
    void writeFloat(float a);
    char readChar();
    const char * readString();
    int readShort();
    int readInt();
    float readFloat();
    void setresponseDate(char *data);
    void setrequstDate();
    int isgetResponseData();

    
};

#endif /* defined(__GameSanGuo__SGLuaDataProto__) */
