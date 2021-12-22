#pragma once
#include <fstream>
#include <functional>

constexpr int BUFFER_SIZE = 1024;

class NetworkStream;

namespace net
{
std::ostream &endl(std::ostream &stream);
std::ostream &endrequest(std::ostream &stream);
} // namespace net

class NetworkStreamBuffer : public std::streambuf
{
  private:
    bool disconnected = false;
    int socketFileDescriptor;
    char *buffer;

    NetworkStreamBuffer();

  public:
    NetworkStreamBuffer(const char *hostname);
    NetworkStreamBuffer(int socketFd);
    ~NetworkStreamBuffer();

    virtual std::streambuf::int_type underflow();
    virtual std::streambuf::int_type overflow(std::streambuf::int_type value);
    virtual int sync();

    friend class NetworkStream;
};

class NetworkStream : public std::iostream
{
  public:
    NetworkStream(const char *hostname);
    NetworkStream(int socketFd);
    virtual ~NetworkStream();

    bool disconnected();
    int socket();
};
