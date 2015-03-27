#include <cstdlib>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include "ForkPipe.h"

using namespace std;
BEGIN_BIN_NAMESPACE(backend)

ForkPipe::ForkPipe(const char *path, char *const argv[], size_t bufsize) :
    _path(path), _argv(argv), _buf(new char[bufsize]), _bufsize(bufsize)
{
    _finished = false;
    _statLoc = 0;
    _pid = 0;
}

ForkPipe::~ForkPipe()
{
    delete[] _buf;
}

int ForkPipe::execAndWait()
{
    if(!_path || !_argv) {
        return -1;
    }
    int pipefds[2];
    if(pipe(pipefds)==-1) {
        return -1;
    }
    _pid = fork();
    switch(_pid) {
    case -1:
        return -1;
    case 0:
        dup2(pipefds[1], 1);
        dup2(pipefds[1], 2);
        close(0);
        if(execvp(_path, _argv) == -1) {
            exit(errno);
        }
        return 0;
    default:
        fcntl(pipefds[0], F_SETFL, O_NONBLOCK);
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(pipefds[0], &readset);
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        char *bufptr = _buf;
        pid_t waitres;
        while(select(pipefds[0]+1, &readset, NULL, NULL, &timeout)!=-1) {
            if(!FD_ISSET(pipefds[0], &readset)) {
                break; //timeout
            }
            ssize_t readres;
            while((readres = read(pipefds[0],
                            bufptr, _bufsize-(bufptr-_buf))) != 0)
            {
                if(readres == -1 &&
                        (errno == EAGAIN || errno == EWOULDBLOCK))
                {
                    waitres = waitpid(_pid, &_statLoc, WNOHANG);
                    if(waitres == _pid) {
                        continue;
                    }
                    break;
                }
                bufptr += readres;
            }
            if(readres == -1 &&
                    (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                continue;
            }
            break;
        }
        *bufptr = 0;

        if((waitres = waitpid(_pid, &_statLoc, WNOHANG)) != _pid) {
            kill(_pid, SIGKILL);
            return -1;
        }
        _finished = true;
        return 0;
    }
}

const char *ForkPipe::getBuf(size_t *size)
{
    if(size != NULL) {
        *size = _bufsize;
    }
    return _buf;
}

END_BIN_NAMESPACE
