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

	std::string	getPath(void);
	std::string	getRoot(void);
	std::string	getIndex(void);
	std::string	getRedir(void);
	int			getMethods(void);
	bool		getDirListing(void);

	std::map<std::string, std::string>	getCgi(void);


	void	setPath(std::string pth);
	void	setRoot(std::string rt);
	void	setIndex(std::string idx);
	void	setRedir(std::string rdr);
	void	setMethods(int mtd);
	void	setDirListing(bool dl);

	void	setCgi(std::string ext, std::string bin);

private:

	std::string	_path;
	std::string	_root;
	std::string	_index;
	std::string	_redir;

	int		_methods; // 1 GET 2 POST 4 DELETE 8 PUT 16 HEAD
	bool	_dirListing;

	std::map<std::string, std::string>	_cgiMap;

};

std::ostream &operator<<(std::ostream &out, Location &loc);

#endif
