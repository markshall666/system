#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <sys/socket.h>
#include "itc.h"
#include "trace.h"

#define THREAD_1 0x10000666
#define THREAD_2 0x10000667
#define THREAD_3 0x10000668

void initSetup(int no)
{
  expect_value(socket, domain, AF_UNIX);
  expect_value(socket, type, SOCK_DGRAM);
  expect_value(socket, protocol, 0);

  will_return(socket, 3 + no);
  will_return(bind, 0);
  will_return(pthread_self, THREAD_1 + no);
}

static void itcInit(void **state)
{
  initSetup(0);

  assert_int_equal(initItc("test1"), THREAD_1);
}

static void itcTerminate(void **state)
{
  assert_int_equal(terminateItc(THREAD_1), 1);
}

static void itcTerminateFailed(void **state)
{
  assert_int_equal(terminateItc(THREAD_1), 0);
}

static void itcInitFailed1(void **state)
{
  expect_value(socket, domain, AF_UNIX);
  expect_value(socket, type, SOCK_DGRAM);
  expect_value(socket, protocol, 0);

  will_return(socket, -1);

  assert_int_equal(initItc("test1"), 0);
}

static void itcInitFailed2(void **state)
{
  expect_value(socket, domain, AF_UNIX);
  expect_value(socket, type, SOCK_DGRAM);
  expect_value(socket, protocol, 0);

  will_return(socket, 3);
  will_return(bind, 1);

  assert_int_equal(initItc("test1"), 0);
}

static void itcInitMulti(void **state)
{
  initSetup(0);

  assert_int_equal(initItc("test1"), THREAD_1);

  initSetup(1);

  assert_int_equal(initItc("test2"), THREAD_2);

  initSetup(2);

  assert_int_equal(initItc("test3"), THREAD_3);

  assert_int_equal(terminateItc(THREAD_1), 1);

  assert_int_equal(terminateItc(THREAD_2), 1);

  assert_int_equal(terminateItc(THREAD_3), 1);
}

static void itcInitMax(void **state)
{
  for (int i = 0; i < 16; i++)
  {
    initSetup(i);
    char name[7];
    sprintf(name, "test%d", i);
    initItc(name);
  }

  assert_int_equal(initItc("test"), 0);

  for (int i = 0; i < 16; i++)
  {
      assert_int_equal(terminateItc(i + THREAD_1), 1);
  }
}

static void itcInitSameName(void **state)
{
  initSetup(0);

  assert_int_equal(initItc("test"), THREAD_1);

  assert_int_equal(initItc("test"), 0);

  assert_int_equal(terminateItc(THREAD_1), 1);
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
  initSetup(0);
  initItc("test1");
  void* msg = itcAlloc(8, 666);
  uint32_t* msgPtr = msg;
  TRACE_MSG(msg);

  will_return_count(pthread_self, THREAD_1, 2);
  expect_value(sendto, fd, 3);
  expect_value(sendto, buf, msgPtr - 2);
  expect_value(sendto, n, 16);
  expect_value(sendto, flags, 0);
  expect_any(sendto, addr);
  expect_value(sendto, addr_len, 110);
  will_return(sendto, 16);
  assert_int_equal(sendData("test2", msg), 1);

  assert_int_equal(terminateItc(THREAD_1), 1);
}

static void itcSendDataFailed1(void **state)
{
  initSetup(0);
  initItc("test1");
  void* msg = itcAlloc(8, 666);
  uint32_t* msgPtr = msg;

  will_return_count(pthread_self, THREAD_1, 2);
  expect_value(sendto, fd, 3);
  expect_value(sendto, buf, msgPtr - 2);
  expect_value(sendto, n, 16);
  expect_value(sendto, flags, 0);
  expect_any(sendto, addr);
  expect_value(sendto, addr_len, 110);
  will_return(sendto, 0);
  assert_int_equal(sendData("test2", msg), 0);

  assert_int_equal(terminateItc(THREAD_1), 1);
}

static void itcSendDataFailed2(void **state)
{
  void* msg = itcAlloc(8, 666);
  assert_int_equal(sendData("test2", msg), 0);
}

static void itcReceiveData(void **state)
{
  initSetup(0);
  initItc("test1");

  uint32_t* buf = (uint32_t*)itcAlloc(8, 666);
  *(buf - 1) = THREAD_1;
  will_return_count(pthread_self, THREAD_1, 1);
  expect_value(recv, fd, 3);
  expect_value(recv, n, ITC_MAX_MSG_SIZE);
  will_return(recv_helper, (buf - 2));
  will_return(recv, 16);
  uint32_t* msg = (uint32_t*)receiveData();

  assert_int_equal(*msg, 666);
  assert_int_equal(*(msg - 1), THREAD_1);
  assert_int_equal(*(msg - 2), 8);

  itcFree((void*)buf);
  itcFree((void*)msg);
  assert_int_equal(terminateItc(THREAD_1), 1);
}

static void itcReceiveDataFailed(void **state)
{
  assert_int_equal(receiveData(), NULL);
}

int main(int argc, const char** argv)
{
  TRACE_INIT(argc, argv);

  const struct CMUnitTest tests[] =
  {
    cmocka_unit_test(itcInit),
    cmocka_unit_test(itcTerminate),
    cmocka_unit_test(itcTerminateFailed),
    cmocka_unit_test(itcInitFailed1),
    cmocka_unit_test(itcInitFailed2),
    cmocka_unit_test(itcInitMulti),
    cmocka_unit_test(itcInitMax),
    cmocka_unit_test(itcInitSameName),
    cmocka_unit_test(allocFreeItc),
    cmocka_unit_test(itcSendData),
    cmocka_unit_test(itcSendDataFailed1),
    cmocka_unit_test(itcSendDataFailed2),
    cmocka_unit_test(itcReceiveData),
    cmocka_unit_test(itcReceiveDataFailed),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
