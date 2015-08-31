#ifndef __STRING_LIST_H__
#define __STRING_LIST_H__

#include <stdlib.h>
#include <string.h>

class StringList
{
private:
	static const int m_blocksize = 100;
	
	char** m_buffer;    //!< buffer.
	size_t m_count;     //!< string count pushed to the buffer.
	size_t m_size;      //!< buffer size.
	size_t m_curpos;	//!< current position.
	
	bool x_reallocMemory( size_t size ) {
		char** p = (char**)realloc(m_buffer, size * sizeof(char*));
		if ( p ) {
			m_buffer = p;
			m_size = size;
			memset(m_buffer+m_count, 0, m_size-m_count);
		}
		return (p != NULL);
	}
	
public:
	StringList() : m_buffer(NULL), m_count(0), m_size(0), m_curpos(0) {
		x_reallocMemory(m_blocksize);
	};
	
	virtual ~StringList() {
		clear();
		free(m_buffer);
	}
	
	void clear() {
		if ( m_buffer ) {
			for ( size_t i = 0; i < m_count; i++ ) {
				free(m_buffer[i]);
				m_buffer[i] = NULL;
			}
		}
		m_count = 0;
		m_curpos = 0;
	}
	
	bool push( const char *str ) {
		if ( !str ) {
			return false;
		}
		if ( m_count >= m_size ) {
			if ( x_reallocMemory( m_size + m_blocksize ) ) {
				return false;
			}
		}
		char *p = strdup(str);
		if ( !p ) {
			return false;
		}
		m_buffer[m_count++] = p;
		return true;
	}
	
	char* at( size_t index ) {
		if ( !m_buffer || index >= m_count ) {
			return NULL;
		}
		return m_buffer[index];
	}
	
	char* getNext() {
		if ( !m_buffer || m_curpos >= m_count ) {
			return NULL;
		}
		return m_buffer[m_curpos++];
	}
	
	size_t count() {
		return m_count;
	};
};

#endif /* __STRING_LIST_H__ */
