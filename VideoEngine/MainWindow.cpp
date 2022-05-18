#include "ImageWidget.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    capture_thread.start();

	startTimer(1000 / 30);
}

MainWindow::~MainWindow()
{
    capture_thread.requestInterruption();
    capture_thread.wait();
	delete ui;
}

void MainWindow::timerEvent(QTimerEvent*)
{
    QImage image = capture_thread.GetImageFromV4LBuffer();
	if (!image.isNull()) {
		ui->widget->setImage(image);
	}
}
