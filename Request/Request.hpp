/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:42:58 by tkilling          #+#    #+#             */
/*   Updated: 2024/04/16 13:44:45 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Head.hpp"
# include "Location.hpp"

class Request {

public:

    Request();
    Request(std::string str);
    ~Request();

    // void init();
    // void parse();

    // void setFinish();

    void append(char const *bf, size_t n);

    std::string &get();
    // bool getBad();
    // int getMethod();
    // std::string &getDir();
    // std::


    void init(std::string str);
    void parse();

    void clear();

    void setFinish();

    int         get_method();
    std::string get_dir();
    std::string get_protocol();
    std::string get_host();
    std::string get_port();


private:

    std::string _request;
    bool _finish;
    std::string _body;

    bool _bad;
    int _method;
    std::string _dir;
    std::string _protocol;
    std::string _host;
    std::string _port;
    std::string _cookie;
    std::vector<std::string> _accept;

};

#endif
