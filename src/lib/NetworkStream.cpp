#include "NetworkStream.hpp"
#include "AddrInfo.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

NetworkStreamBuffer::NetworkStreamBuffer() : buffer(new char[BUFFER_SIZE])
{
}

NetworkStreamBuffer::NetworkStreamBuffer(const char *hostname)
    : NetworkStreamBuffer()
{
    setg(0, 0, 0);
    setp(buffer, buffer + BUFFER_SIZE);

    const addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
    };
    const AddrInfo addrInfo(hostname, "http", hints);

    std::cout << "> Opening socket..." << std::endl;
    socketFileDescriptor = socket(addrInfo.ai_family(), addrInfo.ai_socktype(),
                                  addrInfo.ai_protocol());

    if (socketFileDescriptor < 0) {
        throw std::runtime_error("Unable to open socket");
    }

    if (connect(socketFileDescriptor, addrInfo.ai_addr(),
                addrInfo.ai_addrlen()) < 0) {
        throw std::runtime_error("Unable to connect");
    }
    std::cout << "> Connected" << std::endl;
}

NetworkStreamBuffer::NetworkStreamBuffer(int socketFd) : NetworkStreamBuffer()
{
    socketFileDescriptor = socketFd;
}

NetworkStreamBuffer::~NetworkStreamBuffer()
{
    close(socketFileDescriptor);
    delete[] buffer;
}

std::streambuf::int_type NetworkStreamBuffer::underflow()
{
    int bytes = recv(socketFileDescriptor, buffer, BUFFER_SIZE, 0);

    if (bytes < 0) {
        std::cout << "Unable to receive data: \"" << std::strerror(errno) << '"'
                  << std::endl;
        return traits_type::eof();
    }
    if (!bytes) {
        disconnected = true;
        return traits_type::eof();
    }

    setg(buffer, buffer, buffer + bytes);
    return traits_type::to_int_type(*gptr());
}

std::streambuf::int_type
NetworkStreamBuffer::overflow(std::streambuf::int_type value)
{
    auto *from = pbase();
    int amount = pptr() - pbase();
    while (amount) {
        int written = send(socketFileDescriptor, from, amount, MSG_NOSIGNAL);
        if (written < 0) {
            if (errno == EPIPE) {
                disconnected = true;
            }
            return traits_type::eof();
        }

        from += written;
        amount -= written;
    }

    setp(buffer, buffer + BUFFER_SIZE);
    if (!traits_type::eq_int_type(value, traits_type::eof()))
        sputc(value);
    return traits_type::not_eof(value);
}

int NetworkStreamBuffer::sync()
{
    std::streambuf::int_type result = this->overflow(traits_type::eof());
    return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
}

std::ostream &net::endl(std::ostream &stream)
{
    return stream << "\r\n";
}
std::ostream &net::endrequest(std::ostream &stream)
{
    return stream << "\r\n\r\n";
}

NetworkStream::NetworkStream(const char *hostname)
    : std::iostream(new NetworkStreamBuffer(hostname))
{
}

NetworkStream::NetworkStream(int socketFd)
    : std::iostream(new NetworkStreamBuffer(socketFd))
{
}

NetworkStream::~NetworkStream()
{
    delete rdbuf();
}

int NetworkStream::socket()
{
    return dynamic_cast<NetworkStreamBuffer *>(rdbuf())->socketFileDescriptor;
}

bool NetworkStream::disconnected()
{
    return dynamic_cast<NetworkStreamBuffer *>(rdbuf())->disconnected;
}