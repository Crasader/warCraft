//
//  SGPacketHeader.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-14.
//
//

#include "SGPacketHeader.h"
#include "PlatformAdp.h"

#if (PLATFORM == ANDROID)
#include <netinet/in.h>
#endif

#define DELETE_MEM(a) ({if(a){delete a; a=NULL;}})
#define HADER_SIZE (6)


stPackData::~stPackData()
{
//        unsigned char* realBuffer = data - USHORT_SIZE;
//        CC_SAFE_DELETE_ARRAY(realBuffer);
    CC_SAFE_DELETE_ARRAY(data);
}

SGPacketHeader::SGPacketHeader()
:msgId(0),
zoneId(0),
packData(NULL)
{
    
}
SGPacketHeader::~SGPacketHeader()
{
//    printf("~SGPacketHeader\n");
    DELETE_MEM(packData);
}
void SGPacketHeader::writePacketBytes(short _msgId, google::protobuf::MessageLite *_msg)
{
    msgId = _msgId;
    zoneId = 0;
    
    DELETE_MEM(packData);
    packData = new stPackData();
    
    int msgSize = _msg->ByteSize();
    
    packData->length = msgSize + HADER_SIZE;
    packData->data = new unsigned char[packData->length];
    
    memset(packData->data, 0, packData->length);
    
    short len = htons(packData->length-USHORT_SIZE);
    memcpy(packData->data, &len, USHORT_SIZE);
    
    short zid = htons(zoneId);
    memcpy(packData->data + USHORT_SIZE, &zid, USHORT_SIZE);
    
    short sid = htons(msgId);
    memcpy(packData->data + USHORT_SIZE + USHORT_SIZE, &sid, USHORT_SIZE);
    
    //传入proto数据流
    google::protobuf::io::ArrayOutputStream * zo_output = new google::protobuf::io::ArrayOutputStream(packData->data + HADER_SIZE, msgSize);
    google::protobuf::io::CodedOutputStream * coded_output = new google::protobuf::io::CodedOutputStream(zo_output);
    _msg->SerializeWithCachedSizes(coded_output);
	//    printf("datalen%ld", strlen((char*)packData->data));
    delete _msg;
    delete zo_output;
    delete coded_output;
}

void SGPacketHeader::readPacketBytes(const char* _pack, const unsigned short _length)
{
    //_pack=old response+2。
    //length2 + msgId2 + dataX
    
    memset(&msgId, 0, sizeof(msgId));
    memcpy(&msgId, _pack, USHORT_SIZE);
    msgId = htons(msgId);
    
    packData = new stPackData();

    packData->length = _length - USHORT_SIZE; //strip the msgId size
	if (packData->length < 0)
    {
        packData->length = 4;
    }
    
//    packData->data = (unsigned char*) (_pack + USHORT_SIZE);
    
    packData->data = new unsigned char[packData->length];
    memcpy(packData->data, _pack+USHORT_SIZE, packData->length);
    
    return;
    
    //obsolete code 0.22
//    memset(&msgId, 0, sizeof(msgId));
//    memcpy(&msgId, _pack+USHORT_SIZE, USHORT_SIZE);
//    msgId = htons(msgId);
//    
//    
//    packData = new stPackData();
//	
//    memset(&packData->length, 0, sizeof(packData->length));
//    memcpy(&packData->length, _pack, USHORT_SIZE);
//    packData->length = htons(packData->length) - USHORT_SIZE;
//	if (packData->length < 0) {
//        packData->length = 4;
//    }
//    packData->data = new unsigned char[packData->length];
//    memcpy(packData->data, _pack + USHORT_SIZE + USHORT_SIZE, packData->length);
}
