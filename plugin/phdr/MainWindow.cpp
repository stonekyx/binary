#include <elf.h>
#include <QtCore/QTextStream>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(phdr)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginPhdrMainWindow", "Program header"), plugin, param, parent),
    _infoModel(NULL)
{
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

#define HEX(val) (QString("0x%1").arg(val, 0, 16))

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }

    QString modelData;
    QTextStream textStream(&modelData);

    Elf64_Phdr phdr;
    size_t phdrNum;
    if(file->getPhdrNum(&phdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<phdrNum; i++) {
        if(!file->getPhdr(i, &phdr)) {
            continue;
        }
        if(!modelData.isEmpty()) {
            textStream << "\n";
        }
        textStream << "Entry " << i+1 << "\t" << Defines::commentText_PT(phdr.p_type);

        textStream << "\n\tFlags\t" << HEX(phdr.p_flags);
        char *buf = new char[Defines::commentText_PF(phdr.p_flags, NULL)+1];
        Defines::commentText_PF(phdr.p_flags, buf);
        textStream << " (" << buf << ")";
        delete[] buf;

        textStream << "\n\tOffset\t" << HEX(phdr.p_offset);
        textStream << "\n\tVirtual address\t" << HEX(phdr.p_vaddr);
        textStream << "\n\tPhysical address\t" << HEX(phdr.p_paddr);
        textStream << "\n\tSize in file\t" << HEX(phdr.p_filesz);
        textStream << "\n\tSize in memory\t" << HEX(phdr.p_memsz);
        textStream << "\n\tAlignment\t" << HEX(phdr.p_align);
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(modelData, 2);
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
