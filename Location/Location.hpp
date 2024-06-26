#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Head.hpp"

# define GET 1
# define POST 2
# define DELETE 4
# define PUT 8
# define HEAD 16


class Location {

public:

	Location(void);
	Location(Location const &loc);
	Location &operator=(Location const &loc);
	~Location();

	/* Getters for private members */
	std::string							getPath(void);
	std::string							getRoot(void);
	std::string							getIndex(void);
	std::map<int, std::string>			getErrorPages(void);
	unsigned int						getAllowedMethods(void);
	unsigned int						getClientMaxBodySize(void);
	bool								getAutoindex(void);
	std::string							getAlias(void);
	std::map<std::string, std::string>	getCgi(void);
	std::string							getRedir(void);

	// Special getters
	bool			hasErrorPage(int code);
	bool			hasCgi(std::string ext);
	std::string		getErrorPage(int code);
	std::string		getCgi(std::string ext);
	bool			hasMethod(unsigned int method);

	/* Setters for private members */
	void	setPath(std::string path);
	void	setRoot(std::string root);
	void	addIndex(std::string index);
	void	setErrorPage(int code, std::string path);
	void	setAllowedMethods(unsigned int methods);
	void	setClientMaxBodySize(unsigned int cmb);
	void	setAutoindex(bool autoindex);
	void	setAlias(std::string alias);
	void	setCgi(std::string ext, std::string bin);
	void	setRedir(std::string redir);

	bool	operator==(Location const &loc) const;
	bool	operator!=(Location const &loc) const;

private:

	std::string							_path; // URI
	std::string							_root;
	std::string							_index;
	std::map<int, std::string>			_error_page;
	unsigned int						_methods;
	unsigned int						_client_max_body_size;
	bool								_autoindex;
	std::string							_alias;
	std::map<std::string, std::string>	_cgi;
	std::string							_return;

};

std::ostream &operator<<(std::ostream &out, Location &loc);

#endif
