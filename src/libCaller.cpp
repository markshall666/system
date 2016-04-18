#include "libCaller.h"
#include "callbackObj.h"
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


using namespace std;

LibCaller::LibCaller(const char* _appName)
  :mapIter(0)
{
  appName = (char*)_appName;
}

bool LibCaller::dispatch()
{
  const char* message = "Something";
  return objMap[1]->onCall(message);
}

bool LibCaller::registerObj(CallbackObj* obj)
{
  objMap.insert(pair<int,  CallbackObj*>(++mapIter, obj));
  cout << "Object registered with index: " << mapIter << "\n";
  return true;
}

void LibCaller::init()
{
  cout << "Init called\n";

  int sock, sock2, msgsock, rval;
  struct sockaddr_un server;
  char buf[1024];
     
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock < 0) 
  { 
    perror("opening stream socket");
    exit(1);
  }
  
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, "server");

  if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) 
  {
    close(sock);
    perror("connecting stream socket");
    exit(1);
  }
      
  if (write(sock, appName, sizeof(appName)) < 0)
  {
    perror("writing on stream socket");
    close(sock);
  }

  close(sock);
  sock2 = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock2 < 0)
  {
    perror("opening stream socket");
    exit(1);
  }
  strcpy(server.sun_path, appName);
  if (bind(sock2, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
      perror("binding stream socket");
      exit(1);
  }
  printf("Socket has name %s\n", server.sun_path);
       listen(sock2, 5);
      // for (;;) {
           msgsock = accept(sock2, 0, 0);
           if (msgsock == -1)
               perror("accept");
           else do {
               bzero(buf, sizeof(buf));
               if ((rval = read(msgsock, buf, 1024)) < 0)
                   perror("reading stream message");
               else if (rval == 0)
                   printf("Ending connection\n");
               else
                   printf("-->%s\n", buf);
           } while (rval > 0);
           close(msgsock);
       //}
       close(sock2);
       unlink(appName);

}
