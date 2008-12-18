#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

class AbstractProject;
class QAbstractItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

private slots:
    void on_quitButton_clicked();
    void on_deleteButton_clicked();
    void on_addButton_clicked();
    void on_loadButton_clicked();

private:
    Ui::MainWindowClass ui;
    AbstractProject *m_project;
    QAbstractItemModel *m_model;

};

#endif // MAINWINDOW_H
