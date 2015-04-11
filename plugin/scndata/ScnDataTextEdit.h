#ifndef PLUGIN_SCNDATA_SCNDATATEXTEDIT_H
#define PLUGIN_SCNDATA_SCNDATATEXTEDIT_H

#include <QtGui/QTextEdit>
#include <QtCore/QList>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class OffsetMapper;
class ScnDataTextEdit;

END_PLUG_NAMESPACE

class binary::plugin::scndata::ScnDataTextEdit : public QTextEdit {
    Q_OBJECT
public:
    ScnDataTextEdit(OffsetMapper * = NULL, QWidget *parent = NULL);
    ~ScnDataTextEdit();
    void markCursor(int start, int end, bool recover = false);
    void unmarkCursor();
    void setOffsetMapper(OffsetMapper *);
    void listenGroup(const QList<ScnDataTextEdit*> &);
    void setBlockOM(bool);
protected:
    virtual void focusOutEvent(QFocusEvent *e);
protected slots:
    void calcCursorPos();
public slots:
    void changeCursorPos(int, int);
signals:
    void offsetMapped(int, int);
private:
    int lastPos();

    QTextCursor _mark;
    OffsetMapper *_om;
    bool _blockOM;
};

#endif
