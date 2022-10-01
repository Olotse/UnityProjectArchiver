#include "projectinfos.h"

string ProjectInfos::DestinationPath = "";

ProjectInfos::ProjectInfos(string const& projectPath, bool keepLibrary, bool keepLogs, bool keepObj, bool keepVS, bool keepMeta, bool keepGit) :
	projectPath(projectPath),

	keepLibrary(keepLibrary),
	keepLogs(keepLogs),
	keepObj(keepObj),

	keepVS(keepVS),
	keepMeta(keepMeta),
	keepGit(keepGit)
{
	// Source https://cplusplus.com/reference/string/string/find_last_of/
	size_t index = projectPath.find_last_of("/\\");

	parentFolderPath = projectPath.substr(0, index);
	projectName = projectPath.substr(index+1);
}

ProjectInfos::ProjectInfos(ProjectInfos const& projectInfos) :
	ProjectInfos(projectInfos.projectPath, projectInfos.keepLibrary, projectInfos.keepLogs, projectInfos.keepObj)
{}


string const& ProjectInfos::getDestinationPath()
{ return DestinationPath; }

void ProjectInfos::setDestinationPath(string const& path)
{ DestinationPath = path; }


string const& ProjectInfos::getProjectPath() const
{ return projectPath; }

void ProjectInfos::setProjectPath(string const& path)
{
	projectPath = path;

	// Source https://cplusplus.com/reference/string/string/find_last_of/
	size_t index = projectPath.find_last_of("/\\");

	parentFolderPath = projectPath.substr(0, index);
	projectName = projectPath.substr(index+1);
}


string const& ProjectInfos::getProjectName() const
{ return projectName; }

string const& ProjectInfos::getParentFolderPath() const
{ return parentFolderPath; }


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


bool ProjectInfos::isKeepingVisualStudio() const
{ return keepVS; }

void ProjectInfos::hasToKeepVisualStudio(bool keep)
{ keepVS = keep; }


bool ProjectInfos::isKeepingMetaFiles() const
{ return keepMeta; }

void ProjectInfos::hasToKeepMetaFiles(bool keep)
{ keepMeta = keep; }


bool ProjectInfos::isKeepingGit() const
{ return keepGit; }

void ProjectInfos::hasToKeepGit(bool keep)
{ keepGit = keep; }


ProjectInfos& ProjectInfos::operator=(const ProjectInfos &projectInfos)
{
	if(this == &projectInfos)
		return *this;

	projectPath = projectInfos.projectPath;
	projectName = projectInfos.projectName;

	parentFolderPath = projectInfos.parentFolderPath;

	keepLibrary = projectInfos.keepLibrary;
	keepLogs = projectInfos.keepLogs;
	keepObj = projectInfos.keepObj;

	keepVS = projectInfos.keepVS;
	keepMeta = projectInfos.keepMeta;
	keepGit = projectInfos.keepGit;

	return *this;
}


ProjectInfos::~ProjectInfos() {}
