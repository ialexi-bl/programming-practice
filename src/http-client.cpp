#include "lib/NetworkStream.hpp"
#include <iostream>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    NetworkStream stream("localhost");

    stream << "GET /.gitignore HTTP/1.1" << net::endl
           << "Host: localhost" << net::endl
           << "Connection: close" << net::endrequest << std::flush;

    std::cout << stream.rdbuf() << std::endl;
}