#ifndef PLUGIN_SCNDATA_UI_MAINWINDOW_H
#define PLUGIN_SCNDATA_UI_MAINWINDOW_H

#include <QtGui/QTextEdit>
#include <QtGui/QScrollBar>
#include <QtGui/QAbstractSlider>

#include "ui_MWBase.h"

BEGIN_PLUG_NAMESPACE(scndata)

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
    MainWindow() : MWBase("PluginScnDataMainWindow", "Section data") {}

    QTextEdit *textEdit;
    QTextEdit *rawTextEdit;
    QTextEdit *addrTextEdit;

    virtual void setupUi(QMainWindow *window) {
        MWBase::setupUi(window);

        window->resize(650, 360);

        textEdit = new QTextEdit(centralWidget);
        OBJNAME(textEdit);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(5);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);
        textEdit->setReadOnly(true);
        textEdit->setTextInteractionFlags(
                textEdit->textInteractionFlags() |
                Qt::TextSelectableByKeyboard);

        rawTextEdit = new QTextEdit(centralWidget);
        OBJNAME(rawTextEdit);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(rawTextEdit->sizePolicy().hasHeightForWidth());
        rawTextEdit->setSizePolicy(sizePolicy);
        rawTextEdit->setReadOnly(true);
        rawTextEdit->setTextInteractionFlags(
                rawTextEdit->textInteractionFlags() |
                Qt::TextSelectableByKeyboard);

        addrTextEdit = new QTextEdit(centralWidget);
        OBJNAME(addrTextEdit);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(addrTextEdit->sizePolicy().hasHeightForWidth());
        addrTextEdit->setSizePolicy(sizePolicy);
        addrTextEdit->setReadOnly(true);
        addrTextEdit->setTextInteractionFlags(
                addrTextEdit->textInteractionFlags() |
                Qt::TextSelectableByKeyboard);
        addrTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(textEdit, 0, 1, 1, 1);
        gridLayout->addWidget(rawTextEdit, 0, 2, 1, 1);
        gridLayout->addWidget(addrTextEdit, 0, 0, 1, 1);

        const QAbstractSlider *textEditSlider =
            textEdit->verticalScrollBar();
        const QAbstractSlider *rawTextEditSlider =
            rawTextEdit->verticalScrollBar();
        const QAbstractSlider *addrTextEditSlider =
            addrTextEdit->verticalScrollBar();
        QObject::connect(textEditSlider, SIGNAL(valueChanged(int)),
                rawTextEditSlider, SLOT(setValue(int)));
        QObject::connect(rawTextEditSlider, SIGNAL(valueChanged(int)),
                textEditSlider, SLOT(setValue(int)));
        QObject::connect(rawTextEditSlider, SIGNAL(valueChanged(int)),
                addrTextEditSlider, SLOT(setValue(int)));
        QObject::connect(addrTextEditSlider, SIGNAL(valueChanged(int)),
                rawTextEditSlider, SLOT(setValue(int)));
    }
    virtual void retranslateUi(QMainWindow *window) {
        MWBase::retranslateUi(window);
    }
};

#undef OBJNAME
#undef TRANS

END_PLUG_NAMESPACE

#endif
