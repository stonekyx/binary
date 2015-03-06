#ifndef BACKEND_BACKEND_H
#define BACKEND_BACKEND_H

#include <vector>

#include "File.h"
#include "Observer.h"
#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Backend {
public:
    typedef enum {
        BACKEND_LIBELF,
        BACKEND_ELFSH,
        BACKEND_NUM
    } BackendType;

    Backend();
    ~Backend();
    void registerView(Observer *);
    void setBackendType(BackendType);
    File *openFile(const char *);
    File *getFile();
    void updateAll();
private:
    BackendType _type;
    std::vector<Observer*> _observers;
    File *_file;

    void closeFile();
};

END_BIN_NAMESPACE

#endif
