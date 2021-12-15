#include "lib/HttpServer.hpp"
#include <filesystem>
#include <iostream>

int main()
{
    // std::filesystem::path p{"abc"};

    // std::cout << std::boolalpha << std::filesystem::file_size(p) <<
    // std::endl; return 0;

    HttpServer server("80", [](const Request &request) {
        std::cout << request;

        if (request.method != "GET") {
            throw MethodNotAllowedError();
        }

        std::filesystem::path path{"." + request.path};
        if (std::filesystem::is_directory(path)) {
            path /= "index.html";
        }

        if (!std::filesystem::is_regular_file(path)) {
            throw NotFoundError();
        }

        return std::unique_ptr<Response>(new FileResponse(path));
    });
}