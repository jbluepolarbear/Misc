#pragma once

#include <string>
#include <winsock2.h>

std::string  GetAddressString(const sockaddr_in *addr);
int          Check(int errCode);
std::string  GetByteString(const char *buffer, unsigned bufferLen);

