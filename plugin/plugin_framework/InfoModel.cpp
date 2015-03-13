#include <QtCore/QtAlgorithms>
#include <QtCore/QChar>
#include <QtCore/QStringList>

#include "InfoModel.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

InfoModel::InfoModel(const QString &data, QObject *parent) :
    QAbstractItemModel(parent),
    _root(new InfoTree),
    _rootIndex(createIndex(-1, -1, (void*)0))
{
    buildTree(data.split("\n", QString::SkipEmptyParts));
}

int InfoModel::columnCount(const QModelIndex &) const
{
    return 2;
}

static bool isVariable(const QVariant &data)
{
    if(data.type() != QVariant::String) {
        return false;
    }
    const QString &str = data.toString();
    return str.startsWith(QChar('\002')) && str.endsWith(QChar('\003'));
}

QVariant InfoModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }
    InfoTree *node = static_cast<InfoTree*>(index.internalPointer());
    if(node) {
        QVariant res = node->data(index.column());
        if(isVariable(res)) {
            return QVariant(QString("N/A"));
        }
        return res;
    } else {
        return QVariant();
    }
}

bool InfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::DisplayRole || index == _rootIndex) {
        return false;
    }
    InfoTree *node = static_cast<InfoTree*>(index.internalPointer());
    node->setData(index.column(), value);
    emit dataChanged(index, index);
    return true;
}

bool InfoModel::setData(const QString &key, const QVariant &value)
{
    if(!_variableMap.contains(key)) {
        return false;
    }
    setData(_variableMap[key], value);
    return true;
}

QModelIndex InfoModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    InfoTree *node = static_cast<InfoTree*>(parent.internalPointer());
    if(!node || parent == _rootIndex) {
        node = _root;
    }
    return createIndex(row, column, node->getChild(row));
}

QModelIndex InfoModel::parent(const QModelIndex &index) const
{
    InfoTree *node = static_cast<InfoTree*>(index.internalPointer());
    if(!node || index == _rootIndex) {
        return QModelIndex();
    }
    InfoTree *parent = node->getParent();
    if(node->getParent() == _root) {
        return _rootIndex;
    }
    return createIndex(parent->getRow(), 0, parent);
}

int InfoModel::rowCount(const QModelIndex &parent) const
{
    InfoTree *node = static_cast<InfoTree*>(parent.internalPointer());
    if(!node || parent == _rootIndex) {
        return _root->rowCount();
    }
    return node->rowCount();
}

static int indentLevel(const QString &line)
{
    int i;
    for(i=0; i<line.length() && line[i]==QChar('\t'); i++);
    return i;
}

void InfoModel::buildTree(const QStringList &data)
{
    int currentLevel = -1;
    InfoTree *currentNode = _root;
    for(int i=0; i<data.size(); i++) {
        const QString &line = data.at(i);
        QList<QVariant> newData;
        QStringList lineFields = line.split("\t", QString::SkipEmptyParts);
        foreach(QString s, lineFields) {
            newData << s;
        }

        int lineLevel = indentLevel(line);
        if(lineLevel<currentLevel) {
            while(currentLevel>lineLevel) {
                currentLevel--;
                currentNode = currentNode->getParent();
            }
            currentNode->getParent()->addChild(newData);
            currentNode = currentNode->nextSibling();
        } else if(lineLevel>currentLevel) {
            if(lineLevel!=currentLevel+1) {
                throw std::exception();
            }
            currentNode->addChild(newData);
            currentNode = currentNode->getChild(0);
            currentLevel = lineLevel;
        } else {
            currentNode->getParent()->addChild(newData);
            currentNode = currentNode->nextSibling();
        }
        if(isVariable(newData[1])) {
            QString key = lineFields[1].midRef(1, lineFields[1].length()-2).toString();
            QModelIndex value = createIndex(currentNode->getRow(), 1, currentNode);
            _variableMap.insert(key, value);
        }
    }
}

//----------------InfoTree------------------

InfoTree::InfoTree(const QList<QVariant> &data, InfoTree *parent) :
    _parent(parent),
    _data(data)
{
    _child.clear();
}

InfoTree::~InfoTree()
{
    qDeleteAll(_child);
}

int InfoTree::rowCount() const
{
    return _child.size();
}

int InfoTree::columnCount() const
{
    return _data.size();
}

InfoTree *InfoTree::getChild(int idx)
{
    if(idx>=0 && idx<_child.size()) {
        return _child[idx];
    } else {
        return NULL;
    }
}

InfoTree *InfoTree::getParent()
{
    return _parent;
}

InfoTree *InfoTree::nextSibling()
{
    if(getRow()<_parent->rowCount()-1) {
        return _parent->_child[getRow()+1];
    }
    return NULL;
}

int InfoTree::getRow()
{
    return _parent->_child.indexOf(this);
}

QVariant InfoTree::data(int col)
{
    return _data[col];
}

void InfoTree::addChild(const QList<QVariant> &data)
{
    InfoTree *node = new InfoTree(data, this);
    _child.push_back(node);
}

bool InfoTree::setData(int col, const QVariant &value)
{
    if(col<0 || col>=_data.size()) {
        return false;
    }
    _data[col] = value;
    return true;
}

END_PLUG_NAMESPACE