#ifndef _SOCKET_LIBRARY_
#define _SOCKET_LIBRARY_

#ifdef _WIN32
#include <Ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#include <Windows.h>
#pragma warning(disable:4996)
#endif

namespace core
{

class SocketLibrary
{
  public :

    static void initialise()
    {
#ifdef _WIN32
        WORD version = MAKEWORD(2, 2);
        WSADATA data;
        WSAStartup(version, &data);
#endif
    }

    static void uninitialise()
    {
#ifdef _WIN32
        WSACleanup();
#endif
    }

  private :
#ifdef _WIN32
    static WSADATA wsd;
#endif

};

#ifdef _WIN32
WSADATA SocketLibrary::wsd = { 0 };
#endif

}//namespace

#endif