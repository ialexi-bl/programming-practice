#include "HttpServer.hpp"
#include "AddrInfo.hpp"
#include "NetworkStream.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <filesystem>
#include <fstream>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

HttpServer::HttpServer(const std::string &port, RequestHandler handler)
{
    const addrinfo hints = {.ai_family = AF_UNSPEC,
                            .ai_socktype = SOCK_STREAM,
                            .ai_flags = AI_PASSIVE};
    const AddrInfo addrInfo(nullptr, port, hints);

    masterSocketFd = socket(addrInfo.ai_family(), addrInfo.ai_socktype(),
                            addrInfo.ai_protocol());
    if (masterSocketFd < 0) {
        throw std::runtime_error(std::string("Unable to open socket: \"") +
                                 std::strerror(errno) + '"');
    }

    int yes = 0;
    if (setsockopt(masterSocketFd, SOL_SOCKET, SO_REUSEADDR, &yes,
                   sizeof(yes)) == -1) {
        throw std::runtime_error(
            std::string("Unable to set socket option: \"") +
            std::strerror(errno) + '"');
    }

    if (bind(masterSocketFd, addrInfo.ai_addr(), addrInfo.ai_addrlen()) < 0) {
        throw std::runtime_error(std::string("Unable to bind to port: \"") +
                                 std::strerror(errno) + '"');
    }

    if (listen(masterSocketFd, 5) < 0) {
        throw std::runtime_error(std::string("listen() call failed: \"") +
                                 std::strerror(errno) + '"');
    }

    serve(handler);
}

HttpServer::~HttpServer()
{
    close(masterSocketFd);
}

void HttpServer::serve(RequestHandler handler) const
{
    while (true) {
        sockaddr addr;
        socklen_t addrSize = sizeof(addr);

        int socketFd = accept(masterSocketFd,
                              reinterpret_cast<sockaddr *>(&addr), &addrSize);

        if (socketFd < 0) {
            std::cout << "> Unable to accept connection: \""
                      << std::strerror(errno) << '"' << std::endl;
            continue;
        }

        std::cout << "> Accepted connection";
        sockaddr peerAddr;
        socklen_t addrlen = sizeof(peerAddr);

        if (getpeername(socketFd, &peerAddr, &addrlen) == -1) {
            std::cout << ", unable to get info about peer ("
                      << std::strerror(errno) << ')' << std::endl;
        } else {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, reinterpret_cast<sockaddr_in *>(&peerAddr), ip,
                      INET_ADDRSTRLEN);
            std::cout << " from " << ip << std::endl;
        }

        NetworkStream stream(socketFd);
        handle(stream, handler);

        std::cout << "> Closing connection" << std::endl;
    }
}

static std::istream &ignorews(std::istream &stream)
{
    while (stream.peek() == ' ') {
        stream.get();
    }
    return stream;
}

void HttpServer::handle(NetworkStream &stream, RequestHandler handler) const
{
    Request request;
    while (true) {
        try {
            if (stream.peek() == DISCONNECTED) {
                return;
            }

            request = {};

            std::cout << "> Reading method" << std::endl;
            stream >> request.method;

            if (stream.peek() != ' ') {
                throw InvalidRequestError();
            }

            std::cout << "> Reading headers" << std::endl;
            stream >> request.path;
            if (stream.peek() == ' ') {
                std::string httpVersion;
                stream >> httpVersion;
                if (httpVersion != "HTTP/1.1") {
                    throw HttpVersionNotSupportedError();
                }
            }
            if (stream.get() != '\r' || stream.get() != '\n') {
                throw InvalidRequestError();
            }

            // Headers
            while (true) {
                if (stream.peek() == '\r') {
                    stream.get();
                    if (stream.get() == '\n') {
                        break;
                    }

                    throw InvalidRequestError();
                }

                std::string headerName{""};
                char t;
                for (t = stream.get(); t != ':' && t != '\r';
                     t = stream.get()) {
                    headerName += tolower(t);
                }

                if (t == '\r')
                    throw InvalidRequestError();
                if (stream.get() != ' ')
                    throw InvalidRequestError();

                stream >> ignorews;

                std::string headerValue{""};
                while (true) {
                    for (char t = stream.get(); t != '\r'; t = stream.get()) {
                        headerValue += t;
                    }

                    if (stream.get() != '\n') {
                        throw InvalidRequestError();
                    }
                    if (stream.peek() != ' ') {
                        break;
                    }

                    headerValue += '\n';
                    stream >> ignorews;
                }

                auto &&it = request.headers.find(headerName);
                if (it != request.headers.end()) {
                    it->second += ", ";
                    it->second += headerValue;
                } else {
                    request.headers[headerName] = headerValue;
                }
            }

            auto &&it = request.headers.find("content-length");
            if (it != request.headers.end()) {
                int contentLength = std::stoi(it->second);
                char *body = new char[contentLength + 1];
                stream.read(body, contentLength);
                body[contentLength] = '\0';
            }

            std::unique_ptr<Response> response = handler(request);
            stream << *response.get() << std::flush;

        } catch (const HttpError &e) {
            stream << e;

            if (e.closeConnection) {
                return;
            }
        }

        auto &&it = request.headers.find("connection");
        if (it != request.headers.end() && it->second == "close") {
            return;
        }
    }
}

