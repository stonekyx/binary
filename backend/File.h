#ifndef BACKEND_FILE_H
#define BACKEND_FILE_H

#include <elf.h>
#include <cstdlib>
#include <cstring>
#include <set>
#include <QtCore/QObject>

#include "common.h"
#include "Arhdr.h"
#include "Arsym.h"

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
    virtual int getPhdrNum(size_t *dst) = 0;
    virtual int getShdrNum(size_t *dst) = 0;
    virtual int getShdrStrNdx(size_t *dst) = 0;
    virtual bool getPhdr(size_t, Elf64_Phdr *) = 0;
    virtual bool getShdr(size_t, Elf64_Shdr *) = 0;
    virtual const char *getScnName(Elf64_Shdr *) = 0;
    virtual ssize_t getScnData(size_t idx, void *, size_t bufsize) = 0;
    virtual bool getSym(size_t scnIdx, int idx, Elf64_Sym *) = 0;
    virtual bool getSyminfo(size_t scnIdx, int idx, Elf64_Syminfo *) = 0;
    virtual const char *getStrPtr(size_t scnIdx, size_t offset) = 0;
    virtual bool getDyn(size_t scnIdx, int idx, Elf64_Dyn *) = 0;
    virtual Arhdr getArhdr(size_t objIdx) = 0;
    virtual Arhdr getArhdrByOffset(size_t objOff) = 0;
    virtual size_t getArhdrNum() = 0;
    virtual bool setArObj(size_t objIdx) = 0;
    virtual Arsym getArsym(size_t symIdx) = 0;
    virtual size_t getArsymNum() = 0;
    virtual char *getRawData(size_t offset) = 0;
    virtual bool queryDynSym(const char *, Elf64_Sym *dst) = 0;
    virtual const char *queryDynSymDeps(const char *, Elf64_Sym *) = 0;
    typedef int (*DisasmCB)(char *, size_t, void *);
    struct DisasmCBInfo {
        const uint8_t *cur;
        const uint8_t *last;
        Elf64_Addr vaddr;
        Elf64_Shdr *shdr; //invalid after all calls finished
        void *data;
        char *labelBuf;
        void *callerData;
    };
    virtual int disasm(size_t scnIdx, DisasmCB, void*) = 0;
    virtual const char *getName() {
        return _name;
    }

    void setBackend(Backend *b) {
        _backend = b;
    }
    void registerWatcher(const QObject *o) {
        _watchers.insert(o);
        QObject::connect(o, SIGNAL(destroyed()),
                this, SLOT(admitDestroy()));
    }
    void arrangeDelete() {
        if(_watchers.empty()) {
            delete this;
            return;
        }
        _arrangedDelete = true;
        emit aboutToBeDestroyed();
    }
signals:
    void aboutToBeDestroyed();
public slots:
    void admitDestroy() {
        _watchers.erase(sender());
        if(_arrangedDelete && _watchers.empty()) {
            deleteLater();
        }
    }
protected:
    Backend *_backend;
    char *_name;
    std::set<const QObject*> _watchers;
    bool _arrangedDelete;

    File(const char *name) :
        _backend(NULL), _name(strdup(name)), _arrangedDelete(false) {}
    virtual ~File() { free(_name); }
};

#endif
