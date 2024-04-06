#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

# include "../Head.hpp"

class MimeType {

public:

    MimeType();
    ~MimeType();

    std::string &getType(std::string const &sufix);

private:

    std::map<std::string, std::string> _mmty;

};

MimeType mm_ty;

#endif
