#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../Head.hpp"

# define GET 1
# define POST 2
# define DELETE 4
# define PUT 8
# define HEAD 16

class Location {

public:

private:

    std::string _path;
    std::string _root;
    std::string _index;
    std::string _redir;

    int _methods; // 1 GET 2 POST 4 DELETE 8 PUT 16 HEAD

    bool _dirListing;

};

#endif
