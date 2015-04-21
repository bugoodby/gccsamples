#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "test2.h"

#ifndef _countof
#define _countof(a) sizeof(a)/sizeof(a[0])
#endif

bool list_files_r_sub( char *buffer, size_t len, size_t init_len );

//LCOV_EXCL_START
void list_files()
{
	DIR* dir = NULL;
	struct dirent* dp = NULL; test();

	if ( NULL == (dir = opendir("./")) ) {
		printf("ディレクトリを開けませんでした\n");
		exit(1);
	}
	for ( int i = 0; NULL != (dp = readdir(dir)); i++ ) {
		printf("%d:%s\n" , i , dp->d_name);
	}
	closedir(dir);
}
//LCOV_EXCL_STOP
//----------------------------------------------------------------------
//! @brief      list files.
//! @param[in]  path  target path.
//! @return     true on success, otherwise false.
//----------------------------------------------------------------------
bool list_files_r( const char* path )
{
	char buffer[NAME_MAX + 1] = "";
	
	if ( !path ) {
		printf("[ERR] invalid path\n");
		return false;
	}
	strncpy(buffer, path, NAME_MAX);
	
	return list_files_r_sub( buffer, strlen(buffer), strlen(buffer) );
}

//----------------------------------------------------------------------
//! @brief      list files recursively.
//! @param[in]  buffer   a buffer of a target path.
//! @param[in]  len      length of a target path.
//! @param[in]  init_len initial length of a target path.
//! @return     true on success, otherwise false. 
//----------------------------------------------------------------------
bool list_files_r_sub( char *buffer, size_t len, size_t init_len )
{
	DIR* dir = NULL;
	struct dirent* dp = NULL;
	struct stat st = {0};
	
	
	if ( buffer[strlen(buffer)-1] != '/' ) {
		strncat(buffer, "/", NAME_MAX);
		len += 1;
	}

	if ( !(dir = opendir(buffer)) ) {
		printf("[ERR] cannot open %s\n", buffer);
		return false;
	}
	while ( (dp = readdir(dir)) ) {
		if ( (strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0) ) {
			continue;
		}
		strncpy(buffer + len, dp->d_name, NAME_MAX - len);
		if ( stat(buffer, &st) != 0 ) {
			printf("[ERR] stat(%s) failed. (err=%d)\n", buffer, errno);
			break;
		}
		if ( S_ISDIR(st.st_mode) ) {
			// directory
			if ( !list_files_r_sub(buffer, len + strlen(dp->d_name), init_len) ) {
				break;
			}
		} else {
			// file
			printf("%s : %s\n", dp->d_name, buffer);
		}
	}
	
	closedir(dir);
	return true;
}

			
void test()
{
	list_files_r(NULL);
}
