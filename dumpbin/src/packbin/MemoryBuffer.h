#ifndef __MEMORYBUFFER_H__
#define __MEMORYBUFFER_H__

#include <stdlib.h>

class MemoryBuffer
{
private:
	char* m_buffer;     //!< buffer.
	size_t m_len;       //!< data size that is written in buffer.
	size_t m_size;      //!< buffer size.
	
	static const size_t m_blocksize = 1024;    //!< block unit size.
	
	bool x_reallocMemory( size_t size );
	bool x_reallocEnoughMemory( size_t string_len );
	
public:
	MemoryBuffer();
	virtual ~MemoryBuffer();
	
	void clear();
	bool add( void *data, size_t size );
	const void* ptr();
	size_t len();
};

#endif /* __MEMORYBUFFER_H__ */
