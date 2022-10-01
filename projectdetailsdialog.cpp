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


	keepLibraryCheckBox = new QCheckBox(tr("Conserver Library"));
	keepLibraryCheckBox->setChecked(o_project.isKeepingLibrary());

	keepLogsCheckbox = new QCheckBox(tr("Conserver Logs"));
	keepLogsCheckbox->setChecked(o_project.isKeepingLogs());

	keepObjCheckBox = new QCheckBox(tr("Conserver obj"));
	keepObjCheckBox->setChecked(o_project.isKeepingObj());

	keepVSCheckBox = new QCheckBox(tr("Conserver .vs"));
	keepVSCheckBox->setChecked(o_project.isKeepingVisualStudio());

	keepGitCheckBox = new QCheckBox(tr("Conserver .git"));
	keepGitCheckBox->setChecked(o_project.isKeepingGit());

	lt_optionalDirs = new QHBoxLayout();
	lt_optionalDirs->addWidget(keepLibraryCheckBox);
	lt_optionalDirs->addWidget(keepLogsCheckbox);
	lt_optionalDirs->addWidget(keepObjCheckBox);
	lt_optionalDirs->addWidget(keepVSCheckBox);
	lt_optionalDirs->addWidget(keepGitCheckBox);


	keepCSProjCheckBox = new QCheckBox(tr("Conserver *.csproj, *.sln et .vsconfig"));
	keepCSProjCheckBox->setChecked(o_project.isKeepingVisualStudio());

	keepMetaCheckBox = new QCheckBox(tr("Conserver *.meta"));
	keepMetaCheckBox->setChecked(o_project.isKeepingMetaFiles());

	lt_optionalExtensions = new QHBoxLayout();
	lt_optionalExtensions->addWidget(keepCSProjCheckBox);
	lt_optionalExtensions->addWidget(keepMetaCheckBox);


	lt_details = new QFormLayout();
	lt_details->addRow(tr("Chemin du projet"), lt_projectPath);
	lt_details->addRow(tr("Répertoires qui peuvent être ignorés"), lt_optionalDirs);
	lt_details->addRow(tr("Fichiers qui peuvent être ignorés"), lt_optionalExtensions);


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

	connect(keepLibraryCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepLibrary);
	connect(keepLogsCheckbox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepLogs);
	connect(keepObjCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepObj);
	connect(keepVSCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepVS);
	connect(keepGitCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepGit);

	connect(keepCSProjCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepVS);
	connect(keepMetaCheckBox, &QCheckBox::stateChanged, this, &ProjectDetailsDialog::setKeepMeta);

	connect(validButton, SIGNAL(clicked(bool)), this, SLOT(validAndCloseDialog()));
	connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetAndCloseDialog()));
}

ProjectDetailsDialog::~ProjectDetailsDialog()
{
	StaticUtils::deleteLayout(lt_projectPath);
	StaticUtils::deleteLayout(lt_optionalDirs);
	StaticUtils::deleteLayout(lt_optionalExtensions);

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


void ProjectDetailsDialog::setKeepLibrary(int state)
{ o_project.hasToKeepLibrary(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepLogs(int state)
{ o_project.hasToKeepLogs(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepObj(int state)
{ o_project.hasToKeepObj(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepVS(int state)
{
	o_project.hasToKeepVisualStudio(state == Qt::CheckState::Checked);

	keepVSCheckBox->setChecked(state == Qt::CheckState::Checked);
	keepCSProjCheckBox->setChecked(state == Qt::CheckState::Checked);
}

void ProjectDetailsDialog::setKeepGit(int state)
{ o_project.hasToKeepGit(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepMeta(int state)
{ o_project.hasToKeepMetaFiles(state == Qt::CheckState::Checked); }


void ProjectDetailsDialog::validAndCloseDialog()
{
	o_project.setProjectPath(o_project.getProjectPath());
	this->close();
}

void ProjectDetailsDialog::resetAndCloseDialog()
{
	o_project.setProjectPath(a_project.getProjectPath());

	o_project.hasToKeepLibrary(a_project.isKeepingLibrary());
	o_project.hasToKeepLogs(a_project.isKeepingLogs());
	o_project.hasToKeepObj(a_project.isKeepingObj());

	o_project.hasToKeepVisualStudio(a_project.isKeepingVisualStudio());
	o_project.hasToKeepMetaFiles(a_project.isKeepingMetaFiles());
	o_project.hasToKeepGit(a_project.isKeepingGit());

	this->close();
}
