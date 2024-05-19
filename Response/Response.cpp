#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::init(Request &request, bool dirListing) {


    // clear();

    if (request.get_method() == GET) {
        getMethod(request, dirListing);

        // setBody(request.get_dir());

        // if (_code == 200)





        // Handle GET request
        // Send response with requested data

        // send_response("HTTP/1.1 200 OK\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>");
    // } else if (request.get_method() == POST) {
    //     postMethod(request);
        // Handle POST request
        // Process received data and send response
    } else if (request.get_method() == DELETE) {
        deleteMethod();
        // Handle DELETE request
        // Delete requested resource and send confirmation
    } else {
        _code = 405; // Method Not Allowed

        // Send error response
    }


}

// bool  Response::isDirectory(const std::string &path) {
//     struct stat statbuf;
//     if (stat(path.c_str(), &statbuf) != 0) {
//         // the path doesn't exist
//         return false;
//     }
//     return S_ISDIR(statbuf.st_mode);
// }

// bool  Response::isFile(const std::string &path) {
//     struct stat statbuf;
//     if (stat(path.c_str(), &statbuf) != 0) {
//         // the path doesn't exist
//         return false;
//     }
//     return S_ISREG(statbuf.st_mode);
// }

void    Response::getMethod(Request &request, bool dirListing) {
    std::string path = "." + request.get_dir(); // ?? ./...

    struct stat path_stat;
    if (stat(path.c_str(), &path_stat) != 0) {
        _code = 404;
        return;
    } else if (S_ISREG(path_stat.st_mode)) { // check if it is a file
        setBody(request.get_dir());
    } else if (S_ISDIR(path_stat.st_mode)) {
        if (path.back() != '/') {
            _code = 301;
            return;
        }
        if (dirListing) {
            setDirListing(path); // path = dir listing file setDirListing(path);
        } else {
            path += "index.html"; // Default file
            if (stat(path.c_str(), &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
                setBody(request.get_dir());
            } else {
                _code = 403;
            }
        }
        // if (dir listing is on?) {
        //      path = dir listing file
        // } else {
        //    _code = 403;
        // }

    } else {
        _code = 404;
    }

            
    setMimeType(path);
}

// void    Response::postMethod(Request &request) {

// }

void    Response::deleteMethod() {

}

void Response::initResponsePhrase() {

    _responsePhrase[200] = "OK";
    _responsePhrase[201] = "Created";
    _responsePhrase[204] = "No Content";
    _responsePhrase[400] = "Bad Request";
    _responsePhrase[401] = "Unauthorized";
    _responsePhrase[403] = "Directory listing is forbidden";
    _responsePhrase[404] = "Not Found";
    _responsePhrase[405] = "Method Not Allowed";
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
    if (_code == 200) {
        response_stream << "Content-Type: " << _mimeType << "\r\n";
    }
    response_stream << "Content-Length: " << _body.size() << "\r\n";
    response_stream << "\r\n";
    if (_code == 200) {
        response_stream << _body;
    }

    std::string response = response_stream.str();
    return (response);
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
    std::string line;
    std::ifstream myfile(file);
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            _body = _body + line;
        }
        myfile.close();
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

void Response::setDirListing(std::string const &path) {

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
