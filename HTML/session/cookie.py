import sys
import os
import uuid

print("Content-type: text/html")

input_string = sys.stdin.read()
cookie = os.environ['HTTP_COOKIE']
method = os.environ['REQUEST_METHOD']
query = os.environ['QUERY_STRING']

cookies = cookie.replace(" ","").split(';')
session = ""
found = False
for cokie in cookies:
    if cokie.startswith('session_id='):
        session = cokie[11:]

if session != "":
    found = os.path.isfile(session)

def print_name():
    f = open(session, "r")
    name = f.read()
    f.close()
    print()
    print('<!DOCTYPE html>')
    print("<html>")
    print("<head>")
    print("<title>CGI Test</title>")
    print("</head>")
    print("<body>")
    print("<h1>Hello " + name + "</h1>")
    print("</body>")
    print("</html>")
    print('</html>')

def print_post_page():
    new_session=uuid.uuid4().hex
    f = open(new_session, "w")
    f.close()
    print(f"Set-Cookie: session_id={new_session}\n")
    print('<!DOCTYPE html>')
    print('<html>')
    print('<head>')
    print('  <title>Post name</title>')
    print('</head>')
    print('<body>')
    print('  <form id="post-name">')
    print('    <input type="text" id="uri-input" name="uri" placeholder="your name">')
    print('    <button type="button" onclick="uploadFile()">confirm</button><br>')
    print('    <a id="result"></a><br>')
    print('    <a id="location"></a>')
    print('  </form>')
    print('</body>')
    print('<script>')
    print(' function uploadFile() {')
    print('    const apiUrl = "/session/cookie.py"')
    print("    const name = document.getElementById('uri-input').value;")
    print('    const requestOptions = {')
    print("      method: 'POST',")
    print('      body: name,')
    print('    };')
    print('    fetch(apiUrl, requestOptions)')
    print('      .then(response => { })')
    print('      .catch(error => { });')
    print('  }')
    print('</script>')
    print('</html>')

def post_name():
    f = open(session, "w")
    f.write(input_string)
    f.close()
    print(input_string)

def print_err():
    print()
    print('<!DOCTYPE html>')
    print("<html>")
    print("<head>")
    print("<title>CGI Test</title>")
    print("</head>")
    print("<body>")
    print("<h1>method " + method + "</h1>")
    print("</body>")
    print("</html>")
    print('</html>')

if method == "GET" and found == True:
    print_name()
    # print("print_name")
elif method == "GET" and found == False:
    print_post_page()
elif method == "POST" and found == True:
    post_name()
    # print("post_name")
else:
    print_err()
    # print("print_err")

