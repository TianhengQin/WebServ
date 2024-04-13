
# include "Head.hpp"
# include "WebServ.hpp"
# include "Response.hpp"
# include "Connection.hpp"
# include "MimeType.hpp"
# include "Cgi.hpp"
# include "Log.hpp"
# include "Server.hpp"
# include "Configuration.hpp"
# include "Location.hpp"
// int main(int argc, char **argv) {
//     try {
//         if (argc != 2) {
//             throw std::runtime_error("Usage: " + std::string(argv[0]) + " <config_file>");//throw invalid_argument
//         }
//         Configuration config(argv[1]);
//         // std::cout << "Configuration file: " << std::endl;
//         WebServ web_serv(config);
//         web_serv.run();
//     } catch (std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;//Log::print
//         return 1;
//     }

//     return 0;
// }


int main() {
    Configuration config;
    WebServ web_serv(config);
    web_serv.run();
    return 0;
}
