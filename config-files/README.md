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

- `index` - Defines files that will be used as an index.

- `error_page` - Defines the URI that will be shown for the specified errors.

- `client_max_body_size` - Sets the maximum allowed size of the client request body.  

- `allow_methods` - S the allowed methods

- `return` - Sends an HTTP response with a specified status code and optional URL. 

- `autoindex` - Enables or disables the directory listing output.  

- `cgi` - Maps a response file extension to a CGI script.   


# Documentation

## **Block Directives**

Block directives are used to group multiple directives together. The block directives are: `http`, `server`, and `location`.


```nginx
http {
    server {
        location [ uri ] {
            [ directives ]
        }
    }
}  
```  

#### `http`

> Syntax: **http** { ... }  
> Default: —  
> Context: main   

Provides the configuration file context in which the servers are specified.   

#### `server`

> Syntax: **server** { ... }  
> Default: —  
> Context: http   

Sets configuration for a virtual server.   

#### `location`

> Syntax: **location** uri { ... }  
> Default: —  
> Context: 	server   

Sets configuration depending on a request URI.   


  
## **Simple Directives**  

Simple directives are used to specify a single configuration parameter. The simple directives are: `listen`, `server_name`, `root`, `allow_methods`, `index`, `error_page`, `client_max_body_size`, `return`, `autoindex`, and `cgi`.

```nginx
http {
    server {
        listen address[:port];
        server_name name;
        root path;
        allow_methods pattern;
        index file;
        error_page code uri;
        client_max_body_size size;
        return code URL;
        autoindex [on | off];
        cgi extension script;
        location [ uri ] {
            # directives defined here override the server directives
            index file;
            error_page code uri;
            client_max_body_size size;
            return code URL;
            autoindex [on | off];
            cgi extension script;
        }
    }
}
```

#### `listen`
> Syntax: **listen** address[:port];  
> Default: listen *:80;  
> Context: server  

- Sets the address and port.
- If the port is not specified, the default port is 80.  

#### `server_name`
> Syntax: server_name name;  
> Default: server_name "";  
> Context: server  
 
-  Sets the names of a virtual server.

#### `root`
> Syntax: **root** path;  
> Default: root html;  
> Context: http, server, location, if in location

-  Sets the root directory for requests.


#### `index`
> Syntax: index file;  
> Default: index index.html;  
> Context: http, server, location  

- Defines files that will be used as an index.  

#### `error_page`
> Syntax: error_page code uri;  
> Default: —  
> Context: http, server, location, if in location  

- Defines the URI that will be shown for the specified errors.
- Overrides the default error page of the server.


#### `client_max_body_size`
> Syntax:  client_max_body_size size;  
> Default:  client_max_body_size 1m;  
> Context:  http, server, location  

- Sets the maximum allowed size of the client request body.  
- If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.  
- Sizes can be specified in bytes (e.g., “1024”) kilobytes (e.g., “8k”), or megabytes (e.g., “1m”).

#### `allow_methods`
> Syntax: allow_methods method ...;  
> Default: —  
> Context: http, server, location, if in location


- Describes HTTP methods be passed along a request.  
- The method can be specified as a single method or a list of methods separated by spaces.  
- GET POST PUT DELETE  

#### `return`
> Syntax: return code URL;  
> Default: —  
> Context: server, location, if  

- Sends an HTTP response with a specified status code and optional URL.  

#### `autoindex`
> Syntax: autoindex [on | off];  
> Default: autoindex off;  
> Context: http, server, location  

- Enables or disables the directory listing output.

#### `cgi`
> Syntax: cgi extension script;  
> Default: —  
> Context: location  
- Maps a response file extension to a CGI script.