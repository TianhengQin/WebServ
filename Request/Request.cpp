#include "Request.hpp"

Request::Request() {
	clear();
}

Request::Request(std::string &str)
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

std::string Request::get() {
    return _request;
}

void Request::init(std::string &str)
{
	clear();
	_request = str;
	parse();
}

void Request::clear()
{
    _headers.clear();
	_request = "";
	_finish = false;
	_body = "";
	_bad = 200;
	_method = "";
	_dir = "";
	_protocol = "";
	_host = "";
	_port = "";
	_cookie = "";
	_query = "";
	// std::vector<std::string> _accept; ???
}

void Request::parse()
{
	// std::vec
	std::istringstream request_stream(_request);
	std::string line;

	// Parse request line
	if (std::getline(request_stream, line)) {
		std::istringstream line_stream(line);
		if (!(line_stream >> _method >> _dir >> _protocol)) {
			_bad = 400;
			return;
		}
	} else {
		_bad = 400;
		return;
	}

	// Parse headers
	while (std::getline(request_stream, line) && line != "\r") {
		size_t pos = line.find(':');
		if (pos != std::string::npos) {
			std::string header_name = line.substr(0, pos);
			size_t val_len = line.find('\r') - (pos + 2);
			if (pos != std::string::npos && line[pos + 1] == ' ') {
				std::string header_value = line.substr(pos + 2, val_len);
				_headers[header_name] = header_value;
			}
		}
	}

	
	// request_stream.clear(); // Clear error flags
	// request_stream.seekg(0);
	// Parse body
	// size_t pos = _request.find("\r\n\r\n");
	// if (pos != std::string::npos) {
	// 	std::cout << "testtest:: " << _request.substr(0, pos + 4) << "|" << std::endl;
	// }

	if (request_stream.good()) {
		std::ostringstream body_stream;
		body_stream << request_stream.rdbuf();
		_body = body_stream.str();
	}
	// std::cout << "1body: "<< _body << "|" << _body.size() << std:: endl;


	// Parse _host && _port
	std::map<std::string, std::string>::const_iterator it = _headers.find("Host");
	if (it == _headers.end()) {
		_bad  = 400; // no Host given
		return;
	} else {
		size_t pos = it->second.find(':');
		if (pos == std::string::npos) {
			_bad  = 400; // no Host without port
			return;
		}
		_host = it->second.substr(0, pos);
		_port = it->second.substr(pos + 1);
	}

	// Parse _cookies
	it = _headers.find("Cookie");
	if (it != _headers.end()) {
		_cookie = it->second;
	}
	// _cookie = "aaaaaaaaa";
	// Parse _query
	// _query = "bbbbbbbbb";
	size_t query = _dir.find("?");
	if (query != std::string::npos) {
		_query = _dir.substr(query + 1);
		_dir = _dir.substr(0, query);
	}

	// std::cout << "query: " << _query << "---" << std::endl;
	// std::cout << "dir: " << _dir << "---" << std::endl;


	_bad = validate_request();

	if (_bad == 200) {
		setFinish();
	}
}

int Request::validate_request()
{
	// validate method
	if (_method != "GET" && _method != "PUT" && _method != "POST" && _method != "HEAD" && _method != "DELETE") {
		return (405);
	}

	// validate HTTP protocol
	if (_protocol != "HTTP/1.1") {
		return (505);
	}

	// validate Port
	for (size_t i = 0; _port[i] != '\0'; i++) { // or shoud i only allow 8080, 8081, 8082?
		if (!isdigit(_port[i])) {
			return (400);
		}
	}

	return (200);
}



// getters
unsigned int	Request::get_method()
{
	if (_method == "GET") {
		return (GET);
	} else if (_method == "POST") {
		return (POST);
	} else if (_method == "PUT") {
		return (PUT);
	} else if (_method == "HEAD") {
		return (HEAD);
	} else if (_method == "DELETE") {
		return (DELETE);
	}
	return (0);
}

std::string Request::get_dir()
{
	// std::cout << _dir.length() << std::endl;
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

std::string Request::get_cookie()
{
	return (_cookie);
}

std::string Request::get_body()
{
	// std::cout << "body: "<< _request << "|" << _body.size() << std:: endl;
	return (_body);
}

int         Request::get_bad()
{
	return (_bad);
}

std::string         Request::get_query()
{
	return (_query);
}


std::map<std::string, std::string> Request::get_headers()
{
	return (_headers);
}
