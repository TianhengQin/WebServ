#include "Request.hpp"

Request::Request() {
	_cookie = "abcde";
}

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

void Request::append(char const *bf, size_t n) { // ??
    _request.append(bf, n);
}

std::string Request::get() {
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
	// std::vector<std::string> _accept; ???
}

void Request::parse()
{
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

	// Parse body
	if (request_stream.good()) {
		std::ostringstream body_stream;
		body_stream << request_stream.rdbuf();
		_body = body_stream.str();
	}

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
		// std::cout << "host: " << _host << std::endl;
		// std::cout << "port: " << _port << std::endl;
	}

	// Parse _cookies
	it = _headers.find("Cookie");
	if (it != _headers.end()) {
		_cookie = it->second;
	}


	_bad = validate_request();

	if (_bad == 200) {
		setFinish();
	}

	// // // test printing
	// // std::cout << _method << std::endl;
	// // std::cout << _dir << std::endl;
	// // std::cout << _protocol << std::endl;
	// // std::cout << _host << std::endl;
	// // std::cout << _port << std::endl;
	// // std::cout << _cookie << std::endl;
	// // std::cout << _bad << std::endl;
	// // std::cout << _body << std::endl;
	// // std::cout << std::endl;
}

int Request::validate_request()
{
	// validate method
	if (_method != "GET" && _method != "PUT" && _method != "POST" && _method != "HEAD" && _method != "DELETE") {
		return (400);
	}

	// validate HTTP protocol
	if (_protocol != "HTTP/1.1") {
		return (505);
	}

	// validate Host
	if (_host != "localhost" && _host != "127.0.0.1") {  // is that fine?
		return (400);
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
int	Request::get_method()
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
	return (-1);
}

std::string Request::get_dir()
{
	std::cout << _dir.length() << std::endl;
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
	return (_body);
}

int         Request::get_bad()
{
	return (_bad);
}


std::map<std::string, std::string> Request::get_headers()
{
	return (_headers);
}
