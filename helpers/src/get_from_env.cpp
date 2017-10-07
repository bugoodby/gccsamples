#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// ex)
// export TESTVAL=3
// export TESTVAL2=1,2,5,10,-3


//環境変数からの取得
void test_getenv()
{
	printf("********** %s **********\n", __FUNCTION__);
	
	char *str = getenv("TESTVAL");
	printf("TESTVAL = %s\n", str);
	if ( str ) {
		uint32_t num = strtoul(str, NULL, 0);
		switch ( num ) {
		case 1:
			printf("  1 is set.\n");
			break;
		case 2:
			printf("  2 is set.\n");
			break;
		case 3:
			printf("  3 is set.\n");
			break;
		default:
			break;
		}
	}
}

//環境変数からの取得（,区切りのリスト）
void test_getenv2()
{
	printf("********** %s **********\n", __FUNCTION__);
	
	char *str = getenv("TESTVAL2");
	printf("TESTVAL2 = %s\n", str);
	if ( str ) {
		char *token = NULL, *saveptr = NULL;
		token = strtok_r(str, ",", &saveptr);
		while ( token ) {
			uint32_t num = strtoul(token, NULL, 0);
			printf("  token=%d\n", num);
			token = strtok_r(NULL, ",", &saveptr);
		}
	}
}




void Sample_get_from_env()
{
	test_getenv();
	test_getenv2();
}

