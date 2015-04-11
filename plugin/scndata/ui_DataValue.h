#ifndef PLUGIN_SCNDATA_UI_DATAVALUE_H
#define PLUGIN_SCNDATA_UI_DATAVALUE_H

#include <elf.h>

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

namespace Ui {
    class DataValue;
}

END_PLUG_NAMESPACE

class binary::plugin::scndata::Ui::DataValue : public QObject
{
    Q_OBJECT
public:
    DataValue(const QString &);

    QHBoxLayout *hboxLayout;
    QLabel *label;
    QLineEdit *lineEdit;

    virtual void setupUi(QWidget *);
    void retranslateUi(QWidget *);
    void setValue(const QString &);
private:
    QString _lblText;
};

#endif
