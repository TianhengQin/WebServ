#include "Response.hpp"
#include "Connection.hpp"

Response::Response() {
    initResponsePhrase();
}

Response::~Response() {}

void Response::init(Connection &connection, Request &request, Server server, Location location) {

    // config setup
    _server = server;
    _location = location;

    if (location.getClientMaxBodySize() < request.get_body().size()) {
        _code = 413;
        return ;
    }

    // replaces locationPath with root and put in _realPath right now /bla not ./bla
    _realPath = request.get_dir();
    std::string pathLocaton = location.getPath();
    _realPath.replace(0, pathLocaton.size(), location.getRoot())


    // if () // cgi??
    std::map<std::string, std::string> cgi = location.getCgi();
    setCgiKey(request.get_dir());
    if (cgi.find(_cgi) != cgi.end()) {
        std::string pathToCgi = cgi[_cgi];
        // _cgiProgram = pathToCgi;
        // _cgiScript = _realPath; // u need it with ./ or /?

        // std::string cgiScript = request.get_dir(); // ?? chould i expend with root? 
        // std::string path = location.getPath();
        // cgiScript.replace(0, path.size(), location.getRoot()); // replaces locationPath with root
        
        // http://127.0.0.1:8080/test/hdjahd.py
        // ./HTTP/otherdir/hdjahd.py

        // which function to call the cgi?

    }



    clear();
    _method = request.get_method();
    unsigned int allowed_methods = location.getAllowedMethods();

    if ((allowed_methods & GET) && _method == GET) {
        getMethod(request);


    } else if ((allowed_methods & POST) && _method == POST) {
        postMethod(request);
        // Handle POST request
        // Process received data and send response
    } else if ((allowed_methods & DELETE) && _method == DELETE) {
        deleteMethod(request);
        // Handle DELETE request
        // Delete requested resource and send confirmation
    }else if ((allowed_methods & HEAD) && _method == HEAD) {
        getMethod(request);
    } else {
        _code = 405; // Method Not Allowed

        // Send error response
    }


}

void    Response::clear() {
    _code = 200;
    // std::string _phrase;
    _body = "";


    _mimeType = "text/html";

    // std::size_t bodySize;

}

