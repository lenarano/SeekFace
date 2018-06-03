#include "tablemodel.h"


TableModel::TableModel(QObject *parent) : QSqlTableModel(parent)
{
	unknown = RUS("НЕ РАСПОЗНАН");
}



QVariant TableModel::data(const QModelIndex &idx, int role) const
{
	QColor col;
	if (role == Qt::BackgroundColorRole) {
		if (!(QSqlTableModel::data(this->index(idx.row(), 2)).toString().isEmpty()))
		{
			col = QColor(Qt::green);
			if (QSqlTableModel::data(this->index(idx.row(), 2)).toString() == unknown)
			{
				col = QColor(Qt::red);
			}
			return col;
		}
	}
	else
		if (role == Qt::DecorationRole && idx.data().type() == 12) //12 - QByteArray [вне зависимости от нумерации столбца]
		{
			QPixmap pixmap;
			QSize PicSize(160,120);
			pixmap.loadFromData(data(idx, Qt::DisplayRole).toByteArray(), "PNG", Qt::AutoColor);
			pixmap = pixmap.scaled(PicSize, Qt::KeepAspectRatio);
			return pixmap;
		}
		else if (role == Qt::DisplayRole) { return QSqlTableModel::data(idx); }
		return QVariant();
}
