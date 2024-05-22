/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:42:58 by tkilling          #+#    #+#             */
/*   Updated: 2024/05/22 12:01:50 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Head.hpp"
# include "Location.hpp"

class Request {

public:

    Request();
    Request(std::string &str);
    ~Request();

    void append(char const *bf, size_t n);

    std::string get();

    void    init(std::string &str);
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
    // std::vector<std::string> _accept;



};

#endif