void    Response::getMethod(Request &request) {
    std::string path = "." + _realPath; // ?? ./...

    struct stat path_stat;
    if (stat(path.c_str(), &path_stat) != 0) {
        _code = 404;
        return;
    } else if (S_ISREG(path_stat.st_mode)) { // check if it is a file
        setBody(path);
    } else if (S_ISDIR(path_stat.st_mode)) {
        if (path.back() != '/') {
            _code = 301;
            return;
        }
        if (_location.getAutoindex()) {
            setAutoindex(path); // path = dir listing file setDirListing(path);
            return ;
        } else {
            path += _location.getIndex(); // Default file
            if (stat(path.c_str(), &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
                setBody(path);
            } else {
                _code = 403;
            }
        }
    } else {
        _code = 404;
    }

    setMimeType(path);
}

void    Response::postMethod(Request &request) {
    std::string postData = request.get_body();
    std::string newFileName = _realPath;

    newFileName = newFileName.substr(1);

    // check if file exists 409 otherwise
    struct stat buffer;
    if (stat(newFileName.c_str(), &buffer) == 0) {
        _code = 409;
        return ;
    } else if (S_ISDIR(buffer.st_mode)) {

    }

    // create file
    std::ofstream file(newFileName.c_str());
    if (file.is_open()) {
        // if (!postData.empty()) {
            file.write(postData.c_str(), postData.size());
            file.close();
        // } else {
        //     _code = 400;
        // }
        _code = 201;
    } else {
        _code = 500;
    }
}



void    Response::deleteMethod(Request &request) {
    std::string newFileName = _realPath;
    newFileName = newFileName.substr(1);


    struct stat path_stat;
    if (stat(newFileName.c_str(), &path_stat) != 0) {
        _code = 404;
        return;
    } else if (S_ISREG(path_stat.st_mode)) {
        if (std::remove(newFileName.c_str()) == 0) {
            _code = 204;
        } else {
            _code = 500;
        }
    } else if (S_ISDIR(path_stat.st_mode)) {
        if (access(newFileName.c_str(), W_OK) != 0) {
            _code = 403;
            return ;
        } else {
            std::string	command = "rm -rf " + newFileName;
            if (std::system(command.c_str()) == 0) {
                _code = 204;
            } else {
                _code = 500;
            }
        }

    }
}

void Response::initResponsePhrase() {

    _responsePhrase[200] = "OK";
    _responsePhrase[201] = "Created";
    _responsePhrase[204] = "No Content";
    _responsePhrase[400] = "Bad Request";
    _responsePhrase[401] = "Unauthorized";
    _responsePhrase[403] = "Forbidden";
    _responsePhrase[404] = "Not Found";
    _responsePhrase[405] = "Method Not Allowed";
    _responsePhrase[413] = "Request Entity Too Long";
    _responsePhrase[500] = "Internal Server Error";
    _responsePhrase[501] = "Not Implemented";
    _responsePhrase[503] = "Service Unavailable";

}

std::string Response::getResponsePhrase(int const &sufix) {
    if (_responsePhrase.count(sufix)) {
        return _responsePhrase[sufix];
    }
    return _responsePhrase[400];
}


std::string Response::generate() {
    std::stringstream response_stream;
    
    if (_code > 399) { // ?? 
        std::map<int, std::string> error_pages = _location.getErrorPages();
        if (error_pages.find(_code) != error_pages.end()) {
            _body = error_pages[_code];
            _mimeType = "text/html";
        }
    }
    response_stream << "HTTP/1.1 " << _code << " " << getResponsePhrase(_code) << "\r\n";
    response_stream << "Content-Type: " << _mimeType << "\r\n";
    response_stream << "Content-Length: " << _body.size() << "\r\n";
    response_stream << "\r\n";
    if (_method != HEAD) {
        response_stream << _body;
    }

    std::string response = response_stream.str();
    return (response);
}

void Response::setCgiKey(std::string const &path) {
    std::size_t dot = path.rfind(".");
    if (dot == std::string::npos) {
        _cgi = "";
    } else {
        _cgi = path.substr(dot);
    }
}

void Response::setMimeType(std::string const &path) {
    std::size_t dot = path.rfind(".");
    if (dot == std::string::npos) {
        _mimeType = _mmTy.getType("bin");
    } else {
        _mimeType = _mmTy.getType(path.substr(dot + 1));
    }
}

int Response::setBody(std::string const &file) {

    std::ifstream myfile(file.c_str(), std::ios::in | std::ios::binary);
    if (myfile.is_open()) {
        // Determine the file size
        myfile.seekg(0, std::ios::end);
        std::streampos fileSize = myfile.tellg();
        myfile.seekg(0, std::ios::beg);

        // string to the correct size and read the file into it
        _body.resize(fileSize);
        myfile.read(&_body[0], fileSize);
        myfile.close();
        setMimeType(file);
        return 0;
    }
    _code = 404;
    return 1;
}

template <typename T>
std::string toString(const T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

void Response::setAutoindex(std::string const &path) {

    struct dirent *dirStruct;
    DIR *dir;
    
    dir = opendir(path.c_str());
    if (dir == NULL) {
        _code = 500;
        return;
    }
    _body.clear();
    _body.append("<html>\n<head>\n<title> Index of");
    _body.append(path);
    _body.append("</title>\n</head>\n<body >\n<h1> Index of ");
    _body.append(path);
    _body.append("</h1>\n<table style=\"width:80%; font-size: 15px\">\n<hr>\n");
    _body.append("<th style=\"text-align:left\"> File Name </th>\n");
    _body.append("<th style=\"text-align:left\"> Last Modification  </th>\n");
    _body.append("<th style=\"text-align:left\"> File Size </th>\n");
    struct stat fileStat;
    std::string filePath;
    while((dirStruct = readdir(dir)) != NULL)
    {
        if(strcmp(dirStruct->d_name, ".") == 0)
            continue;
        filePath = path + dirStruct->d_name;
        stat(filePath.c_str() , &fileStat);
        _body.append("<tr>\n<td>\n<a href=\"");
        _body.append(dirStruct->d_name);
        if (S_ISDIR(fileStat.st_mode))
            _body.append("/");
        _body.append("\">");
        _body.append(dirStruct->d_name);
        if (S_ISDIR(fileStat.st_mode))
            _body.append("/");
        _body.append("</a>\n</td>\n<td>\n");
        _body.append(ctime(&fileStat.st_mtime));
        _body.append("</td>\n<td>\n");
        if (!S_ISDIR(fileStat.st_mode))
            _body.append(toString(fileStat.st_size));
        _body.append("</td>\n</tr>\n");
    }
    _body.append("</table>\n<hr>\n</body>\n</html>\n");
}
