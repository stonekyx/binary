/*
 * Copyright 2015 KANG Yuxuan
 *
 * ScnDataTextEdit.h by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    void changeCursorPosInit(int, int);
protected slots:
    void calcCursorPos();
public slots:
    void changeCursorPos(int, int, bool = false);
signals:
    void offsetMapped(int, int);
private:
    int lastPos();

    QTextCursor _mark;
    OffsetMapper *_om;
    bool _blockOM;
};

#endif
