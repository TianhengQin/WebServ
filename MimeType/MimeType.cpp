#include "MimeType.hpp"

MimeType::MimeType() {

    _mmty["aac"] = "audio/aac";
    _mmty["abw"] = "application/x-abiword";
    _mmty["apng"] = "image/apng";
    _mmty["arc"] = "application/x-freearc";
    _mmty["avif"] = "image/avif";
    _mmty["avi"] = "video/x-msvideo";
    _mmty["azw"] = "application/vnd.amazon.ebook";
    _mmty["bin"] = "application/octet-stream";
    _mmty["bmp"] = "image/bmp";
    _mmty["bz"] = "application/x-bzip";
    _mmty["bz2"] = "application/x-bzip2";
    _mmty["cda"] = "application/x-cdf";
    _mmty["csh"] = "application/x-csh";
    _mmty["css"] = "text/css";
    _mmty["csv"] = "text/csv";
    _mmty["doc"] = "application/msword";
    _mmty["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    _mmty["eot"] = "application/vnd.ms-fontobject";
    _mmty["epub"] = "application/epub+zip";
    _mmty["gz"] = "application/gzip";
    _mmty["gif"] = "image/gif";
    _mmty["htm"] = "text/html";
    _mmty["html"] = "text/html";
    _mmty["ico"] = "image/vnd.microsoft.icon";
    _mmty["ics"] = "text/calendar";
    _mmty["jar"] = "application/java-archive";
    _mmty["jpeg"] = "image/jpeg";
    _mmty["jpg"] = "image/jpeg";
    _mmty["js"] = "text/javascript";
    _mmty["json"] = "application/json";
    _mmty["jsonld"] = "application/ld+json";
    _mmty["mid"] = "audio/midi";
    _mmty["midi"] = "audio/midi";
    _mmty["mjs"] = "text/javascript";
    _mmty["mp3"] = "audio/mpeg";
    _mmty["mp4"] = "video/mp4";
    _mmty["mpeg"] = "video/mpeg";
    _mmty["mpkg"] = "application/vnd.apple.installer+xml";
    _mmty["odp"] = "application/vnd.oasis.opendocument.presentation";
    _mmty["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    _mmty["odt"] = "application/vnd.oasis.opendocument.text";
    _mmty["oga"] = "audio/ogg";
    _mmty["ogv"] = "video/ogg";
    _mmty["ogx"] = "application/ogg";
    _mmty["opus"] = "audio/opus";
    _mmty["otf"] = "font/otf";
    _mmty["png"] = "image/png";
    _mmty["pdf"] = "application/pdf";
    _mmty["php"] = "application/x-httpd-php";
    _mmty["ppt"] = "application/vnd.ms-powerpoint";
    _mmty["pptx"] = "pplication/vnd.openxmlformats-officedocument.presentationml.presentation";
    _mmty["rar"] = "application/vnd.rar";
    _mmty["rtf"] = "application/rtf";
    _mmty["sh"] = "application/x-sh";
    _mmty["svg"] = "image/svg+xml";
    _mmty["tar"] = "application/x-tar";
    _mmty["tif"] = "image/tiff";
    _mmty["tiff"] = "image/tiff";
    _mmty["ts"] = "video/mp2t";
    _mmty["ttf"] = "font/ttf";
    _mmty["txt"] = "text/plain";
    _mmty["vsd"] = "application/vnd.visio";
    _mmty["wav"] = "audio/wav";
    _mmty["weba"] = "audio/webm";
    _mmty["webm"] = "video/webm";
    _mmty["webp"] = "image/webp";
    _mmty["woff"] = "font/woff";
    _mmty["woff2"] = "font/woff2";
    _mmty["xhtml"] = "application/xhtml+xml";
    _mmty["xls"] = "application/vnd.ms-excel";
    _mmty["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    _mmty["xml"] = "application/xml";
    _mmty["xul"] = "application/vnd.mozilla.xul+xml";
    _mmty["zip"] = "application/zip";
    _mmty["3gp"] = "video/3gpp";
    _mmty["3g2"] = "video/3gpp2";
    _mmty["7z"] = "application/x-7z-compressed";
}

MimeType::~MimeType() {}

std::string &MimeType::getType(std::string const &sufix) {
    if (_mmty.count(sufix)) {
        return _mmty[sufix];
    }
    return _mmty["bin"];
}
