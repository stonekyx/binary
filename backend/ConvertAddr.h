#ifndef BACKEND_CONVERT_ADDR_H
#define BACKEND_CONVERT_ADDR_H

#include <vector>

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
    bool vaddrToFileOff(Elf64_Off &dst, Elf64_Addr);
    bool fileOffToVaddr(Elf64_Addr &dst, Elf64_Off);
    bool vaddrToSecOff(size_t &scnIdx, Elf64_Off &scnOff, Elf64_Addr);
    bool fileOffToSecOff(size_t &scnIdx, Elf64_Off &scnOff, Elf64_Off);
    bool secOffToFileOff(Elf64_Off &dst, size_t scnIdx, Elf64_Off scnOff);
    char *secOffStr(size_t scnIdx, Elf64_Off scnOff);
    char *vaddrToSecOffStr(Elf64_Addr);
    char *vaddrToSecOffStrWithOrig(Elf64_Addr);
    char *vaddrToSecOffStrWithData(Elf64_Addr, size_t dataSize = 20);
public slots:
    void invalidate();
signals:
    void invalidated();
private:
    File *_file;
    std::vector<Elf64_Phdr> _phdrs;
    std::vector<Elf64_Shdr> _shdrs;
    const char *_shdrStrRaw;
};

#endif
