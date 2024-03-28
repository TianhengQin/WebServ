#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "head.hpp"

class Location {

public:

private:

    std::string _path;
    std::string _root;
    std::string _index;
    std::string _redir;
    std::string _dirFile;

    int _methods; // 1 GET 2 POST 4 DELETE 8 PUT 16 HEAD

    bool _dirListing;

};

#endif
