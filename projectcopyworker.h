#ifndef PROJECTCOPYWORKER_H
#define PROJECTCOPYWORKER_H

#include <QThread>

#include "projectjob.h"

class ProjectCopyWorker : public QThread
{
	Q_OBJECT

public:
	explicit ProjectCopyWorker(QList<ProjectJob> &projects, QObject *parent = nullptr);

	virtual void run() override;
	virtual void stopCopyProcess();

Q_SIGNALS:
	void interruptCopy();

private:
	QList<ProjectJob> &projects;
	bool interrupt;
};

#endif // PROJECTCOPYWORKER_H
