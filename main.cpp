
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
			std::cout << config << std::endl;

			WebServ web_serv(config);
			web_serv.run();
		} catch (std::exception &e) {
			Log::print(ERROR, e.what(), 0);
			Log::print(WARNING, "Watch Dog ", watch_dog);
		}
	}
	return 1;
}
