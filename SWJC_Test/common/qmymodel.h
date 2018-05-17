#ifndef QMYMODEL_H
#define QMYMODEL_H

#include <QTimer>
#include <QAbstractTableModel>
#include <QList>

class QMyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QMyModel(QObject *parent = 0);

    void setDoubleList(QList<QList<QVariant> > *data);
    void setDoubleListPtr(QList<QList<QVariant> > *data, QStringList &headerData);
    void upData();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;


    void insertData(QList <QVariant> dataRow);

signals:
    void tableUpdata(QModelIndex &index, QVariant value);

private:
    QTimer *m_timer;
    QList< QList <QVariant>> *m_doubleListData;
    QStringList m_headerData;

    int m_rowCount;
    int m_columnCount;
};

#endif // QMYMODEL_H
