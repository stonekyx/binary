#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctime>

#include "ForkPipe.h"

using namespace std;
BEGIN_BIN_NAMESPACE(backend)

ForkPipe::ForkPipe(const char *path, const char *const argv[], size_t bufsize) :
    _path(path), _buf(new char[bufsize]), _bufsize(bufsize)
{
    size_t i;
    for(i=0; argv[i]; i++);
    _argv = new char*[i+1];
    for(i=0; argv[i]; i++) {
        _argv[i] = strdup(argv[i]);
    }
    _argv[i] = NULL;
    _finished = false;
    _statLoc = 0;
    _pid = 0;
}

ForkPipe::~ForkPipe()
{
    for(size_t i=0; _argv[i]; i++) {
        free(_argv[i]);
    }
    delete[] _argv;
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
        close(pipefds[0]);
        dup2(pipefds[1], 1);
        dup2(pipefds[1], 2);
        close(0);
        if(execvp(_path, _argv) == -1) {
            exit(errno);
        }
        exit(0);
    default:
        if(!watchTime(pipefds)) {
            kill(_pid, SIGKILL);
            return -1;
        }
        close(pipefds[1]);
        fcntl(pipefds[0], F_SETFL, O_NONBLOCK);
        FILE *pipein = fdopen(pipefds[0], "r");
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(pipefds[0], &readset);
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        memset(_buf, 0, _bufsize);
        char *bufptr = _buf;
        pid_t waitres;
        while(select(pipefds[0]+1, &readset, NULL, NULL, &timeout)!=-1) {
            if(!FD_ISSET(pipefds[0], &readset)) {
                break; //timeout
            }
            while(fread(bufptr, _bufsize-(bufptr-_buf)-1, 1, pipein) != 0) {
                bufptr += strlen(bufptr);
            }
            bufptr += strlen(bufptr);
            if(feof(pipein)) {
                break;
            } else {
                continue;
            }
        }

        fclose(pipein);
        if((waitres = waitpid(_pid, &_statLoc, 0)) != _pid) {
            kill(_pid, SIGKILL);
            return -1;
        }
        if(WIFSIGNALED(_statLoc)) {
            return -1;
        }
        kill(_monPid, SIGKILL);
        waitpid(_monPid, NULL, 0);
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

bool ForkPipe::watchTime(int pipefds[])
{
    _monPid = fork();
    switch(_monPid) {
    case -1:
        return false;
    case 0:
        close(pipefds[0]);
        close(pipefds[1]);
        struct timespec timeout;
        timeout.tv_sec = 1;
        timeout.tv_nsec = 0;
        nanosleep(&timeout, NULL);
        kill(_pid, SIGKILL);
        exit(0);
    default:
        return true;
    }
}

END_BIN_NAMESPACE
