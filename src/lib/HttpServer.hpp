#pragma once
#include "NetworkStream.hpp"
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <string>

struct Request;
struct Response;
struct FileResponse;
class HttpError;

using RequestHandler = std::unique_ptr<Response> (*)(const Request &request);

struct Request {
  public:
    std::string method;
    std::string path;
    std::string body;
    std::map<std::string, std::string> headers;
};

struct Response {
  public:
    const int status;
    const std::string message;
    std::map<std::string, std::string> headers;

    Response();
    Response(const std::map<std::string, std::string> &headers);
    Response(const int status, const std::string &message,
             const std::map<std::string, std::string> &headers);
    virtual ~Response() = default;

    virtual std::ostream &writeBody(std::ostream &stream) const = 0;
};

struct FileResponse : public Response {
  private:
    std::filesystem::path path;

  public:
    FileResponse(const std::filesystem::path &path);

    virtual std::ostream &writeBody(std::ostream &stream) const override;
};

class HttpServer
{
  private:
    int masterSocketFd;

    void serve(RequestHandler handler) const;
    void handle(NetworkStream &stream, RequestHandler handler) const;

  public:
    HttpServer(const std::string &port, RequestHandler handler);
    ~HttpServer();
};

std::ostream &operator<<(std::ostream &stream, const HttpError &error);
std::ostream &operator<<(std::ostream &stream, const Request &request);
std::ostream &operator<<(std::ostream &stream, const Response &request);

class HttpError : std::exception
{
  public:
    const bool closeConnection;
    const int status;
    const std::string message;

    HttpError(int status, std::string message, bool closeConnection = false);
};

class InvalidRequestError : public HttpError
{
  public:
    InvalidRequestError();
};

class BadRequestError : public HttpError
{
  public:
    BadRequestError();
};

class NotFoundError : public HttpError
{
  public:
    NotFoundError();
};

class MethodNotAllowedError : public HttpError
{
  public:
    MethodNotAllowedError();
};

class HttpVersionNotSupportedError : public HttpError
{
  public:
    HttpVersionNotSupportedError();
};