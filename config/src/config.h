#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>

int GetSectionList( const char* fname, std::vector<std::string>& list );
int GetItemList( const char* fname, const char* section, std::vector<std::string>& list );
int GetKeyList( const char* fname, const char* section, std::vector<std::string>& list );

bool GetValueString( const char* fname, const char* section, const char* key, char *buf, size_t bufsize );

#endif /* __CONFIG_H__ */

