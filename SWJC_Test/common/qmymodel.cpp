#include "qmymodel.h"



QMyModel::QMyModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_doubleListData = NULL;
}

void QMyModel::setDoubleListPtr(QList<QList<QVariant> > *data, QStringList &headerData)
{
    m_headerData = headerData;
    m_doubleListData = data;
    upData();
}

void QMyModel::setDoubleList(QList<QList<QVariant> > *data)
{
    //m_headerData = headerData;
    m_doubleListData = data;
    upData();
}

void QMyModel::upData()
{
    beginResetModel();
    if (0 == m_doubleListData->size())
    {
        m_rowCount = 0;
        m_columnCount = 0;
    }
    else
    {
        m_rowCount = m_doubleListData->size();
        m_columnCount = m_doubleListData->at(0).size();
    }

//    m_columnCount = 0;
//    int colSize=0;
//    for(int i = 0; i < m_rowCount; ++i)
//    {
//        colSize = m_doubleListData->at(i).size();
//        if(colSize > m_columnCount)
//        {
//            m_columnCount = colSize;    //取最大的列数
//        }
//    }
    endResetModel();
}

int QMyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rowCount;
}

int QMyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columnCount;
}

QVariant QMyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || 0 == m_doubleListData->size())
    {
        return QVariant();
    }

    if(Qt::DisplayRole == role || role == Qt::EditRole)
    {
        const int row = index.row();
        const int col = index.column();
        if(row >= m_doubleListData->size() || col >= m_doubleListData->at(row).size())
        {
            return QVariant();
        }

        return m_doubleListData->at(row).at(col);
    }
    return QVariant();
}

QVariant QMyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return m_headerData[section];
        }
        return section + 1;
    }
    return QVariant();
}

bool QMyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (0 == m_doubleListData->size())
        return false;
    // 设置什么条件下可以编辑
    if(Qt::ItemIsEditable != role)
        return false;
    if (index.column() != 1)
        return false;
    if( index.row() >= m_doubleListData->size())
        return false;

    QList<QVariant> rowList = m_doubleListData->at(index.row());
    rowList.replace(index.column(), value);
    m_doubleListData->replace(index.row(), rowList);

    upData();

    emit dataChanged(index, index);


    return true;
}

Qt::ItemFlags QMyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag =  QAbstractTableModel::flags(index);

    // 假设第一列的checkstate可以编辑
    if (index.column()  == 1)
    {
        flag = flag | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        return flag;
    }
    return QAbstractTableModel::flags(index);
}

void QMyModel::insertData(QList<QVariant> dataRow)
{
    Q_UNUSED(dataRow);
    return;
}


