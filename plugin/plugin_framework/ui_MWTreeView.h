#ifndef PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H
#define PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H

#include "common.h"
#include "ui_MWBase.h"

#include <QtGui/QMenu>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QClipboard>
#include <QtGui/QHeaderView>

BEGIN_PLUG_NAMESPACE(plugin_framework)

namespace Ui {
    class MWTreeView;
}

END_PLUG_NAMESPACE

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

class binary::plugin::plugin_framework::Ui::MWTreeView :
    public Ui::MWBase {
    Q_OBJECT
public:
    QTreeView *infoTree;
    QPushButton *btnExpand;
    QMenu *ctxMenu;

    MWTreeView(const char *context, const char *title) :
        MWBase(context, title)
    {}
    virtual ~MWTreeView() {}

    virtual bool switchMode(bool file) {
        if(!file) {
            infoTree->hide();
            defaultLabel->show();
            return false;
        }
        defaultLabel->hide();
        infoTree->show();
        return true;
    }

    virtual void setupUi(QMainWindow *MainWindow) {
        MWBase::setupUi(MainWindow);

        infoTree = new QTreeView(centralWidget);
        OBJNAME(infoTree);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoTree->sizePolicy().hasHeightForWidth());
        infoTree->setSizePolicy(sizePolicy);
        infoTree->header()->setResizeMode(QHeaderView::ResizeToContents);
        infoTree->header()->setStretchLastSection(false);
        infoTree->hide();

        gridLayout->addWidget(infoTree, 0, 0, 1, 1);

        ctxMenu = new QMenu(infoTree);
        QAction *actionCopy = ctxMenu->addAction(
                tr("Copy"),
                this, SLOT(copyCell()));
        actionCopy->setParent(ctxMenu);

        infoTree->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(
                infoTree,
                SIGNAL(customContextMenuRequested(const QPoint &)),
                this,
                SLOT(ctxMenuTreeView(const QPoint &)));
    }
    virtual void retranslateUi(QMainWindow *window) {
        MWBase::retranslateUi(window);
    }
public slots:
    virtual void ctxMenuTreeView(const QPoint &pos) {
        foreach(QAction *p, ctxMenu->actions()) {
            p->setData(pos);
        }
        ctxMenu->exec(infoTree->mapToGlobal(pos));
    }
    virtual void copyCell() {
        QAction *action = dynamic_cast<QAction*>(sender());
        if(!action) {
            return;
        }
        QModelIndex index = infoTree->indexAt(action->data().toPoint());
        QApplication::clipboard()->setText(
                infoTree->model()->data(index).toString());
        QApplication::clipboard()->setText(
                infoTree->model()->data(index).toString(),
                QClipboard::Selection);
    }
};

#undef TRANS
#undef OBJNAME

#endif
