#ifndef PROJECTINFOS_H
#define PROJECTINFOS_H

#include <QObject>
#include <string>

using namespace std;

class ProjectInfos : public QObject
{
	Q_OBJECT

public:
	explicit ProjectInfos();
	ProjectInfos(string const& folderPath, bool keepLibrary = false, bool keepLogs = false, bool keepObj = false);
	ProjectInfos(ProjectInfos const& projectInfos);

	static string const& getDestinationPath();
	static void setDestinationPath(string const& path);

	virtual string const& getFolderPath() const;
	virtual void setFolderPath(string const& path);

	virtual bool isKeepingLibrary() const;
	virtual void hasToKeepLibrary(bool keep);

	virtual bool isKeepingLogs() const;
	virtual void hasToKeepLogs(bool keep);

	virtual bool isKeepingObj() const;
	virtual void hasToKeepObj(bool keep);

	virtual ~ProjectInfos();

Q_SIGNALS:

private:
	static string DestinationPath;

	string folderPath;

	bool keepLibrary;
	bool keepLogs;
	bool keepObj;
};

#endif // PROJECTINFOS_H
