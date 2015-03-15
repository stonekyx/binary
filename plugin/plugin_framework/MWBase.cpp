#include <iostream>
#include <cstdio>
#include <QKeyEvent>
#include <QtGui/QHeaderView>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"

#include "ui_MWBase.h"

#include "MWBase.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(plugin_framework)

MWBase::MWBase(Ui::MWBase *ui,
        BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string>,
        QWidget *parent) :
    QMainWindow(parent),
    _ui(ui),
    _plugin(plugin)
{
    _ui->setupUi(this);
    QObject::connect(_plugin->manager->getBackend(),
            SIGNAL(fileChanged(binary::backend::File *)),
            this, SLOT(updateInfo(binary::backend::File *)));
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MWBase::~MWBase()
{
    delete _ui;
}

void MWBase::keyPressEvent(QKeyEvent *event)
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

void MWBase::updateInfo(File *)
{
}

END_PLUG_NAMESPACE
