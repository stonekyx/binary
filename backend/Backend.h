#ifndef BACKEND_BACKEND_H
#define BACKEND_BACKEND_H

#include <vector>
#include <QtCore/QObject>

#include "File.h"
#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Backend;

END_BIN_NAMESPACE

class binary::backend::Backend : public QObject {
    Q_OBJECT
public:
    typedef enum {
        BACKEND_LIBELF,
        BACKEND_ELFSH,
        BACKEND_NUM
    } BackendType;

    Backend();
    ~Backend();
    void setBackendType(BackendType);
    File *openFile(const char *);
    void closeFile();
    File *getFile();
    void signalFileChange(File*);
signals:
    void fileChanged(binary::backend::File *);
private:
    BackendType _type;
    File *_file;
};

#endif
