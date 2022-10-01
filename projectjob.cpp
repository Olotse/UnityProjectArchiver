#include "projectjob.h"

ProjectJob::ProjectJob(QString const& projectDirPath, QObject *parent) :
	QObject(parent),
	projectInfos(projectDirPath.toStdString()),

	projectNameItem(nullptr),
	projectSizeItem(nullptr),
	projectPathItem(nullptr),
	projectCopyItem(nullptr),

	projectSizeWorker(nullptr),
	projectSizeWorkerRunning(false),
	projectSize(0),
	needToCalculateProjectSize(true),

	projectCopyRunning(false),
	projectCopyProgress(0),
	interruptCopy(false)
{}


ProjectJob::ProjectJob(ProjectJob const& projectJob) :
	ProjectJob(projectJob.getProjectPath())
{
	if(projectJob.projectNameItem != nullptr)
		this->projectNameItem = new QStandardItem(projectJob.getProjectName());

	if(projectJob.projectSizeItem != nullptr)
		this->projectSizeItem = new QStandardItem(projectJob.getProjectSize());

	if(projectJob.projectPathItem != nullptr)
		this->projectPathItem = new QStandardItem(projectJob.getProjectPath());

	if(projectJob.projectCopyItem != nullptr)
		this->projectCopyItem = new QStandardItem(tr("En attente"));
}

QString ProjectJob::getDestinationPath()
{ return QString::fromStdString(ProjectInfos::getDestinationPath()); }

void ProjectJob::setDestinationPath(QString const& dirPath)
{ ProjectInfos::setDestinationPath(dirPath.toStdString()); }


QString ProjectJob::getProjectPath() const
{ return QString::fromStdString(projectInfos.getProjectPath()); }

QString ProjectJob::getProjectName() const
{ return QString::fromStdString(projectInfos.getProjectName()); }

QString ProjectJob::getParentFolderPath() const
{ return QString::fromStdString(projectInfos.getParentFolderPath()); }

QStringList ProjectJob::getUnkeepedDirNames() const
{
	QStringList dirs;

	if(!projectInfos.isKeepingLibrary())
		dirs << "Library";

	if(!projectInfos.isKeepingLogs())
		dirs << "Logs";

	if(!projectInfos.isKeepingObj())
		dirs << "obj";

	if(!projectInfos.isKeepingVisualStudio())
		dirs << ".vs";

	if(!projectInfos.isKeepingGit())
		dirs << ".git";

	dirs << "Temp";

	return dirs;
}

QStringList ProjectJob::getUnkeepedFileExtensions() const
{
	QStringList extensions;

	if(!projectInfos.isKeepingVisualStudio())
		extensions << "csproj" << "vsconfig" << "sln";

	if(!projectInfos.isKeepingMetaFiles())
		extensions << "meta";

	return extensions;
}

QList<QStandardItem*> ProjectJob::getStandardItems()
{
	QList<QStandardItem*> standardItems;

	standardItems.append(getProjectNameItem());
	standardItems.append(getProjectSizeItem());
	standardItems.append(getProjectPathItem());
	standardItems.append(getProjectCopyItem());

	return standardItems;
}

QStandardItem* ProjectJob::getProjectNameItem()
{
	QString text = getProjectName();

	if(projectNameItem == nullptr)
		projectNameItem = new QStandardItem(text);
	else
		projectNameItem->setText(text);

	projectNameItem->setToolTip(text);
	return projectNameItem;
}

QStandardItem* ProjectJob::getProjectSizeItem()
{
	QString text = tr("Calcul en cours...");

	if(needToCalculateProjectSize)
		proceedToSize();
	else if(!projectSizeWorkerRunning)
		text = StaticUtils::HumanizedSize(projectSize, tr("- B"));

	if(projectSizeItem == nullptr)
		projectSizeItem = new QStandardItem(text);
	else
		projectSizeItem->setText(text);

	projectSizeItem->setToolTip(text);
	return projectSizeItem;
}

QStandardItem* ProjectJob::getProjectPathItem()
{
	QString text = getProjectPath();

	if(projectPathItem == nullptr)
		projectPathItem = new QStandardItem(text);
	else
		projectPathItem->setText(text);

	projectPathItem->setToolTip(text);
	return projectPathItem;
}

