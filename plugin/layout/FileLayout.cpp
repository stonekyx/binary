#include <algorithm>

#include <QtGui/QPainter>
#include <QtGui/QColor>
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
    QWidget(parent), _whole(0,0), _type(type), _ref(ref)
{
    setMouseTracking(true);
}

FileLayout::~FileLayout()
{
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
        QToolTip::showText(e->globalPos(), QString("%1, %2 KiB")
                .arg(it->name).arg((it->end-it->begin)/1024.0));
        break;
    }
}

FileLayout::SegInfo FileLayout::arrange()
{
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
                it->colorIdx = rand()%(sizeof(colors)/sizeof(QColor));
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

    painter.fillRect(left, top, right-left, bottom-top,
            colors[info.colorIdx]);
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
        info.name = Defines::commentText_PT(phdr.p_type);
        _segs.push_back(info);
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

END_PLUG_NAMESPACE
