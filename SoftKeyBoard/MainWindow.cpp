#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Dialog.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//QDialog示例
void MainWindow::on_btnOpen_clicked()
{
    Dialog d(this);
    d.exec();
}
//QFileDialog示例
void MainWindow::on_btnFileDialog_clicked()
{
    //不能直接用QFileDialog::getOpenFileName()
    QFileDialog dlg(this);
    dlg.setWindowModality(Qt::WindowModal);
    if(dlg.exec() == QDialog::Accepted){
        qDebug()<<__PRETTY_FUNCTION__<<dlg.selectedFiles();
    }
}
