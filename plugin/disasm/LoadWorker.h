#ifndef PLUGIN_DISASM_LOADWORKER_H
#define PLUGIN_DISASM_LOADWORKER_H

#include <QtCore/QThread>

#include "common.h"
#include "backend/File.h"
#include "InfoModel.h"

BEGIN_PLUG_NAMESPACE(disasm)

class LoadWorker;

END_PLUG_NAMESPACE

class binary::plugin::disasm::LoadWorker :
    public QThread
{
    Q_OBJECT
public:
    LoadWorker(BIN_NAMESPACE(backend)::File *file,
            PLUG_NAMESPACE(plugin_framework)::InfoModel *im,
            QObject * = NULL);
    LoadWorker(size_t begin, size_t end,
            BIN_NAMESPACE(backend)::File *file,
            PLUG_NAMESPACE(plugin_framework)::InfoModel *im,
            QObject * = NULL);
signals:
    void symbolStarted(QModelIndex);
protected:
    virtual void run();
private:
    BIN_NAMESPACE(backend)::File *_file;
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
    bool _restricted;
    size_t _begin, _end;
    size_t _instIndentLevel;

    static int disasmCallback(char *, size_t, void *);
    void runAll();
};

#endif
