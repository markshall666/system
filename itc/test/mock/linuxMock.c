/*
 * linuxMock.c
 *
 *  Created on: 25-07-2016
 *      Author: marek
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>


int socket(int domain, int type, int protocol)
{
	check_expected(domain);
	check_expected(type);
	check_expected(protocol);
	return mock_type(int);
}

void unlink(const char* name)
{
}

int bind (int fd, const struct sockaddr* addr, socklen_t len)
{
	return mock_type(int);
}

pthread_t pthread_self()
{
	return mock_type(pthread_t);
}

ssize_t sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len)
{
	check_expected(fd);
	check_expected(buf);
	check_expected(n);
	check_expected(flags);
	check_expected(addr);
	check_expected(addr_len);
	return mock_type(ssize_t);
}

