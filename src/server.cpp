#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define NAME "server"

int main()
{ 
    int sock, sock2, msgsock, rval;
     struct sockaddr_un server;
     char buf[1024];

     sock = socket(AF_UNIX, SOCK_STREAM, 0);
     if (sock < 0) {
         perror("opening stream socket");
         exit(1);
     }
     server.sun_family = AF_UNIX;
     strcpy(server.sun_path, NAME);
     if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
         perror("binding stream socket");
         exit(1);
     }
     printf("Socket has name %s\n", server.sun_path);
     listen(sock, 5);
     //for (;;) {
         msgsock = accept(sock, 0, 0);
         if (msgsock == -1)
             perror("accept");
         else do {
             //bzero(buf, sizeof(buf));
             if ((rval = read(msgsock, buf, 1024)) < 0)
                 perror("reading stream message");
             else if (rval == 0)
                 printf("Ending connection\n");
             else
                 printf("-->%s\n", buf);
         } while (rval > 0);
         std::cout << "miau1 " <<"\n";
         close(msgsock);
     //}
         std::cout << "miau2 " << "\n";
     close(sock);
     printf("miau3 %s", buf);
     sock2 = socket(AF_UNIX, SOCK_STREAM, 0);
       if (sock2 < 0)
       {
         perror("opening stream socket");
         exit(1);
       }

       server.sun_family = AF_UNIX;
       strcpy(server.sun_path, (const char*)buf);
       printf("server addres = %s", server.sun_path);
       if (connect(sock2, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0)
       {
         close(sock2);
         perror("connecting stream socket");
         exit(1);
       }

       if (write(sock2, "InitOk", sizeof("InitOk")) < 0)
       {
         perror("writing on stream socket");
         close(sock2);
       }
       close(sock2);
     unlink(NAME);
}
