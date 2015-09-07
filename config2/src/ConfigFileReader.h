#ifndef __CONFIGFILEREADER_H__
#define __CONFIGFILEREADER_H__

#include "main.h"
#include "StringList.h"
#include <stdio.h>
#include <list>

class ConfigFileReader
{
protected:
	FILE *m_fp;
	
	bool checkSection( const char* line );
	bool getLine( char *buffer, size_t bufsize );
	bool parseSectionData( StringList &strlist, SECTION_DATA &sectData );

public:
	ConfigFileReader();
	virtual ~ConfigFileReader();
	
	bool open( const char *fname );
	void close();
	bool read( std::list<SECTION_DATA> &list );
};

#endif /* __CONFIGFILEREADER_H__ */
