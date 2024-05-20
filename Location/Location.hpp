#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Head.hpp"

# define GET 1
# define POST 2
# define DELETE 4
# define PUT 8
# define HEAD 16

// class Server;

class Location {

public:

	Location(void);
	Location(Location const &loc);
	Location &operator=(Location const &loc);
	~Location();

	// Location(Server &context);

	std::string	getPath(void);
	std::string	getRoot(void);
	std::string	getIndex(void);
	std::string	getRedir(void);
	unsigned int	getMethods(void);
	bool			getAutoindex(void);
	unsigned int	getClientMaxBodySize(void);

	std::map<int, std::string>	getErrorPages(void);
	std::map<std::string, std::string>	getCgi(void);


	void	setPath(std::string path);
	void	setRoot(std::string root);
	void	setIndex(std::string index);
	void	setRedir(std::string redir);

	void	setMethods(unsigned int methods);
	void	setAutoindex(bool autoindex);
	void	setClientMaxBodySize(unsigned int cmb);
	void	setErrorPage(int code, std::string path);

	void	setCgi(std::string ext, std::string bin);

private:

	std::string	_path;
	std::string	_root;
	std::string	_index;
	std::string	_redir;

	unsigned int	_methods; // 1 GET 2 POST 4 DELETE 8 PUT 16 HEAD
	bool			_autoindex;
	unsigned int	_client_max_body_size;		// client_max_body_size
	std::map<int, std::string>	_error_page;

	std::map<std::string, std::string>	_cgiMap;

};

std::ostream &operator<<(std::ostream &out, Location &loc);

#endif
