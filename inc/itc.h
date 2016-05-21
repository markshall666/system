#ifndef ITC_H
#define ITC_H

bool initItc(const char* name, int* fd);

bool sendData(int sender, const char* receiver, void* buf, int numOfBytes);

int receiveData(int sock, void* buf);

void terminateItc(int fd);

#endif
