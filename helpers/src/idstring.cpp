#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>




//数値に対する文字列を取得
const char* getString( int num )
{
	struct {
		int type;
		const char* str;
	} table[] = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
		{ 5, "five" }
	};
	size_t cnt = sizeof(table)/sizeof(table[0]);
	
	for ( size_t i = 0; i < cnt; ++i ) {
		if ( table[i].type == num ) {
			return table[i].str;
		}
	}
	return "???";
}



void Sample_IDString()
{
	printf("********** %s **********\n", __FUNCTION__);
	
	for ( int i = 5; i >= 0; --i ) {
		printf("num=%d => str=%s\n", i, getString(i));
	}
}

