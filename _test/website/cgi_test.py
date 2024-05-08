import sys

input_string = sys.stdin.read()

print("Content-type: text/html\n")
print("<html>")
print("<head>")
print("<title>CGI Test</title>")
print("</head>")
print("<body>")
print("<h1>This is python3 CGI test!</h1>")
print("<h1>" + input_string + "</h1>")
print("</body>")
print("</html>")
