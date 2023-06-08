#ifndef PROJECTINFOS_H
#define PROJECTINFOS_H

#include <string>

#include <QtDebug>

using namespace std;

class ProjectInfos
{
public:
	// Constructeurs
	explicit ProjectInfos(string const& projectPath, bool keepApk = true, bool keepBuild = true, bool keepExe = true, bool keepLibrary = false, bool keepLogs = false, bool keepObj = false, bool keepGit = true, bool keepPlastic = true, bool keepVSDir = false, bool keepVSFiles = false);
	ProjectInfos(ProjectInfos const& projectInfos);

	// Chemin du répertoire de destination de la copie
	static string const& getDestinationPath();
	static void setDestinationPath(string const& path);

	// Chemin du répertoire du projet
	virtual string const& getProjectPath() const;
	virtual void setProjectPath(string const& path);

	// Nom du projet et chemin du répertoire parent
	virtual string const& getProjectName() const;
	virtual string const& getParentFolderPath() const;


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


	// Répertoire .vs
	virtual bool isKeepingVSDir() const;
	virtual void hasToKeepVSDir(bool keep);

	// Fichiers .csproj, .vsconfig et .sln
	virtual bool isKeepingVSFiles() const;
	virtual void hasToKeepVSFiles(bool keep);


	virtual ProjectInfos& operator=(ProjectInfos const& projectInfos);

	// Destructeur
	virtual ~ProjectInfos();

private:
	static string DestinationPath;

	string projectPath;
	string projectName;

	string parentFolderPath;

	bool keepApk;
	bool keepBuild;
	bool keepExe;

	bool keepLibrary;
	bool keepLogs;
	bool keepObj;

	bool keepGit;
	bool keepPlastic;

	bool keepVSDir;
	bool keepVSFiles;
};

#endif // PROJECTINFOS_H
