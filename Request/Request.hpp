#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "head.hpp"

class Request {

public:

    Request();
    ~Request();

    void init();
    void parse();

    void setFinish();

    void append(std::string const &bf);

private:

    std::string _request;
    bool _finish;

    int _method;
    std::string _dir;
    std::string _protocol;
    std::string _host;
    std::string _port;
    std::string _cookie;
    std::vector<std::string> _accept;

};

#endif
