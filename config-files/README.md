# Webserv Configuration

The Webserv configuration is controlled by **directives** specified in the configuration file. Directives are divided into simple directives and block directives.
A **simple directive** consists of the name and parameters separated by spaces and ends with a semicolon `;`. A **block directive** has a set of additional instructions surrounded by braces `{` and `}`.

## The File Structure
The webserv configuration file (based on the **nginx** configuration file structure). 



<!--webserver is based on the Nginx configuration file structure. The Webserv configuration is defined by 
 . The configuration file is divided into blocks, each block consists of directives. The configuration file is divided into the following blocks -->


<!-- consists of modules which are  -->
### Block directives

- `http` - Provides the configuration file context in which the servers are specified.  

- `server` - Sets configuration for a virtual server.  

- `location` - Sets configuration depending on a request URI.  

```nginx
http {
    server {
        location [ uri ] {
            [ directives ]
        }
    }
}
```


### Simple directives


- `listen` - Sets the root directory for requests.
- `server_name` - The server name of the server
- `allow_methods` - Specify the allowed methods

- `root` - Sets the root directory for requests.
- `index` - The index files of the server. If autonindex is off, a HTTP request that ends with a '/' will try to return the first found index file instead.
- `error_page` - overrides defualt error page of the server

Generating a response by other means:

- `client_max_body_size` - The maximum body size of the client. If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.
- `return` - returns a response with the specified status code. Redirections are handled by this directive: temporily (307) or permanantly (301) redirection. If the status code is not a redirection, it can also return a string that will be sent in the HTTP response body. Return has the highest priority than the `cgi` directive.
- `autoindex` - Specify whether to show directory listings. 
- `cgi` - Map response file name extensions to a CGI script.

## Directives 

### http
> Syntax: **http** { ... }  
> Default: —  
> Context: main  

### server
> Syntax: **server** { ... }  
> Default: —  
> Context: http  

### location
> Syntax: **location** uri { ... }  
> Default: —  
> Context: http  

> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location

> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location

> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location

> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location

> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location
