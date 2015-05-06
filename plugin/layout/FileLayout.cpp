/*
 * Copyright 2015 KANG Yuxuan
 *
 * FileLayout.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <algorithm>

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QToolTip>

#include "Defines.h"
#include "FileLayout.h"
#include "Colors.h"

using namespace std;
USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);
BEGIN_PLUG_NAMESPACE(layout)

FileLayout::FileLayout(LayoutType type, FileLayout *ref, QWidget *parent) :
    QWidget(parent), _whole(0,0), _type(type), _ref(ref), _highlight(0,0),
    _colors(new Colors())
{
    setMouseTracking(true);
}

FileLayout::~FileLayout()
{
    delete _colors;
}

void FileLayout::updateInfo(File *file)
{
    if(!file) {
        _whole = SegInfo(0,0);
        return;
    }
    _segs.clear();

    switch(_type) {
    case LAYOUT_SCN:
        updateScn(file); break;
    case LAYOUT_SGM:
        updateSgm(file); break;
    case LAYOUT_FILE:
        updateFile(file); break;
    default:
        return;
    }

    SegInfo newWhole = arrange();
    _whole.level = newWhole.level;
    updateWhole(newWhole);

    update();
}

void FileLayout::setRef(FileLayout *ref)
{
    if((_ref = ref)) {
        _ref->updateWhole(_whole);
    }
}

void FileLayout::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    for(vector<SegInfo>::iterator it = _segs.begin();
            it != _segs.end(); it++)
    {
        draw(*it, _whole);
    }
    if(_highlight.begin == 0 && _highlight.end == 0) {
        return;
    }
    QPainter painter(this);
    int left = segHPosToScr(_highlight.begin);
    int right = segHPosToScr(_highlight.end);
    int top = 0;
    int bottom = height();

    QColor hlColor = _highlight.color;
    hlColor.setAlpha(125);
    painter.fillRect(left, top, right-left, bottom-top, hlColor);
}

void FileLayout::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    QToolTip::hideText();
    QPoint pos = e->pos();
    double levelHeight = (double)height()/(_whole.level+1);
    int curLevel = pos.y()/levelHeight;
    for(vector<SegInfo>::iterator it = _segs.begin();
            it != _segs.end(); it++)
    {
        if(it->level!=curLevel) {
            continue;
        }
        if(segHPosToScr(it->begin)>pos.x() ||
                segHPosToScr(it->end)<pos.x()) {
            continue;
        }
        QToolTip::showText(e->globalPos(), QString("%1, %2 KiB\n"
                    "0x%3 -- 0x%4")
                .arg(it->name).arg((it->end-it->begin)/1024.0)
                .arg(it->begin, 0, 16).arg(it->end-1, 0, 16));
        if(_ref) {
            _ref->highlight(*it, this);
        }
        return;
    }
    if(_ref) {
        _ref->highlight(SegInfo(0,0), this);
    }
}

void FileLayout::enterEvent(QEvent *e)
{
    QWidget::enterEvent(e);
    _highlight = SegInfo(0,0);
    update();
    if(_ref) {
        _ref->highlight(SegInfo(0,0), this);
    }
}

void FileLayout::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    QPoint pos = e->pos();
    double levelHeight = (double)height()/(_whole.level+1);
    int curLevel = pos.y()/levelHeight;
    for(vector<SegInfo>::iterator it = _segs.begin();
            it != _segs.end(); it++)
    {
        if(it->level!=curLevel) {
            continue;
        }
        if(segHPosToScr(it->begin)>pos.x() ||
                segHPosToScr(it->end)<pos.x()) {
            continue;
        }
        switch(_type) {
        case LAYOUT_SCN:
            emit openScn(it->data.scnIdx);
            break;
        case LAYOUT_SGM:
            break;
        case LAYOUT_FILE:
            break;
        default:
            break;
        }
        return;
    }
}

FileLayout::SegInfo FileLayout::arrange()
{
    if(_segs.size() == 0) {
        return SegInfo(0,0);
    }

    sort(_segs.begin(), _segs.end());

    SegInfo whole(_segs.front().begin, _segs.front().end);

    vector<SegInfo> save;
    int level = 0;
    while(!_segs.empty()) {
        int currEnd = _segs.front().begin;
        for(vector<SegInfo>::iterator it = _segs.begin();
                it != _segs.end(); )
        {
            if(currEnd<=it->begin) {
                currEnd = it->end;
                it->level = level;
                it->color = _colors->next();
                save.push_back(*it);
                if(whole.begin>it->begin) {
                    whole.begin = it->begin;
                }
                if(whole.end<it->end) {
                    whole.end = it->end;
                }
                whole.level = level;
                it = _segs.erase(it);
            } else {
                it++;
            }
        }
        level++;
    }
    _segs = save;
    return whole;
}

void FileLayout::draw(const SegInfo &info, const SegInfo &whole)
{
    QPainter painter(this);
    int left = segHPosToScr(info.begin);
    int right = segHPosToScr(info.end);
    double levelHeight = (double)height()/(whole.level+1);
    int top = levelHeight*info.level;
    int bottom = levelHeight*(info.level+1);

    painter.fillRect(left, top, right-left, bottom-top, info.color);
}

double FileLayout::segHPosToScr(int pos)
{
    return (double)(pos-_whole.begin)/(_whole.end-_whole.begin)*width();
}

void FileLayout::updateScn(File *file)
{
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!file->getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type == SHT_NOBITS) {
            continue;
        }
        SegInfo info(shdr.sh_offset, shdr.sh_offset+shdr.sh_size);
        info.name = file->getScnName(&shdr);
        info.data.scnIdx = i;
        _segs.push_back(info);
    }
}

void FileLayout::updateSgm(File *file)
{
    size_t phdrNum;
    if(file->getPhdrNum(&phdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<phdrNum; i++) {
        Elf64_Phdr phdr;
        if(!file->getPhdr(i, &phdr)) {
            continue;
        }
        SegInfo info(phdr.p_offset, phdr.p_offset+phdr.p_filesz);
        info.name = Defines::commentText(phdr.p_type, defines_PT);
        info.data.sgmIdx = i;
        _segs.push_back(info);
    }
}

void FileLayout::updateFile(File *file)
{
    Elf64_Ehdr ehdr;
    if(!file->getEhdr(&ehdr)) {
        return;
    }
    SegInfo ehdrInfo(0, ehdr.e_ehsize);
    ehdrInfo.name = "Executable header";
    _segs.push_back(ehdrInfo);

    size_t shdrNum, phdrNum;
    if(ehdr.e_phoff && file->getPhdrNum(&phdrNum) == 0) {
        SegInfo phdrInfo(ehdr.e_phoff,
                ehdr.e_phoff + phdrNum*ehdr.e_phentsize);
        phdrInfo.name = "Program header table";
        _segs.push_back(phdrInfo);
    }
    if(ehdr.e_shoff && file->getShdrNum(&shdrNum) == 0) {
        SegInfo shdrInfo(ehdr.e_shoff,
                ehdr.e_shoff + shdrNum*ehdr.e_shentsize);
        shdrInfo.name = "Section header table";
        _segs.push_back(shdrInfo);
    }
}

void FileLayout::updateWhole(const SegInfo &whole)
{
    bool changed = false;
    if(whole.begin < _whole.begin) {
        _whole.begin = whole.begin;
        changed = true;
    }
    if(whole.end > _whole.end) {
        _whole.end = whole.end;
        changed = true;
    }
    if(changed && _ref) {
        _ref->updateWhole(_whole);
    }
    if(changed) {
        update();
    }
}

void FileLayout::highlight(const SegInfo &info, const FileLayout *src)
{
    if(src == this) {
        return;
    }
    _highlight = info;
    update();
    if(_ref) {
        _ref->highlight(info, src);
    }
}

END_PLUG_NAMESPACE
