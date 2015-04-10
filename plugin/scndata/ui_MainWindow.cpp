#include "ui_MainWindow.h"

BEGIN_PLUG_NAMESPACE(scndata)
namespace Ui {

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

bool MainWindow::switchMode(bool file)
{
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

void MainWindow::setupUi(QMainWindow *window)
{
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

Elf64_Off MainWindow::parseAddr(int cursorPos)
{
    ScnDataTextEdit *edit = addrTextEdit;
    QTextDocument *doc = edit->document();
    if(doc->characterCount() == 0) {
        return 0;
    }
    QTextCursor cursor(doc);
    cursor.setPosition(cursorPos);
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    return cursor.selectedText().toULong(NULL, 16);
}

}
END_PLUG_NAMESPACE
