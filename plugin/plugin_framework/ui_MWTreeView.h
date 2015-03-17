#ifndef PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H
#define PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H

#include "common.h"
#include "ui_MWBase.h"

#include <QtGui/QTreeView>
#include <QtGui/QPushButton>

BEGIN_PLUG_NAMESPACE(plugin_framework)

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

namespace Ui {
    class MWTreeView;
}

class Ui::MWTreeView : public Ui::MWBase {
public:
    QTreeView *infoTree;
    QPushButton *btnExpand;

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
        infoTree->hide();

        gridLayout->addWidget(infoTree, 0, 0, 1, 1);
    }
    virtual void retranslateUi(QMainWindow *window) {
        MWBase::retranslateUi(window);
    }
};

#undef TRANS
#undef OBJNAME

END_PLUG_NAMESPACE

#endif
