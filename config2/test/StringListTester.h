//LCOV_EXCL_START
#define malloc g_mallocUT
#define realloc g_reallocUT
#define strdup g_strdupUT
#include "../src/StringList.h"
#undef malloc
#undef realloc
#undef strdup

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
		
		test_class();
		test_1();
		test_2();
		test_3();
		test_4();
		
		term();
	}
	
	void test_class() {
		StringList obj;
		
		StringList *p = new StringList;
		delete p;
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
	
	void test_3() {
		g_reallocUT = stub_realloc;
		
		StringList strlist;
		TEST_ASSERT( strlist.push("aaa") == false );
		TEST_ASSERT( strlist.at(0) == NULL );
		TEST_ASSERT( strlist.getNext() == NULL );
		
		g_reallocUT = realloc;
	}
	
	void test_4() {
		g_strdupUT = stub_strdup;
		
		StringList strlist;
		TEST_ASSERT( strlist.push("aaa") == false );
		TEST_ASSERT( strlist.at(0) == NULL );
		TEST_ASSERT( strlist.getNext() == NULL );
		
		g_strdupUT = strdup;
	}
};
//LCOV_EXCL_END

