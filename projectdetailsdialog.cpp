#include "projectdetailsdialog.h"

ProjectDetailsDialog::ProjectDetailsDialog(ProjectJob& project, QWidget *parent) :
	QDialog(parent),
	a_project(project),
	o_project(project)
{
	projectPathLineEdit = new QLineEdit(o_project.getProjectPath());
	projectPathLineEdit->setToolTip(o_project.getProjectPath());

	projectPathButton = new QPushButton(tr("Modifier"));

	lt_projectPath = new QHBoxLayout();
	lt_projectPath->addWidget(projectPathLineEdit);
	lt_projectPath->addWidget(projectPathButton);


	keepApkCheckBox = new QCheckBox(tr("Conserver APKs"));
	keepApkCheckBox->setChecked(o_project.isKeepingApk());

	keepBuildCheckbox = new QCheckBox(tr("Conserver Builds"));
	keepBuildCheckbox->setChecked(o_project.isKeepingBuild());

	keepExeCheckBox = new QCheckBox(tr("Conserver EXEs"));
	keepExeCheckBox->setChecked(o_project.isKeepingExe());

	lt_buildDirs = new QHBoxLayout();
	lt_buildDirs->addWidget(keepApkCheckBox);
	lt_buildDirs->addWidget(keepBuildCheckbox);
	lt_buildDirs->addWidget(keepExeCheckBox);


	keepLibraryCheckBox = new QCheckBox(tr("Conserver Library"));
	keepLibraryCheckBox->setChecked(o_project.isKeepingLibrary());

	keepLogsCheckbox = new QCheckBox(tr("Conserver Logs"));
	keepLogsCheckbox->setChecked(o_project.isKeepingLogs());

	keepObjCheckBox = new QCheckBox(tr("Conserver obj"));
	keepObjCheckBox->setChecked(o_project.isKeepingObj());

	lt_cacheDirs = new QHBoxLayout();
	lt_cacheDirs->addWidget(keepLibraryCheckBox);
	lt_cacheDirs->addWidget(keepLogsCheckbox);
	lt_cacheDirs->addWidget(keepObjCheckBox);


	keepGitCheckBox = new QCheckBox(tr("Conserver .git"));
	keepGitCheckBox->setChecked(o_project.isKeepingGit());

	keepPlasticCheckBox = new QCheckBox(tr("Conserver .plastic"));
	keepPlasticCheckBox->setChecked(o_project.isKeepingPlastic());

	lt_versioningDirs = new QHBoxLayout();
	lt_versioningDirs->addWidget(keepGitCheckBox);
	lt_versioningDirs->addWidget(keepPlasticCheckBox);


	keepVSDirCheckBox = new QCheckBox(tr("Conserver .vs"));
	keepVSDirCheckBox->setChecked(o_project.isKeepingVSDir());

	keepVSFilesCheckBox = new QCheckBox(tr("Conserver .csproj, .vsconfig et .sln"));
	keepVSFilesCheckBox->setChecked(o_project.isKeepingVSFiles());

	lt_vsDirs = new QHBoxLayout();
	lt_vsDirs->addWidget(keepGitCheckBox);
	lt_vsDirs->addWidget(keepVSFilesCheckBox);


	lt_details = new QFormLayout();
	lt_details->addRow(tr("Chemin du projet"), lt_projectPath);
	lt_details->addRow(tr("Cache et Logs de Unity"), lt_cacheDirs);
	lt_details->addRow(tr("Répertoires des compilations"), lt_buildDirs);
	lt_details->addRow(tr("Répertoire des outils de versioning"), lt_versioningDirs);
	lt_details->addRow(tr("Répertoire et fichiers de Visual Studio"), lt_vsDirs);


	validButton = new QPushButton(tr("Valider"));
	resetButton = new QPushButton(tr("Annuler"));

	lt_buttons = new QHBoxLayout();
	lt_buttons->addWidget(validButton);
	lt_buttons->addWidget(resetButton);


	lt_main = new QVBoxLayout();
	lt_main->addLayout(lt_details);
	lt_main->addLayout(lt_buttons);

	this->setWindowTitle(o_project.getProjectName());
	this->setLayout(lt_main);

	connect(projectPathButton, &QPushButton::clicked, this, &ProjectDetailsDialog::browseProjectPath);

	connect(keepApkCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepApk);
	connect(keepBuildCheckbox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepBuild);
	connect(keepExeCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepExe);

	connect(keepLibraryCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepLibrary);
	connect(keepLogsCheckbox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepLogs);
	connect(keepObjCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepObj);

	connect(keepGitCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepGit);
	connect(keepPlasticCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepPlastic);

	connect(keepVSDirCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepVSDir);
	connect(keepVSFilesCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepVSFiles);

	connect(validButton, SIGNAL(clicked(bool)), this, SLOT(validAndCloseDialog()));
	connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetAndCloseDialog()));
}

ProjectDetailsDialog::~ProjectDetailsDialog()
{
	StaticUtils::deleteLayout(lt_projectPath);
	StaticUtils::deleteLayout(lt_cacheDirs);
	StaticUtils::deleteLayout(lt_buildDirs);
	StaticUtils::deleteLayout(lt_versioningDirs);
	StaticUtils::deleteLayout(lt_vsDirs);

	StaticUtils::deleteFormLayout(lt_details);
	StaticUtils::deleteLayout(lt_buttons);

	StaticUtils::deleteLayout(lt_main);
}

void ProjectDetailsDialog::browseProjectPath()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Sélection du dossier du projet"), o_project.getProjectPath());

	if(dirPath != "")
	{
		o_project.setProjectPath(dirPath);
		projectPathLineEdit->setText(dirPath);
	}
}

void ProjectDetailsDialog::setKeepApk(int state)
{ o_project.hasToKeepApk(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepBuild(int state)
{ o_project.hasToKeepBuild(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepExe(int state)
{ o_project.hasToKeepExe(state == Qt::CheckState::Checked); }


void ProjectDetailsDialog::setKeepLibrary(int state)
{ o_project.hasToKeepLibrary(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepLogs(int state)
{ o_project.hasToKeepLogs(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepObj(int state)
{ o_project.hasToKeepObj(state == Qt::CheckState::Checked); }


void ProjectDetailsDialog::setKeepGit(int state)
{ o_project.hasToKeepGit(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepPlastic(int state)
{ o_project.hasToKeepPlastic(state == Qt::CheckState::Checked); }


void ProjectDetailsDialog::setKeepVSDir(int state)
{ o_project.hasToKeepVSDir(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepVSFiles(int state)
{ o_project.hasToKeepVSFiles(state == Qt::CheckState::Checked); }


void ProjectDetailsDialog::validAndCloseDialog()
{
	o_project.setProjectPath(o_project.getProjectPath());
	this->close();
}

void ProjectDetailsDialog::resetAndCloseDialog()
{
	o_project.setProjectPath(a_project.getProjectPath());

	o_project.hasToKeepApk(a_project.isKeepingApk());
	o_project.hasToKeepBuild(a_project.isKeepingBuild());
	o_project.hasToKeepExe(a_project.isKeepingExe());

	o_project.hasToKeepLibrary(a_project.isKeepingLibrary());
	o_project.hasToKeepLogs(a_project.isKeepingLogs());
	o_project.hasToKeepObj(a_project.isKeepingObj());

	o_project.hasToKeepGit(a_project.isKeepingGit());
	o_project.hasToKeepPlastic(a_project.isKeepingPlastic());

	o_project.hasToKeepVSDir(a_project.isKeepingVSDir());
	o_project.hasToKeepVSFiles(a_project.isKeepingVSFiles());

	this->close();
}
