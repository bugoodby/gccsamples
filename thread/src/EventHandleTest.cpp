#include "EventHandleTest.h"
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

EventHandleTest::EventHandleTest()
{
	m_mutex = PTHREAD_MUTEX_INITIALIZER;
	int ret = pthread_mutex_init(&m_mutex, NULL);
	if ( ret != 0 ) {
		printf("[ERROR] pthread_mutex_lock failed (ret=%d)\n", ret);
	}
}

EventHandleTest::~EventHandleTest()
{
	int ret = pthread_mutex_destroy(&m_mutex);
	if ( ret != 0 ) {
		printf("[ERROR] pthread_mutex_lock failed (ret=%d)\n", ret);
	}
}

// lock mutex
void EventHandleTest::lock()
{
	int ret = pthread_mutex_lock(&m_mutex);
	if ( ret != 0 ) {
		printf("[ERROR] pthread_mutex_lock failed (ret=%d)\n", ret);
	}
}

// unlock mutex
void EventHandleTest::unlock()
{
	int ret = pthread_mutex_unlock(&m_mutex);
	if ( ret != 0 ) {
		printf("[ERROR] pthread_mutex_unlock failed (ret=%d)\n", ret);
	}
}

void EventHandleTest::test_func()
{
	printf("%s start\n", __FUNCTION__);
	lock();
	
	sleep(1);
	
	unlock();
	printf("%s end\n", __FUNCTION__);
}

void* EventHandleTest::thread_main( void* param )
{
	EventHandleTest* pIns = reinterpret_cast<EventHandleTest*>(param);
	struct EVENT_DATA data;
	bool loop = true;
	
	while ( loop )
	{
		if ( !pIns->receive(data) ) {
			sleep(5);
			continue;
		}
		
		printf("--> receive event: %d\n", data.event_id);
		switch ( data.event_id )
		{
		case 10:
			printf("%s event:10\n", __FUNCTION__);
			pIns->test_func();
			break;
		case 11:
			// dead lock !!!
			printf("%s event:11\n", __FUNCTION__);
			pIns->lock();
			pIns->test_func();
			pIns->unlock();
			printf("%s event:11\n", __FUNCTION__);
			break;
		case 9999:
			loop = false;
			break;
		default:
			break;
		}
	}
	
	return NULL;
}

void EventHandleTest::start()
{
	pthread_t thread;
	pthread_create(&thread, NULL, this->thread_main, this);
	
	char input_str[1024];
	while (1) {
		printf("event_id: ");
		fgets(input_str, sizeof(input_str), stdin);
		uint32_t num = strtoul(input_str, NULL, 0);
		send(num, NULL);
		
		if ( num == 9999 ) break;
	}
	
	pthread_join(thread, NULL);
}
