#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <sys/socket.h>
#include "itc.h"

int fd1, fd2, fd3;

#define THREAD_1 0x10000666
#define THREAD_2 0x10000667
#define THREAD_3 0x10000668

static void itcInit(void **state)
{
	expect_value(socket, domain, AF_UNIX);
	expect_value(socket, type, SOCK_DGRAM);
	expect_value(socket, protocol, 0);

	will_return(socket, 3);
	will_return(bind, 0);
	will_return(pthread_self, THREAD_1);

	assert_int_equal(initItc("test1", &fd1), 1);
    assert_int_equal(fd1, 3);
}

static void itcTerminate(void **state)
{
	will_return(pthread_self, THREAD_1);

	terminateItc(&fd1);
	assert_int_equal(fd1, 0);
}

static void itcInitFailed1(void **state)
{
	expect_value(socket, domain, AF_UNIX);
	expect_value(socket, type, SOCK_DGRAM);
	expect_value(socket, protocol, 0);

	will_return(socket, -1);

	assert_int_equal(initItc("test1", &fd1), 0);
    assert_int_equal(fd1, 0);
}

static void itcInitFailed2(void **state)
{
	expect_value(socket, domain, AF_UNIX);
	expect_value(socket, type, SOCK_DGRAM);
	expect_value(socket, protocol, 0);

	will_return(socket, 3);
	will_return(bind, 1);

	assert_int_equal(initItc("test1", &fd1), 0);
    assert_int_equal(fd1, 0);
}

static void itcInitMulti(void **state)
{
	expect_value(socket, domain, AF_UNIX);
	expect_value(socket, type, SOCK_DGRAM);
	expect_value(socket, protocol, 0);

	will_return(socket, 3);
	will_return(bind, 0);
	will_return(pthread_self, THREAD_1);

	assert_int_equal(initItc("test1", &fd1), 1);
    assert_int_equal(fd1, 3);

	expect_value(socket, domain, AF_UNIX);
	expect_value(socket, type, SOCK_DGRAM);
	expect_value(socket, protocol, 0);

	will_return(socket, 4);
    will_return(bind, 0);
	will_return(pthread_self, THREAD_2);

	assert_int_equal(initItc("test2", &fd2), 1);
    assert_int_equal(fd2, 4);

	expect_value(socket, domain, AF_UNIX);
	expect_value(socket, type, SOCK_DGRAM);
	expect_value(socket, protocol, 0);

	will_return(socket, 5);
    will_return(bind, 0);
	will_return(pthread_self, THREAD_3);

	assert_int_equal(initItc("test3", &fd3), 1);
    assert_int_equal(fd3, 5);

	will_return(pthread_self, THREAD_1);
	terminateItc(&fd1);
	assert_int_equal(fd1, 0);

	will_return(pthread_self, THREAD_2);
	terminateItc(&fd2);
	assert_int_equal(fd2, 0);

	will_return(pthread_self, THREAD_3);
	terminateItc(&fd3);
	assert_int_equal(fd3, 0);
}

static void itcInitMax(void **state)
{
	int fd;
	for (int i = 0; i < 16; i++)
	{
		expect_value(socket, domain, AF_UNIX);
		expect_value(socket, type, SOCK_DGRAM);
		expect_value(socket, protocol, 0);

		will_return(socket, i + 3);
		will_return(bind, 0);
		will_return(pthread_self, i + THREAD_1);
		initItc("test", &fd);
	}

	assert_int_equal(initItc("test", &fd), 0);

	for (int i = 0; i < 16; i++)
	{
		will_return(pthread_self, i + THREAD_1);
		terminateItc(&fd);
	}
}

static void allocFreeItc(void **state)
{
	void* msg = itcAlloc(8, 666);
	uint32_t* msgNo = msg;
	assert_int_equal(*msgNo, 666);
	uint32_t* msgSize = msgNo - 2;
	assert_int_equal(*msgSize, 8);
	uint32_t* msgData = msgNo + 1;
	assert_int_equal(*msgData, 0);
	itcFree(msg);
}

static void itcSendData(void **state)
{

}

int main(void)
{
    const struct CMUnitTest tests[] =
    {
    	cmocka_unit_test(itcInit),
    	cmocka_unit_test(itcTerminate),
    	cmocka_unit_test(itcInitFailed1),
    	cmocka_unit_test(itcInitFailed2),
    	cmocka_unit_test(itcInitMulti),
    	cmocka_unit_test(itcInitMax),
    	cmocka_unit_test(allocFreeItc),
    	cmocka_unit_test(itcSendData),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
