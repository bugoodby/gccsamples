#include "ConfigFileReader.h"
#include "LineUtility.h"
#include "ValueLoader.h"
#include <string.h>


#define INI_LINE_MAX 1024



bool ConfigFileReader::checkSection( const char* line ) 
{
	bool ret = false;
	
	if ( !line ) return false;
	if ( line[0] == '\0' ) return false;
	
	size_t len = strlen(line);
	if ( line[0] == '[' && line[len-1] == ']' ) {
		ret = true;
	}
	
	return ret;
}

bool ConfigFileReader::getLine( char *buffer, size_t bufsize ) 
{
	bool ret = true;
	
	if ( !buffer || bufsize == 0 ) {
		return false;
	}
	
	while ( (ret = fgets(buffer, bufsize, m_fp)) ) {
		LineUtility::delete_comment(buffer);
		LineUtility::trim(buffer);
		if ( buffer[0] != '\0' ) break;
	}
	return ret;
}

bool ConfigFileReader::parseSectionData( StringList &strlist, SECTION_DATA &sectData ) 
{
	fprintf(stderr, "%s: start\n", __func__ );
	
	bool ret = true;
	memset(&sectData, 0, sizeof(sectData));
	
	char* line = NULL;
	while ( (line = strlist.getNext()) != NULL ) {
		char *key = NULL, *value = NULL;
		if ( !LineUtility::get_key_value(line, key, value) ) {
			fprintf(stderr, "  not key-value\n");
			continue;
		}
		fprintf(stderr, "  Key-Value: [%s] = [%s]\n", key, value);
		
		if ( strcmp(key, "key") == 0 ) {
			sectData.id = static_cast<uint16_t>(strtoul(value, NULL, 0));
		}
		else if ( strcmp(key, "size") == 0 ) {
			sectData.size = static_cast<size_t>(strtoul(value, NULL, 0));
		}
		else if ( strcmp(key, "Value") == 0 ) {
			ValueLoader *pLoader = ValueLoaderFactory::create(value, sectData.id, sectData.size);
			if ( !pLoader ) {
				ret = false;
				break;
			}
			if ( !pLoader->load(strlist, sectData.value) ) {
				ret = false;
			}
			delete pLoader;
		}
	}
	fprintf(stderr, "%s: end\n", __func__ );
	return ret;
}



ConfigFileReader::ConfigFileReader() : m_fp(NULL)
{
}

ConfigFileReader::~ConfigFileReader() 
{
	close();
}
	
bool ConfigFileReader::open( const char *fname ) 
{
	m_fp = fopen(fname, "r");
	if ( !m_fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return false;
	}
	return true;
}

void ConfigFileReader::close()
{
	if ( m_fp ) {
		fclose(m_fp);
		m_fp = NULL;
	}
}

bool ConfigFileReader::read( std::list<SECTION_DATA> &list )
{
	char line[INI_LINE_MAX] = "";
	StringList lines;
	SECTION_DATA data = {0};
	
	while ( getLine(line, sizeof(line)) ) {
		fprintf(stderr, "%s: LINE:%s\n", __func__, line);
		
		if ( checkSection(line) ) {
			if ( parseSectionData(lines, data) ) {
				list.push_back(data);
			}
			lines.clear();
		} else {
			lines.push(line);
		}
	}
	if ( parseSectionData(lines, data) ) {
		list.push_back(data);
	}
	
	return true;
}

