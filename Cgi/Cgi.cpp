#include "Cgi.hpp"

Cgi::Cgi() {}

Cgi::Cgi(Connection &conn) {
    _connectFd = conn.getFd();
    _program = conn.getCgiProgram();
    _script = conn.getCgiScript();

    if (pipe(_pipeIn) == -1) {
        Log::print(ERROR, "Cgi pipe create failed on connection ", _connectFd);
        conn.setCgiState(CGI_FAILED);
    }
    if (pipe(_pipeOut) == -1) {
        Log::print(ERROR, "Cgi pipe create failed on connection ", _connectFd);
        close(_pipeIn[0]);
        close(_pipeIn[1]);
        conn.setCgiState(CGI_FAILED);
    }
    if (fcntl(_pipeOut[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(_pipeIn[1], F_SETFL, O_NONBLOCK) == -1) {
        Log::print(ERROR, "Cgi pipe set non-block failed on connection ", _connectFd);
        close(_pipeIn[0]);
        close(_pipeIn[1]);
        close(_pipeOut[0]);
        close(_pipeOut[1]);
        conn.setCgiState(CGI_FAILED);
    }
    setEnv();
    Log::print(DEBUG, "Cgi create succeed", 0);
}

int Cgi::run(Connection &conn) {
    Log::print(DEBUG, "Cgi before fork", 0);
    _pid = fork();
    Log::print(WARNING, "Cgi after fork ", _pid);
    if (_pid < 0) {
        Log::print(ERROR, "Cgi fork failed on connection ", _connectFd);
        conn.setCgiState(CGI_FAILED);
        return 1;
    }
    if (_pid == 0) {
        close(_pipeOut[0]);
        close(_pipeIn[1]);
        if (dup2(_pipeOut[1], 1) == -1 || dup2(_pipeIn[0], 0) == -1) {
            Log::print(ERROR, "Cgi dup failed on connection ", _connectFd);
            close(_pipeOut[1]);
            close(_pipeIn[0]);
            exit(1);
        }
        close(_pipeOut[1]);
        close(_pipeIn[0]);
        Log::print(DEBUG, "Cgi exe", 0);
        exeCgi();
    }
    close(_pipeOut[1]);
    close(_pipeIn[0]);
    return 0;
}

int Cgi::end() {
    int exit;
    waitpid(_pid, &exit, 0);
    if (exit) {
        return 1;
    } else {
        return 0;
    }
}

Cgi::~Cgi() {
    close(_pipeIn[0]);
    close(_pipeIn[1]);
    close(_pipeOut[0]);
    close(_pipeOut[1]);
    kill(_pid, SIGKILL);
    waitpid(_pid, NULL, 0);
}

int Cgi::getConnectFd() {
    return _connectFd;
}

int Cgi::getPipeOutFd() {
    return _pipeOut[0];
}

int Cgi::getPipeInFd() {
    return _pipeIn[1];
}

int Cgi::send() {

    int sent;
    size_t bf_size = _sendBf.size();
    Log::print(DEBUG, "Cgi send buffer ", bf_size);
    if (bf_size <= RS_BF_SIZE) {
        sent = write(_pipeIn[1], _sendBf.c_str(), bf_size);
        if (sent < 0) {
            Log::print(ERROR, "Write error on pipe ", _pipeIn[1]);
            return -1;
        }
        Log::print(DEBUG, "Cgi sent ", sent);
        _sendBf.clear();
        return 0;
    }
    sent = write(_pipeIn[1], _sendBf.c_str(), RS_BF_SIZE);
    if (sent < 0) {
        Log::print(ERROR, "Write error on pipe ", _pipeIn[1]);
        return -1;
    }
    _sendBf.erase(0, RS_BF_SIZE);
    return 1;
}

int Cgi::receive() {

    char bf[RS_BF_SIZE + 1];
    int received = read(_pipeOut[0], bf, RS_BF_SIZE);
    if (received < 0) {
        Log::print(ERROR, "Read error on pip ", _pipeOut[0]);
        return -1;
    } else if (received < RS_BF_SIZE) {
        _recvBf.append(bf, received);
        Log::print(DEBUG, "Received cgi finish ", _pipeOut[0]);
        return 0;
    } else {
        _recvBf.append(bf, received);
        return 1;
    }
}

std::string &Cgi::response() {
    return _recvBf;
}

void Cgi::setRequestBody(std::string &rb) {
    _sendBf = rb;
}

void Cgi::setEnv() {
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    _env.push_back("SERVER_SOFTWARE=python3");
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("SCRIPT_FILENAME=index.py");
    _env.push_back("REQUEST_URI=");
    _env.push_back("HTTP_HOST=localhost");
    _env.push_back("REQUEST_METHOD=GET");
    _env.push_back("QUERY_STRING=/?");
    _env.push_back("REDIRECT_STATUS=200");
    _env.push_back("SCRIPT_NAME=index.py");
    _env.push_back("CONTENT_LENGTH=0");
    _env.push_back("CONTENT_TYPE=text/plain");
    _env.push_back("SERVER_NAME=localhost");
    _env.push_back("SERVER_PORT=8080");
    _env.push_back("HTTP_COOKIE=");
}

void Cgi::exeCgi() {
    std::vector<char *> arg;
    std::vector<char *> env;
    arg.push_back(const_cast<char *>(_program.c_str()));
    arg.push_back(const_cast<char *>(_script.c_str()));
    arg.push_back(NULL);
    std::vector<std::string>::iterator it;
    for (it = _env.begin(); it != _env.end(); ++it) {
        env.push_back(const_cast<char *>(it->c_str()));
    }
    env.push_back(NULL);
    execve(_program.c_str(), arg.data(), env.data());
    Log::print(WARNING, "Cgi exe failed on connection ", _connectFd);
    exit(1);
}
