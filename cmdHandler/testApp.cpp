#include "cmdAppClient.h"
#include "stdio.h"
#include "unistd.h"
#include "trace.h"
#include "stdlib.h"
#include "signal.h"

CmdAppClient* aaa = NULL;

void sigHandler(int signo)
{
  delete aaa;
  abort();
}

void func(std::string& text, int argc, char** argv)
{
  text = "testCmd output";
  for (int i = 0; i < argc; i++)
  {
    printf("%s ", argv[i]);
  }
}

int main(int argc, const char** argv)
{
  TRACE_INIT(argc, argv);
  signal(SIGINT, sigHandler);
  aaa = new CmdAppClient;
  aaa->registerCmd("testCmd", func);
  aaa->start();
  while(true)
  {}
  return 0;
}
