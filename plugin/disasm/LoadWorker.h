#ifndef PLUGIN_DISASM_LOADWORKER_H
#define PLUGIN_DISASM_LOADWORKER_H

#include <QtCore/QThread>

#include "common.h"
#include "backend/File.h"
#include "backend/TimeMeasure.h"
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
    void timerStopped(double, double);
public slots:
    void stopTimer();
protected:
    virtual void run();
private:
    BIN_NAMESPACE(backend)::File *_file;
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
    bool _restricted;
    size_t _begin, _end;
    size_t _instIndentLevel;
    size_t _noSleep;
    BIN_NAMESPACE(backend)::TimeMeasure _tm;
    size_t _instCnt;
    Elf64_Ehdr _ehdr;

    static QString processBuffer(
            const BIN_NAMESPACE(backend)::File::DisasmInstInfo &,
            const BIN_NAMESPACE(backend)::File::DisasmCBInfo &);
    static int disasmCallback(
            const BIN_NAMESPACE(backend)::File::DisasmInstInfo &,
            BIN_NAMESPACE(backend)::File::DisasmCBInfo &);
    void runAll();
};

#endif
