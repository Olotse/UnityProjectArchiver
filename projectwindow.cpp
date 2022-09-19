#include "projectwindow.h"

ProjectWindow::ProjectWindow(QWidget *parent) :
	QMainWindow(parent),
	projectCopyWorker(nullptr),
	projectCopyWorkerRunning(false)
{
	// Boutons pour ajouter et supprimer les projets de la liste
	addProjectButton = new QPushButton(tr("Ajouter un projet"));
	delProjectButton = new QPushButton(tr("Supprimer les projets sélectionnés"));

	// Layout des boutons
	lt_topBar = new QHBoxLayout();
	lt_topBar->addWidget(addProjectButton);
	lt_topBar->addWidget(delProjectButton);

	// Frame pour contenir les boutons
	topBarFrame = new QFrame();
	topBarFrame->setLayout(lt_topBar);


	// Liste avec les noms des colonnes
	QStringList hHeaderLabels;
	hHeaderLabels << tr("Nom du projet") << tr("Taille") << tr("Emplacement") << tr("Progression de la copie");

	// Modèle contenant les projets
	o_projectModel = new QStandardItemModel();
	o_projectModel->setHorizontalHeaderLabels(hHeaderLabels);

	// Tableau affichant les projets
	projectTableView = new QTableView();
	projectTableView->setModel(o_projectModel);
	projectTableView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	projectTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	projectTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	projectTableView->verticalHeader()->hide();
	projectTableView->setColumnWidth(0, 150);
	projectTableView->setColumnWidth(1, 80);
	projectTableView->setColumnWidth(2, 200);
	projectTableView->setColumnWidth(3, 150);


	// Champ et bouton pour sélectionner le répertoire de destination
	destinationPathLabel = new QLabel(tr("Répertoire de destination"));
	destinationPathLineEdit = new QLineEdit();
	destinationPathButton = new QPushButton(tr("Sélectionner"));

	lt_destinationPathForm = new QHBoxLayout();
	lt_destinationPathForm->addWidget(destinationPathLabel);
	lt_destinationPathForm->addWidget(destinationPathLineEdit);
	lt_destinationPathForm->addWidget(destinationPathButton);


	// Avertissement si le répertoire n'est pas vide
	destinationWarningLabel = new QLabel(tr("Ce répertoire n'est pas un répertoire vide, veuillez vous assurez qu'aucun projet ne sera écrasé par inadvertance"));
	destinationWarningLabel->setStyleSheet("color: red");
	destinationWarningLabel->hide();

	lt_destinationPathSet = new QVBoxLayout();
	lt_destinationPathSet->addLayout(lt_destinationPathForm);
	lt_destinationPathSet->addWidget(destinationWarningLabel);


	copyButton = new QPushButton(tr("Lancer la copie"));
	copyButton->setDisabled(true);

	quitButton = new QPushButton(tr("Quitter"));

	lt_footerBar = new QHBoxLayout();
	lt_footerBar->addWidget(copyButton);
	lt_footerBar->addWidget(quitButton);

	footerBarFrame = new QFrame();
	footerBarFrame->setLayout(lt_footerBar);


	lt_main = new QVBoxLayout();
	lt_main->addWidget(topBarFrame);
	lt_main->addWidget(projectTableView);
	lt_main->addLayout(lt_destinationPathSet);
	lt_main->addWidget(footerBarFrame);

	centralWidget = new QWidget();
	centralWidget->setLayout(lt_main);

	this->setCentralWidget(centralWidget);
	this->setMinimumWidth(600);

	connect(addProjectButton, &QPushButton::clicked, this, &ProjectWindow::getProjectByBrowsingInFiles);
	connect(delProjectButton, &QPushButton::clicked, this, &ProjectWindow::removeSelectedProjects);

	connect(projectTableView, &QAbstractItemView::doubleClicked, this, &ProjectWindow::showProjectDetails);
	connect(destinationPathButton, &QPushButton::clicked, this, &ProjectWindow::getDestinationByBrowsingInFiles);

	connect(copyButton, &QPushButton::clicked, this, &ProjectWindow::copyProjectsToDestination);
	connect(quitButton, &QPushButton::clicked, this, &QWidget::close);
}

