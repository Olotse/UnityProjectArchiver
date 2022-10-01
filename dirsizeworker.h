#ifndef DIRSIZEWORKER_H
#define DIRSIZEWORKER_H

#include <QThread>
#include <QDirIterator>

#include <QString>

class DirSizeWorker : public QThread
{
	Q_OBJECT

public:
	explicit DirSizeWorker(QString const& dirPath, QStringList const& dirList, QStringList const& extensionList, QObject *parent = nullptr);
	virtual void run() override;

protected:
	virtual void calculateDirSize(QString const& dirPath);

Q_SIGNALS:
	void dirSizeCalculated(qint64 const& byteCount);

private:
	QString dirPath;
	qint64 totalSize;

	QStringList dirList;
	QStringList extensionList;
};

#endif // DIRSIZEWORKER_H
