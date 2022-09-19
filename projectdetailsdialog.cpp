#include "projectdetailsdialog.h"

ProjectDetailsDialog::ProjectDetailsDialog(QtProjectInfos& projectInfos, QWidget *parent) :
	QDialog(parent),
	a_projectInfos(projectInfos),
	o_projectInfos(projectInfos)
{
	projectPathLineEdit = new QLineEdit(QString::fromStdString(o_projectInfos.getFolderPath()));
	projectPathLineEdit->setToolTip(QString::fromStdString(o_projectInfos.getFolderPath()));

	projectPathButton = new QPushButton(tr("Modifier"));

	lt_projectPath = new QHBoxLayout();
	lt_projectPath->addWidget(projectPathLineEdit);
	lt_projectPath->addWidget(projectPathButton);


	keepLibraryCheckBox = new QCheckBox(tr("Conserver Library"));
	keepLibraryCheckBox->setChecked(o_projectInfos.isKeepingLibrary());

	keepLogsCheckbox = new QCheckBox(tr("Conserver Logs"));
	keepLogsCheckbox->setChecked(o_projectInfos.isKeepingLogs());

	keepObjCheckBox = new QCheckBox(tr("Conserver obj"));
	keepObjCheckBox->setChecked(o_projectInfos.isKeepingObj());

	lt_optionalDirs = new QHBoxLayout();
	lt_optionalDirs->addWidget(keepLibraryCheckBox);
	lt_optionalDirs->addWidget(keepLogsCheckbox);
	lt_optionalDirs->addWidget(keepObjCheckBox);


	lt_details = new QFormLayout();
	lt_details->addRow(tr("Chemin du projet"), lt_projectPath);
	lt_details->addRow(tr("Répertoires qui peuvent être ignorés"), lt_optionalDirs);


	validButton = new QPushButton(tr("Valider"));
	resetButton = new QPushButton(tr("Annuler"));

	lt_buttons = new QHBoxLayout();
	lt_buttons->addWidget(validButton);
	lt_buttons->addWidget(resetButton);


	lt_main = new QVBoxLayout();
	lt_main->addLayout(lt_details);
	lt_main->addLayout(lt_buttons);


	QDir dir(QString::fromStdString(o_projectInfos.getFolderPath()));

	this->setWindowTitle(dir.dirName());
	this->setLayout(lt_main);

	connect(projectPathButton, SIGNAL(clicked(bool)), this, SLOT(browseProjectPath()));

	connect(keepLibraryCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setKeepLibrary(int)));
	connect(keepLogsCheckbox, SIGNAL(stateChanged(int)), this, SLOT(setKeepLogs(int)));
	connect(keepObjCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setKeepObj(int)));

	connect(validButton, SIGNAL(clicked(bool)), this, SLOT(validAndCloseDialog()));
	connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetAndCloseDialog()));
}

ProjectDetailsDialog::~ProjectDetailsDialog()
{
	StaticUtils::deleteLayout(lt_projectPath);
	StaticUtils::deleteLayout(lt_optionalDirs);

	StaticUtils::deleteFormLayout(lt_details);
	StaticUtils::deleteLayout(lt_buttons);

	StaticUtils::deleteLayout(lt_main);
}

void ProjectDetailsDialog::browseProjectPath()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Sélection du dossier du projet"), QString::fromStdString(o_projectInfos.getFolderPath()));

	if(dirPath != "")
	{
		o_projectInfos.setFolderPath(dirPath.toStdString());
		projectPathLineEdit->setText(dirPath);
	}
}

void ProjectDetailsDialog::setKeepLibrary(int state)
{ o_projectInfos.hasToKeepLibrary(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepLogs(int state)
{ o_projectInfos.hasToKeepLogs(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::setKeepObj(int state)
{ o_projectInfos.hasToKeepObj(state == Qt::CheckState::Checked); }

void ProjectDetailsDialog::validAndCloseDialog()
{
	this->close();
}

void ProjectDetailsDialog::resetAndCloseDialog()
{
	o_projectInfos.setFolderPath(a_projectInfos.getFolderPath());

	o_projectInfos.hasToKeepLibrary(a_projectInfos.isKeepingLibrary());
	o_projectInfos.hasToKeepLogs(a_projectInfos.isKeepingLogs());
	o_projectInfos.hasToKeepObj(a_projectInfos.isKeepingObj());

	this->close();
}
