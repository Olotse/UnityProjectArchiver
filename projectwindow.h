#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <QGroupBox>

#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTableView>
#include <QHeaderView>

#include <QStandardItemModel>

#include "qtprojectinfos.h"
#include "staticutils.h"

#include "projectdetailsdialog.h"
#include "projectcopyworker.h"

using namespace std;

class ProjectWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit ProjectWindow(QWidget *parent = nullptr);
	virtual ~ProjectWindow();

Q_SIGNALS:

private Q_SLOTS:
	virtual void getProjectByBrowsingInFiles();
	virtual void removeSelectedProjects();

	virtual void showProjectDetails(QModelIndex const& index);

	virtual void getDestinationByBrowsingInFiles();

	virtual void copyProjectsToDestination();
	virtual void onCopyFinished();

private:
	static QString Destination_Path;

	QString o_currentProjectPath;
	QString o_currentDestinationPath;

	bool o_keepLibrary;
	bool o_keepLogs;
	bool o_keepObj;

	QList<QtProjectInfos> o_projectList;
	QStandardItemModel *o_projectModel;

	ProjectCopyWorker *projectCopyWorker;
	bool projectCopyWorkerRunning;
	Qt::WindowFlags currentWindowFlags;

	QWidget *centralWidget;
		QVBoxLayout *lt_main;
			QFrame *topBarFrame;
				QHBoxLayout *lt_topBar;
					QPushButton *addProjectButton;
					QPushButton *delProjectButton;
			QTableView *projectTableView;
			QVBoxLayout *lt_destinationPathSet;
				QHBoxLayout *lt_destinationPathForm;
					QLabel *destinationPathLabel;
					QLineEdit *destinationPathLineEdit;
					QPushButton *destinationPathButton;
				QLabel *destinationWarningLabel;
			QFrame *footerBarFrame;
				QHBoxLayout *lt_footerBar;
					QPushButton *copyButton;
					QPushButton *quitButton;
};

#endif // PROJECTWINDOW_H
