#ifndef DIRSIZEWORKER_H
#define DIRSIZEWORKER_H

#include <QThread>
#include <QDirIterator>

#include <QString>

class DirSizeWorker : public QThread
{
	Q_OBJECT

public:
	explicit DirSizeWorker(QString const& dirPath, bool keepLibrary = false, bool keepLogs = false, bool keepObj = false, QObject *parent = nullptr);
	virtual void run() override;

protected:
	virtual void calculateDirSize(QString const& dirPath);

Q_SIGNALS:
	void dirSizeCalculated(qint64 const& byteCount);

private:
	QString dirPath;

	bool keepLibrary;
	bool keepLogs;
	bool keepObj;

	qint64 totalSize;
};

#endif // DIRSIZEWORKER_H
