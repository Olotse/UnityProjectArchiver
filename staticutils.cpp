#include "staticutils.h"

void StaticUtils::deleteLayout(QLayout *layout)
{
	if(layout != nullptr)
	{
		if(layout->count() > 0)
			while(QLayoutItem* item = layout->takeAt(0))
			{
				delete item->widget();
				delete item;
			}

		delete layout;
	}
}

void StaticUtils::deleteFormLayout(QFormLayout *layout)
{
	if(layout != nullptr)
	{
		while(layout->rowCount() > 0)
			layout->removeRow(0);

		delete layout;
	}
}

void StaticUtils::deleteItemModel(QAbstractItemModel *model)
{
	if(model != nullptr)
	{
		model->removeRows(0, model->rowCount());
		model->removeColumns(0, model->columnCount());

		delete model;
	}
}

void StaticUtils::copyToClipboard(QString text)
{
  QClipboard* clipboard = QApplication::clipboard();

  clipboard->setText(text, QClipboard::Clipboard);

  if (clipboard->supportsSelection())
	  clipboard->setText(text, QClipboard::Selection);

  #if defined(Q_OS_LINUX)
	  QThread::msleep(1); // workaround for copied text not being available...
  #endif
}

QString const StaticUtils::HumanizedSize(qint64 byteCount, QString const& nullText)
{
	QString size = "";

	if(byteCount > qPow(qint64(2), qint64(30)))
		size = QString::number(round(byteCount / qPow(qint64(2), qint64(30)))) + " GiB (" + QString::number(round(byteCount / qPow(qint64(10), qint64(9)))) + " Go)";
	else if(byteCount > qPow(qint64(2), qint64(20)))
		size = QString::number(round(byteCount / qPow(qint64(2), qint64(20)))) + " MiB (" + QString::number(round(byteCount / qPow(qint64(10), qint64(6)))) + " Mo)";
	else if(byteCount > qPow(qint64(2), qint64(10)))
		size = QString::number(round(byteCount / qPow(qint64(2), qint64(10)))) + " KiB (" + QString::number(round(byteCount / qPow(qint64(10), qint64(3)))) + " Ko)";
	else if(byteCount > 0)
		size = QString::number(byteCount) + " B";
	else
		size = nullText;

	return size;
}
