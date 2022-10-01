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

	virtual void setKeepLibrary(int state);
	virtual void setKeepLogs(int state);
	virtual void setKeepObj(int state);
	virtual void setKeepVS(int state);
	virtual void setKeepGit(int state);
	virtual void setKeepMeta(int state);

	virtual void validAndCloseDialog();
	virtual void resetAndCloseDialog();

private:
	ProjectJob a_project;
	ProjectJob& o_project;

	QVBoxLayout *lt_main;
		QFormLayout *lt_details;
			QHBoxLayout *lt_projectPath;
				QLineEdit *projectPathLineEdit;
				QPushButton *projectPathButton;
			QHBoxLayout *lt_optionalDirs;
				QCheckBox *keepLibraryCheckBox;
				QCheckBox *keepLogsCheckbox;
				QCheckBox *keepObjCheckBox;
				QCheckBox *keepVSCheckBox;
				QCheckBox *keepGitCheckBox;
			QHBoxLayout *lt_optionalExtensions;
				QCheckBox *keepCSProjCheckBox;
				QCheckBox *keepMetaCheckBox;
		QHBoxLayout *lt_buttons;
			QPushButton *validButton;
			QPushButton *resetButton;
};

#endif // PROJECTDETAILSDIALOG_H
