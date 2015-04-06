#include <elf.h>

#include "backend/File.h"
#include "backend/Backend.h"
#include "frontend/PluginManager.h"
#include "backend/ConvertAddr.h"
#include "ui_MainWindow.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(disasm)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MainWindow(), plugin, param, parent),
    _infoModel(NULL),
    _loadWorker(NULL),
    _ui(dynamic_cast<Ui::MainWindow*>(MWTreeView::_ui))
{
    QObject::connect(_ui, SIGNAL(signalStopDisasm()),
            this, SLOT(resetWorker()));
    QObject::connect(_ui, SIGNAL(signalRefreshDisasm()),
            this, SLOT(updateInfo()));
    QObject::connect(_ui, SIGNAL(signalRangeChange(size_t, size_t)),
            this, SLOT(setRange(size_t, size_t)));
    QObject::connect(_ui, SIGNAL(signalVaddrRangeChange(size_t, size_t)),
            this, SLOT(setVaddrRange(size_t, size_t)));
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    _useRange = _useVRange = false;
    _begin = _end = 0;
    if(param.find("vBegin") != param.end() &&
            param.find("vEnd") != param.end())
    {
        _useRange = true;
        _useVRange = true;
        _begin = QString(param["vBegin"].c_str()).toULong();
        _end = QString(param["vEnd"].c_str()).toULong();
    }
    QFont font = _ui->infoTree->font();
    font.setFamily("Courier");
    font.setPointSize(9);
    _ui->infoTree->setFont(font);
    updateInfo();
}

MainWindow::~MainWindow()
{
    resetWorker();
    if(_infoModel) {
        delete _infoModel;
    }
}

void MainWindow::updateInfo(File *file)
{
    resetWorker();
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 5, NULL);
    _ui->infoTree->setModel(_infoModel);

    ConvertAddr convertAddr(file);

    if(_useRange) {
        if(_useVRange) {
            size_t fBegin=0, fEnd=0;
            convertAddr.vaddrToFileOff(fBegin, _begin);
            convertAddr.vaddrToFileOff(fEnd, _end);
            _begin = fBegin;
            _end = fEnd;
            _useVRange = false;
        }
        _ui->setRange(_begin, _end);
        size_t _vBegin=0, _vEnd=0;
        convertAddr.fileOffToVaddr(_vBegin, _begin);
        convertAddr.fileOffToVaddr(_vEnd, _end);
        _ui->setVaddrRange(_vBegin, _vEnd);
        if(_begin < _end) {
            _loadWorker = new LoadWorker(_begin, _end, file, _infoModel);
        }
    } else if(_scnIndex != 0) {
        Elf64_Shdr shdr;
        if(!file->getShdr(_scnIndex, &shdr)) {
            QMessageBox::critical(this, tr("Error"),
                    tr("Can't find specified section!"));
            close();
            return;
        }
        _ui->setRange(shdr.sh_offset, shdr.sh_offset + shdr.sh_size);
        _ui->setVaddrRange(shdr.sh_addr, shdr.sh_addr + shdr.sh_size);
        _loadWorker = new LoadWorker(
                shdr.sh_offset, shdr.sh_offset + shdr.sh_size,
                file, _infoModel);
    } else {
        _loadWorker = new LoadWorker(file, _infoModel);
    }

    if(_loadWorker) {
        QObject::connect(_loadWorker, SIGNAL(symbolStarted(QModelIndex)),
                this, SLOT(spanFirstColumn(QModelIndex)));
        QObject::connect(_loadWorker, SIGNAL(started()),
                _ui, SLOT(changeStatus()));
        QObject::connect(_loadWorker, SIGNAL(finished()),
                _ui, SLOT(changeStatus()));
        QObject::connect(_loadWorker, SIGNAL(terminated()),
                _ui, SLOT(changeStatus()));
        _loadWorker->start();
    }
}

void MainWindow::spanFirstColumn(QModelIndex index)
{
    _ui->infoTree->setFirstColumnSpanned(index.row(), index.parent(),
            true);
}

void MainWindow::resetWorker()
{
    if(!_loadWorker) {
        return;
    }
    _loadWorker->terminate();
    _loadWorker->wait(1000);
    _loadWorker->deleteLater();
    _loadWorker = NULL;
}

void MainWindow::setRange(size_t begin, size_t end)
{
    if(!_useRange || begin != _begin || end != _end) {
        _useRange = true;
        _useVRange = false;
        _begin = begin;
        _end = end;
        updateInfo();
    }
}

void MainWindow::setVaddrRange(size_t begin, size_t end)
{
    size_t fBegin = 0, fEnd = 0;
    ConvertAddr convertAddr(_plugin->manager->getBackend()->getFile());
    convertAddr.vaddrToFileOff(fBegin, begin);
    convertAddr.vaddrToFileOff(fEnd, end);
    if(!_useRange || fBegin != _begin || fEnd != _end) {
        _useRange = true;
        _useVRange = false;
        _begin = fBegin;
        _end = fEnd;
        updateInfo();
    }
}

END_PLUG_NAMESPACE
