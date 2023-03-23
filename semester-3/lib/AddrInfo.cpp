#include "AddrInfo.hpp"

AddrInfo::AddrInfo(const char *hostname, const std::string &servname, const addrinfo &hints)
{
    if (hostname) {
        std::cout << "Retrieving host info for \"" << hostname << '"' << std::endl;
    }

    int status = getaddrinfo(hostname, servname.c_str(), &hints, &addrInfo);
    if (status) {
        throw std::runtime_error(gai_strerror(status));
    }
}

AddrInfo::~AddrInfo()
{
    freeaddrinfo(addrInfo);
}