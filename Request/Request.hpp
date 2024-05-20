/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:42:58 by tkilling          #+#    #+#             */
/*   Updated: 2024/05/20 16:16:24 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Head.hpp"
# include "Location.hpp"
// #include <iostream>
// #include <string>
// #include <sstream>
// #include <map>
// #include <vector>
// #include <algorithm>
// # define GET 1
// # define POST 2
// # define DELETE 4
// # define PUT 8
// # define HEAD 16


class Request {

public:

    Request();
    Request(std::string str);
    ~Request();

    // void init();
    // void parse();

    // void setFinish();

    void append(char const *bf, size_t n);

    std::string get();
    // bool getBad();
    // int getMethod();
    // std::string &getDir();
    // std::


    void    init(std::string str);
    void    parse();
    int     validate_request();

    void clear();

    void setFinish();

    int         get_method();
    std::string get_dir();
    std::string get_protocol();
    std::string get_host();
    std::string get_port();
    std::string get_cookie();
    std::string get_body();
    int         get_bad();
    std::map<std::string, std::string> get_headers();

private:

    std::string _request;

    std::string _method; // method
    std::string _dir; // path
    std::string _protocol; // http_version;
    std::map<std::string, std::string> _headers; // all the headers
    std::string _body; // body

    // within headers;
    std::string _host;
    std::string _port;
    std::string _cookie;

    int _bad; // valid

    bool _finish;
    // int _method; // method
    // std::vector<std::string> _accept;



};

#endif
