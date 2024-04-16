#include "Request.hpp"

Request::Request() {}

Request::Request(std::string str)
{
	clear();
	_request = str;
	parse();
}

Request::~Request() {}

void Request::setFinish() {
    _finish = true;
}

void Request::append(char const *bf, size_t n) {
    _request.append(bf, n);
}

std::string &Request::get() {
    return _request;
}

void Request::init(std::string str)
{
	clear();
	_request = str;
	parse();
}

void Request::clear()
{
	_request = "";
	// bool _finish; ???
	_body = "";

	_bad = 0;
	_method = 0;
	_dir = "";
	_protocol = "";
	_host = "";
	_port = "";
	_cookie = "";
	// std::vector<std::string> _accept; ???
}

void Request::parse()
{
	std::size_t pos;

	// parse _method
	pos = 4;
	if (_request.compare(0, pos, "GET ") == 0)
		_method = GET;
	else if (_request.compare(0, pos, "PUT ") == 0)
		_method = PUT;
	else if (_request.compare(0, ++pos, "POST ") == 0)
		_method = POST;
	else if (_request.compare(0, pos, "HEAD ") == 0)
		_method = HEAD;
	else if (_request.compare(0, 7, "DELETE ") == 0)
	{
		_method = DELETE;
		pos = 7;
	}
	else
	{
		_bad = 400; // error ?
		return ;
	}

	// parse _dir
	_dir = _request.substr(pos, _request.find("HTTP") - (pos + 1));
	if (_dir.find_first_of('/', 0) != 0)
	{
		_bad = 400; // error ?
		return ;
	}

	// parse _protocol
	pos = _request.find("HTTP");
	_protocol = _request.substr(pos, _request.find('\r') - pos);
	if (_protocol.compare("HTTP/1.1") != 0)
	{
		_bad = 505; // error ?
		return ;
	}


	// parse _host
	pos = _request.find("Host: ") + 6;
	if (pos == std::string::npos)
	{
		_bad = 400; // error ?
		return ;
	}
	_host = _request.substr(pos, _request.find_first_of(':', pos) - pos);

	// parse _port
	pos = _request.find_first_of(':', pos) + 1;
	_port = _request.substr(pos, _request.find_first_of('\r', pos) - pos);
	for (size_t i = 0; _port[i] != '\0'; i++)
	{
		if (!isdigit(_port[i]))
		{
			_bad = 400; // error ?
			return ;
		}
	}

	// parse _cookie ???
	pos = _request.find("Cookie: ");
	if (pos != std::string::npos)
	{
		pos += 8;
		_cookie = _request.substr(pos, _request.find_first_of('\r', pos) - pos);
	}

	// pos = _request.find("/r/n/r/n");
	// if (pos == std::string::npos)
	// {
	// 	_bad = 400; // error ?
	// 	ft_test();
	// 	return ;
	// }
	// _body = _request.substr(pos);
	


	// // test printing
	// std::cout << _method << std::endl;
	// std::cout << _dir << std::endl;
	// std::cout << _protocol << std::endl;
	// std::cout << _host << std::endl;
	// std::cout << _port << std::endl;
	// std::cout << _cookie << std::endl;
	// std::cout << _bad << std::endl;
	// std::cout << _body << std::endl;
	// std::cout << std::endl;
}

// getters
int	Request::get_method()
{
	return (_method);
}

std::string Request::get_dir()
{
	return (_dir);
}

std::string Request::get_protocol()
{
	return (_protocol);
}

std::string Request::get_host()
{
	return (_host);
}

std::string Request::get_port()
{
	return (_port);
}
