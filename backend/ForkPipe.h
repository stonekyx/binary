#ifndef BACKEND_FORKPIPE_H
#define BACKEND_FORKPIPE_H

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class ForkPipe {
public:
    ForkPipe(const char *path, const char *const argv[], size_t bufsize=65536);
    ~ForkPipe();
    int execAndWait();
    const char *getBuf(size_t *size = NULL);
private:
    const char *_path;
    char **_argv;
    bool _finished;
    int _statLoc;
    pid_t _pid;
    char *_buf;
    size_t _bufsize;
};

END_BIN_NAMESPACE

#endif
