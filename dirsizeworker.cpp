#include "dirsizeworker.h"

DirSizeWorker::DirSizeWorker(QString const& dirPath, const QStringList &dirList, const QStringList &extensionList, QObject *parent) :
	QThread(parent),
	dirPath(dirPath),
	dirList(dirList),
	extensionList(extensionList)
{}

void DirSizeWorker::run()
{
	calculateDirSize(dirPath);
	emit dirSizeCalculated(totalSize);
}

void DirSizeWorker::calculateDirSize(const QString &dirPath)
{
	QDirIterator iterator(dirPath, QDir::AllEntries | QDir::Hidden, QDirIterator::IteratorFlag::Subdirectories);
	totalSize = 0;

	// Répertoire d'origine
	QString fileRelativePath;

	while(iterator.hasNext())
	{
		// Si l'iterator est sur un dossier, on passe
		if(iterator.fileInfo().isDir())
		{
			iterator.next();
			continue;
		}

		fileRelativePath = iterator.fileInfo().filePath().remove(dirPath + "/");
		QFileInfo fileInfo(fileRelativePath);

		qsizetype index = fileRelativePath.indexOf("/");
		QString dirname = fileInfo.path().remove(index, fileInfo.path().length() - index);

		// Si le répertoire ou l'extension du fichier n'est pas à traiter, on passe au fichier suivant
		if(dirList.contains(dirname) || extensionList.contains(fileInfo.suffix()))
		{
			iterator.next();
			continue;
		}

		totalSize += iterator.fileInfo().size();
		iterator.next();
	}
}
