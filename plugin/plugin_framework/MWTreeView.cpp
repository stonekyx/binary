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
}

MWTreeView::~MWTreeView()
{ }

void MWTreeView::updateInfo(File *file)
{
    _ui->switchMode(file);
}

END_PLUG_NAMESPACE
