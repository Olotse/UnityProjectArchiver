#include "qtprojectinfos.h"

QtProjectInfos::QtProjectInfos(QString const& projectDirPath) :
	ProjectInfos(projectDirPath.toStdString()),
	projectNameItem(nullptr),
	projectSizeItem(nullptr),
	projectPathItem(nullptr),
	projectCopyItem(nullptr),

	projectSizeWorker(nullptr),
	projectSizeWorkerRunning(false),
	projectPathChanged(true),

	projectCopyProgress(0),
	interruptCopy(false)
{}


QtProjectInfos::QtProjectInfos(QtProjectInfos const& projectInfos) :
	QtProjectInfos(QString::fromStdString(projectInfos.getFolderPath()))
{
	if(projectInfos.projectNameItem != nullptr)
		this->projectNameItem = new QStandardItem(projectInfos.projectNameItem->text());

	if(projectInfos.projectSizeItem != nullptr)
		this->projectSizeItem = new QStandardItem(projectInfos.projectSizeItem->text());

	if(projectInfos.projectPathItem != nullptr)
		this->projectPathItem = new QStandardItem(projectInfos.projectPathItem->text());

	if(projectInfos.projectCopyItem != nullptr)
		this->projectCopyItem = new QStandardItem(projectInfos.projectCopyItem->text());
}

QList<QStandardItem*> QtProjectInfos::getStandardItems()
{
	QList<QStandardItem*> standardItems;

	standardItems.append(getProjectNameItem());
	standardItems.append(getProjectSizeItem());
	standardItems.append(getProjectPathItem());
	standardItems.append(getProjectCopyItem());

	return standardItems;
}

QStandardItem* QtProjectInfos::getProjectNameItem()
{
	QDir dir(QString::fromStdString(getFolderPath()));

	if(projectNameItem == nullptr)
		projectNameItem = new QStandardItem(dir.dirName());
	else
		projectNameItem->setText(dir.dirName());

	projectNameItem->setToolTip(projectNameItem->text());
	return projectNameItem;
}

QStandardItem* QtProjectInfos::getProjectSizeItem()
{
	QString text = tr("Calcul en cours...");

	if(projectPathChanged)
	{
		if(projectSizeWorkerRunning)
			projectSizeWorker->quit();

		projectSizeWorker = new DirSizeWorker(QString::fromStdString(getFolderPath()), isKeepingLibrary(), isKeepingLogs(), isKeepingObj());

		connect(projectSizeWorker, &QThread::finished, projectSizeWorker, &QObject::deleteLater);
		connect(projectSizeWorker, &DirSizeWorker::dirSizeCalculated, this, &QtProjectInfos::updateProjectSize);

		projectSizeWorker->start();
		projectSizeWorkerRunning = true;
	}
	else
		text = StaticUtils::HumanizedSize(projectSize, tr("- B"));

	if(projectSizeItem == nullptr)
		projectSizeItem = new QStandardItem(text);
	else
		projectSizeItem->setText(text);

	projectSizeItem->setToolTip(projectSizeItem->text());
	return projectSizeItem;
}

QStandardItem* QtProjectInfos::getProjectPathItem()
{
	if(projectPathItem == nullptr)
		projectPathItem = new QStandardItem(QString::fromStdString(getFolderPath()));
	else
		projectPathItem->setText(QString::fromStdString(getFolderPath()));

	projectPathItem->setToolTip(projectPathItem->text());
	return projectPathItem;
}

QStandardItem* QtProjectInfos::getProjectCopyItem()
{
	QString text = tr("Prêt à copier");

	if(projectCopyItem == nullptr)
		projectCopyItem = new QStandardItem(text);
	else
		projectCopyItem->setText(text);

	return projectCopyItem;
}

qint64 const& QtProjectInfos::getProjectSize() const
{ return projectSize; }

void QtProjectInfos::setFolderPath(const string &folderPath)
{
	if(folderPath != ProjectInfos::getFolderPath())
	{
		ProjectInfos::setFolderPath(folderPath);
		projectPathChanged = true;
	}
}

