//LCOV_EXCL_START
#include <stdio.h>
#define protected public
#include "../src/ConfigFileWriter.h"
#undef protected

class ConfigFileWriterTester
{
public:
	void init() {
		printf("**** TEST ConfigFileWriterTester START ***\n");
	};
	void term() {
		printf("**** TEST ConfigFileWriterTester END ***\n");
	};
	
	void run() {
		init();
		
		test_class();
		test_open_close();
		
		term();
	}
	
	void test_class() {
		ConfigFileWriter obj;
		
		ConfigFileWriter *p = new ConfigFileWriter;
		delete p;
	}
	
	void test_open_close() {
		ConfigFileWriter writer;
		std::list<SECTION_DATA> list;
		
		//Test:
		TEST_ASSERT( writer.open("/hogehoge/not_found.ini", true) == false );
		TEST_ASSERT( writer.write(list) == false );
		writer.close(); //exception check
		
		//Test:
		TEST_ASSERT( writer.open("/hogehoge/not_found.ini", false) == false );
		TEST_ASSERT( writer.write(list) == false );
		writer.close(); //exception check
		
		//Test:
		TEST_ASSERT( writer.open("./ConfigFileWriterTest01.ini", true) == true );
		TEST_ASSERT( writer.write(list) == true );
		writer.close(); //exception check
		
		//Test:
		SECTION_DATA data = {0};
		data.size = 0;
		list.push_back(data);
		
		TEST_ASSERT( writer.open("./ConfigFileWriterTest02.ini", true) == true );
		TEST_ASSERT( writer.write(list) == true );
		writer.close(); //exception check
	}
};
//LCOV_EXCL_END


