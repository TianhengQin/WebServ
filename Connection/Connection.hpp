#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "head.hpp"

class Connection {

public:

    Connection(Server const &sv);
    ~Connection();

    void setFd(int fd);

private:

    MimeType _mime;

    Request _quest;
    Response _sponse;

    int fd;
};

#endif
