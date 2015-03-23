#ifndef PLUGIN_AR_UI_MAINWINDOW_H
#define PLUGIN_AR_UI_MAINWINDOW_H

#include "ui_MWTreeView.h"

BEGIN_PLUG_NAMESPACE(ar)

namespace Ui {
    class MainWindow;
}

END_PLUG_NAMESPACE

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

class binary::plugin::ar::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWTreeView
{
    Q_OBJECT
public:
    MainWindow() : MWTreeView("PluginArMainWindow", "Ar iterator") {}

    virtual void setupUi(QMainWindow *window) {
        MWTreeView::setupUi(window);

        QAction *actionSetObj = ctxMenu->addAction(tr("Set object"),
                this, SLOT(setObj()));
        actionSetObj->setParent(ctxMenu);

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
    }
public slots:
    void setObj() {
        QAction *action = dynamic_cast<QAction*>(sender());
        if(!action) {
            return;
        }
        QModelIndex index = infoTree->indexAt(action->data().toPoint());
        while(index.parent() != infoTree->rootIndex()) {
            index = index.parent();
        }
        emit changeFileToObj(index.row());
    }
signals:
    void changeFileToObj(size_t);
};

#undef OBJNAME
#undef TRANS

#endif
