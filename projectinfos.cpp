#include "projectinfos.h"

string ProjectInfos::DestinationPath = "";

ProjectInfos::ProjectInfos() : QObject()
{}

ProjectInfos::ProjectInfos(string const& folderPath, bool keepLibrary, bool keepLogs, bool keepObj) :
	QObject(),
	folderPath(folderPath),
	keepLibrary(keepLibrary),
	keepLogs(keepLogs),
	keepObj(keepObj)
{}

ProjectInfos::ProjectInfos(ProjectInfos const& projectInfos) :
	ProjectInfos(projectInfos.folderPath, projectInfos.keepLibrary, projectInfos.keepLogs, projectInfos.keepObj)
{}


string const& ProjectInfos::getDestinationPath()
{ return DestinationPath; }

void ProjectInfos::setDestinationPath(string const& path)
{ DestinationPath = path; }


string const& ProjectInfos::getFolderPath() const
{ return folderPath; }

void ProjectInfos::setFolderPath(const string &path)
{ this->folderPath = path; }


bool ProjectInfos::isKeepingLibrary() const
{ return keepLibrary; }

void ProjectInfos::hasToKeepLibrary(bool keep)
{ keepLibrary = keep; }


bool ProjectInfos::isKeepingLogs() const
{ return keepLogs; }

void ProjectInfos::hasToKeepLogs(bool keep)
{ keepLogs = keep; }


bool ProjectInfos::isKeepingObj() const
{ return keepObj; }

void ProjectInfos::hasToKeepObj(bool keep)
{ keepObj = keep; }

ProjectInfos::~ProjectInfos() {}
