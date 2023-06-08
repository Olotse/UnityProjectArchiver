#ifndef PROJECTDETAILSDIALOG_H
#define PROJECTDETAILSDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

#include <QDir>

#include "projectjob.h"
#include "staticutils.h"

class ProjectDetailsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ProjectDetailsDialog(ProjectJob& project, QWidget *parent = nullptr);
	virtual ~ProjectDetailsDialog();

Q_SIGNALS:

private Q_SLOTS:
	virtual void browseProjectPath();

	virtual void setKeepApk(int state);
	virtual void setKeepBuild(int state);
	virtual void setKeepExe(int state);

	virtual void setKeepLibrary(int state);
	virtual void setKeepLogs(int state);
	virtual void setKeepObj(int state);

	virtual void setKeepGit(int state);
	virtual void setKeepPlastic(int state);

	virtual void setKeepVSDir(int state);
	virtual void setKeepVSFiles(int state);

	virtual void validAndCloseDialog();
	virtual void resetAndCloseDialog();

private:
	ProjectJob a_project;
	ProjectJob& o_project;

	QVBoxLayout *lt_main;
		QFormLayout *lt_details;
			// Project path
			QHBoxLayout *lt_projectPath;
				QLineEdit *projectPathLineEdit;
				QPushButton *projectPathButton;

			// Builds dirs
			QHBoxLayout *lt_buildDirs;
				QCheckBox *keepApkCheckBox;
				QCheckBox *keepBuildCheckbox;
				QCheckBox *keepExeCheckBox;

			// Unity cache and logs dirs
			QHBoxLayout *lt_cacheDirs;
				QCheckBox *keepLibraryCheckBox;
				QCheckBox *keepLogsCheckbox;
				QCheckBox *keepObjCheckBox;

			// Versioning dirs
			QHBoxLayout *lt_versioningDirs;
				QCheckBox *keepGitCheckBox;
				QCheckBox *keepPlasticCheckBox;

			// Visual studio dirs
			QHBoxLayout *lt_vsDirs;
				QCheckBox *keepVSDirCheckBox;
				QCheckBox *keepVSFilesCheckBox;

		QHBoxLayout *lt_buttons;
			QPushButton *validButton;
			QPushButton *resetButton;
};

#endif // PROJECTDETAILSDIALOG_H
