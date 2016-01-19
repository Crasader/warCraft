//
//  SGThread.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-14.
//
//

#include "SGThread.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//运行线程
void* run(void* arg)
{
	if ( arg!=NULL ) {
		SGRunnable *obj = (SGRunnable*)arg;
		obj->run();
	}
    
	return NULL;
}

SGThread::SGThread(SGRunnable& obj)
:_obj(obj),
_mutexId(0),
_thread(0)
{
    memset(&_thread, 0, sizeof(_thread));
}
SGThread::~SGThread()
{
    
}
int SGThread::start(void)
{
    int errCode = 0;
    do
    {
        pthread_attr_t tAttr;
        errCode = pthread_attr_init(&tAttr);
        
        errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
        
        if ( 0 != errCode ) {
            pthread_attr_destroy(&tAttr);
            break;
        }
        
        errCode = pthread_create(&_thread, &tAttr, run, &_obj);
    }while (0);
    return errCode;
}

//终止该线程
int SGThread::exit(void)
{
	int err=0;
	if( 0 != (err = pthread_join(_thread, NULL)) )
    {
		printf("退出线程失败 ERROR:%d\n", err);
		return (-1);
	}
	return 0;
}
//让线程休眠几毫秒
void SGThread::sleepMillisecond(unsigned int Millisecond)
{
	usleep(Millisecond*1000);
}