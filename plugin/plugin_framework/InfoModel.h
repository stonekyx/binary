#ifndef PLUGIN_BASIC_INFOMODEL_H
#define PLUGIN_BASIC_INFOMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>
#include <QtCore/QString>

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

class InfoTree;

class InfoModel : public QAbstractItemModel {
public:
    InfoModel(const QString &data, size_t colCount = 2,
            QObject *parent = 0);
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
            int role = Qt::DisplayRole ) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value,
            int role = Qt::DisplayRole );
    bool setData(const QString &key, const QVariant &value);
    virtual QModelIndex index(int row, int column,
            const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    void buildMore(const QString &line);
    QModelIndex nextRow(const QModelIndex &cur);
private:
    void buildTree(const QStringList &data);
    InfoTree *_root;
    QHash<QString, QModelIndex> _variableMap;
    size_t _columnCount;

    int _currentLevel;
    InfoTree *_currentNode;
};

class InfoTree {
public:
    InfoTree(const QList<QVariant> &data = QList<QVariant>(),
            InfoTree *parent = NULL);
    ~InfoTree();
    int rowCount() const;
    int columnCount() const;
    InfoTree *getChild(int idx);
    InfoTree *getParent();
    InfoTree *nextSibling();
    int getRow();
    QVariant data(int);

    void addChild(const QList<QVariant> &);
    bool setData(int, const QVariant &);
private:
    QList<InfoTree*> _child;
    InfoTree *_parent;
    QList<QVariant> _data;
};

END_PLUG_NAMESPACE

#endif
