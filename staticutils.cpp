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
	QString bUnit[] = { "B", "KiB", "MiB", "GiB" };
	QString oUnit[] = { "o", "Ko", "Mo", "Go" };

	for(int i = 3; i > 0; i--)
	{
		if(i > 0)
		{
			double bresult = round(byteCount /qPow(2, i*10) * 100) / 100;
			double oresult = round((byteCount / qPow(10, i*3)) * 100) / 100;

			if(byteCount > qPow(2, i*10))
				return QString::number(bresult) + " " + bUnit[i] + " (" + QString::number(oresult) + " " + oUnit[i] + ")";
		}
	}

	return nullText;
}
