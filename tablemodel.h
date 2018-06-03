#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QString>
#include <QColor>
#include <QPixmap>

class TableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = 0);
    QVariant data(const QModelIndex &idx, int role) const;

signals:

public slots:

private:
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }
	QString unknown;
	
};

#endif // TABLEMODEL_H
