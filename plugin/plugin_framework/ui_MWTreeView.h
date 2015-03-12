#ifndef PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H
#define PLUGIN_PLUGIN_FRAMEWORK_UI_MWTREEVIEW_H

#include "common.h"

#include <QtGui/QPushButton>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QTreeView>

BEGIN_PLUG_NAMESPACE(plugin_framework)

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

namespace Ui {
    class MWTreeView;
}

class Ui::MWTreeView {
private:
    const char *_context;
    const char *_title;

public:
    QWidget *centralWidget;
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QTreeView *infoTree;
    QLabel *defaultLabel;

    MWTreeView(const char *context, const char *title) :
        _context(context),
        _title(title)
    {}

    void setupUi(QMainWindow *MainWindow) {
        if(MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8(_context));
        MainWindow->resize(400, 360);

        centralWidget = new QWidget(MainWindow);
        OBJNAME(centralWidget);
        MainWindow->setCentralWidget(centralWidget);

        infoTree = new QTreeView(centralWidget);
        OBJNAME(infoTree);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoTree->sizePolicy().hasHeightForWidth());
        infoTree->setSizePolicy(sizePolicy);
        infoTree->hide();

        defaultLabel = new QLabel(centralWidget);
        OBJNAME(defaultLabel);
        sizePolicy.setHeightForWidth(defaultLabel->sizePolicy().hasHeightForWidth());
        defaultLabel->setSizePolicy(sizePolicy);
        defaultLabel->setAlignment(Qt::AlignCenter);
        defaultLabel->show();

        gridLayout = new QGridLayout();
        OBJNAME(gridLayout);
        gridLayout->addWidget(infoTree, 0, 0, 1, 1);
        gridLayout->addWidget(defaultLabel, 0, 0, 1, 1);

        vboxLayout = new QVBoxLayout(centralWidget);
        OBJNAME(vboxLayout);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setSpacing(6);
        vboxLayout->addLayout(gridLayout);

        retranslateUi(MainWindow);
    }
    void retranslateUi(QMainWindow *MainWindow) {
        MainWindow->setWindowTitle(TRANS(_title));
        defaultLabel->setText(TRANS("No file opened."));
    }
};

#undef TRANS
#undef OBJNAME

END_PLUG_NAMESPACE

#endif
