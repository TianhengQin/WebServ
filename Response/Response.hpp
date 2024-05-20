#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Head.hpp"
# include "MimeType.hpp"
# include "../Request/Request.hpp"
#include "Server.hpp"

class Server;
class Response {

public:

    Response();
    ~Response();

    void init(Request &request, Server server, Location location);
    void initResponsePhrase();
    std::string getResponsePhrase(int const &sufix);
    void clear();

    std::string generate();

    void    getMethod(Request &request);
    void    postMethod(Request &request);
    void    deleteMethod(Request &request);

    int setBody(std::string const &file);
    void setMimeType(std::string const &path);
    void setAutoindex(std::string const &path);

private:

    int _method;

    int _code;
    std::string _phrase;
    std::string _body;

    MimeType _mmTy;

    std::string _mimeType;

    // std::size_t bodySize;

    std::map<int, std::string> _responsePhrase;


    Server      _server;
    Location    _location;

};

#endif
