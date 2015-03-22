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

    FileLayout *fileLayout;
    FileLayout *scnLayout;
    FileLayout *sgmLayout;
    QLabel *lblFileLayout;
    QLabel *lblScnLayout;
    QLabel *lblSgmLayout;

    virtual bool switchMode(bool file) {
        if(!file) {
            fileLayout->hide();
            scnLayout->hide();
            sgmLayout->hide();
            lblFileLayout->hide();
            lblScnLayout->hide();
            lblSgmLayout->hide();
            defaultLabel->show();
            return false;
        }
        defaultLabel->hide();
        fileLayout->show();
        scnLayout->show();
        sgmLayout->show();
        lblFileLayout->show();
        lblScnLayout->show();
        lblSgmLayout->show();
        return true;
    }

    virtual void setupUi(QMainWindow *window) {
        MWBase::setupUi(window);
        window->resize(600, 400);

        fileLayout = new FileLayout(FileLayout::LAYOUT_FILE, NULL, centralWidget);
        OBJNAME(fileLayout);

        scnLayout = new FileLayout(FileLayout::LAYOUT_SCN, NULL, centralWidget);
        OBJNAME(scnLayout);

        sgmLayout = new FileLayout(FileLayout::LAYOUT_SGM, NULL, centralWidget);
        OBJNAME(sgmLayout);

        fileLayout->setRef(scnLayout);
        scnLayout->setRef(sgmLayout);
        sgmLayout->setRef(fileLayout);

        lblFileLayout = new QLabel(centralWidget);
        OBJNAME(lblFileLayout);
        lblScnLayout = new QLabel(centralWidget);
        OBJNAME(lblScnLayout);
        lblSgmLayout = new QLabel(centralWidget);
        OBJNAME(lblSgmLayout);

        gridLayout->setRowMinimumHeight(2, 60);
        gridLayout->setRowMinimumHeight(5, 60);
        lblFileLayout->setFixedHeight(30);
        lblScnLayout->setFixedHeight(30);
        lblSgmLayout->setFixedHeight(30);
        gridLayout->addWidget(lblFileLayout, 0, 0, 1, 1);
        gridLayout->addWidget(fileLayout, 1, 0, 1, 1);
        gridLayout->addWidget(lblScnLayout, 3, 0, 1, 1);
        gridLayout->addWidget(scnLayout, 4, 0, 1, 1);
        gridLayout->addWidget(lblSgmLayout, 6, 0, 1, 1);
        gridLayout->addWidget(sgmLayout, 7, 0, 1, 1);

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
        lblFileLayout->setText(TRANS("File layout"));
        lblScnLayout->setText(TRANS("Section layout"));
        lblSgmLayout->setText(TRANS("Segment layout"));
    }
};

#undef OBJNAME
#undef TRANS

END_PLUG_NAMESPACE

#endif
