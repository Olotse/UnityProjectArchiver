#ifndef PROJECTINFOS_H
#define PROJECTINFOS_H

#include <string>

using namespace std;

class ProjectInfos
{
public:
	// Constructeurs
	explicit ProjectInfos(string const& projectPath, bool keepLibrary = false, bool keepLogs = false, bool keepObj = false, bool keepVS = false, bool keepMeta = false, bool keepGit = true);
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

	// Répertoire Library
	virtual bool isKeepingLibrary() const;
	virtual void hasToKeepLibrary(bool keep);

	// Répertoire Logs
	virtual bool isKeepingLogs() const;
	virtual void hasToKeepLogs(bool keep);

	// Répertoire obj
	virtual bool isKeepingObj() const;
	virtual void hasToKeepObj(bool keep);

	// Répertoire .vs et fichiers .csproj
	virtual bool isKeepingVisualStudio() const;
	virtual void hasToKeepVisualStudio(bool keep);

	// Fichiers .meta
	virtual bool isKeepingMetaFiles() const;
	virtual void hasToKeepMetaFiles(bool keep);

	// Répertoire .git
	virtual bool isKeepingGit() const;
	virtual void hasToKeepGit(bool keep);

	virtual ProjectInfos& operator=(ProjectInfos const& projectInfos);

	// Destructeur
	virtual ~ProjectInfos();

private:
	static string DestinationPath;

	string projectPath;
	string projectName;

	string parentFolderPath;

	bool keepLibrary;
	bool keepLogs;
	bool keepObj;

	bool keepVS;
	bool keepMeta;
	bool keepGit;
};

#endif // PROJECTINFOS_H
