#ifndef PROJECTCOPYWORKER_H
#define PROJECTCOPYWORKER_H

#include <QThread>

#include "qtprojectinfos.h"

class ProjectCopyWorker : public QThread
{
	Q_OBJECT

public:
	explicit ProjectCopyWorker(QList<QtProjectInfos> &projects, QObject *parent = nullptr);

	virtual void run() override;
	virtual void stopCopyProcess();

Q_SIGNALS:
	void interruptCopy();

private:
	QList<QtProjectInfos> &projects;
	bool interrupt;
};

#endif // PROJECTCOPYWORKER_H
