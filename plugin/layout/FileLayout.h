#ifndef PLUGIN_LAYOUT_FILELAYOUT_H
#define PLUGIN_LAYOUT_FILELAYOUT_H

#include <vector>
#include <utility>
#include <QtGui/QWidget>
#include <QtGui/QLabel>

#include "common.h"
#include "backend/File.h"

BEGIN_PLUG_NAMESPACE(layout)

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
        SegInfo(int b, int e):begin(b),end(e),level(0),colorIdx(0) {}
        bool operator<(const SegInfo &o) const {
            if(begin==o.begin) return end<o.end;
            return begin<o.begin;
        }
        int begin, end;
        int level;
        size_t colorIdx;
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
