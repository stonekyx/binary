#include <iostream>
#include <elf.h>

#include <QtGui/QMessageBox>

#include "backend/File.h"
#include "backend/ConvertAddr.h"
#include "ui_MainWindow.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(flow)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWBase(new Ui::MainWindow(), plugin, param, parent),
    _ui(dynamic_cast<Ui::MainWindow*>(MWBase::_ui))
{
    _noArg = true;
    _scnIndex = 0;
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
        _noArg = false;
    }
    _begin = _end = 0;
    if(param.find("vBegin") != param.end() &&
            param.find("vEnd") != param.end())
    {
        _begin = QString(param["vBegin"].c_str()).toULong();
        _end = QString(param["vEnd"].c_str()).toULong();
        _noArg = false;
    }
    updateInfo();
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_noArg) {
        close();
        QMessageBox::information(this, tr("About"),
                tr("This plugin shows a flow diagram for disassembly"
                    " code. To use this, right click a symbol start point"
                    " in <em>Disassemble</em> plugin."));
        return;
    }

    Elf64_Ehdr ehdr;
    if(!file->getEhdr(&ehdr)) { return; }
    ConvertAddr convertAddr(file);
    Elf64_Off fBegin=0, fEnd=0;
    if(ehdr.e_type == ET_REL) {
        convertAddr.secOffToFileOff(fBegin, _scnIndex, _begin);
        convertAddr.secOffToFileOff(fEnd, _scnIndex, _end);
    } else {
        convertAddr.vaddrToFileOff(fBegin, _begin);
        convertAddr.vaddrToFileOff(fEnd, _end);
    }
    _vBegin = _begin;
    _begin = fBegin;
    _end = fEnd;

    _blocks.clear();
    _breaks.clear();
    file->disasm(_begin, _end, disasmCallback, this);

    _blocks.clear();
    generateBlocks();
    cout<<_blocks.size()<<endl;
}

int MainWindow::disasmCallback(const File::DisasmInstInfo &inst,
        File::DisasmCBInfo &info)
{
    MainWindow *window = (MainWindow*)info.data;
    window->_inst.push_back(inst);

    if(window->_blocks.isEmpty()) {
        window->_blocks.push_back(CodeBlock(info.vaddr));
    }

    info.vaddr += inst.size;
    info.last = info.cur;

    CodeBlock &block = window->_blocks.back();
    if(!block.addInst(inst)) {
        window->_breaks.insert(block.getJumpTarget());
        window->_blocks.clear();
    }
    return 0;
}

void MainWindow::generateBlocks()
{
    Elf64_Addr vaddr = _vBegin;
    foreach(const File::DisasmInstInfo &inst, _inst) {
        if(_blocks.empty()) {
            _blocks.push_back(CodeBlock(vaddr));
        }

        vaddr += inst.size;

        CodeBlock &last = _blocks.back();
        if(!last.addInst(inst) || _breaks.find(vaddr) != _breaks.end()) {
            _blocks.push_back(CodeBlock(vaddr));
        }
    }
}

END_PLUG_NAMESPACE
