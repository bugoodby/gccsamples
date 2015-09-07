//LCOV_EXCL_START
#include <stdio.h>
#include "../src/ConfigFileReader.h"
#include "../src/ConfigFileWriter.h"

class JointTester
{
public:
	void init() {
		printf("**** TEST JointTester START ***\n");
	};
	void term() {
		printf("**** TEST JointTester END ***\n");
	};
	
	void run() {
		init();
		
		test(1);
		test(2);
		test(3);
		
		term();
	}
	
	bool equals( const char *fname1, const char *fname2 ) {
		bool ret = true;
		char line1[1024], line2[1024];
		
		FILE *fp1 = fopen(fname1, "r");
		FILE *fp2 = fopen(fname2, "r");
		
		while ( fgets(line1, sizeof(line1), fp1) ) {
			if ( !fgets(line2, sizeof(line2), fp2) ) {
				ret = false;
				break;
			}
			if ( strcmp(line1, line2) != 0 ) {
				ret = false;
				break;
			}
		}
		
		fclose(fp1);
		fclose(fp2);
		
		return ret;
	}
	
	void test( int idx ) {
		char fname1[1024];
		char fname2[1024];
		
		snprintf(fname1, sizeof(fname1), "./test%02d.ini", idx);
		snprintf(fname2, sizeof(fname2), "./test%02d_after.ini", idx);
		
		std::list<SECTION_DATA> list;
		
		ConfigFileReader reader;
		reader.open(fname1);
		reader.read(list);
		reader.close();
		
		ConfigFileWriter writer;
		writer.open(fname2, true);
		writer.write(list);
		writer.close();
		
		TEST_ASSERT(equals(fname1, fname2));
	}
};
//LCOV_EXCL_END
