#include "Server/Server.hpp"
#include <sys/socket.h>


int main() {

    TcpServer serv("127.0.0.1", 8081);

    serv.beginListen();

    return 0;
}
