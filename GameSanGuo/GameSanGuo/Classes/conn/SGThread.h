//
//  SGThread.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-14.
//
//

#ifndef __GameSanGuo__SGThread__
#define __GameSanGuo__SGThread__

#include <stdio.h>
#include "pthread.h"
//#include <vector.h>

#define MUTEX_NUM (5)

//线程运行接口
class SGRunnable
{
public:
    //启动该线程时将导致在独立执行的线程中调用对象的run方法
    virtual void run(void) = 0;
};

class SGThread
{
protected:
    //线程id
    pthread_t _thread;
    //运行线程对象
    SGRunnable &_obj;
    //当前线程锁ID
    int _mutexId;
    
public:
    //创建线程对象 传入线程运行对象
    SGThread(SGRunnable &obj);
    //释放线程对象
    ~SGThread();
    //使线程开始执行，调用run
	int start(void);
	//终止该线程
	int exit(void);
    //让线程休眠几毫秒
    void sleepMillisecond(unsigned int Millisecond);
};
#endif /* defined(__GameSanGuo__SGThread__) */
