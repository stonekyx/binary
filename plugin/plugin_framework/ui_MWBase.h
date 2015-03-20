#ifndef PLUGIN_PLUGIN_FRAMEWORK_UI_MWBASE_H
#define PLUGIN_PLUGIN_FRAMEWORK_UI_MWBASE_H

#include "common.h"

#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

BEGIN_PLUG_NAMESPACE(plugin_framework)

namespace Ui {
    class MWBase;
}

END_PLUG_NAMESPACE

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

class binary::plugin::plugin_framework::Ui::MWBase : public QObject {
protected:
    const char *_context;
    const char *_title;

public:
    QWidget *centralWidget;
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *defaultLabel;

    MWBase(const char *context, const char *title) :
        _context(context),
        _title(title)
    {}
    virtual ~MWBase() {}

    virtual bool switchMode(bool) = 0;

    virtual void setupUi(QMainWindow *MainWindow) {
        if(MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8(_context));
        MainWindow->resize(400, 360);

        centralWidget = new QWidget(MainWindow);
        OBJNAME(centralWidget);
        MainWindow->setCentralWidget(centralWidget);

        defaultLabel = new QLabel(centralWidget);
        OBJNAME(defaultLabel);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(defaultLabel->sizePolicy().hasHeightForWidth());
        defaultLabel->setSizePolicy(sizePolicy);
        defaultLabel->setAlignment(Qt::AlignCenter);
        defaultLabel->show();

        vboxLayout = new QVBoxLayout(centralWidget);
        OBJNAME(vboxLayout);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setSpacing(6);

        gridLayout = new QGridLayout();
        vboxLayout->addLayout(gridLayout);
        OBJNAME(gridLayout);
        gridLayout->addWidget(defaultLabel, 0, 0, 1, 1);

        retranslateUi(MainWindow);
    }
    void retranslateUi(QMainWindow *MainWindow) {
        MainWindow->setWindowTitle(TRANS(_title));
        defaultLabel->setText(TRANS("No file opened."));
    }
};

#undef TRANS
#undef OBJNAME

#endif
