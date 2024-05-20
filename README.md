# WebServ
WebServer based on c++

### Usage

```bash
$ make
$ ./webserver [ configuration file ]
```

#### Example
```bash
$ ./webserver config-files/sample.conf
```

## The Configuration File

```nginx

http {
    include mime.types;
    server {
        listen 127.0.0.1:8080;
        server_name localhost;

        root /var/www/html;
        index index.html;

        autoindex on;
        
        error_page 404 /config-files/default_error_pages/404.html;

        
        location / {
            cgi py /cgi-bin;
        }
        
        location /images {
            allow_methods GET POST DELETE;
            return /new;
        }
        
        location /cgi-bin {
            allow_methods GET DELETE;
            return "you are welcome";
        }
        
        location /hello {
            return "lol";
        }
    }
}
```


#### Block directives

- `http` - Provides the configuration file context in which the servers are specified.
- `server` - Sets configuration for a virtual server.
- `location` - Sets configuration depending on a request URI.

#### Directives related to HTTP requests

- `listen` - The port that the server will listen on
- `server_name` - The server name of the server
- `allow_methods` - Specify the allowed methods

#### Directives for HTTP response generation

Generating a response from a requested file:

- `root` - The root directory of the server.
- `index` - The index files of the server. If autonindex is off, a HTTP request that ends with a '/' will try to return the first found index file instead.
- `types` - Map response file name extensions to MIME types
- `error_page` - The error page of the server

Generating a response by other means:

- `client_max_body_size` - The maximum body size of the client. If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.
- `return` - returns a response with the specified status code. Redirections are handled by this directive: temporily (307) or permanantly (301) redirection. If the status code is not a redirection, it can also return a string that will be sent in the HTTP response body. Return has the highest priority than the `cgi` directive.
- `autoindex` - Specify whether to show directory listings
- `cgi` - Specify a cgi script

#### Misc directives

For logging information:

- `worker_connections` - Maximum amount of connections that the server will handle at any given point of time 
