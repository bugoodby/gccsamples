#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


//ファイルの1行目を読み込み
//
// test.conf
// -------------------------
// 10
// -------------------------
void test_file()
{
	printf("********** %s **********\n", __FUNCTION__);
	
	char line[1024] = "";
	FILE *fp = fopen("test.conf", "r");
	if ( !fp ) {
		fprintf(stderr, "conf not found.\n");
	} else {
		fgets(line, sizeof(line), fp);
		fclose(fp);
		printf("LINE = %s\n", line);
		int32_t num = strtol(line, NULL, 0);
		printf("  %d is set.\n", num);
	}
}



//ファイルの各行を読み込み（#で始まる行はコメント）
//
// test.conf
// -------------------------
// 1
// #2
// 3
// 4
// -------------------------
void test_file2()
{
	printf("********** %s **********\n", __FUNCTION__);
	
	char line[1024] = "";
	FILE *fp = fopen("test.conf", "r");
	if ( !fp ) {
		fprintf(stderr, "conf not found.\n");
	} else {
		while ( fgets(line, sizeof(line), fp) != NULL ) {
			// remove \n
			char *p = strchr(line, '\n');
			if ( p ) { *p = '\0'; }
			
			printf("LINE = %s\n", line);
			if ( line[0] == '#' || line[0] == '\0' ) {
				continue;
			}
			switch ( line[0] ) {
			case '1':
				printf("  1 is set.\n");
				break;
			case '2':
				printf("  2 is set.\n");
				break;
			case '3':
				printf("  3 is set.\n");
				break;
			default:
				break;
			}
		}
		fclose(fp);
	}
}


void Sample_get_from_file()
{
	test_file();
	test_file2();
}

