#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Head.hpp"

class Request {

public:

    Request();
    ~Request();

    void init();
    void parse();

    void setFinish();

    void append(char const *bf);

private:

    std::string _request;
    bool _finish;
    std::string _body;

    bool _bad;
    int _method;
    std::string _dir;
    std::string _protocol;
    std::string _host;
    std::string _port;
    std::string _cookie;
    std::vector<std::string> _accept;

};

#endif
