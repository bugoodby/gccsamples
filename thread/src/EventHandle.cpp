#include "EventHandle.h"

EventHandle::EventHandle()
{
	printf("EventHandle::EventHandle\n");
	m_evQueMutex = PTHREAD_MUTEX_INITIALIZER;
	int ret = pthread_mutex_init(&m_evQueMutex, NULL);
	if ( ret != 0 ) {
		printf("[ERROR] pthread_mutex_lock failed (ret=%d)\n", ret);
	}
}

EventHandle::~EventHandle()
{
	printf("EventHandle::~EventHandle\n");
	std::list<struct EVENT_DATA*>::iterator itr = m_eventQueue.begin();
	std::list<struct EVENT_DATA*>::iterator itrEnd = m_eventQueue.end();
	for ( ; itr != itrEnd; itr++ ) {
		struct EVENT_DATA* p = *itr;
		delete p;
	}
	
	int ret = pthread_mutex_destroy(&m_evQueMutex);
	if ( ret != 0 ) {
		printf("[ERROR] pthread_mutex_lock failed (ret=%d)\n", ret);
	}
}

bool EventHandle::send( int event_id, void* param )
{
	pthread_mutex_lock(&m_evQueMutex);
	
	bool ret = false;
	struct EVENT_DATA *p = new struct EVENT_DATA;
	if ( p )
	{
		printf("<-- send event: %d\n", event_id);
		p->event_id = event_id;
		p->param = param;
		m_eventQueue.push_back(p);
		ret = true;
	}
	
	pthread_mutex_unlock(&m_evQueMutex);
	
	return ret;
}

bool EventHandle::receive( struct EVENT_DATA& data )
{
	pthread_mutex_lock(&m_evQueMutex);
	
	bool ret = false;
	memset(&data, 0, sizeof(data));
	
	if ( !m_eventQueue.empty() ) {
		EVENT_DATA *p = m_eventQueue.front();
		m_eventQueue.pop_front();
		data.event_id = p->event_id;
		data.param = p->param;
		delete p;
		ret = true;
	}
	
	pthread_mutex_unlock(&m_evQueMutex);
	return ret;
}

