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

