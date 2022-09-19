#include "projectcopyworker.h"

ProjectCopyWorker::ProjectCopyWorker(QList<QtProjectInfos> &projects, QObject *parent) :
	QThread(parent),
	projects(projects),
	interrupt(false)
{}

void ProjectCopyWorker::run()
{
	for(int i = 0; i < projects.count(); i++)
	{
		if(interrupt)
		{
			interrupt = false;
			break;
		}

		connect(this, &ProjectCopyWorker::interruptCopy, &projects[i], &QtProjectInfos::stopCopyProcess);
		projects[i].proceedToCopy();
		disconnect(this, &ProjectCopyWorker::interruptCopy, &projects[i], &QtProjectInfos::stopCopyProcess);
	}
}

void ProjectCopyWorker::stopCopyProcess()
{
	interrupt = true;
	emit interruptCopy();
}
