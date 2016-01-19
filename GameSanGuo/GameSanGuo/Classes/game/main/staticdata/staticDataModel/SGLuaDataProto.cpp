//
//  SGLuaDataProto.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#include "SGLuaDataProto.h"
static SGLuaDataProto *lua=NULL;
SGLuaDataProto::SGLuaDataProto():toAdd(0) , msgId(0) , isget(0)
{
  requestData = NULL;
  responseData = NULL;
}

SGLuaDataProto::~SGLuaDataProto()
{
   
    delete [] requestData;
    if (responseData) {
         delete [] responseData;
    }
}

char *SGLuaDataProto::getRequestData()
{
    return (char *)requestData;
}
int SGLuaDataProto::getRequestLength()
{
    return toAdd;
}

SGLuaDataProto* SGLuaDataProto::shareStatic()
{
    if(NULL == lua)
    {
        lua = new SGLuaDataProto();
    }
    return lua;
}
void SGLuaDataProto::writeShort(int s)
{

    unsigned short type = 18;
    unsigned short m = s;
    memcpy(requestData+toAdd, &type, 1);
    toAdd +=1;
    memcpy(requestData+toAdd, &m, 1);
    toAdd +=1;
}
void SGLuaDataProto::writeString(const char *str)
{
    int sl = strlen(str);
    this->writeShort(sl);
    memcpy(requestData+toAdd, str, sl);
    toAdd+= sl;
}

void SGLuaDataProto::writeInt(int a)
{
    unsigned short type = 8;
//    unsigned short m = s;
    memcpy(requestData+toAdd, &type, 1);
    toAdd +=1;
    unsigned short sl = 1;
    memcpy(requestData+toAdd, &a, sl);
    toAdd+= sl;
}
void SGLuaDataProto::writeFloat(float a)
{
    unsigned short sl = sizeof(float);
    memcpy(requestData+toAdd, &a, sl);
    toAdd+= sl;
}
char SGLuaDataProto::readChar()
{
    char charnum = 0;
    memset(&charnum, 0, 1);
    memcpy(&charnum, responseData + toAdd, 1);
    toAdd+=1;
    return charnum;
}
const char * SGLuaDataProto::readString()
{
    int strlen = this->readShort();
    char *str = new char[strlen+1];
    memset(str, 0, strlen);
    memcpy(str, responseData+toAdd, strlen);
    str[strlen] = '\0';
    toAdd+= strlen;
    return str;
}
int SGLuaDataProto::readShort()
{
    short shortnum = 0;
    unsigned short num =0;
    memcpy(&num, responseData + toAdd, 1);
    toAdd+=1;
    if (num==18) {
        memcpy(&shortnum, responseData + toAdd, 1);
//        shortnum = htons(shortnum);
        toAdd += 1;
 
    }
    return shortnum;
}

int SGLuaDataProto::readInt()
{

    unsigned short num =0;
    memcpy(&num, responseData + toAdd, 1);
    toAdd+=1;

    int intnum = 0;
    memcpy(&intnum, responseData + toAdd, 1);
//    intnum = htons(intnum);
    toAdd += 1;
    return intnum;
}

float SGLuaDataProto::readFloat()
{
    float floatnum = 0;
    memcpy(&floatnum, responseData + toAdd, sizeof(float));
    //    intnum = htons(intnum);
    toAdd += sizeof(float);
    return floatnum;
}


void SGLuaDataProto::setresponseDate(char *data)
{
    responseData = data;
    toAdd=0;
}

void SGLuaDataProto::setrequstDate()
{
    requestData = new unsigned char[HTTPLENGHT];
    toAdd=0;
    responseData = NULL;
}

int SGLuaDataProto::isgetResponseData()
{
    if (responseData!=NULL) {
        return 1;
    }
    else
        return 0;
}
