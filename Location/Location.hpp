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

    Location();
    Location(Location const &loc);
    Location &operator=(Location const &loc);
    ~Location();

    std::string &getPath();
    std::string &getRoot();
    std::string &getIndex();
    std::string &getRedir();
    int getMethods();
    bool getDirListing();

    void setPath(std::string const pth);
    void setRoot(std::string const rt);
    void setIndex(std::string const idx);
    void setRedir(std::string const rdr);
    void setMethods(int mtd);
    void setDirListing(bool dl);

private:

    std::string _path;
    std::string _root;
    std::string _index;
    std::string _redir;

    int _methods; // 1 GET 2 POST 4 DELETE 8 PUT 16 HEAD

    bool _dirListing;

};

#endif
