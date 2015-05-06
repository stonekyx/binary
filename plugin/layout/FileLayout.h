/*
 * Copyright 2015 KANG Yuxuan
 *
 * FileLayout.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_LAYOUT_FILELAYOUT_H
#define PLUGIN_LAYOUT_FILELAYOUT_H

#include <vector>
#include <utility>
#include <QtGui/QColor>
#include <QtGui/QWidget>
#include <QtGui/QLabel>

#include "common.h"
#include "backend/File.h"

BEGIN_PLUG_NAMESPACE(layout)

class Colors;
class FileLayout;

END_PLUG_NAMESPACE

class binary::plugin::layout::FileLayout : public QWidget {
    Q_OBJECT
public:
    typedef enum {
        LAYOUT_SCN,
        LAYOUT_SGM,
        LAYOUT_FILE,
        LAYOUT_NUM
    } LayoutType;
    FileLayout(LayoutType type, FileLayout *ref = NULL, QWidget *parent = NULL);
    ~FileLayout();
    void updateInfo(binary::backend::File *);
    void setRef(FileLayout *);
signals:
    void openScn(size_t scnIdx);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void enterEvent(QEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
private:
    struct SegInfo {
        SegInfo(int b, int e):begin(b),end(e),level(0) {}
        bool operator<(const SegInfo &o) const {
            if(begin==o.begin) return end<o.end;
            return begin<o.begin;
        }
        int begin, end;
        int level;
        QColor color;
        union {
            size_t scnIdx;
            size_t sgmIdx;
        } data;
        const char *name;
    };
    std::vector<SegInfo> _segs;
    SegInfo _whole;
    LayoutType _type;
    FileLayout *_ref;
    SegInfo _highlight;
    Colors *_colors;

    SegInfo arrange();
    void draw(const SegInfo &, const SegInfo &);
    double segHPosToScr(int);
    void updateScn(binary::backend::File *);
    void updateSgm(binary::backend::File *);
    void updateFile(binary::backend::File *);
    void updateWhole(const SegInfo &);
    void highlight(const SegInfo &, const FileLayout *);
};

#endif
