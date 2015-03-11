#ifndef BACKEND_FILE_H
#define BACKEND_FILE_H

#include <elf.h>
#include <cstdlib>
#include <QtCore/QObject>

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Backend;
class File;

END_BIN_NAMESPACE

class binary::backend::File : public QObject {
    Q_OBJECT
public:
    typedef enum {
        KIND_UNKNOWN,
        KIND_AR,
        KIND_ELF,
        KIND_COFF
    } ELFKind;
    virtual bool isValid() = 0;
    virtual int getClass() = 0;
    virtual ELFKind getKind() = 0;
    virtual bool getEhdr(Elf64_Ehdr *) = 0;

    void setBackend(Backend *b) {
        _backend = b;
    }
protected:
    Backend *_backend;

    File() : _backend(NULL) {}
    virtual ~File() {}
};

#endif
