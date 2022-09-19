#include "dirsizeworker.h"

DirSizeWorker::DirSizeWorker(QString const& dirPath, bool keepLibrary, bool keepLogs, bool keepObj, QObject *parent) :
	QThread(parent),
	dirPath(dirPath),
	keepLibrary(keepLibrary),
	keepLogs(keepLogs),
	keepObj(keepObj)
{}

void DirSizeWorker::run()
{
	calculateDirSize(dirPath);
	emit dirSizeCalculated(totalSize);
}

void DirSizeWorker::calculateDirSize(const QString &dirPath)
{
	QDirIterator iterator(dirPath, QDirIterator::IteratorFlag::Subdirectories);
	totalSize = 0;

	while(iterator.hasNext())
	{
		if(!keepLibrary && iterator.filePath().contains("/Library/"))
		{
			//qDebug() << "Detected /Library/ in " << iterator.filePath();
			iterator.next();

			continue;
		}
		else if(!keepLogs && iterator.filePath().contains("/Logs/"))
		{
			//qDebug() << "Detected /Logs/ in " << iterator.filePath();
			iterator.next();

			continue;
		}
		else if(!keepObj && iterator.filePath().contains("/obj/"))
		{
			//qDebug() << "Detected /obj/ in " << iterator.filePath();
			iterator.next();

			continue;
		}
		else if(iterator.filePath().contains("/Temp/"))
		{
			//qDebug() << "Detected /Temp/ in " << iterator.filePath();
			iterator.next();

			continue;
		}

		totalSize += iterator.fileInfo().size();
		iterator.next();
	}
}
