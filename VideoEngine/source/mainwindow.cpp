#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDebug>
#include <QString>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,  SLOT(updateView()));
    timer->start(30);

    ui->camViewer->setScene(&scene);

}

MainWindow::~MainWindow()
{
    capture_thread.requestInterruption();
    capture_thread.wait();
    delete ui;
}

void MainWindow::on_Start_pushButton_clicked()
{
        capture_thread.start();
}

void MainWindow::updateView()
{
    QImage image = capture_thread.GetImageFromV4LBuffer();
    if (!image.isNull()) {
        ui->camViewer->setImage(image);
    }
}
