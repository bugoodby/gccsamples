//LCOV_EXCL_START
#include <stdio.h>
#include "../src/Handler.h"

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
		
		test_lex();
		test_dump();
		
		term();
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
		strcpy(str, "char* string ====\"###");
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
		
		//Test:
		strcpy(str, "                ");
		lex(str, tokens);
		TEST_ASSERT( tokens.getNext() == NULL );
		tokens.clear();
	}
	
	void test_dump() {
		{
			//Test:
			uint8_t val = 0;
			dumpValuePtr ptr = getDumpFunc(1);
			ptr(stderr, &val, 0);
		}
		{
			//Test:
			uint16_t val = 0;
			dumpValuePtr ptr = getDumpFunc(2);
			ptr(stderr, &val, 0);
		}
		{
			//Test:
			STRING8 val = {0};
			dumpValuePtr ptr = getDumpFunc(5);
			ptr(stderr, &val, 0);
		}
	}
};
//LCOV_EXCL_END


