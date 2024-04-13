#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Head.hpp"
# include "MimeType.hpp"

class Response {

public:

    Response();
    ~Response();

    void init();
    std::string &generate();

    void setMimeType(std::string const &path);

private:

    int _code;
    std::string _phrase;
    std::string _body;

    MimeType _mmTy;

    std::string _mimeType;

};

#endif
