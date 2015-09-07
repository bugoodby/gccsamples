//LCOV_EXCL_START
#include "unit.h"
#include "StringListTester.h"
#include "LineUtilityTester.h"
#include "handlerTester.h"
#include "ConfigFileReaderTester.h"
#include "ConfigFileWriterTester.h"
#include "JointTester.h"


int main( int argc, char **argv )
{
	StringListTester tester1;
	tester1.run();
	
	LineUtilityTester tester2;
	tester2.run();
	
	HandlerTester tester3;
	tester3.run();
	
	ConfigFileReaderTester tester4;
	tester4.run();
	
	ConfigFileWriterTester tester5;
	tester5.run();
	
	JointTester tester6;
	tester6.run();
	
	return 0;
}
//LCOV_EXCL_END

