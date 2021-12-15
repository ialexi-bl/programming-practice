#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>

struct AddrInfo {
  private:
    addrinfo *addrInfo = nullptr;

  public:
    inline int ai_family() const
    {
        return addrInfo->ai_family;
    }
    inline int ai_socktype() const
    {
        return addrInfo->ai_socktype;
    }
    inline int ai_protocol() const
    {
        return addrInfo->ai_protocol;
    }
    inline int ai_addrlen() const
    {
        return addrInfo->ai_addrlen;
    }
    inline const sockaddr *ai_addr() const
    {
        return addrInfo->ai_addr;
    }
    inline const sockaddr_in *ai_addr_in() const
    {
        return reinterpret_cast<const sockaddr_in *>(addrInfo->ai_addr);
    }

    AddrInfo(const char *hostname, const std::string &servname,
             const addrinfo &hints);
    ~AddrInfo();
};