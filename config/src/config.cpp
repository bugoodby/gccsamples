#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INI_LINE_MAX 1024


static void trim( char* line )
{
	int i = 0, j = 0;
	if ( !line ) return;
	
	for ( i = strlen(line)-1; i >= 0 && isspace(line[i]); --i );
	line[i+1] = '\0';
	for( i = 0; isspace(line[i]); ++i );
	if ( i > 0 ) {
		while ( line[i] != '\0' ) { line[j++] = line[i++]; }
		line[j] = '\0';
	}
}

#if 0
static void strip_dquote( char *line )
{
	size_t cnt = strlen(line);
	if ( cnt >= 2 ) {
		if ( line[0] == '\"' && line[cnt-1] == '\"' ) {
			line[cnt-1] = '\0';
			size_t i = 1, j = 0;
			while ( line[i] != '\0' ) { line[j++] = line[i++]; }
			line[j] = '\0';
		}
	}
}
#endif

static bool is_section( const char* line, char* sectname, size_t bufsize )
{
	size_t cnt = 0;
	bool found_start_end = false;
	sectname[0] = '\0';
	
	const char* pWork = line;
	if ( *pWork++ == '[' ) {
		while ( *pWork != '\0' ) {
			if ( *pWork == ']' ) {
				found_start_end = true;
				break;
			}
			pWork++;
			cnt++;
		}
	}
	if ( found_start_end && sectname ) {
		if ( cnt < bufsize ) {
			memcpy(sectname, &line[1], cnt);
			sectname[cnt] = '\0';
			trim(sectname);
		} else {
			fprintf(stderr, "%s: Too small buffer size. (size=%lu, need=%lu)\n", __func__, bufsize, cnt+1);
		}
	}
	
	return found_start_end;
}

static bool search_section( FILE* fp, const char* section )
{
	char line[INI_LINE_MAX] = "";
	char sectname[INI_LINE_MAX] = "";
	
	while ( fgets(line, sizeof(line), fp) )
	{
		trim(line);
		
		// comment or empty line
		if ( line[0] == ';' || line[0] == '\0' )
			continue;
		
		if ( is_section(line, sectname, sizeof(sectname)) )
		{
			if ( strcmp(sectname, section) == 0 )
			{
				return true;
			}
		}
	}
	return false;
}


int GetSectionList( const char* fname, std::vector<std::string>& list )
{
	char line[INI_LINE_MAX] = "";
	char sectname[INI_LINE_MAX] = "";
	
	list.clear();
	if ( !fname ) {
		fprintf(stderr, "%s: Invalid argument\n", __func__);
		return 0;
	}
	
	FILE *fp = fopen(fname, "r");
	if ( !fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return 0;
	}
	
	while ( fgets(line, sizeof(line), fp) )
	{
		trim(line);
		
		// comment or empty line
		if ( line[0] == ';' || line[0] == '\0' )
			continue;
		
		if ( is_section(line, sectname, sizeof(sectname)) )
		{
			list.push_back(sectname);
		}
	}
	
	fclose(fp);
	return list.size();
}

int GetItemList( const char* fname, const char* section, std::vector<std::string>& list )
{
	char line[INI_LINE_MAX] = "";
	char sectname[INI_LINE_MAX] = "";
	
	list.clear();
	if ( !fname || !section ) {
		fprintf(stderr, "%s: Invalid argument\n", __func__);
		return 0;
	}
	
	FILE *fp = fopen(fname, "r");
	if ( !fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return 0;
	}
	
	if ( search_section(fp, section) )
	{
		while ( fgets(line, sizeof(line), fp) )
		{
			trim(line);
			
			// comment or empty line
			if ( line[0] == ';' || line[0] == '\0' )
				continue;
			
			if ( is_section(line, sectname, sizeof(sectname)) )
				break;
			
			list.push_back(line);
		}
	}
	
	fclose(fp);
	return list.size();
}

int GetKeyList( const char* fname, const char* section, std::vector<std::string>& list )
{
	char line[INI_LINE_MAX] = "";
	char sectname[INI_LINE_MAX] = "";
	char *pos = NULL;
	
	list.clear();
	if ( !fname || !section ) {
		fprintf(stderr, "%s: Invalid argument\n", __func__);
		return 0;
	}
	
	FILE *fp = fopen(fname, "r");
	if ( !fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return 0;
	}
	
	if ( search_section(fp, section) )
	{
		while ( fgets(line, sizeof(line), fp) )
		{
			trim(line);
			
			// comment or empty line
			if ( line[0] == ';' || line[0] == '\0' )
				continue;
			
			if ( is_section(line, sectname, sizeof(sectname)) )
				break;
			
			if ( (pos = strstr(line, "=")) )
			{
				*pos = '\0';
				char* key_str = line;
				
				trim(key_str);
				if ( key_str[0] != '\0' ) {
					list.push_back(key_str);
				}
			}
		}
	}
	
	fclose(fp);
	return list.size();
}

bool GetValueString( const char* fname, const char* section, const char* key, char *buf, size_t bufsize )
{
	char line[INI_LINE_MAX] = "";
	char sectname[INI_LINE_MAX] = "";
	char *pos = NULL;
	bool ret = false;
	
	if ( !fname || !section || !buf || bufsize == 0 ) {
		fprintf(stderr, "%s: Invalid argument\n", __func__);
		return 0;
	}
	
	FILE *fp = fopen(fname, "r");
	if ( !fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return 0;
	}
	
	if ( search_section(fp, section) )
	{
		while ( fgets(line, sizeof(line), fp) )
		{
			trim(line);
			
			// comment or empty line
			if ( line[0] == ';' || line[0] == '\0' )
				continue;
			
			if ( is_section(line, sectname, sizeof(sectname)) )
				break;
			
			if ( (pos = strstr(line, "=")) )
			{
				*pos = '\0';
				char* key_str = line;
				
				trim(key_str);
				
				if ( strcmp(key, key_str) == 0 ) {
					char* val_str = pos + 1;
					trim(val_str);
					
					trim(val_str);
					strncpy(buf, val_str, bufsize - 1);
					buf[bufsize-1] = '\0';
					ret = true;
				}
			}
		}
	}
	
	fclose(fp);
	return ret;
}

