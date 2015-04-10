#ifndef PLUGIN_SCNDATA_SCNDATATEXTEDIT_H
#define PLUGIN_SCNDATA_SCNDATATEXTEDIT_H

#include <QtGui/QTextEdit>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class ScnDataTextEdit;

END_PLUG_NAMESPACE

class binary::plugin::scndata::ScnDataTextEdit : public QTextEdit {
    Q_OBJECT
public:
    ScnDataTextEdit(QWidget *parent = NULL);
    void markCursor(int start, int end, bool recover = false);
    void unmarkCursor();
protected:
    virtual void focusOutEvent(QFocusEvent *e);
protected slots:
    void calcCursorPos();
signals:
    void offsetMapped(size_t, size_t);
private:
    QTextCursor _mark;
};

#endif
