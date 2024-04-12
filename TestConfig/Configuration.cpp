#include "Configuration.hpp"

Configuration::Configuration() {
    Server s1;
    s1.setHost("127.0.0.1");
    s1.setPort(8080);
    s1.setServName("127.0.0.1");
    s1.setErrPage(301, "./websites/err_page/301.html");
    s1.setErrPage(404, "./websites/err_page/404.html");
    s1.setCliMaxBody(1024*1024);
    s1.setRoot("./websites");
}

Configuration::~Configuration() {}


std::vector<Server> &Configuration::getServs(void) {
    return _servs;
}
