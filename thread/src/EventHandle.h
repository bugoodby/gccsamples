#ifndef __EVENTHANDLE_H__
#define __EVENTHANDLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <list>

struct EVENT_DATA
{
	int event_id;
	void* param;
};

class EventHandle
{
private:
	pthread_mutex_t m_evQueMutex;
	std::list<struct EVENT_DATA*> m_eventQueue;
	
public:
	EventHandle();
	virtual ~EventHandle();
	
	bool send( int event_id, void* param );
	bool receive( struct EVENT_DATA& data );
};


#endif /* __EVENTHANDLE_H__ */
