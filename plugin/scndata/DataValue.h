#ifndef PLUGIN_SCNDATA_DATAVALUE_H
#define PLUGIN_SCNDATA_DATAVALUE_H

#include <QtCore/QString>
#include <QtGui/QWidget>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class DataValue;
class DataValueConvert;

END_PLUG_NAMESPACE

class binary::plugin::scndata::DataValue : public QWidget {
    Q_OBJECT
public:
    explicit DataValue(const QString &, DataValueConvert *,
            QWidget * = NULL);
    ~DataValue();
    void clear();
public slots:
    void setValue(const char *, size_t);
private:
    class Private;
    Private *_priv;
};

#endif
