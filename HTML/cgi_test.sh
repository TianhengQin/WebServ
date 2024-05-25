#!/bin/bash

echo -e "Content-type: text/html\n\n\
<html>\n\
<head>\n\
<title>CGI Test</title>\n\
</head>\n\
<body>\n\
<h1>This is bash CGI test!</h1>\n\
<h1></h1>\n\
<h1>Cookie: [$HTTP_COOKIE]</h1>\n\
</body>\n\
</html>"
sleep 100
exit 1
