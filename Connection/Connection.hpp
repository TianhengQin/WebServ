#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "Head.hpp"
# include "Server.hpp"
# include "Response.hpp"
# include "MimeType.hpp"
# include "Request.hpp"

# define CGI_SUCCEED 2
# define CGI_ON 1
# define CGI_OFF 0
# define CGI_FAILED -1
# define CGI_TIMEOUT 3

class Connection {

public:

    Connection();
    Connection(std::map<int, Server> &svs, int fd);
    ~Connection();

    void setFd(int fd);
    int getFd();

    void buildResponse();
    void setResponse(std::string &bf);

    void updateTime();

    std::time_t getTimeStamp();

    void receive(char const *bf, size_t rcvd);
    int send();
    bool session();
    int cgiState();
    void setCgiState(int s);
    void buildCgiResponse(std::string const &bd);
    std::string getCookie();
    std::string &getCgiProgram();
    std::string &getCgiScript();
    std::string &getCgiSendBf();
    std::string getUri();
    std::string getHost();
    std::string getMethod();
    std::string getQuery();
    std::size_t getBodyLen();
    std::string getMimeType();
    std::string getServName();
    std::string getPort();

    void    setCgiProgram(std::string pathToCgi);
    void    setCgiScript(std::string realPath);
    void    setCgiSendBf(std::string bf);

private:

    void chooseServer();

    int saveFile(std::string const &file);

    std::map<int, Server> _server;
    int _servChoice;
    int _locationChoice;

    Request _quest;
    Response _sponse;

    MimeType _mmty;

    std::time_t _timeStamp;

    std::string _sendBf;
    std::string _cgiSendBf;

    int _fd;

    bool _keepAlive;

    int _cgiState;

    std::string _cgiProgram;
    std::string _cgiScript;
};

#endif
