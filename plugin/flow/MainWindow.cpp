#include <sstream>
#include <elf.h>

#include <QtGui/QMessageBox>

#include "backend/File.h"
#include "backend/ConvertAddr.h"
#include "ui_MainWindow.h"
#include "FlowDrawer.h"
#include "GraphicsView.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(flow)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWBase(new Ui::MainWindow(), plugin, param, parent),
    _ui(dynamic_cast<Ui::MainWindow*>(MWBase::_ui)),
    _scene(NULL),
    _algo(GVGraph::GV_FDP)
{
    QObject::connect(_ui->btnGrpAlgo, SIGNAL(buttonClicked(int)),
            this, SLOT(setAlgo(int)));
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
    _useVaddr = true;
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

    if(_useVaddr) {
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
        _useVaddr = false;
    }

    _blocks.clear();
    _breaks.clear();
    _inst.clear();
    _loopCnt = 0;
    file->disasm(_begin, _end, disasmCallback, this);
    _ui->setLoopCnt(_loopCnt);

    _blocks.clear();
    generateBlocks();

    if(_scene) { _scene->deleteLater(); }
    _scene = new QGraphicsScene(_ui->graphicsView);
    FlowDrawer(_algo, _ui->graphicsView).draw(_scene, outputBlocks());
    _ui->graphicsView->setScene(_scene);
}

void MainWindow::setAlgo(int algo)
{
    _algo = (GVGraph::GVAlgoType)algo;
    updateInfo();
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
        if(block.getJumpTarget() < info.vaddr) {
            window->_loopCnt ++;
        }
        window->_breaks.insert(block.getJumpTarget());
        window->_blocks.clear();
    }
    return 0;
}

void MainWindow::generateBlocks()
{
    Elf64_Addr vaddr = _vBegin;
    Elf64_Addr raddr = _vBegin;
    foreach(const File::DisasmInstInfo &inst, _inst) {
        if(_blocks.empty()) {
            _blocks.push_back(CodeBlock(vaddr, raddr));
        }

        vaddr += inst.size;
        raddr += inst.size;

        CodeBlock &last = _blocks.back();
        if(!last.addInst(inst) || _breaks.find(vaddr) != _breaks.end()) {
            _blocks.push_back(CodeBlock(vaddr, raddr));
        }
    }
    if(_blocks.back().getRepr().isEmpty()) {
        _blocks.removeLast();
    }
}

string MainWindow::outputBlocks()
{
    stringstream ss;
    ss << "digraph G {"<<endl;
    ss << "node [fontname=Courier, group=a]" << endl;
    ss << "splines=ortho" << endl;
    int nodeId = 0;
    foreach(CodeBlock block, _blocks) {
        ss << nodeId << " [shape=box, label=\"" <<
            block.getRepr().toStdString() << "\", comment=" <<
            block.getStartAddr() << "]" << endl;
        int targetBlk = getBlockByStartAddr(block.getJumpTarget());
        if(targetBlk < _blocks.size()) {
            ss << nodeId << "->" << targetBlk << " [constraint=false";
            if(block.getJumpCond() && targetBlk != nodeId+1) {
                ss << ", taillabel=jump";
            }
            ss << "]" << endl;
        }
        if(targetBlk != nodeId+1 && block.getJumpCond() && nodeId < _blocks.size()) {
            ss << nodeId << "->" << nodeId+1 << " [constraint=false, taillabel=normal]" << endl;
        }
        nodeId ++;
    }
    ss << "edge [style=invis]" << endl;
    if(_blocks.size()>1) ss << "0";
    for(int i=1; i<_blocks.size(); i++) {
        ss << "->" << i;
    }
    ss << endl;
    ss << "}" << endl;
    return ss.str();
}

int MainWindow::getBlockByStartAddr(Elf64_Addr addr)
{
    int nodeId = 0;
    foreach(const CodeBlock &block, _blocks) {
        if(block.getStartAddr() == addr) {
            return nodeId;
        }
        nodeId ++;
    }
    return nodeId;
}

END_PLUG_NAMESPACE
