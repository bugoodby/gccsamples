#include <stdio.h>
#include <string.h>
#include "MemoryBuffer.h"

//! constructor
MemoryBuffer::MemoryBuffer() : m_buffer(NULL), m_len(0), m_size(0)
{
	x_reallocMemory(m_blocksize);
}

//! destructor
MemoryBuffer::~MemoryBuffer()
{
	if ( m_buffer ) free(m_buffer);
}

bool MemoryBuffer::x_reallocMemory( size_t size )
{
//	fprintf(stderr, "[MemoryBuffer] realloc(size=%lu)\n", size);
	
	char *p = (char*)realloc(m_buffer, size);
	if ( p ) {
		m_buffer = p;
		m_size = size;
		if ( m_len == 0 ) memset(m_buffer, 0, m_size); // first time only.
	} else {
		fprintf(stderr, "[MemoryBuffer] realloc(size=%lu) failed\n", size);
	}
	
	return (p != NULL);
}

bool MemoryBuffer::x_reallocEnoughMemory( size_t len )
{
	if ( len >= m_size ) {
		size_t new_size = m_size;
		do {
			new_size += m_blocksize;
		} while ( len >= new_size );
		return x_reallocMemory(new_size);
	}
	return true;
}

void MemoryBuffer::clear()
{
	if ( m_buffer ) memset(m_buffer, 0, m_size);
	m_len = 0;
}

bool MemoryBuffer::add( void *data, size_t size )
{
	if ( m_len + size > m_size ) {
		// buffer is not enough.
		if ( !x_reallocEnoughMemory(m_len + size) ) {
			return false;
		}
	}
	memcpy(m_buffer + m_len, data, size);
	m_len += size;
	
	return true;
}

const void* MemoryBuffer::ptr()
{
	return m_buffer;
}

size_t MemoryBuffer::len()
{
	return m_len;
}

