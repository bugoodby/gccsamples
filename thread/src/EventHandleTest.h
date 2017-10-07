#ifndef __EVENTHANDLETEST_H__
#define __EVENTHANDLETEST_H__

#include "EventHandle.h"

class EventHandleTest : public EventHandle
{
protected:
	pthread_mutex_t m_mutex;

public:
	EventHandleTest();
	virtual ~EventHandleTest();
	
	static void* thread_main( void* param );
	
	void lock();
	void unlock();
	void test_func();
	void start();
};


#endif /* __EVENTHANDLETEST_H__ */
