#ifndef BACKEND_CONVERT_ADDR_H
#define BACKEND_CONVERT_ADDR_H

#include <QtCore/QObject>

#include "common.h"
#include "File.h"

BEGIN_BIN_NAMESPACE(backend)

class ConvertAddr;

END_BIN_NAMESPACE

class binary::backend::ConvertAddr : public QObject {
    Q_OBJECT
public:
    ConvertAddr(File *file);
    bool vaddrToFileOff(size_t &dst, Elf64_Addr);
public slots:
    void invalidate();
signals:
    void invalidated();
private:
    File *_file;
};

#endif
