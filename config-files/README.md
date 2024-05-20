# Webserv Configuration

The Webserv configuration is controlled by **directives** specified in the configuration file. Directives are divided into simple directives and block directives.
A **simple directive** consists of the name and parameters separated by spaces and ends with a semicolon `;`. A **block directive** has a set of additional instructions surrounded by braces `{` and `}`.

## The File Structure
The webserv configuration file is based on the **nginx** configuration file structure. 


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

- `listen` - Sets the address and port.  

- `server_name` - Sets the name of a virtual server.

- `root` - Sets the root directory for requests.

- `allow_methods` - Specify the allowed methods

- `index` - Defines files that will be used as an index.

- `error_page` - Defines the URI that will be shown for the specified errors.

Generating a response by other means:

- `client_max_body_size` - The maximum body size of the client. If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.
- `return` - returns a response with the specified status code. Redirections are handled by this directive: temporily (307) or permanantly (301) redirection. If the status code is not a redirection, it can also return a string that will be sent in the HTTP response body. Return has the highest priority than the `cgi` directive.
- `autoindex` - Specify whether to show directory listings. 
- `cgi` - Map response file name extensions to a CGI script.

# Documentation

### `http`

> Syntax: **http** { ... }  
> Default: —  
> Context: main   

Provides the configuration file context in which the servers are specified.   

### `server`

> Syntax: **server** { ... }  
> Default: —  
> Context: http   

Sets configuration for a virtual server.   

### `location`

> Syntax: **location** uri { ... }  
> Default: —  
> Context: 	server   

Sets configuration depending on a request URI.   

```nginx
http {
    server {
        location [ uri ] {
            [ directives ]
        }
    }
}
```

### `listen`
> Syntax: **listen** address[:port];  
> Default: listen *:80;  
> Context: server  

Sets the address and port. If the port is not specified, the default port is 80.  

### `server_name`
> Syntax: server_name name ...;  
> Default: server_name "";  
> Context: server  
 
 Sets the names of a virtual server.

### `root`
> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location

 Sets the root directory for requests.


### `index`
> Syntax: index file;  
> Default: index index.html;  
> Context: http, server, location  

Defines files that will be used as an index. If autonindex is off, a HTTP request that ends with a '/' will try to return the first found index file instead.

### `error_page`
> Syntax: error_page code uri;  
> Default: —  
> Context: http, server, location, if in location  

Defines the URI that will be shown for the specified errors. Overrides the default error page of the server.


### `client_max_body_size`


The maximum body size of the client. If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.
### `return`


returns a response with the specified status code. Redirections are handled by this directive: temporily (307) or permanantly (301) redirection. If the status code is not a redirection, it can also return a string that will be sent in the HTTP response body. Return has the highest priority than the `cgi` directive.
### `autoindex`


Specify whether to show directory listings. 
### `cgi`


Map response file name extensions to a CGI script.
