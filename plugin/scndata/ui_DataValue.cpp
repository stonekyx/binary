#include "ui_DataValue.h"

BEGIN_PLUG_NAMESPACE(scndata)
namespace Ui {

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

DataValue::DataValue(const QString &labelText) :
    _lblText(labelText)
{
}

void DataValue::setupUi(QWidget *widget)
{
    label = new QLabel(widget);
    OBJNAME(label);

    lineEdit = new QLineEdit(widget);
    OBJNAME(lineEdit);
    lineEdit->setReadOnly(true);

    hboxLayout = new QHBoxLayout(widget);
    hboxLayout->setContentsMargins(0,0,0,0);
    hboxLayout->addWidget(label, 0);
    hboxLayout->addWidget(lineEdit, 0);

    retranslateUi(widget);
}

void DataValue::retranslateUi(QWidget *)
{
    label->setText(_lblText);
}

void DataValue::setValue(const QString &text)
{
    lineEdit->setText(text);
}

}
END_PLUG_NAMESPACE
