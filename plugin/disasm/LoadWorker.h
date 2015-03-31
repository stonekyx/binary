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
public:
    LoadWorker(BIN_NAMESPACE(backend)::File *file,
            PLUG_NAMESPACE(plugin_framework)::InfoModel *im,
            QObject * = NULL);
protected:
    virtual void run();
private:
    BIN_NAMESPACE(backend)::File *_file;
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;

    static int disasmCallback(char *, size_t, void *);
};

#endif
