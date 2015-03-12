#include <iostream>
#include <cstdio>
#include <QKeyEvent>
#include <QtGui/QHeaderView>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"

#include "ui_MWTreeView.h"

#include "MWTreeView.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(plugin_framework)

MWTreeView::MWTreeView(Ui::MWTreeView *ui,
        BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param,
        QWidget *parent) :
    QMainWindow(parent),
    _ui(ui),
    _plugin(plugin)
{
    _ui->setupUi(this);
    _ui->infoTree->header()->resizeSection(0, 180);
    QObject::connect(_plugin->manager->getBackend(),
            SIGNAL(fileChanged(binary::backend::File *)),
            this, SLOT(updateInfo(binary::backend::File *)));
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MWTreeView::~MWTreeView()
{
    delete _ui;
}

void MWTreeView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W:
        if(event->modifiers() & Qt::ControlModifier)
            close();
        break;
    default:
        break;
    }
}

void MWTreeView::updateInfo(File *file)
{
    if(!file) {
        _ui->infoTree->hide();
        _ui->defaultLabel->show();
        return;
    }
    _ui->defaultLabel->hide();
    _ui->infoTree->show();
}

END_PLUG_NAMESPACE
