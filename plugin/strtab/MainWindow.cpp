/*
 * Copyright 2015 KANG Yuxuan
 *
 * MainWindow.cpp by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <elf.h>
#include <QtGui/QMessageBox>

#include "backend/File.h"
#include "ui_MWTreeView.h"

#include "MainWindow.h"
#include "DemangleWrap.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(strtab)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginStrTabMainWindow", "String table"), plugin, param, parent),
    _infoModel(NULL)
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    updateInfo();
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        _ui->defaultLabel->setText(tr("No file opened."));
        return;
    }
    if(_scnIndex == 0 && file->getShdrStrNdx(&_scnIndex) != 0) {
        _ui->switchMode(false);
        QMessageBox::critical(this, tr("Error"),
                tr("String table not found!"));
        close();
        return;
    }
    Elf64_Shdr shdr;
    if(!file->getShdr(_scnIndex, &shdr)) {
        _ui->switchMode(false);
        QMessageBox::critical(this, tr("Error"),
                tr("Failed to get section header!"));
        close();
        return;
    }
    if(!(shdr.sh_flags & SHF_STRINGS) && shdr.sh_type != SHT_STRTAB) {
        if(QMessageBox::warning(this, tr("Warning"),
                tr("This section may not contain nul-terminated strings!"
                    "\nDo you want to continue?"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok) != QMessageBox::Ok)
        {
            close();
            return;
        }
    }
    char *buf = new char[shdr.sh_size];
    ssize_t got = file->getScnData(_scnIndex, buf, shdr.sh_size);
    if(got == -1 || (size_t)got != shdr.sh_size) {
        QMessageBox::critical(this, tr("Error"),
                tr("Error getting section data!"));
        close();
        return;
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);
    int entryCount = 0;
    QString line;

#define BL(C) do{ \
    line.clear();line.append(C);_infoModel->buildMore(line);}while(0)

    for(size_t i=(buf[0]==0); i<shdr.sh_size-1; i+=strlen(buf+i)+1) {
        BL(QString("Entry %1\t%2").arg(++entryCount).arg(QString(buf+i)));
        BL(QString("\tOffset\t%1").arg(i));
        char *demangle = cplus_demangle(buf+i);
        BL(QString("\tDemangle\t%1").arg(QString(demangle)));
        free(demangle);
    }
    delete[] buf;

    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
