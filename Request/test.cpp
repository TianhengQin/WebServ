#include "Request.hpp"

int main() {
    std::string raw_request = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: test\r\n\r\n";
    Request request(raw_request);

    if (1) {
        std::cout << "Method: " << request.get_method() << std::endl;
        std::cout << "Path: " << request.get_dir() << std::endl;
        std::cout << "HTTP Version: " << request.get_protocol() << std::endl;
        
        std::map<std::string, std::string> headers = request.get_headers();
        for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
            std::cout << it->first.length() << ": " << it->second.length() << std::endl;
            std::cout << it->first << ": " << it->second << std::endl;
        }
        
        // std::cout << "Body: " << request.get_body() << std::endl;
    } else {
        std::cout << "Invalid request" << std::endl;
    }

    return 0;
}