QStandardItem* ProjectJob::getProjectCopyItem()
{
	QString text = tr("Prêt à copier");

	if(projectCopyItem == nullptr)
		projectCopyItem = new QStandardItem(text);
	else
		projectCopyItem->setText(text);

	return projectCopyItem;
}

qint64 const& ProjectJob::getProjectSize() const
{ return projectSize; }

void ProjectJob::setProjectPath(const QString &projectPath)
{
	if(projectPath != getProjectPath())
	{
		projectInfos.setProjectPath(projectPath.toStdString());
		needToCalculateProjectSize = true;
	}
}


void ProjectJob::proceedToSize()
{
	if(projectSizeWorkerRunning)
	{
		projectSizeWorker->quit();
		projectSizeWorker->wait();
	}

	projectSizeWorker = new DirSizeWorker(getProjectPath(), getUnkeepedDirNames(), getUnkeepedFileExtensions());

	connect(projectSizeWorker, &QThread::finished, projectSizeWorker, &QObject::deleteLater);
	connect(projectSizeWorker, &DirSizeWorker::dirSizeCalculated, this, &ProjectJob::updateProjectSize);

	projectSizeWorker->start();
	projectSizeWorkerRunning = true;
	needToCalculateProjectSize = false;
}


void ProjectJob::proceedToCopy()
{
	projectCopyRunning = true;
	emit projectCopyStateChanged(projectCopyRunning);

	QDir sourceDirectory(getProjectPath());
	QDir destinationDirectory(getDestinationPath() + "/" + getProjectName());

	// On stoppe la copie si le projet est déjà présent dans le répertoire de destination
	if(destinationDirectory.exists())
	{
		projectCopyRunning = false;
		emit projectCopyStateChanged(projectCopyRunning);

		return;
	}


	QString fileRelativePath, destinationFilePath, relativeDirname;
	qsizetype index = -1;

	QStringList unkeepedDirs = getUnkeepedDirNames();
	QStringList unkeepedExtensions = getUnkeepedFileExtensions();

	QDirIterator iterator(sourceDirectory.path(), QDir::AllEntries | QDir::Hidden, QDirIterator::IteratorFlag::Subdirectories);

	qint64 totalSize = 0;

	while(iterator.hasNext())
	{
		// Interruption de la copie
		if(interruptCopy)
		{
			updateCopyProjectProgress();
			break;
		}

		// Si l'iterator est sur un dossier, on passe
		if(iterator.fileInfo().isDir())
		{
			iterator.next();
			continue;
		}

		// On construit le chemin relatif du fichier dans le projet en retirant le chemin du dossier source
		fileRelativePath = iterator.fileInfo().filePath().remove(sourceDirectory.path() + "/");

		// On construit le chemin de destination du fichier
		destinationFilePath = destinationDirectory.path() + "/" + fileRelativePath;

		// On crée un FileInfo pour déterminer si le répertoire ou l'extension est à traiter ou non et vérifier que le chemin n'existe pas déjà
		QFileInfo fileInfo(destinationFilePath);

		// On cherche le premier /
		index = fileRelativePath.indexOf("/");
		relativeDirname = fileRelativePath.remove(index, fileRelativePath.length() - index);

		// Si le répertoire ou l'extension du fichier n'est pas à traiter, on passe au fichier suivant
		if(unkeepedDirs.contains(relativeDirname) || unkeepedExtensions.contains(fileInfo.suffix()))
		{
			iterator.next();
			continue;
		}

		// Si le répertoire de destination n'existe pas, on le crée
		if(!fileInfo.dir().exists())
			destinationDirectory.mkpath(fileInfo.dir().path());
		else if(QFile::exists(destinationFilePath))
			QFile::remove(destinationFilePath);

		// Copie du fichier
		QFile::copy(iterator.fileInfo().filePath(), destinationFilePath);

		// Ajout de la taille du fichier au total traité pour la progression de la copie
		totalSize += iterator.fileInfo().size();
		iterator.next();

		if(!projectSizeWorkerRunning && projectSize > 0)
		{
			projectCopyProgress = totalSize * 100 / projectSize;
			updateCopyProjectProgress();
		}
	}

	projectCopyRunning = false;
	emit projectCopyStateChanged(projectCopyRunning);
}

void ProjectJob::stopCopyProcess()
{
	interruptCopy = true;
}

