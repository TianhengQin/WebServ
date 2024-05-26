
# include "Head.hpp"
# include "Webserver.hpp"
# include "Response.hpp"
# include "Connection.hpp"
# include "MimeType.hpp"
# include "Cgi.hpp"
# include "Log.hpp"
# include "Server.hpp"
# include "Configuration.hpp"
# include "Location.hpp"


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}

	int watch_dog = 1000;
	while (--watch_dog) {
		try {
			Log::print(INFO, "Server Restarting ...", 0);

			Configuration config(argv[1]);

			// std::vector<Server> servers = config.getServers();
			// for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
			// 	Log::print(INFO, "Server ", it->getPort());
			// 	std::vector<Location> locations = it->getLocations();
			// 	for (std::vector<Location>::iterator it2 = locations.begin(); it2 != locations.end(); ++it2) {
			// 		// Log::print(INFO, "Location ", it2->getPath());
			// 		std::cout << "Location " << it2->getPath() << std::endl;
			// 		unsigned int i = it2->getAllowedMethods();
			// 		for (int j = 0; j < 5; j++) {
			// 			if (i & (1 << j)) {
			// 				Log::print(INFO, "Method ", 1);
			// 			} else {
			// 				Log::print(INFO, "Method ", 0);
			// 			}
			// 		}
			// 	}
			// }
			WebServ web_serv(config);
			web_serv.run();
		} catch (std::exception &e) {
			Log::print(ERROR, e.what(), 0);
			Log::print(WARNING, "Watch Dog ", watch_dog);
		}
	}
	return 1;
}
