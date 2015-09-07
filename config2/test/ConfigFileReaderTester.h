//LCOV_EXCL_START
#include <stdio.h>
#define protected public
#include "../src/ConfigFileReader.h"
#undef protected

class ConfigFileReaderTester
{
public:
	void init() {
		printf("**** TEST ConfigFileReaderTester START ***\n");
	};
	void term() {
		printf("**** TEST ConfigFileReaderTester END ***\n");
	};
	
	void run() {
		init();
		
		test_class();
		test_checkSection();
		test_getLine();
		test_open_close();
		
		term();
	}
	
	void test_class() {
		ConfigFileReader obj;
		
		ConfigFileReader *p = new ConfigFileReader;
		delete p;
	}
	
	void test_checkSection() {
		ConfigFileReader reader;
		
		TEST_ASSERT( reader.checkSection(NULL) == false );
		TEST_ASSERT( reader.checkSection("") == false );
		TEST_ASSERT( reader.checkSection("[]") == false );
		TEST_ASSERT( reader.checkSection("aa]") == false );
		TEST_ASSERT( reader.checkSection("[aa") == false );
		TEST_ASSERT( reader.checkSection("[abcdefg]") == true );
	}

	void test_getLine() {
		ConfigFileReader reader;
		char buffer[1024];
		
		TEST_ASSERT( reader.getLine(NULL, 1024) == false );
		TEST_ASSERT( reader.getLine(buffer, 0) == false );
	}
	
	void test_open_close() {
		ConfigFileReader reader;
		std::list<SECTION_DATA> list;
		
		//Test:
		TEST_ASSERT( reader.open("./not_found.ini") == false );
		TEST_ASSERT( reader.read(list) == false );
		reader.close(); //exception check
		
		//Test:
		TEST_ASSERT( reader.open("./ConfigFileReaderTest01.ini") == true );
		TEST_ASSERT( reader.read(list) == true );
		reader.close(); //exception check
		
		//Test:
		g_mallocUT = stub_malloc;
		TEST_ASSERT( reader.open("./ConfigFileReaderTest02.ini") == true );
		TEST_ASSERT( reader.read(list) == true );
		reader.close(); //exception check
		g_mallocUT = malloc;
	}
};
//LCOV_EXCL_END


