#ifndef __STRING_LIST_H__
#define __STRING_LIST_H__

#include <stdlib.h>

class StringList
{
private:
	char** m_buffer;    //!< buffer.
	size_t m_count;     //!< string count pushed to the buffer.
	size_t m_size;      //!< buffer size.
	
	bool x_reallocMemory( size_t size );
	void debug();
	
public:
	StringList();
	virtual ~StringList();
	
	void clear();
	bool push( const char *str );
	char* at( size_t index );
	size_t count();
};

#endif /* __STRING_LIST_H__ */
