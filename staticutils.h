#ifndef STATICUTILS_H
#define STATICUTILS_H

#include <QWidget>

#include <QLayout>
#include <QFormLayout>

#include <QAbstractItemModel>

#include <QClipboard>
#include <QApplication>
#include <QThread>

using namespace std;

class StaticUtils
{
public:
	static void deleteLayout(QLayout* layout);
	static void deleteFormLayout(QFormLayout* layout);

	static void deleteItemModel(QAbstractItemModel* model);

	static void copyToClipboard(QString text);

	static QString const HumanizedSize(qint64 byteCount, const QString &nullText = "0 B");
};

#endif // STATICUTILS_H