void ProjectWindow::getProjectByBrowsingInFiles()
{
	// On récupère le chemin du répertoire
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("Ajouter un projet"));

	if(folderPath != "")
	{
		QtProjectInfos projectInfos(folderPath);

		// On l'ajoute à la liste des projets
		o_projectList.append(projectInfos);

		// On l'ajoute dans l'affichage des projets
		o_projectModel->appendRow(o_projectList.last().getStandardItems());
	}
}

void ProjectWindow::removeSelectedProjects()
{
	// On récupère les lignes sélectionnées
	QModelIndexList indexList = projectTableView->selectionModel()->selectedRows();

	// Si on a une ligne on effectue un tri des index et on les retire du modèle
	if(indexList.count() > 0)
	{
		// Trie des index en passant par une liste intermédiaire
		// (QModelIndex n'intègre pas l'opérateur > permettant le tri qui suit)
		QList<int> indexes = QList<int>(indexList.count());
		for(int i = 0; i < indexList.count(); i++)
			indexes[i] = indexList[i].row();

		// Tri du plus grand au plus petit
		std::sort(indexes.begin(), indexes.end(), std::greater<int>());

		// On retire chaque item correspondant à l'index dans le modèle
		foreach(int index, indexes)
		{
			o_projectList.remove(index);
			o_projectModel->removeRow(index);
		}
	}
}

void ProjectWindow::showProjectDetails(QModelIndex const& index)
{
	ProjectDetailsDialog dialog(o_projectList[index.row()], this);
	dialog.exec();

	o_projectModel->setItem(index.row(), 0, o_projectList[index.row()].getProjectNameItem());
	o_projectModel->setItem(index.row(), 1, o_projectList[index.row()].getProjectSizeItem());
	o_projectModel->setItem(index.row(), 2, o_projectList[index.row()].getProjectPathItem());
}

void ProjectWindow::getDestinationByBrowsingInFiles()
{
	// On récupère le chemin du répertoire
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("Sélectionner où copier les projets"));

	if(folderPath != "")
	{
		QtProjectInfos::setDestinationPath(folderPath.toStdString());
		destinationPathLineEdit->setText(folderPath);

		copyButton->setEnabled(true);

		QDir dir(folderPath);

		if(dir.entryList().count() > 2)
			destinationWarningLabel->show();
		else
			destinationWarningLabel->hide();
	}
}

void ProjectWindow::copyProjectsToDestination()
{
	if(projectCopyWorkerRunning)
		projectCopyWorker->stopCopyProcess();
	else
	{
		projectCopyWorker = new ProjectCopyWorker(o_projectList);

		connect(projectCopyWorker, &QThread::finished, this, &ProjectWindow::onCopyFinished);
		connect(projectCopyWorker, &QThread::finished, projectCopyWorker, &QObject::deleteLater);

		projectCopyWorker->start();
		projectCopyWorkerRunning = true;

		copyButton->setText(tr("Arrêter la copie"));

		quitButton->setDisabled(true);
		projectTableView->setDisabled(true);

		currentWindowFlags = windowFlags();

		setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
		show();
	}
}

void ProjectWindow::onCopyFinished()
{
	projectCopyWorkerRunning = false;

	copyButton->setText(tr("Lancer la copie"));

	quitButton->setEnabled(true);
	projectTableView->setEnabled(true);

	setWindowFlags(currentWindowFlags);
	show();
}

ProjectWindow::~ProjectWindow()
{
	StaticUtils::deleteLayout(lt_topBar);

	StaticUtils::deleteLayout(lt_destinationPathForm);
	StaticUtils::deleteLayout(lt_destinationPathSet);

	StaticUtils::deleteLayout(lt_footerBar);

	StaticUtils::deleteLayout(lt_main);
	delete centralWidget;

	StaticUtils::deleteItemModel(o_projectModel);

	if(projectCopyWorkerRunning)
		projectCopyWorker->quit();
}
