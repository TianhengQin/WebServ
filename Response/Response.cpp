#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::init() {

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
