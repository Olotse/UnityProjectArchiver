#include "projectinfos.h"

string ProjectInfos::DestinationPath = "";

ProjectInfos::ProjectInfos(string const& projectPath, bool keepApk, bool keepBuild, bool keepExe, bool keepLibrary, bool keepLogs, bool keepObj, bool keepGit, bool keepPlastic, bool keepVSDir, bool keepVSFiles) :
	projectPath(projectPath),

	keepApk(keepApk),
	keepBuild(keepBuild),
	keepExe(keepExe),

	keepLibrary(keepLibrary),
	keepLogs(keepLogs),
	keepObj(keepObj),

	keepGit(keepGit),
	keepPlastic(keepPlastic),

	keepVSDir(keepVSDir),
	keepVSFiles(keepVSFiles)
{
	// Source https://cplusplus.com/reference/string/string/find_last_of/
	size_t index = projectPath.find_last_of("/\\");

	parentFolderPath = projectPath.substr(0, index);
	projectName = projectPath.substr(index+1);
}

ProjectInfos::ProjectInfos(ProjectInfos const& projectInfos) :
	ProjectInfos(projectInfos.projectPath, projectInfos.keepApk, projectInfos.keepBuild, projectInfos.keepExe, projectInfos.keepLibrary, projectInfos.keepLogs, projectInfos.keepObj, projectInfos.keepGit, projectInfos.keepPlastic, projectInfos.keepVSDir, projectInfos.keepVSFiles)
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


bool ProjectInfos::isKeepingApk() const
{ return keepApk; }

void ProjectInfos::hasToKeepApk(bool keep)
{ keepApk = keep; }


bool ProjectInfos::isKeepingBuild() const
{ return keepBuild; }

void ProjectInfos::hasToKeepBuild(bool keep)
{ keepBuild = keep; }


bool ProjectInfos::isKeepingExe() const
{ return keepExe; }

void ProjectInfos::hasToKeepExe(bool keep)
{ keepExe = keep; }


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


bool ProjectInfos::isKeepingGit() const
{ return keepGit; }

void ProjectInfos::hasToKeepGit(bool keep)
{ keepGit = keep; }


bool ProjectInfos::isKeepingPlastic() const
{ return keepPlastic; }

void ProjectInfos::hasToKeepPlastic(bool keep)
{ keepPlastic = keep; }


bool ProjectInfos::isKeepingVSDir() const
{ return keepVSDir; }

void ProjectInfos::hasToKeepVSDir(bool keep)
{ keepVSDir = keep; }


bool ProjectInfos::isKeepingVSFiles() const
{ return keepVSFiles; }

void ProjectInfos::hasToKeepVSFiles(bool keep)
{ keepVSFiles = keep; }


ProjectInfos& ProjectInfos::operator=(const ProjectInfos &projectInfos)
{
	if(this == &projectInfos)
		return *this;

	projectPath = projectInfos.projectPath;
	projectName = projectInfos.projectName;

	parentFolderPath = projectInfos.parentFolderPath;

	keepApk = projectInfos.keepApk;
	keepBuild = projectInfos.keepBuild;
	keepExe = projectInfos.keepExe;

	keepLibrary = projectInfos.keepLibrary;
	keepLogs = projectInfos.keepLogs;
	keepObj = projectInfos.keepObj;

	keepGit = projectInfos.keepGit;
	keepPlastic = projectInfos.keepPlastic;

	keepVSDir = projectInfos.keepVSDir;
	keepVSFiles = projectInfos.keepVSFiles;

	return *this;
}


ProjectInfos::~ProjectInfos() {}
