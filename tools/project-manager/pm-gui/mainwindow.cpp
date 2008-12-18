#include <QFileDialog>
#include "mainwindow.h"

#include "../qmakeproject.h"
#include "../abstractproject.h"
#include "projectmodel.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    m_project = NULL;
    m_model = NULL;
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::on_deleteButton_clicked()
{
}

void MainWindow::on_addButton_clicked()
{
}

void MainWindow::on_loadButton_clicked()
{
	qDebug() << __FILE__ << __LINE__;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open project"),
	"../tests/", tr("Projects (*.pro *.pri *.prf)"));
    if (fileName.isEmpty())
        return;

    if (!m_project)
        m_project = new QMakeProject( this );

    m_project->loadProject( fileName );
    ui.projectLabel->setText( fileName );

    if (!m_model)
    {
        m_model = new ProjectModel( this, m_project );
    }
    ui.treeView->setModel( m_model );
}
