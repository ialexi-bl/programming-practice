#include "lib/NetworkStream.hpp"
#include <iostream>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    NetworkStream stream("localhost");

    stream << "GET / HTTP/1.1" << net::endl
           << "Host: localhost" << net::endl
           << "Connection: close" << net::endrequest << std::flush;

    while (stream.peek() != DISCONNECTED && stream.peek() != EOF) {
        std::cout << static_cast<char>(stream.get());
    }
    std::cout << std::endl;
}