#pragma once

#ifdef __WIN32__
	#include <winsock2.h>
#else
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <netdb.h>
//#include <string.h>
//#define TRUE 1
//#define FALSE 0
#endif




//Simple socket class for datagrams.  Platform independent between
//unix and Windows.
class DatagramSocket {
private:

#ifdef __WIN32__
    WSAData wsaData;
    SOCKET sock;
#else
    int sock;
#endif

    long retval;
    sockaddr_in outaddr;
    char ip[30];

public:
    DatagramSocket(int port, bool Datagram, bool reusesock);
    ~DatagramSocket();
    long receive(char* msg, int msgsize, char *address, int &port);
    long sendTo(const char* msg, int msgsize, const char* name, int port);
    int getAddress(const char * name, char * addr);
    const char* getAddress(const char * name);

};