void QtProjectInfos::proceedToCopy()
{
	// Répertoire d'origine
	QDir sourceDirectory(QString::fromStdString(getFolderPath()));
	QDir destinationDirectory(QString::fromStdString(getDestinationPath()) + "/" + sourceDirectory.dirName());

	QString sourceFilePath, destinationFilePath, destinationFileDirectoryPath;

	QDirIterator iterator(sourceDirectory.path(), QDirIterator::IteratorFlag::Subdirectories);
	qint64 totalSize = 0;

	QString itemText = tr("Copie en cours...");

	while(iterator.hasNext())
	{
		if(interruptCopy)
		{
			if(projectCopyItem != nullptr)
				projectCopyItem->setText(tr("Copie annulée"));

			interruptCopy = false;
			break;
		}

		sourceFilePath = iterator.fileInfo().filePath();
		destinationFilePath = iterator.fileInfo().filePath().remove(sourceDirectory.path()).insert(0, destinationDirectory.path());
		destinationFileDirectoryPath = iterator.fileInfo().filePath().remove(sourceDirectory.path()).insert(0, destinationDirectory.path()).remove(iterator.fileInfo().fileName());

		if(iterator.fileInfo().isDir()
			|| (!isKeepingLibrary() && sourceFilePath.contains("/Library/"))
			|| (!isKeepingLogs() && sourceFilePath.contains("/Logs/"))
			|| (!isKeepingObj() && sourceFilePath.contains("/obj/"))
			|| (sourceFilePath.contains("/Temp/")))
		{
			iterator.next();
			continue;
		}

		// Création du chemin si nécessaire
		QDir destinationDirectory(destinationFileDirectoryPath);

		if(!destinationDirectory.exists())
			destinationDirectory.mkpath(destinationFileDirectoryPath);
		else if(QFile::exists(destinationFilePath))
			QFile::remove(destinationFilePath);

		// Copie du fichier
		QFile::copy(iterator.fileInfo().filePath(), destinationFilePath);

		totalSize += iterator.fileInfo().size();
		iterator.next();

		if(!projectSizeWorkerRunning && projectSize > 0)
		{
			projectCopyProgress = totalSize * 100 / projectSize;

			if(projectCopyProgress < 100)
				itemText = QString::number(projectCopyProgress) + "%";
			else
				itemText = tr("Copie terminée");
		}

		if(projectCopyItem != nullptr)
			projectCopyItem->setText(itemText);
	}
}

void QtProjectInfos::stopCopyProcess()
{
	interruptCopy = true;
}

void QtProjectInfos::updateProjectSize(qint64 const& byteCount)
{
	if(projectSizeItem != nullptr)
	{
		projectSizeItem->setText(StaticUtils::HumanizedSize(byteCount));
		projectSizeItem->setToolTip(projectSizeItem->text());
	}

	projectSize = byteCount;
	projectSizeWorkerRunning = false;
}

QtProjectInfos& QtProjectInfos::operator=(const QtProjectInfos &projectInfos)
{
	if(projectInfos.projectNameItem != nullptr && this->projectNameItem != nullptr)
		this->projectNameItem->setText(projectInfos.projectNameItem->text());
	else if(projectInfos.projectNameItem != nullptr && this->projectNameItem == nullptr)
		this->projectNameItem = new QStandardItem(projectInfos.projectNameItem->text());
	else if(projectInfos.projectNameItem == nullptr && this->projectNameItem != nullptr)
	{
		delete this->projectNameItem;
		this->projectNameItem = nullptr;
	}

	if(projectInfos.projectSizeItem != nullptr && this->projectSizeItem != nullptr)
		this->projectSizeItem->setText(projectInfos.projectSizeItem->text());
	else if(projectInfos.projectSizeItem != nullptr && this->projectSizeItem == nullptr)
		this->projectSizeItem = new QStandardItem(projectInfos.projectSizeItem->text());
	else if(projectInfos.projectSizeItem == nullptr && this->projectSizeItem != nullptr)
	{
		delete this->projectSizeItem;
		this->projectSizeItem = nullptr;
	}

	if(projectInfos.projectPathItem != nullptr && this->projectPathItem != nullptr)
		this->projectPathItem->setText(projectInfos.projectPathItem->text());
	else if(projectInfos.projectPathItem != nullptr && this->projectPathItem == nullptr)
		this->projectPathItem = new QStandardItem(projectInfos.projectPathItem->text());
	else if(projectInfos.projectPathItem == nullptr && this->projectPathItem != nullptr)
	{
		delete this->projectPathItem;
		this->projectPathItem = nullptr;
	}

	return *this;
}

QtProjectInfos::~QtProjectInfos()
{
	if(projectSizeWorkerRunning)
		projectSizeWorker->quit();
}
