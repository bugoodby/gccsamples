#include <stdio.h>
#include "foo.h"
#include "bar.h"

#include "test1.h"
#include "test2.h"

int main( int argc, char **argv )
{
	printf("%d\n", foo(2, 3));
	printf("%d\n", foo(3, 4));

	// so1
	say_hello();
	say_yourname("tanaka");
	say_yourname("hoge");
	say_goodbye();
	list_files_r("./");

	return 0;
}

