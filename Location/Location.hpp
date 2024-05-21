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

	std::string		getPath(void);
	std::string		getRoot(void);
	std::string		getIndex(void);
	std::string		getRedir(void);
	unsigned int	getAllowedMethods(void);
	bool			getAutoindex(void);
	unsigned int	getClientMaxBodySize(void);

	std::map<int, std::string>	getErrorPages(void);
	std::map<std::string, std::string>	getCgi(void);


	void	setPath(std::string path);
	void	setRoot(std::string root);
	void	addIndex(std::string index);
	void	setRedir(std::string redir);

	void	setAllowedMethods(unsigned int methods);
	void	setAutoindex(bool autoindex);
	void	setClientMaxBodySize(unsigned int cmb);
	void	setErrorPage(int code, std::string path);

	void	setCgi(std::string ext, std::string bin);

private:

	std::string		_path; // URI
	std::string		_root;
	std::string		_index;
	std::map<int, std::string>	_error_page;
	unsigned int	_methods;
	unsigned int	_client_max_body_size;
	std::string		_return;
	bool			_autoindex;
	std::map<std::string, std::string>	_cgi;

};

std::ostream &operator<<(std::ostream &out, Location &loc);

#endif
