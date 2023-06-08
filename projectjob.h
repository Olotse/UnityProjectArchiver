#ifndef PROJECTJOB_H
#define PROJECTJOB_H

#include <QObject>
#include <QStandardItemModel>

#include <QList>
#include <QString>

#include <QDir>
#include <QDirIterator>

#include "projectinfos.h"
#include "dirsizeworker.h"

#include "staticutils.h"

using namespace std;

class ProjectJob : public QObject
{
	Q_OBJECT

public:
	explicit ProjectJob(QString const& projectDirPath, QObject *parent = nullptr);
	ProjectJob(ProjectJob const& projectJob);

	static QString getDestinationPath();
	static void setDestinationPath(QString const& dirPath);

	virtual QString getProjectPath() const;
	virtual QString getProjectName() const;
	virtual QString getParentFolderPath() const;

	virtual QStringList getUnkeepedDirNames() const;
	virtual QStringList getUnkeepedFileExtensions() const;

	virtual QList<QStandardItem*> getStandardItems();

	virtual QStandardItem* getProjectNameItem();
	virtual QStandardItem* getProjectSizeItem();
	virtual QStandardItem* getProjectPathItem();
	virtual QStandardItem* getProjectCopyItem();

	virtual qint64 const& getProjectSize() const;
	virtual void setProjectPath(QString const& projectPath);

	// Répertoire Apk[s]
	virtual bool isKeepingApk() const;
	virtual void hasToKeepApk(bool keep);

	// Répertoire Build[s]
	virtual bool isKeepingBuild() const;
	virtual void hasToKeepBuild(bool keep);

	// Répertoire Exe[s]
	virtual bool isKeepingExe() const;
	virtual void hasToKeepExe(bool keep);

	// Répertoire Library
	virtual bool isKeepingLibrary() const;
	virtual void hasToKeepLibrary(bool keep);

	// Répertoire Logs
	virtual bool isKeepingLogs() const;
	virtual void hasToKeepLogs(bool keep);

	// Répertoire obj
	virtual bool isKeepingObj() const;
	virtual void hasToKeepObj(bool keep);

	// Répertoire .git
	virtual bool isKeepingGit() const;
	virtual void hasToKeepGit(bool keep);

	// Répertoire .plastic
	virtual bool isKeepingPlastic() const;
	virtual void hasToKeepPlastic(bool keep);

	// Fichiers .csproj
	virtual bool isKeepingVSDir() const;
	virtual void hasToKeepVSDir(bool keep);

	// Répertoire .vs
	virtual bool isKeepingVSFiles() const;
	virtual void hasToKeepVSFiles(bool keep);

	virtual ProjectJob& operator=(ProjectJob const& projectJob);

	virtual ~ProjectJob();

public Q_SLOTS:
	virtual void proceedToCopy();
	virtual void stopCopyProcess();
	virtual void updateCopyProjectProgress();

Q_SIGNALS:
	void projectCopyStateChanged(bool state);
	void projectCopyProgressed();

private Q_SLOTS:
	virtual void proceedToSize();
	virtual void updateProjectSize(qint64 const& byteCount);

private:
	ProjectInfos projectInfos;

	QStandardItem* projectNameItem;
	QStandardItem* projectSizeItem;
	QStandardItem* projectPathItem;
	QStandardItem* projectCopyItem;

	DirSizeWorker* projectSizeWorker;
	bool projectSizeWorkerRunning;
	qint64 projectSize;
	bool needToCalculateProjectSize;

	bool projectCopyRunning;
	unsigned char projectCopyProgress;
	bool interruptCopy;
};

#endif // QTPROJECTINFOS_H
