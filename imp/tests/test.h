#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <math.h>

static inline void good(const char* format, ...){
	char buf[1024] = {}, txt[1024] = {};
	va_list args;

	va_start(args, format);
	vsnprintf((char*)buf, (size_t)sizeof(buf), format, args);
	va_end(args);

	sprintf(txt, "\t\033[0;32m%s\033[0m\n", buf);
	write(STDERR_FILENO, txt, strlen(txt) + 1);

	return;
}

static inline void info(const char* format, ...){
	char buf[1024] = {}, txt[1024] = {};
	va_list args;

	va_start(args, format);
	vsnprintf((char*)buf, (size_t)sizeof(buf), format, args);
	va_end(args);


	sprintf(txt, "%s", buf);
	write(STDERR_FILENO, txt, strlen(txt) + 1);

	return;
}


static inline void bad(const char* format, ...){
	char buf[1024] = {}, txt[1024] = {};
	va_list args;

	va_start(args, format);
	vsnprintf((char*)buf, (size_t)sizeof(buf), format, args);
	va_end(args);


	sprintf(txt, "\t\033[1;31m%s\033[0m\n", buf);
	write(STDERR_FILENO, txt, strlen(txt) + 1);

	return;
}

int NEAR(float x, float y)
{
	return fabs(x - y) <= 0.0001;
}

int run_test(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	int testNumber = 1;
	int totalTests = 1;
	if(argc >= 3)
	{
		testNumber = atoi(argv[1]);
		totalTests = atoi(argv[2]);
	}

	info("\e[1;33m[%s] (%d/%d)\033[0m\n", argv[0], testNumber + 1, totalTests);

	#ifdef DESCRIPTION
	info("%s\n", DESCRIPTION);
	#endif

	int ret = run_test(argc, argv);

	if (ret)
	{
		bad("(%d) FAILED\n\n", ret);
	}
	else
	{
		good("(%d) PASSED\n\n", ret);
	}

	return ret;
}

int run_test(int argc, char* argv[])
