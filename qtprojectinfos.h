#ifndef QTPROJECTINFOS_H
#define QTPROJECTINFOS_H

#include <QStandardItemModel>

#include <QList>
#include <QString>

#include <QDir>
#include <QDirIterator>

#include "projectinfos.h"

#include "dirsizeworker.h"

#include "staticutils.h"

using namespace std;

class QtProjectInfos : public ProjectInfos
{
	Q_OBJECT

public:
	explicit QtProjectInfos(QString const& projectDirPath);
	QtProjectInfos(QtProjectInfos const& projectInfos);

	virtual QList<QStandardItem*> getStandardItems();

	virtual QStandardItem* getProjectNameItem();
	virtual QStandardItem* getProjectSizeItem();
	virtual QStandardItem* getProjectPathItem();
	virtual QStandardItem* getProjectCopyItem();

	virtual qint64 const& getProjectSize() const;
	virtual void setFolderPath(string const& folderPath) override;

	virtual QtProjectInfos& operator=(QtProjectInfos const& projectInfos);

	virtual ~QtProjectInfos() override;

public Q_SLOTS:
	virtual void proceedToCopy();
	virtual void stopCopyProcess();

private Q_SLOTS:
	virtual void updateProjectSize(qint64 const& byteCount);

private:
	QStandardItem* projectNameItem;
	QStandardItem* projectSizeItem;
	QStandardItem* projectPathItem;
	QStandardItem* projectCopyItem;

	DirSizeWorker* projectSizeWorker;
	bool projectSizeWorkerRunning;
	qint64 projectSize;
	bool projectPathChanged;

	unsigned char projectCopyProgress;
	bool interruptCopy;
};

#endif // QTPROJECTINFOS_H
