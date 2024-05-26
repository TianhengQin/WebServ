#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Head.hpp"
# include "MimeType.hpp"
# include "../Request/Request.hpp"
# include "Server.hpp"

class Server;
class Connection;
class Response {

public:

    Response();
    ~Response();

    void init(Connection &connection, Request &request, Server &server, Location &location);
    void initResponsePhrase();
    std::string getResponsePhrase(int const &sufix);
    void clear();

    std::string generate();

    void    getMethod();
    void    postMethod(Request &request);
    void    deleteMethod();

    int setBody(std::string const &file);
    void setMimeType(std::string const &path);
    void setAutoindex(std::string const &path);
    void setCgiKey(std::string const &path);
    void set_code(int const &code);

private:

    unsigned int _method;

    int _code;
    std::string _phrase;
    std::string _body;

    MimeType _mmTy;

    std::string _mimeType;

    // std::size_t bodySize;

    std::map<int, std::string> _responsePhrase;


    std::string _realPath;
    std::string _cgi;
    Server      _server;
    Location    _location;

};

#endif
