#include <stdio.h>
#include "../src/LineUtility.h"
#include "../src/StringList.h"
#include "../src/handler.h"

#define TEST_ASSERT(condition)                             \
	if ( (condition) ) {                                   \
		printf("%s(%u): Test OK\n", __func__, __LINE__ );  \
	} else {                                               \
		printf("%s(%u): Test NG\n", __func__, __LINE__ );  \
		printf("      %s\n", #condition);                  \
	}                                                      \



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
		strcpy(str, "a");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "a") == 0 );
		
		//Test:
		strcpy(str, "\"");
		LineUtility::strip_double_quote(str);
		TEST_ASSERT( strcmp(str, "\"") == 0 );
	}
	
	void test_get_key_value() {
		bool ret = true;
		char str[1024] = "", *key = NULL, *val = NULL;
		
		//Test:
		ret = LineUtility::get_key_value(NULL, key, val);
		TEST_ASSERT( ret == false );
		
#if 0
		//Test:
		ret = LineUtility::get_key_value(str, NULL, val);
		TEST_ASSERT( ret == false );
		
		//Test:
		ret = LineUtility::get_key_value(str, key, NULL);
		TEST_ASSERT( ret == false );
#endif
		
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
	}
};

class StringListTester
{
public:
	void init() {
		printf("**** TEST StringListTester START ***\n");
	};
	void term() {
		printf("**** TEST StringListTester END ***\n");
	};
	
	void run() {
		init();
		
		test_1();
		test_2();
		
		term();
	}
	
	void test_1() {
		StringList strlist;
		
		TEST_ASSERT( strlist.push(NULL) == false );
		
		TEST_ASSERT( strlist.push("aaa") == true );
		TEST_ASSERT( strlist.push("bbb") == true );
		TEST_ASSERT( strlist.push("ccc") == true );
		TEST_ASSERT( strlist.push("ddd") == true );
		
		TEST_ASSERT( strlist.count() == 4 );
		TEST_ASSERT( strcmp(strlist.at(2), "ccc") == 0 );
		TEST_ASSERT( strlist.at(100) == NULL );
		
		TEST_ASSERT( strcmp(strlist.getNext(), "aaa") == 0 );
		TEST_ASSERT( strcmp(strlist.getNext(), "bbb") == 0 );
		TEST_ASSERT( strcmp(strlist.getNext(), "ccc") == 0 );
		TEST_ASSERT( strcmp(strlist.getNext(), "ddd") == 0 );
		TEST_ASSERT( strlist.getNext() == NULL );
		
		strlist.clear();
		
		TEST_ASSERT( strlist.count() == 0 );
		
		TEST_ASSERT( strlist.push("eee") == true );
		TEST_ASSERT( strcmp(strlist.getNext(), "eee") == 0 );
	}

	void test_2() {
		StringList strlist;
		char str[1024];
		
		bool ret = true;
		for ( int i = 0; i < 10000; i++ ) {
			sprintf(str, "string string string %u\n", i);
//			TEST_ASSERT( strlist.push(str) == true );
			ret &= strlist.push(str);
		}
		TEST_ASSERT(ret==true);
		
		TEST_ASSERT( strlist.count() == 10000 );
		TEST_ASSERT( strcmp(strlist.at(9000), "string string string 9000\n") == 0 );
		
		strlist.clear();
		
		TEST_ASSERT( strlist.count() == 0 );
		
		TEST_ASSERT( strlist.push("clear and push") == true );
		TEST_ASSERT( strcmp(strlist.getNext(), "clear and push") == 0 );
	}
};

class HandlerTester
{
public:
	void init() {
		printf("**** TEST HandlerTester START ***\n");
	};
	void term() {
		printf("**** TEST HandlerTester END ***\n");
	};
	
	void run() {
		init();
		
		test_is_identifier();
		test_lex();
		
		term();
	}
	
	void test_is_identifier() {
		TEST_ASSERT(is_identifier('*') == true);
		TEST_ASSERT(is_identifier('=') == true);
		TEST_ASSERT(is_identifier(';') == true);
		TEST_ASSERT(is_identifier('A') == false);
		TEST_ASSERT(is_identifier('{') == false);
		TEST_ASSERT(is_identifier('\0') == false);
	}
	
	void test_lex() {
		char str[1024] = "";
		StringList tokens;
		
		//Test:
		strcpy(str, "int a = 1;");
		lex(str, tokens);
		TEST_ASSERT( strcmp(tokens.getNext(), "int") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "a") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "=") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "1") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), ";") == 0 );
		tokens.clear();
		
		//Test:
		strcpy(str, "char * string=\"##############\";");
		lex(str, tokens);
		TEST_ASSERT( strcmp(tokens.getNext(), "char") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "*") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "string") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "=") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "\"##############\"") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), ";") == 0 );
		tokens.clear();
		
		//Test:
		strcpy(str, "char *string=\"\";");
		lex(str, tokens);
		TEST_ASSERT( strcmp(tokens.getNext(), "char") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "*") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "string") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "=") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "\"\"") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), ";") == 0 );
		tokens.clear();
		
		//Test:
		strcpy(str, "char * string ====\"###");
		lex(str, tokens);
		TEST_ASSERT( strcmp(tokens.getNext(), "char") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "*") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "string") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "====") == 0 );
		TEST_ASSERT( strcmp(tokens.getNext(), "\"###") == 0 );
		TEST_ASSERT( tokens.getNext() == NULL );
		tokens.clear();
		
		//Test:
		strcpy(str, "");
		lex(str, tokens);
		TEST_ASSERT( tokens.getNext() == NULL );
		tokens.clear();
		
		//Test:
		strcpy(str, "aaa");
		lex(str, tokens);
		TEST_ASSERT( strcmp(tokens.getNext(), "aaa") == 0 );
		tokens.clear();
	}
};
	
int main( int argc, char **argv )
{
	LineUtilityTester tester;
	tester.run();
	
	StringListTester tester2;
	tester2.run();
	
	HandlerTester tester3;
	tester3.run();
	
	return 0;
}

