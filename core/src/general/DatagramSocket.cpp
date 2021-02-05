#include "DatagramSocket.h"

namespace udpcom {

#ifdef __WIN32__
	typedef int socklen_t;
#endif

DatagramSocket::DatagramSocket(int port, bool broadcast, bool reusesock) {

#ifdef __WIN32__
    retval = WSAStartup(MAKEWORD(2,2),&wsaData);
#endif

    sockaddr_in addr;
    sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    //set up bind address
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    //set up address to use for sending
    memset(&outaddr,0,sizeof(outaddr));
    outaddr.sin_family = AF_INET;
    //outaddr.sin_addr.s_addr = inet_addr(address);
    //outaddr.sin_port = htons(port);

#ifdef __WIN32__
    bool bOptVal = 1;
    int bOptLen = sizeof(bool);
#else
    int OptVal = 1;
#endif

    if (broadcast)
#ifdef __WIN32__
        retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
#else
        retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &OptVal, sizeof(OptVal));
#endif

    if (reusesock)
#ifdef __WIN32__
        retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen);
#else
        retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &OptVal, sizeof(OptVal));
#endif

    retval = bind(sock,(struct sockaddr *)&addr,sizeof(addr));
}

DatagramSocket::~DatagramSocket() {close();}

int DatagramSocket::getAddress(const char * name, char * addr) {
    struct hostent *hp;
    if ((hp = gethostbyname(name)) == NULL) return (0);
    strcpy(addr, inet_ntoa(*( struct in_addr*)( hp->h_addr)));
    return (1);
}

const char* DatagramSocket::getAddress(const char * name) {
    struct hostent *hp;
    if ((hp = gethostbyname(name)) == NULL) return (0);
    strcpy(ip, inet_ntoa( *( struct in_addr*)( hp->h_addr)));
    return ip;
}

long DatagramSocket::receive(char* msg, int msgsize, char *address, int &port) {
    struct sockaddr_in sender;
    socklen_t sendersize = sizeof(sender);
    int retval = recvfrom(sock,msg,msgsize,0, (struct sockaddr *)&sender, &sendersize);
    if(retval>-1) {
        strcpy(address,inet_ntoa(sender.sin_addr));
        port = ntohs(sender.sin_port);
    }
    return retval;
}

long DatagramSocket::sendTo(const char* msg, int msgsize, const char* addr, int port) {
    outaddr.sin_addr.s_addr = inet_addr(addr);
    outaddr.sin_port = htons(port);
    return sendto(sock, msg, msgsize, 0, (struct sockaddr *)&outaddr,sizeof(outaddr));
}

void DatagramSocket::close() {
#if __WIN32__
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
}

}
