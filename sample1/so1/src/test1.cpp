#include <stdio.h>
#include <iostream>
#include <cstring>
#include "test1.h"

int g_val = 100;

int say_hello()
{
	std::cout << "Hello, world" << std::endl;
	return 0;
}

int say_yourname( const char *name )
{
	std::cout << "Your name is " << name << std::endl;
	std::cout << "length = " << strlen(name) << std::endl;
	return 0;
}

int say_goodbye()
{
	std::cout << "Good bye!" << std::endl;
	return 0;
}