std::ostream &operator<<(std::ostream &stream, const HttpError &error)
{
    stream << "HTTP/1.1 " << error.status << ' ' << error.message << net::endl
           << "Content-Length: 0" << net::endl
           << "Connection: " << (error.closeConnection ? "close" : "keep-alive")
           << net::endl
           << "Content-Type: text/plain" << net::endl
           << net::endl
           << std::flush;
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const Request &request)
{
    stream << "> Received request" << std::endl
           << "- Method: \"" << request.method << '"' << std::endl
           << "- Path: \"" << request.path << '"' << std::endl;

    stream << "- Body: " << std::endl;
    if (request.body.length()) {
        stream << request.body;
    } else {
        stream << "  Empty" << std::endl;
    }
    stream << std::endl;
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const Response &response)
{
    std::cout << "> Writing response" << std::endl;
    stream << "HTTP/1.1 " << response.status << ' ' << response.message
           << net::endl;

    for (auto &&it : response.headers) {
        stream << it.first << ": " << it.second << net::endl;
    }
    stream << net::endl;
    response.writeBody(stream);

    return stream;
}

Response::Response() : status(200), message("OK")
{
}
Response::Response(const std::map<std::string, std::string> &headers)
    : status(200), message("OK"), headers(headers)
{
}
Response::Response(const int status, const std::string &message,
                   const std::map<std::string, std::string> &headers)
    : status(status), message(message), headers(headers)
{
}

static std::string HTML_EXTENSION = ".html";
static std::string JPEG_EXTENSION = ".jpg";

static std::map<std::string, std::string>
getFileResponseHeaders(const std::filesystem::path &path)
{
    std::map<std::string, std::string> headers;
    headers["Content-Length"] =
        std::to_string(std::filesystem::file_size(path));

    std::string pathStr = path;
    if (std::equal(pathStr.end() - HTML_EXTENSION.size(), pathStr.end(),
                   HTML_EXTENSION.begin())) {
        headers["Content-Type"] = "text/html";
    } else if (std::equal(pathStr.end() - JPEG_EXTENSION.size(), pathStr.end(),
                          JPEG_EXTENSION.begin())) {
        headers["Content-Type"] = "image/jpeg";
    } else {
        headers["Content-Type"] = "text/plain";
    }
    return headers;
}

FileResponse::FileResponse(const std::filesystem::path &path)
    : Response(getFileResponseHeaders(path)), path(path)
{
}

std::ostream &FileResponse::writeBody(std::ostream &stream) const
{
    std::ifstream fileStream(path);

    if (!fileStream) {
        throw NotFoundError();
    }

    while (fileStream.peek() != EOF) {
        stream << static_cast<char>(fileStream.get());
    }

    return stream;
}

HttpError::HttpError(int status, std::string message, bool closeConnection)
    : closeConnection(closeConnection), status(status), message(message)
{
}
InvalidRequestError::InvalidRequestError() : HttpError(400, "Bad Request", true)
{
}
BadRequestError::BadRequestError() : HttpError(400, "Bad Request")
{
}
NotFoundError::NotFoundError() : HttpError(404, "Not Found")
{
}
MethodNotAllowedError::MethodNotAllowedError()
    : HttpError(405, "Method Not Allowed")
{
}
HttpVersionNotSupportedError::HttpVersionNotSupportedError()
    : HttpError(505, "HTTP Version Not Supported")
{
}