#include "DataValueConvert.h"
#include "ui_DataValue.h"

#include "DataValue.h"

BEGIN_PLUG_NAMESPACE(scndata)

class DataValue::Private {
public:
    Ui::DataValue *_ui;
    DataValueConvert *_dvc;
};

DataValue::DataValue(const QString &lblText, DataValueConvert *dvc,
        QWidget *parent) :
    QWidget(parent), _priv(new Private())
{
    _priv->_ui = new Ui::DataValue(lblText);
    _priv->_dvc = dvc;
    _priv->_ui->setupUi(this);
}

DataValue::~DataValue()
{
    delete _priv->_ui;
    delete _priv->_dvc;
    delete _priv;
}

void DataValue::clear()
{
    _priv->_ui->setValue(QString());
}

void DataValue::setValue(const char *data, size_t size)
{
    if(!_priv->_dvc) {
        return;
    }
    _priv->_ui->setValue(_priv->_dvc->toString(data, size));
}

END_PLUG_NAMESPACE
