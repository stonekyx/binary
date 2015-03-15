#include <QtGui/QHeaderView>

#include "ui_MWTreeView.h"

#include "MWTreeView.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(plugin_framework)

MWTreeView::MWTreeView(Ui::MWTreeView *ui,
        BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param,
        QWidget *parent) :
    MWBase(ui, plugin, param, parent),
    _ui(ui)
{
    _ui->infoTree->header()->resizeSection(0, 180);
}

MWTreeView::~MWTreeView()
{ }

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