void ProjectJob::updateCopyProjectProgress()
{
	QString itemText = tr("Copie en cours...");

	// Si la copie a été annulée, on affiche un message pour l'indiquer
	if(interruptCopy)
	{
		itemText = tr("Copie annulée");
		interruptCopy = true;
	}
	// Affichage de la progression de la copie du projet
	else if(projectCopyProgress < 100)
		itemText = QString::number(projectCopyProgress) + "%";
	else
		itemText = tr("Copie terminée");

	if(projectCopyItem != nullptr)
		projectCopyItem->setText(itemText);
}


void ProjectJob::updateProjectSize(qint64 const& byteCount)
{
	if(projectSizeItem != nullptr)
	{
		projectSizeItem->setText(StaticUtils::HumanizedSize(byteCount, "- B"));
		projectSizeItem->setToolTip(projectSizeItem->text());
	}

	projectSize = byteCount;
	projectSizeWorkerRunning = false;
}


bool ProjectJob::isKeepingLibrary() const
{ return projectInfos.isKeepingLibrary(); }

void ProjectJob::hasToKeepLibrary(bool keep)
{
	if(keep != projectInfos.isKeepingLibrary())
	{
		projectInfos.hasToKeepLibrary(keep);
		needToCalculateProjectSize = true;
	}
}


bool ProjectJob::isKeepingLogs() const
{ return projectInfos.isKeepingLogs(); }

void ProjectJob::hasToKeepLogs(bool keep)
{
	if(keep != projectInfos.isKeepingLogs())
	{
		projectInfos.hasToKeepLogs(keep);
		needToCalculateProjectSize = true;
	}
}


bool ProjectJob::isKeepingObj() const
{ return projectInfos.isKeepingObj(); }

void ProjectJob::hasToKeepObj(bool keep)
{
	if(keep != projectInfos.isKeepingObj())
	{
		projectInfos.hasToKeepObj(keep);
		needToCalculateProjectSize = true;
	}
}


bool ProjectJob::isKeepingVisualStudio() const
{ return projectInfos.isKeepingVisualStudio(); }

void ProjectJob::hasToKeepVisualStudio(bool keep)
{
	if(keep != projectInfos.isKeepingVisualStudio())
	{
		projectInfos.hasToKeepVisualStudio(keep);
		needToCalculateProjectSize = true;
	}
}


bool ProjectJob::isKeepingMetaFiles() const
{ return projectInfos.isKeepingMetaFiles(); }

void ProjectJob::hasToKeepMetaFiles(bool keep)
{
	if(keep != projectInfos.isKeepingMetaFiles())
	{
		projectInfos.hasToKeepMetaFiles(keep);
		needToCalculateProjectSize = true;
	}
}


bool ProjectJob::isKeepingGit() const
{ return projectInfos.isKeepingGit(); }

void ProjectJob::hasToKeepGit(bool keep)
{
	if(keep != projectInfos.isKeepingGit())
	{
		projectInfos.hasToKeepGit(keep);
		needToCalculateProjectSize = true;
	}
}


ProjectJob& ProjectJob::operator=(ProjectJob const& projectJob)
{
	if(this == &projectJob)
		return *this;

	if(projectNameItem != nullptr)
		delete projectNameItem;

	projectNameItem = new QStandardItem(projectJob.projectNameItem->text());

	if(projectSizeItem != nullptr)
		delete projectSizeItem;

	projectSizeItem = new QStandardItem(projectJob.projectSizeItem->text());

	if(projectPathItem != nullptr)
		delete projectPathItem;

	projectPathItem = new QStandardItem(projectJob.projectPathItem->text());

	if(projectCopyItem != nullptr)
		delete projectCopyItem;

	projectCopyItem = new QStandardItem(projectJob.projectCopyItem->text());

	projectSizeWorker->quit();
	projectSizeWorkerRunning = projectJob.projectSizeWorkerRunning;


	projectInfos = projectJob.projectInfos;

	if(projectSizeWorkerRunning)
		proceedToSize();
	else
	{
		projectSize = projectJob.projectSize;
		needToCalculateProjectSize = projectJob.needToCalculateProjectSize;
	}

	projectCopyRunning = false;
	projectCopyProgress = 0;
	interruptCopy = false;

	return *this;
}

ProjectJob::~ProjectJob()
{
	if(projectSizeWorkerRunning)
	{
		projectSizeWorker->quit();
		projectSizeWorker->wait();
	}
}
