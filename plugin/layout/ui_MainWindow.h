#ifndef PLUGIN_LAYOUT_UI_MAINWINDOW_H
#define PLUGIN_LAYOUT_UI_MAINWINDOW_H

#include "ui_MWBase.h"
#include "FileLayout.h"

BEGIN_PLUG_NAMESPACE(layout)

namespace Ui {
    class MainWindow;
}

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

class Ui::MainWindow :
    public PLUG_NAMESPACE(plugin_framework)::Ui::MWBase
{
public:
    MainWindow() : MWBase("PluginLayoutMainWindow", "File layout") {}

    FileLayout *scnLayout;
    FileLayout *sgmLayout;
    QLabel *lblScnLayout;
    QLabel *lblSgmLayout;

    virtual bool switchMode(bool file) {
        if(!file) {
            scnLayout->hide();
            sgmLayout->hide();
            lblScnLayout->hide();
            lblSgmLayout->hide();
            defaultLabel->show();
            return false;
        }
        defaultLabel->hide();
        scnLayout->show();
        sgmLayout->show();
        lblScnLayout->show();
        lblSgmLayout->show();
        return true;
    }

    virtual void setupUi(QMainWindow *window) {
        MWBase::setupUi(window);
        window->resize(600, 300);

        scnLayout = new FileLayout(FileLayout::LAYOUT_SCN, NULL, centralWidget);
        OBJNAME(scnLayout);

        sgmLayout = new FileLayout(FileLayout::LAYOUT_SGM, NULL, centralWidget);
        OBJNAME(sgmLayout);

        scnLayout->setRef(sgmLayout);
        sgmLayout->setRef(scnLayout);

        lblScnLayout = new QLabel(centralWidget);
        OBJNAME(lblScnLayout);
        lblSgmLayout = new QLabel(centralWidget);
        OBJNAME(lblSgmLayout);

        gridLayout->setRowMinimumHeight(2, 60);
        lblScnLayout->setFixedHeight(30);
        lblSgmLayout->setFixedHeight(30);
        gridLayout->addWidget(lblScnLayout, 0, 0, 1, 1);
        gridLayout->addWidget(scnLayout, 1, 0, 1, 1);
        gridLayout->addWidget(sgmLayout, 3, 0, 1, 1);
        gridLayout->addWidget(lblSgmLayout, 4, 0, 1, 1);

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
        lblScnLayout->setText(TRANS("Section layout"));
        lblSgmLayout->setText(TRANS("Segment layout"));
    }
};

#undef OBJNAME
#undef TRANS

END_PLUG_NAMESPACE

#endif
