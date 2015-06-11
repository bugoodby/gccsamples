#include "main.h"
#include "unittest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define UNIT_EXPECT_XXX( target, expect_data, expect_flag1, expect_flag2, expect_param ) \
{                                                                                   \
	bool ret = true;                                                                \
	bool flag1 = false, flag2 = false;                                              \
	int param = 0;                                                                  \
	long data = 100;                                                                \
	                                                                                \
	printf("%s(%u): getdata(%s, xxx)\n", __func__, __LINE__, #target );             \
	ret = getdata(target, 1000, &data, sizeof(data), &flag1, &flag2, &param);       \
	UNIT_ASSERT_EQUAL( true, ret );                                                 \
	UNIT_ASSERT_EQUAL( (expect_data), data );                                       \
	UNIT_ASSERT_EQUAL( (expect_flag2), flag1 );                                     \
	UNIT_ASSERT_EQUAL( (expect_flag2), flag2 );                                     \
	UNIT_ASSERT_EQUAL( (expect_param), param );                                     \
}                                                                                   \


bool getdata( int target, int id, void* buf, size_t size, bool* flg1, bool* flg2, int* param )
{
	if ( flg1 ) *flg1 = true;
	if ( flg2 ) *flg2 = true;
	if ( param ) *param = 100;
	return true;
}

int main( int argc, char **argv )
{
	int hoge = 1;
	bool fuga = false;
	char piyo[] = "piyopiyo";
	
	bool unit_ok_flag = false;
	
	UNIT_ASSERT( hoge == 1 );
	UNIT_ASSERT( ( hoge == 2 ) && ( !fuga ) );
	UNIT_ASSERT( strcmp(piyo, "fugafuga") == 0 );

	UNIT_ASSERT( ( hoge == 2 ) &&
	             ( !fuga ) &&
	             ( strcmp(piyo, "fugafuga") == 0 ) );
	
	
	int target = 9999;
	int id = 35;
	
	{
		bool ret = true;
		bool flag1 = false, flag2 = false;
		int param = 0;
		
		long data = 100;
		
		UNIT_MESSAGE( "ID1" );
			ret = getdata(target, id, &data, sizeof(data), &flag1, &flag2, &param);
			UNIT_ASSERT_EQUAL( true, ret );
			UNIT_ASSERT_EQUAL( 50, data );
			UNIT_ASSERT_EQUAL( false, flag1 );
			UNIT_ASSERT_EQUAL( false, flag2 );
			UNIT_ASSERT_EQUAL( 1, param );
	}

	UNIT_EXPECT_XXX( target, 100, false, false, 0 );
	UNIT_EXPECT_XXX( target, 100, true, true, 100 );
	
	return 0;
}

