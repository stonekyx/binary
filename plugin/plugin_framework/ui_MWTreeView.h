#ifndef PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H
#define PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H

#include "common.h"
#include "ui_MWBase.h"
#include "InfoModel.h"
#include "CtxAction.h"

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QClipboard>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

#include "AutoToolTipDelegate.h"

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

    QMenuBar *menuBar;
    QMenu *menuView;
    QAction *actionViewSearch;
    QAction *actionViewSearchAgain;
    QAction *actionViewExpand;
    QAction *actionViewExpandTo;
    QAction *actionViewCollapse;

    QMainWindow *mainWindow;

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

        mainWindow = MainWindow;

        menuBar = new QMenuBar(MainWindow);
        OBJNAME(menuBar);
        MainWindow->setMenuBar(menuBar);

        menuView = new QMenu(menuBar);
        OBJNAME(menuView);
        menuBar->addAction(menuView->menuAction());

        actionViewSearch = new QAction(MainWindow);
        OBJNAME(actionViewSearch);
        actionViewSearch->setShortcut(QKeySequence::Find);

        actionViewSearchAgain = new QAction(MainWindow);
        OBJNAME(actionViewSearchAgain);
        actionViewSearchAgain->setShortcut(QKeySequence::FindNext);

        actionViewExpand = new QAction(MainWindow);
        OBJNAME(actionViewExpand);

        actionViewExpandTo = new QAction(MainWindow);
        OBJNAME(actionViewExpandTo);

        actionViewCollapse = new QAction(MainWindow);
        OBJNAME(actionViewCollapse);

        menuView->addAction(actionViewSearch);
        menuView->addAction(actionViewSearchAgain);
        menuView->addSeparator();
        menuView->addAction(actionViewExpand);
        menuView->addAction(actionViewExpandTo);
        menuView->addAction(actionViewCollapse);

        if(!infoTree) {
            infoTree = new QTreeView(centralWidget);
        }
        OBJNAME(infoTree);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoTree->sizePolicy().hasHeightForWidth());
        infoTree->setSizePolicy(sizePolicy);
        infoTree->header()->setResizeMode(QHeaderView::ResizeToContents);
        infoTree->header()->setStretchLastSection(true);
        infoTree->setItemDelegate( new AutoToolTipDelegate( infoTree ) );
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

        QObject::connect(
                actionViewSearch, SIGNAL(triggered()),
                this, SLOT(searchTree()));
        QObject::connect(
                actionViewSearchAgain, SIGNAL(triggered()),
                this, SLOT(searchTreeAgain()));
        QObject::connect(
                actionViewExpand, SIGNAL(triggered()),
                this, SLOT(expandTree()));
        QObject::connect(
                actionViewExpandTo, SIGNAL(triggered()),
                this, SLOT(expandTreeTo()));
        QObject::connect(
                actionViewCollapse, SIGNAL(triggered()),
                this, SLOT(collapseTree()));

        retranslateUi(MainWindow);
    }
    void retranslateUi(QMainWindow *) {
        menuView->setTitle(TRANS("&View"));
        actionViewSearch->setText(TRANS("&Search"));
        actionViewSearchAgain->setText(TRANS("Search &next"));
        actionViewExpand->setText(TRANS("&Expand"));
        actionViewExpandTo->setText(TRANS("Expand &to"));
        actionViewCollapse->setText(TRANS("&Collapse"));
    }
public slots:
    virtual void ctxMenuTreeView(const QPoint &pos) {
        foreach(QAction *p, ctxMenu->actions()) {
            p->setData(pos);
            CtxAction *ca = dynamic_cast<CtxAction*>(p);
            if(ca) {
                ca->setVisible(ca->judgeVisible(infoTree->indexAt(pos)));
            }
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
    void searchTree() {
        bool ok;
        QString key = QInputDialog::getText(mainWindow,
                tr("Search"), tr("Search for text"), QLineEdit::Normal,
                QString(), &ok);
        if(!ok) {
            return;
        }
        searchNoPrompt(key);
    }
    void searchTreeAgain() {
        searchNoPrompt(_lastSearchKey);
    }
    void expandTree() {
        infoTree->expandAll();
    }
    void expandTreeTo() {
        bool ok;
        int depth = QInputDialog::getInt(mainWindow,
                tr("Expand to depth"), tr("Depth"), 0, 0, 100, 1, &ok);
        if(ok) {
            infoTree->expandToDepth(depth);
        }
    }
    void collapseTree() {
        infoTree->collapseAll();
    }
private:
    void searchNoPrompt(const QString &key) {
        const QModelIndex &from = infoTree->currentIndex();
        InfoModel *model = dynamic_cast<InfoModel*>(infoTree->model());
        if(!model) {
            QMessageBox::critical(mainWindow, tr("Error"),
                    tr("Can't recognize data model used in tree view!"));
            return;
        }
        if(searchFrom(from, key, model)) {
            return;
        }
        if(QMessageBox::question(mainWindow, tr("Again?"),
                tr("Pattern not found. Search again from beginning?"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok) == QMessageBox::Ok)
        {
            if(searchFrom(infoTree->rootIndex(), key, model)) {
                return;
            }
            QMessageBox::information(mainWindow, tr("Not found"),
                    tr("Pattern not found."));
        }
    }
    bool searchFrom(const QModelIndex &from, const QString &key,
            InfoModel *model)
    {
        QModelIndex i=from;
        if(i==infoTree->rootIndex()) {
            i = model->nextRow(i);
        }
        const QModelIndex begin = i;
        for(; i.isValid(); i=model->nextRow(i)) {
            for(int j=i.column()+
                    (i==begin && i==_lastSearchResult && key==_lastSearchKey);
                    j<model->columnCount(i); j++)
            {
                QString item = model->data(i.sibling(i.row(), j)).toString();
                if(item.contains(key)) {
                    infoTree->clearSelection();
                    infoTree->setCurrentIndex(i.sibling(i.row(), j));
                    _lastSearchResult = i.sibling(i.row(), j);
                    _lastSearchKey = key;
                    return true;
                }
            }
        }
        _lastSearchResult = QModelIndex();
        _lastSearchKey = key;
        return false;
    }
    QModelIndex _lastSearchResult;
    QString _lastSearchKey;
};

#undef TRANS
#undef OBJNAME

#endif
