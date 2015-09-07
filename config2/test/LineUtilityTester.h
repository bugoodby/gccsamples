//LCOV_EXCL_START
#include <stdio.h>
#include "../src/LineUtility.h"

class LineUtilityTester
{
public:
	void init() {
		printf("**** TEST LineUtilityTester START ***\n");
	};
	void term() {
		printf("**** TEST LineUtilityTester END ***\n");
	};
	
	void run() {
		init();
		
		test_trim();
		test_delete_comment();
		test_strip_double_quote();
		test_get_key_value();
		
		term();
	}
	
	void test_trim() {
		char str[1024] = "";
		
		//Test:
		LineUtility::trim(NULL);
		
		//Test:
		strcpy(str, "");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
		
		//Test:
		strcpy(str, "\n");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
		
		//Test:
		strcpy(str, "a");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "a") == 0 );
		
		//Test:
		strcpy(str, "a             \n");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "a") == 0 );
		
		//Test:
		strcpy(str, "                   a");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "a") == 0 );
		
		//Test:
		strcpy(str, "   abc   \r\n");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "abc") == 0 );
		
		//Test:
		strcpy(str, "月 火 水 木 金 土 日");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "月 火 水 木 金 土 日") == 0 );
		
		//Test:
		strcpy(str, "\t  \t あ　　　い   \t  \t\r\n");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "あ　　　い") == 0 );
		
		//Test:
		strcpy(str, "   \t \t  \t   \t       \t\t    \t\t  \t \t\t\t  \r\n");
		LineUtility::trim(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
	}
	
	void test_delete_comment() {
		char str[1024] = "";
		
		//Test:
		LineUtility::delete_comment(NULL);
		
		//Test:
		strcpy(str, "");
		LineUtility::delete_comment(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
		
		//Test:
		strcpy(str, "#コメントのみ文字列\n");
		LineUtility::delete_comment(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
		
		//Test:
		strcpy(str, "コメントなし文字列\n");
		LineUtility::delete_comment(str);
		TEST_ASSERT( strcmp(str, "コメントなし文字列\n") == 0 );
		
		//Test:
		strcpy(str, "コメント混在文字列   #comment.  \r\n");
		LineUtility::delete_comment(str);
		TEST_ASSERT( strcmp(str, "コメント混在文字列   ") == 0 );
		
		//Test:
		strcpy(str, "string = \"###\"  #comment.\r\n");
		LineUtility::delete_comment(str);
		TEST_ASSERT( strcmp(str, "string = \"###\"  ") == 0 );
		
		//Test:
		strcpy(str, "str1=\"###\", str2=\"###      #comment.\n");
		LineUtility::delete_comment(str);
		TEST_ASSERT( strcmp(str, "str1=\"###\", str2=\"###      #comment.\n") == 0 );
	}
	
	void test_strip_double_quote() {
		char str[1024] = "";
		
		//Test:
		LineUtility::strip_double_quote(NULL);
		
		//Test:
		strcpy(str, "");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
		
		//Test:
		strcpy(str, "\"");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "\"") == 0 );
		
		//Test:
		strcpy(str, "\"\"");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "") == 0 );
		
		//Test:
		strcpy(str, "\"a\"");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "a") == 0 );
		
		//Test:
		strcpy(str, "\"abc\"");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "abc") == 0 );
		
		//Test:
		strcpy(str, "abcedf");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "abcedf") == 0 );
		
		//Test:
		strcpy(str, "\"aaa");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "\"aaa") == 0 );
		
		//Test:
		strcpy(str, "aaa\"");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "aaa\"") == 0 );
	}
	
	void test_get_key_value() {
		bool ret = true;
		char str[1024] = "", *key = NULL, *val = NULL;
		
		//Test:
		ret = LineUtility::get_key_value(NULL, key, val);
		TEST_ASSERT( ret == false );
		
		//Test:
		strcpy(str, "a=b");
		ret = LineUtility::get_key_value(str, key, val);
		TEST_ASSERT( ret == true );
		TEST_ASSERT( strcmp(key, "a") == 0 );
		TEST_ASSERT( strcmp(val, "b") == 0 );
		
		//Test:
		strcpy(str, "  abc = ddd  ");
		ret = LineUtility::get_key_value(str, key, val);
		TEST_ASSERT( ret == true );
		TEST_ASSERT( strcmp(key, "abc") == 0 );
		TEST_ASSERT( strcmp(val, "ddd") == 0 );
		
		//Test:
		strcpy(str, "  =  ");
		ret = LineUtility::get_key_value(str, key, val);
		TEST_ASSERT( ret == true );
		TEST_ASSERT( strcmp(key, "") == 0 );
		TEST_ASSERT( strcmp(val, "") == 0 );
		
		//Test:
		strcpy(str, "aaa-bbb");
		ret = LineUtility::get_key_value(str, key, val);
		TEST_ASSERT( ret == false );
	}
};
//LCOV_EXCL_END

