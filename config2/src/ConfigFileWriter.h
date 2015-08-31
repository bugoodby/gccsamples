#ifndef __CONFIGFILEWRITER_H__
#define __CONFIGFILEWRITER_H__

#include "main.h"
#include <stdio.h>
#include <list>

class ConfigFileWriter
{
protected:
	FILE *m_fp;
	
	void dumpValueHex( FILE *fp, const void *pData, size_t size );
	bool writeValueData( uint16_t id, void *pData, size_t size );

public:
	ConfigFileWriter();
	virtual ~ConfigFileWriter();
	
	bool open( const char *fname, bool isCreate );
	void close();
	bool write( std::list<SECTION_DATA> &list );
};

#endif /* __CONFIGFILEWRITER_H__ */
