#ifndef PLUGIN_FLOW_MAINWINDOW_H
#define PLUGIN_FLOW_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWBase.h"
#include "CodeBlock.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QMainWindow>
#include <QtCore/QSet>
#include <QtCore/QList>

BEGIN_PLUG_NAMESPACE(flow)

namespace Ui {
    class MainWindow;
}
class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::flow::MainWindow :
    public binary::plugin::plugin_framework::MWBase
{
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
    using MWBase::updateInfo;
private:
    bool _noArg;
    size_t _scnIndex;
    Elf64_Off _begin, _end;
    Ui::MainWindow *_ui;
    QList<CodeBlock> _blocks;
    QSet<Elf64_Addr> _breaks;
    QList<BIN_NAMESPACE(backend)::File::DisasmInstInfo> _inst;
    Elf64_Addr _vBegin;
    QGraphicsScene *_scene;

    static int disasmCallback(
            const BIN_NAMESPACE(backend)::File::DisasmInstInfo &,
            BIN_NAMESPACE(backend)::File::DisasmCBInfo &);
    void generateBlocks();
    std::string outputBlocks();
    int getBlockByStartAddr(Elf64_Addr);
};

#endif
