#include <string>
#include <iostream>
#include <sstream>
#include <winsock2.h>
#include "socket_debug.h"


std::string GetAddressString(const sockaddr_in *addr)
{
  std::ostringstream stream;
  stream << inet_ntoa(addr->sin_addr) << ":"
         << ntohs(addr->sin_port);
  return stream.str();
}


int Check(int errCode)
{
  if(errCode == SOCKET_ERROR)
  {
	  int lastError = WSAGetLastError();

	  if(lastError != WSAEWOULDBLOCK)
	  {
    	std::cout << "ERROR " << lastError << std::endl;
  	}
  	else
  	{
	  	return 0;
  	}
  }

  return errCode;
}


std::string GetByteString(const char *buffer, unsigned bufferLen)
{
  std::ostringstream stream;
  stream << std::hex;
  for(unsigned i = 0; i < bufferLen; i++)
  {
    unsigned k = buffer[i] & 0x000000FF;
    stream << k << " ";
  }
  return stream.str();
}
