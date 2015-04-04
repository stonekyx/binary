#ifndef PLUGIN_FRAMEWORK_AUTO_TOOLTIP_DELEGATE_H
#define PLUGIN_FRAMEWORK_AUTO_TOOLTIP_DELEGATE_H

#include <QtCore/QObject>
#include <QtGui/QStyledItemDelegate>

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

class AutoToolTipDelegate;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::AutoToolTipDelegate :
    public QStyledItemDelegate
{
    Q_OBJECT
public:
    AutoToolTipDelegate(QObject*);
    ~AutoToolTipDelegate();

public slots:
    bool helpEvent(QHelpEvent* e, QAbstractItemView* view,
        const QStyleOptionViewItem& option, const QModelIndex& index);
};

#endif
