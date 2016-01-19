//
//  SGPacketHeader.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-14.
//
//

#ifndef __GameSanGuo__SGPacketHeader__
#define __GameSanGuo__SGPacketHeader__

#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "cocos2d.h"

//ushort长度
const static unsigned int USHORT_SIZE = sizeof(unsigned short);

//uint长度
const static unsigned int UINT_SIZE = sizeof(unsigned int);

class stPackData : cocos2d::CCObject
{
public:
    int length;
    unsigned char* data;
    stPackData() : length(0), data(NULL) {};
    ~stPackData();
};


class SGPacketHeader : public cocos2d::CCObject
{
private:
    unsigned short msgId;//消息的id
    unsigned short zoneId;//选区id
    
    stPackData* packData;    //数据
public:
    SGPacketHeader();
    ~SGPacketHeader();
public:
    void writePacketBytes(short _msgId, google::protobuf::MessageLite* _msg);
    void readPacketBytes(const char* _pack, const unsigned short _length);
    inline stPackData* getPackData() const { return packData; }
    inline int getMsgId() const { return msgId; }

};
#endif /* defined(__GameSanGuo__SGPacketHeader__) */
