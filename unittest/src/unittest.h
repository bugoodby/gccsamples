#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#define UNIT_ASSERT_SIMPLE(condition)                           \
	if ( (condition) ) {                                 \
		printf("%s(%u): Test OK\n", __func__, __LINE__ ); \
	} else {                                             \
		printf("%s(%u): Test NG\n", __func__, __LINE__ ); \
		printf("      %s\n", #condition);                \
	}                                                    \


#define UNIT_MESSAGE( msg )                                         \
	printf("%s(%u): %s\n", __func__, __LINE__, (msg) );             \


#define UNIT_ASSERT_EQUAL(expected, actual)                         \
	if ( (expected) == (actual) ) {                                 \
		if ( unit_ok_flag ) {                                       \
			printf("    OK: %s = %ld\n", #actual, (long)(actual));  \
		}                                                           \
	} else {                                                        \
		printf("    NG: %s = %ld (expected = %ld)\n",               \
			#actual, (long)(actual), (long)(expected));             \
	}                                                               \


#define UNIT_ASSERT(condition)                                      \
	if ( (condition) ) {                                            \
		if ( unit_ok_flag ) {                                       \
			printf("    NG: %s\n", #condition);                     \
		}                                                           \
	} else {                                                        \
		printf("    NG: %s\n", #condition);                         \
	}                                                               \


#endif /* __UNITTEST_H__ */
