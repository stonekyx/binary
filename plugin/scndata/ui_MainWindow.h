#ifndef PLUGIN_SCNDATA_UI_MAINWINDOW_H
#define PLUGIN_SCNDATA_UI_MAINWINDOW_H

#include <QtCore/QList>
#include <QtGui/QTextEdit>
#include <QtGui/QScrollBar>
#include <QtGui/QAbstractSlider>

#include "ui_MWBase.h"

BEGIN_PLUG_NAMESPACE(scndata)

namespace Ui {
    class MainWindow;
}

class ScnDataTextEdit : public QTextEdit {
public:
    ScnDataTextEdit(QWidget *parent = NULL) : QTextEdit(parent)
    {}
    void markCursor(int start, int end, bool recover = false) {
        QTextCursor bkp(textCursor());
        if(!_mark.isNull()) {
            setTextCursor(_mark);
            setTextBackgroundColor(Qt::white);
        }
        _mark = bkp;
        _mark.setPosition(start);
        _mark.setPosition(end, QTextCursor::KeepAnchor);
        setTextCursor(_mark);
        setTextBackgroundColor(Qt::lightGray);
        if(recover) {
            setTextCursor(bkp);
        }
    }
    void unmarkCursor() {
        QTextCursor bkp(textCursor());
        if(!_mark.isNull()) {
            setTextCursor(_mark);
            setTextBackgroundColor(Qt::white);
        }
        setTextCursor(bkp);
    }
protected:
    virtual void focusOutEvent(QFocusEvent *e) {
        bool oldState = blockSignals(true);
        unmarkCursor();
        blockSignals(oldState);
        QTextEdit::focusOutEvent(e);
    }
private:
    QTextCursor _mark;
};

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

class Ui::MainWindow :
    public PLUG_NAMESPACE(plugin_framework)::Ui::MWBase
{
public:
    MainWindow() : MWBase("PluginScnDataMainWindow", "Section data") {}

    ScnDataTextEdit *hexTextEdit;
    ScnDataTextEdit *rawTextEdit;
    ScnDataTextEdit *addrTextEdit;
    QList<ScnDataTextEdit*> textEdits;

    virtual bool switchMode(bool file) {
        if(!file) {
            foreach(ScnDataTextEdit *p, textEdits) {
                p->hide();
            }
            defaultLabel->show();
            return false;
        }
        defaultLabel->hide();
        foreach(ScnDataTextEdit *p, textEdits) {
            p->show();
            p->clear();
        }
        return true;
    }

    virtual void setupUi(QMainWindow *window) {
        MWBase::setupUi(window);

        window->resize(650, 360);
        window->setMinimumWidth(650);
        window->setMinimumHeight(360);

        hexTextEdit = new ScnDataTextEdit(centralWidget);
        OBJNAME(hexTextEdit);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(5);
        sizePolicy.setHeightForWidth(hexTextEdit->sizePolicy().hasHeightForWidth());
        hexTextEdit->setSizePolicy(sizePolicy);
        hexTextEdit->setReadOnly(true);
        hexTextEdit->setTextInteractionFlags(
                hexTextEdit->textInteractionFlags() |
                Qt::TextSelectableByKeyboard);

        rawTextEdit = new ScnDataTextEdit(centralWidget);
        OBJNAME(rawTextEdit);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(rawTextEdit->sizePolicy().hasHeightForWidth());
        rawTextEdit->setSizePolicy(sizePolicy);
        rawTextEdit->setReadOnly(true);
        rawTextEdit->setTextInteractionFlags(
                rawTextEdit->textInteractionFlags() |
                Qt::TextSelectableByKeyboard);

        addrTextEdit = new ScnDataTextEdit(centralWidget);
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

        gridLayout->addWidget(hexTextEdit, 0, 1, 1, 1);
        gridLayout->addWidget(rawTextEdit, 0, 2, 1, 1);
        gridLayout->addWidget(addrTextEdit, 0, 0, 1, 1);

        textEdits.push_back(hexTextEdit);
        textEdits.push_back(rawTextEdit);
        textEdits.push_back(addrTextEdit);

        for(int i=1; i<textEdits.size(); i++) {
            QObject::connect(textEdits[i]->verticalScrollBar(),
                    SIGNAL(valueChanged(int)),
                    textEdits[i-1]->verticalScrollBar(),
                    SLOT(setValue(int)));
            QObject::connect(textEdits[i-1]->verticalScrollBar(),
                    SIGNAL(valueChanged(int)),
                    textEdits[i]->verticalScrollBar(),
                    SLOT(setValue(int)));
        }
        foreach(ScnDataTextEdit *p, textEdits) {
            p->ensureCursorVisible();
        }

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
    }
};

#undef OBJNAME
#undef TRANS

END_PLUG_NAMESPACE

#endif
