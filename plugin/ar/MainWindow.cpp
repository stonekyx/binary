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

#include <map>
#include <string>
#include <ctime>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;
USE_BIN_NAMESPACE(frontend);
USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(ar)

MainWindow::MainWindow(Plugin *plugin, map<string, string> param,
        QWidget *parent) :
    MWTreeView(new Ui::MainWindow(), plugin, param, parent),
    _ui(dynamic_cast<Ui::MainWindow*>(MWTreeView::_ui)),
    _model(NULL)
{
    QObject::connect(_ui, SIGNAL(changeFileToObj(size_t)),
            this, SLOT(changeFileToObj(size_t)));
    updateInfo();
}

MainWindow::~MainWindow()
{ }

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_model) {
        delete _model;
    }
    _model = new InfoModel(QString(), 2);

    size_t objNum = file->getArhdrNum();
    for(size_t i=0; i<objNum; i++) {
        Arhdr arhdr = file->getArhdr(i);
        _model->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(arhdr.name()));
        time_t date = arhdr.date();
        char *strDate = ctime(&date);
        strDate[strlen(strDate)-1] = 0;
        _model->buildMore(QString("\tDate\t%1")
                .arg(strDate));
        _model->buildMore(QString("\tUID\t%1")
                .arg(arhdr.uid()));
        _model->buildMore(QString("\tGID\t%1")
                .arg(arhdr.gid()));
        _model->buildMore(QString("\tMode\t%1")
                .arg(arhdr.mode()));
        _model->buildMore(QString("\tSize\t0x%1 (%2 KiB)")
                .arg(arhdr.size(), 0, 16)
                .arg((double)arhdr.size()/1024));
        _model->buildMore(QString("\tRaw name\t%1")
                .arg(arhdr.rawName()));
    }
    _ui->infoTree->setModel(_model);
}

void MainWindow::changeFileToObj(size_t objIdx)
{
    _plugin->manager->getBackend()->getFile()->setArObj(objIdx);
}

END_PLUG_NAMESPACE
