#include "Response.hpp"
#include "Connection.hpp"

Response::Response() {
    initResponsePhrase();
}

Response::~Response() {}

void Response::init(Connection &connection, Request &request, Server &server, Location &location) {

    // config setup
    _server = server;
    _location = location;

    clear();
    _code = request.get_bad();
    if (_code != 200) {
        return ;
    }

    if (location.getClientMaxBodySize() < (unsigned int) request.get_body().size()) {
        _code = 413;
        return ;
    }

    if (location.getRedir() != "" && location.getRedir() != request.get_dir()) {
        _code = 301;
        return ;
    }

    // replaces locationPath with root and put in _realPath right now /bla not ./bla
    _realPath = request.get_dir();
    std::string pathLocaton = location.getPath();

    if (_realPath.size() >= location.getPath().size() && location.getAlias() != "") {
        _realPath.replace(0, location.getPath().size(), location.getAlias()); // or alias???
    }


    // if () // cgi??
    std::map<std::string, std::string> cgi = location.getCgi();
    // std::cout << cgi[".py"] << std::endl;
    setCgiKey(request.get_dir());
    // std::cout << "_cgi: " << _cgi << _cgi.size() << std::endl;
    if (cgi.find(_cgi) != cgi.end()) {
        // std::cout << "cgi in loop" << std::endl;
        std::string pathToCgi = cgi[_cgi];
        connection.setCgiProgram(pathToCgi);
        connection.setCgiScript("." + _realPath);
        connection.setCgiState(CGI_ON);
        connection.setCgiSendBf(request.get_body());
        return ;
    }


    _method = request.get_method();
    unsigned int allowed_methods = location.getAllowedMethods();

    if ((allowed_methods & GET) && _method == GET) {
        getMethod();


    } else if ((allowed_methods & POST) && _method == POST) {
        postMethod(request);
        // Handle POST request
        // Process received data and send response
    } else if ((allowed_methods & DELETE) && _method == DELETE) {
        deleteMethod();
        // Handle DELETE request
        // Delete requested resource and send confirmation
    }else if ((allowed_methods & HEAD) && _method == HEAD) {
        getMethod();
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


void    Response::getMethod() {
    std::string path = "." + _realPath; // ?? ./...

    if (path.find("..") != std::string::npos) {
        _code = 403;
        return ;
    }

    struct stat path_stat;
    if (stat(path.c_str(), &path_stat) != 0) {
        _code = 404;
        return;
    } else if (S_ISREG(path_stat.st_mode)) { // check if it is a file
        setBody(path);
    } else if (S_ISDIR(path_stat.st_mode)) {
        if (path[path.size() - 1] != '/') {
            path += "/";
            
            // _code = 301;
            // return;
        }
        if (_location.getAutoindex()) {
            setAutoindex(path); // path = dir listing file setDirListing(path);
            return ;
        } else if (_location.getIndex() != "") {
            path += _location.getIndex(); // Default file
            if (stat(path.c_str(), &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
                setBody(path);
            } else {
                _code = 403;
            }
        } else {
            _code = 404;
        }
    } else {
        _code = 404;
    }

    setMimeType(path);
}

std::string getFileName(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

void    Response::postMethod(Request &request) {
    std::string targetDirectory = _location.getRoot();
    std::string newFileName     = targetDirectory + "/" + getFileName(_realPath);
    // std::cerr << "craet file" << std::endl;
    if (newFileName.find("..") != std::string::npos) {
        _code = 403;
        return ;
    }

    if (newFileName[0] == '/') {
        newFileName = newFileName.substr(1);
        targetDirectory = targetDirectory.substr(1);
    }

    // checking if root of configurationfile is valid;
    struct stat dirStat;
    if (stat(targetDirectory.c_str(), &dirStat) != 0 || !S_ISDIR(dirStat.st_mode)) {
        _code = 500;
        return ;
    }

    // check if file exists 409 otherwise
    struct stat buffer;
    if (stat(newFileName.c_str(), &buffer) == 0) {
        _code = 409;
        return ;
    } else if (S_ISDIR(buffer.st_mode)) {

    }

    // check if file is empty
    // if (request.get_body() == "") {
    //     _code = 400;
    //     return ;
    // }

    // create file
    std::ofstream file(newFileName.c_str());
    if (file.is_open()) {
        // if (!postData.empty()) {
            

            file.write(request.get_body().c_str(), request.get_body().size());
            file.close();
        // } else {
        //     _code = 400;
        // }
        _code = 201;
    } else {
        _code = 500;
    }
}



void    Response::deleteMethod() {
    // std::string newFileName = _realPath;
    // newFileName = newFileName.substr(1);


    std::string targetDirectory = _location.getRoot();;
    std::string newFileName     = targetDirectory + "/" + getFileName(_realPath);


    if (newFileName.find("..") != std::string::npos) {
        _code = 403;
        return ;
    }

    if (newFileName[0] == '/') {
        newFileName = newFileName.substr(1);
        targetDirectory = targetDirectory.substr(1);
    }

    // checking if root of configurationfile is valid;
    struct stat dirStat;
    if (stat(targetDirectory.c_str(), &dirStat) != 0 || !S_ISDIR(dirStat.st_mode)) {
        _code = 500;
        return ;
    }

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
    _responsePhrase[301] = "Moved Permanently";
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
    response_stream << "HTTP/1.1 " << _code << " " << getResponsePhrase(_code) << "\r\n";
    
    if (_code > 399) { // ?? 
        if (_location.hasErrorPage(_code)) {
            setBody("." + _location.getErrorPage(_code));
        } else {
            setBody("");
        }
        _mimeType = "text/html";
    } else if (_code > 299) {
        

        // _body = _location.getRedir();
        response_stream << "Location: " << _location.getRedir() << "\r\n";
        _body = "<html><body><h1>301 Moved Permanently</h1></body></html>";
        _mimeType = "text/html";
    }
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

void Response::set_code(int const &code) {
    _code = code;
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
    closedir(dir);
}
